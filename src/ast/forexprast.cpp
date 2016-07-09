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
	// Make new basic block for the loop header, inserting after current
	// block
	llvm::Function* func = m_Builder.GetInsertBlock()->getParent();
	// Create an alloca for the var in entry block
	llvm::AllocaInst* alloca = m_IR.CreateEntryBlockAlloca(func, m_VarName);

	// Emit the 'start' code first, without 'var' in scope
	llvm::Value* startv = m_Start->codegen();
	if (!startv)
		return nullptr;

	// Store the var in alloca
	m_Builder.CreateStore(startv, alloca);

	// Make the new basic block for the loop header, inserting after current block
	llvm::BasicBlock* loopbb = llvm::BasicBlock::Create(m_IR.Context(),
							    "loop",
							    func);

	// Insert an explicit fall through from the current block to the loopbb
	m_Builder.CreateBr(loopbb);

	// Start insertion in loopbb
	m_Builder.SetInsertPoint(loopbb);

	// Within the loop, the var is defined equal to the PHI node.
	// If it shadows an existing var, we need to restore it, so save it now
	llvm::AllocaInst* oldv = m_IR.NamedValues()[m_VarName];
	m_IR.NamedValues()[m_VarName] = alloca;

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

	// Compute the end condition
	llvm::Value* endv = m_End->codegen();
	if (!endv)
		return nullptr;

	// Reload, increment and restore the alloca. This handles the case where
	// the body of the loop mutates the var
	llvm::Value* curv = m_Builder.CreateLoad(alloca, m_VarName.c_str());
	llvm::Value* nextv = m_Builder.CreateFAdd(curv, stepv, "nextvar");
	m_Builder.CreateStore(nextv, alloca);

	// Convert condition to bool using double
	endv = m_Builder.CreateFCmpONE(endv,
					    llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(0.)),
					    "loopcond");

	// Create the "after loop" block and insert it
	llvm::BasicBlock* afterbb = llvm::BasicBlock::Create(m_IR.Context(),
							     "afterloop",
							     func);

	// Insert the conditional branch into the end of loopbb
	m_Builder.CreateCondBr(endv, loopbb, afterbb);

	// Any new code will be inserted in afterbb
	m_Builder.SetInsertPoint(afterbb);

	// Restore the unshadowed variable
	if (oldv)
		m_IR.NamedValues()[m_VarName] = oldv;
	else
		m_IR.NamedValues().erase(m_VarName);

	// 'for' exprs always return 0.
	return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(m_IR.Context()));
}
