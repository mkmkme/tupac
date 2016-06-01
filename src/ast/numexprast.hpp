#ifndef NUMEXPRAST_HPP
#define NUMEXPRAST_HPP

#include "exprast.hpp"

// Node class for numeric literals
class CNumberExprAST : public CExprAST
{
	double m_Value;
public:
	CNumberExprAST(double v) : m_Value(v) {}
};

#endif
