#ifndef VAREXPRAST_HPP
#define VAREXPRAST_HPP

#include "exprast.hpp"

#include <string>

// Node class for variables
class CVariableExprAST : public CExprAST
{
	std::string m_Name;
public:
	CVariableExprAST(const std::string& s) : m_Name(s) {}
};

#endif
