#include "parser.hpp"

#include "../ast/binexprast.hpp"
#include "../ast/callexprast.hpp"
#include "../ast/forexprast.hpp"
#include "../ast/ifexprast.hpp"
#include "../ast/numexprast.hpp"
#include "../ast/unaryexprast.hpp"
#include "../ast/varexprast.hpp"

#include <cstdint>
#include <sstream>

std::map<char, int> CParser::m_BinOpPrecedence = {
	{ '<', 10 },
//	{ '>', 10 },
	{ '+', 20 },
	{ '-', 20 },
	{ '*', 40 }
};

CParser::CParser(CLexer &l) :
m_CurrentToken(' '),
m_Lexer(l),
m_TopLevelExprNumber(0)
{

}

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
	auto lhs = ParseUnary();
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

std::unique_ptr<CExprAST> CParser::ParseIfExpr()
{
	GetNextToken(); // eating 'if'

	// Condition
	auto cond_ = ParseExpression();
	if (!cond_)
		return nullptr;

	if (m_CurrentToken != tThen)
		return LogError("Expecting then");

	GetNextToken(); // eating 'then'

	auto then_ = ParseExpression();
	if (!then_)
		return nullptr;

	if (m_CurrentToken != tElse)
		return LogError("Expecting else");

	GetNextToken(); // eating 'else'

	auto else_ = ParseExpression();
	if (!else_)
		return nullptr;

	return std::make_unique<CIfExprAST>(std::move(cond_),
					    std::move(then_),
					    std::move(else_));
}

std::unique_ptr<CExprAST> CParser::ParseForExpr()
{
	GetNextToken(); // eating 'for'

	if (m_CurrentToken != tIdentifier)
		return LogError("Expecting identifier after 'for'");

	std::string id = m_Lexer.GetIdStr();
	GetNextToken(); // eating identifier

	if (m_CurrentToken != '=')
		return LogError("Expecting '=' after identifier");
	GetNextToken(); // eating '='

	auto start = ParseExpression();
	if (!start)
		return nullptr;

	if (m_CurrentToken != ',')
		return LogError("Expecting ',' after start value in 'for'");
	GetNextToken(); // eating ','

	auto end = ParseExpression();
	if (!end)
		return nullptr;

	// Step value is optional
	std::unique_ptr<CExprAST> step;
	if (m_CurrentToken == ',') {
		GetNextToken();
		step = ParseExpression();
		if (!step)
			return nullptr;
	}

	if (m_CurrentToken != tIn)
		return LogError("Expecting 'in' after 'for'");
	GetNextToken(); // eating 'in'

	auto body = ParseExpression();
	if (!body)
		return nullptr;

	return std::make_unique<CForExprAST>(id,
					     std::move(start),
					     std::move(end),
					     std::move(step),
					     std::move(body));
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
	case tIf:
		return ParseIfExpr();
	case tFor:
		return ParseForExpr();
	}
}

std::unique_ptr<CExprAST> CParser::ParseUnary()
{
	// If current token is not an operator, it must be a primary expr
	if (!isascii(m_CurrentToken) || m_CurrentToken == '(' || m_CurrentToken == ',')
		return ParsePrimary();

	// If this is an unary operator, read it
	int opcode = m_CurrentToken;
	GetNextToken();
	auto operand = ParseUnary();
	if (operand)
		return std::make_unique<CUnaryExprAST>(opcode, std::move(operand));

	return nullptr;
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

		// Parsing unary expression after the binary operator
		auto rhs = ParseUnary();
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
	std::string name;
	unsigned kind = 0; // 0 - id, 1 - unary, 2 - binary
	unsigned precedence = 30;

	switch (m_CurrentToken) {
	default:
		return LogError<CPrototypeAST>("Expected function name in prototype");
	case tIdentifier:
		name = m_Lexer.GetIdStr();
		kind = 0;
		GetNextToken();
		break;
	case tUnary:
		GetNextToken();
		if (!isascii(m_CurrentToken))
			return LogError<CPrototypeAST>("Expected unary operator");

		name = "unary";
		name += (char)m_CurrentToken;
		kind = 1;
		GetNextToken();
		break;
	case tBinary:
		GetNextToken();
		if (!isascii(m_CurrentToken))
			return LogError<CPrototypeAST>("Expected binary operator");
		name = "binary";
		name += (char)m_CurrentToken;
		kind = 2;
		GetNextToken();

		// Read the precedence if present
		if (m_CurrentToken == tNumber) {
			if (m_Lexer.GetNumVal() < 1 || m_Lexer.GetNumVal() > 100)
				return LogError<CPrototypeAST>("Invalid precedence: must be 1..100");
			precedence = (unsigned)m_Lexer.GetNumVal();
			GetNextToken();
		}
		break;
	}

	if (m_CurrentToken != '(')
		return LogError<CPrototypeAST>("Expected '(' in prototype");

	std::vector<std::string> args;
	while (GetNextToken() == tIdentifier)
		args.push_back(m_Lexer.GetIdStr());

	if (m_CurrentToken != ')')
		return LogError<CPrototypeAST>("Expected ')' in prototype");
	GetNextToken(); // eating ')'

	// Verify right number of names for operator
	if (kind && args.size() != kind)
		return LogError<CPrototypeAST>("Invalid number of operands in operator");

	return std::make_unique<CPrototypeAST>(name, std::move(args), kind != 0, precedence);
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

#if 0
	// Make an anonymous prototype
	std::stringstream ss;
	ss << "__anon_expr_";
	ss << m_TopLevelExprNumber++;
	std::string s = ss.str();
#endif
	auto p = std::make_unique<CPrototypeAST>("__anon_expr", std::vector<std::string>());
	return std::make_unique<CFunctionAST>(std::move(p), std::move(e));
}

std::unique_ptr<CPrototypeAST> CParser::ParseExtern()
{
	GetNextToken(); // eating "extern"
	return ParsePrototype();
}


