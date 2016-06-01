#include "parser.hpp"

std::map<char, int> CParser::m_BinOpPrecedence = {
	{ '<', 10 },
//	{ '>', 10 },
	{ '+', 20 },
	{ '-', 20 },
	{ '*', 40 }
};

int CParser::GetTokenPrecedence() const
{
	if (!isascii(m_CurrentToken))
		return -1;

	// Make sure it's a declared binary operation
	int r = m_BinOpPrecedence[m_CurrentToken];
	return (r <= 0) ? -1 : r;
}
