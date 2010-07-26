using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Chameleon.Parsing
{	
	class ASTNodeTreeAdapter : ILinqTree<ASTNode>
	{
		private ASTNode m_node;

		public ASTNodeTreeAdapter(ASTNode node)
        {
			m_node = node;                   
        }

		public IEnumerable<ASTNode> Children()
		{
			List<ASTNode> children = m_node.GetChildren();

			foreach(ASTNode node in children)
			{
				yield return node;
			}
		}

		public ASTNode Parent
		{
			get
			{
				return m_node.parent;
			}
		}

		public ASTNode Item
		{
			get
			{
				return m_node;
			}			
		}
	}
	
}
