#include "MainWorker.hpp"

#include <cstdio>

CMainWorker::CMainWorker() :
m_Lexer(),
m_Parser(m_Lexer)
{
	fprintf(stderr, "ready> ");
	m_Parser.GetNextToken();
}

void CMainWorker::HandleDefinition()
{
	if (m_Parser.ParseDefinition())
		fprintf(stderr, "Parsed a function definition\n");
	else
		m_Parser.GetNextToken(); // Skip token for error recovery
}

void CMainWorker::HandleExtern()
{
	if (m_Parser.ParseExtern())
		fprintf(stderr, "Parsed an extern\n");
	else
		m_Parser.GetNextToken();
}

void CMainWorker::HandleTopLevelExpr()
{
	if (m_Parser.ParseTopLevelExpr())
		fprintf(stderr, "Parsed a top-level expression\n");
	else
		m_Parser.GetNextToken();
}

void CMainWorker::MainLoop()
{
	while (true) {
		fprintf(stderr, "ready> ");
		int t = m_Parser.GetCurrentToken();
		switch (t) {
		case tEOF:
			return;
		case ';':
			m_Parser.GetNextToken();
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
