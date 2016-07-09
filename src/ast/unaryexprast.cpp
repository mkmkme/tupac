#include "unaryexprast.hpp"


llvm::Value *CUnaryExprAST::codegen()
{
	llvm::Value* operandv = m_Operand->codegen();
	if (!operandv)
		return nullptr;

	llvm::Function* f = m_IR.GetFunction(std::string("unary") + m_OpCode);
	if (!f)
		return LogErrorCodegen("Unknown unary operator '%c'\n", m_OpCode);

	return m_Builder.CreateCall(f, operandv, "unop");
}
