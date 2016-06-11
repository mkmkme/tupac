#ifndef MAINWORKER_HPP
#define MAINWORKER_HPP

#include "Globals.hpp"

class CMainWorker
{
public:
	CMainWorker();

	void HandleDefinition();
	void HandleExtern();
	void HandleTopLevelExpr();

	void MainLoop();
};

#endif
