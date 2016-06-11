#include "ir.hpp"

#include <llvm/Transforms/Scalar.h>

llvm::Value* LogErrorCodegen(const char *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	vfprintf(stderr, fmt, v);
	va_end(v);
	fprintf(stderr, "\n");
	return nullptr;
}

CIR::CIR() :
m_Module(std::make_unique<llvm::Module>("my hot jit", m_Context)),
m_Builder(m_Context),
m_FPM(std::make_unique<FunctionPassManager>(m_Module.get())),
m_JIT(nullptr)
{
	m_JIT = std::make_unique<CKaleidoscopeJIT>();
}

void CIR::BuildPassManager()
{
	// Open a new module
	m_Module = std::make_unique<llvm::Module>("my hot jit", m_Context);
	m_Module->setDataLayout(m_JIT->TargetMachine().createDataLayout());

	// Create a new pass manager attached to module
	m_FPM = std::make_unique<FunctionPassManager>(m_Module.get());

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

