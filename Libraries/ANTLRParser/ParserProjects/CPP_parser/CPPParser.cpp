/* $ANTLR 2.7.7 (20060906): "CPP_parser.g" -> "CPPParser.cpp"$ */
// pre_include_cpp
	
#include "CPPParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
// post_include_cpp
	

// File generated from CPP_parser.g
// Version 3.2 November 2007
// This file is best viewed in courier font with tabs set to 4 spaces
//
//	The statements in this block appear only in CPPParser.cpp

int statementTrace = 2;	// Used to control selected (level) tracing (see support.cpp)
						// 1 Shows which external and member statements selected
						// 2 Shows above plus all declarations/definitions
						// 3 reserved for future use
						// 4 and above available for user

void CPPParser::init()
	{
	antlrTrace(false);	// This is a dynamic trace facility for use with -traceParser etc.
						// It requires modification in LLkParser.cpp and LLkParser.hpp
						//  (Copies of these modified files are supplied with this code)
						//  otherwise it should be commented out (see MyReadMe.txt)
						// true shows antlr trace (or can be set and reset during parsing)
						// false stops showing antlr trace 
						// Provided the parser is always generated with -traceParser this
						//  facility allows trace output to be turned on or off just by changing
						//  the setting here from false to true or vice versa and then
						//  recompiling and linking CPPParser only thus avoiding the need
						//  to use antlr.Tool to re-generate the lexer and parser again
						//  with (or without) -traceParser each time before recompiling. 
						// Antlr trace can also be turned on and off dynamically using
						//  antlrTrace_on or antlrTrace_off statements inserted into the
						//  source code being parsed (See antlrTrace_on and antlrTrace_off below).
					
	// Creates a dictionary to hold symbols with 4001 buckets, 200 scopes and 800,000 characters
	// These can be changed to suit the size of program(s) being parsed
	symbols = new CPPDictionary(4001, 200, 800000);

	// Set template parameter scope - Not used at present
	templateParameterScope = symbols->getCurrentScopeIndex();	// Set template parameter scope to 0

	symbols->saveScope();	// Advance currentScope from 0 to 1
	// Set "external" scope for all types
	externalScope = symbols->getCurrentScopeIndex();	// Set "external" scope to 1 for types

	// Declare predefined scope "std" in external scope
	CPPSymbol *a = new CPPSymbol("std", CPPSymbol::otTypedef);
	symbols->define("std", a);

	symbols->saveScope();	// Advance currentScope from 1 to 2 (and higher) for all other symbols
							//  treated as locals

	// Global flags to allow for nested declarations
	_td = false;		// For typedef
	_fd = false;		// For friend
	_sc = scInvalid;	// For StorageClass
	_tq = tqInvalid;	// For TypeQualifier
	_ts = tsInvalid;	// For TypeSpecifier
	_fs = fsInvalid;	// For FunctionSpecifier

	functionDefinition = 0;
	qualifierPrefix[0] = '\0';
	enclosingClass = "";
	assign_stmt_RHS_found = 0;
	in_parameter_list = false;
	K_and_R = false;	// used to distinguish old K & R parameter definitions
	in_return = false;
	is_address = false;
	is_pointer = false;
	}	// End of CPPParser::init()

int lineNo = 0;
bool in_user_file;	// true = in principal file, false = in an include file
					// Set from in_user_file_deferred by external_declaration in CPP_parser.g

//	The statements in this block appear only in CPPParser.cpp

CPPParser::CPPParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

CPPParser::CPPParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,2)
{
}

CPPParser::CPPParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

CPPParser::CPPParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,2)
{
}

CPPParser::CPPParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,2)
{
}

void CPPParser::translation_unit() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode translation_unit_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if ( inputState->guessing==0 ) {
			enterExternalScope();
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				external_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop3;
			}
			
		}
		_loop3:;
		} // ( ... )*
		RefPNode tmp1_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp1_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp1_AST));
		}
		match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
		if ( inputState->guessing==0 ) {
			exitExternalScope();
		}
		translation_unit_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_1);
		} else {
			throw;
		}
	}
	returnAST = translation_unit_AST;
}

void CPPParser::external_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode external_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  m = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode m_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode sc_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode fc_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		 data s;
		 lineNo = LT(1)->getLine();
		 K_and_R = false;
		 FunctionSpecifier fs = fsInvalid;	// inline,virtual,explicit
		 in_user_file = in_user_file_deferred;
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_namespace:
		{
			if ( inputState->guessing==0 ) {
				if (statementTrace>=1) 
							OUTPUT_DEBUG("%d external_declaration Namespace definition\n",LT(1)->getLine());
						
			}
			match(LITERAL_namespace);
			namespace_definition();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		{
			if ( inputState->guessing==0 ) {
				if (statementTrace>=1) 
							OUTPUT_DEBUG("%d external_declaration Semicolon\n",LT(1)->getLine());
						
			}
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		case LITERAL_antlrTrace_on:
		{
			RefPNode tmp4_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp4_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp4_AST));
			}
			match(LITERAL_antlrTrace_on);
			if ( inputState->guessing==0 ) {
				antlrTrace(true);
			}
			break;
		}
		case LITERAL_antlrTrace_off:
		{
			RefPNode tmp5_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp5_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp5_AST));
			}
			match(LITERAL_antlrTrace_off);
			if ( inputState->guessing==0 ) {
				antlrTrace(false);
			}
			break;
		}
		default:
			bool synPredMatched7 = false;
			if (((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN))) {
				int _m7 = mark();
				synPredMatched7 = true;
				inputState->guessing++;
				try {
					{
					match(LITERAL_template);
					match(LESSTHAN);
					match(GREATERTHAN);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched7 = false;
				}
				rewind(_m7);
				inputState->guessing--;
			}
			if ( synPredMatched7 ) {
				if ( inputState->guessing==0 ) {
					if(statementTrace>=1) 
								OUTPUT_DEBUG("%d external_declaration template explicit-specialisation\n",LT(1)->getLine());
							
				}
				RefPNode tmp6_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp6_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
				}
				match(LITERAL_template);
				RefPNode tmp7_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp7_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
				}
				match(LESSTHAN);
				RefPNode tmp8_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp8_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp8_AST));
				}
				match(GREATERTHAN);
				external_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				bool synPredMatched9 = false;
				if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
					int _m9 = mark();
					synPredMatched9 = true;
					inputState->guessing++;
					try {
						{
						match(LITERAL_typedef);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched9 = false;
					}
					rewind(_m9);
					inputState->guessing--;
				}
				if ( synPredMatched9 ) {
					{
					bool synPredMatched12 = false;
					if (((LA(1) == LITERAL_typedef) && (LA(2) == LITERAL_enum))) {
						int _m12 = mark();
						synPredMatched12 = true;
						inputState->guessing++;
						try {
							{
							match(LITERAL_typedef);
							match(LITERAL_enum);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched12 = false;
						}
						rewind(_m12);
						inputState->guessing--;
					}
					if ( synPredMatched12 ) {
						if ( inputState->guessing==0 ) {
							if(statementTrace>=1) 
											OUTPUT_DEBUG("%d external_declaration Typedef enum type\n",LT(1)->getLine());
										
						}
						match(LITERAL_typedef);
						enum_specifier();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						if ( inputState->guessing==0 ) {
							_td = true;
						}
						{
						switch ( LA(1)) {
						case ID:
						case COLON:
						case LPAREN:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case STAR:
						case AMPERSAND:
						case SCOPE:
						case LITERAL__cdecl:
						case LITERAL___cdecl:
						case LITERAL__near:
						case LITERAL___near:
						case LITERAL__far:
						case LITERAL___far:
						case LITERAL___interrupt:
						case LITERAL_pascal:
						case LITERAL__pascal:
						case LITERAL___pascal:
						case LITERAL__stdcall:
						case LITERAL___stdcall:
						{
							init_declarator_list();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							break;
						}
						case SEMICOLON:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						match(SEMICOLON);
						if ( inputState->guessing==0 ) {
							end_of_stmt();
						}
						if ( inputState->guessing==0 ) {
							external_declaration_AST = RefPNode(currentAST.root);
							external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
							currentAST.root = external_declaration_AST;
							if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
								external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
								  currentAST.child = external_declaration_AST->getFirstChild();
							else
								currentAST.child = external_declaration_AST;
							currentAST.advanceChildToEnd();
						}
					}
					else {
						bool synPredMatched15 = false;
						if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
							int _m15 = mark();
							synPredMatched15 = true;
							inputState->guessing++;
							try {
								{
								declaration_specifiers();
								function_declarator(0);
								match(SEMICOLON);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched15 = false;
							}
							rewind(_m15);
							inputState->guessing--;
						}
						if ( synPredMatched15 ) {
							if ( inputState->guessing==0 ) {
								if(statementTrace>=1) 
												OUTPUT_DEBUG("%d external_declaration Typedef function type\n",LT(1)->getLine());
											
							}
							function_declaration();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
						}
						else {
							bool synPredMatched18 = false;
							if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
								int _m18 = mark();
								synPredMatched18 = true;
								inputState->guessing++;
								try {
									{
									declaration_specifiers();
									{
									switch ( LA(1)) {
									case ID:
									case COLON:
									case LPAREN:
									case OPERATOR:
									case TILDE:
									case LITERAL_typeid:
									case STAR:
									case AMPERSAND:
									case SCOPE:
									case LITERAL__cdecl:
									case LITERAL___cdecl:
									case LITERAL__near:
									case LITERAL___near:
									case LITERAL__far:
									case LITERAL___far:
									case LITERAL___interrupt:
									case LITERAL_pascal:
									case LITERAL__pascal:
									case LITERAL___pascal:
									case LITERAL__stdcall:
									case LITERAL___stdcall:
									{
										init_declarator_list();
										break;
									}
									case SEMICOLON:
									{
										break;
									}
									default:
									{
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									}
									}
									match(SEMICOLON);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched18 = false;
								}
								rewind(_m18);
								inputState->guessing--;
							}
							if ( synPredMatched18 ) {
								if ( inputState->guessing==0 ) {
									if(statementTrace>=1) 
													OUTPUT_DEBUG("%d external_declaration Typedef variable type\n",LT(1)->getLine());
												
								}
								declaration();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else {
								bool synPredMatched20 = false;
								if (((LA(1) == LITERAL_typedef) && ((LA(2) >= LITERAL_struct && LA(2) <= LITERAL_class)))) {
									int _m20 = mark();
									synPredMatched20 = true;
									inputState->guessing++;
									try {
										{
										match(LITERAL_typedef);
										class_specifier();
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched20 = false;
									}
									rewind(_m20);
									inputState->guessing--;
								}
								if ( synPredMatched20 ) {
									if ( inputState->guessing==0 ) {
										if(statementTrace>=1) 
														OUTPUT_DEBUG("%d external_declaration Typedef class type\n",LT(1)->getLine());
													
									}
									match(LITERAL_typedef);
									class_decl_or_def(fs);
									if (inputState->guessing==0) {
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
									}
									if ( inputState->guessing==0 ) {
										_td = true;
									}
									{
									switch ( LA(1)) {
									case ID:
									case COLON:
									case LPAREN:
									case OPERATOR:
									case TILDE:
									case LITERAL_typeid:
									case STAR:
									case AMPERSAND:
									case SCOPE:
									case LITERAL__cdecl:
									case LITERAL___cdecl:
									case LITERAL__near:
									case LITERAL___near:
									case LITERAL__far:
									case LITERAL___far:
									case LITERAL___interrupt:
									case LITERAL_pascal:
									case LITERAL__pascal:
									case LITERAL___pascal:
									case LITERAL__stdcall:
									case LITERAL___stdcall:
									{
										init_declarator_list();
										if (inputState->guessing==0) {
											astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
										}
										break;
									}
									case SEMICOLON:
									{
										break;
									}
									default:
									{
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									}
									}
									match(SEMICOLON);
									if ( inputState->guessing==0 ) {
										end_of_stmt();
									}
									if ( inputState->guessing==0 ) {
										external_declaration_AST = RefPNode(currentAST.root);
										external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
										currentAST.root = external_declaration_AST;
										if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
											external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
											  currentAST.child = external_declaration_AST->getFirstChild();
										else
											currentAST.child = external_declaration_AST;
										currentAST.advanceChildToEnd();
									}
								}
					else {
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}}}
					}
				}
				else {
					bool synPredMatched25 = false;
					if (((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN))) {
						int _m25 = mark();
						synPredMatched25 = true;
						inputState->guessing++;
						try {
							{
							template_head();
							{ // ( ... )*
							for (;;) {
								if ((_tokenSet_6.member(LA(1)))) {
									fs=function_specifier();
								}
								else {
									goto _loop24;
								}
								
							}
							_loop24:;
							} // ( ... )*
							class_specifier();
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched25 = false;
						}
						rewind(_m25);
						inputState->guessing--;
					}
					if ( synPredMatched25 ) {
						if ( inputState->guessing==0 ) {
							if (statementTrace>=1) 
										OUTPUT_DEBUG("%d external_declaration Templated class decl or def\n",LT(1)->getLine());
									
						}
						template_head();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						{ // ( ... )*
						for (;;) {
							if ((_tokenSet_6.member(LA(1)))) {
								fs=function_specifier();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else {
								goto _loop27;
							}
							
						}
						_loop27:;
						} // ( ... )*
						class_decl_or_def(fs);
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						{
						switch ( LA(1)) {
						case ID:
						case COLON:
						case LPAREN:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case STAR:
						case AMPERSAND:
						case SCOPE:
						case LITERAL__cdecl:
						case LITERAL___cdecl:
						case LITERAL__near:
						case LITERAL___near:
						case LITERAL__far:
						case LITERAL___far:
						case LITERAL___interrupt:
						case LITERAL_pascal:
						case LITERAL__pascal:
						case LITERAL___pascal:
						case LITERAL__stdcall:
						case LITERAL___stdcall:
						{
							init_declarator_list();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							break;
						}
						case SEMICOLON:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						match(SEMICOLON);
						if ( inputState->guessing==0 ) {
							end_of_stmt();
						}
						if ( inputState->guessing==0 ) {
							external_declaration_AST = RefPNode(currentAST.root);
							external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTEMPLATE,"templatepro")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
							currentAST.root = external_declaration_AST;
							if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
								external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
								  currentAST.child = external_declaration_AST->getFirstChild();
							else
								currentAST.child = external_declaration_AST;
							currentAST.advanceChildToEnd();
						}
					}
					else {
						bool synPredMatched31 = false;
						if (((LA(1) == LITERAL_enum) && (_tokenSet_7.member(LA(2))))) {
							int _m31 = mark();
							synPredMatched31 = true;
							inputState->guessing++;
							try {
								{
								match(LITERAL_enum);
								{
								switch ( LA(1)) {
								case ID:
								{
									match(ID);
									break;
								}
								case LCURLY:
								{
									break;
								}
								default:
								{
									throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
								}
								}
								}
								match(LCURLY);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched31 = false;
							}
							rewind(_m31);
							inputState->guessing--;
						}
						if ( synPredMatched31 ) {
							if ( inputState->guessing==0 ) {
								if (statementTrace>=1) 
											OUTPUT_DEBUG("%d external_declaration Enum definition\n",LT(1)->getLine());
										
							}
							enum_specifier();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							{
							switch ( LA(1)) {
							case ID:
							case COLON:
							case LPAREN:
							case OPERATOR:
							case TILDE:
							case LITERAL_typeid:
							case STAR:
							case AMPERSAND:
							case SCOPE:
							case LITERAL__cdecl:
							case LITERAL___cdecl:
							case LITERAL__near:
							case LITERAL___near:
							case LITERAL__far:
							case LITERAL___far:
							case LITERAL___interrupt:
							case LITERAL_pascal:
							case LITERAL__pascal:
							case LITERAL___pascal:
							case LITERAL__stdcall:
							case LITERAL___stdcall:
							{
								init_declarator_list();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								break;
							}
							case SEMICOLON:
							{
								break;
							}
							default:
							{
								throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
							}
							}
							}
							match(SEMICOLON);
							if ( inputState->guessing==0 ) {
								end_of_stmt();
							}
							if ( inputState->guessing==0 ) {
								external_declaration_AST = RefPNode(currentAST.root);
								external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
								currentAST.root = external_declaration_AST;
								if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
									external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
									  currentAST.child = external_declaration_AST->getFirstChild();
								else
									currentAST.child = external_declaration_AST;
								currentAST.advanceChildToEnd();
							}
						}
						else {
							bool synPredMatched35 = false;
							if (((_tokenSet_8.member(LA(1))) && (_tokenSet_9.member(LA(2))))) {
								int _m35 = mark();
								synPredMatched35 = true;
								inputState->guessing++;
								try {
									{
									{
									switch ( LA(1)) {
									case LITERAL_template:
									{
										template_head();
										break;
									}
									case ID:
									case LITERAL_inline:
									case LITERAL__inline:
									case LITERAL___inline:
									case LITERAL_virtual:
									case TILDE:
									case SCOPE:
									{
										break;
									}
									default:
									{
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									}
									}
									dtor_head(1);
									match(LCURLY);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched35 = false;
								}
								rewind(_m35);
								inputState->guessing--;
							}
							if ( synPredMatched35 ) {
								if ( inputState->guessing==0 ) {
									if (statementTrace>=1) 
												OUTPUT_DEBUG("%d external_declaration Destructor definition\n",LT(1)->getLine());
											
								}
								{
								switch ( LA(1)) {
								case LITERAL_template:
								{
									template_head();
									if (inputState->guessing==0) {
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
									}
									break;
								}
								case ID:
								case LITERAL_inline:
								case LITERAL__inline:
								case LITERAL___inline:
								case LITERAL_virtual:
								case TILDE:
								case SCOPE:
								{
									break;
								}
								default:
								{
									throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
								}
								}
								}
								dtor_definition();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								if ( inputState->guessing==0 ) {
									external_declaration_AST = RefPNode(currentAST.root);
									external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTEMPLATE,"templatepro")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
									currentAST.root = external_declaration_AST;
									if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
										external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
										  currentAST.child = external_declaration_AST->getFirstChild();
									else
										currentAST.child = external_declaration_AST;
									currentAST.advanceChildToEnd();
								}
							}
							else {
								bool synPredMatched39 = false;
								if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
									int _m39 = mark();
									synPredMatched39 = true;
									inputState->guessing++;
									try {
										{
										{
										if ((true) && (true)) {
											ctor_decl_spec();
										}
										else {
										}
										
										}
										if (!(qualifiedItemIsOneOf(qiCtor)))
											throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched39 = false;
									}
									rewind(_m39);
									inputState->guessing--;
								}
								if ( synPredMatched39 ) {
									if ( inputState->guessing==0 ) {
										if (statementTrace>=1) 
													OUTPUT_DEBUG("%d external_declaration Constructor definition\n",LT(1)->getLine());
												
									}
									ctor_definition();
									if (inputState->guessing==0) {
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
									}
								}
								else {
									bool synPredMatched42 = false;
									if (((_tokenSet_12.member(LA(1))) && (_tokenSet_13.member(LA(2))))) {
										int _m42 = mark();
										synPredMatched42 = true;
										inputState->guessing++;
										try {
											{
											{
											switch ( LA(1)) {
											case LITERAL_inline:
											{
												match(LITERAL_inline);
												break;
											}
											case ID:
											case OPERATOR:
											case SCOPE:
											{
												break;
											}
											default:
											{
												throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
											}
											}
											}
											scope_override();
											conversion_function_decl_or_def();
											}
										}
										catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
											synPredMatched42 = false;
										}
										rewind(_m42);
										inputState->guessing--;
									}
									if ( synPredMatched42 ) {
										if ( inputState->guessing==0 ) {
											if (statementTrace>=1) 
														OUTPUT_DEBUG("%d external_declaration Operator function\n",LT(1)->getLine());
													
										}
										{
										switch ( LA(1)) {
										case LITERAL_inline:
										{
											m = LT(1);
											m_AST = astFactory->create(m);
											match(LITERAL_inline);
											break;
										}
										case ID:
										case OPERATOR:
										case SCOPE:
										{
											break;
										}
										default:
										{
											throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
										}
										}
										}
										s=scope_override();
										if (inputState->guessing==0) {
											sc_AST = returnAST;
										}
										conversion_function_decl_or_def();
										if (inputState->guessing==0) {
											fc_AST = returnAST;
										}
										if ( inputState->guessing==0 ) {
											external_declaration_AST = RefPNode(currentAST.root);
											
													 external_declaration_AST = astFactory->create(MYFUNCTION,"convertor");
													 if(m_AST != nullAST)
														external_declaration_AST->addChild(RefPNode(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(m_AST))))));
													 if(sc_AST != nullAST)
														external_declaration_AST->addChild(RefPNode(sc_AST));
													 external_declaration_AST->addChild(RefPNode(fc_AST));
													
											currentAST.root = external_declaration_AST;
											if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
												external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
												  currentAST.child = external_declaration_AST->getFirstChild();
											else
												currentAST.child = external_declaration_AST;
											currentAST.advanceChildToEnd();
										}
									}
									else {
										bool synPredMatched45 = false;
										if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
											int _m45 = mark();
											synPredMatched45 = true;
											inputState->guessing++;
											try {
												{
												declaration_specifiers();
												function_declarator(0);
												match(SEMICOLON);
												}
											}
											catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
												synPredMatched45 = false;
											}
											rewind(_m45);
											inputState->guessing--;
										}
										if ( synPredMatched45 ) {
											if ( inputState->guessing==0 ) {
												if (statementTrace>=1) 
															OUTPUT_DEBUG("%d external_declaration Function declaration\n",LT(1)->getLine());
														
											}
											function_declaration();
											if (inputState->guessing==0) {
												astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
											}
										}
										else {
											bool synPredMatched47 = false;
											if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
												int _m47 = mark();
												synPredMatched47 = true;
												inputState->guessing++;
												try {
													{
													declaration_specifiers();
													function_declarator(1);
													match(LCURLY);
													}
												}
												catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
													synPredMatched47 = false;
												}
												rewind(_m47);
												inputState->guessing--;
											}
											if ( synPredMatched47 ) {
												if ( inputState->guessing==0 ) {
													if (statementTrace>=1) 
																OUTPUT_DEBUG("%d external_declaration Function definition\n",LT(1)->getLine());
															
												}
												function_definition();
												if (inputState->guessing==0) {
													astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
												}
											}
											else {
												bool synPredMatched49 = false;
												if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
													int _m49 = mark();
													synPredMatched49 = true;
													inputState->guessing++;
													try {
														{
														function_declarator(1);
														match(LCURLY);
														}
													}
													catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
														synPredMatched49 = false;
													}
													rewind(_m49);
													inputState->guessing--;
												}
												if ( synPredMatched49 ) {
													if ( inputState->guessing==0 ) {
														if (statementTrace>=1) 
																	OUTPUT_DEBUG("%d external_declaration Function definition without return type\n",LT(1)->getLine());
																
													}
													function_definition();
													if (inputState->guessing==0) {
														astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
													}
												}
												else {
													bool synPredMatched51 = false;
													if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
														int _m51 = mark();
														synPredMatched51 = true;
														inputState->guessing++;
														try {
															{
															declaration_specifiers();
															function_declarator(1);
															declaration();
															}
														}
														catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
															synPredMatched51 = false;
														}
														rewind(_m51);
														inputState->guessing--;
													}
													if ( synPredMatched51 ) {
														if ( inputState->guessing==0 ) {
															K_and_R = true;
																	 if (statementTrace>=1) 
																		OUTPUT_DEBUG("%d external_declaration K & R function definition\n",LT(1)->getLine());
																	
														}
														function_definition();
														if (inputState->guessing==0) {
															astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
														}
													}
													else {
														bool synPredMatched53 = false;
														if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
															int _m53 = mark();
															synPredMatched53 = true;
															inputState->guessing++;
															try {
																{
																function_declarator(1);
																declaration();
																}
															}
															catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																synPredMatched53 = false;
															}
															rewind(_m53);
															inputState->guessing--;
														}
														if ( synPredMatched53 ) {
															if ( inputState->guessing==0 ) {
																K_and_R = true;
																		 if (statementTrace>=1) 
																			OUTPUT_DEBUG("%d external_declaration K & R function definition without return type\n",LT(1)->getLine());
																		
															}
															function_definition();
															if (inputState->guessing==0) {
																astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
															}
														}
														else {
															bool synPredMatched58 = false;
															if (((_tokenSet_16.member(LA(1))) && (_tokenSet_17.member(LA(2))))) {
																int _m58 = mark();
																synPredMatched58 = true;
																inputState->guessing++;
																try {
																	{
																	{
																	switch ( LA(1)) {
																	case LITERAL_extern:
																	{
																		match(LITERAL_extern);
																		break;
																	}
																	case LITERAL_inline:
																	case LITERAL_struct:
																	case LITERAL_union:
																	case LITERAL_class:
																	case LITERAL__inline:
																	case LITERAL___inline:
																	case LITERAL_virtual:
																	case LITERAL_explicit:
																	{
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																	}
																	}
																	}
																	{ // ( ... )*
																	for (;;) {
																		if ((_tokenSet_6.member(LA(1)))) {
																			fs=function_specifier();
																		}
																		else {
																			goto _loop57;
																		}
																		
																	}
																	_loop57:;
																	} // ( ... )*
																	class_specifier();
																	}
																}
																catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																	synPredMatched58 = false;
																}
																rewind(_m58);
																inputState->guessing--;
															}
															if ( synPredMatched58 ) {
																if ( inputState->guessing==0 ) {
																	if (statementTrace>=1) 
																				OUTPUT_DEBUG("%d external_declaration Class decl or def\n",LT(1)->getLine());
																			
																}
																{
																switch ( LA(1)) {
																case LITERAL_extern:
																{
																	match(LITERAL_extern);
																	break;
																}
																case LITERAL_inline:
																case LITERAL_struct:
																case LITERAL_union:
																case LITERAL_class:
																case LITERAL__inline:
																case LITERAL___inline:
																case LITERAL_virtual:
																case LITERAL_explicit:
																{
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																}
																}
																}
																{ // ( ... )*
																for (;;) {
																	if ((_tokenSet_6.member(LA(1)))) {
																		fs=function_specifier();
																		if (inputState->guessing==0) {
																			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																		}
																	}
																	else {
																		goto _loop61;
																	}
																	
																}
																_loop61:;
																} // ( ... )*
																class_decl_or_def(fs);
																if (inputState->guessing==0) {
																	astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																}
																{
																switch ( LA(1)) {
																case ID:
																case COLON:
																case LPAREN:
																case OPERATOR:
																case TILDE:
																case LITERAL_typeid:
																case STAR:
																case AMPERSAND:
																case SCOPE:
																case LITERAL__cdecl:
																case LITERAL___cdecl:
																case LITERAL__near:
																case LITERAL___near:
																case LITERAL__far:
																case LITERAL___far:
																case LITERAL___interrupt:
																case LITERAL_pascal:
																case LITERAL__pascal:
																case LITERAL___pascal:
																case LITERAL__stdcall:
																case LITERAL___stdcall:
																{
																	init_declarator_list();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																	break;
																}
																case SEMICOLON:
																{
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																}
																}
																}
																match(SEMICOLON);
																if ( inputState->guessing==0 ) {
																	end_of_stmt();
																}
																if ( inputState->guessing==0 ) {
																	external_declaration_AST = RefPNode(currentAST.root);
																	external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
																	currentAST.root = external_declaration_AST;
																	if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																		external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																		  currentAST.child = external_declaration_AST->getFirstChild();
																	else
																		currentAST.child = external_declaration_AST;
																	currentAST.advanceChildToEnd();
																}
															}
															else {
																bool synPredMatched65 = false;
																if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
																	int _m65 = mark();
																	synPredMatched65 = true;
																	inputState->guessing++;
																	try {
																		{
																		declaration_specifiers();
																		{
																		switch ( LA(1)) {
																		case ID:
																		case COLON:
																		case LPAREN:
																		case OPERATOR:
																		case TILDE:
																		case LITERAL_typeid:
																		case STAR:
																		case AMPERSAND:
																		case SCOPE:
																		case LITERAL__cdecl:
																		case LITERAL___cdecl:
																		case LITERAL__near:
																		case LITERAL___near:
																		case LITERAL__far:
																		case LITERAL___far:
																		case LITERAL___interrupt:
																		case LITERAL_pascal:
																		case LITERAL__pascal:
																		case LITERAL___pascal:
																		case LITERAL__stdcall:
																		case LITERAL___stdcall:
																		{
																			init_declarator_list();
																			break;
																		}
																		case SEMICOLON:
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																		}
																		}
																		}
																		match(SEMICOLON);
																		}
																	}
																	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																		synPredMatched65 = false;
																	}
																	rewind(_m65);
																	inputState->guessing--;
																}
																if ( synPredMatched65 ) {
																	if ( inputState->guessing==0 ) {
																		if (statementTrace>=1) 
																					OUTPUT_DEBUG("%d external_declaration Declaration\n",LT(1)->getLine());
																				
																	}
																	declaration();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																}
																else if ((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN)) {
																	if ( inputState->guessing==0 ) {
																		beginTemplateDeclaration();
																	}
																	template_head();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																	{
																	bool synPredMatched69 = false;
																	if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
																		int _m69 = mark();
																		synPredMatched69 = true;
																		inputState->guessing++;
																		try {
																			{
																			declaration_specifiers();
																			{
																			switch ( LA(1)) {
																			case ID:
																			case COLON:
																			case LPAREN:
																			case OPERATOR:
																			case TILDE:
																			case LITERAL_typeid:
																			case STAR:
																			case AMPERSAND:
																			case SCOPE:
																			case LITERAL__cdecl:
																			case LITERAL___cdecl:
																			case LITERAL__near:
																			case LITERAL___near:
																			case LITERAL__far:
																			case LITERAL___far:
																			case LITERAL___interrupt:
																			case LITERAL_pascal:
																			case LITERAL__pascal:
																			case LITERAL___pascal:
																			case LITERAL__stdcall:
																			case LITERAL___stdcall:
																			{
																				init_declarator_list();
																				break;
																			}
																			case SEMICOLON:
																			{
																				break;
																			}
																			default:
																			{
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																			}
																			}
																			}
																			match(SEMICOLON);
																			}
																		}
																		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																			synPredMatched69 = false;
																		}
																		rewind(_m69);
																		inputState->guessing--;
																	}
																	if ( synPredMatched69 ) {
																		if ( inputState->guessing==0 ) {
																			if (statementTrace>=1) 
																							OUTPUT_DEBUG("%d external_declaration Templated class forward declaration\n",LT(1)->getLine());
																						
																		}
																		declaration();
																		if (inputState->guessing==0) {
																			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																		}
																	}
																	else {
																		bool synPredMatched71 = false;
																		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
																			int _m71 = mark();
																			synPredMatched71 = true;
																			inputState->guessing++;
																			try {
																				{
																				declaration_specifiers();
																				function_declarator(0);
																				match(SEMICOLON);
																				}
																			}
																			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																				synPredMatched71 = false;
																			}
																			rewind(_m71);
																			inputState->guessing--;
																		}
																		if ( synPredMatched71 ) {
																			if ( inputState->guessing==0 ) {
																				if (statementTrace>=1) 
																								OUTPUT_DEBUG("%d external_declaration Templated function declaration\n",LT(1)->getLine());
																							
																			}
																			function_declaration();
																			if (inputState->guessing==0) {
																				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																			}
																		}
																		else {
																			bool synPredMatched73 = false;
																			if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
																				int _m73 = mark();
																				synPredMatched73 = true;
																				inputState->guessing++;
																				try {
																					{
																					declaration_specifiers();
																					function_declarator(1);
																					match(LCURLY);
																					}
																				}
																				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																					synPredMatched73 = false;
																				}
																				rewind(_m73);
																				inputState->guessing--;
																			}
																			if ( synPredMatched73 ) {
																				if ( inputState->guessing==0 ) {
																					if (statementTrace>=1) 
																									OUTPUT_DEBUG("%d external_declaration Templated function definition\n",LT(1)->getLine());
																								
																				}
																				function_definition();
																				if (inputState->guessing==0) {
																					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																				}
																			}
																			else {
																				bool synPredMatched75 = false;
																				if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
																					int _m75 = mark();
																					synPredMatched75 = true;
																					inputState->guessing++;
																					try {
																						{
																						ctor_decl_spec();
																						if (!(qualifiedItemIsOneOf(qiCtor)))
																							throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
																						}
																					}
																					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																						synPredMatched75 = false;
																					}
																					rewind(_m75);
																					inputState->guessing--;
																				}
																				if ( synPredMatched75 ) {
																					if ( inputState->guessing==0 ) {
																						if (statementTrace>=1) 
																										OUTPUT_DEBUG("%d external_declaration Templated constructor definition\n",LT(1)->getLine());
																									
																					}
																					ctor_definition();
																					if (inputState->guessing==0) {
																						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																					}
																				}
																	else {
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																	}
																	}}}
																	}
																	if ( inputState->guessing==0 ) {
																		endTemplateDeclaration();
																	}
																	if ( inputState->guessing==0 ) {
																		external_declaration_AST = RefPNode(currentAST.root);
																		external_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTEMPLATE,"templatepro")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(external_declaration_AST))));
																		currentAST.root = external_declaration_AST;
																		if ( external_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																			external_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																			  currentAST.child = external_declaration_AST->getFirstChild();
																		else
																			currentAST.child = external_declaration_AST;
																		currentAST.advanceChildToEnd();
																	}
																}
																else if ((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2)))) {
																	if ( inputState->guessing==0 ) {
																		if (statementTrace>=1) 
																					OUTPUT_DEBUG("%d external_declaration Other Declaration\n",LT(1)->getLine());
																				
																	}
																	declaration();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}}}}}}}}}}}}}}
		}
		external_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_18);
		} else {
			throw;
		}
	}
	returnAST = external_declaration_AST;
}

void CPPParser::enum_specifier() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode enum_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data id;
		
	
	try {      // for error handling
		match(LITERAL_enum);
		{
		switch ( LA(1)) {
		case LCURLY:
		{
			match(LCURLY);
			enumerator_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(RCURLY);
			break;
		}
		case ID:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case SCOPE:
		{
			id=qualified_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				beginEnumDefinition(id.c_str());
			}
			{
			switch ( LA(1)) {
			case LCURLY:
			{
				match(LCURLY);
				enumerator_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RCURLY);
				break;
			}
			case SEMICOLON:
			case ID:
			case COLON:
			case LPAREN:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case SCOPE:
			case LITERAL__cdecl:
			case LITERAL___cdecl:
			case LITERAL__near:
			case LITERAL___near:
			case LITERAL__far:
			case LITERAL___far:
			case LITERAL___interrupt:
			case LITERAL_pascal:
			case LITERAL__pascal:
			case LITERAL___pascal:
			case LITERAL__stdcall:
			case LITERAL___stdcall:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				endEnumDefinition();
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			enum_specifier_AST = RefPNode(currentAST.root);
			enum_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYENUM,"enum")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(enum_specifier_AST))));
			currentAST.root = enum_specifier_AST;
			if ( enum_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				enum_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = enum_specifier_AST->getFirstChild();
			else
				currentAST.child = enum_specifier_AST;
			currentAST.advanceChildToEnd();
		}
		enum_specifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_19);
		} else {
			throw;
		}
	}
	returnAST = enum_specifier_AST;
}

void CPPParser::init_declarator_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode init_declarator_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		member_declarator();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				member_declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop258;
			}
			
		}
		_loop258:;
		} // ( ... )*
		init_declarator_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_20);
		} else {
			throw;
		}
	}
	returnAST = init_declarator_list_AST;
}

void CPPParser::declaration_specifiers() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode declaration_specifiers_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	// Locals
		bool td = false;	// For typedef
		bool fd = false;	// For friend
		StorageClass sc = scInvalid;	// auto,register,static,extern,mutable
		TypeQualifier tq = tqInvalid;	// const,volatile	// aka cv_qualifier See type_qualifier
		TypeSpecifier ts = tsInvalid;	// char,int,double, etc., class,struct,union
		FunctionSpecifier fs = fsInvalid;	// inline,virtual,explicit
		
	
	try {      // for error handling
		if ( inputState->guessing==0 ) {
			
				// Global flags to allow for nested declarations
				_td = false;		// For typedef
				_fd = false;		// For friend
				_sc = scInvalid;	// For StorageClass		// auto,register,static,extern,mutable
				_tq = tqInvalid;	// For TypeQualifier	// aka cv_qualifier See type_qualifier
				_ts = tsInvalid;	// For TypeSpecifier
				_fs = fsInvalid;	// For FunctionSpecifier	// inline,virtual,explicit
			
				
		}
		{
		specifier_prefix(td, fd, sc,tq,fs);
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ts=type_specifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile)) {
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop192;
			}
			
		}
		_loop192:;
		} // ( ... )*
		}
		if ( inputState->guessing==0 ) {
			declarationSpecifier(td,fd,sc,tq,ts,fs);
		}
		declaration_specifiers_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_21);
		} else {
			throw;
		}
	}
	returnAST = declaration_specifiers_AST;
}

void CPPParser::function_declarator(
	int definition
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		bool synPredMatched310 = false;
		if (((_tokenSet_22.member(LA(1))) && (_tokenSet_23.member(LA(2))))) {
			int _m310 = mark();
			synPredMatched310 = true;
			inputState->guessing++;
			try {
				{
				ptr_operator();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched310 = false;
			}
			rewind(_m310);
			inputState->guessing--;
		}
		if ( synPredMatched310 ) {
			ptr_operator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			function_declarator(definition);
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			function_declarator_AST = RefPNode(currentAST.root);
		}
		else if ((_tokenSet_24.member(LA(1))) && (_tokenSet_25.member(LA(2)))) {
			function_direct_declarator(definition);
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			function_declarator_AST = RefPNode(currentAST.root);
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_26);
		} else {
			throw;
		}
	}
	returnAST = function_declarator_AST;
}

void CPPParser::function_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		bool bTypeDef = false;
		
	
	try {      // for error handling
		declaration_specifiers();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			bTypeDef=_td;
		}
		function_declarator(0);
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(SEMICOLON);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
		}
		if ( inputState->guessing==0 ) {
			function_declaration_AST = RefPNode(currentAST.root);
			
				 if(bTypeDef)
					function_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(function_declaration_AST))))))));
				 else
					function_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(function_declaration_AST))));
				
			currentAST.root = function_declaration_AST;
			if ( function_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				function_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = function_declaration_AST->getFirstChild();
			else
				currentAST.child = function_declaration_AST;
			currentAST.advanceChildToEnd();
		}
		function_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = function_declaration_AST;
}

