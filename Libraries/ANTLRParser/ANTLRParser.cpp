#include <vector>

#include <vcclr.h>
#include <msclr\marshal.h>

#using <System.dll>
#using <System.XML.dll>
#using <mscorlib.dll>

#include <PNode.hpp>
#include <CPPLexer.hpp>
#include <CPPParser.hpp>
#include <antlr/CommonAST.hpp>
#include <antlr/CharInputBuffer.hpp>

#include "Utilities.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace System::Timers;
using namespace System::Threading;
using namespace System::Xml;


using namespace std;


namespace Chameleon
{
namespace Parsing
{

	public ref class ASTNode
	{
	public:
		int lineNumber;
		int column;
		String^ filename;

		int type;
		String^ text;
		

		ASTNode^ firstChild;
		ASTNode^ parent;
		ASTNode^ nextSibling;
		ASTNode^ prevSibling;


		List<ASTNode^>^ GetChildren()
		{
			List<ASTNode^>^ children = gcnew List<ASTNode^>();

			ASTNode^ current = firstChild;

			while(current != nullptr)
			{
				children->Add(current);
				current = current->nextSibling;
			}

			return children;
		}

		List<ASTNode^>^ GetSiblings()
		{
			List<ASTNode^>^ siblings = gcnew List<ASTNode^>();

			ASTNode^ current = nextSibling;

			while(current != nullptr)
			{
				siblings->Add(current);
				current = current->nextSibling;
			}

			return siblings;
		}
		
	};


	public ref class ANTLRParser
	{
	private:
		ANTLRParser()
		{
		}
		~ANTLRParser()
		{
			Destroy();
		}

	public:
		void SetSource(String^ textToParse, String^ filename)
		{
			Destroy();

			m_text = new string;
			*m_text = ConvertString(textToParse);

			m_filename = filename;

			m_cib = new CharInputBuffer((unsigned char*)m_text->c_str(), m_text->size(), false);
			m_lexer = new CPPLexer(*m_cib);
			m_lexer->setTabsize(4);
			m_parser = new CPPParser(*m_lexer);
			m_nodeFactory = new _PNodeFactory();
		}

		property bool ParseCompleted
		{
			bool get()
			{			
				return m_parseCompleted;
			}
		}

		bool Parse()
		{
			m_parseCompleted = false;

			if(!m_text)
			{
				return false;
			}

			try
			{
				m_parser->initializeASTFactory(*m_nodeFactory);
				m_parser->setASTFactory(m_nodeFactory);

				m_parser->setFilename(ConvertString(m_filename));
				m_parser->init();
				m_parser->translation_unit();

				RefPNode ast = RefPNode(m_parser->getAST());
				m_astRoot = gcnew ASTNode();
				m_astRoot->firstChild = gcnew ASTNode();
				ConvertAST(ast, m_astRoot->firstChild);
			}
			catch(ANTLRException& e)
			{
				return false;
			}
			catch (exception& e) 
			{
				return false;
			}
			

			m_parseCompleted = true;

			return true;
		}

		ASTNode^ GetAST()
		{
			if(!m_parseCompleted)
			{
				return nullptr;
			}

			return m_astRoot;
		}


	protected:
		!ANTLRParser() 
		{
			Destroy();
		}

	private:
		String^ m_filename;
		CPPLexer* m_lexer;
		CPPParser* m_parser;
		string* m_text;
		CharInputBuffer* m_cib;
		_PNodeFactory* m_nodeFactory;
		bool m_parseCompleted;
		ASTNode^ m_astRoot;

		void Destroy()
		{
			m_parseCompleted = false;

			if(m_lexer)
			{
				delete m_lexer;
				m_lexer = NULL;
			}

			if(m_parser)
			{
				delete m_parser;
				m_parser = NULL;
			}

			if(m_text)
			{
				delete m_text;
				m_text = NULL;
			}

			if(m_nodeFactory)
			{
				delete m_nodeFactory;
				m_nodeFactory = NULL;
			}

			m_astRoot = nullptr;
		}

		void ConvertAST(RefPNode top, ASTNode^ node)
		{
			if (top != NULL) 
			{
				node->text = ConvertString(top->getText());
				node->type = top->getType();
				node->filename = ConvertString(top->getFilename());
				node->lineNumber = top->getLine();
				node->column = top->getColumn();

				if (top->getFirstChild() != NULL) 
				{
					node->firstChild = gcnew ASTNode();
					node->firstChild->parent = node;
					ConvertAST( top->getFirstChild(),  node->firstChild);
				}
				if (top->getNextSibling()) 
				{
					node->nextSibling = gcnew ASTNode();
					node->nextSibling->prevSibling = node;
					node->nextSibling->parent = node->parent;
					ConvertAST( top->getNextSibling(), node->nextSibling);
				}
			}
		}
	};


}

}