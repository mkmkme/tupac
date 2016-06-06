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

public:
	CPrototypeAST(const std::string& n, std::vector<std::string> a) :
	m_Name(n),
	m_Args(std::move(a)) {}

	inline const std::string& Name() const { return m_Name; }

	virtual llvm::Function *codegen();
};

#endif
