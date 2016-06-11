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

using llvm::legacy::FunctionPassManager;
class CIR
{
	llvm::LLVMContext m_Context;
	std::unique_ptr<llvm::Module> m_Module;
	llvm::IRBuilder<> m_Builder;
	std::map<std::string, llvm::Value*> m_NamedValues;

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

};

llvm::Value* LogErrorCodegen(const char *fmt, ...);
#endif
