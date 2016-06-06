#ifndef EXPRAST_HPP
#define EXPRAST_HPP

#include "abstractast.hpp"

#include <llvm/IR/Value.h>

// Basic class for all exp nodes in AST
class CExprAST : public IAbstractAST
{
public:
	virtual ~CExprAST() {}
	virtual llvm::Value* codegen() = 0;
};

#endif
