#ifndef UNARYEXPRAST_HPP
#define UNARYEXPRAST_HPP

#include "exprast.hpp"

#include <memory>

class CUnaryExprAST : public CExprAST
{
	char m_OpCode;
	std::unique_ptr<CExprAST> m_Operand;
public:
	CUnaryExprAST(char opcode, std::unique_ptr<CExprAST> operand) :
	m_OpCode(opcode),
	m_Operand(std::move(operand))
	{}

	virtual llvm::Value* codegen() override;
};

#endif
