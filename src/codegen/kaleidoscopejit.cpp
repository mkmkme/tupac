#include "kaleidoscopejit.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/Support/DynamicLibrary.h>

CKaleidoscopeJIT::CKaleidoscopeJIT() :
m_TM(nullptr),
m_DL(nullptr),
m_OL(nullptr),
m_CL(nullptr)
{
	llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}

void CKaleidoscopeJIT::Init()
{
	m_TM = llvm::EngineBuilder().selectTarget();
	assert(m_TM != nullptr);
	m_DL = std::make_unique<llvm::DataLayout>(m_TM->createDataLayout());
	m_OL = std::make_unique<ObjLayer_t>();
	m_CL = std::make_unique<CompileLayer_t>(*m_OL, llvm::orc::SimpleCompiler(*m_TM));
}
