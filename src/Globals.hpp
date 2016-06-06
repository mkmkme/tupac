#ifndef GLOBALS_HPP
#define GLOBALS_HPP

class CIR;

class CGlobals
{
	static CIR m_IR;
public:
	CGlobals()
	{}

	static inline CIR& IR() { return m_IR; }
};


#endif
