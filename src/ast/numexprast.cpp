#include "numexprast.hpp"

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constants.h>

llvm::Value *CNumberExprAST::codegen()
{
	return llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(m_Value));
}
