#ifndef IR_HPP
#define IR_HPP

#include "kaleidoscopejit.hpp"

#include <cstdio>
#include <map>
#include <memory>
#include <string>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class CPrototypeAST;

using llvm::legacy::FunctionPassManager;
class CIR
{
	llvm::LLVMContext m_Context;
	std::unique_ptr<llvm::Module> m_Module;
	llvm::IRBuilder<> m_Builder;
	std::map<std::string, llvm::AllocaInst*> m_NamedValues;
	std::map<std::string, std::unique_ptr<CPrototypeAST>> m_FunctionPrototypes;

	std::unique_ptr<FunctionPassManager> m_FPM;
	std::unique_ptr<CKaleidoscopeJIT> m_JIT;

public:
	CIR();

	inline auto& NamedValues() { return m_NamedValues; }
	inline llvm::LLVMContext& Context() { return m_Context; }
	inline llvm::IRBuilder<>& Builder() { return m_Builder; }
	inline llvm::Module* Module() { return m_Module.get(); }
	inline std::unique_ptr<llvm::Module> MoveModule() { return std::move(m_Module); }
	inline FunctionPassManager& FPM() { return *m_FPM; }
	inline CKaleidoscopeJIT& JIT() { return *m_JIT; }
	void BuildPassManager();
	llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* f, const std::string& n);

	llvm::Function* GetFunction(const std::string& name);
	void AddFunction(std::unique_ptr<CPrototypeAST>&& proto);

};

llvm::Value* LogErrorCodegen(const char *fmt, ...);
#endif
