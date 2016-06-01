#ifndef PARSER_HPP
#define PARSER_HPP

#include "../lexer/lexer.hpp"

#include <map>

class CParser
{
	// Current token buffer
	int m_CurrentToken;
	// Reference to a lexer (providing GetToken)
	CLexer& m_Lexer;

	// Map for bin operation precedence
	static const std::map<char, int> m_BinOpPrecedence;
public:
	CParser(CLexer& l) : m_CurrentToken(' '), m_Lexer(l) {}
	inline int GetNextToken() { return m_CurrentToken = m_Lexer.GetToken(); }
	int GetTokenPrecedence() const;
};

#endif // PARSER_HPP
