#include "Globals.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/ir.hpp"

CLexer CGlobals::m_Lexer;
CParser CGlobals::m_Parser(CGlobals::m_Lexer);
CIR CGlobals::m_IR;
