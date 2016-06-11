#include "MainWorker.hpp"

#include "codegen/ir.hpp"
#include "codegen/kaleidoscopejit.hpp"
#include "modules/print.hpp"
#include "Globals.hpp"

#include <memory>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>

int main(int argc, char *argv[])
{
	LLVMInitializeNativeTarget();
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmParser();
	llvm::InitializeNativeTargetAsmPrinter();
	CGlobals::IR().JIT().Init();
	CGlobals::IR().BuildPassManager();

	CMainWorker mw;
	mw.MainLoop();

	CGlobals::IR().Module()->dump();

	return 0;
}
