using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Granados
{
	public class LsEntry
	{
		private String filename;
		private String longname;
		private SftpATTRS attrs;
		internal LsEntry(String filename, String longname, SftpATTRS attrs)
		{
			setFilename(filename);
			setLongname(longname);
			setAttrs(attrs);
		}
		public String getFilename() { return filename; }
		void setFilename(String filename) { this.filename = filename; }
		public String getLongname() { return longname; }
		void setLongname(String longname) { this.longname = longname; }
		public SftpATTRS getAttrs() { return attrs; }
		void setAttrs(SftpATTRS attrs) { this.attrs = attrs; }
		public override string ToString() { return toString(); }
		public String toString() { return longname; }
	}
}
