#ifndef PARSER_HPP
#define PARSER_HPP

#include "../lexer/lexer.hpp"
#include "../ast/exprast.hpp"
#include "../ast/functionast.hpp"
#include "../ast/prototypeast.hpp"

#include <map>
#include <memory>

class CParser
{
	// Current token buffer
	int m_CurrentToken;
	// Reference to a lexer (providing GetToken)
	CLexer& m_Lexer;
	// Current number of top-level expression
	uint64_t m_TopLevelExprNumber;

	// Map for bin operation precedence
	static std::map<char, int> m_BinOpPrecedence;
public:
	CParser(CLexer& l);
	inline int GetCurrentToken() const { return m_CurrentToken; }
	inline int GetNextToken() { return m_CurrentToken = m_Lexer.GetToken(); }
	int GetTokenPrecedence() const;
	static inline std::map<char, int>& BinOpPrecedenceMap() { return m_BinOpPrecedence; }

	// Parsing functions
	std::unique_ptr<CExprAST> ParseExpression();
	std::unique_ptr<CExprAST> ParseNumber();
	std::unique_ptr<CExprAST> ParseParenExpr();
	std::unique_ptr<CExprAST> ParseIfExpr();
	std::unique_ptr<CExprAST> ParseForExpr();
	std::unique_ptr<CExprAST> ParseIdentifierExpr();
	std::unique_ptr<CExprAST> ParsePrimary();
	std::unique_ptr<CExprAST> ParseUnary();
	std::unique_ptr<CExprAST> ParseBinOpRHS(int precedence, std::unique_ptr<CExprAST> lhs);

	std::unique_ptr<CPrototypeAST> ParsePrototype();
	std::unique_ptr<CFunctionAST> ParseDefinition();
	std::unique_ptr<CFunctionAST> ParseTopLevelExpr();
	std::unique_ptr<CPrototypeAST> ParseExtern();

};

template<class T = CExprAST>
std::unique_ptr<T> LogError(const char *s)
{
	fprintf(stderr, "Parser error: %s\n", s);
	return nullptr;
}

#endif // PARSER_HPP
