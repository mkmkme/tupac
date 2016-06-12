#include "ir.hpp"

#include <llvm/Transforms/Scalar.h>
#include "../ast/prototypeast.hpp"

llvm::Value* LogErrorCodegen(const char *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	vfprintf(stderr, fmt, v);
	va_end(v);
	fprintf(stderr, "\n");
	return nullptr;
}

CIR::CIR() :
m_Module(std::make_unique<llvm::Module>("my hot jit", m_Context)),
m_Builder(m_Context),
m_FPM(std::make_unique<FunctionPassManager>(m_Module.get())),
m_JIT(nullptr)
{
	m_JIT = std::make_unique<CKaleidoscopeJIT>();
}

void CIR::BuildPassManager()
{
	// Open a new module
	m_Module = std::make_unique<llvm::Module>("my hot jit", m_Context);
	m_Module->setDataLayout(m_JIT->TargetMachine().createDataLayout());

	// Create a new pass manager attached to module
	m_FPM = std::make_unique<FunctionPassManager>(m_Module.get());

	// Do simple "peephole" optimizations and bit-twiddling optzns.
	m_FPM->add(llvm::createInstructionCombiningPass());
	// Reassociate expressions
	m_FPM->add(llvm::createReassociatePass());
	// Eliminate Common SubExpressions
	m_FPM->add(llvm::createGVNPass());
	// Simplify the control flow graph (i.e. delete unreachable blocks)
	m_FPM->add(llvm::createCFGSimplificationPass());

	m_FPM->doInitialization();
}

//Create an alloca instruction in the entry block of the function
llvm::AllocaInst *CIR::CreateEntryBlockAlloca(llvm::Function* f, const std::string& n)
{
	llvm::IRBuilder<> tmpb(&f->getEntryBlock(),
			       f->getEntryBlock().begin());
	return tmpb.CreateAlloca(llvm::Type::getDoubleTy(m_Context),
				 0,
				 n.c_str());
}

llvm::Function *CIR::GetFunction(const std::string &name)
{
	// First, see if this function is already exists in this module
	auto* f = m_Module->getFunction(name);
	if (f)
		return f;

	// If not, check whether we can codegen for declaration from some
	// existing prototype
	auto fpi = m_FunctionPrototypes.find(name);
	if (fpi != m_FunctionPrototypes.end())
		return fpi->second->codegen();

	// If no existing prototype exists, returning nothing
	return nullptr;
}

void CIR::AddFunction(std::unique_ptr<CPrototypeAST>&& proto)
{
	m_FunctionPrototypes[proto.get()->Name()] = std::move(proto);
}

