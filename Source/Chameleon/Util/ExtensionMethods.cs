using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.Parsing;

namespace Chameleon.Util
{
	public static class ExtensionMethods
	{
		public static string ASTToString(this ASTNode node)
		{
			StringBuilder sb = new StringBuilder();

			ASTToString(node, sb, 0);

			return sb.ToString();
		}

		public static void ASTToString(this ASTNode node, StringBuilder sb, int indent)
		{			
			if (node != null) 
			{
				sb.Append(node.lineNumber + " ");

				string indentText = new string(' ', indent);
				
				sb.AppendFormat("{0}{1} (type: {2})\r\n", indentText, node.text, node.type);

				if (node.firstChild != null) 
				{
					ASTToString(node.firstChild, sb, indent + 2);
				}
				if (node.nextSibling != null) 
				{
					ASTToString(node.nextSibling, sb, indent);
				}
			}
		}

		public static ASTNode GetAncestor(this ASTNode node, int numLevels)
		{
			ASTNode current = node;

			if(numLevels <= 0)
			{
				return current;
			}

			for(int i = 0; i < numLevels; i++)
			{
				ASTNode parent = current.parent;

				if(parent == null)
				{
					return null;
				}

				current = parent;
			}

			return current;
		}

		public static void TraverseAST(this ASTNode node, bool includeSiblings, Func<ASTNode, bool> func)
		{
			if(node != null)
			{
				bool result = func(node);

				if(!result)
				{
					return;
				}

				if(node.firstChild != null)
				{
					TraverseAST(node.firstChild, true, func);
				}
				if(includeSiblings && node.nextSibling != null)
				{
					TraverseAST(node.nextSibling, true, func);
				}
			}
		}
	}
}
