using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DevInstinct.Patterns;

using Chameleon.GUI;
using Chameleon.Network;
using Chameleon.Features;
using LVE = ListViewEx;
using Aspiring.Linq;

namespace ProfManager
{
	public partial class Form1 : Form
	{
		private Networking m_networking;

		private Dictionary<string, ChameleonFeatures> m_groups;
		private Dictionary<string, string> m_students;

		private TextBox m_cellText;
		private ComboBox m_cellCombo;

		private ListViewItem m_prevSelectedGroup;

		private string m_prevGroupName;

		private int m_nextGroupNum;
		private int m_nextStudentNum;

		public Form1()
		{
			InitializeComponent();

			m_networking = Networking.Instance;
			Singleton<RemoteFileDialog>.Instance.Networking = m_networking;

			m_students = new Dictionary<string, string>();
			m_groups = new Dictionary<string, ChameleonFeatures>();

			m_cellCombo = new ComboBox();
			m_cellCombo.Parent = this;
			m_cellCombo.Visible = false;
			m_cellCombo.DropDownStyle = ComboBoxStyle.DropDownList;


			m_cellText = new TextBox();
			m_cellText.Parent = this;
			m_cellText.Visible = false;

			toolStripStatusLabel1.Text = "Not connected";

			m_nextGroupNum = 0;

			List<ChameleonFeatures> cf = Enum.GetValues(typeof(ChameleonFeatures)).Cast<ChameleonFeatures>().ToList();

			foreach(ChameleonFeatures feature in cf)
			{
				lbFeatures.Items.Add(feature);
			}

			lvGroups.DoubleClickActivation = true;
			lvStudents.DoubleClickActivation = true;
			
			
		}

		private void AddNewGroup()
		{
			string name = GetNextGroupName();

			while(m_groups.ContainsKey(name))
			{
				name = GetNextGroupName();
			}

			SaveCurrentFeatures();

			m_groups[name] = new ChameleonFeatures();

			ListViewItem lvi = lvGroups.Items.Add(name);
			lvi.Selected = true;
			
		}

		private void SaveCurrentFeatures()
		{
			/*
			ListViewItem lvi = new ListViewItem();
			ChameleonFeatures cf = m_groups[lvi.Text];

			for(int i = 0; i < lbFeatures.Items.Count; i++)
			{

			}
			*/
		}

		private string GetNextGroupName()
		{
			string name = "newGroup" + m_nextGroupNum;
			m_nextGroupNum++;

			return name;
		}

		private void btnAddGroup_Click(object sender, EventArgs e)
		{
			AddNewGroup();
		}

		private void OnSubItemClicked(object sender, ListViewEx.SubItemEventArgs e)
		{
			Control c = null;
			LVE.ListViewEx lv = sender as LVE.ListViewEx;

			if(lv == lvGroups)
			{
				c = m_cellText;

				m_prevGroupName = e.Item.Text;
			}
			else
			{
				if(e.SubItem == 0)
				{
					c = m_cellCombo;
					m_cellCombo.Items.Clear();

					string[] groups = (from lvi in lvGroups.Items.Cast<ListViewItem>()
									   select lvi.Text).ToArray();

					Array.Sort<string>(groups);

					m_cellCombo.Items.AddRange(groups);
				}
				else
				{
					c = m_cellText;
				}				
			}

			lv.StartEditing(c, e.Item, e.SubItem);
			
		}

		private void lvGroups_SubItemEndEditing(object sender, LVE.SubItemEndEditingEventArgs e)
		{
			string newGroupName = e.DisplayText;

			if(m_prevGroupName != newGroupName)
			{
				ReplaceGroupName(m_prevGroupName, newGroupName);

				ChameleonFeatures cf = m_groups[m_prevGroupName];
				m_groups.Remove(m_prevGroupName);
				m_groups[newGroupName] = cf;
			}
		}

		private void ReplaceGroupName(string oldGroupName, string newGroupName)
		{
			int count = (from lvi in lvStudents.Items.Cast<ListViewItem>()
						 where lvi.Text == oldGroupName
						 select lvi).Update(lvi => lvi.Text = newGroupName);
		}

		private void lvGroups_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			Console.WriteLine("Item: {0}, index = {1}, selected = {2}", e.Item.Text, e.ItemIndex, e.IsSelected);
		}

		private void btnRemoveGroup_Click(object sender, EventArgs e)
		{
			if(lvGroups.SelectedItems.Count == 1)
			{
				ListViewItem lvi = lvGroups.SelectedItems[0];
				int prevIndex = lvi.Index;

				lvGroups.Items.Remove(lvi);

				m_groups.Remove(lvi.Text);

				ReplaceGroupName(lvi.Text, "");

				if(lvGroups.Items.Count > 0)
				{
					int nextSelection = 0;

					if(prevIndex < lvGroups.Items.Count)
					{
						nextSelection = prevIndex;
					}
					else
					{
						if(prevIndex > 0)
						{
							nextSelection = prevIndex - 1;
						}
					}

					lvGroups.SelectedIndices.Add(nextSelection);
				}
			}
		}

		private void btnSave_Click(object sender, EventArgs e)
		{
			bool emptyGroups = (from lvi in lvStudents.Items.Cast<ListViewItem>()
								where lvi.Text == ""
								select lvi).Any();

			if(emptyGroups)
			{
				MessageBox.Show("Can't save until all students are part of a group");
			}
		}

		private void btnAddFromFile_Click(object sender, EventArgs e)
		{

		}

	}
}
