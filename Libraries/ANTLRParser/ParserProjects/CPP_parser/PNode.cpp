#include "PNode.hpp"
#include <fstream>

//////////////////////////////////////////////////////////////////////////
//PNode
//////////////////////////////////////////////////////////////////////////
PNode::PNode(void)
{
	lineNumber = 0;
}

PNode::PNode(RefToken t) 
{
	CommonAST::setType(t->getType());
	CommonAST::setText(t->getText());

	setLine(t->getLine());
	setFilename(t->getFilename().c_str());
	setColumn(t->getColumn());
	
}

void PNode::initialize(int t, const std::string& txt)
{
	CommonAST::setType(t);
	CommonAST::setText(txt);

	lineNumber = 0; // to be noticed !
	column = 0;
}

void PNode::initialize(RefPNode t)
{
	CommonAST::setType(t->getType() );
	CommonAST::setText(t->getText() );

	PNode::setLine(t->getLine());
	PNode::setFilename(t->getFilename().c_str());
	PNode::setColumn(t->getColumn());
}

void PNode::initialize(RefAST t)
{
	CommonAST::initialize(t);
}

void PNode::initialize(RefToken t)
{
	CommonAST::initialize(t);

	PNode::setLine(t->getLine() );
	setFilename(t->getFilename().c_str());
	PNode::setColumn(t->getColumn());
}

void PNode::setText(const std::string& txt)
{
	CommonAST::setText(txt);
}

void PNode::setType(int type)
{
	CommonAST::setType(type);
}

void PNode::addChild( RefPNode c )
{
	BaseAST::addChild( static_cast<RefAST>(c) );
}

RefAST PNode::factory(void)
{
	antlr::RefAST ret = static_cast<RefAST>(RefPNode(new PNode));
	return ret;
}

RefPNode PNode::getFirstChild()
{
	return static_cast<RefAST>(BaseAST::getFirstChild());
}

RefPNode PNode::getNextSibling()
{
	return static_cast<RefAST>(BaseAST::getNextSibling());
}

// my extensions
void PNode::setLine(int l_)
{
	lineNumber = l_;
}

void PNode::setColumn(int col)
{
	column = col;
}

void PNode::setFilename(const char* fn)
{
	filename = fn;
}

// ok cause imaginary tokens always has children...
int PNode::getLine(void) const
{
	if(lineNumber != 0)
		return lineNumber;
	else 
	{
		if(static_cast<RefPNode>(BaseAST::getFirstChild()) == NULL)
		{
			return lineNumber;
		}
		else
		{
			return ( (static_cast<RefPNode>(BaseAST::getFirstChild()))->getLine() );
		}
	}
}

int PNode::getColumn(void) const
{
	return column;
	/*
	if(column != 0)
		return column;
	else 
	{
		if(static_cast<RefPNode>(BaseAST::getFirstChild()) == NULL)
		{
			return lineNumber;
		}
		else
		{
			return ( (static_cast<RefPNode>(BaseAST::getFirstChild()))->getLine() );
		}
	}
	*/
}

string PNode::getFilename() const
{
	if(!filename.empty())
		return filename;
	else 
	{
		if(static_cast<RefPNode>(BaseAST::getFirstChild()) == NULL)
		{
			return filename;
		}
		else
		{
			return ( (static_cast<RefPNode>(BaseAST::getFirstChild()))->getFilename());
		}
	}
}

void PNode::xmlSerialize(ofstream& fout)
{
 	fout << "<" << text.c_str() << ">" << std::endl;
 	RefPNode pIt = getFirstChild();
 	while(pIt != NULL)
 	{
 		pIt->xmlSerialize(fout);
 		pIt = pIt->getNextSibling();
 	}
 	fout << "</" << text.c_str() << ">" << std::endl;
}

void CNamespaceNode::xmlSerialize(ofstream& fout)
{
	fout << "<" << text.c_str();
	RefPNode pIt = getFirstChild();
	if(pIt != NULL && !pIt->IsVirtual())
	{
		fout << " Name=\"" << pIt->getText() << "\" ";
	}
	fout << ">" << std::endl;
	while(pIt != NULL)
	{
		pIt = pIt->getNextSibling();
		if(pIt)
			pIt->xmlSerialize(fout);
	}
	fout << "</" << text.c_str() << ">" << std::endl;
}
