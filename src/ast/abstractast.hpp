#ifndef ABSTRACTAST_HPP
#define ABSTRACTAST_HPP

#include "../codegen/ir.hpp"

#include "../Globals.hpp"

class IAbstractAST
{
protected:

	CIR& m_IR;
public:
	IAbstractAST() : m_IR(CGlobals::IR()) {}
	virtual ~IAbstractAST() {}
};

#endif
