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
	public class CodeRuleBase
	{
		protected List<CodeRuleError> m_errors;
		protected string m_name;

		public List<CodeRuleError> Errors
		{
			get { return m_errors; }
		}
		

		public CodeRuleBase()
		{
			m_errors = new List<CodeRuleError>();
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

	public class CodeRuleError
	{
		public string text;
		public Range range;
	}

	public class CodeRuleManager
	{
		private List<CodeRuleBase> m_rules;

		public List<CodeRuleBase> Rules
		{
			get { return m_rules; }
		}

		private CodeRuleManager()
		{
			m_rules = new List<CodeRuleBase>();
		}

		public void AddRule(CodeRuleBase rule)
		{
			m_rules.Add(rule);
		}

		public void AddRules()
		{
			AddRule(Singleton<NoAssignmentsInConditionsRule>.Instance);
		}

		public virtual bool ExamineSource(ChameleonEditor ed, Range searchRange)
		{
			bool overallResult = true;

			foreach(CodeRuleBase rule in m_rules)
			{
				bool ruleResult = rule.ExamineSource(ed, searchRange);
				overallResult &= overallResult;
			}

			return overallResult;
		}

		public void ClearErrors()
		{
			foreach(CodeRuleBase rule in m_rules)
			{
				rule.ClearErrors();
			}
		}
	}
}
