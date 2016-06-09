#include "optpasses.hpp"

#include "../codegen/kaleidoscopejit.hpp"


COptPasses::COptPasses(llvm::LLVMContext& c,
					   llvm::Module* m,
					   FunctionPassManager* f,
					   CKaleidoscopeJIT& jit) :
m_Context(c),
m_Module(m),
m_FPM(f)
{
	m_Module->setDataLayout(jit.TargetMachine().createDataLayout());
}
