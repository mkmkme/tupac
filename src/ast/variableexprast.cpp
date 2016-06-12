#include "variableexprast.hpp"

#include <cstdio>

llvm::Value *CVariableExprAST::codegen()
{
	// Look this var up in the function
	llvm::Value* v = m_IR.NamedValues()[m_Name];
	if (!v)
			return LogErrorCodegen("Unrecognized variable name: %s", m_Name.c_str());

	// Load the value
	return m_IR.Builder().CreateLoad(v, m_Name.c_str());
}
