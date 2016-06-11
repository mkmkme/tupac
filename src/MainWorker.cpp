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
		fprintf(stderr, "Read top-level expression: ");
		fir->dump();
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
