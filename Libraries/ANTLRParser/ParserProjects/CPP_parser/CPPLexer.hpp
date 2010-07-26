#ifndef INC_CPPLexer_hpp_
#define INC_CPPLexer_hpp_

#line 90 "CPP_parser.g"
// pre_include_hpp
	
#line 8 "CPPLexer.hpp"
#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (20060906): "CPP_parser.g" -> "CPPLexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "STDCTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 94 "CPP_parser.g"
// post_include_hpp
	
#line 19 "CPPLexer.hpp"
#line 106 "CPP_parser.g"

	// File generated from CPP_parser.g
	// Version 3.2 November 2007
	// This file is best viewed in courier font with tabs set to 4 spaces
	//
	// The statements in this block appear in both CPPLexer.hpp and CPPParser.hpp
	#include "antlr/CharScanner.hpp"
	#include "CPPDictionary.hpp"
	#include "data.h"
	#include "PNode.hpp"
	#include "PNodeFactory.hpp"

	// Following externs declared here to be available for users

	// Declared and set in CPPParser.cpp
	extern int lineNo; // current line
	extern bool in_user_file;	// true = in principal file, false = in an include file

	// Declared and set in CPPLexer.cpp
	extern bool in_user_file_deferred;

	extern int deferredLineCount;	// used to accumulate line numbers in comments etc.

	extern char principal_file[128];	// Name of user file
	extern int principal_line;		// Principal file's line number
	extern int principal_last_set;	// Where principal file's line number was last set
									//   in preprocessed *.i file

	extern char current_included_file[128];	// Name of current include file
	extern int include_line;		// Included file's line number
	extern int include_last_set;	// Where included file's line number was last set
									//   in preprocessed *.i file

	// The statements in this block appear in both CPPLexer.hpp and CPPParser.hpp
	
