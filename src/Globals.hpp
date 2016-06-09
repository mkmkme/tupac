#ifndef GLOBALS_HPP
#define GLOBALS_HPP

class CIR;
class CLexer;
class CParser;
class CKaleidoscopeJIT;

class CGlobals
{
	static CIR m_IR;
	static CLexer m_Lexer;
	static CParser m_Parser;
	static CKaleidoscopeJIT m_JIT;
public:
	CGlobals()
	{}

	static inline CIR& IR() { return m_IR; }
	static inline CLexer& Lexer() { return m_Lexer; }
	static inline CParser& Parser() { return m_Parser; }
};


#endif
