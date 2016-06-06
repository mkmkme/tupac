#ifndef BINEXPRAST_HPP
#define BINEXPRAST_HPP

#include "exprast.hpp"

#include <memory>

// Node class for binary operator
class CBinaryExprAST : public CExprAST
{
	char m_Operation;
	std::unique_ptr<CExprAST> m_LHS;
	std::unique_ptr<CExprAST> m_RHS;
public:
	CBinaryExprAST(char op,
				   std::unique_ptr<CExprAST> l,
				   std::unique_ptr<CExprAST> r) :
	m_Operation(op),
	m_LHS(std::move(l)),
	m_RHS(std::move(r)) {}

	virtual llvm::Value *codegen() override;
};

#endif	// BINEXPRAST_HPP