#line 56 "CPPLexer.hpp"
class CUSTOM_API CPPLexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public STDCTokenTypes
{
#line 2728 "CPP_parser.g"

void deferredNewline() 
	{
	deferredLineCount++;	// Declared and initialised in main.cpp (Not true!)
	}

void newline() 
	{
	for (;deferredLineCount>0;deferredLineCount--)
	    {
		CharScanner::newline();
		}
    CharScanner::newline();
    }

// for processing #line .... lines (see note above)
void process_line_directive(const char *includedFile, const char *includedLineNo)
	{
	// See global interface variables above
	// Working variables
	static int line, result;
	static bool principal_file_set = false;
	static int x;
	// Extract included file line no.
	result = sscanf(includedLineNo, "%d \n", &line);

	//OUTPUT_DEBUG("CPPLexer line directive on line %d\n",lineNo);
	// remove first " from file path+name by shifting all characters left
	for(x=1;includedFile[x]!='"';x++)
		{
		current_included_file[x-1] = includedFile[x];
		}

	// Check path and name are not too long
	if(x>128)
		{
		//
		OUTPUT_DEBUG("Path and name of included file too long\n");
		OUTPUT_DEBUG("Increase length of current_included_file and\n");
		OUTPUT_DEBUG("  principal_file to at least %d characters\n",x);
		OUTPUT_DEBUG("Abandon run\n");
		getchar();		    
		}

	// Replace last " from file name with null
	current_included_file[x-1] = NULL;

	// This will occur on reading the first preprocessor #line directive
	if (!principal_file_set)
		{
		strcpy (principal_file, current_included_file);
		principal_file_set = true;
		}

	// check for main file name
	if (strcmp(principal_file, current_included_file) == 0)
		{
		principal_line = line;	// this is line no. shown in line directive
		principal_last_set = CharScanner::getLine();	// This is line no. in preprocessed *.i file
		strcpy(current_included_file, " "); // delete include file name
		in_user_file_deferred = true; // Users's .C or .CPP file (aka principal_file) detected
		}
	else
		// Check that this is a genuine path
		if(current_included_file[1]==':')
			{//OUTPUT_DEBUG("main.cpp 222 entered\n");
			//OUTPUT_DEBUG("main.cpp 223 %s %s\n",principal_file,current_included_file);
			include_line = line;	// this is line no. shown in line directive
			include_last_set = CharScanner::getLine();	// This is line no. in preprocessed *.i file
			in_user_file_deferred = false; // Header file file detected
			}
	}

#line 60 "CPPLexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	CPPLexer(ANTLR_USE_NAMESPACE(std)istream& in);
	CPPLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	CPPLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mASSIGNEQUAL(bool _createToken);
	public: void mCOLON(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mQUESTIONMARK(bool _createToken);
	public: void mSEMICOLON(bool _createToken);
	public: void mPOINTERTO(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mLSQUARE(bool _createToken);
	public: void mRSQUARE(bool _createToken);
	public: void mLCURLY(bool _createToken);
	public: void mRCURLY(bool _createToken);
	public: void mEQUAL(bool _createToken);
	public: void mNOTEQUAL(bool _createToken);
	public: void mLESSTHANOREQUALTO(bool _createToken);
	public: void mLESSTHAN(bool _createToken);
	public: void mGREATERTHANOREQUALTO(bool _createToken);
	public: void mGREATERTHAN(bool _createToken);
	public: void mDIVIDE(bool _createToken);
	public: void mDIVIDEEQUAL(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mPLUSEQUAL(bool _createToken);
	public: void mPLUSPLUS(bool _createToken);
	public: void mMINUS(bool _createToken);
	public: void mMINUSEQUAL(bool _createToken);
	public: void mMINUSMINUS(bool _createToken);
	public: void mSTAR(bool _createToken);
	public: void mTIMESEQUAL(bool _createToken);
	public: void mMOD(bool _createToken);
	public: void mMODEQUAL(bool _createToken);
	public: void mSHIFTRIGHT(bool _createToken);
	public: void mSHIFTRIGHTEQUAL(bool _createToken);
	public: void mSHIFTLEFT(bool _createToken);
	public: void mSHIFTLEFTEQUAL(bool _createToken);
	public: void mAND(bool _createToken);
	public: void mNOT(bool _createToken);
	public: void mOR(bool _createToken);
	public: void mAMPERSAND(bool _createToken);
	public: void mBITWISEANDEQUAL(bool _createToken);
	public: void mTILDE(bool _createToken);
	public: void mBITWISEOR(bool _createToken);
	public: void mBITWISEOREQUAL(bool _createToken);
	public: void mBITWISEXOR(bool _createToken);
	public: void mBITWISEXOREQUAL(bool _createToken);
	public: void mPOINTERTOMBR(bool _createToken);
	public: void mDOTMBR(bool _createToken);
	public: void mSCOPE(bool _createToken);
	public: void mWhitespace(bool _createToken);
	protected: void mSpace(bool _createToken);
	public: void mComment(bool _createToken);
	protected: void mEndOfLine(bool _createToken);
	public: void mCPPComment(bool _createToken);
	public: void mPREPROC_DIRECTIVE(bool _createToken);
	protected: void mLineDirective(bool _createToken);
	protected: void mDecimal(bool _createToken);
	public: void mStringLiteral(bool _createToken);
	public: void mPragma(bool _createToken);
	protected: void mEndOfContinuedLine(bool _createToken);
	public: void mError(bool _createToken);
	public: void mCharLiteral(bool _createToken);
	protected: void mEscape(bool _createToken);
	protected: void mUniversalCharacterName(bool _createToken);
	public: void mWCharLiteral(bool _createToken);
	public: void mWStringLiteral(bool _createToken);
	protected: void mDigit(bool _createToken);
	protected: void mHexadecimalDigit(bool _createToken);
	protected: void mLongSuffix(bool _createToken);
	protected: void mUnsignedSuffix(bool _createToken);
	protected: void mFloatSuffix(bool _createToken);
	protected: void mExponent(bool _createToken);
	protected: void mHexQuad(bool _createToken);
	protected: void mVocabulary(bool _createToken);
	public: void mNumber(bool _createToken);
	public: void mID(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
	static const unsigned long _tokenSet_7_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_7;
	static const unsigned long _tokenSet_8_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_8;
	static const unsigned long _tokenSet_9_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_9;
	static const unsigned long _tokenSet_10_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_10;
	static const unsigned long _tokenSet_11_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_11;
	static const unsigned long _tokenSet_12_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_12;
	static const unsigned long _tokenSet_13_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_13;
};

#endif /*INC_CPPLexer_hpp_*/
