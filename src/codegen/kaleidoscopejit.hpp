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
	typedef llvm::orc::ObjectLinkingLayer<> ObjLayer_t;
	typedef llvm::orc::IRCompileLayer<ObjLayer_t> CompileLayer_t;
	typedef CompileLayer_t::ModuleSetHandleT ModuleHandle_t;

	// Target Machine
	llvm::TargetMachine* m_TM;
	// Data Layout
	std::unique_ptr<llvm::DataLayout> m_DL;
	// Object Layer
	std::unique_ptr<ObjLayer_t> m_OL;
	// Compile Layer
	std::unique_ptr<CompileLayer_t> m_CL;
	// Module Handles
	std::vector<ModuleHandle_t> m_ModuleHandles;
public:
	CKaleidoscopeJIT();
	void Init();

	inline llvm::TargetMachine& TargetMachine() { return *m_TM; }

	ModuleHandle_t AddModule(std::unique_ptr<llvm::Module> m);
	void RemoveModule(ModuleHandle_t h);
	llvm::orc::JITSymbol FindSymbol(const std::string& s);
private:
	std::string Mangle(std::string name);
	template <typename T> static std::vector<T> SingletonSet(T t);
	llvm::orc::JITSymbol FindMangledSymbol(const std::string& name);
};

#endif
