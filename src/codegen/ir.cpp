#include "ir.hpp"


llvm::Value* LogErrorCodegen(const char *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	vfprintf(stderr, fmt, v);
	va_end(v);
	fprintf(stderr, "\n");
	return nullptr;
}
