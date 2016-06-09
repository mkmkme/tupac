#include "optpasses.hpp"


COptPasses::COptPasses(llvm::LLVMContext& c,
					   llvm::Module* m,
					   FunctionPassManager* f) :
m_Context(c),
m_Module(m),
m_FPM(f)
{
	m_Module->setDataLayout();
}
