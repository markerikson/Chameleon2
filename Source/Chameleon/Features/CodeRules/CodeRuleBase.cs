using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using Chameleon.GUI;
using DevInstinct.Patterns;
using System.Text.RegularExpressions;

namespace Chameleon.Features.CodeRules
{
	public class CodeRuleError
	{
		public string text;
		public Range range;
	}

	public class CodeRuleBase
	{
		protected List<CodeRuleError> m_errors;
		protected string m_name;
		protected bool m_isGlobal;
		protected bool m_checkSucceeded;

		public bool CheckSucceeded
		{
			get { return m_checkSucceeded; }
		}
		public bool IsGlobal
		{
			get { return m_isGlobal; }
		}
		public List<CodeRuleError> Errors
		{
			get { return m_errors; }
		}


		public CodeRuleBase()
		{
			m_errors = new List<CodeRuleError>();
			m_isGlobal = false;
		}

		public virtual bool ExamineSource(ChameleonEditor ed, Range searchRange) { return true; }

		public void ClearErrors()
		{
			m_errors.Clear();
		}

		protected void AddError(ChameleonEditor ed, int lineNum, string errorMessage)
		{
			Line l = ed.Lines[lineNum];
			int pos = l.StartPosition;

			Regex reNotWhitespace = new Regex("[^\\s]");
			Range startRange = ed.FindReplace.Find(l.Range, reNotWhitespace);

			int startPos = l.StartPosition;
			int endPos = l.EndPosition;

			if(startRange != null)
			{
				startPos = startRange.Start;
			}

			CodeRuleError error = new CodeRuleError();
			error.range = new Range(startPos, endPos, ed);
			error.text = errorMessage;

			ed.AddError(error);
		}
	}

	
}
