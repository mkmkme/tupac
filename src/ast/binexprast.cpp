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
		break;
	}

	// If it wasn't a builtin binop, it can be user-defined. Emit a call for it then
	llvm::Function* f = m_IR.GetFunction(std::string("binary") + m_Operation);

	if (!f)
		return LogErrorCodegen("Binary operator '%c' not found\n", m_Operation);

	llvm::Value* ops[2] = { l, r };
	return m_IR.Builder().CreateCall(f, ops, "binop");
}

