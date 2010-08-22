namespace ProfManager
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if(disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem(new string[] {
            "group2",
            "student1"}, -1);
			System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem(new string[] {
            "group2",
            "student2"}, -1);
			System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem(new string[] {
            "group3",
            "student3"}, -1);
			this.btnConnect = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.txtHost = new System.Windows.Forms.TextBox();
			this.txtUser = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.txtPassword = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.btnDisconnect = new System.Windows.Forms.Button();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			this.txtSelectedFolder = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.btnFolderLocal = new System.Windows.Forms.Button();
			this.btnFolderRemote = new System.Windows.Forms.Button();
			this.btnAddStudent = new System.Windows.Forms.Button();
			this.btnRemoveStudent = new System.Windows.Forms.Button();
			this.lbFeatures = new System.Windows.Forms.CheckedListBox();
			this.btnRemoveGroup = new System.Windows.Forms.Button();
			this.btnAddGroup = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.btnSave = new System.Windows.Forms.Button();
			this.label7 = new System.Windows.Forms.Label();
			this.btnAddFromFile = new System.Windows.Forms.Button();
			this.lvGroups = new ListViewEx.ListViewEx();
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.lvStudents = new ListViewEx.ListViewEx();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.statusStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnConnect
			// 
			this.btnConnect.Image = global::ProfManager.Properties.Resources.connect16;
			this.btnConnect.Location = new System.Drawing.Point(541, 3);
			this.btnConnect.Name = "btnConnect";
			this.btnConnect.Size = new System.Drawing.Size(24, 24);
			this.btnConnect.TabIndex = 0;
			this.btnConnect.UseVisualStyleBackColor = true;
			this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.Location = new System.Drawing.Point(12, 7);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(39, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Host:";
			// 
			// txtHost
			// 
			this.txtHost.Location = new System.Drawing.Point(57, 5);
			this.txtHost.Name = "txtHost";
			this.txtHost.Size = new System.Drawing.Size(142, 20);
			this.txtHost.TabIndex = 2;
			// 
			// txtUser
			// 
			this.txtUser.Location = new System.Drawing.Point(251, 5);
			this.txtUser.Name = "txtUser";
			this.txtUser.Size = new System.Drawing.Size(100, 20);
			this.txtUser.TabIndex = 4;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label2.Location = new System.Drawing.Point(205, 7);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(40, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "User:";
			// 
			// txtPassword
			// 
			this.txtPassword.AcceptsReturn = true;
			this.txtPassword.Location = new System.Drawing.Point(435, 5);
			this.txtPassword.Name = "txtPassword";
			this.txtPassword.Size = new System.Drawing.Size(100, 20);
			this.txtPassword.TabIndex = 6;
			this.txtPassword.UseSystemPasswordChar = true;
			this.txtPassword.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPassword_KeyPress);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label3.Location = new System.Drawing.Point(358, 7);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(71, 16);
			this.label3.TabIndex = 5;
			this.label3.Text = "Password:";
			// 
			// btnDisconnect
			// 
			this.btnDisconnect.Enabled = false;
			this.btnDisconnect.Image = global::ProfManager.Properties.Resources.disconnect16;
			this.btnDisconnect.Location = new System.Drawing.Point(571, 3);
			this.btnDisconnect.Name = "btnDisconnect";
			this.btnDisconnect.Size = new System.Drawing.Size(24, 24);
			this.btnDisconnect.TabIndex = 7;
			this.btnDisconnect.UseVisualStyleBackColor = true;
			this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
			this.statusStrip1.Location = new System.Drawing.Point(0, 420);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(624, 22);
			this.statusStrip1.TabIndex = 8;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
			this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
			this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
			// 
			// txtSelectedFolder
			// 
			this.txtSelectedFolder.Location = new System.Drawing.Point(125, 45);
			this.txtSelectedFolder.Name = "txtSelectedFolder";
			this.txtSelectedFolder.ReadOnly = true;
			this.txtSelectedFolder.Size = new System.Drawing.Size(245, 20);
			this.txtSelectedFolder.TabIndex = 9;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label4.Location = new System.Drawing.Point(12, 46);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(107, 16);
			this.label4.TabIndex = 10;
			this.label4.Text = "Selected Folder:";
			// 
			// btnFolderLocal
			// 
			this.btnFolderLocal.Location = new System.Drawing.Point(376, 44);
			this.btnFolderLocal.Name = "btnFolderLocal";
			this.btnFolderLocal.Size = new System.Drawing.Size(60, 20);
			this.btnFolderLocal.TabIndex = 12;
			this.btnFolderLocal.Text = "Local";
			this.btnFolderLocal.UseVisualStyleBackColor = true;
			this.btnFolderLocal.Click += new System.EventHandler(this.btnFolderLocal_Click);
			// 
			// btnFolderRemote
			// 
			this.btnFolderRemote.Enabled = false;
			this.btnFolderRemote.Location = new System.Drawing.Point(442, 44);
			this.btnFolderRemote.Name = "btnFolderRemote";
			this.btnFolderRemote.Size = new System.Drawing.Size(60, 20);
			this.btnFolderRemote.TabIndex = 13;
			this.btnFolderRemote.Text = "Remote";
			this.btnFolderRemote.UseVisualStyleBackColor = true;
			this.btnFolderRemote.Click += new System.EventHandler(this.btnFolderRemote_Click);
			// 
			// btnAddStudent
			// 
			this.btnAddStudent.Location = new System.Drawing.Point(15, 387);
			this.btnAddStudent.Name = "btnAddStudent";
			this.btnAddStudent.Size = new System.Drawing.Size(81, 23);
			this.btnAddStudent.TabIndex = 14;
			this.btnAddStudent.Text = "Add Student";
			this.btnAddStudent.UseVisualStyleBackColor = true;
			this.btnAddStudent.Click += new System.EventHandler(this.btnAddStudent_Click);
			// 
			// btnRemoveStudent
			// 
			this.btnRemoveStudent.Location = new System.Drawing.Point(192, 387);
			this.btnRemoveStudent.Name = "btnRemoveStudent";
			this.btnRemoveStudent.Size = new System.Drawing.Size(97, 23);
			this.btnRemoveStudent.TabIndex = 15;
			this.btnRemoveStudent.Text = "Remove Student";
			this.btnRemoveStudent.UseVisualStyleBackColor = true;
			this.btnRemoveStudent.Click += new System.EventHandler(this.btnRemoveStudent_Click);
			// 
			// lbFeatures
			// 
			this.lbFeatures.CheckOnClick = true;
			this.lbFeatures.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lbFeatures.FormattingEnabled = true;
			this.lbFeatures.Location = new System.Drawing.Point(326, 224);
			this.lbFeatures.Name = "lbFeatures";
			this.lbFeatures.Size = new System.Drawing.Size(286, 157);
			this.lbFeatures.TabIndex = 16;
			this.lbFeatures.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lbFeatures_ItemCheck);
			// 
			// btnRemoveGroup
			// 
			this.btnRemoveGroup.Location = new System.Drawing.Point(435, 387);
			this.btnRemoveGroup.Name = "btnRemoveGroup";
			this.btnRemoveGroup.Size = new System.Drawing.Size(104, 23);
			this.btnRemoveGroup.TabIndex = 19;
			this.btnRemoveGroup.Text = "Remove Group";
			this.btnRemoveGroup.UseVisualStyleBackColor = true;
			this.btnRemoveGroup.Click += new System.EventHandler(this.btnRemoveGroup_Click);
			// 
			// btnAddGroup
			// 
			this.btnAddGroup.Location = new System.Drawing.Point(325, 387);
			this.btnAddGroup.Name = "btnAddGroup";
			this.btnAddGroup.Size = new System.Drawing.Size(104, 23);
			this.btnAddGroup.TabIndex = 18;
			this.btnAddGroup.Text = "Add Group";
			this.btnAddGroup.UseVisualStyleBackColor = true;
			this.btnAddGroup.Click += new System.EventHandler(this.btnAddGroup_Click);
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label5.Location = new System.Drawing.Point(12, 85);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(63, 16);
			this.label5.TabIndex = 20;
			this.label5.Text = "Students:";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label6.Location = new System.Drawing.Point(323, 85);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(55, 16);
			this.label6.TabIndex = 21;
			this.label6.Text = "Groups:";
			// 
			// btnSave
			// 
			this.btnSave.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btnSave.Location = new System.Drawing.Point(535, 42);
			this.btnSave.Name = "btnSave";
			this.btnSave.Size = new System.Drawing.Size(60, 24);
			this.btnSave.TabIndex = 22;
			this.btnSave.Text = "Save";
			this.btnSave.UseVisualStyleBackColor = true;
			this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label7.Location = new System.Drawing.Point(323, 205);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(64, 16);
			this.label7.TabIndex = 23;
			this.label7.Text = "Features:";
			// 
			// btnAddFromFile
			// 
			this.btnAddFromFile.Location = new System.Drawing.Point(102, 387);
			this.btnAddFromFile.Name = "btnAddFromFile";
			this.btnAddFromFile.Size = new System.Drawing.Size(84, 23);
			this.btnAddFromFile.TabIndex = 25;
			this.btnAddFromFile.Text = "Add From File";
			this.btnAddFromFile.UseVisualStyleBackColor = true;
			this.btnAddFromFile.Click += new System.EventHandler(this.btnAddFromFile_Click);
			// 
			// lvGroups
			// 
			this.lvGroups.AllowColumnReorder = true;
			this.lvGroups.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3});
			this.lvGroups.DoubleClickActivation = false;
			this.lvGroups.FullRowSelect = true;
			this.lvGroups.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.lvGroups.HideSelection = false;
			this.lvGroups.Location = new System.Drawing.Point(326, 104);
			this.lvGroups.MultiSelect = false;
			this.lvGroups.Name = "lvGroups";
			this.lvGroups.Size = new System.Drawing.Size(286, 98);
			this.lvGroups.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvGroups.TabIndex = 24;
			this.lvGroups.UseCompatibleStateImageBehavior = false;
			this.lvGroups.View = System.Windows.Forms.View.Details;
			this.lvGroups.SubItemClicked += new ListViewEx.SubItemEventHandler(this.OnSubItemClicked);
			this.lvGroups.SubItemEndEditing += new ListViewEx.SubItemEndEditingEventHandler(this.lvGroups_SubItemEndEditing);
			this.lvGroups.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.lvGroups_ItemSelectionChanged);
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Groups";
			this.columnHeader3.Width = 260;
			// 
			// lvStudents
			// 
			this.lvStudents.AllowColumnReorder = true;
			this.lvStudents.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.lvStudents.DoubleClickActivation = false;
			this.lvStudents.FullRowSelect = true;
			this.lvStudents.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3});
			this.lvStudents.Location = new System.Drawing.Point(15, 104);
			this.lvStudents.Name = "lvStudents";
			this.lvStudents.Size = new System.Drawing.Size(274, 277);
			this.lvStudents.TabIndex = 11;
			this.lvStudents.UseCompatibleStateImageBehavior = false;
			this.lvStudents.View = System.Windows.Forms.View.Details;
			this.lvStudents.SubItemClicked += new ListViewEx.SubItemEventHandler(this.OnSubItemClicked);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Group";
			this.columnHeader1.Width = 123;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Student";
			this.columnHeader2.Width = 116;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.Controls.Add(this.btnAddFromFile);
			this.Controls.Add(this.lvGroups);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.btnSave);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.btnRemoveGroup);
			this.Controls.Add(this.btnAddGroup);
			this.Controls.Add(this.lbFeatures);
			this.Controls.Add(this.btnRemoveStudent);
			this.Controls.Add(this.btnAddStudent);
			this.Controls.Add(this.btnFolderRemote);
			this.Controls.Add(this.btnFolderLocal);
			this.Controls.Add(this.lvStudents);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.txtSelectedFolder);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.btnDisconnect);
			this.Controls.Add(this.txtPassword);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.txtUser);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.txtHost);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.btnConnect);
			this.MinimumSize = new System.Drawing.Size(640, 480);
			this.Name = "Form1";
			this.Text = "Chameleon Professor Management Tool";
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnConnect;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtHost;
		private System.Windows.Forms.TextBox txtUser;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtPassword;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button btnDisconnect;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
		private System.Windows.Forms.TextBox txtSelectedFolder;
		private System.Windows.Forms.Label label4;
		private ListViewEx.ListViewEx lvStudents;
		private System.Windows.Forms.Button btnFolderLocal;
		private System.Windows.Forms.Button btnFolderRemote;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.Button btnAddStudent;
		private System.Windows.Forms.Button btnRemoveStudent;
		private System.Windows.Forms.CheckedListBox lbFeatures;
		private System.Windows.Forms.Button btnRemoveGroup;
		private System.Windows.Forms.Button btnAddGroup;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.Label label7;
		private ListViewEx.ListViewEx lvGroups;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.Button btnAddFromFile;
	}
}

