#ifndef TOKEN_HPP
#define TOKEN_HPP

enum Token {
	tEOF		= -1,
	tDef		= -2,
	tExtern		= -3,
	tIdentifier	= -4,
	tNumber		= -5,
	tIf		= -6,
	tThen		= -7,
	tElse		= -8,
	tFor		= -9,
	tIn		= -10,
	tBinary		= -11,
	tUnary		= -12,
	tVar		= -13,
	tError		= -14
};

#endif // TOKEN_HPP
