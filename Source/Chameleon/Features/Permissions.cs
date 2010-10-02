using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon;

namespace Chameleon.Features
{
	[Flags]
	public enum ChameleonFeatures
	{
		DragDropSnippets		= 1 << 1,
		Compiler				= 1 << 2,
		Debugger				= 1 << 3,
		CodeRules				= 1 << 4,
	}

	public class Permissions
	{
		public static ChameleonFeatures ParsePermissions(string text)
		{
			string[] items = text.Split('|');
			ChameleonFeatures cf = (ChameleonFeatures)0;

			foreach(string item in items)
			{
				ChameleonFeatures flag;
				if(Enum.TryParse<ChameleonFeatures>(item, out flag))
				{
					cf |= flag;
				}
			}

			return cf;
		}
	}
}