void CPPParser::declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		bool synPredMatched179 = false;
		if (((LA(1) == LITERAL_extern) && (LA(2) == StringLiteral))) {
			int _m179 = mark();
			synPredMatched179 = true;
			inputState->guessing++;
			try {
				{
				match(LITERAL_extern);
				match(StringLiteral);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched179 = false;
			}
			rewind(_m179);
			inputState->guessing--;
		}
		if ( synPredMatched179 ) {
			linkage_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((_tokenSet_4.member(LA(1))) && (_tokenSet_28.member(LA(2)))) {
			simple_declaration();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((LA(1) == LITERAL_using)) {
			using_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		if ( inputState->guessing==0 ) {
			declaration_AST = RefPNode(currentAST.root);
			
				 if(_td==true)
					declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(declaration_AST))));
				 else
					declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(declaration_AST))));
				
			currentAST.root = declaration_AST;
			if ( declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = declaration_AST->getFirstChild();
			else
				currentAST.child = declaration_AST;
			currentAST.advanceChildToEnd();
		}
		declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = declaration_AST;
}

CPPParser::TypeSpecifier  CPPParser::class_specifier() {
	CPPParser::TypeSpecifier ts = tsInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode class_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_class:
		{
			RefPNode tmp24_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp24_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp24_AST));
			}
			match(LITERAL_class);
			if ( inputState->guessing==0 ) {
				ts = tsCLASS;
			}
			break;
		}
		case LITERAL_struct:
		{
			RefPNode tmp25_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp25_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp25_AST));
			}
			match(LITERAL_struct);
			if ( inputState->guessing==0 ) {
				ts = tsSTRUCT;
			}
			break;
		}
		case LITERAL_union:
		{
			RefPNode tmp26_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp26_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp26_AST));
			}
			match(LITERAL_union);
			if ( inputState->guessing==0 ) {
				ts = tsUNION;
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		class_specifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_29);
		} else {
			throw;
		}
	}
	returnAST = class_specifier_AST;
	return ts;
}

void CPPParser::class_decl_or_def(
	FunctionSpecifier fs
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode class_decl_or_def_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	char *saveClass; 
		 data id;
		 data typeName;
		 char qid[CPPParser_MaxQualifiedItemSize+1];
		 TypeSpecifier ts = tsInvalid;	// Available for use
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_class:
		{
			match(LITERAL_class);
			if ( inputState->guessing==0 ) {
				ts = tsCLASS;typeName="class";
			}
			break;
		}
		case LITERAL_struct:
		{
			match(LITERAL_struct);
			if ( inputState->guessing==0 ) {
				ts = tsSTRUCT;typeName="class";
			}
			break;
		}
		case LITERAL_union:
		{
			match(LITERAL_union);
			if ( inputState->guessing==0 ) {
				ts = tsUNION;typeName="union";
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		class_prefix();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case ID:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case SCOPE:
		{
			id=qualified_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				strcpy(qid,id.c_str());
			}
			{
			bool synPredMatched226 = false;
			if (((_tokenSet_19.member(LA(1))) && (_tokenSet_30.member(LA(2))))) {
				int _m226 = mark();
				synPredMatched226 = true;
				inputState->guessing++;
				try {
					{
					switch ( LA(1)) {
					case SEMICOLON:
					{
						match(SEMICOLON);
						break;
					}
					case ID:
					case COLON:
					case LPAREN:
					case OPERATOR:
					case TILDE:
					case LITERAL_typeid:
					case STAR:
					case AMPERSAND:
					case SCOPE:
					case LITERAL__cdecl:
					case LITERAL___cdecl:
					case LITERAL__near:
					case LITERAL___near:
					case LITERAL__far:
					case LITERAL___far:
					case LITERAL___interrupt:
					case LITERAL_pascal:
					case LITERAL__pascal:
					case LITERAL___pascal:
					case LITERAL__stdcall:
					case LITERAL___stdcall:
					{
						member_declarator();
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched226 = false;
				}
				rewind(_m226);
				inputState->guessing--;
			}
			if ( synPredMatched226 ) {
				if ( inputState->guessing==0 ) {
					classForwardDeclaration(qid, ts, fs);
				}
			}
			else if ((LA(1) == LCURLY || LA(1) == COLON) && (_tokenSet_31.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case COLON:
				{
					base_clause();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case LCURLY:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(LCURLY);
				if ( inputState->guessing==0 ) {
					saveClass = enclosingClass; enclosingClass = symbols->strdup(qid);
									 beginClassDefinition(qid, ts);
				}
				{ // ( ... )*
				for (;;) {
					if ((_tokenSet_32.member(LA(1)))) {
						member_declaration();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
					}
					else {
						goto _loop229;
					}
					
				}
				_loop229:;
				} // ( ... )*
				if ( inputState->guessing==0 ) {
					endClassDefinition();
				}
				match(RCURLY);
				if ( inputState->guessing==0 ) {
					enclosingClass = saveClass;
				}
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case LCURLY:
		{
			match(LCURLY);
			if ( inputState->guessing==0 ) {
				saveClass = enclosingClass; enclosingClass = "__anonymous";
							 beginClassDefinition("anonymous", ts);
			}
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_32.member(LA(1)))) {
					member_declaration();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					goto _loop231;
				}
				
			}
			_loop231:;
			} // ( ... )*
			if ( inputState->guessing==0 ) {
				endClassDefinition();
			}
			match(RCURLY);
			if ( inputState->guessing==0 ) {
				enclosingClass = saveClass;
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			class_decl_or_def_AST = RefPNode(currentAST.root);
			class_decl_or_def_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYCLASS,typeName.c_str())))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(class_decl_or_def_AST))));
			currentAST.root = class_decl_or_def_AST;
			if ( class_decl_or_def_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				class_decl_or_def_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = class_decl_or_def_AST->getFirstChild();
			else
				currentAST.child = class_decl_or_def_AST;
			currentAST.advanceChildToEnd();
		}
		class_decl_or_def_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_19);
		} else {
			throw;
		}
	}
	returnAST = class_decl_or_def_AST;
}

void CPPParser::template_head() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_head_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LITERAL_template);
		match(LESSTHAN);
		template_parameter_list();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(GREATERTHAN);
		if ( inputState->guessing==0 ) {
			template_head_AST = RefPNode(currentAST.root);
			template_head_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"templatedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(template_head_AST))));
			currentAST.root = template_head_AST;
			if ( template_head_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				template_head_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = template_head_AST->getFirstChild();
			else
				currentAST.child = template_head_AST;
			currentAST.advanceChildToEnd();
		}
		template_head_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_33);
		} else {
			throw;
		}
	}
	returnAST = template_head_AST;
}

CPPParser::FunctionSpecifier  CPPParser::function_specifier() {
	CPPParser::FunctionSpecifier fs = fsInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_inline:
		case LITERAL__inline:
		case LITERAL___inline:
		{
			{
			switch ( LA(1)) {
			case LITERAL_inline:
			{
				RefPNode tmp37_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp37_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp37_AST));
				}
				match(LITERAL_inline);
				break;
			}
			case LITERAL__inline:
			{
				RefPNode tmp38_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp38_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp38_AST));
				}
				match(LITERAL__inline);
				break;
			}
			case LITERAL___inline:
			{
				RefPNode tmp39_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp39_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp39_AST));
				}
				match(LITERAL___inline);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				fs = fsINLINE;
			}
			function_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_virtual:
		{
			RefPNode tmp40_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp40_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp40_AST));
			}
			match(LITERAL_virtual);
			if ( inputState->guessing==0 ) {
				fs = fsVIRTUAL;
			}
			function_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_explicit:
		{
			RefPNode tmp41_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp41_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp41_AST));
			}
			match(LITERAL_explicit);
			if ( inputState->guessing==0 ) {
				fs = fsEXPLICIT;
			}
			if ( inputState->guessing==0 ) {
				function_specifier_AST = RefPNode(currentAST.root);
				function_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(function_specifier_AST))));
				currentAST.root = function_specifier_AST;
				if ( function_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					function_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = function_specifier_AST->getFirstChild();
				else
					currentAST.child = function_specifier_AST;
				currentAST.advanceChildToEnd();
			}
			function_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_14);
		} else {
			throw;
		}
	}
	returnAST = function_specifier_AST;
	return fs;
}

void CPPParser::dtor_head(
	int definition
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dtor_head_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		dtor_decl_spec();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		dtor_declarator(definition);
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		dtor_head_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_34);
		} else {
			throw;
		}
	}
	returnAST = dtor_head_AST;
}

void CPPParser::dtor_definition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dtor_definition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		dtor_head(1);
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		dtor_body();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			dtor_definition_AST = RefPNode(currentAST.root);
			dtor_definition_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"dtor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(dtor_definition_AST))));
			currentAST.root = dtor_definition_AST;
			if ( dtor_definition_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				dtor_definition_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = dtor_definition_AST->getFirstChild();
			else
				currentAST.child = dtor_definition_AST;
			currentAST.advanceChildToEnd();
		}
		dtor_definition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_18);
		} else {
			throw;
		}
	}
	returnAST = dtor_definition_AST;
}

void CPPParser::ctor_decl_spec() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_decl_spec_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			switch ( LA(1)) {
			case LITERAL_inline:
			case LITERAL__inline:
			case LITERAL___inline:
			{
				{
				switch ( LA(1)) {
				case LITERAL_inline:
				{
					RefPNode tmp42_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp42_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp42_AST));
					}
					match(LITERAL_inline);
					break;
				}
				case LITERAL__inline:
				{
					RefPNode tmp43_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp43_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp43_AST));
					}
					match(LITERAL__inline);
					break;
				}
				case LITERAL___inline:
				{
					RefPNode tmp44_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp44_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp44_AST));
					}
					match(LITERAL___inline);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_explicit:
			{
				RefPNode tmp45_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp45_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp45_AST));
				}
				match(LITERAL_explicit);
				break;
			}
			default:
			{
				goto _loop329;
			}
			}
		}
		_loop329:;
		} // ( ... )*
		ctor_decl_spec_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_29);
		} else {
			throw;
		}
	}
	returnAST = ctor_decl_spec_AST;
}

void CPPParser::ctor_definition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_definition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ctor_head();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ctor_body();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			endConstructorDefinition();
		}
		if ( inputState->guessing==0 ) {
			ctor_definition_AST = RefPNode(currentAST.root);
			ctor_definition_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"ctor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ctor_definition_AST))));
			currentAST.root = ctor_definition_AST;
			if ( ctor_definition_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				ctor_definition_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = ctor_definition_AST->getFirstChild();
			else
				currentAST.child = ctor_definition_AST;
			currentAST.advanceChildToEnd();
		}
		ctor_definition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = ctor_definition_AST;
}

data  CPPParser::scope_override() {
	data so;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode scope_override_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id1_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  tp1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode tp1_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  tp2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode tp2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		char sitem[CPPParser_MaxQualifiedItemSize+1];
		// The above statement must be non static because its calling is nested
		sitem[0] = '\0';
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case SCOPE:
		{
			RefPNode tmp46_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp46_AST = astFactory->create(LT(1));
			}
			match(SCOPE);
			if ( inputState->guessing==0 ) {
				strcat(sitem,"::");
			}
			break;
		}
		case ID:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{ // ( ... )*
		for (;;) {
			bool synPredMatched608 = false;
			if (((LA(1) == ID) && (LA(2) == LESSTHAN))) {
				int _m608 = mark();
				synPredMatched608 = true;
				inputState->guessing++;
				try {
					{
					match(ID);
					match(LESSTHAN);
					template_argument_list();
					match(GREATERTHAN);
					match(SCOPE);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched608 = false;
				}
				rewind(_m608);
				inputState->guessing--;
			}
			if ( synPredMatched608 ) {
				id1 = LT(1);
				if ( inputState->guessing == 0 ) {
					id1_AST = astFactory->create(id1);
				}
				match(ID);
				if ( inputState->guessing==0 ) {
					strcat(sitem,(id1->getText()).data());
				}
				RefPNode tmp47_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp47_AST = astFactory->create(LT(1));
				}
				match(LESSTHAN);
				template_argument_list();
				RefPNode tmp48_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp48_AST = astFactory->create(LT(1));
				}
				match(GREATERTHAN);
				if ( inputState->guessing==0 ) {
					id1_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id1_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"templatelist")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))))))));
				}
				RefPNode tmp49_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp49_AST = astFactory->create(LT(1));
				}
				match(SCOPE);
				if ( inputState->guessing==0 ) {
					strcat(sitem,"::");
				}
				{
				switch ( LA(1)) {
				case LITERAL_template:
				{
					tp1 = LT(1);
					tp1_AST = astFactory->create(tp1);
					match(LITERAL_template);
					if ( inputState->guessing==0 ) {
						strcat(sitem,"template");id1_AST->getFirstChild()->setNextSibling(ANTLR_USE_NAMESPACE(antlr)RefAST(tp1_AST));
					}
					break;
				}
				case ID:
				case OPERATOR:
				case TILDE:
				case LITERAL_typeid:
				case STAR:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				if ( inputState->guessing==0 ) {
					scope_override_AST = RefPNode(currentAST.root);
					
								 if(scope_override_AST == nullAST)	scope_override_AST=astFactory->create(MY,"scope");
								 scope_override_AST->addChild(id1_AST);
								
					currentAST.root = scope_override_AST;
					if ( scope_override_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						scope_override_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = scope_override_AST->getFirstChild();
					else
						currentAST.child = scope_override_AST;
					currentAST.advanceChildToEnd();
				}
			}
			else if ((LA(1) == ID) && (LA(2) == SCOPE)) {
				id2 = LT(1);
				if ( inputState->guessing == 0 ) {
					id2_AST = astFactory->create(id2);
				}
				match(ID);
				if ( inputState->guessing==0 ) {
					strcat(sitem,(id2->getText()).data());
				}
				RefPNode tmp50_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp50_AST = astFactory->create(LT(1));
				}
				match(SCOPE);
				if ( inputState->guessing==0 ) {
					strcat(sitem,"::");
				}
				{
				switch ( LA(1)) {
				case LITERAL_template:
				{
					tp2 = LT(1);
					tp2_AST = astFactory->create(tp2);
					match(LITERAL_template);
					if ( inputState->guessing==0 ) {
						strcat(sitem,"template");id2_AST->addChild(tp2_AST);
					}
					break;
				}
				case ID:
				case OPERATOR:
				case TILDE:
				case LITERAL_typeid:
				case STAR:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				if ( inputState->guessing==0 ) {
					scope_override_AST = RefPNode(currentAST.root);
					
								 if(scope_override_AST == nullAST)	scope_override_AST=astFactory->create(MY,"scope");
								 scope_override_AST->addChild(id2_AST);
								
					currentAST.root = scope_override_AST;
					if ( scope_override_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						scope_override_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = scope_override_AST->getFirstChild();
					else
						currentAST.child = scope_override_AST;
					currentAST.advanceChildToEnd();
				}
			}
			else {
				goto _loop611;
			}
			
		}
		_loop611:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			
					 so = sitem;
					
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_35);
		} else {
			throw;
		}
	}
	returnAST = scope_override_AST;
	return so;
}

void CPPParser::conversion_function_decl_or_def() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode conversion_function_decl_or_def_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	CPPParser::TypeQualifier tq;
	
	try {      // for error handling
		match(OPERATOR);
		declaration_specifiers();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		conversion_type_modifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case LESSTHAN:
		{
			match(LESSTHAN);
			template_parameter_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(GREATERTHAN);
			break;
		}
		case LPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case ELLIPSIS:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			parameter_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile)) {
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop302;
			}
			
		}
		_loop302:;
		} // ( ... )*
		{
		switch ( LA(1)) {
		case LITERAL_throw:
		{
			exception_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		case LCURLY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LCURLY:
		{
			compound_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		{
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		conversion_function_decl_or_def_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = conversion_function_decl_or_def_AST;
}

void CPPParser::function_definition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_definition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))&&(( !(LA(1)==SCOPE||LA(1)==ID) || qualifiedItemIsOneOf(qiType|qiCtor) ))) {
			declaration_specifiers();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			function_declarator(1);
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			if ((_tokenSet_36.member(LA(1))) && (_tokenSet_37.member(LA(2)))) {
				{ // ( ... )*
				for (;;) {
					if ((_tokenSet_2.member(LA(1)))) {
						if ( inputState->guessing==0 ) {
							lineNo = LT(1)->getLine();
						}
						declaration();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
					}
					else {
						goto _loop172;
					}
					
				}
				_loop172:;
				} // ( ... )*
				if ( inputState->guessing==0 ) {
					in_parameter_list = false;
				}
			}
			else if ((LA(1) == LCURLY) && (_tokenSet_38.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			compound_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((_tokenSet_39.member(LA(1))) && (_tokenSet_40.member(LA(2)))) {
			function_declarator(1);
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			if ((_tokenSet_36.member(LA(1))) && (_tokenSet_37.member(LA(2)))) {
				{ // ( ... )*
				for (;;) {
					if ((_tokenSet_2.member(LA(1)))) {
						if ( inputState->guessing==0 ) {
							lineNo = LT(1)->getLine();
						}
						declaration();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
					}
					else {
						goto _loop175;
					}
					
				}
				_loop175:;
				} // ( ... )*
				if ( inputState->guessing==0 ) {
					in_parameter_list = false;
				}
			}
			else if ((LA(1) == LCURLY) && (_tokenSet_38.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			compound_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		if ( inputState->guessing==0 ) {
			function_definition_AST = RefPNode(currentAST.root);
			function_definition_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(function_definition_AST))));
			currentAST.root = function_definition_AST;
			if ( function_definition_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				function_definition_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = function_definition_AST->getFirstChild();
			else
				currentAST.child = function_definition_AST;
			currentAST.advanceChildToEnd();
		}
		if ( inputState->guessing==0 ) {
			endFunctionDefinition();
		}
		function_definition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = function_definition_AST;
}

void CPPParser::namespace_definition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode namespace_definition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  ns = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode ns_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case ID:
		{
			ns = LT(1);
			if ( inputState->guessing == 0 ) {
				ns_AST = astFactory->create(ns);
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(ns_AST));
			}
			match(ID);
			if ( inputState->guessing==0 ) {
				declaratorID((ns->getText()).data(),qiType);
			}
			break;
		}
		case LCURLY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LCURLY);
		if ( inputState->guessing==0 ) {
			enterNewLocalScope();
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				external_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop79;
			}
			
		}
		_loop79:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			exitLocalScope();
		}
		match(RCURLY);
		if ( inputState->guessing==0 ) {
			namespace_definition_AST = RefPNode(currentAST.root);
			namespace_definition_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYNAMESPACE,"namespace")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(namespace_definition_AST))));
			currentAST.root = namespace_definition_AST;
			if ( namespace_definition_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				namespace_definition_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = namespace_definition_AST->getFirstChild();
			else
				currentAST.child = namespace_definition_AST;
			currentAST.advanceChildToEnd();
		}
		namespace_definition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_18);
		} else {
			throw;
		}
	}
	returnAST = namespace_definition_AST;
}

void CPPParser::namespace_alias_definition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode namespace_alias_definition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  ns2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode ns2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data qid;
		
	
	try {      // for error handling
		RefPNode tmp59_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp59_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp59_AST));
		}
		match(LITERAL_namespace);
		ns2 = LT(1);
		if ( inputState->guessing == 0 ) {
			ns2_AST = astFactory->create(ns2);
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(ns2_AST));
		}
		match(ID);
		if ( inputState->guessing==0 ) {
			declaratorID((ns2->getText()).data(),qiType);
		}
		RefPNode tmp60_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp60_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp60_AST));
		}
		match(ASSIGNEQUAL);
		qid=qualified_id();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		RefPNode tmp61_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp61_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp61_AST));
		}
		match(SEMICOLON);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
		}
		namespace_alias_definition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = namespace_alias_definition_AST;
}

data  CPPParser::qualified_id() {
	data qid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode qualified_id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data so = NULL;
		data op = NULL;
		char qitem02[CPPParser_MaxQualifiedItemSize+1];
		qitem02[0] = '\0';
		
	
	try {      // for error handling
		so=scope_override();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			
					 strcpy(qitem02, so.c_str());
					
		}
		{
		switch ( LA(1)) {
		case ID:
		{
			id = LT(1);
			if ( inputState->guessing == 0 ) {
				id_AST = astFactory->create(id);
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST));
			}
			match(ID);
			if ( inputState->guessing==0 ) {
				strcat(qitem02,(id->getText()).data());
			}
			{
			bool synPredMatched254 = false;
			if (((LA(1) == LESSTHAN) && (_tokenSet_42.member(LA(2))))) {
				int _m254 = mark();
				synPredMatched254 = true;
				inputState->guessing++;
				try {
					{
					match(LESSTHAN);
					template_argument_list();
					match(GREATERTHAN);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched254 = false;
				}
				rewind(_m254);
				inputState->guessing--;
			}
			if ( synPredMatched254 ) {
				RefPNode tmp62_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp62_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp62_AST));
				}
				match(LESSTHAN);
				template_argument_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				RefPNode tmp63_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp63_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp63_AST));
				}
				match(GREATERTHAN);
			}
			else if ((_tokenSet_43.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case OPERATOR:
		{
			match(OPERATOR);
			op=optor();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				strcat(qitem02,"operator"); strcat(qitem02,op.c_str());
			}
			break;
		}
		case TILDE:
		{
			RefPNode tmp65_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp65_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp65_AST));
			}
			match(TILDE);
			id_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_typeid:
		{
			RefPNode tmp66_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp66_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp66_AST));
			}
			match(LITERAL_typeid);
			if ( inputState->guessing==0 ) {
				strcat(qitem02, "typeid");
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			
					qid = qitem02;
					
		}
		qualified_id_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_43);
		} else {
			throw;
		}
	}
	returnAST = qualified_id_AST;
	return qid;
}

