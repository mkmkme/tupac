#include "binexprast.hpp"

llvm::Value* CBinaryExprAST::codegen()
{
	llvm::Value* l = m_LHS->codegen();
	llvm::Value* r = m_RHS->codegen();
	if (!l || !r)
		return nullptr;

	switch (m_Operation) {
	case '+':
		return m_IR.Builder().CreateFAdd(l, r, "addtmp");
	case '-':
		return m_IR.Builder().CreateFSub(l, r, "subtmp");
	case '*':
		return m_IR.Builder().CreateFMul(l, r, "multmp");
	case '<':
		l = m_IR.Builder().CreateFCmpULT(l, r, "cmptmp");
		return m_IR.Builder().CreateUIToFP(l, llvm::Type::getDoubleTy(m_IR.Context()),
									  "booltmp");
	default:
		return LogErrorCodegen("Invalid binary operator");
	}
}

