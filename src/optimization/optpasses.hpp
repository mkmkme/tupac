#ifndef OPTPASSES_HPP
#define OPTPASSES_HPP

#include <memory>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>

#include "../codegen/kaleidoscopejit.hpp"

class CKaleidoscopeJIT;

using llvm::legacy::FunctionPassManager;
class COptPasses
{
	llvm::LLVMContext& m_Context;
	llvm::Module& m_Module;
	FunctionPassManager& m_FPM;

public:
	COptPasses(llvm::LLVMContext& c,
			   llvm::Module& m,
			   FunctionPassManager& f,
			   CKaleidoscopeJIT& jit);

	inline FunctionPassManager& FPM() { return m_FPM; }
};

#endif
