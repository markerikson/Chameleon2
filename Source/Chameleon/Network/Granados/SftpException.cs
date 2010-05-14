using System;
using System.Collections.Generic;
using System.Text;

namespace Granados
{
	class SftpException : Exception
	{
		public int id;
		public String message;

		public SftpException(SSH_FX id, string message)
		{
			this.id = (int)id;
			this.message = message;
		}

		public SftpException(int id, String message)
			: base()
		{
			this.id = id;
			this.message = message;
		}

		public override String ToString()
		{
			return message;
		}
	}
}
