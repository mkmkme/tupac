#include "kaleidoscopejit.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITSymbolFlags.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/RTDyldMemoryManager.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>

CKaleidoscopeJIT::CKaleidoscopeJIT() :
m_TM(nullptr),
m_DL(nullptr),
m_OL(nullptr),
m_CL(nullptr)
{
	llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}

void CKaleidoscopeJIT::Init()
{
	m_TM = llvm::EngineBuilder().selectTarget();
	assert(m_TM != nullptr);
	m_DL = std::make_unique<llvm::DataLayout>(m_TM->createDataLayout());
	m_OL = std::make_unique<ObjLayer_t>();
	m_CL = std::make_unique<CompileLayer_t>(*m_OL, llvm::orc::SimpleCompiler(*m_TM));
}

CKaleidoscopeJIT::ModuleHandle_t CKaleidoscopeJIT::AddModule(std::unique_ptr<llvm::Module> m)
{
	// Use a memory manager to allocate memory and resolve symbols for new module.
	// Create one to resolve symbols by looking back into JIT
	auto resolver = llvm::orc::createLambdaResolver(
		[&](const std::string& n)
		{
			auto s = FindMangledSymbol(n);
			if (s)
				return llvm::RuntimeDyld::SymbolInfo(s.getAddress(), s.getFlags());
			return llvm::RuntimeDyld::SymbolInfo(nullptr);

		},
		[](const std::string& dummy) { return nullptr; }
	);

	auto h = m_CL->addModuleSet(SingletonSet(std::move(m)),
				    std::make_unique<llvm::SectionMemoryManager>(),
				    std::move(resolver));

	m_ModuleHandles.push_back(h);
	return h;
}

void CKaleidoscopeJIT::RemoveModule(CKaleidoscopeJIT::ModuleHandle_t h)
{
	m_ModuleHandles.erase(std::find(m_ModuleHandles.begin(), m_ModuleHandles.end(), h));
	m_CL->removeModuleSet(h);
}

llvm::orc::JITSymbol CKaleidoscopeJIT::FindSymbol(const std::string &s)
{
	return FindMangledSymbol(Mangle(s));
}

std::string CKaleidoscopeJIT::Mangle(std::string name)
{
	std::string mangled;
	llvm::raw_string_ostream mns(mangled);
	llvm::Mangler::getNameWithPrefix(mns, name, *m_DL);
	return mangled;
}

llvm::orc::JITSymbol CKaleidoscopeJIT::FindMangledSymbol(const std::string &name)
{
	// Searching modules in reverse order
	// It makes more sense in REPL
	for (auto h : llvm::make_range(m_ModuleHandles.rbegin(), m_ModuleHandles.rend())) {
		auto s = m_CL->findSymbolIn(h, name, true);
		if (s)
			return s;
	}

	// If we can't find symbol in JIT, try looking in the host process
	auto symAddr = llvm::RTDyldMemoryManager::getSymbolAddressInProcess(name);
	if (symAddr)
		return llvm::orc::JITSymbol(symAddr, llvm::JITSymbolFlags::Exported);

	return nullptr;
}

template <typename T>
std::vector<T> CKaleidoscopeJIT::SingletonSet(T t)
{
	std::vector<T> v;
	v.push_back(std::move(t));
	return v;
}