void CPPParser::member_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode member_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  m = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode m_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode fc_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  mt = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode mt_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode fct_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data q;
		lineNo = LT(1)->getLine();
		FunctionSpecifier fs = fsInvalid;	// inline,virtual,explicit
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_public:
		case LITERAL_protected:
		case LITERAL_private:
		{
			if ( inputState->guessing==0 ) {
				if (statementTrace>=1) 
							OUTPUT_DEBUG("%d member_declaration Access specifier\n",LT(1)->getLine());
						
			}
			access_specifier();
			RefPNode tmp67_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp67_AST = astFactory->create(LT(1));
			}
			match(COLON);
			break;
		}
		case SEMICOLON:
		{
			if ( inputState->guessing==0 ) {
				if (statementTrace>=1) 
							OUTPUT_DEBUG("%d member_declaration Semicolon\n",LT(1)->getLine());
						
			}
			RefPNode tmp68_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp68_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp68_AST));
			}
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		case LITERAL_antlrTrace_on:
		{
			RefPNode tmp69_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp69_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp69_AST));
			}
			match(LITERAL_antlrTrace_on);
			if ( inputState->guessing==0 ) {
				antlrTrace(true);
			}
			break;
		}
		case LITERAL_antlrTrace_off:
		{
			RefPNode tmp70_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp70_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp70_AST));
			}
			match(LITERAL_antlrTrace_off);
			if ( inputState->guessing==0 ) {
				antlrTrace(false);
			}
			break;
		}
		default:
			bool synPredMatched84 = false;
			if (((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN))) {
				int _m84 = mark();
				synPredMatched84 = true;
				inputState->guessing++;
				try {
					{
					match(LITERAL_template);
					match(LESSTHAN);
					match(GREATERTHAN);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched84 = false;
				}
				rewind(_m84);
				inputState->guessing--;
			}
			if ( synPredMatched84 ) {
				if ( inputState->guessing==0 ) {
					if(statementTrace>=1) 
								OUTPUT_DEBUG("%d member_declaration Template explicit-specialisation\n",LT(1)->getLine());
							
				}
				RefPNode tmp71_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp71_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp71_AST));
				}
				match(LITERAL_template);
				RefPNode tmp72_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp72_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp72_AST));
				}
				match(LESSTHAN);
				RefPNode tmp73_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp73_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp73_AST));
				}
				match(GREATERTHAN);
				member_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				bool synPredMatched86 = false;
				if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
					int _m86 = mark();
					synPredMatched86 = true;
					inputState->guessing++;
					try {
						{
						match(LITERAL_typedef);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched86 = false;
					}
					rewind(_m86);
					inputState->guessing--;
				}
				if ( synPredMatched86 ) {
					{
					bool synPredMatched89 = false;
					if (((LA(1) == LITERAL_typedef) && (LA(2) == LITERAL_enum))) {
						int _m89 = mark();
						synPredMatched89 = true;
						inputState->guessing++;
						try {
							{
							match(LITERAL_typedef);
							match(LITERAL_enum);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched89 = false;
						}
						rewind(_m89);
						inputState->guessing--;
					}
					if ( synPredMatched89 ) {
						if ( inputState->guessing==0 ) {
							if(statementTrace>=1) 
											OUTPUT_DEBUG("%d member_declaration Typedef enum type\n",LT(1)->getLine());
										
						}
						RefPNode tmp74_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp74_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp74_AST));
						}
						match(LITERAL_typedef);
						enum_specifier();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						if ( inputState->guessing==0 ) {
							_td = true;
						}
						{
						switch ( LA(1)) {
						case ID:
						case COLON:
						case LPAREN:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case STAR:
						case AMPERSAND:
						case SCOPE:
						case LITERAL__cdecl:
						case LITERAL___cdecl:
						case LITERAL__near:
						case LITERAL___near:
						case LITERAL__far:
						case LITERAL___far:
						case LITERAL___interrupt:
						case LITERAL_pascal:
						case LITERAL__pascal:
						case LITERAL___pascal:
						case LITERAL__stdcall:
						case LITERAL___stdcall:
						{
							init_declarator_list();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							break;
						}
						case SEMICOLON:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						match(SEMICOLON);
						if ( inputState->guessing==0 ) {
							end_of_stmt();
						}
						if ( inputState->guessing==0 ) {
							member_declaration_AST = RefPNode(currentAST.root);
							member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
							currentAST.root = member_declaration_AST;
							if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
								member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
								  currentAST.child = member_declaration_AST->getFirstChild();
							else
								currentAST.child = member_declaration_AST;
							currentAST.advanceChildToEnd();
						}
					}
					else {
						bool synPredMatched92 = false;
						if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
							int _m92 = mark();
							synPredMatched92 = true;
							inputState->guessing++;
							try {
								{
								declaration_specifiers();
								function_declarator(0);
								match(SEMICOLON);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched92 = false;
							}
							rewind(_m92);
							inputState->guessing--;
						}
						if ( synPredMatched92 ) {
							if ( inputState->guessing==0 ) {
								if(statementTrace>=1) 
												OUTPUT_DEBUG("%d member_declaration Typedef function type\n",LT(1)->getLine());
											
							}
							function_declaration();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
						}
						else {
							bool synPredMatched95 = false;
							if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
								int _m95 = mark();
								synPredMatched95 = true;
								inputState->guessing++;
								try {
									{
									declaration_specifiers();
									{
									switch ( LA(1)) {
									case ID:
									case COLON:
									case LPAREN:
									case OPERATOR:
									case TILDE:
									case LITERAL_typeid:
									case STAR:
									case AMPERSAND:
									case SCOPE:
									case LITERAL__cdecl:
									case LITERAL___cdecl:
									case LITERAL__near:
									case LITERAL___near:
									case LITERAL__far:
									case LITERAL___far:
									case LITERAL___interrupt:
									case LITERAL_pascal:
									case LITERAL__pascal:
									case LITERAL___pascal:
									case LITERAL__stdcall:
									case LITERAL___stdcall:
									{
										init_declarator_list();
										break;
									}
									case SEMICOLON:
									{
										break;
									}
									default:
									{
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									}
									}
									match(SEMICOLON);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched95 = false;
								}
								rewind(_m95);
								inputState->guessing--;
							}
							if ( synPredMatched95 ) {
								if ( inputState->guessing==0 ) {
									if(statementTrace>=1) 
													OUTPUT_DEBUG("%d member_declaration Typedef variable type\n",LT(1)->getLine());
												
								}
								declaration();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else {
								bool synPredMatched97 = false;
								if (((LA(1) == LITERAL_typedef) && ((LA(2) >= LITERAL_struct && LA(2) <= LITERAL_class)))) {
									int _m97 = mark();
									synPredMatched97 = true;
									inputState->guessing++;
									try {
										{
										match(LITERAL_typedef);
										class_specifier();
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched97 = false;
									}
									rewind(_m97);
									inputState->guessing--;
								}
								if ( synPredMatched97 ) {
									if ( inputState->guessing==0 ) {
										if(statementTrace>=1) 
														OUTPUT_DEBUG("%d member_declaration Typedef class type\n",LT(1)->getLine());
													
									}
									RefPNode tmp76_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
									if ( inputState->guessing == 0 ) {
										tmp76_AST = astFactory->create(LT(1));
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp76_AST));
									}
									match(LITERAL_typedef);
									class_decl_or_def(fs);
									if (inputState->guessing==0) {
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
									}
									if ( inputState->guessing==0 ) {
										_td = true;
									}
									{
									switch ( LA(1)) {
									case ID:
									case COLON:
									case LPAREN:
									case OPERATOR:
									case TILDE:
									case LITERAL_typeid:
									case STAR:
									case AMPERSAND:
									case SCOPE:
									case LITERAL__cdecl:
									case LITERAL___cdecl:
									case LITERAL__near:
									case LITERAL___near:
									case LITERAL__far:
									case LITERAL___far:
									case LITERAL___interrupt:
									case LITERAL_pascal:
									case LITERAL__pascal:
									case LITERAL___pascal:
									case LITERAL__stdcall:
									case LITERAL___stdcall:
									{
										init_declarator_list();
										if (inputState->guessing==0) {
											astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
										}
										break;
									}
									case SEMICOLON:
									{
										break;
									}
									default:
									{
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									}
									}
									match(SEMICOLON);
									if ( inputState->guessing==0 ) {
										end_of_stmt();
									}
									if ( inputState->guessing==0 ) {
										member_declaration_AST = RefPNode(currentAST.root);
										member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPEDEF,"typedef")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
										currentAST.root = member_declaration_AST;
										if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
											member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
											  currentAST.child = member_declaration_AST->getFirstChild();
										else
											currentAST.child = member_declaration_AST;
										currentAST.advanceChildToEnd();
									}
								}
					else {
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}}}
					}
				}
				else {
					bool synPredMatched102 = false;
					if (((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN))) {
						int _m102 = mark();
						synPredMatched102 = true;
						inputState->guessing++;
						try {
							{
							template_head();
							{ // ( ... )*
							for (;;) {
								if ((_tokenSet_6.member(LA(1)))) {
									fs=function_specifier();
								}
								else {
									goto _loop101;
								}
								
							}
							_loop101:;
							} // ( ... )*
							class_specifier();
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched102 = false;
						}
						rewind(_m102);
						inputState->guessing--;
					}
					if ( synPredMatched102 ) {
						if ( inputState->guessing==0 ) {
							if (statementTrace>=1) 
										OUTPUT_DEBUG("%d member_declaration Templated class decl or def\n",LT(1)->getLine());
									
						}
						template_head();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						{ // ( ... )*
						for (;;) {
							if ((_tokenSet_6.member(LA(1)))) {
								fs=function_specifier();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else {
								goto _loop104;
							}
							
						}
						_loop104:;
						} // ( ... )*
						class_decl_or_def(fs);
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						{
						switch ( LA(1)) {
						case ID:
						case COLON:
						case LPAREN:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case STAR:
						case AMPERSAND:
						case SCOPE:
						case LITERAL__cdecl:
						case LITERAL___cdecl:
						case LITERAL__near:
						case LITERAL___near:
						case LITERAL__far:
						case LITERAL___far:
						case LITERAL___interrupt:
						case LITERAL_pascal:
						case LITERAL__pascal:
						case LITERAL___pascal:
						case LITERAL__stdcall:
						case LITERAL___stdcall:
						{
							init_declarator_list();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							break;
						}
						case SEMICOLON:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						match(SEMICOLON);
						if ( inputState->guessing==0 ) {
							end_of_stmt();
						}
						if ( inputState->guessing==0 ) {
							member_declaration_AST = RefPNode(currentAST.root);
							member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTEMPLATE,"templatepro")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
							currentAST.root = member_declaration_AST;
							if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
								member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
								  currentAST.child = member_declaration_AST->getFirstChild();
							else
								currentAST.child = member_declaration_AST;
							currentAST.advanceChildToEnd();
						}
					}
					else {
						bool synPredMatched108 = false;
						if (((LA(1) == LITERAL_enum) && (_tokenSet_7.member(LA(2))))) {
							int _m108 = mark();
							synPredMatched108 = true;
							inputState->guessing++;
							try {
								{
								match(LITERAL_enum);
								{
								switch ( LA(1)) {
								case ID:
								{
									match(ID);
									break;
								}
								case LCURLY:
								{
									break;
								}
								default:
								{
									throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
								}
								}
								}
								match(LCURLY);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched108 = false;
							}
							rewind(_m108);
							inputState->guessing--;
						}
						if ( synPredMatched108 ) {
							if ( inputState->guessing==0 ) {
								if (statementTrace>=1) 
											OUTPUT_DEBUG("%d member_declaration Enum definition\n",LT(1)->getLine());
										
							}
							enum_specifier();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							{
							switch ( LA(1)) {
							case ID:
							case COLON:
							case LPAREN:
							case OPERATOR:
							case TILDE:
							case LITERAL_typeid:
							case STAR:
							case AMPERSAND:
							case SCOPE:
							case LITERAL__cdecl:
							case LITERAL___cdecl:
							case LITERAL__near:
							case LITERAL___near:
							case LITERAL__far:
							case LITERAL___far:
							case LITERAL___interrupt:
							case LITERAL_pascal:
							case LITERAL__pascal:
							case LITERAL___pascal:
							case LITERAL__stdcall:
							case LITERAL___stdcall:
							{
								init_declarator_list();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								break;
							}
							case SEMICOLON:
							{
								break;
							}
							default:
							{
								throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
							}
							}
							}
							match(SEMICOLON);
							if ( inputState->guessing==0 ) {
								end_of_stmt();
							}
							if ( inputState->guessing==0 ) {
								member_declaration_AST = RefPNode(currentAST.root);
								member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
								currentAST.root = member_declaration_AST;
								if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
									member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
									  currentAST.child = member_declaration_AST->getFirstChild();
								else
									currentAST.child = member_declaration_AST;
								currentAST.advanceChildToEnd();
							}
						}
						else {
							bool synPredMatched111 = false;
							if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
								int _m111 = mark();
								synPredMatched111 = true;
								inputState->guessing++;
								try {
									{
									ctor_decl_spec();
									if (!(qualifiedItemIsOneOf(qiCtor)))
										throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
									ctor_declarator(0);
									match(SEMICOLON);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched111 = false;
								}
								rewind(_m111);
								inputState->guessing--;
							}
							if ( synPredMatched111 ) {
								if ( inputState->guessing==0 ) {
									if (statementTrace>=1) 
												OUTPUT_DEBUG("%d member_declaration Constructor declarator\n",LT(1)->getLine());
											
								}
								ctor_decl_spec();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								ctor_declarator(0);
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								match(SEMICOLON);
								if ( inputState->guessing==0 ) {
									end_of_stmt();
								}
								if ( inputState->guessing==0 ) {
									member_declaration_AST = RefPNode(currentAST.root);
									member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"ctor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
									currentAST.root = member_declaration_AST;
									if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
										member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
										  currentAST.child = member_declaration_AST->getFirstChild();
									else
										currentAST.child = member_declaration_AST;
									currentAST.advanceChildToEnd();
								}
							}
							else {
								bool synPredMatched114 = false;
								if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
									int _m114 = mark();
									synPredMatched114 = true;
									inputState->guessing++;
									try {
										{
										ctor_decl_spec();
										if (!(qualifiedItemIsOneOf(qiCtor)))
											throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
										ctor_declarator(1);
										{
										switch ( LA(1)) {
										case COLON:
										{
											match(COLON);
											break;
										}
										case LCURLY:
										{
											match(LCURLY);
											break;
										}
										default:
										{
											throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
										}
										}
										}
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched114 = false;
									}
									rewind(_m114);
									inputState->guessing--;
								}
								if ( synPredMatched114 ) {
									if ( inputState->guessing==0 ) {
										if (statementTrace>=1) 
													OUTPUT_DEBUG("%d member_declaration Constructor definition\n",LT(1)->getLine());
												
									}
									ctor_definition();
									if (inputState->guessing==0) {
										astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
									}
								}
								else {
									bool synPredMatched117 = false;
									if (((_tokenSet_45.member(LA(1))) && (_tokenSet_9.member(LA(2))))) {
										int _m117 = mark();
										synPredMatched117 = true;
										inputState->guessing++;
										try {
											{
											dtor_head(0);
											{
											switch ( LA(1)) {
											case ASSIGNEQUAL:
											{
												match(ASSIGNEQUAL);
												match(OCTALINT);
												break;
											}
											case SEMICOLON:
											{
												break;
											}
											default:
											{
												throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
											}
											}
											}
											match(SEMICOLON);
											}
										}
										catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
											synPredMatched117 = false;
										}
										rewind(_m117);
										inputState->guessing--;
									}
									if ( synPredMatched117 ) {
										if ( inputState->guessing==0 ) {
											if (statementTrace>=1) 
														OUTPUT_DEBUG("%d member_declaration Destructor declaration\n",LT(1)->getLine());
													
										}
										dtor_head(0);
										if (inputState->guessing==0) {
											astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
										}
										{
										switch ( LA(1)) {
										case ASSIGNEQUAL:
										{
											match(ASSIGNEQUAL);
											match(OCTALINT);
											if ( inputState->guessing==0 ) {
												astFactory->addASTChild(currentAST,astFactory->create(MY,"pure"));
											}
											break;
										}
										case SEMICOLON:
										{
											break;
										}
										default:
										{
											throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
										}
										}
										}
										match(SEMICOLON);
										if ( inputState->guessing==0 ) {
											end_of_stmt();
										}
										if ( inputState->guessing==0 ) {
											member_declaration_AST = RefPNode(currentAST.root);
											member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"dtor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
											currentAST.root = member_declaration_AST;
											if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
												member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
												  currentAST.child = member_declaration_AST->getFirstChild();
											else
												currentAST.child = member_declaration_AST;
											currentAST.advanceChildToEnd();
										}
									}
									else {
										bool synPredMatched120 = false;
										if (((_tokenSet_45.member(LA(1))) && (_tokenSet_9.member(LA(2))))) {
											int _m120 = mark();
											synPredMatched120 = true;
											inputState->guessing++;
											try {
												{
												dtor_head(1);
												match(LCURLY);
												}
											}
											catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
												synPredMatched120 = false;
											}
											rewind(_m120);
											inputState->guessing--;
										}
										if ( synPredMatched120 ) {
											if ( inputState->guessing==0 ) {
												if (statementTrace>=1) 
															OUTPUT_DEBUG("%d member_declaration Destructor definition\n",LT(1)->getLine());
														
											}
											dtor_head(1);
											if (inputState->guessing==0) {
												astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
											}
											dtor_body();
											if (inputState->guessing==0) {
												astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
											}
											if ( inputState->guessing==0 ) {
												member_declaration_AST = RefPNode(currentAST.root);
												member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"dtor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
												currentAST.root = member_declaration_AST;
												if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
													member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
													  currentAST.child = member_declaration_AST->getFirstChild();
												else
													currentAST.child = member_declaration_AST;
												currentAST.advanceChildToEnd();
											}
										}
										else {
											bool synPredMatched122 = false;
											if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
												int _m122 = mark();
												synPredMatched122 = true;
												inputState->guessing++;
												try {
													{
													declaration_specifiers();
													function_declarator(0);
													match(SEMICOLON);
													}
												}
												catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
													synPredMatched122 = false;
												}
												rewind(_m122);
												inputState->guessing--;
											}
											if ( synPredMatched122 ) {
												if ( inputState->guessing==0 ) {
													if (statementTrace>=1) 
																OUTPUT_DEBUG("%d member_declaration Function declaration\n",LT(1)->getLine());
															
												}
												function_declaration();
												if (inputState->guessing==0) {
													astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
												}
											}
											else {
												bool synPredMatched124 = false;
												if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
													int _m124 = mark();
													synPredMatched124 = true;
													inputState->guessing++;
													try {
														{
														declaration_specifiers();
														function_declarator(1);
														match(LCURLY);
														}
													}
													catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
														synPredMatched124 = false;
													}
													rewind(_m124);
													inputState->guessing--;
												}
												if ( synPredMatched124 ) {
													if ( inputState->guessing==0 ) {
														if (statementTrace>=1) 
																	OUTPUT_DEBUG("%d member_declaration Function definition\n",LT(1)->getLine());
																
													}
													function_definition();
													if (inputState->guessing==0) {
														astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
													}
												}
												else {
													bool synPredMatched127 = false;
													if (((LA(1) == LITERAL_inline || LA(1) == OPERATOR) && (_tokenSet_46.member(LA(2))))) {
														int _m127 = mark();
														synPredMatched127 = true;
														inputState->guessing++;
														try {
															{
															{
															switch ( LA(1)) {
															case LITERAL_inline:
															{
																match(LITERAL_inline);
																break;
															}
															case OPERATOR:
															{
																break;
															}
															default:
															{
																throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
															}
															}
															}
															conversion_function_decl_or_def();
															}
														}
														catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
															synPredMatched127 = false;
														}
														rewind(_m127);
														inputState->guessing--;
													}
													if ( synPredMatched127 ) {
														if ( inputState->guessing==0 ) {
															if (statementTrace>=1) 
																		OUTPUT_DEBUG("%d member_declaration Operator function\n",LT(1)->getLine());
																	
														}
														{
														switch ( LA(1)) {
														case LITERAL_inline:
														{
															m = LT(1);
															m_AST = astFactory->create(m);
															match(LITERAL_inline);
															break;
														}
														case OPERATOR:
														{
															break;
														}
														default:
														{
															throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
														}
														}
														}
														conversion_function_decl_or_def();
														if (inputState->guessing==0) {
															fc_AST = returnAST;
														}
														if ( inputState->guessing==0 ) {
															member_declaration_AST = RefPNode(currentAST.root);
															
																	 member_declaration_AST = astFactory->create(MYFUNCTION,"convertor");
																	 if(m_AST != nullAST)
																		member_declaration_AST->addChild(RefPNode(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(m_AST))))));
																	 member_declaration_AST->addChild(RefPNode(fc_AST));
																	
															currentAST.root = member_declaration_AST;
															if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																  currentAST.child = member_declaration_AST->getFirstChild();
															else
																currentAST.child = member_declaration_AST;
															currentAST.advanceChildToEnd();
														}
													}
													else {
														bool synPredMatched130 = false;
														if (((_tokenSet_47.member(LA(1))) && (_tokenSet_48.member(LA(2))))) {
															int _m130 = mark();
															synPredMatched130 = true;
															inputState->guessing++;
															try {
																{
																qualified_id();
																match(SEMICOLON);
																}
															}
															catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																synPredMatched130 = false;
															}
															rewind(_m130);
															inputState->guessing--;
														}
														if ( synPredMatched130 ) {
															if ( inputState->guessing==0 ) {
																if (statementTrace>=1) 
																			OUTPUT_DEBUG("%d member_declaration Qualified ID\n",LT(1)->getLine());
																		
															}
															q=qualified_id();
															if (inputState->guessing==0) {
																astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
															}
															RefPNode tmp84_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
															if ( inputState->guessing == 0 ) {
																tmp84_AST = astFactory->create(LT(1));
																astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp84_AST));
															}
															match(SEMICOLON);
															if ( inputState->guessing==0 ) {
																end_of_stmt();
															}
														}
														else {
															bool synPredMatched135 = false;
															if (((_tokenSet_49.member(LA(1))) && (_tokenSet_17.member(LA(2))))) {
																int _m135 = mark();
																synPredMatched135 = true;
																inputState->guessing++;
																try {
																	{
																	{
																	switch ( LA(1)) {
																	case LITERAL_friend:
																	{
																		match(LITERAL_friend);
																		break;
																	}
																	case LITERAL_inline:
																	case LITERAL_struct:
																	case LITERAL_union:
																	case LITERAL_class:
																	case LITERAL__inline:
																	case LITERAL___inline:
																	case LITERAL_virtual:
																	case LITERAL_explicit:
																	{
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																	}
																	}
																	}
																	{ // ( ... )*
																	for (;;) {
																		if ((_tokenSet_6.member(LA(1)))) {
																			fs=function_specifier();
																		}
																		else {
																			goto _loop134;
																		}
																		
																	}
																	_loop134:;
																	} // ( ... )*
																	class_specifier();
																	}
																}
																catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																	synPredMatched135 = false;
																}
																rewind(_m135);
																inputState->guessing--;
															}
															if ( synPredMatched135 ) {
																if ( inputState->guessing==0 ) {
																	if (statementTrace>=1) 
																				OUTPUT_DEBUG("%d member_declaration Class decl or def\n",LT(1)->getLine());
																			
																}
																{
																switch ( LA(1)) {
																case LITERAL_friend:
																{
																	match(LITERAL_friend);
																	break;
																}
																case LITERAL_inline:
																case LITERAL_struct:
																case LITERAL_union:
																case LITERAL_class:
																case LITERAL__inline:
																case LITERAL___inline:
																case LITERAL_virtual:
																case LITERAL_explicit:
																{
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																}
																}
																}
																{ // ( ... )*
																for (;;) {
																	if ((_tokenSet_6.member(LA(1)))) {
																		fs=function_specifier();
																		if (inputState->guessing==0) {
																			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																		}
																	}
																	else {
																		goto _loop138;
																	}
																	
																}
																_loop138:;
																} // ( ... )*
																class_decl_or_def(fs);
																if (inputState->guessing==0) {
																	astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																}
																{
																switch ( LA(1)) {
																case ID:
																case COLON:
																case LPAREN:
																case OPERATOR:
																case TILDE:
																case LITERAL_typeid:
																case STAR:
																case AMPERSAND:
																case SCOPE:
																case LITERAL__cdecl:
																case LITERAL___cdecl:
																case LITERAL__near:
																case LITERAL___near:
																case LITERAL__far:
																case LITERAL___far:
																case LITERAL___interrupt:
																case LITERAL_pascal:
																case LITERAL__pascal:
																case LITERAL___pascal:
																case LITERAL__stdcall:
																case LITERAL___stdcall:
																{
																	init_declarator_list();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																	break;
																}
																case SEMICOLON:
																{
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																}
																}
																}
																match(SEMICOLON);
																if ( inputState->guessing==0 ) {
																	end_of_stmt();
																}
																if ( inputState->guessing==0 ) {
																	member_declaration_AST = RefPNode(currentAST.root);
																	member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
																	currentAST.root = member_declaration_AST;
																	if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																		member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																		  currentAST.child = member_declaration_AST->getFirstChild();
																	else
																		currentAST.child = member_declaration_AST;
																	currentAST.advanceChildToEnd();
																}
															}
															else {
																bool synPredMatched142 = false;
																if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
																	int _m142 = mark();
																	synPredMatched142 = true;
																	inputState->guessing++;
																	try {
																		{
																		declaration_specifiers();
																		{
																		switch ( LA(1)) {
																		case ID:
																		case COLON:
																		case LPAREN:
																		case OPERATOR:
																		case TILDE:
																		case LITERAL_typeid:
																		case STAR:
																		case AMPERSAND:
																		case SCOPE:
																		case LITERAL__cdecl:
																		case LITERAL___cdecl:
																		case LITERAL__near:
																		case LITERAL___near:
																		case LITERAL__far:
																		case LITERAL___far:
																		case LITERAL___interrupt:
																		case LITERAL_pascal:
																		case LITERAL__pascal:
																		case LITERAL___pascal:
																		case LITERAL__stdcall:
																		case LITERAL___stdcall:
																		{
																			init_declarator_list();
																			break;
																		}
																		case SEMICOLON:
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																		}
																		}
																		}
																		match(SEMICOLON);
																		}
																	}
																	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																		synPredMatched142 = false;
																	}
																	rewind(_m142);
																	inputState->guessing--;
																}
																if ( synPredMatched142 ) {
																	if ( inputState->guessing==0 ) {
																		if (statementTrace>=1) 
																					OUTPUT_DEBUG("%d member_declaration Declaration\n",LT(1)->getLine());
																				
																	}
																	declaration();
																	if (inputState->guessing==0) {
																		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																	}
																}
																else {
																	bool synPredMatched146 = false;
																	if (((_tokenSet_50.member(LA(1))) && (_tokenSet_51.member(LA(2))))) {
																		int _m146 = mark();
																		synPredMatched146 = true;
																		inputState->guessing++;
																		try {
																			{
																			{ // ( ... )*
																			for (;;) {
																				if ((_tokenSet_6.member(LA(1)))) {
																					fs=function_specifier();
																				}
																				else {
																					goto _loop145;
																				}
																				
																			}
																			_loop145:;
																			} // ( ... )*
																			function_declarator(0);
																			match(SEMICOLON);
																			}
																		}
																		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																			synPredMatched146 = false;
																		}
																		rewind(_m146);
																		inputState->guessing--;
																	}
																	if ( synPredMatched146 ) {
																		if ( inputState->guessing==0 ) {
																			fprintf(stderr,"%d warning Function declaration found without return type\n",LT(1)->getLine());
																					 if (statementTrace>=1) 
																						OUTPUT_DEBUG("%d member_declaration Function declaration\n",LT(1)->getLine());
																					
																		}
																		{ // ( ... )*
																		for (;;) {
																			if ((_tokenSet_6.member(LA(1)))) {
																				fs=function_specifier();
																				if (inputState->guessing==0) {
																					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																				}
																			}
																			else {
																				goto _loop148;
																			}
																			
																		}
																		_loop148:;
																		} // ( ... )*
																		function_declarator(0);
																		if (inputState->guessing==0) {
																			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																		}
																		match(SEMICOLON);
																		if ( inputState->guessing==0 ) {
																			end_of_stmt();
																		}
																		if ( inputState->guessing==0 ) {
																			member_declaration_AST = RefPNode(currentAST.root);
																			member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
																			currentAST.root = member_declaration_AST;
																			if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																				member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																				  currentAST.child = member_declaration_AST->getFirstChild();
																			else
																				currentAST.child = member_declaration_AST;
																			currentAST.advanceChildToEnd();
																		}
																	}
																	else {
																		bool synPredMatched152 = false;
																		if (((_tokenSet_50.member(LA(1))) && (_tokenSet_51.member(LA(2))))) {
																			int _m152 = mark();
																			synPredMatched152 = true;
																			inputState->guessing++;
																			try {
																				{
																				{ // ( ... )*
																				for (;;) {
																					if ((_tokenSet_6.member(LA(1)))) {
																						fs=function_specifier();
																					}
																					else {
																						goto _loop151;
																					}
																					
																				}
																				_loop151:;
																				} // ( ... )*
																				function_declarator(1);
																				match(LCURLY);
																				}
																			}
																			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																				synPredMatched152 = false;
																			}
																			rewind(_m152);
																			inputState->guessing--;
																		}
																		if ( synPredMatched152 ) {
																			if ( inputState->guessing==0 ) {
																				fprintf(stderr,"%d warning Function definition found without return type\n",LT(1)->getLine());
																						 if (statementTrace>=1) 
																							OUTPUT_DEBUG("%d member_declaration Function definition without return type\n",LT(1)->getLine());
																						
																			}
																			{ // ( ... )*
																			for (;;) {
																				if ((_tokenSet_6.member(LA(1)))) {
																					fs=function_specifier();
																					if (inputState->guessing==0) {
																						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																					}
																				}
																				else {
																					goto _loop154;
																				}
																				
																			}
																			_loop154:;
																			} // ( ... )*
																			function_declarator(1);
																			if (inputState->guessing==0) {
																				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																			}
																			compound_statement();
																			if (inputState->guessing==0) {
																				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																			}
																			if ( inputState->guessing==0 ) {
																				endFunctionDefinition();
																			}
																			if ( inputState->guessing==0 ) {
																				member_declaration_AST = RefPNode(currentAST.root);
																				member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
																				currentAST.root = member_declaration_AST;
																				if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																					member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																					  currentAST.child = member_declaration_AST->getFirstChild();
																				else
																					currentAST.child = member_declaration_AST;
																				currentAST.advanceChildToEnd();
																			}
																		}
																		else if ((LA(1) == LITERAL_template) && (LA(2) == LESSTHAN)) {
																			if ( inputState->guessing==0 ) {
																				beginTemplateDeclaration();
																			}
																			template_head();
																			if (inputState->guessing==0) {
																				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																			}
																			{
																			bool synPredMatched158 = false;
																			if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
																				int _m158 = mark();
																				synPredMatched158 = true;
																				inputState->guessing++;
																				try {
																					{
																					declaration_specifiers();
																					{
																					switch ( LA(1)) {
																					case ID:
																					case COLON:
																					case LPAREN:
																					case OPERATOR:
																					case TILDE:
																					case LITERAL_typeid:
																					case STAR:
																					case AMPERSAND:
																					case SCOPE:
																					case LITERAL__cdecl:
																					case LITERAL___cdecl:
																					case LITERAL__near:
																					case LITERAL___near:
																					case LITERAL__far:
																					case LITERAL___far:
																					case LITERAL___interrupt:
																					case LITERAL_pascal:
																					case LITERAL__pascal:
																					case LITERAL___pascal:
																					case LITERAL__stdcall:
																					case LITERAL___stdcall:
																					{
																						init_declarator_list();
																						break;
																					}
																					case SEMICOLON:
																					{
																						break;
																					}
																					default:
																					{
																						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																					}
																					}
																					}
																					match(SEMICOLON);
																					}
																				}
																				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																					synPredMatched158 = false;
																				}
																				rewind(_m158);
																				inputState->guessing--;
																			}
																			if ( synPredMatched158 ) {
																				if ( inputState->guessing==0 ) {
																					if (statementTrace>=1) 
																									OUTPUT_DEBUG("%d member_declaration Templated forward declaration\n",LT(1)->getLine());
																								
																				}
																				declaration();
																				if (inputState->guessing==0) {
																					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																				}
																			}
																			else {
																				bool synPredMatched160 = false;
																				if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
																					int _m160 = mark();
																					synPredMatched160 = true;
																					inputState->guessing++;
																					try {
																						{
																						declaration_specifiers();
																						function_declarator(0);
																						match(SEMICOLON);
																						}
																					}
																					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																						synPredMatched160 = false;
																					}
																					rewind(_m160);
																					inputState->guessing--;
																				}
																				if ( synPredMatched160 ) {
																					if ( inputState->guessing==0 ) {
																						if (statementTrace>=1) 
																										OUTPUT_DEBUG("%d member_declaration Templated function declaration\n",LT(1)->getLine());
																									
																					}
																					function_declaration();
																					if (inputState->guessing==0) {
																						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																					}
																				}
																				else {
																					bool synPredMatched162 = false;
																					if (((_tokenSet_14.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
																						int _m162 = mark();
																						synPredMatched162 = true;
																						inputState->guessing++;
																						try {
																							{
																							declaration_specifiers();
																							function_declarator(1);
																							match(LCURLY);
																							}
																						}
																						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																							synPredMatched162 = false;
																						}
																						rewind(_m162);
																						inputState->guessing--;
																					}
																					if ( synPredMatched162 ) {
																						if ( inputState->guessing==0 ) {
																							if (statementTrace>=1) 
																											OUTPUT_DEBUG("%d member_declaration Templated function definition\n",LT(1)->getLine());
																										
																						}
																						function_definition();
																						if (inputState->guessing==0) {
																							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																						}
																					}
																					else {
																						bool synPredMatched164 = false;
																						if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
																							int _m164 = mark();
																							synPredMatched164 = true;
																							inputState->guessing++;
																							try {
																								{
																								ctor_decl_spec();
																								if (!(qualifiedItemIsOneOf(qiCtor)))
																									throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
																								ctor_declarator(0);
																								match(SEMICOLON);
																								}
																							}
																							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																								synPredMatched164 = false;
																							}
																							rewind(_m164);
																							inputState->guessing--;
																						}
																						if ( synPredMatched164 ) {
																							if ( inputState->guessing==0 ) {
																								if (statementTrace>=1) 
																												OUTPUT_DEBUG("%d member_declaration Templated constructor declarator\n",LT(1)->getLine());
																											
																							}
																							ctor_decl_spec();
																							if (inputState->guessing==0) {
																								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																							}
																							ctor_declarator(0);
																							if (inputState->guessing==0) {
																								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																							}
																							RefPNode tmp88_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
																							if ( inputState->guessing == 0 ) {
																								tmp88_AST = astFactory->create(LT(1));
																								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp88_AST));
																							}
																							match(SEMICOLON);
																							if ( inputState->guessing==0 ) {
																								end_of_stmt();
																							}
																						}
																						else {
																							bool synPredMatched166 = false;
																							if (((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2))))) {
																								int _m166 = mark();
																								synPredMatched166 = true;
																								inputState->guessing++;
																								try {
																									{
																									ctor_decl_spec();
																									if (!(qualifiedItemIsOneOf(qiCtor)))
																										throw ANTLR_USE_NAMESPACE(antlr)SemanticException("qualifiedItemIsOneOf(qiCtor)");
																									}
																								}
																								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																									synPredMatched166 = false;
																								}
																								rewind(_m166);
																								inputState->guessing--;
																							}
																							if ( synPredMatched166 ) {
																								if ( inputState->guessing==0 ) {
																									if (statementTrace>=1) 
																													OUTPUT_DEBUG("%d member_declaration Templated constructor definition\n",LT(1)->getLine());
																												
																								}
																								ctor_definition();
																								if (inputState->guessing==0) {
																									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																								}
																							}
																							else if ((LA(1) == LITERAL_inline || LA(1) == OPERATOR) && (_tokenSet_46.member(LA(2)))) {
																								if ( inputState->guessing==0 ) {
																									if (statementTrace>=1) 
																													OUTPUT_DEBUG("%d member_declaration Templated operator function\n",LT(1)->getLine());
																												
																								}
																								{
																								switch ( LA(1)) {
																								case LITERAL_inline:
																								{
																									mt = LT(1);
																									mt_AST = astFactory->create(mt);
																									match(LITERAL_inline);
																									break;
																								}
																								case OPERATOR:
																								{
																									break;
																								}
																								default:
																								{
																									throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																								}
																								}
																								}
																								conversion_function_decl_or_def();
																								if (inputState->guessing==0) {
																									fct_AST = returnAST;
																								}
																								if ( inputState->guessing==0 ) {
																									
																												 fct_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"convertor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(fct_AST))));
																										 		 if(mt_AST != nullAST)
																													fct_AST->addChild(RefPNode(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(mt_AST))))));
																												 astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(fct_AST));
																												
																								}
																							}
																							else if (((LA(1) >= LITERAL_struct && LA(1) <= LITERAL_class)) && (LA(2) == ID || LA(2) == LCURLY)) {
																								if ( inputState->guessing==0 ) {
																									if (statementTrace>=1) 
																													OUTPUT_DEBUG("%d member_declaration Templated class definition\n",LT(1)->getLine());
																												
																								}
																								class_head();
																								if (inputState->guessing==0) {
																									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																								}
																								declaration();
																								if (inputState->guessing==0) {
																									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
																								}
																							}
																			else {
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
																			}
																			}}}}
																			}
																			if ( inputState->guessing==0 ) {
																				endTemplateDeclaration();
																			}
																			if ( inputState->guessing==0 ) {
																				member_declaration_AST = RefPNode(currentAST.root);
																				member_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTEMPLATE,"templatepro")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(member_declaration_AST))));
																				currentAST.root = member_declaration_AST;
																				if ( member_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
																					member_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
																					  currentAST.child = member_declaration_AST->getFirstChild();
																				else
																					currentAST.child = member_declaration_AST;
																				currentAST.advanceChildToEnd();
																			}
																		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}}}}}}}}}}}}}}}}
		}
		member_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = member_declaration_AST;
}

void CPPParser::ctor_declarator(
	int definition
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data q;
	
	try {      // for error handling
		q=qualified_ctor_id();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			declaratorParameterList(definition);
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case ELLIPSIS:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			parameter_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			declaratorEndParameterList(definition);
		}
		{
		switch ( LA(1)) {
		case LITERAL_throw:
		{
			exception_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		case LCURLY:
		case COLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ctor_declarator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_52);
		} else {
			throw;
		}
	}
	returnAST = ctor_declarator_AST;
}

void CPPParser::dtor_body() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dtor_body_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			endDestructorDefinition();
		}
		dtor_body_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = dtor_body_AST;
}

void CPPParser::compound_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode compound_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LCURLY);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
					 enterNewLocalScope();
					
		}
		{
		switch ( LA(1)) {
		case LITERAL_template:
		case LITERAL_typedef:
		case LITERAL_enum:
		case SEMICOLON:
		case ID:
		case LCURLY:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_namespace:
		case LITERAL_antlrTrace_on:
		case LITERAL_antlrTrace_off:
		case OCTALINT:
		case LITERAL_friend:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_public:
		case LITERAL_protected:
		case LITERAL_private:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case LITERAL_throw:
		case LITERAL_using:
		case LITERAL_case:
		case LITERAL_default:
		case LITERAL_if:
		case LITERAL_switch:
		case LITERAL_while:
		case LITERAL_do:
		case LITERAL_for:
		case LITERAL_goto:
		case LITERAL_continue:
		case LITERAL_break:
		case LITERAL_return:
		case LITERAL___try:
		case LITERAL_try:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		case LITERAL_delete:
		{
			statement_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RCURLY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RCURLY);
		if ( inputState->guessing==0 ) {
			exitLocalScope();
		}
		if ( inputState->guessing==0 ) {
			compound_statement_AST = RefPNode(currentAST.root);
			compound_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYBODY,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(compound_statement_AST))));
			currentAST.root = compound_statement_AST;
			if ( compound_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				compound_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = compound_statement_AST->getFirstChild();
			else
				currentAST.child = compound_statement_AST;
			currentAST.advanceChildToEnd();
		}
		compound_statement_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_53);
		} else {
			throw;
		}
	}
	returnAST = compound_statement_AST;
}

void CPPParser::class_head() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode class_head_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_struct:
		{
			RefPNode tmp93_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp93_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp93_AST));
			}
			match(LITERAL_struct);
			break;
		}
		case LITERAL_union:
		{
			RefPNode tmp94_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp94_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp94_AST));
			}
			match(LITERAL_union);
			break;
		}
		case LITERAL_class:
		{
			RefPNode tmp95_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp95_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp95_AST));
			}
			match(LITERAL_class);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ID:
		{
			RefPNode tmp96_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp96_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp96_AST));
			}
			match(ID);
			{
			switch ( LA(1)) {
			case LESSTHAN:
			{
				RefPNode tmp97_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp97_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp97_AST));
				}
				match(LESSTHAN);
				template_argument_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				RefPNode tmp98_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp98_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp98_AST));
				}
				match(GREATERTHAN);
				break;
			}
			case LCURLY:
			case COLON:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case COLON:
			{
				base_clause();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case LCURLY:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LCURLY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(LCURLY);
		class_head_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_2);
		} else {
			throw;
		}
	}
	returnAST = class_head_AST;
}

void CPPParser::access_specifier() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode access_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_public:
		{
			RefPNode tmp100_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp100_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp100_AST));
			}
			match(LITERAL_public);
			break;
		}
		case LITERAL_protected:
		{
			RefPNode tmp101_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp101_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp101_AST));
			}
			match(LITERAL_protected);
			break;
		}
		case LITERAL_private:
		{
			RefPNode tmp102_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp102_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp102_AST));
			}
			match(LITERAL_private);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		access_specifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_54);
		} else {
			throw;
		}
	}
	returnAST = access_specifier_AST;
}

void CPPParser::linkage_specification() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode linkage_specification_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp103_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp103_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp103_AST));
		}
		match(LITERAL_extern);
		RefPNode tmp104_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp104_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp104_AST));
		}
		match(StringLiteral);
		{
		switch ( LA(1)) {
		case LCURLY:
		{
			RefPNode tmp105_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp105_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp105_AST));
			}
			match(LCURLY);
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_0.member(LA(1)))) {
					external_declaration();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					goto _loop183;
				}
				
			}
			_loop183:;
			} // ( ... )*
			RefPNode tmp106_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp106_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp106_AST));
			}
			match(RCURLY);
			break;
		}
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_using:
		case SCOPE:
		{
			declaration();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		linkage_specification_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = linkage_specification_AST;
}

void CPPParser::simple_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode simple_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		declaration_specifiers();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case ID:
		case COLON:
		case LPAREN:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			init_declarator_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(SEMICOLON);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
		}
		simple_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = simple_declaration_AST;
}

void CPPParser::using_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode using_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data qid;
	
	try {      // for error handling
		RefPNode tmp108_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp108_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp108_AST));
		}
		match(LITERAL_using);
		{
		switch ( LA(1)) {
		case LITERAL_namespace:
		{
			RefPNode tmp109_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp109_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp109_AST));
			}
			match(LITERAL_namespace);
			qid=qualified_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case ID:
		case LITERAL_typename:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case SCOPE:
		{
			{
			switch ( LA(1)) {
			case LITERAL_typename:
			{
				RefPNode tmp110_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp110_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp110_AST));
				}
				match(LITERAL_typename);
				break;
			}
			case ID:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case SCOPE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			qid=qualified_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(SEMICOLON);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
		}
		using_statement_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = using_statement_AST;
}

void CPPParser::template_argument_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_argument_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		template_argument();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				template_argument();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop413;
			}
			
		}
		_loop413:;
		} // ( ... )*
		template_argument_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_55);
		} else {
			throw;
		}
	}
	returnAST = template_argument_list_AST;
}

void CPPParser::base_clause() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode base_clause_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(COLON);
		base_specifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				base_specifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop234;
			}
			
		}
		_loop234:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			base_clause_AST = RefPNode(currentAST.root);
			base_clause_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"baseclass")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(base_clause_AST))));
			currentAST.root = base_clause_AST;
			if ( base_clause_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				base_clause_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = base_clause_AST->getFirstChild();
			else
				currentAST.child = base_clause_AST;
			currentAST.advanceChildToEnd();
		}
		base_clause_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_56);
		} else {
			throw;
		}
	}
	returnAST = base_clause_AST;
}

void CPPParser::specifier_prefix(
	bool& td, bool& fd, StorageClass& sc, TypeQualifier& tq, FunctionSpecifier& fs
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode specifier_prefix_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			switch ( LA(1)) {
			case LITERAL_typedef:
			{
				match(LITERAL_typedef);
				if ( inputState->guessing==0 ) {
					td=true;
				}
				break;
			}
			case LITERAL_friend:
			{
				match(LITERAL_friend);
				if ( inputState->guessing==0 ) {
					fd=true;
				}
				break;
			}
			case LITERAL_extern:
			case LITERAL_auto:
			case LITERAL_register:
			case LITERAL_static:
			case LITERAL_mutable:
			{
				sc=storage_class_specifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case LITERAL_const:
			case LITERAL_volatile:
			{
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case LITERAL_inline:
			case LITERAL__inline:
			case LITERAL___inline:
			case LITERAL_virtual:
			case LITERAL_explicit:
			{
				fs=function_specifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case LITERAL__declspec:
			case LITERAL___declspec:
			{
				{
				switch ( LA(1)) {
				case LITERAL__declspec:
				{
					match(LITERAL__declspec);
					break;
				}
				case LITERAL___declspec:
				{
					match(LITERAL___declspec);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(LPAREN);
				RefPNode tmp120_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp120_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp120_AST));
				}
				match(ID);
				match(RPAREN);
				break;
			}
			default:
			{
				goto _loop196;
			}
			}
		}
		_loop196:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			specifier_prefix_AST = RefPNode(currentAST.root);
			
					 if(specifier_prefix_AST != nullAST)
						specifier_prefix_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(specifier_prefix_AST))));
					
			currentAST.root = specifier_prefix_AST;
			if ( specifier_prefix_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				specifier_prefix_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = specifier_prefix_AST->getFirstChild();
			else
				currentAST.child = specifier_prefix_AST;
			currentAST.advanceChildToEnd();
		}
		specifier_prefix_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_57);
		} else {
			throw;
		}
	}
	returnAST = specifier_prefix_AST;
}

CPPParser::TypeSpecifier  CPPParser::type_specifier() {
	CPPParser::TypeSpecifier ts = tsInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode type_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	//char *s;
		 TypeQualifier tq = tqInvalid;
		
	
	try {      // for error handling
		ts=simple_type_specifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		type_specifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_58);
		} else {
			throw;
		}
	}
	returnAST = type_specifier_AST;
	return ts;
}

CPPParser::TypeQualifier  CPPParser::type_qualifier() {
	CPPParser::TypeQualifier tq = tqInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode type_qualifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_const:
		{
			RefPNode tmp122_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp122_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp122_AST));
			}
			match(LITERAL_const);
			if ( inputState->guessing==0 ) {
				tq = tqCONST;
			}
			break;
		}
		case LITERAL_volatile:
		{
			RefPNode tmp123_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp123_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp123_AST));
			}
			match(LITERAL_volatile);
			if ( inputState->guessing==0 ) {
				tq = tqVOLATILE;
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		type_qualifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_59);
		} else {
			throw;
		}
	}
	returnAST = type_qualifier_AST;
	return tq;
}

CPPParser::StorageClass  CPPParser::storage_class_specifier() {
	CPPParser::StorageClass sc = scInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode storage_class_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_auto:
		{
			RefPNode tmp124_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp124_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp124_AST));
			}
			match(LITERAL_auto);
			if ( inputState->guessing==0 ) {
				sc = scAUTO;
			}
			storage_class_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_register:
		{
			RefPNode tmp125_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp125_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp125_AST));
			}
			match(LITERAL_register);
			if ( inputState->guessing==0 ) {
				sc = scREGISTER;
			}
			storage_class_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_static:
		{
			RefPNode tmp126_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp126_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp126_AST));
			}
			match(LITERAL_static);
			if ( inputState->guessing==0 ) {
				sc = scSTATIC;
			}
			storage_class_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_extern:
		{
			RefPNode tmp127_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp127_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp127_AST));
			}
			match(LITERAL_extern);
			if ( inputState->guessing==0 ) {
				sc = scEXTERN;
			}
			storage_class_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_mutable:
		{
			RefPNode tmp128_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp128_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp128_AST));
			}
			match(LITERAL_mutable);
			if ( inputState->guessing==0 ) {
				sc = scMUTABLE;
			}
			storage_class_specifier_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_4);
		} else {
			throw;
		}
	}
	returnAST = storage_class_specifier_AST;
	return sc;
}

CPPParser::TypeSpecifier  CPPParser::simple_type_specifier() {
	CPPParser::TypeSpecifier ts = tsInvalid;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode simple_type_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data s;
		ts = tsInvalid;
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_enum:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL_typename:
		{
			{
			switch ( LA(1)) {
			case LITERAL_typename:
			{
				RefPNode tmp129_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp129_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp129_AST));
				}
				match(LITERAL_typename);
				break;
			}
			case LITERAL_enum:
			{
				RefPNode tmp130_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp130_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp130_AST));
				}
				match(LITERAL_enum);
				break;
			}
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			{
				ts=class_specifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			s=qualified_type();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				declaratorID(s.c_str(),qiType);
			}
			if ( inputState->guessing==0 ) {
				simple_type_specifier_AST = RefPNode(currentAST.root);
				simple_type_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPE,"typename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(simple_type_specifier_AST))));
				currentAST.root = simple_type_specifier_AST;
				if ( simple_type_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					simple_type_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = simple_type_specifier_AST->getFirstChild();
				else
					currentAST.child = simple_type_specifier_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		{
			{ // ( ... )+
			int _cnt210=0;
			for (;;) {
				switch ( LA(1)) {
				case LITERAL_char:
				{
					RefPNode tmp131_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp131_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp131_AST));
					}
					match(LITERAL_char);
					if ( inputState->guessing==0 ) {
						ts |= tsCHAR;
					}
					break;
				}
				case LITERAL_wchar_t:
				{
					RefPNode tmp132_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp132_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp132_AST));
					}
					match(LITERAL_wchar_t);
					if ( inputState->guessing==0 ) {
						ts |= tsWCHAR_T;
					}
					break;
				}
				case LITERAL_bool:
				{
					RefPNode tmp133_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp133_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp133_AST));
					}
					match(LITERAL_bool);
					if ( inputState->guessing==0 ) {
						ts |= tsBOOL;
					}
					break;
				}
				case LITERAL_short:
				{
					RefPNode tmp134_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp134_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp134_AST));
					}
					match(LITERAL_short);
					if ( inputState->guessing==0 ) {
						ts |= tsSHORT;
					}
					break;
				}
				case LITERAL_int:
				{
					RefPNode tmp135_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp135_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp135_AST));
					}
					match(LITERAL_int);
					if ( inputState->guessing==0 ) {
						ts |= tsINT;
					}
					break;
				}
				case 44:
				case 45:
				{
					{
					switch ( LA(1)) {
					case 44:
					{
						RefPNode tmp136_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp136_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp136_AST));
						}
						match(44);
						break;
					}
					case 45:
					{
						RefPNode tmp137_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp137_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp137_AST));
						}
						match(45);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState->guessing==0 ) {
						ts |= tsINT;
					}
					break;
				}
				case 46:
				case 47:
				{
					{
					switch ( LA(1)) {
					case 46:
					{
						RefPNode tmp138_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp138_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp138_AST));
						}
						match(46);
						break;
					}
					case 47:
					{
						RefPNode tmp139_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp139_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp139_AST));
						}
						match(47);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState->guessing==0 ) {
						ts |= tsINT;
					}
					break;
				}
				case 48:
				case 49:
				{
					{
					switch ( LA(1)) {
					case 48:
					{
						RefPNode tmp140_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp140_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp140_AST));
						}
						match(48);
						break;
					}
					case 49:
					{
						RefPNode tmp141_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp141_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp141_AST));
						}
						match(49);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState->guessing==0 ) {
						ts |= tsLONG;
					}
					break;
				}
				case 50:
				case 51:
				{
					{
					switch ( LA(1)) {
					case 50:
					{
						RefPNode tmp142_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp142_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp142_AST));
						}
						match(50);
						break;
					}
					case 51:
					{
						RefPNode tmp143_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp143_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp143_AST));
						}
						match(51);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState->guessing==0 ) {
						ts |= tsLONG;
					}
					break;
				}
				case 52:
				case 53:
				{
					{
					switch ( LA(1)) {
					case 52:
					{
						RefPNode tmp144_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp144_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp144_AST));
						}
						match(52);
						break;
					}
					case 53:
					{
						RefPNode tmp145_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp145_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp145_AST));
						}
						match(53);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					if ( inputState->guessing==0 ) {
						ts |= tsLONG;
					}
					break;
				}
				case LITERAL_long:
				{
					RefPNode tmp146_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp146_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp146_AST));
					}
					match(LITERAL_long);
					if ( inputState->guessing==0 ) {
						ts |= tsLONG;
					}
					break;
				}
				case LITERAL_signed:
				{
					RefPNode tmp147_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp147_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp147_AST));
					}
					match(LITERAL_signed);
					if ( inputState->guessing==0 ) {
						ts |= tsSIGNED;
					}
					break;
				}
				case LITERAL_unsigned:
				{
					RefPNode tmp148_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp148_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp148_AST));
					}
					match(LITERAL_unsigned);
					if ( inputState->guessing==0 ) {
						ts |= tsUNSIGNED;
					}
					break;
				}
				case LITERAL_float:
				{
					RefPNode tmp149_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp149_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp149_AST));
					}
					match(LITERAL_float);
					if ( inputState->guessing==0 ) {
						ts |= tsFLOAT;
					}
					break;
				}
				case LITERAL_double:
				{
					RefPNode tmp150_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp150_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp150_AST));
					}
					match(LITERAL_double);
					if ( inputState->guessing==0 ) {
						ts |= tsDOUBLE;
					}
					break;
				}
				case LITERAL_void:
				{
					RefPNode tmp151_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp151_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp151_AST));
					}
					match(LITERAL_void);
					if ( inputState->guessing==0 ) {
						ts |= tsVOID;
					}
					break;
				}
				default:
				{
					if ( _cnt210>=1 ) { goto _loop210; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				}
				_cnt210++;
			}
			_loop210:;
			}  // ( ... )+
			if ( inputState->guessing==0 ) {
				simple_type_specifier_AST = RefPNode(currentAST.root);
				simple_type_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPE,"basictype")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(simple_type_specifier_AST))));
				currentAST.root = simple_type_specifier_AST;
				if ( simple_type_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					simple_type_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = simple_type_specifier_AST->getFirstChild();
				else
					currentAST.child = simple_type_specifier_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		default:
			if (((LA(1) == ID || LA(1) == SCOPE))&&(qualifiedItemIsOneOf(qiType|qiCtor))) {
				s=qualified_type();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				if ( inputState->guessing==0 ) {
					simple_type_specifier_AST = RefPNode(currentAST.root);
					simple_type_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPE,"type")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(simple_type_specifier_AST))));
					currentAST.root = simple_type_specifier_AST;
					if ( simple_type_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						simple_type_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = simple_type_specifier_AST->getFirstChild();
					else
						currentAST.child = simple_type_specifier_AST;
					currentAST.advanceChildToEnd();
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		simple_type_specifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_58);
		} else {
			throw;
		}
	}
	returnAST = simple_type_specifier_AST;
	return ts;
}

