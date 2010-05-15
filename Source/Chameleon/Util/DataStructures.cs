using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Chameleon.Util
{
	public class DirectoryListing
	{
		public List<string> dirNames;
		public List<string> fileNames;

		public DirectoryListing()
		{
			dirNames = new List<string>();
			fileNames = new List<string>();
		}
	}
}
