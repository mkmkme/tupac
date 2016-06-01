#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include "token.hpp"

class CLexer
{
	std::string	m_IdentifierStr;	// Identifier string
	double		m_NumValue;			// Last numeric token
	char		m_LastChar;			// Last read char
public:
	CLexer();
	virtual ~CLexer() {}

	int GetToken();
};

#endif  // LEXER_HPP
