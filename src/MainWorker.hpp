#ifndef MAINWORKER_HPP
#define MAINWORKER_HPP

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/parser.hpp"

class CMainWorker
{
	CLexer m_Lexer;
	CParser m_Parser;

public:
	CMainWorker();

	void HandleDefinition();
	void HandleExtern();
	void HandleTopLevelExpr();

	void MainLoop();
};

#endif
