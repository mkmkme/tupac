#ifndef VARIABLEEXPRAST_HPP
#define VARIABLEEXPRAST_HPP

#include "exprast.hpp"

#include <string>

// Node class for variables
class CVariableExprAST : public CExprAST
{
	std::string m_Name;
public:
	CVariableExprAST(const std::string& s) : m_Name(s) {}
	inline const std::string& Name() const { return m_Name; }
	virtual llvm::Value *codegen() override;
};

#endif
