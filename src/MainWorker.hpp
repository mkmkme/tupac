#ifndef MAINWORKER_HPP
#define MAINWORKER_HPP



#include "Globals.hpp"

class CMainWorker
{
	CGlobals tupac;

public:
	CMainWorker();

	inline CGlobals& Tupac() { return tupac; }

	void HandleDefinition();
	void HandleExtern();
	void HandleTopLevelExpr();

	void MainLoop();
};

#endif