data  CPPParser::qualified_type() {
	data qit;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode qualified_type_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  m = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode m_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		 data so = NULL;
		 char qitem01[CPPParser_MaxQualifiedItemSize+1];
		 qitem01[0] = '\0';
		
	
	try {      // for error handling
		so=scope_override();
		if ( inputState->guessing==0 ) {
			
					 strcpy(qitem01, so.c_str());
					
		}
		id = LT(1);
		if ( inputState->guessing == 0 ) {
			id_AST = astFactory->create(id);
		}
		match(ID);
		if ( inputState->guessing==0 ) {
			
					 strcat(qitem01, (id->getText()).data());
					 qit = qitem01;
					
		}
		{
		if ((LA(1) == LESSTHAN) && (_tokenSet_42.member(LA(2)))) {
			m = LT(1);
			if ( inputState->guessing == 0 ) {
				m_AST = astFactory->create(m);
			}
			match(LESSTHAN);
			template_argument_list();
			RefPNode tmp152_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp152_AST = astFactory->create(LT(1));
			}
			match(GREATERTHAN);
			if ( inputState->guessing==0 ) {
				m_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"template")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
			}
		}
		else if ((_tokenSet_60.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		if ( inputState->guessing==0 ) {
			qualified_type_AST = RefPNode(currentAST.root);
			qualified_type_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(m_AST))));
			currentAST.root = qualified_type_AST;
			if ( qualified_type_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				qualified_type_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = qualified_type_AST->getFirstChild();
			else
				currentAST.child = qualified_type_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_60);
		} else {
			throw;
		}
	}
	returnAST = qualified_type_AST;
	return qit;
}

void CPPParser::class_prefix() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode class_prefix_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL__declspec || LA(1) == LITERAL___declspec)) {
				{
				switch ( LA(1)) {
				case LITERAL__declspec:
				{
					match(LITERAL__declspec);
					break;
				}
				case LITERAL___declspec:
				{
					match(LITERAL___declspec);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(LPAREN);
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RPAREN);
			}
			else {
				goto _loop220;
			}
			
		}
		_loop220:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			class_prefix_AST = RefPNode(currentAST.root);
			
				 if(class_prefix_AST != nullAST)
					class_prefix_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(class_prefix_AST))));
				
			currentAST.root = class_prefix_AST;
			if ( class_prefix_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				class_prefix_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = class_prefix_AST->getFirstChild();
			else
				currentAST.child = class_prefix_AST;
			currentAST.advanceChildToEnd();
		}
		class_prefix_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_7);
		} else {
			throw;
		}
	}
	returnAST = class_prefix_AST;
}

void CPPParser::expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		lineNo = LT(1)->getLine();
		
	
	try {      // for error handling
		assignment_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				assignment_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop480;
			}
			
		}
		_loop480:;
		} // ( ... )*
		expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_61);
		} else {
			throw;
		}
	}
	returnAST = expression_AST;
}

void CPPParser::member_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode member_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode m_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode v_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode d_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  i = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode i_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		bool synPredMatched262 = false;
		if (((LA(1) == ID || LA(1) == COLON) && (_tokenSet_62.member(LA(2))))) {
			int _m262 = mark();
			synPredMatched262 = true;
			inputState->guessing++;
			try {
				{
				{
				switch ( LA(1)) {
				case ID:
				{
					match(ID);
					break;
				}
				case COLON:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(COLON);
				constant_expression();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched262 = false;
			}
			rewind(_m262);
			inputState->guessing--;
		}
		if ( synPredMatched262 ) {
			{
			switch ( LA(1)) {
			case ID:
			{
				RefPNode tmp158_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp158_AST = astFactory->create(LT(1));
				}
				match(ID);
				if ( inputState->guessing==0 ) {
					m_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tmp158_AST))));
				}
				break;
			}
			case COLON:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			RefPNode tmp159_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp159_AST = astFactory->create(LT(1));
			}
			match(COLON);
			{
			constant_expression();
			if ( inputState->guessing==0 ) {
				v_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"bytes")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
			}
			}
			if ( inputState->guessing==0 ) {
				member_declarator_AST = RefPNode(currentAST.root);
				member_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"variables")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(m_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(v_AST))));
				currentAST.root = member_declarator_AST;
				if ( member_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					member_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = member_declarator_AST->getFirstChild();
				else
					currentAST.child = member_declarator_AST;
				currentAST.advanceChildToEnd();
			}
		}
		else if ((_tokenSet_39.member(LA(1))) && (_tokenSet_63.member(LA(2)))) {
			declarator();
			if (inputState->guessing==0) {
				d_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				d_AST=returnAST;
			}
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				RefPNode tmp160_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp160_AST = astFactory->create(LT(1));
				}
				match(LPAREN);
				expression_list();
				RefPNode tmp161_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp161_AST = astFactory->create(LT(1));
				}
				match(RPAREN);
				if ( inputState->guessing==0 ) {
					i_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"ctor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
				}
				break;
			}
			case SEMICOLON:
			case COMMA:
			{
				break;
			}
			default:
				bool synPredMatched267 = false;
				if (((LA(1) == ASSIGNEQUAL) && (LA(2) == OCTALINT))) {
					int _m267 = mark();
					synPredMatched267 = true;
					inputState->guessing++;
					try {
						{
						match(ASSIGNEQUAL);
						match(OCTALINT);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched267 = false;
					}
					rewind(_m267);
					inputState->guessing--;
				}
				if ( synPredMatched267 ) {
					RefPNode tmp162_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp162_AST = astFactory->create(LT(1));
					}
					match(ASSIGNEQUAL);
					i = LT(1);
					if ( inputState->guessing == 0 ) {
						i_AST = astFactory->create(i);
					}
					match(OCTALINT);
					if ( inputState->guessing==0 ) {
						i_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(i_AST))));
					}
				}
				else if ((LA(1) == ASSIGNEQUAL) && (_tokenSet_64.member(LA(2)))) {
					RefPNode tmp163_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp163_AST = astFactory->create(LT(1));
					}
					match(ASSIGNEQUAL);
					initializer();
					if ( inputState->guessing==0 ) {
						i_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
					}
				}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				member_declarator_AST = RefPNode(currentAST.root);
				
						 if(_td==true)
							member_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"newtype")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(d_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(i_AST))));
						 else
							member_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"variables")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(d_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(i_AST))));
						
				currentAST.root = member_declarator_AST;
				if ( member_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					member_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = member_declarator_AST->getFirstChild();
				else
					currentAST.child = member_declarator_AST;
				currentAST.advanceChildToEnd();
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_65);
		} else {
			throw;
		}
	}
	returnAST = member_declarator_AST;
}

void CPPParser::base_specifier() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode base_specifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  v = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode v_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode q_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  v2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode v2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data qt;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_virtual:
		{
			v = LT(1);
			v_AST = astFactory->create(v);
			match(LITERAL_virtual);
			{
			switch ( LA(1)) {
			case LITERAL_public:
			case LITERAL_protected:
			case LITERAL_private:
			{
				access_specifier();
				if ( inputState->guessing==0 ) {
					s_AST=returnAST;
				}
				break;
			}
			case ID:
			case SCOPE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			qt=qualified_type();
			if (inputState->guessing==0) {
				q_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				q_AST=returnAST;
			}
			break;
		}
		case LITERAL_public:
		case LITERAL_protected:
		case LITERAL_private:
		{
			access_specifier();
			if ( inputState->guessing==0 ) {
				s_AST=returnAST;
			}
			{
			switch ( LA(1)) {
			case LITERAL_virtual:
			{
				v2 = LT(1);
				v2_AST = astFactory->create(v2);
				match(LITERAL_virtual);
				break;
			}
			case ID:
			case SCOPE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			qt=qualified_type();
			if ( inputState->guessing==0 ) {
				q_AST=returnAST;
			}
			break;
		}
		case ID:
		case SCOPE:
		{
			qt=qualified_type();
			if ( inputState->guessing==0 ) {
				q_AST=returnAST;
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			base_specifier_AST = RefPNode(currentAST.root);
			base_specifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPE,"typename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(q_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(4))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(v_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(v2_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))))))));
			currentAST.root = base_specifier_AST;
			if ( base_specifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				base_specifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = base_specifier_AST->getFirstChild();
			else
				currentAST.child = base_specifier_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_66);
		} else {
			throw;
		}
	}
	returnAST = base_specifier_AST;
}

void CPPParser::enumerator_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode enumerator_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		enumerator();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				{
				switch ( LA(1)) {
				case ID:
				{
					enumerator();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case RCURLY:
				case COMMA:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else {
				goto _loop247;
			}
			
		}
		_loop247:;
		} // ( ... )*
		enumerator_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_67);
		} else {
			throw;
		}
	}
	returnAST = enumerator_list_AST;
}

void CPPParser::enumerator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode enumerator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		id = LT(1);
		if ( inputState->guessing == 0 ) {
			id_AST = astFactory->create(id);
		}
		match(ID);
		{
		switch ( LA(1)) {
		case ASSIGNEQUAL:
		{
			match(ASSIGNEQUAL);
			constant_expression();
			if ( inputState->guessing==0 ) {
				id_AST->addChild(RefPNode(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))))));
			}
			break;
		}
		case RCURLY:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			enumElement((id->getText()).data());
		}
		if ( inputState->guessing==0 ) {
			enumerator_AST = RefPNode(currentAST.root);
			enumerator_AST = id_AST;
			currentAST.root = enumerator_AST;
			if ( enumerator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				enumerator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = enumerator_AST->getFirstChild();
			else
				currentAST.child = enumerator_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_68);
		} else {
			throw;
		}
	}
	returnAST = enumerator_AST;
}

void CPPParser::constant_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode constant_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		conditional_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constant_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_69);
		} else {
			throw;
		}
	}
	returnAST = constant_expression_AST;
}

data  CPPParser::optor() {
	data s;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode optor_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		char sitem[CPPParser_MaxQualifiedItemSize+1];
		sitem[0]='\0';
		TypeSpecifier ts=tsInvalid;
		char *opitem;
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_new:
		{
			RefPNode tmp166_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp166_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp166_AST));
			}
			match(LITERAL_new);
			if ( inputState->guessing==0 ) {
				strcat(sitem," new");
			}
			{
			if ((LA(1) == LSQUARE) && (LA(2) == RSQUARE)) {
				RefPNode tmp167_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp167_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp167_AST));
				}
				match(LSQUARE);
				RefPNode tmp168_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp168_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp168_AST));
				}
				match(RSQUARE);
				if ( inputState->guessing==0 ) {
					strcat(sitem,"[]");
				}
			}
			else if ((_tokenSet_43.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case LITERAL_delete:
		{
			RefPNode tmp169_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp169_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp169_AST));
			}
			match(LITERAL_delete);
			if ( inputState->guessing==0 ) {
				strcat(sitem," delete");
			}
			{
			if ((LA(1) == LSQUARE) && (LA(2) == RSQUARE)) {
				RefPNode tmp170_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp170_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp170_AST));
				}
				match(LSQUARE);
				RefPNode tmp171_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp171_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp171_AST));
				}
				match(RSQUARE);
				if ( inputState->guessing==0 ) {
					strcat(sitem,"[]");
				}
			}
			else if ((_tokenSet_43.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		case LPAREN:
		{
			RefPNode tmp172_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp172_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp172_AST));
			}
			match(LPAREN);
			match(RPAREN);
			if ( inputState->guessing==0 ) {
				strcat(sitem,"()");
			}
			break;
		}
		case LSQUARE:
		{
			RefPNode tmp174_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp174_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp174_AST));
			}
			match(LSQUARE);
			match(RSQUARE);
			if ( inputState->guessing==0 ) {
				strcat(sitem,"[]");
			}
			break;
		}
		case LESSTHAN:
		case GREATERTHAN:
		case ASSIGNEQUAL:
		case COMMA:
		case TILDE:
		case STAR:
		case AMPERSAND:
		case TIMESEQUAL:
		case DIVIDEEQUAL:
		case MINUSEQUAL:
		case PLUSEQUAL:
		case MODEQUAL:
		case SHIFTLEFTEQUAL:
		case SHIFTRIGHTEQUAL:
		case BITWISEANDEQUAL:
		case BITWISEXOREQUAL:
		case BITWISEOREQUAL:
		case OR:
		case AND:
		case BITWISEOR:
		case BITWISEXOR:
		case NOTEQUAL:
		case EQUAL:
		case LESSTHANOREQUALTO:
		case GREATERTHANOREQUALTO:
		case SHIFTLEFT:
		case SHIFTRIGHT:
		case PLUS:
		case MINUS:
		case DIVIDE:
		case MOD:
		case POINTERTOMBR:
		case PLUSPLUS:
		case MINUSMINUS:
		case POINTERTO:
		case NOT:
		{
			if ( inputState->guessing==0 ) {
				strcat(sitem,(LT(1)->getText()).data());
			}
			optor_simple_tokclass();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_enum:
		case ID:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case SCOPE:
		{
			if ( inputState->guessing==0 ) {
				strcat(sitem,"type-specifier()");
			}
			ts=type_specifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp176_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp176_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp176_AST));
			}
			match(LPAREN);
			match(RPAREN);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			optor_AST = RefPNode(currentAST.root);
			
					s = sitem;
					optor_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"operator")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(optor_AST))));
					
			currentAST.root = optor_AST;
			if ( optor_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				optor_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = optor_AST->getFirstChild();
			else
				currentAST.child = optor_AST;
			currentAST.advanceChildToEnd();
		}
		optor_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_43);
		} else {
			throw;
		}
	}
	returnAST = optor_AST;
	return s;
}

void CPPParser::id_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode id_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data s;
		
	
	try {      // for error handling
		{
		s=qualified_id();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		}
		id_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_43);
		} else {
			throw;
		}
	}
	returnAST = id_expression_AST;
}

void CPPParser::typeID() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode typeID_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if (!(isTypeName((LT(1)->getText()).data())))
			throw ANTLR_USE_NAMESPACE(antlr)SemanticException("isTypeName((LT(1)->getText()).data())");
		RefPNode tmp178_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp178_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp178_AST));
		}
		match(ID);
		typeID_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_1);
		} else {
			throw;
		}
	}
	returnAST = typeID_AST;
}

void CPPParser::declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode pt_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode d_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		bool synPredMatched274 = false;
		if (((_tokenSet_22.member(LA(1))) && (_tokenSet_23.member(LA(2))))) {
			int _m274 = mark();
			synPredMatched274 = true;
			inputState->guessing++;
			try {
				{
				ptr_operator();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched274 = false;
			}
			rewind(_m274);
			inputState->guessing--;
		}
		if ( synPredMatched274 ) {
			ptr_operator();
			if (inputState->guessing==0) {
				pt_AST = returnAST;
			}
			declarator();
			if (inputState->guessing==0) {
				d_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				declarator_AST = RefPNode(currentAST.root);
				d_AST->addChild(pt_AST); declarator_AST = d_AST;
				currentAST.root = declarator_AST;
				if ( declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = declarator_AST->getFirstChild();
				else
					currentAST.child = declarator_AST;
				currentAST.advanceChildToEnd();
			}
		}
		else if ((_tokenSet_24.member(LA(1))) && (_tokenSet_70.member(LA(2)))) {
			direct_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			declarator_AST = RefPNode(currentAST.root);
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_71);
		} else {
			throw;
		}
	}
	returnAST = declarator_AST;
}

void CPPParser::initializer() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode initializer_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_enum:
		case ID:
		case OCTALINT:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL_delete:
		{
			remainder_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			initializer_AST = RefPNode(currentAST.root);
			break;
		}
		case LCURLY:
		{
			match(LCURLY);
			initializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					match(COMMA);
					{
					switch ( LA(1)) {
					case LITERAL_enum:
					case ID:
					case LCURLY:
					case OCTALINT:
					case StringLiteral:
					case LITERAL_struct:
					case LITERAL_union:
					case LITERAL_class:
					case LPAREN:
					case LITERAL_typename:
					case LITERAL_char:
					case LITERAL_wchar_t:
					case LITERAL_bool:
					case LITERAL_short:
					case LITERAL_int:
					case 44:
					case 45:
					case 46:
					case 47:
					case 48:
					case 49:
					case 50:
					case 51:
					case 52:
					case 53:
					case LITERAL_long:
					case LITERAL_signed:
					case LITERAL_unsigned:
					case LITERAL_float:
					case LITERAL_double:
					case LITERAL_void:
					case OPERATOR:
					case TILDE:
					case LITERAL_typeid:
					case STAR:
					case AMPERSAND:
					case PLUS:
					case MINUS:
					case PLUSPLUS:
					case MINUSMINUS:
					case LITERAL_sizeof:
					case LITERAL___alignof__:
					case SCOPE:
					case LITERAL_dynamic_cast:
					case LITERAL_static_cast:
					case LITERAL_reinterpret_cast:
					case LITERAL_const_cast:
					case LITERAL_this:
					case DECIMALINT:
					case HEXADECIMALINT:
					case CharLiteral:
					case WCharLiteral:
					case WStringLiteral:
					case FLOATONE:
					case FLOATTWO:
					case LITERAL_true:
					case LITERAL_false:
					case NOT:
					case LITERAL_new:
					case LITERAL_delete:
					{
						initializer();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						break;
					}
					case RCURLY:
					case COMMA:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				else {
					goto _loop271;
				}
				
			}
			_loop271:;
			} // ( ... )*
			match(RCURLY);
			if ( inputState->guessing==0 ) {
				initializer_AST = RefPNode(currentAST.root);
				initializer_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"values")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(initializer_AST))));
				currentAST.root = initializer_AST;
				if ( initializer_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					initializer_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = initializer_AST->getFirstChild();
				else
					currentAST.child = initializer_AST;
				currentAST.advanceChildToEnd();
			}
			initializer_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_72);
		} else {
			throw;
		}
	}
	returnAST = initializer_AST;
}

void CPPParser::expression_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode expression_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		assignment_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				assignment_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop616;
			}
			
		}
		_loop616:;
		} // ( ... )*
		expression_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_73);
		} else {
			throw;
		}
	}
	returnAST = expression_list_AST;
}

void CPPParser::remainder_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode remainder_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		bool synPredMatched488 = false;
		if (((_tokenSet_74.member(LA(1))) && (_tokenSet_75.member(LA(2))))) {
			int _m488 = mark();
			synPredMatched488 = true;
			inputState->guessing++;
			try {
				{
				conditional_expression();
				{
				switch ( LA(1)) {
				case COMMA:
				{
					match(COMMA);
					break;
				}
				case SEMICOLON:
				{
					match(SEMICOLON);
					break;
				}
				case RPAREN:
				{
					match(RPAREN);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched488 = false;
			}
			rewind(_m488);
			inputState->guessing--;
		}
		if ( synPredMatched488 ) {
			if ( inputState->guessing==0 ) {
				assign_stmt_RHS_found += 1;
			}
			assignment_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				
							if (assign_stmt_RHS_found > 0)
								assign_stmt_RHS_found -= 1;
							else
								{
								fprintf(stderr,"%d warning Error in assign_stmt_RHS_found = %d\n",
									lineNo,assign_stmt_RHS_found);
								fprintf(stderr,"Press return to continue\n");
								getchar();
								}
							
			}
		}
		else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_75.member(LA(2)))) {
			assignment_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		remainder_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_76);
		} else {
			throw;
		}
	}
	returnAST = remainder_expression_AST;
}

void CPPParser::ptr_operator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ptr_operator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data s;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case AMPERSAND:
		{
			RefPNode tmp183_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp183_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp183_AST));
			}
			match(AMPERSAND);
			if ( inputState->guessing==0 ) {
				is_address = true;
			}
			break;
		}
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		{
			{
			switch ( LA(1)) {
			case LITERAL__cdecl:
			{
				RefPNode tmp184_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp184_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp184_AST));
				}
				match(LITERAL__cdecl);
				break;
			}
			case LITERAL___cdecl:
			{
				RefPNode tmp185_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp185_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp185_AST));
				}
				match(LITERAL___cdecl);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL__near:
		case LITERAL___near:
		{
			{
			switch ( LA(1)) {
			case LITERAL__near:
			{
				RefPNode tmp186_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp186_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp186_AST));
				}
				match(LITERAL__near);
				break;
			}
			case LITERAL___near:
			{
				RefPNode tmp187_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp187_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp187_AST));
				}
				match(LITERAL___near);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL__far:
		case LITERAL___far:
		{
			{
			switch ( LA(1)) {
			case LITERAL__far:
			{
				RefPNode tmp188_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp188_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp188_AST));
				}
				match(LITERAL__far);
				break;
			}
			case LITERAL___far:
			{
				RefPNode tmp189_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp189_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp189_AST));
				}
				match(LITERAL___far);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL___interrupt:
		{
			RefPNode tmp190_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp190_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp190_AST));
			}
			match(LITERAL___interrupt);
			break;
		}
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		{
			{
			switch ( LA(1)) {
			case LITERAL_pascal:
			{
				RefPNode tmp191_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp191_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp191_AST));
				}
				match(LITERAL_pascal);
				break;
			}
			case LITERAL__pascal:
			{
				RefPNode tmp192_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp192_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp192_AST));
				}
				match(LITERAL__pascal);
				break;
			}
			case LITERAL___pascal:
			{
				RefPNode tmp193_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp193_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp193_AST));
				}
				match(LITERAL___pascal);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			{
			switch ( LA(1)) {
			case LITERAL__stdcall:
			{
				RefPNode tmp194_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp194_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp194_AST));
				}
				match(LITERAL__stdcall);
				break;
			}
			case LITERAL___stdcall:
			{
				RefPNode tmp195_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp195_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp195_AST));
				}
				match(LITERAL___stdcall);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case ID:
		case STAR:
		case SCOPE:
		{
			s=scope_override();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp196_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp196_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp196_AST));
			}
			match(STAR);
			if ( inputState->guessing==0 ) {
				is_pointer = true;
			}
			cv_qualifier_seq();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			ptr_operator_AST = RefPNode(currentAST.root);
			ptr_operator_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"modifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ptr_operator_AST))));
			currentAST.root = ptr_operator_AST;
			if ( ptr_operator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				ptr_operator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = ptr_operator_AST->getFirstChild();
			else
				currentAST.child = ptr_operator_AST;
			currentAST.advanceChildToEnd();
		}
		ptr_operator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_21);
		} else {
			throw;
		}
	}
	returnAST = ptr_operator_AST;
}

void CPPParser::direct_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode direct_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode QID_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode d_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data id;
		int nType = 0;
		
	
	try {      // for error handling
		bool synPredMatched278 = false;
		if (((_tokenSet_47.member(LA(1))) && (_tokenSet_77.member(LA(2))))) {
			int _m278 = mark();
			synPredMatched278 = true;
			inputState->guessing++;
			try {
				{
				qualified_id();
				match(LPAREN);
				{
				switch ( LA(1)) {
				case RPAREN:
				{
					match(RPAREN);
					break;
				}
				case LITERAL_typedef:
				case LITERAL_enum:
				case ID:
				case LITERAL_inline:
				case LITERAL_extern:
				case LITERAL_friend:
				case LITERAL_struct:
				case LITERAL_union:
				case LITERAL_class:
				case LITERAL__declspec:
				case LITERAL___declspec:
				case LITERAL_auto:
				case LITERAL_register:
				case LITERAL_static:
				case LITERAL_mutable:
				case LITERAL__inline:
				case LITERAL___inline:
				case LITERAL_virtual:
				case LITERAL_explicit:
				case LITERAL_typename:
				case LITERAL_char:
				case LITERAL_wchar_t:
				case LITERAL_bool:
				case LITERAL_short:
				case LITERAL_int:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case LITERAL_long:
				case LITERAL_signed:
				case LITERAL_unsigned:
				case LITERAL_float:
				case LITERAL_double:
				case LITERAL_void:
				case LITERAL_const:
				case LITERAL_volatile:
				case SCOPE:
				{
					declaration_specifiers();
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched278 = false;
			}
			rewind(_m278);
			inputState->guessing--;
		}
		if ( synPredMatched278 ) {
			function_inline_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			direct_declarator_AST = RefPNode(currentAST.root);
		}
		else {
			bool synPredMatched280 = false;
			if (((_tokenSet_47.member(LA(1))) && (_tokenSet_77.member(LA(2))))) {
				int _m280 = mark();
				synPredMatched280 = true;
				inputState->guessing++;
				try {
					{
					qualified_id();
					match(LPAREN);
					qualified_id();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched280 = false;
				}
				rewind(_m280);
				inputState->guessing--;
			}
			if ( synPredMatched280 ) {
				id=qualified_id();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				if ( inputState->guessing==0 ) {
					declaratorID(id.c_str(),qiVar);
				}
				RefPNode tmp197_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp197_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp197_AST));
				}
				match(LPAREN);
				expression_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				RefPNode tmp198_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp198_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp198_AST));
				}
				match(RPAREN);
				direct_declarator_AST = RefPNode(currentAST.root);
			}
			else {
				bool synPredMatched282 = false;
				if (((_tokenSet_47.member(LA(1))) && (_tokenSet_77.member(LA(2))))) {
					int _m282 = mark();
					synPredMatched282 = true;
					inputState->guessing++;
					try {
						{
						qualified_id();
						match(LSQUARE);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched282 = false;
					}
					rewind(_m282);
					inputState->guessing--;
				}
				if ( synPredMatched282 ) {
					id=qualified_id();
					if (inputState->guessing==0) {
						QID_AST = returnAST;
					}
					if ( inputState->guessing==0 ) {
						QID_AST=returnAST;
					}
					if ( inputState->guessing==0 ) {
						if (_td==true)       // This statement is a typedef   
									declaratorID(id.c_str(),qiType);	// This statement is a typedef
								 else
									declaratorID(id.c_str(),qiVar);
								 is_address = false; is_pointer = false;
								
					}
					{ // ( ... )+
					int _cnt285=0;
					for (;;) {
						if ((LA(1) == LSQUARE)) {
							match(LSQUARE);
							{
							switch ( LA(1)) {
							case LITERAL_enum:
							case ID:
							case OCTALINT:
							case StringLiteral:
							case LITERAL_struct:
							case LITERAL_union:
							case LITERAL_class:
							case LPAREN:
							case LITERAL_typename:
							case LITERAL_char:
							case LITERAL_wchar_t:
							case LITERAL_bool:
							case LITERAL_short:
							case LITERAL_int:
							case 44:
							case 45:
							case 46:
							case 47:
							case 48:
							case 49:
							case 50:
							case 51:
							case 52:
							case 53:
							case LITERAL_long:
							case LITERAL_signed:
							case LITERAL_unsigned:
							case LITERAL_float:
							case LITERAL_double:
							case LITERAL_void:
							case OPERATOR:
							case TILDE:
							case LITERAL_typeid:
							case STAR:
							case AMPERSAND:
							case PLUS:
							case MINUS:
							case PLUSPLUS:
							case MINUSMINUS:
							case LITERAL_sizeof:
							case LITERAL___alignof__:
							case SCOPE:
							case LITERAL_dynamic_cast:
							case LITERAL_static_cast:
							case LITERAL_reinterpret_cast:
							case LITERAL_const_cast:
							case LITERAL_this:
							case DECIMALINT:
							case HEXADECIMALINT:
							case CharLiteral:
							case WCharLiteral:
							case WStringLiteral:
							case FLOATONE:
							case FLOATTWO:
							case LITERAL_true:
							case LITERAL_false:
							case NOT:
							case LITERAL_new:
							case LITERAL_delete:
							{
								constant_expression();
								if ( inputState->guessing==0 ) {
									astFactory->addASTChild(currentAST,ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								break;
							}
							case RSQUARE:
							{
								break;
							}
							default:
							{
								throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
							}
							}
							}
							match(RSQUARE);
						}
						else {
							if ( _cnt285>=1 ) { goto _loop285; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
						}
						
						_cnt285++;
					}
					_loop285:;
					}  // ( ... )+
					if ( inputState->guessing==0 ) {
						declaratorArray();
					}
					if ( inputState->guessing==0 ) {
						direct_declarator_AST = RefPNode(currentAST.root);
						direct_declarator_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(QID_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"array")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root))))))));
						currentAST.root = direct_declarator_AST;
						if ( direct_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
							direct_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
							  currentAST.child = direct_declarator_AST->getFirstChild();
						else
							currentAST.child = direct_declarator_AST;
						currentAST.advanceChildToEnd();
					}
				}
				else {
					bool synPredMatched287 = false;
					if (((_tokenSet_47.member(LA(1))) && (_tokenSet_78.member(LA(2))))) {
						int _m287 = mark();
						synPredMatched287 = true;
						inputState->guessing++;
						try {
							{
							qualified_id();
							match(RPAREN);
							match(LPAREN);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched287 = false;
						}
						rewind(_m287);
						inputState->guessing--;
					}
					if ( synPredMatched287 ) {
						id=qualified_id();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						if ( inputState->guessing==0 ) {
							if (_td==true)       // This statement is a typedef   
										declaratorID(id.c_str(),qiType);	// This statement is a typedef
									 else
										declaratorID(id.c_str(),qiFun);
									 is_address = false; is_pointer = false;
									
						}
						direct_declarator_AST = RefPNode(currentAST.root);
					}
					else if ((_tokenSet_47.member(LA(1))) && (_tokenSet_78.member(LA(2)))) {
						id=qualified_id();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						if ( inputState->guessing==0 ) {
							
									if (_td==true)
										{
										declaratorID(id.c_str(),qiType);	// This statement is a typedef
										}
									 else
										declaratorID(id.c_str(),qiVar);
									 is_address = false; is_pointer = false;
									
						}
						direct_declarator_AST = RefPNode(currentAST.root);
					}
					else if ((LA(1) == LPAREN)) {
						match(LPAREN);
						declarator();
						if (inputState->guessing==0) {
							d_AST = returnAST;
						}
						match(RPAREN);
						{
						if ((LA(1) == LPAREN || LA(1) == LSQUARE) && (_tokenSet_79.member(LA(2)))) {
							nType=declarator_suffix();
							if (inputState->guessing==0) {
								s_AST = returnAST;
							}
						}
						else if ((_tokenSet_71.member(LA(1))) && (_tokenSet_80.member(LA(2)))) {
						}
						else {
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						
						}
						if ( inputState->guessing==0 ) {
							direct_declarator_AST = RefPNode(currentAST.root);
							
									 if(nType == 1)
									 {
										d_AST->addChild(s_AST);
										direct_declarator_AST = d_AST;
									 }
									 else if(nType == 2)
										direct_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(d_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))));
									 else
										direct_declarator_AST = d_AST;
									
							currentAST.root = direct_declarator_AST;
							if ( direct_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
								direct_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
								  currentAST.child = direct_declarator_AST->getFirstChild();
							else
								currentAST.child = direct_declarator_AST;
							currentAST.advanceChildToEnd();
						}
					}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}}}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_71);
		} else {
			throw;
		}
	}
	returnAST = direct_declarator_AST;
}

void CPPParser::function_inline_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_inline_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data id;
		CPPParser::TypeQualifier tq;
		
	
	try {      // for error handling
		id=qualified_id();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			if (_td==true)       // This statement is a typedef   
						declaratorID(id.c_str(),qiType);
					 else
						declaratorID(id.c_str(),qiFun);
					
		}
		match(LPAREN);
		if ( inputState->guessing==0 ) {
			declaratorParameterList(0);
		}
		{
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case ELLIPSIS:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			parameter_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			declaratorEndParameterList(0);
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile)) {
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop321;
			}
			
		}
		_loop321:;
		} // ( ... )*
		{
		switch ( LA(1)) {
		case LITERAL_throw:
		{
			exception_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case GREATERTHAN:
		case SEMICOLON:
		case ASSIGNEQUAL:
		case LPAREN:
		case RPAREN:
		case COMMA:
		case ELLIPSIS:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			function_inline_declarator_AST = RefPNode(currentAST.root);
			function_inline_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYFUNCTION,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(function_inline_declarator_AST))));
			currentAST.root = function_inline_declarator_AST;
			if ( function_inline_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				function_inline_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = function_inline_declarator_AST->getFirstChild();
			else
				currentAST.child = function_inline_declarator_AST;
			currentAST.advanceChildToEnd();
		}
		function_inline_declarator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_71);
		} else {
			throw;
		}
	}
	returnAST = function_inline_declarator_AST;
}

int  CPPParser::declarator_suffix() {
	int nType = 0;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode declarator_suffix_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	CPPParser::TypeQualifier tq;
	
	try {      // for error handling
		{
		if ((LA(1) == LSQUARE)) {
			{ // ( ... )+
			int _cnt293=0;
			for (;;) {
				if ((LA(1) == LSQUARE)) {
					match(LSQUARE);
					{
					switch ( LA(1)) {
					case LITERAL_enum:
					case ID:
					case OCTALINT:
					case StringLiteral:
					case LITERAL_struct:
					case LITERAL_union:
					case LITERAL_class:
					case LPAREN:
					case LITERAL_typename:
					case LITERAL_char:
					case LITERAL_wchar_t:
					case LITERAL_bool:
					case LITERAL_short:
					case LITERAL_int:
					case 44:
					case 45:
					case 46:
					case 47:
					case 48:
					case 49:
					case 50:
					case 51:
					case 52:
					case 53:
					case LITERAL_long:
					case LITERAL_signed:
					case LITERAL_unsigned:
					case LITERAL_float:
					case LITERAL_double:
					case LITERAL_void:
					case OPERATOR:
					case TILDE:
					case LITERAL_typeid:
					case STAR:
					case AMPERSAND:
					case PLUS:
					case MINUS:
					case PLUSPLUS:
					case MINUSMINUS:
					case LITERAL_sizeof:
					case LITERAL___alignof__:
					case SCOPE:
					case LITERAL_dynamic_cast:
					case LITERAL_static_cast:
					case LITERAL_reinterpret_cast:
					case LITERAL_const_cast:
					case LITERAL_this:
					case DECIMALINT:
					case HEXADECIMALINT:
					case CharLiteral:
					case WCharLiteral:
					case WStringLiteral:
					case FLOATONE:
					case FLOATTWO:
					case LITERAL_true:
					case LITERAL_false:
					case NOT:
					case LITERAL_new:
					case LITERAL_delete:
					{
						constant_expression();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						break;
					}
					case RSQUARE:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					match(RSQUARE);
				}
				else {
					if ( _cnt293>=1 ) { goto _loop293; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt293++;
			}
			_loop293:;
			}  // ( ... )+
			if ( inputState->guessing==0 ) {
				declaratorArray();nType=1;
			}
			if ( inputState->guessing==0 ) {
				declarator_suffix_AST = RefPNode(currentAST.root);
				declarator_suffix_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"array")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(declarator_suffix_AST))));
				currentAST.root = declarator_suffix_AST;
				if ( declarator_suffix_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					declarator_suffix_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = declarator_suffix_AST->getFirstChild();
				else
					currentAST.child = declarator_suffix_AST;
				currentAST.advanceChildToEnd();
			}
		}
		else if (((LA(1) == LPAREN))&&((!((LA(1)==LPAREN)&&(LA(2)==ID))||(qualifiedItemIsOneOf(qiType|qiCtor,1))))) {
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				declaratorParameterList(0);
			}
			{
			switch ( LA(1)) {
			case LITERAL_typedef:
			case LITERAL_enum:
			case ID:
			case LITERAL_inline:
			case LITERAL_extern:
			case LITERAL_friend:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LITERAL__declspec:
			case LITERAL___declspec:
			case LPAREN:
			case LITERAL_auto:
			case LITERAL_register:
			case LITERAL_static:
			case LITERAL_mutable:
			case LITERAL__inline:
			case LITERAL___inline:
			case LITERAL_virtual:
			case LITERAL_explicit:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case LITERAL_const:
			case LITERAL_volatile:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case ELLIPSIS:
			case SCOPE:
			case LITERAL__cdecl:
			case LITERAL___cdecl:
			case LITERAL__near:
			case LITERAL___near:
			case LITERAL__far:
			case LITERAL___far:
			case LITERAL___interrupt:
			case LITERAL_pascal:
			case LITERAL__pascal:
			case LITERAL___pascal:
			case LITERAL__stdcall:
			case LITERAL___stdcall:
			{
				parameter_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(RPAREN);
			if ( inputState->guessing==0 ) {
				declaratorEndParameterList(0);
			}
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile)) {
					tq=type_qualifier();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					goto _loop296;
				}
				
			}
			_loop296:;
			} // ( ... )*
			{
			switch ( LA(1)) {
			case LITERAL_throw:
			{
				exception_specification();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case GREATERTHAN:
			case SEMICOLON:
			case ASSIGNEQUAL:
			case LPAREN:
			case RPAREN:
			case COMMA:
			case ELLIPSIS:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				nType = 2;
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		declarator_suffix_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_71);
		} else {
			throw;
		}
	}
	returnAST = declarator_suffix_AST;
	return nType;
}

void CPPParser::parameter_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode parameter_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		parameter_declaration_list();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case ELLIPSIS:
		{
			RefPNode tmp209_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp209_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp209_AST));
			}
			match(ELLIPSIS);
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			parameter_list_AST = RefPNode(currentAST.root);
			parameter_list_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYPARAM,"parameters")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(parameter_list_AST))));
			currentAST.root = parameter_list_AST;
			if ( parameter_list_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				parameter_list_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = parameter_list_AST->getFirstChild();
			else
				currentAST.child = parameter_list_AST;
			currentAST.advanceChildToEnd();
		}
		parameter_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_73);
		} else {
			throw;
		}
	}
	returnAST = parameter_list_AST;
}

