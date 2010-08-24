using System;
using System.Text;
using Chameleon.Parsing;
using System.Windows.Forms;

namespace Chameleon.Extensions.ANTLRParser
{
	public static class ASTNodeExtensions
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
				sb.AppendFormat("{0}:{1} ", node.lineNumber, node.column);

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

		public static ASTNode GetBlock(this ASTNode node)
		{
			string[] blockKeywords = new string[] { "if" };//, "for", "while", "case" };

			ASTNode blockNode = null;

			switch(node.text)
			{
				case "if":
				case "for":
				case "while":
				case "case":
				{
					blockNode = node;
					break;
				}
				case "switch":
				{
					blockNode = node.nextSibling;
					break;
				}
			}

			return blockNode;
		}
	}

}

namespace Chameleon.Extensions.Windows.Forms
{
	public static class ControlExtensions
	{
		public static void Invoke(this Control control, Action action)
		{
			control.Invoke((Delegate)action);
		}


	}

}