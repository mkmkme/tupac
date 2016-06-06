#include "callexprast.hpp"

llvm::Value* CCallExprAST::codegen()
{
	// Look up the name in the global module table
	llvm::Function* callee = m_IR.Module()->getFunction(m_Callee);
	if (!callee)
		return LogErrorCodegen("Unrecognized function referenced");

	// Check for argument mismatch
	if (callee->arg_size() != m_Args.size())
		return LogErrorCodegen("Function '%s' takes %u args (%u given)",
							   m_Callee.c_str(), m_Args.size(), callee->arg_size());

	std::vector<llvm::Value*> args;
	for (auto& a : m_Args) {
		args.push_back(a->codegen());
		if (!args.back())
			return nullptr;
	}

	return m_IR.Builder().CreateCall(callee, args, "calltmp");
}
