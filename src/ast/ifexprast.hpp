#ifndef IFEXPRAST_HPP
#define IFEXPRAST_HPP

#include "exprast.hpp"

#include <memory>

// Expression class for if/then/else
class CIfExprAST : public CExprAST
{
	std::unique_ptr<CExprAST> m_Cond;
	std::unique_ptr<CExprAST> m_Then;
	std::unique_ptr<CExprAST> m_Else;
public:
	CIfExprAST(std::unique_ptr<CExprAST> _cond,
		   std::unique_ptr<CExprAST> _then,
		   std::unique_ptr<CExprAST> _else) :
	m_Cond(std::move(_cond)),
	m_Then(std::move(_then)),
	m_Else(std::move(_else))
	{}

	virtual llvm::Value* codegen() override;
};

#endif
