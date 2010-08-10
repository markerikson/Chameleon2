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
			AddRule(Singleton<BlocksMustHaveBracesRule>.Instance);
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
