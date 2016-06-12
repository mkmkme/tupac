#include "lexer.hpp"

#include <cstdio>
#include <cctype>
#include <cstdlib>

CLexer::CLexer() :
m_IdentifierStr(),
m_NumValue(0.),
m_LastChar(' ')
{
	m_Tokens["def"]		= tDef;
	m_Tokens["extern"]	= tExtern;
	m_Tokens["if"]		= tIf;
	m_Tokens["then"]	= tThen;
	m_Tokens["else"]	= tElse;
	m_Tokens["for"]		= tFor;
	m_Tokens["in"]		= tIn;
	m_Tokens["binary"]	= tBinary;
	m_Tokens["unary"]	= tUnary;
	m_Tokens["var"]		= tVar;
}

int CLexer::GetToken()
{
	// Skip all whitespaces
	while (isspace(m_LastChar))
		m_LastChar = getchar();

	// If token starts with alphabetical symbol, trying to parse it
	if (isalpha(m_LastChar)) {
		// Collecting the token string
		m_IdentifierStr = m_LastChar;
		while (isalnum(m_LastChar = getchar()))
			m_IdentifierStr += m_LastChar;

		// Recognizing if it s one of the keywords
		auto it = m_Tokens.find(m_IdentifierStr);
		if (it != m_Tokens.end())
			return it->second;
		// Otherwise just returning identifier type
		return tIdentifier;
	}

	// If token starts with digit, trying to parse it as a number
	if (isdigit(m_LastChar) || m_LastChar == '.') {
		// Collecting number from a string
		std::string numstr;
		bool dotfound = (m_LastChar == '.');
		while (true) {
			numstr += m_LastChar;
			m_LastChar = getchar();
			if (m_LastChar == '.') {
				if (dotfound)
					return tError;
				else
					dotfound = true;
			}
			else if (!isdigit(m_LastChar))
				break;
		}
		// Trying to convert and returning number type
		m_NumValue = strtod(numstr.c_str(), nullptr);
		return tNumber;
	}

	// If token starts with a '#', this is a comment. Just ignoring it
	if (m_LastChar == '#') {
		do
			m_LastChar = getchar();
		while (m_LastChar != EOF && m_LastChar != '\n' && m_LastChar != '\r');

		// If this is not the end of file, trying to get next token
		if (m_LastChar != EOF)
			return GetToken();
	}

	// Check for EOF
	if (m_LastChar == EOF)
		return tEOF;

	// Otherwise, return the ASCII value of this char
	int r = m_LastChar;
	m_LastChar = getchar();
	return r;
}


