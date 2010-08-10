#ifndef PNode_hpp__
#define PNode_hpp__

#include "antlr/CommonAST.hpp"

#define OUTPUT_DEBUG	1 ? 1 : printf
#define OUTPUT_ERROR	OUTPUT_DEBUG

ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)

class PNode;
typedef antlr::ASTRefCount<PNode> RefPNode;

class PNode : public CommonAST 
{
public:
	PNode( void );
	PNode(RefToken t);
	~PNode( void ){}

public:
	virtual void initialize(int t, const std::string& txt);
	virtual void initialize( RefPNode t );
	virtual void initialize( RefAST t );
	virtual void initialize( antlr::RefToken t );
	virtual void setText(const std::string& txt);
	virtual void setType(int type);
	virtual void addChild( RefPNode c );
	virtual RefPNode getFirstChild();
	virtual RefPNode getNextSibling();
	virtual void setLine(int l_);
	virtual void setColumn(int col);
	virtual void setFilename(const char* fn);
	virtual int getLine(void) const;
	virtual int getColumn(void) const;
	virtual string getFilename() const;
	virtual void xmlSerialize(ofstream& fout);

	static RefAST factory(void);
	bool IsVirtual(){return lineNumber == 0;}

protected:
	int		lineNumber;
	int column;
	string  filename;
};

class CClassNode : public PNode
{
public:
	CClassNode(){}
	virtual ~CClassNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CClassNode));
		return ret;
	}
};

class CFunctionNode : public PNode
{
public:
	CFunctionNode(){}
	virtual ~CFunctionNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CFunctionNode));
		return ret;
	}
};

class CNamespaceNode : public PNode
{
public:
	CNamespaceNode(){}
	virtual ~CNamespaceNode(){}

public:
	virtual void xmlSerialize(ofstream& fout);
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CNamespaceNode));
		return ret;
	}
};

class CDeclarationNode : public PNode
{
public:
	CDeclarationNode(){}
	virtual ~CDeclarationNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CDeclarationNode));
		return ret;
	}
};

class CTemplateNode : public PNode
{
public:
	CTemplateNode(){}
	virtual ~CTemplateNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CTemplateNode));
		return ret;
	}
};

class CTypedefNode : public PNode
{
public:
	CTypedefNode(){}
	virtual ~CTypedefNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CTypedefNode));
		return ret;
	}
};

class CTypeNode : public PNode
{
public:
	CTypeNode(){}
	virtual ~CTypeNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CTypeNode));
		return ret;
	}
};

class CExpressionNode : public PNode
{
public:
	CExpressionNode(){}
	virtual ~CExpressionNode(){}

public:
	static RefAST factory(void)
	{
		antlr::RefAST ret = static_cast<RefAST>(RefPNode(new CExpressionNode));
		return ret;
	}
};

#endif