void CPPParser::exception_specification() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode exception_specification_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	TypeSpecifier ts;
		
	
	try {      // for error handling
		RefPNode tmp210_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp210_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp210_AST));
		}
		match(LITERAL_throw);
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_enum:
		case ID:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case RPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case SCOPE:
		{
			{
			switch ( LA(1)) {
			case LITERAL_enum:
			case ID:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case SCOPE:
			{
				ts=simple_type_specifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				{
				switch ( LA(1)) {
				case ID:
				case STAR:
				case AMPERSAND:
				case SCOPE:
				case LITERAL__cdecl:
				case LITERAL___cdecl:
				case LITERAL__near:
				case LITERAL___near:
				case LITERAL__far:
				case LITERAL___far:
				case LITERAL___interrupt:
				case LITERAL_pascal:
				case LITERAL__pascal:
				case LITERAL___pascal:
				case LITERAL__stdcall:
				case LITERAL___stdcall:
				{
					ptr_operator();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case RPAREN:
				case COMMA:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				{ // ( ... )*
				for (;;) {
					if ((LA(1) == COMMA)) {
						match(COMMA);
						ts=simple_type_specifier();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
						{
						switch ( LA(1)) {
						case ID:
						case STAR:
						case AMPERSAND:
						case SCOPE:
						case LITERAL__cdecl:
						case LITERAL___cdecl:
						case LITERAL__near:
						case LITERAL___near:
						case LITERAL__far:
						case LITERAL___far:
						case LITERAL___interrupt:
						case LITERAL_pascal:
						case LITERAL__pascal:
						case LITERAL___pascal:
						case LITERAL__stdcall:
						case LITERAL___stdcall:
						{
							ptr_operator();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
							break;
						}
						case RPAREN:
						case COMMA:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
					}
					else {
						goto _loop384;
					}
					
				}
				_loop384:;
				} // ( ... )*
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case ELLIPSIS:
		{
			RefPNode tmp213_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp213_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp213_AST));
			}
			match(ELLIPSIS);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		exception_specification_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_81);
		} else {
			throw;
		}
	}
	returnAST = exception_specification_AST;
}

void CPPParser::conversion_type_modifier() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode conversion_type_modifier_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case STAR:
		{
			RefPNode tmp215_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp215_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp215_AST));
			}
			match(STAR);
			break;
		}
		case AMPERSAND:
		{
			RefPNode tmp216_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp216_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp216_AST));
			}
			match(AMPERSAND);
			break;
		}
		case LESSTHAN:
		case LPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			conversion_type_modifier_AST = RefPNode(currentAST.root);
			conversion_type_modifier_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"modifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(conversion_type_modifier_AST))));
			currentAST.root = conversion_type_modifier_AST;
			if ( conversion_type_modifier_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				conversion_type_modifier_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = conversion_type_modifier_AST->getFirstChild();
			else
				currentAST.child = conversion_type_modifier_AST;
			currentAST.advanceChildToEnd();
		}
		conversion_type_modifier_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_82);
		} else {
			throw;
		}
	}
	returnAST = conversion_type_modifier_AST;
}

void CPPParser::template_parameter_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_parameter_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if ( inputState->guessing==0 ) {
			beginTemplateParameterList();
		}
		template_parameter();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				template_parameter();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop388;
			}
			
		}
		_loop388:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			endTemplateParameterList();
		}
		if ( inputState->guessing==0 ) {
			template_parameter_list_AST = RefPNode(currentAST.root);
			template_parameter_list_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"templatelist")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(template_parameter_list_AST))));
			currentAST.root = template_parameter_list_AST;
			if ( template_parameter_list_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				template_parameter_list_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = template_parameter_list_AST->getFirstChild();
			else
				currentAST.child = template_parameter_list_AST;
			currentAST.advanceChildToEnd();
		}
		template_parameter_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_55);
		} else {
			throw;
		}
	}
	returnAST = template_parameter_list_AST;
}

void CPPParser::function_direct_declarator(
	int definition
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode function_direct_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data q;
		CPPParser::TypeQualifier tq;
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			match(LPAREN);
			declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(RPAREN);
			break;
		}
		case ID:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case SCOPE:
		{
			q=qualified_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				
							declaratorID(q.c_str(),qiFun);
							
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
					
			#ifdef MYCODE
					if (definition)
						myCode_function_direct_declarator(q.c_str());
			#endif MYCODE
					
		}
		match(LPAREN);
		if ( inputState->guessing==0 ) {
			
					functionParameterList();
					if (K_and_R == true)
						in_parameter_list = false;
					else
						in_parameter_list = true;
					
		}
		{
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case ELLIPSIS:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			parameter_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			
					if (K_and_R == true)
				  		in_parameter_list = true;
					else
						in_parameter_list = false;
					
		}
		match(RPAREN);
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile) && (_tokenSet_83.member(LA(2)))) {
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop315;
			}
			
		}
		_loop315:;
		} // ( ... )*
		{
		switch ( LA(1)) {
		case ASSIGNEQUAL:
		{
			match(ASSIGNEQUAL);
			match(OCTALINT);
			if ( inputState->guessing==0 ) {
				astFactory->addASTChild(currentAST,astFactory->create(MY,"pure"));
			}
			break;
		}
		case LITERAL_typedef:
		case LITERAL_enum:
		case SEMICOLON:
		case ID:
		case LCURLY:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_throw:
		case LITERAL_using:
		case SCOPE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			functionEndParameterList(definition);
		}
		{
		switch ( LA(1)) {
		case LITERAL_throw:
		{
			exception_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_typedef:
		case LITERAL_enum:
		case SEMICOLON:
		case ID:
		case LCURLY:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_using:
		case SCOPE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		function_direct_declarator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_26);
		} else {
			throw;
		}
	}
	returnAST = function_direct_declarator_AST;
}

void CPPParser::ctor_head() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_head_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ctor_decl_spec();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ctor_declarator(1);
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ctor_head_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_84);
		} else {
			throw;
		}
	}
	returnAST = ctor_head_AST;
}

void CPPParser::ctor_body() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_body_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case COLON:
		{
			ctor_initializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LCURLY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ctor_body_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_27);
		} else {
			throw;
		}
	}
	returnAST = ctor_body_AST;
}

data  CPPParser::qualified_ctor_id() {
	data q;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode qualified_ctor_id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		data so;
		char qitem03[CPPParser_MaxQualifiedItemSize+1];
		qitem03[0] = '\0';
		
	
	try {      // for error handling
		so=scope_override();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			strcpy(qitem03, so.c_str());
		}
		id = LT(1);
		if ( inputState->guessing == 0 ) {
			id_AST = astFactory->create(id);
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST));
		}
		match(ID);
		if ( inputState->guessing==0 ) {
			strcat(qitem03,(id->getText()).data());
					q = qitem03;
					//OUTPUT_DEBUG("CPP_parser.g qualified_ctor_id q %s\n",q);
					
		}
		qualified_ctor_id_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_85);
		} else {
			throw;
		}
	}
	returnAST = qualified_ctor_id_AST;
	return q;
}

void CPPParser::ctor_initializer() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ctor_initializer_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(COLON);
		superclass_init();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				superclass_init();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop338;
			}
			
		}
		_loop338:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			ctor_initializer_AST = RefPNode(currentAST.root);
			ctor_initializer_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"ctorinit")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ctor_initializer_AST))));
			currentAST.root = ctor_initializer_AST;
			if ( ctor_initializer_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				ctor_initializer_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = ctor_initializer_AST->getFirstChild();
			else
				currentAST.child = ctor_initializer_AST;
			currentAST.advanceChildToEnd();
		}
		ctor_initializer_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_56);
		} else {
			throw;
		}
	}
	returnAST = ctor_initializer_AST;
}

void CPPParser::superclass_init() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode superclass_init_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode QID_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode v_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data q;
	
	try {      // for error handling
		q=qualified_id();
		if (inputState->guessing==0) {
			QID_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			QID_AST=returnAST;
		}
		RefPNode tmp226_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp226_AST = astFactory->create(LT(1));
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_enum:
		case ID:
		case OCTALINT:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL_delete:
		{
			expression_list();
			if ( inputState->guessing==0 ) {
				v_AST=returnAST;
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		RefPNode tmp227_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp227_AST = astFactory->create(LT(1));
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			superclass_init_AST = RefPNode(currentAST.root);
			superclass_init_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(QID_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"values")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(v_AST))))))));
			currentAST.root = superclass_init_AST;
			if ( superclass_init_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				superclass_init_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = superclass_init_AST->getFirstChild();
			else
				currentAST.child = superclass_init_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_66);
		} else {
			throw;
		}
	}
	returnAST = superclass_init_AST;
}

void CPPParser::dtor_decl_spec() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dtor_decl_spec_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			switch ( LA(1)) {
			case LITERAL_inline:
			case LITERAL__inline:
			case LITERAL___inline:
			{
				{
				switch ( LA(1)) {
				case LITERAL_inline:
				{
					RefPNode tmp228_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp228_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp228_AST));
					}
					match(LITERAL_inline);
					break;
				}
				case LITERAL__inline:
				{
					RefPNode tmp229_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp229_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp229_AST));
					}
					match(LITERAL__inline);
					break;
				}
				case LITERAL___inline:
				{
					RefPNode tmp230_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp230_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp230_AST));
					}
					match(LITERAL___inline);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case LITERAL_virtual:
			{
				RefPNode tmp231_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp231_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp231_AST));
				}
				match(LITERAL_virtual);
				break;
			}
			default:
			{
				goto _loop345;
			}
			}
		}
		_loop345:;
		} // ( ... )*
		if ( inputState->guessing==0 ) {
			dtor_decl_spec_AST = RefPNode(currentAST.root);
			dtor_decl_spec_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"specifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(dtor_decl_spec_AST))));
			currentAST.root = dtor_decl_spec_AST;
			if ( dtor_decl_spec_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				dtor_decl_spec_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = dtor_decl_spec_AST->getFirstChild();
			else
				currentAST.child = dtor_decl_spec_AST;
			currentAST.advanceChildToEnd();
		}
		dtor_decl_spec_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_86);
		} else {
			throw;
		}
	}
	returnAST = dtor_decl_spec_AST;
}

void CPPParser::dtor_declarator(
	int definition
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dtor_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data s;
	
	try {      // for error handling
		s=scope_override();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(TILDE);
		RefPNode tmp233_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp233_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp233_AST));
		}
		match(ID);
		if ( inputState->guessing==0 ) {
			declaratorParameterList(definition);
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_void:
		{
			RefPNode tmp235_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp235_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp235_AST));
			}
			match(LITERAL_void);
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			declaratorEndParameterList(definition);
		}
		{
		switch ( LA(1)) {
		case LITERAL_throw:
		{
			exception_specification();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		case LCURLY:
		case ASSIGNEQUAL:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		dtor_declarator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_34);
		} else {
			throw;
		}
	}
	returnAST = dtor_declarator_AST;
}

void CPPParser::parameter_declaration_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode parameter_declaration_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		parameter_declaration();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				parameter_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop355;
			}
			
		}
		_loop355:;
		} // ( ... )*
		}
		parameter_declaration_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_87);
		} else {
			throw;
		}
	}
	returnAST = parameter_declaration_list_AST;
}

void CPPParser::parameter_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode parameter_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if ( inputState->guessing==0 ) {
			beginParameterDeclaration();
		}
		{
		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_88.member(LA(2))))&&(!((LA(1)==SCOPE) && (LA(2)==STAR||LA(2)==OPERATOR)) &&
			 (!(LA(1)==SCOPE||LA(1)==ID) || qualifiedItemIsOneOf(qiType|qiCtor) ))) {
			declaration_specifiers();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			bool synPredMatched361 = false;
			if (((_tokenSet_39.member(LA(1))) && (_tokenSet_89.member(LA(2))))) {
				int _m361 = mark();
				synPredMatched361 = true;
				inputState->guessing++;
				try {
					{
					declarator();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched361 = false;
				}
				rewind(_m361);
				inputState->guessing--;
			}
			if ( synPredMatched361 ) {
				declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((_tokenSet_90.member(LA(1))) && (_tokenSet_91.member(LA(2)))) {
				abstract_declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
		}
		else {
			bool synPredMatched363 = false;
			if (((_tokenSet_39.member(LA(1))) && (_tokenSet_89.member(LA(2))))) {
				int _m363 = mark();
				synPredMatched363 = true;
				inputState->guessing++;
				try {
					{
					declarator();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched363 = false;
				}
				rewind(_m363);
				inputState->guessing--;
			}
			if ( synPredMatched363 ) {
				declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((LA(1) == ELLIPSIS)) {
				RefPNode tmp238_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp238_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp238_AST));
				}
				match(ELLIPSIS);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ASSIGNEQUAL:
		{
			match(ASSIGNEQUAL);
			parameter_initializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case GREATERTHAN:
		case RPAREN:
		case COMMA:
		case ELLIPSIS:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			parameter_declaration_AST = RefPNode(currentAST.root);
			parameter_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"declaration")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(parameter_declaration_AST))));
			currentAST.root = parameter_declaration_AST;
			if ( parameter_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				parameter_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = parameter_declaration_AST->getFirstChild();
			else
				currentAST.child = parameter_declaration_AST;
			currentAST.advanceChildToEnd();
		}
		parameter_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_92);
		} else {
			throw;
		}
	}
	returnAST = parameter_declaration_AST;
}

void CPPParser::parameter_initializer() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode parameter_initializer_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		remainder_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			parameter_initializer_AST = RefPNode(currentAST.root);
			parameter_initializer_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(parameter_initializer_AST))));
			currentAST.root = parameter_initializer_AST;
			if ( parameter_initializer_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				parameter_initializer_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = parameter_initializer_AST->getFirstChild();
			else
				currentAST.child = parameter_initializer_AST;
			currentAST.advanceChildToEnd();
		}
		parameter_initializer_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_92);
		} else {
			throw;
		}
	}
	returnAST = parameter_initializer_AST;
}

void CPPParser::abstract_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode abstract_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if ((_tokenSet_22.member(LA(1)))) {
			ptr_operator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			abstract_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			abstract_declarator_AST = RefPNode(currentAST.root);
		}
		else {
			bool synPredMatched369 = false;
			if (((LA(1) == LPAREN) && (_tokenSet_93.member(LA(2))))) {
				int _m369 = mark();
				synPredMatched369 = true;
				inputState->guessing++;
				try {
					{
					match(LPAREN);
					abstract_declarator();
					match(RPAREN);
					{
					switch ( LA(1)) {
					case LSQUARE:
					{
						match(LSQUARE);
						break;
					}
					case LPAREN:
					{
						match(LPAREN);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched369 = false;
				}
				rewind(_m369);
				inputState->guessing--;
			}
			if ( synPredMatched369 ) {
				RefPNode tmp240_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp240_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp240_AST));
				}
				match(LPAREN);
				abstract_declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				RefPNode tmp241_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp241_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp241_AST));
				}
				match(RPAREN);
				{
				abstract_declarator_suffix();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				}
				abstract_declarator_AST = RefPNode(currentAST.root);
			}
			else if ((_tokenSet_94.member(LA(1))) && (_tokenSet_95.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case LPAREN:
				case LSQUARE:
				{
					abstract_declarator_suffix();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case GREATERTHAN:
				case ASSIGNEQUAL:
				case RPAREN:
				case COMMA:
				case ELLIPSIS:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				abstract_declarator_AST = RefPNode(currentAST.root);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_96);
		} else {
			throw;
		}
	}
	returnAST = abstract_declarator_AST;
}

void CPPParser::type_id() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode type_id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode a_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		declaration_specifiers();
		if (inputState->guessing==0) {
			s_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			s_AST=returnAST;
		}
		abstract_declarator();
		if (inputState->guessing==0) {
			a_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			a_AST=returnAST;
		}
		if ( inputState->guessing==0 ) {
			type_id_AST = RefPNode(currentAST.root);
			
			//		 #s->addChild(RefPNode(#a));
					 type_id_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYTYPE,"typeid")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(a_AST))));
					
			currentAST.root = type_id_AST;
			if ( type_id_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				type_id_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = type_id_AST->getFirstChild();
			else
				currentAST.child = type_id_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_97);
		} else {
			throw;
		}
	}
	returnAST = type_id_AST;
}

void CPPParser::abstract_declarator_suffix() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode abstract_declarator_suffix_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode c_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LSQUARE:
		{
			{ // ( ... )+
			int _cnt375=0;
			for (;;) {
				if ((LA(1) == LSQUARE)) {
					match(LSQUARE);
					{
					switch ( LA(1)) {
					case LITERAL_enum:
					case ID:
					case OCTALINT:
					case StringLiteral:
					case LITERAL_struct:
					case LITERAL_union:
					case LITERAL_class:
					case LPAREN:
					case LITERAL_typename:
					case LITERAL_char:
					case LITERAL_wchar_t:
					case LITERAL_bool:
					case LITERAL_short:
					case LITERAL_int:
					case 44:
					case 45:
					case 46:
					case 47:
					case 48:
					case 49:
					case 50:
					case 51:
					case 52:
					case 53:
					case LITERAL_long:
					case LITERAL_signed:
					case LITERAL_unsigned:
					case LITERAL_float:
					case LITERAL_double:
					case LITERAL_void:
					case OPERATOR:
					case TILDE:
					case LITERAL_typeid:
					case STAR:
					case AMPERSAND:
					case PLUS:
					case MINUS:
					case PLUSPLUS:
					case MINUSMINUS:
					case LITERAL_sizeof:
					case LITERAL___alignof__:
					case SCOPE:
					case LITERAL_dynamic_cast:
					case LITERAL_static_cast:
					case LITERAL_reinterpret_cast:
					case LITERAL_const_cast:
					case LITERAL_this:
					case DECIMALINT:
					case HEXADECIMALINT:
					case CharLiteral:
					case WCharLiteral:
					case WStringLiteral:
					case FLOATONE:
					case FLOATTWO:
					case LITERAL_true:
					case LITERAL_false:
					case NOT:
					case LITERAL_new:
					case LITERAL_delete:
					{
						constant_expression();
						if ( inputState->guessing==0 ) {
							c_AST=returnAST;
						}
						break;
					}
					case RSQUARE:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					match(RSQUARE);
				}
				else {
					if ( _cnt375>=1 ) { goto _loop375; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt375++;
			}
			_loop375:;
			}  // ( ... )+
			if ( inputState->guessing==0 ) {
				declaratorArray();
			}
			if ( inputState->guessing==0 ) {
				abstract_declarator_suffix_AST = RefPNode(currentAST.root);
				abstract_declarator_suffix_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"modifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYDECLAR,"array")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(c_AST))))))));
				currentAST.root = abstract_declarator_suffix_AST;
				if ( abstract_declarator_suffix_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					abstract_declarator_suffix_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = abstract_declarator_suffix_AST->getFirstChild();
				else
					currentAST.child = abstract_declarator_suffix_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LPAREN:
		{
			RefPNode tmp244_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp244_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp244_AST));
			}
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				declaratorParameterList(0);
			}
			{
			switch ( LA(1)) {
			case LITERAL_typedef:
			case LITERAL_enum:
			case ID:
			case LITERAL_inline:
			case LITERAL_extern:
			case LITERAL_friend:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LITERAL__declspec:
			case LITERAL___declspec:
			case LPAREN:
			case LITERAL_auto:
			case LITERAL_register:
			case LITERAL_static:
			case LITERAL_mutable:
			case LITERAL__inline:
			case LITERAL___inline:
			case LITERAL_virtual:
			case LITERAL_explicit:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case LITERAL_const:
			case LITERAL_volatile:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case ELLIPSIS:
			case SCOPE:
			case LITERAL__cdecl:
			case LITERAL___cdecl:
			case LITERAL__near:
			case LITERAL___near:
			case LITERAL__far:
			case LITERAL___far:
			case LITERAL___interrupt:
			case LITERAL_pascal:
			case LITERAL__pascal:
			case LITERAL___pascal:
			case LITERAL__stdcall:
			case LITERAL___stdcall:
			{
				parameter_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			RefPNode tmp245_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp245_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp245_AST));
			}
			match(RPAREN);
			if ( inputState->guessing==0 ) {
				declaratorEndParameterList(0);
			}
			cv_qualifier_seq();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			switch ( LA(1)) {
			case LITERAL_throw:
			{
				exception_specification();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case GREATERTHAN:
			case ASSIGNEQUAL:
			case RPAREN:
			case COMMA:
			case ELLIPSIS:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			abstract_declarator_suffix_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_96);
		} else {
			throw;
		}
	}
	returnAST = abstract_declarator_suffix_AST;
}

void CPPParser::cv_qualifier_seq() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode cv_qualifier_seq_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	CPPParser::TypeQualifier tq;
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_const || LA(1) == LITERAL_volatile)) {
				tq=type_qualifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop603;
			}
			
		}
		_loop603:;
		} // ( ... )*
		cv_qualifier_seq_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_98);
		} else {
			throw;
		}
	}
	returnAST = cv_qualifier_seq_AST;
}

void CPPParser::template_parameter() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_parameter_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		if ((_tokenSet_99.member(LA(1))) && (_tokenSet_100.member(LA(2)))) {
			type_parameter();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			bool synPredMatched392 = false;
			if (((_tokenSet_101.member(LA(1))) && (_tokenSet_15.member(LA(2))))) {
				int _m392 = mark();
				synPredMatched392 = true;
				inputState->guessing++;
				try {
					{
					parameter_declaration();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched392 = false;
				}
				rewind(_m392);
				inputState->guessing--;
			}
			if ( synPredMatched392 ) {
				parameter_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((_tokenSet_101.member(LA(1))) && (_tokenSet_15.member(LA(2)))) {
				template_parameter_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		template_parameter_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_102);
		} else {
			throw;
		}
	}
	returnAST = template_parameter_AST;
}

void CPPParser::type_parameter() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode type_parameter_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode as_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode mod_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_class:
		case LITERAL_typename:
		{
			{
			switch ( LA(1)) {
			case LITERAL_class:
			{
				match(LITERAL_class);
				break;
			}
			case LITERAL_typename:
			{
				match(LITERAL_typename);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case ID:
			{
				id = LT(1);
				if ( inputState->guessing == 0 ) {
					id_AST = astFactory->create(id);
				}
				match(ID);
				if ( inputState->guessing==0 ) {
					
									templateTypeParameter((id->getText()).data());
									
				}
				{
				switch ( LA(1)) {
				case ASSIGNEQUAL:
				{
					match(ASSIGNEQUAL);
					assigned_type_name();
					if ( inputState->guessing==0 ) {
						as_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
					}
					break;
				}
				case GREATERTHAN:
				case COMMA:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case GREATERTHAN:
			case COMMA:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				type_parameter_AST = RefPNode(currentAST.root);
				type_parameter_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(as_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(mod_AST))));
				currentAST.root = type_parameter_AST;
				if ( type_parameter_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					type_parameter_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = type_parameter_AST->getFirstChild();
				else
					currentAST.child = type_parameter_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LITERAL_template:
		{
			template_head();
			if (inputState->guessing==0) {
				mod_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				mod_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"modifier")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
			}
			match(LITERAL_class);
			{
			switch ( LA(1)) {
			case ID:
			{
				id2 = LT(1);
				if ( inputState->guessing == 0 ) {
					id2_AST = astFactory->create(id2);
				}
				match(ID);
				if ( inputState->guessing==0 ) {
					
									templateTypeParameter((id2->getText()).data());
									
				}
				{
				switch ( LA(1)) {
				case ASSIGNEQUAL:
				{
					match(ASSIGNEQUAL);
					assigned_type_name();
					if ( inputState->guessing==0 ) {
						as_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"assign")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST))));
					}
					break;
				}
				case GREATERTHAN:
				case COMMA:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case GREATERTHAN:
			case COMMA:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			if ( inputState->guessing==0 ) {
				type_parameter_AST = RefPNode(currentAST.root);
				type_parameter_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id2_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(as_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(mod_AST))));
				currentAST.root = type_parameter_AST;
				if ( type_parameter_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					type_parameter_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = type_parameter_AST->getFirstChild();
				else
					currentAST.child = type_parameter_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_102);
		} else {
			throw;
		}
	}
	returnAST = type_parameter_AST;
}

void CPPParser::template_parameter_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_parameter_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if ( inputState->guessing==0 ) {
			beginParameterDeclaration();
		}
		{
		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_103.member(LA(2))))&&(!((LA(1)==SCOPE) && (LA(2)==STAR||LA(2)==OPERATOR)) &&
			 (!(LA(1)==SCOPE||LA(1)==ID) || qualifiedItemIsOneOf(qiType|qiCtor) ))) {
			declaration_specifiers();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			bool synPredMatched406 = false;
			if (((_tokenSet_39.member(LA(1))) && (_tokenSet_40.member(LA(2))))) {
				int _m406 = mark();
				synPredMatched406 = true;
				inputState->guessing++;
				try {
					{
					declarator();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched406 = false;
				}
				rewind(_m406);
				inputState->guessing--;
			}
			if ( synPredMatched406 ) {
				declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((_tokenSet_104.member(LA(1))) && (_tokenSet_105.member(LA(2)))) {
				abstract_declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
		}
		else {
			bool synPredMatched408 = false;
			if (((_tokenSet_39.member(LA(1))) && (_tokenSet_40.member(LA(2))))) {
				int _m408 = mark();
				synPredMatched408 = true;
				inputState->guessing++;
				try {
					{
					declarator();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched408 = false;
				}
				rewind(_m408);
				inputState->guessing--;
			}
			if ( synPredMatched408 ) {
				declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((LA(1) == ELLIPSIS)) {
				RefPNode tmp251_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp251_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp251_AST));
				}
				match(ELLIPSIS);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ASSIGNEQUAL:
		{
			RefPNode tmp252_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp252_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp252_AST));
			}
			match(ASSIGNEQUAL);
			additive_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case GREATERTHAN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		template_parameter_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_102);
		} else {
			throw;
		}
	}
	returnAST = template_parameter_declaration_AST;
}

void CPPParser::assigned_type_name() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode assigned_type_name_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data qt; TypeSpecifier ts;
	
	try {      // for error handling
		{
		if ((LA(1) == ID || LA(1) == SCOPE) && (_tokenSet_106.member(LA(2)))) {
			qt=qualified_type();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			abstract_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((_tokenSet_57.member(LA(1))) && (_tokenSet_107.member(LA(2)))) {
			ts=simple_type_specifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			abstract_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		assigned_type_name_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_102);
		} else {
			throw;
		}
	}
	returnAST = assigned_type_name_AST;
}

void CPPParser::additive_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode additive_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		multiplicative_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == PLUS || LA(1) == MINUS)) {
				{
				switch ( LA(1)) {
				case PLUS:
				{
					RefPNode tmp253_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp253_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp253_AST));
					}
					match(PLUS);
					break;
				}
				case MINUS:
				{
					RefPNode tmp254_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp254_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp254_AST));
					}
					match(MINUS);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				multiplicative_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop522;
			}
			
		}
		_loop522:;
		} // ( ... )*
		additive_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_108);
		} else {
			throw;
		}
	}
	returnAST = additive_expression_AST;
}

void CPPParser::template_id() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp255_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp255_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp255_AST));
		}
		match(ID);
		RefPNode tmp256_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp256_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp256_AST));
		}
		match(LESSTHAN);
		template_argument_list();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		RefPNode tmp257_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp257_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp257_AST));
		}
		match(GREATERTHAN);
		template_id_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_1);
		} else {
			throw;
		}
	}
	returnAST = template_id_AST;
}

void CPPParser::template_argument() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode template_argument_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_109.member(LA(2))))&&(( !(LA(1)==SCOPE||LA(1)==ID) || qualifiedItemIsOneOf(qiType|qiCtor) ))) {
			type_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			template_argument_AST = RefPNode(currentAST.root);
		}
		else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_110.member(LA(2)))) {
			shift_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			template_argument_AST = RefPNode(currentAST.root);
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_102);
		} else {
			throw;
		}
	}
	returnAST = template_argument_AST;
}

void CPPParser::shift_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode shift_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		additive_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == SHIFTLEFT || LA(1) == SHIFTRIGHT)) {
				{
				switch ( LA(1)) {
				case SHIFTLEFT:
				{
					RefPNode tmp258_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp258_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp258_AST));
					}
					match(SHIFTLEFT);
					break;
				}
				case SHIFTRIGHT:
				{
					RefPNode tmp259_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp259_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp259_AST));
					}
					match(SHIFTRIGHT);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				additive_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop518;
			}
			
		}
		_loop518:;
		} // ( ... )*
		shift_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_111);
		} else {
			throw;
		}
	}
	returnAST = shift_expression_AST;
}

void CPPParser::statement_list() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode statement_list_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )+
		int _cnt417=0;
		for (;;) {
			if ((_tokenSet_112.member(LA(1)))) {
				statement();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				if ( _cnt417>=1 ) { goto _loop417; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt417++;
		}
		_loop417:;
		}  // ( ... )+
		statement_list_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_67);
		} else {
			throw;
		}
	}
	returnAST = statement_list_AST;
}

void CPPParser::statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		lineNo = LT(1)->getLine();
		FunctionSpecifier fs = fsInvalid;	// inline,virtual,explicit
		
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_case:
		{
			case_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_default:
		{
			default_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LCURLY:
		{
			compound_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_if:
		case LITERAL_switch:
		{
			selection_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_while:
		case LITERAL_do:
		case LITERAL_for:
		{
			iteration_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_goto:
		case LITERAL_continue:
		case LITERAL_break:
		case LITERAL_return:
		{
			jump_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_try:
		{
			try_block();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL___try:
		{
			ep_try_block();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_throw:
		{
			throw_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		default:
			bool synPredMatched421 = false;
			if (((_tokenSet_113.member(LA(1))) && (_tokenSet_114.member(LA(2))))) {
				int _m421 = mark();
				synPredMatched421 = true;
				inputState->guessing++;
				try {
					{
					switch ( LA(1)) {
					case LITERAL_namespace:
					{
						match(LITERAL_namespace);
						break;
					}
					case LITERAL_using:
					{
						match(LITERAL_using);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched421 = false;
				}
				rewind(_m421);
				inputState->guessing--;
			}
			if ( synPredMatched421 ) {
				block_declaration();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				bool synPredMatched425 = false;
				if (((_tokenSet_32.member(LA(1))) && (_tokenSet_115.member(LA(2))))) {
					int _m425 = mark();
					synPredMatched425 = true;
					inputState->guessing++;
					try {
						{
						{
						switch ( LA(1)) {
						case LITERAL_typedef:
						{
							match(LITERAL_typedef);
							break;
						}
						case LITERAL_struct:
						case LITERAL_union:
						case LITERAL_class:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						class_specifier();
						{
						if ((_tokenSet_47.member(LA(1)))) {
							qualified_id();
						}
						else {
						}
						
						}
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched425 = false;
					}
					rewind(_m425);
					inputState->guessing--;
				}
				if ( synPredMatched425 ) {
					member_declaration();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					bool synPredMatched427 = false;
					if (((_tokenSet_32.member(LA(1))) && (_tokenSet_115.member(LA(2))))) {
						int _m427 = mark();
						synPredMatched427 = true;
						inputState->guessing++;
						try {
							{
							declaration_specifiers();
							match(LPAREN);
							ptr_operator();
							qualified_id();
							match(RPAREN);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched427 = false;
						}
						rewind(_m427);
						inputState->guessing--;
					}
					if ( synPredMatched427 ) {
						member_declaration();
						if (inputState->guessing==0) {
							astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
						}
					}
					else {
						bool synPredMatched430 = false;
						if (((_tokenSet_32.member(LA(1))) && (_tokenSet_115.member(LA(2))))) {
							int _m430 = mark();
							synPredMatched430 = true;
							inputState->guessing++;
							try {
								{
								declaration_specifiers();
								{
								ptr_operator();
								ptr_operator();
								}
								qualified_id();
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched430 = false;
							}
							rewind(_m430);
							inputState->guessing--;
						}
						if ( synPredMatched430 ) {
							member_declaration();
							if (inputState->guessing==0) {
								astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
							}
						}
						else {
							bool synPredMatched435 = false;
							if (((_tokenSet_32.member(LA(1))) && (_tokenSet_115.member(LA(2))))) {
								int _m435 = mark();
								synPredMatched435 = true;
								inputState->guessing++;
								try {
									{
									declaration_specifiers();
									{
									bool synPredMatched434 = false;
									if (((_tokenSet_22.member(LA(1))) && (_tokenSet_116.member(LA(2))))) {
										int _m434 = mark();
										synPredMatched434 = true;
										inputState->guessing++;
										try {
											{
											ptr_operator();
											}
										}
										catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
											synPredMatched434 = false;
										}
										rewind(_m434);
										inputState->guessing--;
									}
									if ( synPredMatched434 ) {
										ptr_operator();
									}
									else if ((_tokenSet_47.member(LA(1))) && (true)) {
									}
									else {
										throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
									}
									
									}
									qualified_id();
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched435 = false;
								}
								rewind(_m435);
								inputState->guessing--;
							}
							if ( synPredMatched435 ) {
								member_declaration();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else if ((LA(1) == ID) && (LA(2) == COLON)) {
								labeled_statement();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
							}
							else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_117.member(LA(2)))) {
								expression();
								if (inputState->guessing==0) {
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
								}
								match(SEMICOLON);
								if ( inputState->guessing==0 ) {
									end_of_stmt();
								}
								if ( inputState->guessing==0 ) {
									statement_AST = RefPNode(currentAST.root);
									if(statement_AST != nullAST) statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"expression")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(statement_AST))));
									currentAST.root = statement_AST;
									if ( statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
										statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
										  currentAST.child = statement_AST->getFirstChild();
									else
										currentAST.child = statement_AST;
									currentAST.advanceChildToEnd();
								}
							}
							else if ((LA(1) == SEMICOLON) && (_tokenSet_41.member(LA(2)))) {
								match(SEMICOLON);
								if ( inputState->guessing==0 ) {
									end_of_stmt();
								}
							}
							else if ((LA(1) == LITERAL_antlrTrace_on) && (_tokenSet_41.member(LA(2)))) {
								RefPNode tmp262_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
								if ( inputState->guessing == 0 ) {
									tmp262_AST = astFactory->create(LT(1));
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp262_AST));
								}
								match(LITERAL_antlrTrace_on);
								if ( inputState->guessing==0 ) {
									antlrTrace(true);
								}
							}
							else if ((LA(1) == LITERAL_antlrTrace_off) && (_tokenSet_41.member(LA(2)))) {
								RefPNode tmp263_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
								if ( inputState->guessing == 0 ) {
									tmp263_AST = astFactory->create(LT(1));
									astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp263_AST));
								}
								match(LITERAL_antlrTrace_off);
								if ( inputState->guessing==0 ) {
									antlrTrace(false);
								}
							}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}}}}}
		}
		statement_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = statement_AST;
}

void CPPParser::block_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode block_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case SCOPE:
		{
			simple_declaration();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			block_declaration_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_namespace:
		{
			namespace_alias_definition();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			block_declaration_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_using:
		{
			using_statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			block_declaration_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = block_declaration_AST;
}

void CPPParser::labeled_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode labeled_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		id = LT(1);
		if ( inputState->guessing == 0 ) {
			id_AST = astFactory->create(id);
		}
		match(ID);
		RefPNode tmp264_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp264_AST = astFactory->create(LT(1));
		}
		match(COLON);
		statement();
		if (inputState->guessing==0) {
			s_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			labeled_statement_AST = RefPNode(currentAST.root);
			labeled_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"label")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST))));
					 labeled_statement_AST->setNextSibling(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST));
					
			currentAST.root = labeled_statement_AST;
			if ( labeled_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				labeled_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = labeled_statement_AST->getFirstChild();
			else
				currentAST.child = labeled_statement_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = labeled_statement_AST;
}

void CPPParser::case_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode case_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode ce_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LITERAL_case);
		constant_expression();
		if (inputState->guessing==0) {
			ce_AST = returnAST;
		}
		RefPNode tmp266_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp266_AST = astFactory->create(LT(1));
		}
		match(COLON);
		statement();
		if (inputState->guessing==0) {
			s_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			case_statement_AST = RefPNode(currentAST.root);
			case_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"value")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ce_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))))))));
			currentAST.root = case_statement_AST;
			if ( case_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				case_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = case_statement_AST->getFirstChild();
			else
				currentAST.child = case_statement_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = case_statement_AST;
}

void CPPParser::default_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode default_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LITERAL_default);
		RefPNode tmp268_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp268_AST = astFactory->create(LT(1));
		}
		match(COLON);
		statement();
		if (inputState->guessing==0) {
			s_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			default_statement_AST = RefPNode(currentAST.root);
			default_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"default")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))))))));
			currentAST.root = default_statement_AST;
			if ( default_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				default_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = default_statement_AST->getFirstChild();
			else
				currentAST.child = default_statement_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = default_statement_AST;
}

void CPPParser::selection_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode selection_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode condition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode left_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode right_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_if:
		{
			match(LITERAL_if);
			RefPNode tmp270_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp270_AST = astFactory->create(LT(1));
			}
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				enterNewLocalScope();
			}
			condition();
			if (inputState->guessing==0) {
				condition_AST = returnAST;
			}
			RefPNode tmp271_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp271_AST = astFactory->create(LT(1));
			}
			match(RPAREN);
			statement();
			if (inputState->guessing==0) {
				left_AST = returnAST;
			}
			{
			if ((LA(1) == LITERAL_else) && (_tokenSet_112.member(LA(2)))) {
				match(LITERAL_else);
				statement();
				if (inputState->guessing==0) {
					right_AST = returnAST;
				}
			}
			else if ((_tokenSet_41.member(LA(1))) && (_tokenSet_118.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			if ( inputState->guessing==0 ) {
				exitLocalScope();
			}
			if ( inputState->guessing==0 ) {
				selection_statement_AST = RefPNode(currentAST.root);
				selection_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(4))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYSTATEMENT,"if")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(condition_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"left")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(left_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYEXPRESSION,"right")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(right_AST))))))));
				currentAST.root = selection_statement_AST;
				if ( selection_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					selection_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = selection_statement_AST->getFirstChild();
				else
					currentAST.child = selection_statement_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LITERAL_switch:
		{
			RefPNode tmp273_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp273_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp273_AST));
			}
			match(LITERAL_switch);
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				enterNewLocalScope();
			}
			condition();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(RPAREN);
			statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				exitLocalScope();
			}
			selection_statement_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = selection_statement_AST;
}

