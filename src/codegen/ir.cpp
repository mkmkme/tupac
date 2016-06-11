#include "ir.hpp"


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
m_Module(llvm::make_unique<llvm::Module>("my hot jit", m_Context)),
m_Builder(m_Context),
m_FPM(std::make_unique<FunctionPassManager>(m_Module.get())),
m_JIT(nullptr),
m_Passes(nullptr)
{
	m_JIT = std::make_unique<CKaleidoscopeJIT>();
	m_Passes = std::make_unique<COptPasses>(m_Context, *m_Module, *m_FPM, *m_JIT);
}
