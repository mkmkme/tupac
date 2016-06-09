#include "kaleidoscopejit.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>

CKaleidoscopeJIT::CKaleidoscopeJIT() :
m_TM(llvm::EngineBuilder().selectTarget()),
m_DL(m_TM->createDataLayout()),
m_CL(m_OL, SimpleCompiler(*m_TM))
{
	llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}
