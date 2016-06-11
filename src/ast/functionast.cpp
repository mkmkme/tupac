#include "functionast.hpp"

#include <llvm/IR/Verifier.h>

llvm::Function *CFunctionAST::codegen()
{
	// Check for an existing function from a previous 'extern' declaration
	auto* f = m_IR.Module()->getFunction(m_Proto->Name());

	if (!f)
		f = m_Proto->codegen();

	if (!f)
		return nullptr;

	if (!f->empty())
		return (llvm::Function*)LogErrorCodegen("Function cannot be redefined");

	// Create a new basic block to start insertion into
	auto* bb = llvm::BasicBlock::Create(m_IR.Context(), "entry", f);
	m_IR.Builder().SetInsertPoint(bb);

	// Record the function arguments in the map
	m_IR.NamedValues().clear();
	for (auto& a : f->args())
		m_IR.NamedValues()[a.getName()] = &a;

	auto* r = m_Body->codegen();
	if (r) {
		// Finish off the function
		m_IR.Builder().CreateRet(r);

		// Validate the generated code, checking for consistency
		llvm::verifyFunction(*f);

		// Optimize function
		m_IR.FPM().run(*f);

		return f;
	}

	// Error reading body, remove function
	f->eraseFromParent();
	return nullptr;
}
