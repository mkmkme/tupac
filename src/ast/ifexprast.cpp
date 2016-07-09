#include "ifexprast.hpp"

llvm::Value *CIfExprAST::codegen()
{
	llvm::Value* condv = m_Cond->codegen();
	if (!condv)
		return nullptr;

	// Converting condition to a bool by comparing with 0.
	condv = m_Builder.CreateFCmpONE(condv,
					     llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(0.)),
					     "ifcond");

	llvm::Function* func = m_Builder.GetInsertBlock()->getParent();

	// Create blocks for then and else. Insert them in the end of the func
	llvm::BasicBlock* thenbb = llvm::BasicBlock::Create(m_IR.Context(),
							    "then",
							    func);
	llvm::BasicBlock* elsebb = llvm::BasicBlock::Create(m_IR.Context(),
							    "else");
	llvm::BasicBlock* mergebb = llvm::BasicBlock::Create(m_IR.Context(),
							     "ifcont");

	m_Builder.CreateCondBr(condv, thenbb, elsebb);

	// Emit 'then' value
	m_Builder.SetInsertPoint(thenbb);

	llvm::Value* thenv = m_Then->codegen();
	if (!thenv)
		return nullptr;

	m_Builder.CreateBr(mergebb);
	// Codegen for 'then' can change the current block, update thenbb for the PHI
	thenbb = m_Builder.GetInsertBlock();

	// Emit 'else' block
	func->getBasicBlockList().push_back(elsebb);
	m_Builder.SetInsertPoint(elsebb);

	llvm::Value* elsev = m_Else->codegen();
	if (!elsev)
		return nullptr;

	m_Builder.CreateBr(mergebb);
	// codegen for 'else' can change the current block, update elsebb for the PHI
	elsebb = m_Builder.GetInsertBlock();

	// Emit merge block
	func->getBasicBlockList().push_back(mergebb);
	m_Builder.SetInsertPoint(mergebb);
	llvm::PHINode* phi = m_Builder.CreatePHI(llvm::Type::getDoubleTy(m_IR.Context()),
						      2,
						      "iftmp");

	phi->addIncoming(thenv, thenbb);
	phi->addIncoming(elsev, elsebb);

	return phi;
}
