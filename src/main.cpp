#include "MainWorker.hpp"

#include "codegen/ir.hpp"

int main(int argc, char *argv[])
{
	CMainWorker mw;
	mw.MainLoop();
	mw.Tupac().IR().Module()->dump();
	return 0;
}
