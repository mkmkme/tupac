#ifndef PROTOTYPEAST_HPP
#define PROTOTYPEAST_HPP

#include "abstractast.hpp"

#include <string>
#include <vector>

// Node class for function prototype
class CPrototypeAST : public IAbstractAST
{
	// Function name
	std::string m_Name;
	// Function args' names
	std::vector<std::string> m_Args;
	// Flag if this prototype is an operator (binary or unary)
	bool m_IsOperator;
	// Precedence of the operator
	unsigned m_Precedence;

public:
	CPrototypeAST(const std::string& n,
		      std::vector<std::string> a,
		      bool isop = false,
		      unsigned precedence = 0) :
	m_Name(n),
	m_Args(std::move(a)),
	m_IsOperator(isop),
	m_Precedence(precedence)
	{}

	inline const std::string& Name() const { return m_Name; }
	inline bool IsUnaryOp() const { return m_IsOperator && m_Args.size() == 1; }
	inline bool IsBinaryOp() const { return m_IsOperator && m_Args.size() == 2; }
	inline unsigned GetBinaryPrecedence() const { return m_Precedence; }
	inline char GetOperatorName() const
	{
		if (!IsUnaryOp() && !IsBinaryOp())
			return 0;
		return m_Name[m_Name.size() - 1];
	}

	virtual llvm::Function *codegen();
};

#endif
