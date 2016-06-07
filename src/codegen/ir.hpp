#ifndef IR_HPP
#define IR_HPP

#include <cstdio>
#include <map>
#include <memory>
#include <string>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class CIR
{
	llvm::LLVMContext m_Context;
	std::unique_ptr<llvm::Module> m_Module;
	llvm::IRBuilder<> m_Builder;
	std::map<std::string, llvm::Value*> m_NamedValues;

public:
	CIR() :
	m_Module(llvm::make_unique<llvm::Module>("my hot jit", m_Context)),
	m_Builder(m_Context) {}

	inline std::map<std::string, llvm::Value*>& NamedValues()
	{
		return m_NamedValues;
	}

	inline llvm::LLVMContext& Context()
	{
		return m_Context;
	}

	inline llvm::IRBuilder<>& Builder()
	{
		return m_Builder;
	}

	inline std::unique_ptr<llvm::Module>& Module()
	{
		return m_Module;
	}

};

llvm::Value* LogErrorCodegen(const char *fmt, ...);
#endif
