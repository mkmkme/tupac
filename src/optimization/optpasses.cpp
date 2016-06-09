#include "optpasses.hpp"

#include "../codegen/kaleidoscopejit.hpp"

#include <llvm/Transforms/Scalar.h>

COptPasses::COptPasses(llvm::LLVMContext& c,
					   std::unique_ptr<llvm::Module>& m,
					   std::unique_ptr<FunctionPassManager>& f,
					   CKaleidoscopeJIT& jit) :
m_Context(c),
m_Module(m),
m_FPM(f)
{
	m_Module->setDataLayout(jit.TargetMachine().createDataLayout());

	// Create a new passmanager
	m_FPM = llvm::make_unique<FunctionPassManager>(m_Module.get());

	// Do simple "peephole" optimizations and bit-twiddling optzns.
	m_FPM->add(llvm::createInstructionCombiningPass());
	// Reassociate expressions
	m_FPM->add(llvm::createReassociatePass());
	// Eliminate Common SubExpressions
	m_FPM->add(llvm::createGVNPass());
	// Simplify the control flow graph (i.e. delete unreachable blocks)
	m_FPM->add(llvm::createCFGSimplificationPass());

	m_FPM->doInitialization();
}
