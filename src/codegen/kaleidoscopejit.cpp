#include "kaleidoscopejit.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/Support/DynamicLibrary.h>

CKaleidoscopeJIT::CKaleidoscopeJIT() :
m_TM(llvm::EngineBuilder().selectTarget()),
m_DL(m_TM->createDataLayout()),
m_CL(m_OL, llvm::orc::SimpleCompiler(*m_TM))
{
	llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}
