#include "MainWorker.hpp"

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/parser.hpp"

#include <cstdio>

CMainWorker::CMainWorker()
{
	fprintf(stderr, "ready> ");
	CGlobals::Parser().GetNextToken();
}

void CMainWorker::HandleDefinition()
{
	auto fast = CGlobals::Parser().ParseDefinition();
	if (!fast) {
		CGlobals::Parser().GetNextToken(); // Skip token for error recovery
		return;
	}

	auto* fir = fast->codegen();
	if (fir) {
		fprintf(stderr, "Read function definition: ");
		fir->dump();
	}
}

void CMainWorker::HandleExtern()
{
	auto past = CGlobals::Parser().ParseExtern();
	if (!past) {
		CGlobals::Parser().GetNextToken();
		return;
	}
	printf("Read extern named %s\n", past->Name().c_str());

	auto* fir = past->codegen();
	if (fir) {
		fprintf(stderr, "Read extern: ");
		fir->dump();
	}
}

void CMainWorker::HandleTopLevelExpr()
{
	auto fast = CGlobals::Parser().ParseTopLevelExpr();
	if (!fast) {
		CGlobals::Parser().GetNextToken();
		return;
	}

	auto* fir = fast->codegen();
	if (fir) {
		fir->dump();

		CKaleidoscopeJIT& jit = CGlobals::IR().JIT();

		// JIT the module containing the anonymous expression, keeping a
		// handle so we can free it later
		auto h = jit.AddModule(CGlobals::IR().MoveModule());
		CGlobals::IR().BuildPassManager();

		// Search the JIT for the __anon_expr symbol
		auto s = jit.FindSymbol("__anon_expr");
		assert(s && "Function not found!");

		// Get the symbol's address and cast it to the right type
		// double(). Then we can call it as a native function
		double (*fp)() = (double(*)())(intptr_t)s.getAddress();
		fprintf(stderr, "Result is: %f\n", fp());

		// Delete the anonymous expression module from JIT
		jit.RemoveModule(h);
	}
}

void CMainWorker::MainLoop()
{
	while (true) {
		fprintf(stderr, "ready> ");
		int t = CGlobals::Parser().GetCurrentToken();
		switch (t) {
		case tEOF:
			return;
		case ';':
			CGlobals::Parser().GetNextToken();
			break;
		case tDef:
			HandleDefinition();
			break;
		case tExtern:
			HandleExtern();
			break;
		default:
			HandleTopLevelExpr();
			break;
		}
	}
}
