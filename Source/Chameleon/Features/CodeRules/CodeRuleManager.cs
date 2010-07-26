﻿using System;
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
		protected bool m_isGlobal;

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

	public class CodeRuleError
	{
		public string text;
		public Range range;
	}

	public class CodeRuleManager
	{
		private List<CodeRuleBase> m_localRules;
		private List<CodeRuleBase> m_globalRules;

		public List<CodeRuleBase> LocalRules
		{
			get { return m_localRules; }
		}

		public List<CodeRuleBase> GlobalRules
		{
			get { return m_globalRules; }
		}

		private CodeRuleManager()
		{
			m_localRules = new List<CodeRuleBase>();
			m_globalRules = new List<CodeRuleBase>();
		}

		public void AddRule(CodeRuleBase rule)
		{
			if(rule.IsGlobal)
			{
				m_globalRules.Add(rule);
			}
			else
			{
				m_localRules.Add(rule);
			}
			
		}

		public void AddRules()
		{
			AddRule(Singleton<NoGlobalVariablesRule>.Instance);
			AddRule(Singleton<NoAssignmentsInConditionsRule>.Instance);
			AddRule(Singleton<SingleCharVarsOnlyInLoopsRule>.Instance);
		}

		public virtual bool ExamineSource(ChameleonEditor ed, Range searchRange, bool global)
		{
			bool overallResult = true;

			List<CodeRuleBase> rules = m_localRules;

			if(global)
			{
				rules = m_globalRules;
			}

			foreach(CodeRuleBase rule in rules)
			{
				bool ruleResult = rule.ExamineSource(ed, searchRange);
				overallResult &= overallResult;
			}

			return overallResult;
		}

		public void ClearErrors()
		{
			foreach(CodeRuleBase rule in m_localRules)
			{
				rule.ClearErrors();
			}

			foreach(CodeRuleBase rule in m_globalRules)
			{
				rule.ClearErrors();
			}
		}
	}
}
