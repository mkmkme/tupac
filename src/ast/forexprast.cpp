#include "forexprast.hpp"

// Output for-loop as:
//   ...
//   start = startexpr
//   goto loop
// loop:
//   variable = phi [start, loopheader], [nextvariable, loopend]
//   ...
//   bodyexpr
//   ...
// loopend:
//   step = stepexpr
//   nextvariable = variable + step
//   endcond = endexpr
//   br endcond, loop, endloop
// outloop:

llvm::Value *CForExprAST::codegen()
{
	llvm::Value* startv = m_Start->codegen();
	if (!startv)
		return nullptr;

	// Make new basic block for the loop header, inserting after current
	// block
	llvm::Function* func = m_IR.Builder().GetInsertBlock()->getParent();
	llvm::BasicBlock* preheaderbb = m_IR.Builder().GetInsertBlock();
	llvm::BasicBlock* loopbb = llvm::BasicBlock::Create(m_IR.Context(),
							    "loop",
							    func);

	// Insert an explicit fall through from the current block to the loopbb
	m_IR.Builder().CreateBr(loopbb);

	// Start insertion in loopbb
	m_IR.Builder().SetInsertPoint(loopbb);

	// Start the PHI node with an entry for start
	llvm::PHINode* var = m_IR.Builder().CreatePHI(llvm::Type::getDoubleTy(m_IR.Context()),
						      2,
						      m_VarName.c_str());
	var->addIncoming(startv, preheaderbb);

	// Within the loop, the var is defined equal to the PHI node.
	// If it shadows an existing var, we need to restore it, so save it now
	llvm::Value* oldv = m_IR.NamedValues()[m_VarName];
	m_IR.NamedValues()[m_VarName] = var;

	// Emit the body of the loop. This, like any other expr, can change
	// the current bb. Note that we ignore the value computed by the body,
	// but don't allow the error
	if (!m_Body->codegen())
		return nullptr;

	// Emit the step value
	llvm::Value* stepv = nullptr;
	if (m_Step) {
		stepv = m_Step->codegen();
		if (!stepv)
			return nullptr;
	} else {
		// If not specified, use 1.
		stepv = llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(1.));
	}

	llvm::Value* nextv = m_IR.Builder().CreateFAdd(var, stepv, "nextvar");

	// Compute the end condition
	llvm::Value* endv = m_End->codegen();
	if (!endv)
		return nullptr;

	// Convert condition to bool using double
	endv = m_IR.Builder().CreateFCmpONE(endv,
					    llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(0.)),
					    "loopcond");

	// Create the "after loop" block and insert it
	llvm::BasicBlock* loopendbb = m_IR.Builder().GetInsertBlock();
	llvm::BasicBlock* afterbb = llvm::BasicBlock::Create(m_IR.Context(),
							     "afterloop",
							     func);

	// Insert the conditional branch into the end of loopbb
	m_IR.Builder().CreateCondBr(endv, loopbb, afterbb);

	// Any new code will be inserted in afterbb
	m_IR.Builder().SetInsertPoint(afterbb);

	// Add a new entry to the PHI node for the backedge
	var->addIncoming(nextv, loopendbb);

	// Restore the unshadowed variable
	if (oldv)
		m_IR.NamedValues()[m_VarName] = oldv;
	else
		m_IR.NamedValues().erase(m_VarName);

	// 'for' exprs always return 0.
	return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(m_IR.Context()));
}
