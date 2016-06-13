#include "varexprast.hpp"


llvm::Value *CVarExprAST::codegen()
{
	std::vector<llvm::AllocaInst*> oldbindings;

    llvm::Function* func = m_Builder.GetInsertBlock()->getParent();

	// Register all variables and emit their initializers
	for (const auto& varpair : m_VarNames) {
		const std::string& varname = varpair.first;
		CExprAST* init = varpair.second.get();

		// Emit the initializer before adding the var to scope, this
		// prevents the initializer from referencing the var itself and
		// permits stuff like this:
		//	var a = 1 in
		//		var a = a in ... # refers to outer 'a'
		llvm::Value* initv;
		if (init) {
			initv = init->codegen();
			if (!initv)
				return nullptr;
		} else {
			initv = llvm::ConstantFP::get(m_IR.Context(), llvm::APFloat(1.));
		}

		llvm::AllocaInst* alloca = m_IR.CreateEntryBlockAlloca(func, varname);
        m_Builder.CreateStore(initv, alloca);

		// Remember the old var binding so that we can restore the
		// binding when unrecurse
		oldbindings.push_back(m_IR.NamedValues()[varname]);

		// Remember this binding
		m_IR.NamedValues()[varname] = alloca;
	}

	// Codegen the body, now that all vars are in scope
	llvm::Value* bodyv = m_Body->codegen();
	if (!bodyv)
		return nullptr;

	// Pop all our variables from scope
	for (size_t i = 0, e = m_VarNames.size(); i != e; i++)
		m_IR.NamedValues()[m_VarNames[i].first] = oldbindings[i];

	// Return the body computation
	return bodyv;
}
