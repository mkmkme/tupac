#ifndef LEXER_HPP
#define LEXER_HPP

#include <map>
#include <string>
#include "token.hpp"

class CLexer
{
	std::string	m_IdentifierStr;	// Identifier string
	double		m_NumValue;			// Last numeric token
	char		m_LastChar;			// Last read char
	std::map<std::string, int> m_Tokens;
public:
	CLexer();
	virtual ~CLexer() {}

	int GetToken();
	inline double GetNumVal() const { return m_NumValue; }
	inline std::string GetIdStr() const { return m_IdentifierStr; }
};

#endif  // LEXER_HPP
