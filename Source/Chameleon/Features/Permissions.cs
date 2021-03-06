﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon;

namespace Chameleon.Features
{
	[Flags]
	public enum ChameleonFeatures
	{
		None					= 0,
		DragDropSnippets		= 1 << 1,
		Compiler				= 1 << 2,
		Debugger				= 1 << 3,
		CodeRules				= 1 << 4,
		AutoReformat			= 1 << 5,
		FileTemplates			= 1 << 6,
		SimpleRunProgram		= 1 << 7,
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
