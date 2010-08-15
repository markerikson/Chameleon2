namespace Chameleon
{
	partial class ChameleonForm
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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("Group 1", System.Windows.Forms.HorizontalAlignment.Left);
			System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("Group 2", System.Windows.Forms.HorizontalAlignment.Left);
			System.Windows.Forms.ListViewGroup listViewGroup3 = new System.Windows.Forms.ListViewGroup("Group 3", System.Windows.Forms.HorizontalAlignment.Left);
			System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("For loop", 0);
			System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("While loop", 2);
			System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("If statement", 1);
			System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("item 4");
			System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("item 5");
			System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("item 6");
			System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("item 7");
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ChameleonForm));
			this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.toolStatusConnected = new System.Windows.Forms.ToolStripStatusLabel();
			this.splitSnippetsEditor = new System.Windows.Forms.SplitContainer();
			this.listView1 = new System.Windows.Forms.ListView();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.splitEditorTerminal = new System.Windows.Forms.SplitContainer();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.menuFile = new System.Windows.Forms.ToolStripMenuItem();
			this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFileOpenLocal = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFileOpenRemote = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.menuFileSave = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFileSaveAsLocal = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFileSaveAsRemote = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.menuFileClose = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFileCloseAll = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.menuFileExit = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEdit = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEditUndo = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEditRedo = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
			this.menuEditCut = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEditCopy = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEditPaste = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
			this.menuEditFind = new System.Windows.Forms.ToolStripMenuItem();
			this.menuEditReplace = new System.Windows.Forms.ToolStripMenuItem();
			this.menuHelp = new System.Windows.Forms.ToolStripMenuItem();
			this.menuHelpAbout = new System.Windows.Forms.ToolStripMenuItem();
			this.menuFeatures = new System.Windows.Forms.ToolStripMenuItem();
			this.executeRemoteCommandToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.parserToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tagsByScopeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.localVariablesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.parseExpressionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.btnNewFile = new System.Windows.Forms.ToolStripButton();
			this.btnOpenLocal = new System.Windows.Forms.ToolStripButton();
			this.btnOpenRemote = new System.Windows.Forms.ToolStripButton();
			this.btnSave = new System.Windows.Forms.ToolStripSplitButton();
			this.btnSaveNormal = new System.Windows.Forms.ToolStripMenuItem();
			this.btnSaveAsL = new System.Windows.Forms.ToolStripMenuItem();
			this.btnSaveAsR = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.btnDummyFeature1 = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.btnDummyFeature2 = new System.Windows.Forms.ToolStripButton();
			this.toolStrip2 = new System.Windows.Forms.ToolStrip();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.toolTextHost = new System.Windows.Forms.ToolStripTextBox();
			this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
			this.toolTextUser = new System.Windows.Forms.ToolStripTextBox();
			this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
			this.toolTextPassword = new System.Windows.Forms.ToolStripTextBox();
			this.toolHostConnect = new System.Windows.Forms.ToolStripButton();
			this.toolHostDisconnect = new System.Windows.Forms.ToolStripButton();
			this.terminalEmulator1 = new WalburySoftware.TerminalEmulator();
			this.toolStripContainer1.BottomToolStripPanel.SuspendLayout();
			this.toolStripContainer1.ContentPanel.SuspendLayout();
			this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
			this.toolStripContainer1.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitSnippetsEditor)).BeginInit();
			this.splitSnippetsEditor.Panel1.SuspendLayout();
			this.splitSnippetsEditor.Panel2.SuspendLayout();
			this.splitSnippetsEditor.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitEditorTerminal)).BeginInit();
			this.splitEditorTerminal.Panel2.SuspendLayout();
			this.splitEditorTerminal.SuspendLayout();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.toolStrip2.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStripContainer1
			// 
			// 
			// toolStripContainer1.BottomToolStripPanel
			// 
			this.toolStripContainer1.BottomToolStripPanel.Controls.Add(this.statusStrip1);
			// 
			// toolStripContainer1.ContentPanel
			// 
			this.toolStripContainer1.ContentPanel.AutoScroll = true;
			this.toolStripContainer1.ContentPanel.Controls.Add(this.splitSnippetsEditor);
			this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(784, 453);
			this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.toolStripContainer1.LeftToolStripPanelVisible = false;
			this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
			this.toolStripContainer1.Name = "toolStripContainer1";
			this.toolStripContainer1.RightToolStripPanelVisible = false;
			this.toolStripContainer1.Size = new System.Drawing.Size(784, 562);
			this.toolStripContainer1.TabIndex = 5;
			this.toolStripContainer1.Text = "toolStripContainer1";
			// 
			// toolStripContainer1.TopToolStripPanel
			// 
			this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.menuStrip1);
			this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip1);
			this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip2);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.statusStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStatusConnected});
			this.statusStrip1.Location = new System.Drawing.Point(0, 0);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(784, 22);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// toolStatusConnected
			// 
			this.toolStatusConnected.Name = "toolStatusConnected";
			this.toolStatusConnected.Size = new System.Drawing.Size(0, 17);
			// 
			// splitSnippetsEditor
			// 
			this.splitSnippetsEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitSnippetsEditor.Location = new System.Drawing.Point(0, 0);
			this.splitSnippetsEditor.Name = "splitSnippetsEditor";
			// 
			// splitSnippetsEditor.Panel1
			// 
			this.splitSnippetsEditor.Panel1.Controls.Add(this.listView1);
			// 
			// splitSnippetsEditor.Panel2
			// 
			this.splitSnippetsEditor.Panel2.Controls.Add(this.splitEditorTerminal);
			this.splitSnippetsEditor.Size = new System.Drawing.Size(784, 453);
			this.splitSnippetsEditor.SplitterDistance = 120;
			this.splitSnippetsEditor.TabIndex = 5;
			// 
			// listView1
			// 
			this.listView1.Alignment = System.Windows.Forms.ListViewAlignment.Default;
			this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
			listViewGroup1.Header = "Group 1";
			listViewGroup1.Name = "listViewGroup1";
			listViewGroup2.Header = "Group 2";
			listViewGroup2.Name = "listViewGroup2";
			listViewGroup3.Header = "Group 3";
			listViewGroup3.Name = "listViewGroup3";
			this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1,
            listViewGroup2,
            listViewGroup3});
			this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			listViewItem1.Tag = "for";
			listViewItem2.Tag = "while";
			listViewItem3.Tag = "if";
			this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3,
            listViewItem4,
            listViewItem5,
            listViewItem6,
            listViewItem7});
			this.listView1.LargeImageList = this.imageList1;
			this.listView1.Location = new System.Drawing.Point(0, 0);
			this.listView1.Margin = new System.Windows.Forms.Padding(0);
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(120, 453);
			this.listView1.TabIndex = 0;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.listView1_ItemDrag);
			// 
			// imageList1
			// 
			this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			this.imageList1.Images.SetKeyName(0, "for.png");
			this.imageList1.Images.SetKeyName(1, "if.png");
			this.imageList1.Images.SetKeyName(2, "while.png");
			// 
			// splitEditorTerminal
			// 
			this.splitEditorTerminal.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitEditorTerminal.Location = new System.Drawing.Point(0, 0);
			this.splitEditorTerminal.Name = "splitEditorTerminal";
			this.splitEditorTerminal.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitEditorTerminal.Panel2
			// 
			this.splitEditorTerminal.Panel2.Controls.Add(this.terminalEmulator1);
			this.splitEditorTerminal.Size = new System.Drawing.Size(660, 453);
			this.splitEditorTerminal.SplitterDistance = 261;
			this.splitEditorTerminal.TabIndex = 5;
			// 
			// menuStrip1
			// 
			this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile,
            this.menuEdit,
            this.menuHelp,
            this.menuFeatures,
            this.parserToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(784, 24);
			this.menuStrip1.TabIndex = 2;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// menuFile
			// 
			this.menuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.menuFileOpenLocal,
            this.menuFileOpenRemote,
            this.toolStripMenuItem1,
            this.menuFileSave,
            this.menuFileSaveAsLocal,
            this.menuFileSaveAsRemote,
            this.toolStripMenuItem2,
            this.menuFileClose,
            this.menuFileCloseAll,
            this.toolStripMenuItem3,
            this.menuFileExit});
			this.menuFile.Name = "menuFile";
			this.menuFile.Size = new System.Drawing.Size(37, 20);
			this.menuFile.Text = "File";
			// 
			// newToolStripMenuItem
			// 
			this.newToolStripMenuItem.Name = "newToolStripMenuItem";
			this.newToolStripMenuItem.Size = new System.Drawing.Size(230, 22);
			this.newToolStripMenuItem.Text = "New";
			this.newToolStripMenuItem.Click += new System.EventHandler(this.OnNewFile);
			// 
			// menuFileOpenLocal
			// 
			this.menuFileOpenLocal.Name = "menuFileOpenLocal";
			this.menuFileOpenLocal.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.menuFileOpenLocal.Size = new System.Drawing.Size(230, 22);
			this.menuFileOpenLocal.Text = "Open (Local)";
			this.menuFileOpenLocal.Click += new System.EventHandler(this.OnFileOpenLocal);
			// 
			// menuFileOpenRemote
			// 
			this.menuFileOpenRemote.Name = "menuFileOpenRemote";
			this.menuFileOpenRemote.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
						| System.Windows.Forms.Keys.O)));
			this.menuFileOpenRemote.Size = new System.Drawing.Size(230, 22);
			this.menuFileOpenRemote.Text = "Open (Remote)";
			this.menuFileOpenRemote.Click += new System.EventHandler(this.OnFileOpenRemote);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(227, 6);
			// 
			// menuFileSave
			// 
			this.menuFileSave.Name = "menuFileSave";
			this.menuFileSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.menuFileSave.Size = new System.Drawing.Size(230, 22);
			this.menuFileSave.Text = "Save File";
			this.menuFileSave.Click += new System.EventHandler(this.OnFileSave);
			// 
			// menuFileSaveAsLocal
			// 
			this.menuFileSaveAsLocal.Name = "menuFileSaveAsLocal";
			this.menuFileSaveAsLocal.Size = new System.Drawing.Size(230, 22);
			this.menuFileSaveAsLocal.Text = "Save As (Local)";
			this.menuFileSaveAsLocal.Click += new System.EventHandler(this.OnFileSaveAsLocal);
			// 
			// menuFileSaveAsRemote
			// 
			this.menuFileSaveAsRemote.Name = "menuFileSaveAsRemote";
			this.menuFileSaveAsRemote.Size = new System.Drawing.Size(230, 22);
			this.menuFileSaveAsRemote.Text = "Save As (Remote)";
			this.menuFileSaveAsRemote.Click += new System.EventHandler(this.OnFileSaveAsRemote);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(227, 6);
			// 
			// menuFileClose
			// 
			this.menuFileClose.Name = "menuFileClose";
			this.menuFileClose.Size = new System.Drawing.Size(230, 22);
			this.menuFileClose.Text = "Close";
			this.menuFileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// menuFileCloseAll
			// 
			this.menuFileCloseAll.Name = "menuFileCloseAll";
			this.menuFileCloseAll.Size = new System.Drawing.Size(230, 22);
			this.menuFileCloseAll.Text = "Close All";
			this.menuFileCloseAll.Click += new System.EventHandler(this.OnFileCloseAll);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(227, 6);
			// 
			// menuFileExit
			// 
			this.menuFileExit.Name = "menuFileExit";
			this.menuFileExit.Size = new System.Drawing.Size(230, 22);
			this.menuFileExit.Text = "Exit";
			// 
			// menuEdit
			// 
			this.menuEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuEditUndo,
            this.menuEditRedo,
            this.toolStripMenuItem4,
            this.menuEditCut,
            this.menuEditCopy,
            this.menuEditPaste,
            this.toolStripMenuItem5,
            this.menuEditFind,
            this.menuEditReplace});
			this.menuEdit.Name = "menuEdit";
			this.menuEdit.Size = new System.Drawing.Size(39, 20);
			this.menuEdit.Text = "Edit";
			// 
			// menuEditUndo
			// 
			this.menuEditUndo.Name = "menuEditUndo";
			this.menuEditUndo.Size = new System.Drawing.Size(158, 22);
			this.menuEditUndo.Text = "Undo";
			this.menuEditUndo.Click += new System.EventHandler(this.menuEditUndo_Click);
			// 
			// menuEditRedo
			// 
			this.menuEditRedo.Name = "menuEditRedo";
			this.menuEditRedo.Size = new System.Drawing.Size(158, 22);
			this.menuEditRedo.Text = "Redo";
			this.menuEditRedo.Click += new System.EventHandler(this.menuEditRedo_Click);
			// 
			// toolStripMenuItem4
			// 
			this.toolStripMenuItem4.Name = "toolStripMenuItem4";
			this.toolStripMenuItem4.Size = new System.Drawing.Size(155, 6);
			// 
			// menuEditCut
			// 
			this.menuEditCut.Name = "menuEditCut";
			this.menuEditCut.Size = new System.Drawing.Size(158, 22);
			this.menuEditCut.Text = "Cut";
			this.menuEditCut.Click += new System.EventHandler(this.menuEditCut_Click);
			// 
			// menuEditCopy
			// 
			this.menuEditCopy.Name = "menuEditCopy";
			this.menuEditCopy.Size = new System.Drawing.Size(158, 22);
			this.menuEditCopy.Text = "Copy";
			this.menuEditCopy.Click += new System.EventHandler(this.menuEditCopy_Click);
			// 
			// menuEditPaste
			// 
			this.menuEditPaste.Name = "menuEditPaste";
			this.menuEditPaste.Size = new System.Drawing.Size(158, 22);
			this.menuEditPaste.Text = "Paste";
			this.menuEditPaste.Click += new System.EventHandler(this.menuEditPaste_Click);
			// 
			// toolStripMenuItem5
			// 
			this.toolStripMenuItem5.Name = "toolStripMenuItem5";
			this.toolStripMenuItem5.Size = new System.Drawing.Size(155, 6);
			// 
			// menuEditFind
			// 
			this.menuEditFind.Name = "menuEditFind";
			this.menuEditFind.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
			this.menuEditFind.Size = new System.Drawing.Size(158, 22);
			this.menuEditFind.Text = "Find";
			this.menuEditFind.Click += new System.EventHandler(this.menuEditFind_Click);
			// 
			// menuEditReplace
			// 
			this.menuEditReplace.Name = "menuEditReplace";
			this.menuEditReplace.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.H)));
			this.menuEditReplace.Size = new System.Drawing.Size(158, 22);
			this.menuEditReplace.Text = "Replace";
			this.menuEditReplace.Click += new System.EventHandler(this.menuEditReplace_Click);
			// 
			// menuHelp
			// 
			this.menuHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuHelpAbout});
			this.menuHelp.Name = "menuHelp";
			this.menuHelp.Size = new System.Drawing.Size(44, 20);
			this.menuHelp.Text = "Help";
			// 
			// menuHelpAbout
			// 
			this.menuHelpAbout.Name = "menuHelpAbout";
			this.menuHelpAbout.Size = new System.Drawing.Size(107, 22);
			this.menuHelpAbout.Text = "About";
			this.menuHelpAbout.Click += new System.EventHandler(this.menuHelpAbout_Click);
			// 
			// menuFeatures
			// 
			this.menuFeatures.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.executeRemoteCommandToolStripMenuItem});
			this.menuFeatures.Name = "menuFeatures";
			this.menuFeatures.Size = new System.Drawing.Size(63, 20);
			this.menuFeatures.Text = "Features";
			// 
			// executeRemoteCommandToolStripMenuItem
			// 
			this.executeRemoteCommandToolStripMenuItem.Name = "executeRemoteCommandToolStripMenuItem";
			this.executeRemoteCommandToolStripMenuItem.Size = new System.Drawing.Size(218, 22);
			this.executeRemoteCommandToolStripMenuItem.Text = "Execute Remote Command";
			this.executeRemoteCommandToolStripMenuItem.Click += new System.EventHandler(this.executeRemoteCommandToolStripMenuItem_Click);
			// 
			// parserToolStripMenuItem
			// 
			this.parserToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tagsByScopeToolStripMenuItem,
            this.localVariablesToolStripMenuItem,
            this.parseExpressionToolStripMenuItem});
			this.parserToolStripMenuItem.Name = "parserToolStripMenuItem";
			this.parserToolStripMenuItem.Size = new System.Drawing.Size(51, 20);
			this.parserToolStripMenuItem.Text = "Parser";
			// 
			// tagsByScopeToolStripMenuItem
			// 
			this.tagsByScopeToolStripMenuItem.Name = "tagsByScopeToolStripMenuItem";
			this.tagsByScopeToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
			this.tagsByScopeToolStripMenuItem.Text = "Tags by Scope";
			this.tagsByScopeToolStripMenuItem.Click += new System.EventHandler(this.tagsByScopeToolStripMenuItem_Click);
			// 
			// localVariablesToolStripMenuItem
			// 
			this.localVariablesToolStripMenuItem.Name = "localVariablesToolStripMenuItem";
			this.localVariablesToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
			this.localVariablesToolStripMenuItem.Text = "Local Variables";
			this.localVariablesToolStripMenuItem.Click += new System.EventHandler(this.localVariablesToolStripMenuItem_Click);
			// 
			// parseExpressionToolStripMenuItem
			// 
			this.parseExpressionToolStripMenuItem.Name = "parseExpressionToolStripMenuItem";
			this.parseExpressionToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
			this.parseExpressionToolStripMenuItem.Text = "Parse Expression";
			this.parseExpressionToolStripMenuItem.Click += new System.EventHandler(this.parseExpressionToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnNewFile,
            this.btnOpenLocal,
            this.btnOpenRemote,
            this.btnSave,
            this.toolStripSeparator1,
            this.btnDummyFeature1,
            this.toolStripSeparator2,
            this.btnDummyFeature2});
			this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.toolStrip1.Location = new System.Drawing.Point(3, 24);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(330, 38);
			this.toolStrip1.TabIndex = 3;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// btnNewFile
			// 
			this.btnNewFile.Image = global::Chameleon.Properties.Resources.newfile;
			this.btnNewFile.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnNewFile.Name = "btnNewFile";
			this.btnNewFile.Size = new System.Drawing.Size(35, 35);
			this.btnNewFile.Text = "New";
			this.btnNewFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			this.btnNewFile.Click += new System.EventHandler(this.OnNewFile);
			// 
			// btnOpenLocal
			// 
			this.btnOpenLocal.Image = global::Chameleon.Properties.Resources.openlocal;
			this.btnOpenLocal.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnOpenLocal.Name = "btnOpenLocal";
			this.btnOpenLocal.Size = new System.Drawing.Size(57, 35);
			this.btnOpenLocal.Text = "Open (L)";
			this.btnOpenLocal.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			this.btnOpenLocal.Click += new System.EventHandler(this.OnFileOpenLocal);
			// 
			// btnOpenRemote
			// 
			this.btnOpenRemote.Image = global::Chameleon.Properties.Resources.openremote;
			this.btnOpenRemote.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnOpenRemote.Name = "btnOpenRemote";
			this.btnOpenRemote.Size = new System.Drawing.Size(58, 35);
			this.btnOpenRemote.Text = "Open (R)";
			this.btnOpenRemote.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			this.btnOpenRemote.Click += new System.EventHandler(this.OnFileOpenRemote);
			// 
			// btnSave
			// 
			this.btnSave.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnSaveNormal,
            this.btnSaveAsL,
            this.btnSaveAsR});
			this.btnSave.Image = ((System.Drawing.Image)(resources.GetObject("btnSave.Image")));
			this.btnSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnSave.Name = "btnSave";
			this.btnSave.Size = new System.Drawing.Size(47, 35);
			this.btnSave.Text = "Save";
			this.btnSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			this.btnSave.ButtonClick += new System.EventHandler(this.OnFileSave);
			// 
			// btnSaveNormal
			// 
			this.btnSaveNormal.Name = "btnSaveNormal";
			this.btnSaveNormal.Size = new System.Drawing.Size(132, 22);
			this.btnSaveNormal.Text = "Save";
			this.btnSaveNormal.Click += new System.EventHandler(this.OnFileSave);
			// 
			// btnSaveAsL
			// 
			this.btnSaveAsL.Name = "btnSaveAsL";
			this.btnSaveAsL.Size = new System.Drawing.Size(132, 22);
			this.btnSaveAsL.Text = "Save As (L)";
			this.btnSaveAsL.Click += new System.EventHandler(this.OnFileSaveAsLocal);
			// 
			// btnSaveAsR
			// 
			this.btnSaveAsR.Name = "btnSaveAsR";
			this.btnSaveAsR.Size = new System.Drawing.Size(132, 22);
			this.btnSaveAsR.Text = "Save As (R)";
			this.btnSaveAsR.Click += new System.EventHandler(this.OnFileSaveAsRemote);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 38);
			// 
			// btnDummyFeature1
			// 
			this.btnDummyFeature1.Image = ((System.Drawing.Image)(resources.GetObject("btnDummyFeature1.Image")));
			this.btnDummyFeature1.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnDummyFeature1.Name = "btnDummyFeature1";
			this.btnDummyFeature1.Size = new System.Drawing.Size(59, 35);
			this.btnDummyFeature1.Text = "Feature 1";
			this.btnDummyFeature1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 38);
			// 
			// btnDummyFeature2
			// 
			this.btnDummyFeature2.Image = ((System.Drawing.Image)(resources.GetObject("btnDummyFeature2.Image")));
			this.btnDummyFeature2.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.btnDummyFeature2.Name = "btnDummyFeature2";
			this.btnDummyFeature2.Size = new System.Drawing.Size(59, 35);
			this.btnDummyFeature2.Text = "Feature 2";
			this.btnDummyFeature2.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
			// 
			// toolStrip2
			// 
			this.toolStrip2.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip2.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.toolTextHost,
            this.toolStripLabel3,
            this.toolTextUser,
            this.toolStripLabel2,
            this.toolTextPassword,
            this.toolHostConnect,
            this.toolHostDisconnect});
			this.toolStrip2.Location = new System.Drawing.Point(3, 62);
			this.toolStrip2.Name = "toolStrip2";
			this.toolStrip2.Size = new System.Drawing.Size(554, 25);
			this.toolStrip2.TabIndex = 4;
			// 
			// toolStripLabel1
			// 
			this.toolStripLabel1.Name = "toolStripLabel1";
			this.toolStripLabel1.Size = new System.Drawing.Size(35, 22);
			this.toolStripLabel1.Text = "Host:";
			// 
			// toolTextHost
			// 
			this.toolTextHost.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.toolTextHost.Name = "toolTextHost";
			this.toolTextHost.Size = new System.Drawing.Size(140, 25);
			// 
			// toolStripLabel3
			// 
			this.toolStripLabel3.Name = "toolStripLabel3";
			this.toolStripLabel3.Size = new System.Drawing.Size(33, 22);
			this.toolStripLabel3.Text = "User:";
			// 
			// toolTextUser
			// 
			this.toolTextUser.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.toolTextUser.Name = "toolTextUser";
			this.toolTextUser.Size = new System.Drawing.Size(100, 25);
			// 
			// toolStripLabel2
			// 
			this.toolStripLabel2.Name = "toolStripLabel2";
			this.toolStripLabel2.Size = new System.Drawing.Size(60, 22);
			this.toolStripLabel2.Text = "Password:";
			// 
			// toolTextPassword
			// 
			this.toolTextPassword.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.toolTextPassword.Name = "toolTextPassword";
			this.toolTextPassword.Size = new System.Drawing.Size(100, 25);
			this.toolTextPassword.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.toolTextPassword_KeyPress);
			// 
			// toolHostConnect
			// 
			this.toolHostConnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolHostConnect.Image = global::Chameleon.Properties.Resources.connect16;
			this.toolHostConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolHostConnect.Name = "toolHostConnect";
			this.toolHostConnect.Size = new System.Drawing.Size(23, 22);
			this.toolHostConnect.Text = "Connect";
			this.toolHostConnect.Click += new System.EventHandler(this.OnHostConnect);
			// 
			// toolHostDisconnect
			// 
			this.toolHostDisconnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolHostDisconnect.Image = global::Chameleon.Properties.Resources.disconnect16;
			this.toolHostDisconnect.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolHostDisconnect.Name = "toolHostDisconnect";
			this.toolHostDisconnect.Size = new System.Drawing.Size(23, 22);
			this.toolHostDisconnect.Text = "Disconnect";
			this.toolHostDisconnect.Click += new System.EventHandler(this.OnHostDisconnect);
			// 
			// terminalEmulator1
			// 
			this.terminalEmulator1.BackColor = System.Drawing.Color.Black;
			this.terminalEmulator1.Columns = 81;
			this.terminalEmulator1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.terminalEmulator1.Enabled = false;
			this.terminalEmulator1.Font = new System.Drawing.Font("Courier New", 10F);
			this.terminalEmulator1.ForeColor = System.Drawing.Color.White;
			this.terminalEmulator1.Location = new System.Drawing.Point(0, 0);
			this.terminalEmulator1.Name = "terminalEmulator1";
			this.terminalEmulator1.Rows = 11;
			this.terminalEmulator1.Size = new System.Drawing.Size(660, 188);
			this.terminalEmulator1.TabIndex = 0;
			this.terminalEmulator1.Text = "terminalEmulator1";
			// 
			// ChameleonForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.toolStripContainer1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "ChameleonForm";
			this.Text = "Chameleon";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ChameleonForm_FormClosing);
			this.toolStripContainer1.BottomToolStripPanel.ResumeLayout(false);
			this.toolStripContainer1.BottomToolStripPanel.PerformLayout();
			this.toolStripContainer1.ContentPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.PerformLayout();
			this.toolStripContainer1.ResumeLayout(false);
			this.toolStripContainer1.PerformLayout();
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.splitSnippetsEditor.Panel1.ResumeLayout(false);
			this.splitSnippetsEditor.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitSnippetsEditor)).EndInit();
			this.splitSnippetsEditor.ResumeLayout(false);
			this.splitEditorTerminal.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitEditorTerminal)).EndInit();
			this.splitEditorTerminal.ResumeLayout(false);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.toolStrip2.ResumeLayout(false);
			this.toolStrip2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem menuFile;
		private System.Windows.Forms.ToolStripMenuItem menuEdit;
		private System.Windows.Forms.ToolStripMenuItem menuHelp;
		private System.Windows.Forms.ToolStripMenuItem menuHelpAbout;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton btnNewFile;
		private GUI.EditorContainer m_editors;
		private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem menuFileOpenLocal;
		private System.Windows.Forms.ToolStripMenuItem menuFileOpenRemote;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem menuFileExit;
		private System.Windows.Forms.ToolStripMenuItem menuFileSave;
		private System.Windows.Forms.ToolStripMenuItem menuFileSaveAsLocal;
		private System.Windows.Forms.ToolStripMenuItem menuFileSaveAsRemote;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem menuFileClose;
		private System.Windows.Forms.ToolStripMenuItem menuFileCloseAll;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem menuEditUndo;
		private System.Windows.Forms.ToolStripMenuItem menuEditRedo;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
		private System.Windows.Forms.ToolStripMenuItem menuEditCut;
		private System.Windows.Forms.ToolStripMenuItem menuEditCopy;
		private System.Windows.Forms.ToolStripMenuItem menuEditPaste;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
		private System.Windows.Forms.ToolStripMenuItem menuEditFind;
		private System.Windows.Forms.ToolStripMenuItem menuEditReplace;
		private System.Windows.Forms.ToolStripButton btnOpenLocal;
		private System.Windows.Forms.ToolStripButton btnOpenRemote;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripContainer toolStripContainer1;
		private System.Windows.Forms.ToolStrip toolStrip2;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ToolStripTextBox toolTextHost;
		private System.Windows.Forms.ToolStripLabel toolStripLabel2;
		private System.Windows.Forms.ToolStripTextBox toolTextPassword;
		private System.Windows.Forms.ToolStripButton toolHostConnect;
		private System.Windows.Forms.ToolStripButton toolHostDisconnect;
		private System.Windows.Forms.ToolStripStatusLabel toolStatusConnected;
		private System.Windows.Forms.ToolStripLabel toolStripLabel3;
		private System.Windows.Forms.ToolStripTextBox toolTextUser;
		private System.Windows.Forms.SplitContainer splitEditorTerminal;
		private WalburySoftware.TerminalEmulator terminalEmulator1;
		private System.Windows.Forms.ToolStripSplitButton btnSave;
		private System.Windows.Forms.ToolStripMenuItem btnSaveNormal;
		private System.Windows.Forms.ToolStripMenuItem btnSaveAsL;
		private System.Windows.Forms.ToolStripMenuItem btnSaveAsR;
		private System.Windows.Forms.ToolStripButton btnDummyFeature1;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton btnDummyFeature2;
		private System.Windows.Forms.SplitContainer splitSnippetsEditor;
		private System.Windows.Forms.ToolStripMenuItem menuFeatures;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.ToolStripMenuItem parserToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem tagsByScopeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem localVariablesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem parseExpressionToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem executeRemoteCommandToolStripMenuItem;
	}
}

