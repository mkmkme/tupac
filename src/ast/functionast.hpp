#ifndef FUNCTIONAST_HPP
#define FUNCTIONAST_HPP

#include "abstractast.hpp"
#include "exprast.hpp"
#include "prototypeast.hpp"

#include <memory>

// Node class for function definition
class CFunctionAST : public IAbstractAST
{
	std::unique_ptr<CPrototypeAST> m_Proto;
	std::unique_ptr<CExprAST> m_Body;

public:
	CFunctionAST(std::unique_ptr<CPrototypeAST> p,
				 std::unique_ptr<CExprAST> b) :
	m_Proto(std::move(p)),
	m_Body(std::move(b)) {}

	virtual llvm::Function *codegen();
};

#endif	// FUNCTIONAST_HPP
