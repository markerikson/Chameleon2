/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: //depot/code/org.antlr/main/main/lib/cpp/src/LLkParser.cpp#5 $
 */
/* 1999-2007 Version 3.2 November 2007
 *
 * Optional version
 *
 * Modified by David Wigg at London South Bank University for CPP_parser.g
 * to provide dynamic tracing
 *
 * See MyReadMe.txt for further information
 *
 * This file is best viewed in courier font with tabs set to 4 spaces
 */

#include "antlr/LLkParser.hpp"
#include <iostream>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

ANTLR_USING_NAMESPACE(std)

/**An LL(k) parser.
 *
 * @see antlr.Token
 * @see antlr.TokenBuffer
 * @see antlr.LL1Parser
 */

//	LLkParser(int k_);

LLkParser::LLkParser(const ParserSharedInputState& state, int k_)
: Parser(state), k(k_)
{
}

LLkParser::LLkParser(TokenBuffer& tokenBuf, int k_)
: Parser(tokenBuf), k(k_)
{
}

LLkParser::LLkParser(TokenStream& lexer, int k_)
: Parser(new TokenBuffer(lexer)), k(k_)
{
}

void LLkParser::trace(const char* ee, const char* rname)
{
	traceIndent();

	cout << ee << rname << ((inputState->guessing>0)?";[guessing]":"; ");
	// version to show guesing level
	//cout << ee << rname << ((inputState->guessing>0)?";[guessing]":"; ") <<  inputState->guessing ;

	for (int i = 1; i <= k; i++)
	{
		if (i != 1) {
			cout << ", ";
		}
		cout << "LA(" << i << ")==";

		string temp;

		try {
			temp = LT(i)->getText().c_str();
		}
		catch( ANTLRException& ae )
		{
			temp = "[error: ";
			temp += ae.toString();
			temp += ']';
		}
		cout << temp;
	}

	cout << endl;
}

// DW 060204 For dynamic tracing
void LLkParser::antlrTrace(bool traceFlag)
	{
	antlrTracing = traceFlag;
	}

void LLkParser::traceIn(const char* rname)
{
	traceDepth++;
	// DW 060204 For dynamic tracing
	if (antlrTracing)
		trace("> ",rname);
}

void LLkParser::traceOut(const char* rname)
{
	// DW 060204 For dynamic tracing
	if (antlrTracing)	
		trace("< ",rname);
	traceDepth--;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
