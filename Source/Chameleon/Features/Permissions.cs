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
		//NoFeature				= 0,
		Feature1				= 1 << 0,
		Feature2				= 1 << 1,
		DragDropSnippets		= 1 << 2,
	}

	public class Permissions
	{
	}
}
