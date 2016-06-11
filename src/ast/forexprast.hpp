#ifndef FOREXPRAST_HPP
#define FOREXPRAST_HPP

#include "exprast.hpp"

#include <memory>
#include <string>

class CForExprAST : public CExprAST
{
	std::string m_VarName;
	std::unique_ptr<CExprAST> m_Start;
	std::unique_ptr<CExprAST> m_End;
	std::unique_ptr<CExprAST> m_Step;
	std::unique_ptr<CExprAST> m_Body;
public:
	CForExprAST(const std::string& varname,
		    std::unique_ptr<CExprAST> start,
		    std::unique_ptr<CExprAST> end,
		    std::unique_ptr<CExprAST> step,
		    std::unique_ptr<CExprAST> body) :
	m_VarName(varname),
	m_Start(std::move(start)),
	m_End(std::move(end)),
	m_Step(std::move(step)),
	m_Body(std::move(body))
	{}

	virtual llvm::Value* codegen() override;
};

#endif
