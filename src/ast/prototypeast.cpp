#include "prototypeast.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

llvm::Function *CPrototypeAST::codegen()
{
	auto* doublety = llvm::Type::getDoubleTy(m_IR.Context());
	// Checking the function type (double(double, double))
	std::vector<llvm::Type*> doubles(m_Args.size(), doublety);

	auto* ft = llvm::FunctionType::get(doublety, doubles, false);

	auto* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
									 m_Name, m_IR.Module());

	// Set names for all args
	size_t idx = 0;
	for (auto& a : f->args())
		a.setName(m_Args[idx++]);

	return f;
}
