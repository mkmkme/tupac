#include "callexprast.hpp"

llvm::Value* CCallExprAST::codegen()
{
	// Look up the name in the global module table
	llvm::Function* callee = m_IR.Module()->getFunction(m_Callee);
	if (!callee)
		return LogErrorCodegen("Unrecognized function referenced");

	// Check for argument mismatch
	if (callee->arg_size() != m_Args.size()) {
		size_t argsize = callee->arg_size();
		const char *argstring = argsize == 1 ? "arg" : "args";
		return LogErrorCodegen("Function '%s' takes %u %s (%u given)",
							   m_Callee.c_str(), argsize,
							   argstring, m_Args.size());
	}

	std::vector<llvm::Value*> args;
	for (auto& a : m_Args) {
		args.push_back(a->codegen());
		if (!args.back())
			return nullptr;
	}


	return m_IR.Builder().CreateCall(callee, args, "calltmp");
}
