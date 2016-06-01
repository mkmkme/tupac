#include "parser.hpp"

#include "../ast/binexprast.hpp"
#include "../ast/callexprast.hpp"
#include "../ast/numexprast.hpp"
#include "../ast/varexprast.hpp"

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


////////////////////////////////////////////////
///			  Parsing functions				 ///
////////////////////////////////////////////////


std::unique_ptr<CExprAST> CParser::ParseExpression()
{
	auto lhs = ParsePrimary();
	if (!lhs)
		return nullptr;

	return ParseBinOpRHS(0, std::move(lhs));
}

std::unique_ptr<CExprAST> CParser::ParseNumber()
{
	auto r = std::make_unique<CNumberExprAST>(m_Lexer.GetNumVal());
	GetNextToken();
	return std::move(r);
}

std::unique_ptr<CExprAST> CParser::ParseParenExpr()
{
	GetNextToken(); // eating '('
	auto r = ParseExpression();
	if (!r)
		return nullptr;

	if (m_CurrentToken != ')')
		return LogError("Expected ')'");
	GetNextToken(); // eating ')'
	return r;
}

std::unique_ptr<CExprAST> CParser::ParseIdentifierExpr()
{
	std::string id = m_Lexer.GetIdStr();
	GetNextToken(); // eating identifier

	if (m_CurrentToken != '(') // variable reference
		return std::make_unique<CVariableExprAST>(id);

	// if the current token is a '(', this is a function call
	GetNextToken(); // eating '('
	std::vector<std::unique_ptr<CExprAST>> args;
	if (m_CurrentToken != ')') {
		while (true) {
			auto arg = ParseExpression();
			if (arg)
				args.push_back(std::move(arg));
			else
				return nullptr;

			// If current token is ')', we are done with args
			if (m_CurrentToken == ')')
				break;

			if (m_CurrentToken != ',')
				return LogError("Expected ')' or ',' in argument list!");

			GetNextToken();
		}
	}

	GetNextToken(); // eating the ')'
	return std::make_unique<CCallExprAST>(id, std::move(args));
}

std::unique_ptr<CExprAST> CParser::ParsePrimary()
{
	switch (m_CurrentToken) {
	default:
		return LogError("unknown token when expecting an expression!");
	case tIdentifier:
		return ParseIdentifierExpr();
	case tNumber:
		return ParseNumber();
	case '(':
		return ParseParenExpr();
	}
}

std::unique_ptr<CExprAST> CParser::ParseBinOpRHS(int precedence, std::unique_ptr<CExprAST> lhs)
{
	// If this is a binary operation, find its precedence
	while (true) {
		int p = GetTokenPrecedence();

		// If this is a binop that binds at least as tightly as the current binop,
		// consume it, otherwise we are done
		if (p < precedence)
			return lhs;

		int binop = m_CurrentToken;
		GetNextToken(); // eating that binop

		// Parsing primary expression after the binary operator
		auto rhs = ParsePrimary();
		if (!rhs)
			return nullptr;

		// If binop binds less tightly that the operator after rhs,
		// let the pending operator take rhs as it lhs
		int nextp = GetTokenPrecedence();
		if (p < nextp) {
			rhs = ParseBinOpRHS(p + 1, std::move(rhs));
			if (!rhs)
				return nullptr;
		}

		// Merge lhs/rhs
		lhs = std::make_unique<CBinaryExprAST>(binop, std::move(lhs), std::move(rhs));
	}
}

std::unique_ptr<CPrototypeAST> CParser::ParsePrototype()
{
	if (m_CurrentToken != tIdentifier)
		return LogError<CPrototypeAST>("Expected function name in prototype");

	std::string name = m_Lexer.GetIdStr();
	GetNextToken(); // eating name

	if (m_CurrentToken != '(')
		return LogError<CPrototypeAST>("Expected '(' in prototype");

	std::vector<std::string> args;
	while (GetNextToken() == tIdentifier)
		args.push_back(m_Lexer.GetIdStr());

	if (m_CurrentToken != ')')
		return LogError<CPrototypeAST>("Expected ')' in prototype");
	GetNextToken(); // eating ')'

	return std::make_unique<CPrototypeAST>(name, std::move(args));
}

std::unique_ptr<CFunctionAST> CParser::ParseDefinition()
{
	GetNextToken(); // eating "def"
	auto p = ParsePrototype();
	if (!p)
		return nullptr;

	auto e = ParseExpression();
	if (!e)
		return nullptr;

	return std::make_unique<CFunctionAST>(std::move(p), std::move(e));
}

std::unique_ptr<CFunctionAST> CParser::ParseTopLevelExpr()
{
	auto e = ParseExpression();
	if (!e)
		return nullptr;

	// Make an anonymous prototype
	auto p = std::make_unique<CPrototypeAST>("__anon_expr", std::vector<std::string>());
	return std::make_unique<CFunctionAST>(std::move(p), std::move(e));
}

std::unique_ptr<CPrototypeAST> CParser::ParseExtern()
{
	GetNextToken(); // eating "extern"
	return ParsePrototype();
}


