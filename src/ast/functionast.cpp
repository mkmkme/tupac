#include "functionast.hpp"
#include "../parser/parser.hpp"

#include <llvm/IR/Verifier.h>

llvm::Function *CFunctionAST::codegen()
{
	// Transfer ownership of the prototype to the FunctionProtos map, but keep a
	// reference to it for use below.
	auto &p = *m_Proto;
	m_IR.AddFunction(std::move(m_Proto));
	llvm::Function* f = m_IR.GetFunction(p.Name());
	if (!f)
		return nullptr;

	// If this is an operator, install it
	if (p.IsBinaryOp())
		CParser::BinOpPrecedenceMap()[p.GetOperatorName()] = p.GetBinaryPrecedence();

	// Create a new basic block to start insertion into
	auto* bb = llvm::BasicBlock::Create(m_IR.Context(), "entry", f);
	m_IR.Builder().SetInsertPoint(bb);

	// Record the function arguments in the map
	m_IR.NamedValues().clear();
	for (auto& a : f->args()) {
		llvm::AllocaInst* alloca = m_IR.CreateEntryBlockAlloca(f, a.getName());

		// Store the initial value into the alloca
		m_IR.Builder().CreateStore(&a, alloca);

		// Add arguments to variable symbol table
		m_IR.NamedValues()[a.getName()] = alloca;
	}

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
