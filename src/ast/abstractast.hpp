#ifndef ABSTRACTAST_HPP
#define ABSTRACTAST_HPP

#include "../codegen/ir.hpp"

#include "../Globals.hpp"

class IAbstractAST
{
protected:

	CIR& m_IR;
	llvm::IRBuilder<>& m_Builder;
public:
	IAbstractAST() :
	m_IR(CGlobals::IR()),
	m_Builder(m_IR.Builder())
	{}
	virtual ~IAbstractAST() {}
};

#endif
