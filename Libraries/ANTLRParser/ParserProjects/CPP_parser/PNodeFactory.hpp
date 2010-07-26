#ifndef PNode_Factory_hpp__
#define PNode_Factory_hpp__

#include "antlr/ASTFactory.hpp"
#include "PNode.hpp"

ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)

class _PNodeFactory : public ASTFactory 
{
public:
	typedef RefAST (*factory_type)();

	// remark reference to Pnode...
	_PNodeFactory() 
		: ASTFactory("PNode", &PNode::factory), nodeFactory(&PNode::factory)
	{
	}

	// add your own functionality...
	RefPNode _PNodeFactory::create(RefPNode tr)
	{
		if (!tr)
			return nullAST;

		RefPNode t = _PNodeFactory::nodeFactory();
		t->initialize(tr);
		return t;
	}

	RefPNode _PNodeFactory::dup(RefPNode t)
	{
		return _PNodeFactory::create(t);
	}

private:
	factory_type nodeFactory;
};


#endif