void CPPParser::iteration_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode iteration_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode c0_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s0_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s1_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode e0_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode d_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode e1_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode c_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode e2_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case LITERAL_while:
		{
			match(LITERAL_while);
			RefPNode tmp277_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp277_AST = astFactory->create(LT(1));
			}
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				enterNewLocalScope();
			}
			condition();
			if (inputState->guessing==0) {
				c0_AST = returnAST;
			}
			RefPNode tmp278_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp278_AST = astFactory->create(LT(1));
			}
			match(RPAREN);
			statement();
			if (inputState->guessing==0) {
				s0_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				exitLocalScope();
			}
			if ( inputState->guessing==0 ) {
				iteration_statement_AST = RefPNode(currentAST.root);
				iteration_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYSTATEMENT,"while")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(c0_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s0_AST))))))));
				currentAST.root = iteration_statement_AST;
				if ( iteration_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					iteration_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = iteration_statement_AST->getFirstChild();
				else
					currentAST.child = iteration_statement_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LITERAL_do:
		{
			match(LITERAL_do);
			if ( inputState->guessing==0 ) {
				enterNewLocalScope();
			}
			statement();
			if (inputState->guessing==0) {
				s1_AST = returnAST;
			}
			match(LITERAL_while);
			RefPNode tmp281_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp281_AST = astFactory->create(LT(1));
			}
			match(LPAREN);
			expression();
			if (inputState->guessing==0) {
				e0_AST = returnAST;
			}
			RefPNode tmp282_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp282_AST = astFactory->create(LT(1));
			}
			match(RPAREN);
			if ( inputState->guessing==0 ) {
				exitLocalScope();
			}
			RefPNode tmp283_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp283_AST = astFactory->create(LT(1));
			}
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			if ( inputState->guessing==0 ) {
				iteration_statement_AST = RefPNode(currentAST.root);
				iteration_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYSTATEMENT,"do")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s1_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"while")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(e0_AST))))))));
				currentAST.root = iteration_statement_AST;
				if ( iteration_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					iteration_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = iteration_statement_AST->getFirstChild();
				else
					currentAST.child = iteration_statement_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		case LITERAL_for:
		{
			match(LITERAL_for);
			RefPNode tmp285_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp285_AST = astFactory->create(LT(1));
			}
			match(LPAREN);
			if ( inputState->guessing==0 ) {
				enterNewLocalScope();
			}
			{
			bool synPredMatched449 = false;
			if (((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2))))) {
				int _m449 = mark();
				synPredMatched449 = true;
				inputState->guessing++;
				try {
					{
					declaration();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched449 = false;
				}
				rewind(_m449);
				inputState->guessing--;
			}
			if ( synPredMatched449 ) {
				declaration();
				if (inputState->guessing==0) {
					d_AST = returnAST;
				}
			}
			else if ((_tokenSet_119.member(LA(1))) && (_tokenSet_117.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case LITERAL_enum:
				case ID:
				case OCTALINT:
				case StringLiteral:
				case LITERAL_struct:
				case LITERAL_union:
				case LITERAL_class:
				case LPAREN:
				case LITERAL_typename:
				case LITERAL_char:
				case LITERAL_wchar_t:
				case LITERAL_bool:
				case LITERAL_short:
				case LITERAL_int:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case LITERAL_long:
				case LITERAL_signed:
				case LITERAL_unsigned:
				case LITERAL_float:
				case LITERAL_double:
				case LITERAL_void:
				case OPERATOR:
				case TILDE:
				case LITERAL_typeid:
				case STAR:
				case AMPERSAND:
				case PLUS:
				case MINUS:
				case PLUSPLUS:
				case MINUSMINUS:
				case LITERAL_sizeof:
				case LITERAL___alignof__:
				case SCOPE:
				case LITERAL_dynamic_cast:
				case LITERAL_static_cast:
				case LITERAL_reinterpret_cast:
				case LITERAL_const_cast:
				case LITERAL_this:
				case DECIMALINT:
				case HEXADECIMALINT:
				case CharLiteral:
				case WCharLiteral:
				case WStringLiteral:
				case FLOATONE:
				case FLOATTWO:
				case LITERAL_true:
				case LITERAL_false:
				case NOT:
				case LITERAL_new:
				case LITERAL_delete:
				{
					expression();
					if (inputState->guessing==0) {
						e1_AST = returnAST;
					}
					break;
				}
				case SEMICOLON:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				RefPNode tmp286_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp286_AST = astFactory->create(LT(1));
				}
				match(SEMICOLON);
				if ( inputState->guessing==0 ) {
					end_of_stmt();
				}
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			{
			switch ( LA(1)) {
			case LITERAL_typedef:
			case LITERAL_enum:
			case ID:
			case LITERAL_inline:
			case LITERAL_extern:
			case OCTALINT:
			case LITERAL_friend:
			case StringLiteral:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LITERAL__declspec:
			case LITERAL___declspec:
			case LPAREN:
			case LITERAL_auto:
			case LITERAL_register:
			case LITERAL_static:
			case LITERAL_mutable:
			case LITERAL__inline:
			case LITERAL___inline:
			case LITERAL_virtual:
			case LITERAL_explicit:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case LITERAL_const:
			case LITERAL_volatile:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case PLUS:
			case MINUS:
			case PLUSPLUS:
			case MINUSMINUS:
			case LITERAL_sizeof:
			case LITERAL___alignof__:
			case SCOPE:
			case LITERAL_dynamic_cast:
			case LITERAL_static_cast:
			case LITERAL_reinterpret_cast:
			case LITERAL_const_cast:
			case LITERAL_this:
			case DECIMALINT:
			case HEXADECIMALINT:
			case CharLiteral:
			case WCharLiteral:
			case WStringLiteral:
			case FLOATONE:
			case FLOATTWO:
			case LITERAL_true:
			case LITERAL_false:
			case NOT:
			case LITERAL_new:
			case LITERAL_delete:
			{
				condition();
				if (inputState->guessing==0) {
					c_AST = returnAST;
				}
				break;
			}
			case SEMICOLON:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			RefPNode tmp287_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp287_AST = astFactory->create(LT(1));
			}
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			{
			switch ( LA(1)) {
			case LITERAL_enum:
			case ID:
			case OCTALINT:
			case StringLiteral:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LPAREN:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case PLUS:
			case MINUS:
			case PLUSPLUS:
			case MINUSMINUS:
			case LITERAL_sizeof:
			case LITERAL___alignof__:
			case SCOPE:
			case LITERAL_dynamic_cast:
			case LITERAL_static_cast:
			case LITERAL_reinterpret_cast:
			case LITERAL_const_cast:
			case LITERAL_this:
			case DECIMALINT:
			case HEXADECIMALINT:
			case CharLiteral:
			case WCharLiteral:
			case WStringLiteral:
			case FLOATONE:
			case FLOATTWO:
			case LITERAL_true:
			case LITERAL_false:
			case NOT:
			case LITERAL_new:
			case LITERAL_delete:
			{
				expression();
				if (inputState->guessing==0) {
					e2_AST = returnAST;
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			RefPNode tmp288_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp288_AST = astFactory->create(LT(1));
			}
			match(RPAREN);
			statement();
			if (inputState->guessing==0) {
				s_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				exitLocalScope();
			}
			if ( inputState->guessing==0 ) {
				iteration_statement_AST = RefPNode(currentAST.root);
				iteration_statement_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(5))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYSTATEMENT,"for")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"init")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(d_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(e1_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(c_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"iterator")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(e2_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"body")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))))))));
				currentAST.root = iteration_statement_AST;
				if ( iteration_statement_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					iteration_statement_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = iteration_statement_AST->getFirstChild();
				else
					currentAST.child = iteration_statement_AST;
				currentAST.advanceChildToEnd();
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = iteration_statement_AST;
}

void CPPParser::jump_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode jump_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case LITERAL_goto:
		{
			RefPNode tmp289_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp289_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp289_AST));
			}
			match(LITERAL_goto);
			RefPNode tmp290_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp290_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp290_AST));
			}
			match(ID);
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		case LITERAL_continue:
		{
			RefPNode tmp292_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp292_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp292_AST));
			}
			match(LITERAL_continue);
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		case LITERAL_break:
		{
			RefPNode tmp294_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp294_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp294_AST));
			}
			match(LITERAL_break);
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				end_of_stmt();
			}
			break;
		}
		case LITERAL_return:
		{
			RefPNode tmp296_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp296_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp296_AST));
			}
			match(LITERAL_return);
			if ( inputState->guessing==0 ) {
				in_return = true;
			}
			{
			bool synPredMatched461 = false;
			if (((LA(1) == LPAREN) && (LA(2) == ID))) {
				int _m461 = mark();
				synPredMatched461 = true;
				inputState->guessing++;
				try {
					{
					match(LPAREN);
					if (!((qualifiedItemIsOneOf(qiType) )))
						throw ANTLR_USE_NAMESPACE(antlr)SemanticException("(qualifiedItemIsOneOf(qiType) )");
					match(ID);
					match(RPAREN);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched461 = false;
				}
				rewind(_m461);
				inputState->guessing--;
			}
			if ( synPredMatched461 ) {
				match(LPAREN);
				RefPNode tmp298_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp298_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp298_AST));
				}
				match(ID);
				match(RPAREN);
				{
				switch ( LA(1)) {
				case LITERAL_enum:
				case ID:
				case OCTALINT:
				case StringLiteral:
				case LITERAL_struct:
				case LITERAL_union:
				case LITERAL_class:
				case LPAREN:
				case LITERAL_typename:
				case LITERAL_char:
				case LITERAL_wchar_t:
				case LITERAL_bool:
				case LITERAL_short:
				case LITERAL_int:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case LITERAL_long:
				case LITERAL_signed:
				case LITERAL_unsigned:
				case LITERAL_float:
				case LITERAL_double:
				case LITERAL_void:
				case OPERATOR:
				case TILDE:
				case LITERAL_typeid:
				case STAR:
				case AMPERSAND:
				case PLUS:
				case MINUS:
				case PLUSPLUS:
				case MINUSMINUS:
				case LITERAL_sizeof:
				case LITERAL___alignof__:
				case SCOPE:
				case LITERAL_dynamic_cast:
				case LITERAL_static_cast:
				case LITERAL_reinterpret_cast:
				case LITERAL_const_cast:
				case LITERAL_this:
				case DECIMALINT:
				case HEXADECIMALINT:
				case CharLiteral:
				case WCharLiteral:
				case WStringLiteral:
				case FLOATONE:
				case FLOATTWO:
				case LITERAL_true:
				case LITERAL_false:
				case NOT:
				case LITERAL_new:
				case LITERAL_delete:
				{
					expression();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case SEMICOLON:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_117.member(LA(2)))) {
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((LA(1) == SEMICOLON)) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			match(SEMICOLON);
			if ( inputState->guessing==0 ) {
				in_return = false,end_of_stmt();
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		jump_statement_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = jump_statement_AST;
}

void CPPParser::try_block() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode try_block_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp301_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp301_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp301_AST));
		}
		match(LITERAL_try);
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == LITERAL_catch)) {
				handler();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop470;
			}
			
		}
		_loop470:;
		} // ( ... )*
		try_block_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = try_block_AST;
}

void CPPParser::ep_try_block() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ep_try_block_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp302_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp302_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp302_AST));
		}
		match(LITERAL___try);
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case LITERAL___except:
		{
			ep_exception();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_template:
		case LITERAL_typedef:
		case LITERAL_enum:
		case SEMICOLON:
		case ID:
		case LCURLY:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_namespace:
		case LITERAL_antlrTrace_on:
		case LITERAL_antlrTrace_off:
		case RCURLY:
		case OCTALINT:
		case LITERAL_friend:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_public:
		case LITERAL_protected:
		case LITERAL_private:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case LITERAL_throw:
		case LITERAL_using:
		case LITERAL_case:
		case LITERAL_default:
		case LITERAL_if:
		case LITERAL_else:
		case LITERAL_switch:
		case LITERAL_while:
		case LITERAL_do:
		case LITERAL_for:
		case LITERAL_goto:
		case LITERAL_continue:
		case LITERAL_break:
		case LITERAL_return:
		case LITERAL___try:
		case LITERAL___finally:
		case LITERAL_try:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		case LITERAL_delete:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LITERAL___finally:
		{
			ep_finally();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_template:
		case LITERAL_typedef:
		case LITERAL_enum:
		case SEMICOLON:
		case ID:
		case LCURLY:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_namespace:
		case LITERAL_antlrTrace_on:
		case LITERAL_antlrTrace_off:
		case RCURLY:
		case OCTALINT:
		case LITERAL_friend:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LPAREN:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case LITERAL_public:
		case LITERAL_protected:
		case LITERAL_private:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case LITERAL_throw:
		case LITERAL_using:
		case LITERAL_case:
		case LITERAL_default:
		case LITERAL_if:
		case LITERAL_else:
		case LITERAL_switch:
		case LITERAL_while:
		case LITERAL_do:
		case LITERAL_for:
		case LITERAL_goto:
		case LITERAL_continue:
		case LITERAL_break:
		case LITERAL_return:
		case LITERAL___try:
		case LITERAL_try:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		case LITERAL_delete:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ep_try_block_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = ep_try_block_AST;
}

void CPPParser::throw_statement() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode throw_statement_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp303_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp303_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp303_AST));
		}
		match(LITERAL_throw);
		{
		switch ( LA(1)) {
		case LITERAL_enum:
		case ID:
		case OCTALINT:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL_delete:
		{
			assignment_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case SEMICOLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(SEMICOLON);
		if ( inputState->guessing==0 ) {
			end_of_stmt();
		}
		throw_statement_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = throw_statement_AST;
}

void CPPParser::condition() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode condition_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		bool synPredMatched456 = false;
		if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
			int _m456 = mark();
			synPredMatched456 = true;
			inputState->guessing++;
			try {
				{
				declaration_specifiers();
				declarator();
				match(ASSIGNEQUAL);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched456 = false;
			}
			rewind(_m456);
			inputState->guessing--;
		}
		if ( synPredMatched456 ) {
			declaration_specifiers();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp305_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp305_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp305_AST));
			}
			match(ASSIGNEQUAL);
			remainder_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_120.member(LA(2)))) {
			expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				condition_AST = RefPNode(currentAST.root);
				condition_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"condition")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(condition_AST))));
				currentAST.root = condition_AST;
				if ( condition_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					condition_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = condition_AST->getFirstChild();
				else
					currentAST.child = condition_AST;
				currentAST.advanceChildToEnd();
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		condition_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_121);
		} else {
			throw;
		}
	}
	returnAST = condition_AST;
}

void CPPParser::ep_exception() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ep_exception_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode e_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode s_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LITERAL___except);
		RefPNode tmp307_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp307_AST = astFactory->create(LT(1));
		}
		match(LPAREN);
		expression();
		if (inputState->guessing==0) {
			e_AST = returnAST;
		}
		RefPNode tmp308_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp308_AST = astFactory->create(LT(1));
		}
		match(RPAREN);
		compound_statement();
		if (inputState->guessing==0) {
			s_AST = returnAST;
		}
		if ( inputState->guessing==0 ) {
			ep_exception_AST = RefPNode(currentAST.root);
			ep_exception_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"__except")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"condition")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(e_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(s_AST))));
			currentAST.root = ep_exception_AST;
			if ( ep_exception_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				ep_exception_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = ep_exception_AST->getFirstChild();
			else
				currentAST.child = ep_exception_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_122);
		} else {
			throw;
		}
	}
	returnAST = ep_exception_AST;
}

void CPPParser::ep_finally() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ep_finally_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp309_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp309_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp309_AST));
		}
		match(LITERAL___finally);
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ep_finally_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_41);
		} else {
			throw;
		}
	}
	returnAST = ep_finally_AST;
}

void CPPParser::handler() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode handler_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp310_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp310_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp310_AST));
		}
		match(LITERAL_catch);
		if ( inputState->guessing==0 ) {
			exceptionBeginHandler();
		}
		if ( inputState->guessing==0 ) {
			declaratorParameterList(1);
		}
		match(LPAREN);
		exception_declaration();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			declaratorEndParameterList(1);
		}
		compound_statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			exceptionEndHandler();
		}
		handler_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_123);
		} else {
			throw;
		}
	}
	returnAST = handler_AST;
}

void CPPParser::exception_declaration() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode exception_declaration_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		parameter_declaration_list();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			exception_declaration_AST = RefPNode(currentAST.root);
			exception_declaration_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"exceptions")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(exception_declaration_AST))));
			currentAST.root = exception_declaration_AST;
			if ( exception_declaration_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				exception_declaration_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = exception_declaration_AST->getFirstChild();
			else
				currentAST.child = exception_declaration_AST;
			currentAST.advanceChildToEnd();
		}
		exception_declaration_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_73);
		} else {
			throw;
		}
	}
	returnAST = exception_declaration_AST;
}

void CPPParser::assignment_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode assignment_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		conditional_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case ASSIGNEQUAL:
		case TIMESEQUAL:
		case DIVIDEEQUAL:
		case MINUSEQUAL:
		case PLUSEQUAL:
		case MODEQUAL:
		case SHIFTLEFTEQUAL:
		case SHIFTRIGHTEQUAL:
		case BITWISEANDEQUAL:
		case BITWISEXOREQUAL:
		case BITWISEOREQUAL:
		{
			{
			switch ( LA(1)) {
			case ASSIGNEQUAL:
			{
				RefPNode tmp313_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp313_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp313_AST));
				}
				match(ASSIGNEQUAL);
				break;
			}
			case TIMESEQUAL:
			{
				RefPNode tmp314_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp314_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp314_AST));
				}
				match(TIMESEQUAL);
				break;
			}
			case DIVIDEEQUAL:
			{
				RefPNode tmp315_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp315_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp315_AST));
				}
				match(DIVIDEEQUAL);
				break;
			}
			case MINUSEQUAL:
			{
				RefPNode tmp316_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp316_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp316_AST));
				}
				match(MINUSEQUAL);
				break;
			}
			case PLUSEQUAL:
			{
				RefPNode tmp317_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp317_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp317_AST));
				}
				match(PLUSEQUAL);
				break;
			}
			case MODEQUAL:
			{
				RefPNode tmp318_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp318_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp318_AST));
				}
				match(MODEQUAL);
				break;
			}
			case SHIFTLEFTEQUAL:
			{
				RefPNode tmp319_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp319_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp319_AST));
				}
				match(SHIFTLEFTEQUAL);
				break;
			}
			case SHIFTRIGHTEQUAL:
			{
				RefPNode tmp320_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp320_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp320_AST));
				}
				match(SHIFTRIGHTEQUAL);
				break;
			}
			case BITWISEANDEQUAL:
			{
				RefPNode tmp321_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp321_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp321_AST));
				}
				match(BITWISEANDEQUAL);
				break;
			}
			case BITWISEXOREQUAL:
			{
				RefPNode tmp322_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp322_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp322_AST));
				}
				match(BITWISEXOREQUAL);
				break;
			}
			case BITWISEOREQUAL:
			{
				RefPNode tmp323_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp323_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp323_AST));
				}
				match(BITWISEOREQUAL);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			remainder_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case GREATERTHAN:
		case SEMICOLON:
		case RCURLY:
		case COLON:
		case RPAREN:
		case COMMA:
		case RSQUARE:
		case ELLIPSIS:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		assignment_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_76);
		} else {
			throw;
		}
	}
	returnAST = assignment_expression_AST;
}

void CPPParser::conditional_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode conditional_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode l_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode e_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode v_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
			bool bQuestion = false;
		
	
	try {      // for error handling
		logical_or_expression();
		if (inputState->guessing==0) {
			l_AST = returnAST;
		}
		{
		switch ( LA(1)) {
		case QUESTIONMARK:
		{
			RefPNode tmp324_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp324_AST = astFactory->create(LT(1));
			}
			match(QUESTIONMARK);
			expression();
			if (inputState->guessing==0) {
				e_AST = returnAST;
			}
			RefPNode tmp325_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp325_AST = astFactory->create(LT(1));
			}
			match(COLON);
			conditional_expression();
			if (inputState->guessing==0) {
				v_AST = returnAST;
			}
			if ( inputState->guessing==0 ) {
				bQuestion = true;
			}
			break;
		}
		case GREATERTHAN:
		case SEMICOLON:
		case RCURLY:
		case ASSIGNEQUAL:
		case COLON:
		case RPAREN:
		case COMMA:
		case RSQUARE:
		case ELLIPSIS:
		case TIMESEQUAL:
		case DIVIDEEQUAL:
		case MINUSEQUAL:
		case PLUSEQUAL:
		case MODEQUAL:
		case SHIFTLEFTEQUAL:
		case SHIFTRIGHTEQUAL:
		case BITWISEANDEQUAL:
		case BITWISEXOREQUAL:
		case BITWISEOREQUAL:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			conditional_expression_AST = RefPNode(currentAST.root);
			
						if(bQuestion==true)
							conditional_expression_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(4))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYSTATEMENT,"ifexp")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"condition")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(l_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"left")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(e_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"right")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(v_AST))))))));
						else
							conditional_expression_AST = l_AST;
					
			currentAST.root = conditional_expression_AST;
			if ( conditional_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				conditional_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = conditional_expression_AST->getFirstChild();
			else
				currentAST.child = conditional_expression_AST;
			currentAST.advanceChildToEnd();
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_124);
		} else {
			throw;
		}
	}
	returnAST = conditional_expression_AST;
}

void CPPParser::logical_or_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode logical_or_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		logical_and_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == OR)) {
				RefPNode tmp326_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp326_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp326_AST));
				}
				match(OR);
				logical_and_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop494;
			}
			
		}
		_loop494:;
		} // ( ... )*
		logical_or_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_125);
		} else {
			throw;
		}
	}
	returnAST = logical_or_expression_AST;
}

void CPPParser::logical_and_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode logical_and_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		inclusive_or_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == AND)) {
				RefPNode tmp327_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp327_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp327_AST));
				}
				match(AND);
				inclusive_or_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop497;
			}
			
		}
		_loop497:;
		} // ( ... )*
		logical_and_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_126);
		} else {
			throw;
		}
	}
	returnAST = logical_and_expression_AST;
}

void CPPParser::inclusive_or_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode inclusive_or_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		exclusive_or_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == BITWISEOR)) {
				RefPNode tmp328_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp328_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp328_AST));
				}
				match(BITWISEOR);
				exclusive_or_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop500;
			}
			
		}
		_loop500:;
		} // ( ... )*
		inclusive_or_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_127);
		} else {
			throw;
		}
	}
	returnAST = inclusive_or_expression_AST;
}

void CPPParser::exclusive_or_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode exclusive_or_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		and_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == BITWISEXOR)) {
				RefPNode tmp329_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp329_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp329_AST));
				}
				match(BITWISEXOR);
				and_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop503;
			}
			
		}
		_loop503:;
		} // ( ... )*
		exclusive_or_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_128);
		} else {
			throw;
		}
	}
	returnAST = exclusive_or_expression_AST;
}

void CPPParser::and_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode and_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		equality_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == AMPERSAND)) {
				RefPNode tmp330_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp330_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp330_AST));
				}
				match(AMPERSAND);
				equality_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop506;
			}
			
		}
		_loop506:;
		} // ( ... )*
		and_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_129);
		} else {
			throw;
		}
	}
	returnAST = and_expression_AST;
}

void CPPParser::equality_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode equality_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		relational_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == NOTEQUAL || LA(1) == EQUAL)) {
				{
				switch ( LA(1)) {
				case NOTEQUAL:
				{
					RefPNode tmp331_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp331_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp331_AST));
					}
					match(NOTEQUAL);
					break;
				}
				case EQUAL:
				{
					RefPNode tmp332_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp332_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp332_AST));
					}
					match(EQUAL);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				relational_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop510;
			}
			
		}
		_loop510:;
		} // ( ... )*
		equality_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_130);
		} else {
			throw;
		}
	}
	returnAST = equality_expression_AST;
}

void CPPParser::relational_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode relational_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		shift_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_131.member(LA(1))) && (_tokenSet_74.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case LESSTHAN:
				{
					RefPNode tmp333_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp333_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp333_AST));
					}
					match(LESSTHAN);
					break;
				}
				case GREATERTHAN:
				{
					RefPNode tmp334_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp334_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp334_AST));
					}
					match(GREATERTHAN);
					break;
				}
				case LESSTHANOREQUALTO:
				{
					RefPNode tmp335_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp335_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp335_AST));
					}
					match(LESSTHANOREQUALTO);
					break;
				}
				case GREATERTHANOREQUALTO:
				{
					RefPNode tmp336_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp336_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp336_AST));
					}
					match(GREATERTHANOREQUALTO);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				shift_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop514;
			}
			
		}
		_loop514:;
		} // ( ... )*
		relational_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_132);
		} else {
			throw;
		}
	}
	returnAST = relational_expression_AST;
}

void CPPParser::multiplicative_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode multiplicative_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		pm_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_133.member(LA(1)))) {
				{
				switch ( LA(1)) {
				case STAR:
				{
					RefPNode tmp337_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp337_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp337_AST));
					}
					match(STAR);
					break;
				}
				case DIVIDE:
				{
					RefPNode tmp338_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp338_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp338_AST));
					}
					match(DIVIDE);
					break;
				}
				case MOD:
				{
					RefPNode tmp339_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp339_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp339_AST));
					}
					match(MOD);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				pm_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop526;
			}
			
		}
		_loop526:;
		} // ( ... )*
		multiplicative_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_134);
		} else {
			throw;
		}
	}
	returnAST = multiplicative_expression_AST;
}

void CPPParser::pm_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode pm_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		cast_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == DOTMBR || LA(1) == POINTERTOMBR)) {
				{
				switch ( LA(1)) {
				case DOTMBR:
				{
					RefPNode tmp340_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp340_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp340_AST));
					}
					match(DOTMBR);
					break;
				}
				case POINTERTOMBR:
				{
					RefPNode tmp341_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp341_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp341_AST));
					}
					match(POINTERTOMBR);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				cast_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop530;
			}
			
		}
		_loop530:;
		} // ( ... )*
		pm_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_135);
		} else {
			throw;
		}
	}
	returnAST = pm_expression_AST;
}

void CPPParser::cast_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode cast_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		bool synPredMatched533 = false;
		if (((LA(1) == LPAREN) && (_tokenSet_4.member(LA(2))))) {
			int _m533 = mark();
			synPredMatched533 = true;
			inputState->guessing++;
			try {
				{
				match(LPAREN);
				type_id();
				match(RPAREN);
				unary_expression();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched533 = false;
			}
			rewind(_m533);
			inputState->guessing--;
		}
		if ( synPredMatched533 ) {
			match(LPAREN);
			type_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(RPAREN);
			unary_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			if ( inputState->guessing==0 ) {
				cast_expression_AST = RefPNode(currentAST.root);
				cast_expression_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"cast")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(cast_expression_AST))));
				currentAST.root = cast_expression_AST;
				if ( cast_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
					cast_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					  currentAST.child = cast_expression_AST->getFirstChild();
				else
					currentAST.child = cast_expression_AST;
				currentAST.advanceChildToEnd();
			}
			cast_expression_AST = RefPNode(currentAST.root);
		}
		else {
			bool synPredMatched535 = false;
			if (((LA(1) == LPAREN) && (_tokenSet_4.member(LA(2))))) {
				int _m535 = mark();
				synPredMatched535 = true;
				inputState->guessing++;
				try {
					{
					match(LPAREN);
					type_id();
					match(RPAREN);
					cast_expression();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched535 = false;
				}
				rewind(_m535);
				inputState->guessing--;
			}
			if ( synPredMatched535 ) {
				match(LPAREN);
				type_id();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RPAREN);
				cast_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				if ( inputState->guessing==0 ) {
					cast_expression_AST = RefPNode(currentAST.root);
					cast_expression_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"cast")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(cast_expression_AST))));
					currentAST.root = cast_expression_AST;
					if ( cast_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						cast_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = cast_expression_AST->getFirstChild();
					else
						currentAST.child = cast_expression_AST;
					currentAST.advanceChildToEnd();
				}
				cast_expression_AST = RefPNode(currentAST.root);
			}
			else if ((_tokenSet_74.member(LA(1))) && (_tokenSet_75.member(LA(2)))) {
				unary_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				cast_expression_AST = RefPNode(currentAST.root);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = cast_expression_AST;
}

void CPPParser::unary_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode unary_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode uo_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode ce_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case PLUSPLUS:
		{
			RefPNode tmp346_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp346_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp346_AST));
			}
			match(PLUSPLUS);
			unary_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case MINUSMINUS:
		{
			RefPNode tmp347_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp347_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp347_AST));
			}
			match(MINUSMINUS);
			unary_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		{
			{
			switch ( LA(1)) {
			case LITERAL_sizeof:
			{
				RefPNode tmp348_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp348_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp348_AST));
				}
				match(LITERAL_sizeof);
				break;
			}
			case LITERAL___alignof__:
			{
				RefPNode tmp349_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp349_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp349_AST));
				}
				match(LITERAL___alignof__);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			bool synPredMatched543 = false;
			if (((_tokenSet_74.member(LA(1))) && (_tokenSet_75.member(LA(2))))) {
				int _m543 = mark();
				synPredMatched543 = true;
				inputState->guessing++;
				try {
					{
					unary_expression();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched543 = false;
				}
				rewind(_m543);
				inputState->guessing--;
			}
			if ( synPredMatched543 ) {
				unary_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((LA(1) == LPAREN) && (_tokenSet_4.member(LA(2)))) {
				match(LPAREN);
				type_id();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RPAREN);
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		default:
			bool synPredMatched539 = false;
			if (((_tokenSet_137.member(LA(1))) && (_tokenSet_138.member(LA(2))))) {
				int _m539 = mark();
				synPredMatched539 = true;
				inputState->guessing++;
				try {
					{
					postfix_expression();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched539 = false;
				}
				rewind(_m539);
				inputState->guessing--;
			}
			if ( synPredMatched539 ) {
				postfix_expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((_tokenSet_139.member(LA(1))) && (_tokenSet_74.member(LA(2)))) {
				unary_operator();
				if (inputState->guessing==0) {
					uo_AST = returnAST;
				}
				cast_expression();
				if (inputState->guessing==0) {
					ce_AST = returnAST;
				}
				if ( inputState->guessing==0 ) {
					unary_expression_AST = RefPNode(currentAST.root);
					unary_expression_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(uo_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ce_AST))));
					currentAST.root = unary_expression_AST;
					if ( unary_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						unary_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = unary_expression_AST->getFirstChild();
					else
						currentAST.child = unary_expression_AST;
					currentAST.advanceChildToEnd();
				}
			}
			else if ((_tokenSet_140.member(LA(1))) && (_tokenSet_141.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case SCOPE:
				{
					RefPNode tmp352_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp352_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp352_AST));
					}
					match(SCOPE);
					break;
				}
				case LITERAL_new:
				case LITERAL_delete:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				{
				switch ( LA(1)) {
				case LITERAL_new:
				{
					new_expression();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				case LITERAL_delete:
				{
					delete_expression();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		unary_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = unary_expression_AST;
}

void CPPParser::postfix_expression(
	RefPNode recur
) {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode postfix_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode tss_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode tel_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode tdot_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode ps_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode p_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode array_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode el_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode ob_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	RefPNode t_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefPNode id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
		 TypeSpecifier ts;
		 bool bDot;
		
	
	try {      // for error handling
		{
		bool synPredMatched549 = false;
		if ((((_tokenSet_57.member(LA(1))) && (_tokenSet_142.member(LA(2))))&&(!(LA(1)==LPAREN)))) {
			int _m549 = mark();
			synPredMatched549 = true;
			inputState->guessing++;
			try {
				{
				ts=simple_type_specifier();
				match(LPAREN);
				match(RPAREN);
				match(LPAREN);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched549 = false;
			}
			rewind(_m549);
			inputState->guessing--;
		}
		if ( synPredMatched549 ) {
			ts=simple_type_specifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp353_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp353_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp353_AST));
			}
			match(LPAREN);
			RefPNode tmp354_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp354_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp354_AST));
			}
			match(RPAREN);
			RefPNode tmp355_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp355_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp355_AST));
			}
			match(LPAREN);
			{
			switch ( LA(1)) {
			case LITERAL_enum:
			case ID:
			case OCTALINT:
			case StringLiteral:
			case LITERAL_struct:
			case LITERAL_union:
			case LITERAL_class:
			case LPAREN:
			case LITERAL_typename:
			case LITERAL_char:
			case LITERAL_wchar_t:
			case LITERAL_bool:
			case LITERAL_short:
			case LITERAL_int:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case LITERAL_long:
			case LITERAL_signed:
			case LITERAL_unsigned:
			case LITERAL_float:
			case LITERAL_double:
			case LITERAL_void:
			case OPERATOR:
			case TILDE:
			case LITERAL_typeid:
			case STAR:
			case AMPERSAND:
			case PLUS:
			case MINUS:
			case PLUSPLUS:
			case MINUSMINUS:
			case LITERAL_sizeof:
			case LITERAL___alignof__:
			case SCOPE:
			case LITERAL_dynamic_cast:
			case LITERAL_static_cast:
			case LITERAL_reinterpret_cast:
			case LITERAL_const_cast:
			case LITERAL_this:
			case DECIMALINT:
			case HEXADECIMALINT:
			case CharLiteral:
			case WCharLiteral:
			case WStringLiteral:
			case FLOATONE:
			case FLOATTWO:
			case LITERAL_true:
			case LITERAL_false:
			case NOT:
			case LITERAL_new:
			case LITERAL_delete:
			{
				expression_list();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			RefPNode tmp356_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp356_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp356_AST));
			}
			match(RPAREN);
		}
		else {
			bool synPredMatched552 = false;
			if ((((_tokenSet_57.member(LA(1))) && (_tokenSet_142.member(LA(2))))&&(!(LA(1)==LPAREN)))) {
				int _m552 = mark();
				synPredMatched552 = true;
				inputState->guessing++;
				try {
					{
					ts=simple_type_specifier();
					match(LPAREN);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched552 = false;
				}
				rewind(_m552);
				inputState->guessing--;
			}
			if ( synPredMatched552 ) {
				ts=simple_type_specifier();
				if (inputState->guessing==0) {
					tss_AST = returnAST;
				}
				RefPNode tmp357_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp357_AST = astFactory->create(LT(1));
				}
				match(LPAREN);
				{
				switch ( LA(1)) {
				case LITERAL_enum:
				case ID:
				case OCTALINT:
				case StringLiteral:
				case LITERAL_struct:
				case LITERAL_union:
				case LITERAL_class:
				case LPAREN:
				case LITERAL_typename:
				case LITERAL_char:
				case LITERAL_wchar_t:
				case LITERAL_bool:
				case LITERAL_short:
				case LITERAL_int:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case LITERAL_long:
				case LITERAL_signed:
				case LITERAL_unsigned:
				case LITERAL_float:
				case LITERAL_double:
				case LITERAL_void:
				case OPERATOR:
				case TILDE:
				case LITERAL_typeid:
				case STAR:
				case AMPERSAND:
				case PLUS:
				case MINUS:
				case PLUSPLUS:
				case MINUSMINUS:
				case LITERAL_sizeof:
				case LITERAL___alignof__:
				case SCOPE:
				case LITERAL_dynamic_cast:
				case LITERAL_static_cast:
				case LITERAL_reinterpret_cast:
				case LITERAL_const_cast:
				case LITERAL_this:
				case DECIMALINT:
				case HEXADECIMALINT:
				case CharLiteral:
				case WCharLiteral:
				case WStringLiteral:
				case FLOATONE:
				case FLOATTWO:
				case LITERAL_true:
				case LITERAL_false:
				case NOT:
				case LITERAL_new:
				case LITERAL_delete:
				{
					expression_list();
					if (inputState->guessing==0) {
						tel_AST = returnAST;
					}
					break;
				}
				case RPAREN:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				RefPNode tmp358_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp358_AST = astFactory->create(LT(1));
				}
				match(RPAREN);
				if ( inputState->guessing==0 ) {
					
							 tss_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tdot_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYCALL,"call")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"convertor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tss_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"parameters")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tel_AST))))))))))));
							 ps_AST = tss_AST;
							
				}
				{
				switch ( LA(1)) {
				case DOT:
				case POINTERTO:
				{
					bDot=dot_expression();
					if (inputState->guessing==0) {
						tdot_AST = returnAST;
					}
					if ( inputState->guessing==0 ) {
						
									if(bDot==true)
										tss_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tdot_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYCALL,"call")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"ctor")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tss_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"parameters")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(tel_AST))))))))))));
									
					}
					postfix_expression(tss_AST);
					if (inputState->guessing==0) {
						ps_AST = returnAST;
					}
					break;
				}
				case LESSTHAN:
				case GREATERTHAN:
				case SEMICOLON:
				case RCURLY:
				case ASSIGNEQUAL:
				case COLON:
				case RPAREN:
				case COMMA:
				case RSQUARE:
				case STAR:
				case AMPERSAND:
				case ELLIPSIS:
				case TIMESEQUAL:
				case DIVIDEEQUAL:
				case MINUSEQUAL:
				case PLUSEQUAL:
				case MODEQUAL:
				case SHIFTLEFTEQUAL:
				case SHIFTRIGHTEQUAL:
				case BITWISEANDEQUAL:
				case BITWISEXOREQUAL:
				case BITWISEOREQUAL:
				case QUESTIONMARK:
				case OR:
				case AND:
				case BITWISEOR:
				case BITWISEXOR:
				case NOTEQUAL:
				case EQUAL:
				case LESSTHANOREQUALTO:
				case GREATERTHANOREQUALTO:
				case SHIFTLEFT:
				case SHIFTRIGHT:
				case PLUS:
				case MINUS:
				case DIVIDE:
				case MOD:
				case DOTMBR:
				case POINTERTOMBR:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				if ( inputState->guessing==0 ) {
					postfix_expression_AST = RefPNode(currentAST.root);
					postfix_expression_AST=ps_AST;
					currentAST.root = postfix_expression_AST;
					if ( postfix_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						postfix_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = postfix_expression_AST->getFirstChild();
					else
						currentAST.child = postfix_expression_AST;
					currentAST.advanceChildToEnd();
				}
			}
			else if ((_tokenSet_143.member(LA(1))) && (_tokenSet_138.member(LA(2)))) {
				primary_expression();
				if (inputState->guessing==0) {
					p_AST = returnAST;
				}
				if ( inputState->guessing==0 ) {
					if(recur != nullAST) p_AST->addChild(recur);
				}
				{ // ( ... )*
				for (;;) {
					switch ( LA(1)) {
					case LSQUARE:
					{
						{
						RefPNode tmp359_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp359_AST = astFactory->create(LT(1));
						}
						match(LSQUARE);
						expression();
						RefPNode tmp360_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp360_AST = astFactory->create(LT(1));
						}
						match(RSQUARE);
						}
						if ( inputState->guessing==0 ) {
							
								 if(array_AST == nullAST)
									array_AST = astFactory->create(MY,"array");
								 array_AST->addChild(returnAST);
								
						}
						break;
					}
					case LPAREN:
					{
						RefPNode tmp361_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp361_AST = astFactory->create(LT(1));
						}
						match(LPAREN);
						{
						switch ( LA(1)) {
						case LITERAL_enum:
						case ID:
						case OCTALINT:
						case StringLiteral:
						case LITERAL_struct:
						case LITERAL_union:
						case LITERAL_class:
						case LPAREN:
						case LITERAL_typename:
						case LITERAL_char:
						case LITERAL_wchar_t:
						case LITERAL_bool:
						case LITERAL_short:
						case LITERAL_int:
						case 44:
						case 45:
						case 46:
						case 47:
						case 48:
						case 49:
						case 50:
						case 51:
						case 52:
						case 53:
						case LITERAL_long:
						case LITERAL_signed:
						case LITERAL_unsigned:
						case LITERAL_float:
						case LITERAL_double:
						case LITERAL_void:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case STAR:
						case AMPERSAND:
						case PLUS:
						case MINUS:
						case PLUSPLUS:
						case MINUSMINUS:
						case LITERAL_sizeof:
						case LITERAL___alignof__:
						case SCOPE:
						case LITERAL_dynamic_cast:
						case LITERAL_static_cast:
						case LITERAL_reinterpret_cast:
						case LITERAL_const_cast:
						case LITERAL_this:
						case DECIMALINT:
						case HEXADECIMALINT:
						case CharLiteral:
						case WCharLiteral:
						case WStringLiteral:
						case FLOATONE:
						case FLOATTWO:
						case LITERAL_true:
						case LITERAL_false:
						case NOT:
						case LITERAL_new:
						case LITERAL_delete:
						{
							expression_list();
							if (inputState->guessing==0) {
								el_AST = returnAST;
							}
							break;
						}
						case RPAREN:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						RefPNode tmp362_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp362_AST = astFactory->create(LT(1));
						}
						match(RPAREN);
						if ( inputState->guessing==0 ) {
							p_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MYCALL,"call")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"function")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(p_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"parameters")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(el_AST))))))));
						}
						break;
					}
					case DOT:
					case POINTERTO:
					{
						bDot=dot_expression();
						if (inputState->guessing==0) {
							ob_AST = returnAST;
						}
						{
						switch ( LA(1)) {
						case LITERAL_template:
						{
							t = LT(1);
							t_AST = astFactory->create(t);
							match(LITERAL_template);
							break;
						}
						case ID:
						case OPERATOR:
						case TILDE:
						case LITERAL_typeid:
						case SCOPE:
						{
							break;
						}
						default:
						{
							throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
						}
						}
						}
						id_expression();
						if (inputState->guessing==0) {
							id_AST = returnAST;
						}
						if ( inputState->guessing==0 ) {
							p_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(3))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(id_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(ob_AST))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(p_AST))))))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(t_AST))));	
						}
						break;
					}
					case PLUSPLUS:
					{
						RefPNode tmp363_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp363_AST = astFactory->create(LT(1));
						}
						match(PLUSPLUS);
						if ( inputState->guessing==0 ) {
							p_AST->addChild(RefPNode(astFactory->create(MY,"++")));
						}
						break;
					}
					case MINUSMINUS:
					{
						RefPNode tmp364_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
						if ( inputState->guessing == 0 ) {
							tmp364_AST = astFactory->create(LT(1));
						}
						match(MINUSMINUS);
						if ( inputState->guessing==0 ) {
							p_AST->addChild(RefPNode(astFactory->create(MY,"--")));
						}
						break;
					}
					default:
					{
						goto _loop559;
					}
					}
				}
				_loop559:;
				} // ( ... )*
				if ( inputState->guessing==0 ) {
					postfix_expression_AST = RefPNode(currentAST.root);
					
							 if(array_AST != nullAST)
								p_AST->addChild(array_AST);
							 postfix_expression_AST = p_AST;
							
					currentAST.root = postfix_expression_AST;
					if ( postfix_expression_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
						postfix_expression_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						  currentAST.child = postfix_expression_AST->getFirstChild();
					else
						currentAST.child = postfix_expression_AST;
					currentAST.advanceChildToEnd();
				}
			}
			else if (((LA(1) >= LITERAL_dynamic_cast && LA(1) <= LITERAL_const_cast))) {
				{
				switch ( LA(1)) {
				case LITERAL_dynamic_cast:
				{
					RefPNode tmp365_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp365_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp365_AST));
					}
					match(LITERAL_dynamic_cast);
					break;
				}
				case LITERAL_static_cast:
				{
					RefPNode tmp366_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp366_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp366_AST));
					}
					match(LITERAL_static_cast);
					break;
				}
				case LITERAL_reinterpret_cast:
				{
					RefPNode tmp367_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp367_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp367_AST));
					}
					match(LITERAL_reinterpret_cast);
					break;
				}
				case LITERAL_const_cast:
				{
					RefPNode tmp368_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp368_AST = astFactory->create(LT(1));
						astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp368_AST));
					}
					match(LITERAL_const_cast);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				ts=cast_type();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(LPAREN);
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RPAREN);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		postfix_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = postfix_expression_AST;
}

