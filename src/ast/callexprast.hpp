#ifndef CALLEXPRAST_HPP
#define CALLEXPRAST_HPP

#include "exprast.hpp"

#include <memory>
#include <string>
#include <vector>

// Node class for function calls
class CCallExprAST : public CExprAST
{
	std::string m_Callee;
	std::vector<std::unique_ptr<CExprAST>> m_Args;

public:
	CCallExprAST(const std::string& c,
				 std::vector<std::unique_ptr<CExprAST>> a) :
	m_Callee(c),
	m_Args(std::move(a)) {}

	virtual llvm::Value *codegen() override;
};

#endif	// CALLEXPRAST_HPP
