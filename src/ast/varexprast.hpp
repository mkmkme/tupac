#ifndef VAREXPRAST_HPP
#define VAREXPRAST_HPP

#include "exprast.hpp"

#include <memory>

class CVarExprAST : public CExprAST
{
	typedef std::vector<std::pair<std::string, std::unique_ptr<CExprAST>>> VarNamesVector;
	VarNamesVector m_VarNames;
	std::unique_ptr<CExprAST> m_Body;
public:
	CVarExprAST(VarNamesVector varnames,
		    std::unique_ptr<CExprAST> body) :
	m_VarNames(std::move(varnames)),
	m_Body(std::move(body))
	{}

	virtual llvm::Value* codegen() override;
};

#endif