void CPPParser::unary_operator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode unary_operator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case AMPERSAND:
		{
			RefPNode tmp371_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp371_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp371_AST));
			}
			match(AMPERSAND);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		case STAR:
		{
			RefPNode tmp372_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp372_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp372_AST));
			}
			match(STAR);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		case PLUS:
		{
			RefPNode tmp373_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp373_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp373_AST));
			}
			match(PLUS);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		case MINUS:
		{
			RefPNode tmp374_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp374_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp374_AST));
			}
			match(MINUS);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		case TILDE:
		{
			RefPNode tmp375_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp375_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp375_AST));
			}
			match(TILDE);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		case NOT:
		{
			RefPNode tmp376_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp376_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp376_AST));
			}
			match(NOT);
			unary_operator_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_74);
		} else {
			throw;
		}
	}
	returnAST = unary_operator_AST;
}

void CPPParser::new_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode new_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		RefPNode tmp377_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp377_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp377_AST));
		}
		match(LITERAL_new);
		{
		bool synPredMatched577 = false;
		if (((LA(1) == LPAREN) && (_tokenSet_74.member(LA(2))))) {
			int _m577 = mark();
			synPredMatched577 = true;
			inputState->guessing++;
			try {
				{
				match(LPAREN);
				expression_list();
				match(RPAREN);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched577 = false;
			}
			rewind(_m577);
			inputState->guessing--;
		}
		if ( synPredMatched577 ) {
			RefPNode tmp378_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp378_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp378_AST));
			}
			match(LPAREN);
			expression_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp379_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp379_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp379_AST));
			}
			match(RPAREN);
		}
		else if ((_tokenSet_144.member(LA(1))) && (_tokenSet_145.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		{
		switch ( LA(1)) {
		case LITERAL_typedef:
		case LITERAL_enum:
		case ID:
		case LITERAL_inline:
		case LITERAL_extern:
		case LITERAL_friend:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL__declspec:
		case LITERAL___declspec:
		case LITERAL_auto:
		case LITERAL_register:
		case LITERAL_static:
		case LITERAL_mutable:
		case LITERAL__inline:
		case LITERAL___inline:
		case LITERAL_virtual:
		case LITERAL_explicit:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case LITERAL_const:
		case LITERAL_volatile:
		case SCOPE:
		{
			new_type_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LPAREN:
		{
			RefPNode tmp380_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp380_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp380_AST));
			}
			match(LPAREN);
			type_id();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			RefPNode tmp381_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp381_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp381_AST));
			}
			match(RPAREN);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			new_initializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LESSTHAN:
		case GREATERTHAN:
		case SEMICOLON:
		case RCURLY:
		case ASSIGNEQUAL:
		case COLON:
		case RPAREN:
		case COMMA:
		case RSQUARE:
		case STAR:
		case AMPERSAND:
		case ELLIPSIS:
		case TIMESEQUAL:
		case DIVIDEEQUAL:
		case MINUSEQUAL:
		case PLUSEQUAL:
		case MODEQUAL:
		case SHIFTLEFTEQUAL:
		case SHIFTRIGHTEQUAL:
		case BITWISEANDEQUAL:
		case BITWISEXOREQUAL:
		case BITWISEOREQUAL:
		case QUESTIONMARK:
		case OR:
		case AND:
		case BITWISEOR:
		case BITWISEXOR:
		case NOTEQUAL:
		case EQUAL:
		case LESSTHANOREQUALTO:
		case GREATERTHANOREQUALTO:
		case SHIFTLEFT:
		case SHIFTRIGHT:
		case PLUS:
		case MINUS:
		case DIVIDE:
		case MOD:
		case DOTMBR:
		case POINTERTOMBR:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		}
		new_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = new_expression_AST;
}

void CPPParser::delete_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode delete_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefPNode tmp382_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp382_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp382_AST));
		}
		match(LITERAL_delete);
		{
		switch ( LA(1)) {
		case LSQUARE:
		{
			match(LSQUARE);
			match(RSQUARE);
			if ( inputState->guessing==0 ) {
				astFactory->addASTChild(currentAST, astFactory->create(MY,"array"));
			}
			break;
		}
		case LITERAL_enum:
		case ID:
		case OCTALINT:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL_delete:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		cast_expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		delete_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = delete_expression_AST;
}

bool  CPPParser::dot_expression() {
	bool bDot = false;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode dot_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case DOT:
		{
			RefPNode tmp385_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp385_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp385_AST));
			}
			match(DOT);
			if ( inputState->guessing==0 ) {
				bDot=true;
			}
			break;
		}
		case POINTERTO:
		{
			RefPNode tmp386_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp386_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp386_AST));
			}
			match(POINTERTO);
			if ( inputState->guessing==0 ) {
				bDot=false;
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		dot_expression_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_146);
		} else {
			throw;
		}
	}
	returnAST = dot_expression_AST;
	return bDot;
}

void CPPParser::primary_expression() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode primary_expression_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case ID:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case SCOPE:
		{
			id_expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			primary_expression_AST = RefPNode(currentAST.root);
			break;
		}
		case OCTALINT:
		case StringLiteral:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		{
			literal();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			primary_expression_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_this:
		{
			RefPNode tmp387_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp387_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp387_AST));
			}
			match(LITERAL_this);
			primary_expression_AST = RefPNode(currentAST.root);
			break;
		}
		case LPAREN:
		{
			match(LPAREN);
			expression();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(RPAREN);
			primary_expression_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_147);
		} else {
			throw;
		}
	}
	returnAST = primary_expression_AST;
}

CPPParser::TypeSpecifier  CPPParser::cast_type() {
	CPPParser::TypeSpecifier ts;
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode cast_type_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LESSTHAN);
		{
		switch ( LA(1)) {
		case LITERAL_const:
		{
			RefPNode tmp391_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp391_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp391_AST));
			}
			match(LITERAL_const);
			break;
		}
		case LITERAL_enum:
		case ID:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case SCOPE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ts=type_specifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case ID:
		case STAR:
		case AMPERSAND:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			ptr_operator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case GREATERTHAN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(GREATERTHAN);
		if ( inputState->guessing==0 ) {
			cast_type_AST = RefPNode(currentAST.root);
			cast_type_AST=RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"casttype")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(cast_type_AST))));
			currentAST.root = cast_type_AST;
			if ( cast_type_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				cast_type_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = cast_type_AST->getFirstChild();
			else
				currentAST.child = cast_type_AST;
			currentAST.advanceChildToEnd();
		}
		cast_type_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_85);
		} else {
			throw;
		}
	}
	returnAST = cast_type_AST;
	return ts;
}

void CPPParser::literal() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode literal_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case OCTALINT:
		{
			RefPNode tmp393_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp393_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp393_AST));
			}
			match(OCTALINT);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case DECIMALINT:
		{
			RefPNode tmp394_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp394_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp394_AST));
			}
			match(DECIMALINT);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case HEXADECIMALINT:
		{
			RefPNode tmp395_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp395_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp395_AST));
			}
			match(HEXADECIMALINT);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case CharLiteral:
		{
			RefPNode tmp396_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp396_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp396_AST));
			}
			match(CharLiteral);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case WCharLiteral:
		{
			RefPNode tmp397_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp397_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp397_AST));
			}
			match(WCharLiteral);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case StringLiteral:
		case WStringLiteral:
		{
			{ // ( ... )+
			int _cnt571=0;
			for (;;) {
				switch ( LA(1)) {
				case StringLiteral:
				{
					RefPNode tmp398_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp398_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp398_AST));
					}
					match(StringLiteral);
					break;
				}
				case WStringLiteral:
				{
					RefPNode tmp399_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp399_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp399_AST));
					}
					match(WStringLiteral);
					break;
				}
				default:
				{
					if ( _cnt571>=1 ) { goto _loop571; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				}
				_cnt571++;
			}
			_loop571:;
			}  // ( ... )+
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case FLOATONE:
		{
			RefPNode tmp400_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp400_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp400_AST));
			}
			match(FLOATONE);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case FLOATTWO:
		{
			RefPNode tmp401_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp401_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp401_AST));
			}
			match(FLOATTWO);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_true:
		{
			RefPNode tmp402_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp402_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp402_AST));
			}
			match(LITERAL_true);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		case LITERAL_false:
		{
			RefPNode tmp403_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp403_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp403_AST));
			}
			match(LITERAL_false);
			literal_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_147);
		} else {
			throw;
		}
	}
	returnAST = literal_AST;
}

void CPPParser::new_type_id() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode new_type_id_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		declaration_specifiers();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		if ((_tokenSet_148.member(LA(1))) && (_tokenSet_149.member(LA(2)))) {
			new_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else if ((_tokenSet_150.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		new_type_id_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_150);
		} else {
			throw;
		}
	}
	returnAST = new_type_id_AST;
}

void CPPParser::new_initializer() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode new_initializer_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		match(LPAREN);
		{
		switch ( LA(1)) {
		case LITERAL_enum:
		case ID:
		case OCTALINT:
		case StringLiteral:
		case LITERAL_struct:
		case LITERAL_union:
		case LITERAL_class:
		case LPAREN:
		case LITERAL_typename:
		case LITERAL_char:
		case LITERAL_wchar_t:
		case LITERAL_bool:
		case LITERAL_short:
		case LITERAL_int:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case LITERAL_long:
		case LITERAL_signed:
		case LITERAL_unsigned:
		case LITERAL_float:
		case LITERAL_double:
		case LITERAL_void:
		case OPERATOR:
		case TILDE:
		case LITERAL_typeid:
		case STAR:
		case AMPERSAND:
		case PLUS:
		case MINUS:
		case PLUSPLUS:
		case MINUSMINUS:
		case LITERAL_sizeof:
		case LITERAL___alignof__:
		case SCOPE:
		case LITERAL_dynamic_cast:
		case LITERAL_static_cast:
		case LITERAL_reinterpret_cast:
		case LITERAL_const_cast:
		case LITERAL_this:
		case DECIMALINT:
		case HEXADECIMALINT:
		case CharLiteral:
		case WCharLiteral:
		case WStringLiteral:
		case FLOATONE:
		case FLOATTWO:
		case LITERAL_true:
		case LITERAL_false:
		case NOT:
		case LITERAL_new:
		case LITERAL_delete:
		{
			expression_list();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(RPAREN);
		if ( inputState->guessing==0 ) {
			new_initializer_AST = RefPNode(currentAST.root);
			new_initializer_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"parameters")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(new_initializer_AST))));
			currentAST.root = new_initializer_AST;
			if ( new_initializer_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				new_initializer_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = new_initializer_AST->getFirstChild();
			else
				currentAST.child = new_initializer_AST;
			currentAST.advanceChildToEnd();
		}
		new_initializer_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_136);
		} else {
			throw;
		}
	}
	returnAST = new_initializer_AST;
}

void CPPParser::new_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode new_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case ID:
		case STAR:
		case AMPERSAND:
		case SCOPE:
		case LITERAL__cdecl:
		case LITERAL___cdecl:
		case LITERAL__near:
		case LITERAL___near:
		case LITERAL__far:
		case LITERAL___far:
		case LITERAL___interrupt:
		case LITERAL_pascal:
		case LITERAL__pascal:
		case LITERAL___pascal:
		case LITERAL__stdcall:
		case LITERAL___stdcall:
		{
			ptr_operator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			{
			if ((_tokenSet_148.member(LA(1))) && (_tokenSet_149.member(LA(2)))) {
				new_declarator();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else if ((_tokenSet_150.member(LA(1))) && (_tokenSet_44.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			new_declarator_AST = RefPNode(currentAST.root);
			break;
		}
		case LSQUARE:
		{
			direct_new_declarator();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			new_declarator_AST = RefPNode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_150);
		} else {
			throw;
		}
	}
	returnAST = new_declarator_AST;
}

void CPPParser::direct_new_declarator() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode direct_new_declarator_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{ // ( ... )+
		int _cnt590=0;
		for (;;) {
			if ((LA(1) == LSQUARE)) {
				match(LSQUARE);
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				match(RSQUARE);
			}
			else {
				if ( _cnt590>=1 ) { goto _loop590; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt590++;
		}
		_loop590:;
		}  // ( ... )+
		if ( inputState->guessing==0 ) {
			direct_new_declarator_AST = RefPNode(currentAST.root);
			direct_new_declarator_AST = RefPNode(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(MY,"array")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(direct_new_declarator_AST))));
			currentAST.root = direct_new_declarator_AST;
			if ( direct_new_declarator_AST!=RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				direct_new_declarator_AST->getFirstChild() != RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = direct_new_declarator_AST->getFirstChild();
			else
				currentAST.child = direct_new_declarator_AST;
			currentAST.advanceChildToEnd();
		}
		direct_new_declarator_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_150);
		} else {
			throw;
		}
	}
	returnAST = direct_new_declarator_AST;
}

void CPPParser::ptr_to_member() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode ptr_to_member_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	data s;
	
	try {      // for error handling
		s=scope_override();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		RefPNode tmp408_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp408_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp408_AST));
		}
		match(STAR);
		if ( inputState->guessing==0 ) {
			is_pointer = true;
		}
		cv_qualifier_seq();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		ptr_to_member_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_1);
		} else {
			throw;
		}
	}
	returnAST = ptr_to_member_AST;
}

void CPPParser::optor_simple_tokclass() {
	returnAST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefPNode optor_simple_tokclass_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case PLUS:
		{
			RefPNode tmp409_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp409_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp409_AST));
			}
			match(PLUS);
			break;
		}
		case MINUS:
		{
			RefPNode tmp410_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp410_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp410_AST));
			}
			match(MINUS);
			break;
		}
		case STAR:
		{
			RefPNode tmp411_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp411_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp411_AST));
			}
			match(STAR);
			break;
		}
		case DIVIDE:
		{
			RefPNode tmp412_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp412_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp412_AST));
			}
			match(DIVIDE);
			break;
		}
		case MOD:
		{
			RefPNode tmp413_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp413_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp413_AST));
			}
			match(MOD);
			break;
		}
		case BITWISEXOR:
		{
			RefPNode tmp414_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp414_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp414_AST));
			}
			match(BITWISEXOR);
			break;
		}
		case AMPERSAND:
		{
			RefPNode tmp415_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp415_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp415_AST));
			}
			match(AMPERSAND);
			break;
		}
		case BITWISEOR:
		{
			RefPNode tmp416_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp416_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp416_AST));
			}
			match(BITWISEOR);
			break;
		}
		case TILDE:
		{
			RefPNode tmp417_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp417_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp417_AST));
			}
			match(TILDE);
			break;
		}
		case NOT:
		{
			RefPNode tmp418_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp418_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp418_AST));
			}
			match(NOT);
			break;
		}
		case SHIFTLEFT:
		{
			RefPNode tmp419_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp419_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp419_AST));
			}
			match(SHIFTLEFT);
			break;
		}
		case SHIFTRIGHT:
		{
			RefPNode tmp420_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp420_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp420_AST));
			}
			match(SHIFTRIGHT);
			break;
		}
		case ASSIGNEQUAL:
		{
			RefPNode tmp421_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp421_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp421_AST));
			}
			match(ASSIGNEQUAL);
			break;
		}
		case TIMESEQUAL:
		{
			RefPNode tmp422_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp422_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp422_AST));
			}
			match(TIMESEQUAL);
			break;
		}
		case DIVIDEEQUAL:
		{
			RefPNode tmp423_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp423_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp423_AST));
			}
			match(DIVIDEEQUAL);
			break;
		}
		case MODEQUAL:
		{
			RefPNode tmp424_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp424_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp424_AST));
			}
			match(MODEQUAL);
			break;
		}
		case PLUSEQUAL:
		{
			RefPNode tmp425_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp425_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp425_AST));
			}
			match(PLUSEQUAL);
			break;
		}
		case MINUSEQUAL:
		{
			RefPNode tmp426_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp426_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp426_AST));
			}
			match(MINUSEQUAL);
			break;
		}
		case SHIFTLEFTEQUAL:
		{
			RefPNode tmp427_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp427_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp427_AST));
			}
			match(SHIFTLEFTEQUAL);
			break;
		}
		case SHIFTRIGHTEQUAL:
		{
			RefPNode tmp428_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp428_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp428_AST));
			}
			match(SHIFTRIGHTEQUAL);
			break;
		}
		case BITWISEANDEQUAL:
		{
			RefPNode tmp429_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp429_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp429_AST));
			}
			match(BITWISEANDEQUAL);
			break;
		}
		case BITWISEXOREQUAL:
		{
			RefPNode tmp430_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp430_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp430_AST));
			}
			match(BITWISEXOREQUAL);
			break;
		}
		case BITWISEOREQUAL:
		{
			RefPNode tmp431_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp431_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp431_AST));
			}
			match(BITWISEOREQUAL);
			break;
		}
		case EQUAL:
		{
			RefPNode tmp432_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp432_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp432_AST));
			}
			match(EQUAL);
			break;
		}
		case NOTEQUAL:
		{
			RefPNode tmp433_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp433_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp433_AST));
			}
			match(NOTEQUAL);
			break;
		}
		case LESSTHAN:
		{
			RefPNode tmp434_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp434_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp434_AST));
			}
			match(LESSTHAN);
			break;
		}
		case GREATERTHAN:
		{
			RefPNode tmp435_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp435_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp435_AST));
			}
			match(GREATERTHAN);
			break;
		}
		case LESSTHANOREQUALTO:
		{
			RefPNode tmp436_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp436_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp436_AST));
			}
			match(LESSTHANOREQUALTO);
			break;
		}
		case GREATERTHANOREQUALTO:
		{
			RefPNode tmp437_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp437_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp437_AST));
			}
			match(GREATERTHANOREQUALTO);
			break;
		}
		case OR:
		{
			RefPNode tmp438_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp438_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp438_AST));
			}
			match(OR);
			break;
		}
		case AND:
		{
			RefPNode tmp439_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp439_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp439_AST));
			}
			match(AND);
			break;
		}
		case PLUSPLUS:
		{
			RefPNode tmp440_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp440_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp440_AST));
			}
			match(PLUSPLUS);
			break;
		}
		case MINUSMINUS:
		{
			RefPNode tmp441_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp441_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp441_AST));
			}
			match(MINUSMINUS);
			break;
		}
		case COMMA:
		{
			RefPNode tmp442_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp442_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp442_AST));
			}
			match(COMMA);
			break;
		}
		case POINTERTO:
		{
			RefPNode tmp443_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp443_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp443_AST));
			}
			match(POINTERTO);
			break;
		}
		case POINTERTOMBR:
		{
			RefPNode tmp444_AST = RefPNode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp444_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp444_AST));
			}
			match(POINTERTOMBR);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		optor_simple_tokclass_AST = RefPNode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			recover(ex,_tokenSet_43);
		} else {
			throw;
		}
	}
	returnAST = optor_simple_tokclass_AST;
}

void CPPParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.registerFactory(157, "CFunctionNode", CFunctionNode::factory);
	factory.registerFactory(159, "CDeclarationNode", CDeclarationNode::factory);
	factory.registerFactory(160, "CExpressionNode", CExpressionNode::factory);
	factory.registerFactory(163, "CClassNode", CClassNode::factory);
	factory.registerFactory(164, "CTemplateNode", CTemplateNode::factory);
	factory.registerFactory(165, "CNamespaceNode", CNamespaceNode::factory);
	factory.registerFactory(166, "CTypedefNode", CTypedefNode::factory);
	factory.registerFactory(168, "CTypeNode", CTypeNode::factory);
	factory.setMaxNodeType(192);
}
const char* CPPParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"template\"",
	"LESSTHAN",
	"GREATERTHAN",
	"\"typedef\"",
	"\"enum\"",
	"SEMICOLON",
	"ID",
	"LCURLY",
	"\"inline\"",
	"\"extern\"",
	"\"namespace\"",
	"\"antlrTrace_on\"",
	"\"antlrTrace_off\"",
	"RCURLY",
	"ASSIGNEQUAL",
	"COLON",
	"OCTALINT",
	"\"friend\"",
	"StringLiteral",
	"\"struct\"",
	"\"union\"",
	"\"class\"",
	"\"_declspec\"",
	"\"__declspec\"",
	"LPAREN",
	"RPAREN",
	"\"auto\"",
	"\"register\"",
	"\"static\"",
	"\"mutable\"",
	"\"_inline\"",
	"\"__inline\"",
	"\"virtual\"",
	"\"explicit\"",
	"\"typename\"",
	"\"char\"",
	"\"wchar_t\"",
	"\"bool\"",
	"\"short\"",
	"\"int\"",
	"\"_int8\"",
	"\"__int8\"",
	"\"_int16\"",
	"\"__int16\"",
	"\"_int32\"",
	"\"__int32\"",
	"\"_int64\"",
	"\"__int64\"",
	"\"_w64\"",
	"\"__w64\"",
	"\"long\"",
	"\"signed\"",
	"\"unsigned\"",
	"\"float\"",
	"\"double\"",
	"\"void\"",
	"\"const\"",
	"\"volatile\"",
	"COMMA",
	"\"public\"",
	"\"protected\"",
	"\"private\"",
	"\"operator\"",
	"TILDE",
	"\"typeid\"",
	"LSQUARE",
	"RSQUARE",
	"STAR",
	"AMPERSAND",
	"ELLIPSIS",
	"\"throw\"",
	"\"using\"",
	"\"case\"",
	"\"default\"",
	"\"if\"",
	"\"else\"",
	"\"switch\"",
	"\"while\"",
	"\"do\"",
	"\"for\"",
	"\"goto\"",
	"\"continue\"",
	"\"break\"",
	"\"return\"",
	"\"__try\"",
	"\"__except\"",
	"\"__finally\"",
	"\"try\"",
	"\"catch\"",
	"TIMESEQUAL",
	"DIVIDEEQUAL",
	"MINUSEQUAL",
	"PLUSEQUAL",
	"MODEQUAL",
	"SHIFTLEFTEQUAL",
	"SHIFTRIGHTEQUAL",
	"BITWISEANDEQUAL",
	"BITWISEXOREQUAL",
	"BITWISEOREQUAL",
	"QUESTIONMARK",
	"OR",
	"AND",
	"BITWISEOR",
	"BITWISEXOR",
	"NOTEQUAL",
	"EQUAL",
	"LESSTHANOREQUALTO",
	"GREATERTHANOREQUALTO",
	"SHIFTLEFT",
	"SHIFTRIGHT",
	"PLUS",
	"MINUS",
	"DIVIDE",
	"MOD",
	"DOTMBR",
	"POINTERTOMBR",
	"PLUSPLUS",
	"MINUSMINUS",
	"\"sizeof\"",
	"\"__alignof__\"",
	"SCOPE",
	"\"dynamic_cast\"",
	"\"static_cast\"",
	"\"reinterpret_cast\"",
	"\"const_cast\"",
	"DOT",
	"POINTERTO",
	"\"this\"",
	"DECIMALINT",
	"HEXADECIMALINT",
	"CharLiteral",
	"WCharLiteral",
	"WStringLiteral",
	"FLOATONE",
	"FLOATTWO",
	"\"true\"",
	"\"false\"",
	"NOT",
	"\"new\"",
	"\"_cdecl\"",
	"\"__cdecl\"",
	"\"_near\"",
	"\"__near\"",
	"\"_far\"",
	"\"__far\"",
	"\"__interrupt\"",
	"\"pascal\"",
	"\"_pascal\"",
	"\"__pascal\"",
	"\"_stdcall\"",
	"\"__stdcall\"",
	"\"delete\"",
	"MY",
	"MYFUNCTION",
	"MYPARAM",
	"MYDECLAR",
	"MYEXPRESSION",
	"MYBODY",
	"MYSTATEMENT",
	"MYCLASS",
	"MYTEMPLATE",
	"MYNAMESPACE",
	"MYTYPEDEF",
	"MYENUM",
	"MYTYPE",
	"MYCALL",
	"MYNEW",
	"Whitespace",
	"Comment",
	"CPPComment",
	"a line directive",
	"LineDirective",
	"Space",
	"Pragma",
	"Error",
	"EndOfContinuedLine",
	"EndOfLine",
	"Escape",
	"Digit",
	"Decimal",
	"LongSuffix",
	"UnsignedSuffix",
	"FloatSuffix",
	"Exponent",
	"UniversalCharacterName",
	"HexQuad",
	"HexadecimalDigit",
	"Vocabulary",
	"Number",
	0
};

