#ifndef FUNCTIONAST_HPP
#define FUNCTIONAST_HPP

#include "exprast.hpp"
#include "prototypeast.hpp"

#include <memory>

// Node class for function definition
class CFunctionAST
{
	std::unique_ptr<CPrototypeAST> m_Proto;
	std::unique_ptr<CExprAST> m_Body;

public:
	CFunctionAST(std::unique_ptr<CPrototypeAST> p,
				 std::unique_ptr<CExprAST> b) :
	m_Proto(std::move(p)),
	m_Body(std::move(b)) {}
};

#endif	// FUNCTIONAST_HPP
