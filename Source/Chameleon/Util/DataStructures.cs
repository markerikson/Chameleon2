using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Chameleon.Util
{
	public enum ZoomLevel
	{
		Normal,
		Large,
		Larger,
		Largest
	}

	public enum FileLocation
	{
		Local,
		Remote,
		Unknown
	}

	public class DirectoryListing
	{
		public List<string> dirNames;
		public List<string> fileNames;
		public FilePath directoryName;

		public DirectoryListing()
		{
			dirNames = new List<string>();
			fileNames = new List<string>();
		}
	}

	public class FileInformation
	{
		private FileLocation m_location;
		private FilePath m_filename;

		public FilePath Filename
		{
			get { return m_filename; }
			set
			{
				m_filename = value;
			}
		}
		public Chameleon.Util.FileLocation Location
		{
			get { return m_location; }
			set
			{ m_location = value; }
		}

		public FileInformation()
		{
			m_location = FileLocation.Unknown;
			m_filename = new FilePath();
		}
	}
}


namespace CodeLite
{
	public enum LexerToken
	{
		None = 0,

		AUTO = 257,
		lexDOUBLE = 258,
		lexINT = 259,
		lexSTRUCT = 260,
		BREAK = 261,
		ELSE = 262,
		lexLONG = 263,
		SWITCH = 264,
		CASE = 265,
		lexENUM = 266,
		REGISTER = 267,
		TYPEDEF = 268,
		lexCHAR = 269,
		EXTERN = 270,
		RETURN = 271,
		UNION = 272,
		lexCONST = 273,
		lexFLOAT = 274,
		lexSHORT = 275,
		UNSIGNED = 276,
		CONTINUE = 277,
		FOR = 278,
		SIGNED = 279,
		lexVOID = 280,
		lexDEFAULT = 281,
		GOTO = 282,
		SIZEOF = 283,
		VOLATILE = 284,
		DO = 285,
		IF = 286,
		STATIC = 287,
		WHILE = 288,
		NEW = 289,
		lexDELETE = 290,
		lexTHIS = 291,
		lexOPERATOR = 292,
		lexCLASS = 293,
		lexNAMESPACE = 294,
		lexPUBLIC = 295,
		lexPROTECTED = 296,
		lexPRIVATE = 297,
		VIRTUAL = 298,
		FRIEND = 299,
		INLINE = 300,
		OVERLOAD = 301,
		IDENTIFIER = 302,
		STRINGliteral = 303,
		FLOATINGconstant = 304,
		INTEGERconstant = 305,
		CHARACTERconstant = 306,
		OCTALconstant = 307,
		HEXconstant = 308,
		TYPEDEFname = 309,
		lexARROW = 310,
		ICR = 311,
		DECR = 312,
		LS = 313,
		RS = 314,
		LE = 315,
		GE = 316,
		EQ = 317,
		NE = 318,
		ANDAND = 319,
		OROR = 320,
		ELLIPSIS = 321,
		CLCL = 322,
		DOTstar = 323,
		ARROWstar = 324,
		MULTassign = 325,
		DIVassign = 326,
		MODassign = 327,
		PLUSassign = 328,
		MINUSassign = 329,
		LSassign = 330,
		RSassign = 331,
		ANDassign = 332,
		ERassign = 333,
		ORassign = 334,
		CComment = 335,
		CPPComment = 336,
		POUNDPOUND = 337,
	};
}