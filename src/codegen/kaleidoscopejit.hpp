#ifndef KALEIDOSCOPEJIT_HPP
#define KALEIDOSCOPEJIT_HPP

#include <memory>
#include <string>

#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>

class CKaleidoscopeJIT
{
	// Target Machine
	std::unique_ptr<llvm::TargetMachine> m_TM;
	// Data Layout
	const llvm::DataLayout m_DL;
	// Object Layer
	llvm::orc::ObjectLinkingLayer<> m_OL;
	// Compile Layer
	llvm::orc::IRCompileLayer<decltype(m_OL)> m_CL;
public:
	typedef decltype(m_CL)::ModuleSetHandleT ModuleHandle_t;

	CKaleidoscopeJIT();

	inline llvm::TargetMachine& TargetMachine() { return *m_TM; }

	llvm::Module& AddModule(llvm::Module&& m);
	void RemoveModule(llvm::Module&& m);
	llvm::Module* FindSymbol(const std::string& s);
};

#endif
