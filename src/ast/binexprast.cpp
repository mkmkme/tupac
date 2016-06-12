#include "binexprast.hpp"

#include "variableexprast.hpp"

llvm::Value* CBinaryExprAST::codegen()
{
	// Special case '=' because we don't want to emit lhs as an expr
	if (m_Operation == '=') {
		// Assignment requires the lhs to be an id. This assume we are
		// building without RTTI because LLVM build that way by default.
		CVariableExprAST* lhse = static_cast<CVariableExprAST*>(m_LHS.get());
		if (!lhse)
			return LogErrorCodegen("Destination of '=' must be a var");
		// Codegen the rhs
		llvm::Value* val = m_RHS->codegen();
		if (!val)
			return nullptr;

		// Look up the name
		llvm::Value* var = m_IR.NamedValues()[lhse->Name()];
		if (!var)
			return LogErrorCodegen("Unknown variable name '%s'\n", lhse->Name());

		m_IR.Builder().CreateStore(val, var);

		return val;
	}
	llvm::Value* l = m_LHS->codegen();
	llvm::Value* r = m_RHS->codegen();
	if (!l || !r)
		return nullptr;

	switch (m_Operation) {
	case '+':
		return m_IR.Builder().CreateFAdd(l, r, "addtmp");
	case '-':
		return m_IR.Builder().CreateFSub(l, r, "subtmp");
	case '*':
		return m_IR.Builder().CreateFMul(l, r, "multmp");
	case '<':
		l = m_IR.Builder().CreateFCmpULT(l, r, "cmptmp");
		return m_IR.Builder().CreateUIToFP(l, llvm::Type::getDoubleTy(m_IR.Context()),
									  "booltmp");
	default:
		break;
	}

	// If it wasn't a builtin binop, it can be user-defined. Emit a call for it then
	llvm::Function* f = m_IR.GetFunction(std::string("binary") + m_Operation);

	if (!f)
		return LogErrorCodegen("Binary operator '%c' not found\n", m_Operation);

	llvm::Value* ops[2] = { l, r };
	return m_IR.Builder().CreateCall(f, ops, "binop");
}