const unsigned long CPPParser::_tokenSet_0_data_[] = { 3751933840UL, 1073741823UL, 2460UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" "friend" "struct" "union" "class" "_declspec" 
// "__declspec" LPAREN "auto" "register" "static" "mutable" "_inline" "__inline" 
// "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" 
// "volatile" "operator" TILDE "typeid" STAR AMPERSAND "using" SCOPE "_cdecl" 
// "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" 
// "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_0(_tokenSet_0_data_,12);
const unsigned long CPPParser::_tokenSet_1_data_[] = { 2UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_1(_tokenSet_1_data_,8);
const unsigned long CPPParser::_tokenSet_2_data_[] = { 3483383168UL, 1073741823UL, 2048UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" "auto" "register" "static" "mutable" "_inline" 
// "__inline" "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" 
// "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" 
// "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" "double" 
// "void" "const" "volatile" "using" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_2(_tokenSet_2_data_,8);
const unsigned long CPPParser::_tokenSet_3_data_[] = { 3756554144UL, 1073741823UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" "namespace" 
// COLON "friend" StringLiteral "struct" "union" "class" "_declspec" "__declspec" 
// LPAREN "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "operator" TILDE "typeid" STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_3(_tokenSet_3_data_,12);
const unsigned long CPPParser::_tokenSet_4_data_[] = { 3483383168UL, 1073741823UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" "auto" "register" "static" "mutable" "_inline" 
// "__inline" "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" 
// "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" 
// "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" "double" 
// "void" "const" "volatile" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_4(_tokenSet_4_data_,8);
const unsigned long CPPParser::_tokenSet_5_data_[] = { 3751818656UL, 1073741823UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" 
// "class" "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" TILDE "typeid" STAR AMPERSAND 
// SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_5(_tokenSet_5_data_,12);
const unsigned long CPPParser::_tokenSet_6_data_[] = { 4096UL, 60UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "inline" "_inline" "__inline" "virtual" "explicit" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_6(_tokenSet_6_data_,8);
const unsigned long CPPParser::_tokenSet_7_data_[] = { 3072UL, 0UL, 28UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID LCURLY "operator" TILDE "typeid" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_7(_tokenSet_7_data_,8);
const unsigned long CPPParser::_tokenSet_8_data_[] = { 5136UL, 28UL, 8UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "template" ID "inline" "_inline" "__inline" "virtual" TILDE SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_8(_tokenSet_8_data_,8);
const unsigned long CPPParser::_tokenSet_9_data_[] = { 5152UL, 28UL, 8UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN ID "inline" "_inline" "__inline" "virtual" TILDE SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_9(_tokenSet_9_data_,8);
const unsigned long CPPParser::_tokenSet_10_data_[] = { 5120UL, 44UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID "inline" "_inline" "__inline" "explicit" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_10(_tokenSet_10_data_,8);
const unsigned long CPPParser::_tokenSet_11_data_[] = { 268440608UL, 44UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN ID "inline" LPAREN "_inline" "__inline" "explicit" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_11(_tokenSet_11_data_,8);
const unsigned long CPPParser::_tokenSet_12_data_[] = { 5120UL, 0UL, 4UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID "inline" "operator" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_12(_tokenSet_12_data_,8);
const unsigned long CPPParser::_tokenSet_13_data_[] = { 3483383200UL, 1073741823UL, 4UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" 
// "class" "_declspec" "__declspec" "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_13(_tokenSet_13_data_,8);
const unsigned long CPPParser::_tokenSet_14_data_[] = { 3751818624UL, 1073741823UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" TILDE "typeid" STAR AMPERSAND 
// SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_14(_tokenSet_14_data_,12);
const unsigned long CPPParser::_tokenSet_15_data_[] = { 3752080864UL, 2147483647UL, 3758096828UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" ASSIGNEQUAL 
// "friend" "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" 
// "register" "static" "mutable" "_inline" "__inline" "virtual" "explicit" 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "const" "volatile" COMMA 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL 
// MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL 
// BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL 
// EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS 
// MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT 
// "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_15(_tokenSet_15_data_,12);
const unsigned long CPPParser::_tokenSet_16_data_[] = { 58732544UL, 60UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "inline" "extern" "struct" "union" "class" "_inline" "__inline" "virtual" 
// "explicit" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_16(_tokenSet_16_data_,8);
const unsigned long CPPParser::_tokenSet_17_data_[] = { 260054016UL, 60UL, 28UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID LCURLY "inline" "struct" "union" "class" "_declspec" "__declspec" 
// "_inline" "__inline" "virtual" "explicit" "operator" TILDE "typeid" 
// SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_17(_tokenSet_17_data_,8);
const unsigned long CPPParser::_tokenSet_18_data_[] = { 3752064914UL, 1073741823UL, 2460UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" "typedef" "enum" SEMICOLON ID "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" RCURLY "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" TILDE "typeid" STAR AMPERSAND 
// "using" SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_18(_tokenSet_18_data_,12);
const unsigned long CPPParser::_tokenSet_19_data_[] = { 268961280UL, 0UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON ID COLON LPAREN "operator" TILDE "typeid" STAR AMPERSAND SCOPE 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_19(_tokenSet_19_data_,12);
const unsigned long CPPParser::_tokenSet_20_data_[] = { 512UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_20(_tokenSet_20_data_,8);
const unsigned long CPPParser::_tokenSet_21_data_[] = { 806225504UL, 1073741824UL, 3758097404UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON ID RCURLY ASSIGNEQUAL COLON LPAREN RPAREN 
// COMMA "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS 
// TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO 
// SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR SCOPE 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_21(_tokenSet_21_data_,12);
const unsigned long CPPParser::_tokenSet_22_data_[] = { 1024UL, 0UL, 384UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" 
// "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_22(_tokenSet_22_data_,12);
const unsigned long CPPParser::_tokenSet_23_data_[] = { 268436512UL, 805306368UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN ID LPAREN "const" "volatile" "operator" TILDE "typeid" STAR 
// AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_23(_tokenSet_23_data_,12);
const unsigned long CPPParser::_tokenSet_24_data_[] = { 268436480UL, 0UL, 28UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID LPAREN "operator" TILDE "typeid" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_24(_tokenSet_24_data_,8);
const unsigned long CPPParser::_tokenSet_25_data_[] = { 327419232UL, 1342177216UL, 3758096828UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" ID ASSIGNEQUAL "struct" "union" "class" 
// LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" COMMA "operator" 
// TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT "new" "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_25(_tokenSet_25_data_,12);
const unsigned long CPPParser::_tokenSet_26_data_[] = { 3483385728UL, 1073741823UL, 2048UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "friend" "struct" 
// "union" "class" "_declspec" "__declspec" "auto" "register" "static" 
// "mutable" "_inline" "__inline" "virtual" "explicit" "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" "const" "volatile" "using" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_26(_tokenSet_26_data_,8);
const unsigned long CPPParser::_tokenSet_27_data_[] = { 3757309842UL, 3221225471UL, 167771551UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" 
// "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" 
// StringLiteral "struct" "union" "class" "_declspec" "__declspec" LPAREN 
// "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "public" "protected" "private" "operator" TILDE "typeid" STAR AMPERSAND 
// "throw" "using" "case" "default" "if" "else" "switch" "while" "do" "for" 
// "goto" "continue" "break" "return" "__try" "try" PLUS MINUS PLUSPLUS 
// MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" 
// "reinterpret_cast" "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral 
// WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_27(_tokenSet_27_data_,12);
const unsigned long CPPParser::_tokenSet_28_data_[] = { 3752343456UL, 1073741823UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" COLON "friend" 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "operator" TILDE 
// "typeid" STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" 
// "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_28(_tokenSet_28_data_,12);
const unsigned long CPPParser::_tokenSet_29_data_[] = { 1024UL, 0UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_29(_tokenSet_29_data_,8);
const unsigned long CPPParser::_tokenSet_30_data_[] = { 3758096370UL, 4294967295UL, 3925867967UL, 4290772863UL, 268435453UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY 
// ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "public" "protected" "private" 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "try" TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" 
// "reinterpret_cast" "const_cast" POINTERTO "this" DECIMALINT HEXADECIMALINT 
// CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" 
// NOT "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_30(_tokenSet_30_data_,12);
const unsigned long CPPParser::_tokenSet_31_data_[] = { 3752048528UL, 3221225471UL, 2463UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID "inline" "extern" "antlrTrace_on" 
// "antlrTrace_off" RCURLY "friend" "struct" "union" "class" "_declspec" 
// "__declspec" LPAREN "auto" "register" "static" "mutable" "_inline" "__inline" 
// "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" 
// "volatile" "public" "protected" "private" "operator" TILDE "typeid" 
// STAR AMPERSAND "using" SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" 
// "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_31(_tokenSet_31_data_,12);
const unsigned long CPPParser::_tokenSet_32_data_[] = { 3751917456UL, 3221225471UL, 2463UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID "inline" "extern" "antlrTrace_on" 
// "antlrTrace_off" "friend" "struct" "union" "class" "_declspec" "__declspec" 
// LPAREN "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "public" "protected" "private" "operator" TILDE "typeid" STAR AMPERSAND 
// "using" SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_32(_tokenSet_32_data_,12);
const unsigned long CPPParser::_tokenSet_33_data_[] = { 3751818624UL, 1073741823UL, 2460UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" TILDE "typeid" STAR AMPERSAND 
// "using" SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_33(_tokenSet_33_data_,12);
const unsigned long CPPParser::_tokenSet_34_data_[] = { 264704UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON LCURLY ASSIGNEQUAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_34(_tokenSet_34_data_,8);
const unsigned long CPPParser::_tokenSet_35_data_[] = { 1024UL, 0UL, 156UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID "operator" TILDE "typeid" STAR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_35(_tokenSet_35_data_,8);
const unsigned long CPPParser::_tokenSet_36_data_[] = { 3483385216UL, 1073741823UL, 2048UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID LCURLY "inline" "extern" "friend" "struct" "union" 
// "class" "_declspec" "__declspec" "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "using" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_36(_tokenSet_36_data_,8);
const unsigned long CPPParser::_tokenSet_37_data_[] = { 3757834160UL, 3221225471UL, 167738783UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" LESSTHAN "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" 
// "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY COLON OCTALINT "friend" 
// StringLiteral "struct" "union" "class" "_declspec" "__declspec" LPAREN 
// "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "public" "protected" "private" "operator" TILDE "typeid" STAR AMPERSAND 
// "throw" "using" "case" "default" "if" "switch" "while" "do" "for" "goto" 
// "continue" "break" "return" "__try" "try" PLUS MINUS PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_37(_tokenSet_37_data_,12);
const unsigned long CPPParser::_tokenSet_38_data_[] = { 3757309840UL, 3221225471UL, 167738783UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "public" "protected" 
// "private" "operator" TILDE "typeid" STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "switch" "while" "do" "for" "goto" "continue" "break" 
// "return" "__try" "try" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_38(_tokenSet_38_data_,12);
const unsigned long CPPParser::_tokenSet_39_data_[] = { 268436480UL, 0UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID LPAREN "operator" TILDE "typeid" STAR AMPERSAND SCOPE "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_39(_tokenSet_39_data_,12);
const unsigned long CPPParser::_tokenSet_40_data_[] = { 327419232UL, 2147483584UL, 3758096828UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" ID ASSIGNEQUAL "struct" "union" "class" 
// LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// COMMA "operator" TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL 
// MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL 
// BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL 
// EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS 
// MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT 
// "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_40(_tokenSet_40_data_,12);
const unsigned long CPPParser::_tokenSet_41_data_[] = { 3757309840UL, 3221225471UL, 167771551UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "public" "protected" 
// "private" "operator" TILDE "typeid" STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "try" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" 
// "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_41(_tokenSet_41_data_,12);
const unsigned long CPPParser::_tokenSet_42_data_[] = { 3757061504UL, 1073741823UL, 412UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "operator" TILDE 
// "typeid" STAR AMPERSAND PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_42(_tokenSet_42_data_,12);
const unsigned long CPPParser::_tokenSet_43_data_[] = { 806227552UL, 1073741824UL, 3758097404UL, 335544319UL, 134184966UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON ID LCURLY RCURLY ASSIGNEQUAL COLON LPAREN 
// RPAREN COMMA "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO 
// SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS 
// MINUSMINUS SCOPE DOT POINTERTO "_cdecl" "__cdecl" "_near" "__near" "_far" 
// "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_43(_tokenSet_43_data_,12);
const unsigned long CPPParser::_tokenSet_44_data_[] = { 4294967282UL, 4294967295UL, 3925868543UL, 4294967295UL, 268435455UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY 
// ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "public" "protected" "private" 
// "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS "throw" 
// "using" "case" "default" "if" "else" "switch" "while" "do" "for" "goto" 
// "continue" "break" "return" "__try" "try" TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" DOT 
// POINTERTO "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_44(_tokenSet_44_data_,12);
const unsigned long CPPParser::_tokenSet_45_data_[] = { 5120UL, 28UL, 8UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID "inline" "_inline" "__inline" "virtual" TILDE SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_45(_tokenSet_45_data_,8);
const unsigned long CPPParser::_tokenSet_46_data_[] = { 3483383168UL, 1073741823UL, 4UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" "auto" "register" "static" "mutable" "_inline" 
// "__inline" "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" 
// "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" 
// "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" "double" 
// "void" "const" "volatile" "operator" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_46(_tokenSet_46_data_,8);
const unsigned long CPPParser::_tokenSet_47_data_[] = { 1024UL, 0UL, 28UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID "operator" TILDE "typeid" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_47(_tokenSet_47_data_,8);
const unsigned long CPPParser::_tokenSet_48_data_[] = { 327419744UL, 1342177216UL, 3758096828UL, 331349887UL, 134242308UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID ASSIGNEQUAL "struct" "union" 
// "class" LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" COMMA "operator" 
// TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_48(_tokenSet_48_data_,12);
const unsigned long CPPParser::_tokenSet_49_data_[] = { 60821504UL, 60UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "inline" "friend" "struct" "union" "class" "_inline" "__inline" "virtual" 
// "explicit" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_49(_tokenSet_49_data_,8);
const unsigned long CPPParser::_tokenSet_50_data_[] = { 268440576UL, 60UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID "inline" LPAREN "_inline" "__inline" "virtual" "explicit" "operator" 
// TILDE "typeid" STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_50(_tokenSet_50_data_,12);
const unsigned long CPPParser::_tokenSet_51_data_[] = { 327423328UL, 2147483644UL, 3758096828UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" ID "inline" ASSIGNEQUAL "struct" "union" 
// "class" LPAREN "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" COMMA "operator" 
// TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT "new" "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_51(_tokenSet_51_data_,12);
const unsigned long CPPParser::_tokenSet_52_data_[] = { 526848UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON LCURLY COLON 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_52(_tokenSet_52_data_,8);
const unsigned long CPPParser::_tokenSet_53_data_[] = { 3757309842UL, 3221225471UL, 536870303UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" 
// "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" 
// StringLiteral "struct" "union" "class" "_declspec" "__declspec" LPAREN 
// "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "public" "protected" "private" "operator" TILDE "typeid" STAR AMPERSAND 
// "throw" "using" "case" "default" "if" "else" "switch" "while" "do" "for" 
// "goto" "continue" "break" "return" "__try" "__except" "__finally" "try" 
// "catch" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE 
// "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_53(_tokenSet_53_data_,12);
const unsigned long CPPParser::_tokenSet_54_data_[] = { 525312UL, 16UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID COLON "virtual" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_54(_tokenSet_54_data_,8);
const unsigned long CPPParser::_tokenSet_55_data_[] = { 64UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_55(_tokenSet_55_data_,8);
const unsigned long CPPParser::_tokenSet_56_data_[] = { 2048UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LCURLY 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_56(_tokenSet_56_data_,8);
const unsigned long CPPParser::_tokenSet_57_data_[] = { 58721536UL, 268435392UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "enum" ID "struct" "union" "class" "typename" "char" "wchar_t" "bool" 
// "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_57(_tokenSet_57_data_,8);
const unsigned long CPPParser::_tokenSet_58_data_[] = { 806225504UL, 1879048192UL, 3758097404UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON ID RCURLY ASSIGNEQUAL COLON LPAREN RPAREN 
// "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE RSQUARE STAR 
// AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR 
// POINTERTOMBR SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" 
// "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_58(_tokenSet_58_data_,12);
const unsigned long CPPParser::_tokenSet_59_data_[] = { 4289609698UL, 2147483647UL, 3758100476UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY "inline" 
// "extern" RCURLY ASSIGNEQUAL COLON "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE 
// RSQUARE STAR AMPERSAND ELLIPSIS "throw" "using" TIMESEQUAL DIVIDEEQUAL 
// MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL 
// BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR SCOPE "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_59(_tokenSet_59_data_,12);
const unsigned long CPPParser::_tokenSet_60_data_[] = { 806227552UL, 1879048192UL, 3758097404UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON ID LCURLY RCURLY ASSIGNEQUAL COLON LPAREN 
// RPAREN "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE RSQUARE 
// STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR SCOPE "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_60(_tokenSet_60_data_,12);
const unsigned long CPPParser::_tokenSet_61_data_[] = { 537395712UL, 0UL, 64UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON COLON RPAREN RSQUARE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_61(_tokenSet_61_data_,8);
const unsigned long CPPParser::_tokenSet_62_data_[] = { 332924160UL, 268435392UL, 412UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "enum" ID COLON OCTALINT StringLiteral "struct" "union" "class" LPAREN 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "operator" TILDE "typeid" 
// STAR AMPERSAND PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_62(_tokenSet_62_data_,12);
const unsigned long CPPParser::_tokenSet_63_data_[] = { 327419744UL, 2147483584UL, 3758096828UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID ASSIGNEQUAL "struct" "union" 
// "class" LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// COMMA "operator" TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL 
// MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL 
// BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL 
// EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS 
// MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT 
// "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_63(_tokenSet_63_data_,12);
const unsigned long CPPParser::_tokenSet_64_data_[] = { 332401920UL, 268435392UL, 412UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "enum" ID LCURLY OCTALINT StringLiteral "struct" "union" "class" LPAREN 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "operator" TILDE "typeid" 
// STAR AMPERSAND PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_64(_tokenSet_64_data_,12);
const unsigned long CPPParser::_tokenSet_65_data_[] = { 512UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_65(_tokenSet_65_data_,8);
const unsigned long CPPParser::_tokenSet_66_data_[] = { 2048UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LCURLY COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_66(_tokenSet_66_data_,8);
const unsigned long CPPParser::_tokenSet_67_data_[] = { 131072UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// RCURLY 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_67(_tokenSet_67_data_,8);
const unsigned long CPPParser::_tokenSet_68_data_[] = { 131072UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// RCURLY COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_68(_tokenSet_68_data_,8);
const unsigned long CPPParser::_tokenSet_69_data_[] = { 655872UL, 1073741824UL, 64UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON RCURLY COLON COMMA RSQUARE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_69(_tokenSet_69_data_,8);
const unsigned long CPPParser::_tokenSet_70_data_[] = { 864290656UL, 1342177216UL, 3758097340UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID ASSIGNEQUAL "struct" "union" 
// "class" LPAREN RPAREN "typename" "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" COMMA 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL 
// DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL 
// BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO 
// NOT "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_70(_tokenSet_70_data_,12);
const unsigned long CPPParser::_tokenSet_71_data_[] = { 805569088UL, 1073741824UL, 512UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON ASSIGNEQUAL LPAREN RPAREN COMMA ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_71(_tokenSet_71_data_,8);
const unsigned long CPPParser::_tokenSet_72_data_[] = { 131584UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON RCURLY COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_72(_tokenSet_72_data_,8);
const unsigned long CPPParser::_tokenSet_73_data_[] = { 536870912UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_73(_tokenSet_73_data_,8);
const unsigned long CPPParser::_tokenSet_74_data_[] = { 332399872UL, 268435392UL, 412UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "enum" ID OCTALINT StringLiteral "struct" "union" "class" LPAREN "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "operator" TILDE "typeid" STAR AMPERSAND 
// PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" 
// "static_cast" "reinterpret_cast" "const_cast" "this" DECIMALINT HEXADECIMALINT 
// CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" 
// NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_74(_tokenSet_74_data_,12);
const unsigned long CPPParser::_tokenSet_75_data_[] = { 4294850528UL, 2147483647UL, 3758097404UL, 4294967295UL, 134250495UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" 
// RCURLY ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral "struct" "union" 
// "class" "_declspec" "__declspec" LPAREN RPAREN "auto" "register" "static" 
// "mutable" "_inline" "__inline" "virtual" "explicit" "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" 
// LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" DOT 
// POINTERTO "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_75(_tokenSet_75_data_,12);
const unsigned long CPPParser::_tokenSet_76_data_[] = { 537526848UL, 1073741824UL, 576UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY COLON RPAREN COMMA RSQUARE ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_76(_tokenSet_76_data_,8);
const unsigned long CPPParser::_tokenSet_77_data_[] = { 327419232UL, 1342177216UL, 3758096828UL, 331349887UL, 134242308UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" ID ASSIGNEQUAL "struct" "union" "class" 
// LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" COMMA "operator" 
// TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS SCOPE POINTERTO NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_77(_tokenSet_77_data_,12);
const unsigned long CPPParser::_tokenSet_78_data_[] = { 864290656UL, 1342177216UL, 3758097340UL, 331349887UL, 134242308UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID ASSIGNEQUAL "struct" "union" 
// "class" LPAREN RPAREN "typename" "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" COMMA 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL 
// DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL 
// BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO 
// NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_78(_tokenSet_78_data_,12);
const unsigned long CPPParser::_tokenSet_79_data_[] = { 4293932416UL, 1073741823UL, 988UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN RPAREN "auto" 
// "register" "static" "mutable" "_inline" "__inline" "virtual" "explicit" 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "const" "volatile" "operator" 
// TILDE "typeid" RSQUARE STAR AMPERSAND ELLIPSIS PLUS MINUS PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_79(_tokenSet_79_data_,12);
const unsigned long CPPParser::_tokenSet_80_data_[] = { 4294967250UL, 4294967295UL, 167772095UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY "inline" 
// "extern" "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY ASSIGNEQUAL 
// COLON OCTALINT "friend" StringLiteral "struct" "union" "class" "_declspec" 
// "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" "_inline" 
// "__inline" "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" 
// "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" 
// "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" "double" 
// "void" "const" "volatile" COMMA "public" "protected" "private" "operator" 
// TILDE "typeid" LSQUARE STAR AMPERSAND ELLIPSIS "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "try" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" 
// "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_80(_tokenSet_80_data_,12);
const unsigned long CPPParser::_tokenSet_81_data_[] = { 4289478592UL, 2147483647UL, 2560UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" ASSIGNEQUAL 
// COLON "friend" "struct" "union" "class" "_declspec" "__declspec" LPAREN 
// RPAREN "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// COMMA ELLIPSIS "using" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_81(_tokenSet_81_data_,8);
const unsigned long CPPParser::_tokenSet_82_data_[] = { 268435488UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN LPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_82(_tokenSet_82_data_,8);
const unsigned long CPPParser::_tokenSet_83_data_[] = { 3483647872UL, 1073741823UL, 3072UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" ASSIGNEQUAL "friend" 
// "struct" "union" "class" "_declspec" "__declspec" "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "throw" "using" 
// SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_83(_tokenSet_83_data_,8);
const unsigned long CPPParser::_tokenSet_84_data_[] = { 526336UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LCURLY COLON 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_84(_tokenSet_84_data_,8);
const unsigned long CPPParser::_tokenSet_85_data_[] = { 268435456UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_85(_tokenSet_85_data_,8);
const unsigned long CPPParser::_tokenSet_86_data_[] = { 1024UL, 0UL, 8UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// ID TILDE SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_86(_tokenSet_86_data_,8);
const unsigned long CPPParser::_tokenSet_87_data_[] = { 536870912UL, 0UL, 512UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// RPAREN ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_87(_tokenSet_87_data_,8);
const unsigned long CPPParser::_tokenSet_88_data_[] = { 4288951776UL, 2147483647UL, 956UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" ASSIGNEQUAL 
// "friend" "struct" "union" "class" "_declspec" "__declspec" LPAREN RPAREN 
// "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// COMMA "operator" TILDE "typeid" LSQUARE STAR AMPERSAND ELLIPSIS SCOPE 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_88(_tokenSet_88_data_,12);
const unsigned long CPPParser::_tokenSet_89_data_[] = { 864290144UL, 2147483584UL, 3758097340UL, 331349887UL, 268427268UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" ID ASSIGNEQUAL "struct" "union" "class" 
// LPAREN RPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// COMMA "operator" TILDE "typeid" LSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL 
// DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL 
// BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD POINTERTOMBR PLUSPLUS MINUSMINUS SCOPE POINTERTO 
// NOT "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_89(_tokenSet_89_data_,12);
const unsigned long CPPParser::_tokenSet_90_data_[] = { 805569600UL, 1073741824UL, 928UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN ID ASSIGNEQUAL LPAREN RPAREN COMMA LSQUARE STAR AMPERSAND 
// ELLIPSIS SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_90(_tokenSet_90_data_,12);
const unsigned long CPPParser::_tokenSet_91_data_[] = { 4294721520UL, 2147483647UL, 4092UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN RPAREN "auto" 
// "register" "static" "mutable" "_inline" "__inline" "virtual" "explicit" 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "const" "volatile" COMMA 
// "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS "throw" 
// "using" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE 
// "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_91(_tokenSet_91_data_,12);
const unsigned long CPPParser::_tokenSet_92_data_[] = { 536870976UL, 1073741824UL, 512UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN RPAREN COMMA ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_92(_tokenSet_92_data_,8);
const unsigned long CPPParser::_tokenSet_93_data_[] = { 805307392UL, 0UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID LPAREN RPAREN LSQUARE STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_93(_tokenSet_93_data_,12);
const unsigned long CPPParser::_tokenSet_94_data_[] = { 805568576UL, 1073741824UL, 544UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN ASSIGNEQUAL LPAREN RPAREN COMMA LSQUARE ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_94(_tokenSet_94_data_,8);
const unsigned long CPPParser::_tokenSet_95_data_[] = { 4294852594UL, 2147483647UL, 3758100476UL, 4294967295UL, 268435455UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" RCURLY ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN RPAREN "auto" 
// "register" "static" "mutable" "_inline" "__inline" "virtual" "explicit" 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "const" "volatile" COMMA 
// "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS "throw" 
// "using" TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO 
// SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS 
// MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" 
// "reinterpret_cast" "const_cast" DOT POINTERTO "this" DECIMALINT HEXADECIMALINT 
// CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" 
// NOT "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_95(_tokenSet_95_data_,12);
const unsigned long CPPParser::_tokenSet_96_data_[] = { 537133120UL, 1073741824UL, 512UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN ASSIGNEQUAL RPAREN COMMA ELLIPSIS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_96(_tokenSet_96_data_,8);
const unsigned long CPPParser::_tokenSet_97_data_[] = { 536870976UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN RPAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_97(_tokenSet_97_data_,8);
const unsigned long CPPParser::_tokenSet_98_data_[] = { 806225506UL, 1073741824UL, 3758098428UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF LESSTHAN GREATERTHAN SEMICOLON ID RCURLY ASSIGNEQUAL COLON LPAREN 
// RPAREN COMMA "operator" TILDE "typeid" LSQUARE RSQUARE STAR AMPERSAND 
// ELLIPSIS "throw" TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR 
// POINTERTOMBR SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" 
// "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_98(_tokenSet_98_data_,12);
const unsigned long CPPParser::_tokenSet_99_data_[] = { 33554448UL, 64UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "class" "typename" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_99(_tokenSet_99_data_,8);
const unsigned long CPPParser::_tokenSet_100_data_[] = { 1120UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN ID COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_100(_tokenSet_100_data_,8);
const unsigned long CPPParser::_tokenSet_101_data_[] = { 3751818624UL, 1073741823UL, 924UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" "operator" TILDE "typeid" STAR AMPERSAND 
// ELLIPSIS SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_101(_tokenSet_101_data_,12);
const unsigned long CPPParser::_tokenSet_102_data_[] = { 64UL, 1073741824UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_102(_tokenSet_102_data_,8);
const unsigned long CPPParser::_tokenSet_103_data_[] = { 3752080864UL, 2147483647UL, 444UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" ASSIGNEQUAL 
// "friend" "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" 
// "register" "static" "mutable" "_inline" "__inline" "virtual" "explicit" 
// "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" 
// "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" 
// "signed" "unsigned" "float" "double" "void" "const" "volatile" COMMA 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND SCOPE "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_103(_tokenSet_103_data_,12);
const unsigned long CPPParser::_tokenSet_104_data_[] = { 268698688UL, 1073741824UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN ID ASSIGNEQUAL LPAREN COMMA LSQUARE STAR AMPERSAND SCOPE 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_104(_tokenSet_104_data_,12);
const unsigned long CPPParser::_tokenSet_105_data_[] = { 4294194672UL, 2147483647UL, 3068UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" 
// ASSIGNEQUAL OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE 
// RSQUARE STAR AMPERSAND ELLIPSIS "using" PLUS MINUS PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_105(_tokenSet_105_data_,12);
const unsigned long CPPParser::_tokenSet_106_data_[] = { 268436576UL, 1073741824UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN ID LPAREN COMMA LSQUARE STAR AMPERSAND SCOPE "_cdecl" 
// "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" 
// "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_106(_tokenSet_106_data_,12);
const unsigned long CPPParser::_tokenSet_107_data_[] = { 268436576UL, 1342177152UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN ID LPAREN "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" COMMA 
// LSQUARE STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" 
// "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_107(_tokenSet_107_data_,12);
const unsigned long CPPParser::_tokenSet_108_data_[] = { 537789024UL, 1073741824UL, 3758097216UL, 262143UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA 
// RSQUARE AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_108(_tokenSet_108_data_,8);
const unsigned long CPPParser::_tokenSet_109_data_[] = { 3751818720UL, 2147483647UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" "friend" 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" COMMA LSQUARE 
// STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" 
// "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_109(_tokenSet_109_data_,12);
const unsigned long CPPParser::_tokenSet_110_data_[] = { 3757323744UL, 2147483647UL, 3758096828UL, 4294967167UL, 134250495UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" ID "inline" "extern" ASSIGNEQUAL 
// OCTALINT "friend" StringLiteral "struct" "union" "class" "_declspec" 
// "__declspec" LPAREN "auto" "register" "static" "mutable" "_inline" "__inline" 
// "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" "int" 
// "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" 
// "_w64" "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" 
// "volatile" COMMA "operator" TILDE "typeid" LSQUARE STAR AMPERSAND TIMESEQUAL 
// DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL 
// BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" 
// "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" DOT POINTERTO "this" DECIMALINT HEXADECIMALINT CharLiteral 
// WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" 
// "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_110(_tokenSet_110_data_,12);
const unsigned long CPPParser::_tokenSet_111_data_[] = { 537789024UL, 1073741824UL, 3758097216UL, 65535UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA 
// RSQUARE AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_111(_tokenSet_111_data_,8);
const unsigned long CPPParser::_tokenSet_112_data_[] = { 3757178768UL, 3221225471UL, 167738783UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" OCTALINT "friend" StringLiteral "struct" 
// "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" "static" 
// "mutable" "_inline" "__inline" "virtual" "explicit" "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" "const" "volatile" "public" "protected" "private" 
// "operator" TILDE "typeid" STAR AMPERSAND "throw" "using" "case" "default" 
// "if" "switch" "while" "do" "for" "goto" "continue" "break" "return" 
// "__try" "try" PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_112(_tokenSet_112_data_,12);
const unsigned long CPPParser::_tokenSet_113_data_[] = { 3483399552UL, 1073741823UL, 2048UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "namespace" "friend" "struct" 
// "union" "class" "_declspec" "__declspec" "auto" "register" "static" 
// "mutable" "_inline" "__inline" "virtual" "explicit" "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" "const" "volatile" "using" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_113(_tokenSet_113_data_,8);
const unsigned long CPPParser::_tokenSet_114_data_[] = { 3752359840UL, 1073741823UL, 412UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" "namespace" 
// COLON "friend" "struct" "union" "class" "_declspec" "__declspec" LPAREN 
// "auto" "register" "static" "mutable" "_inline" "__inline" "virtual" 
// "explicit" "typename" "char" "wchar_t" "bool" "short" "int" "_int8" 
// "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" 
// "__w64" "long" "signed" "unsigned" "float" "double" "void" "const" "volatile" 
// "operator" TILDE "typeid" STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" 
// "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_114(_tokenSet_114_data_,12);
const unsigned long CPPParser::_tokenSet_115_data_[] = { 3758096368UL, 4294967295UL, 3925867967UL, 4290772863UL, 268435453UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY 
// ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "public" "protected" "private" 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "try" TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD POINTERTOMBR 
// PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" 
// "reinterpret_cast" "const_cast" POINTERTO "this" DECIMALINT HEXADECIMALINT 
// CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" 
// NOT "new" "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" 
// "pascal" "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_115(_tokenSet_115_data_,12);
const unsigned long CPPParser::_tokenSet_116_data_[] = { 1056UL, 805306368UL, 156UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN ID "const" "volatile" "operator" TILDE "typeid" STAR SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_116(_tokenSet_116_data_,8);
const unsigned long CPPParser::_tokenSet_117_data_[] = { 3757324256UL, 2147483647UL, 3758096828UL, 4294967295UL, 134250495UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" 
// ASSIGNEQUAL OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE 
// STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR 
// POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" 
// "static_cast" "reinterpret_cast" "const_cast" DOT POINTERTO "this" DECIMALINT 
// HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO 
// "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_117(_tokenSet_117_data_,12);
const unsigned long CPPParser::_tokenSet_118_data_[] = { 3758096370UL, 4294967295UL, 4294966719UL, 4294967295UL, 268435455UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "template" LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID LCURLY 
// "inline" "extern" "namespace" "antlrTrace_on" "antlrTrace_off" RCURLY 
// ASSIGNEQUAL COLON OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "public" "protected" "private" 
// "operator" TILDE "typeid" LSQUARE STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "__except" "__finally" "try" "catch" TIMESEQUAL 
// DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL 
// BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR 
// BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT 
// SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" DOT POINTERTO "this" DECIMALINT HEXADECIMALINT CharLiteral 
// WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_118(_tokenSet_118_data_,12);
const unsigned long CPPParser::_tokenSet_119_data_[] = { 332400384UL, 268435392UL, 412UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "enum" SEMICOLON ID OCTALINT StringLiteral "struct" "union" "class" 
// LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" "operator" TILDE 
// "typeid" STAR AMPERSAND PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_119(_tokenSet_119_data_,12);
const unsigned long CPPParser::_tokenSet_120_data_[] = { 4294195168UL, 2147483647UL, 3758096828UL, 4294967295UL, 134250495UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" 
// ASSIGNEQUAL OCTALINT "friend" StringLiteral "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" LSQUARE 
// STAR AMPERSAND TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO 
// GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR 
// POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" SCOPE "dynamic_cast" 
// "static_cast" "reinterpret_cast" "const_cast" DOT POINTERTO "this" DECIMALINT 
// HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO 
// "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_120(_tokenSet_120_data_,12);
const unsigned long CPPParser::_tokenSet_121_data_[] = { 536871424UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SEMICOLON RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_121(_tokenSet_121_data_,8);
const unsigned long CPPParser::_tokenSet_122_data_[] = { 3757309840UL, 3221225471UL, 234880415UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "public" "protected" 
// "private" "operator" TILDE "typeid" STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "__finally" "try" PLUS MINUS PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_122(_tokenSet_122_data_,12);
const unsigned long CPPParser::_tokenSet_123_data_[] = { 3757309840UL, 3221225471UL, 436207007UL, 4278976512UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "typedef" "enum" SEMICOLON ID LCURLY "inline" "extern" "namespace" 
// "antlrTrace_on" "antlrTrace_off" RCURLY OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "public" "protected" 
// "private" "operator" TILDE "typeid" STAR AMPERSAND "throw" "using" "case" 
// "default" "if" "else" "switch" "while" "do" "for" "goto" "continue" 
// "break" "return" "__try" "try" "catch" PLUS MINUS PLUSPLUS MINUSMINUS 
// "sizeof" "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" 
// "_near" "__near" "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" 
// "_stdcall" "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_123(_tokenSet_123_data_,12);
const unsigned long CPPParser::_tokenSet_124_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 127UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_124(_tokenSet_124_data_,8);
const unsigned long CPPParser::_tokenSet_125_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 255UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_125(_tokenSet_125_data_,8);
const unsigned long CPPParser::_tokenSet_126_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 511UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_126(_tokenSet_126_data_,8);
const unsigned long CPPParser::_tokenSet_127_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 1023UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_127(_tokenSet_127_data_,8);
const unsigned long CPPParser::_tokenSet_128_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 2047UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_128(_tokenSet_128_data_,8);
const unsigned long CPPParser::_tokenSet_129_data_[] = { 537788992UL, 1073741824UL, 3758096960UL, 4095UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR BITWISEXOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_129(_tokenSet_129_data_,8);
const unsigned long CPPParser::_tokenSet_130_data_[] = { 537788992UL, 1073741824UL, 3758097216UL, 4095UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_130(_tokenSet_130_data_,8);
const unsigned long CPPParser::_tokenSet_131_data_[] = { 96UL, 0UL, 0UL, 49152UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN LESSTHANOREQUALTO GREATERTHANOREQUALTO 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_131(_tokenSet_131_data_,8);
const unsigned long CPPParser::_tokenSet_132_data_[] = { 537788992UL, 1073741824UL, 3758097216UL, 16383UL, 0UL, 0UL, 0UL, 0UL };
// GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA RSQUARE 
// AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL 
// SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL 
// QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_132(_tokenSet_132_data_,8);
const unsigned long CPPParser::_tokenSet_133_data_[] = { 0UL, 0UL, 128UL, 3145728UL, 0UL, 0UL, 0UL, 0UL };
// STAR DIVIDE MOD 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_133(_tokenSet_133_data_,8);
const unsigned long CPPParser::_tokenSet_134_data_[] = { 537789024UL, 1073741824UL, 3758097216UL, 1048575UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA 
// RSQUARE AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_134(_tokenSet_134_data_,8);
const unsigned long CPPParser::_tokenSet_135_data_[] = { 537789024UL, 1073741824UL, 3758097344UL, 4194303UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA 
// RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_135(_tokenSet_135_data_,8);
const unsigned long CPPParser::_tokenSet_136_data_[] = { 537789024UL, 1073741824UL, 3758097344UL, 16777215UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON RPAREN COMMA 
// RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_136(_tokenSet_136_data_,8);
const unsigned long CPPParser::_tokenSet_137_data_[] = { 332399872UL, 268435392UL, 28UL, 4026531840UL, 8185UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "enum" ID OCTALINT StringLiteral "struct" "union" "class" LPAREN "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "operator" TILDE "typeid" SCOPE "dynamic_cast" 
// "static_cast" "reinterpret_cast" "const_cast" "this" DECIMALINT HEXADECIMALINT 
// CharLiteral WCharLiteral WStringLiteral FLOATONE FLOATTWO "true" "false" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_137(_tokenSet_137_data_,12);
const unsigned long CPPParser::_tokenSet_138_data_[] = { 870188896UL, 1342177216UL, 3758097404UL, 4294967295UL, 134250495UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID RCURLY ASSIGNEQUAL COLON OCTALINT 
// StringLiteral "struct" "union" "class" LPAREN RPAREN "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" COMMA "operator" TILDE "typeid" LSQUARE RSQUARE 
// STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL 
// MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" DOT 
// POINTERTO "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_138(_tokenSet_138_data_,12);
const unsigned long CPPParser::_tokenSet_139_data_[] = { 0UL, 0UL, 392UL, 786432UL, 8192UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// TILDE STAR AMPERSAND PLUS MINUS NOT 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_139(_tokenSet_139_data_,12);
const unsigned long CPPParser::_tokenSet_140_data_[] = { 0UL, 0UL, 0UL, 268435456UL, 134234112UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SCOPE "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_140(_tokenSet_140_data_,12);
const unsigned long CPPParser::_tokenSet_141_data_[] = { 3757061504UL, 1073741823UL, 444UL, 4278976512UL, 134250489UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" OCTALINT "friend" StringLiteral 
// "struct" "union" "class" "_declspec" "__declspec" LPAREN "auto" "register" 
// "static" "mutable" "_inline" "__inline" "virtual" "explicit" "typename" 
// "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" 
// "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" 
// "unsigned" "float" "double" "void" "const" "volatile" "operator" TILDE 
// "typeid" LSQUARE STAR AMPERSAND PLUS MINUS PLUSPLUS MINUSMINUS "sizeof" 
// "__alignof__" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" 
// "const_cast" "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral 
// WStringLiteral FLOATONE FLOATTWO "true" "false" NOT "new" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_141(_tokenSet_141_data_,12);
const unsigned long CPPParser::_tokenSet_142_data_[] = { 268436512UL, 268435328UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN ID LPAREN "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_142(_tokenSet_142_data_,8);
const unsigned long CPPParser::_tokenSet_143_data_[] = { 273679360UL, 0UL, 28UL, 268435456UL, 8184UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID OCTALINT StringLiteral LPAREN "operator" TILDE "typeid" SCOPE "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_143(_tokenSet_143_data_,12);
const unsigned long CPPParser::_tokenSet_144_data_[] = { 3751818624UL, 1073741823UL, 0UL, 268435456UL, 0UL, 0UL, 0UL, 0UL };
// "typedef" "enum" ID "inline" "extern" "friend" "struct" "union" "class" 
// "_declspec" "__declspec" LPAREN "auto" "register" "static" "mutable" 
// "_inline" "__inline" "virtual" "explicit" "typename" "char" "wchar_t" 
// "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" 
// "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" 
// "double" "void" "const" "volatile" SCOPE 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_144(_tokenSet_144_data_,8);
const unsigned long CPPParser::_tokenSet_145_data_[] = { 4289607648UL, 2147483647UL, 3758097376UL, 285212671UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "typedef" "enum" SEMICOLON ID "inline" "extern" 
// RCURLY ASSIGNEQUAL COLON "friend" "struct" "union" "class" "_declspec" 
// "__declspec" LPAREN RPAREN "auto" "register" "static" "mutable" "_inline" 
// "__inline" "virtual" "explicit" "typename" "char" "wchar_t" "bool" "short" 
// "int" "_int8" "__int8" "_int16" "__int16" "_int32" "__int32" "_int64" 
// "__int64" "_w64" "__w64" "long" "signed" "unsigned" "float" "double" 
// "void" "const" "volatile" COMMA LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS 
// TIMESEQUAL DIVIDEEQUAL MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL 
// SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK 
// OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO 
// SHIFTLEFT SHIFTRIGHT PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR SCOPE 
// "_cdecl" "__cdecl" "_near" "__near" "_far" "__far" "__interrupt" "pascal" 
// "_pascal" "__pascal" "_stdcall" "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_145(_tokenSet_145_data_,12);
const unsigned long CPPParser::_tokenSet_146_data_[] = { 332399888UL, 268435392UL, 28UL, 4026531840UL, 8185UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "template" "enum" ID OCTALINT StringLiteral "struct" "union" "class" 
// LPAREN "typename" "char" "wchar_t" "bool" "short" "int" "_int8" "__int8" 
// "_int16" "__int16" "_int32" "__int32" "_int64" "__int64" "_w64" "__w64" 
// "long" "signed" "unsigned" "float" "double" "void" "operator" TILDE 
// "typeid" SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" 
// "this" DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral 
// FLOATONE FLOATTWO "true" "false" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_146(_tokenSet_146_data_,12);
const unsigned long CPPParser::_tokenSet_147_data_[] = { 806224480UL, 1073741824UL, 3758097376UL, 67108863UL, 6UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON LPAREN RPAREN 
// COMMA LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL 
// MINUSEQUAL PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL 
// BITWISEXOREQUAL BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR 
// NOTEQUAL EQUAL LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT 
// PLUS MINUS DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS DOT POINTERTO 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_147(_tokenSet_147_data_,12);
const unsigned long CPPParser::_tokenSet_148_data_[] = { 1024UL, 0UL, 416UL, 268435456UL, 134184960UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID LSQUARE STAR AMPERSAND SCOPE "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_148(_tokenSet_148_data_,12);
const unsigned long CPPParser::_tokenSet_149_data_[] = { 870188896UL, 2147483584UL, 3758097404UL, 4294967295UL, 268435449UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN "enum" SEMICOLON ID RCURLY ASSIGNEQUAL COLON OCTALINT 
// StringLiteral "struct" "union" "class" LPAREN RPAREN "typename" "char" 
// "wchar_t" "bool" "short" "int" "_int8" "__int8" "_int16" "__int16" "_int32" 
// "__int32" "_int64" "__int64" "_w64" "__w64" "long" "signed" "unsigned" 
// "float" "double" "void" "const" "volatile" COMMA "operator" TILDE "typeid" 
// LSQUARE RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR PLUSPLUS MINUSMINUS "sizeof" "__alignof__" 
// SCOPE "dynamic_cast" "static_cast" "reinterpret_cast" "const_cast" "this" 
// DECIMALINT HEXADECIMALINT CharLiteral WCharLiteral WStringLiteral FLOATONE 
// FLOATTWO "true" "false" NOT "new" "_cdecl" "__cdecl" "_near" "__near" 
// "_far" "__far" "__interrupt" "pascal" "_pascal" "__pascal" "_stdcall" 
// "__stdcall" "delete" 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_149(_tokenSet_149_data_,12);
const unsigned long CPPParser::_tokenSet_150_data_[] = { 806224480UL, 1073741824UL, 3758097344UL, 16777215UL, 0UL, 0UL, 0UL, 0UL };
// LESSTHAN GREATERTHAN SEMICOLON RCURLY ASSIGNEQUAL COLON LPAREN RPAREN 
// COMMA RSQUARE STAR AMPERSAND ELLIPSIS TIMESEQUAL DIVIDEEQUAL MINUSEQUAL 
// PLUSEQUAL MODEQUAL SHIFTLEFTEQUAL SHIFTRIGHTEQUAL BITWISEANDEQUAL BITWISEXOREQUAL 
// BITWISEOREQUAL QUESTIONMARK OR AND BITWISEOR BITWISEXOR NOTEQUAL EQUAL 
// LESSTHANOREQUALTO GREATERTHANOREQUALTO SHIFTLEFT SHIFTRIGHT PLUS MINUS 
// DIVIDE MOD DOTMBR POINTERTOMBR 
const ANTLR_USE_NAMESPACE(antlr)BitSet CPPParser::_tokenSet_150(_tokenSet_150_data_,8);


