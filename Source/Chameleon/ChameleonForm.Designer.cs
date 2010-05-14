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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ChameleonForm));
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
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
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.btnNewFile = new System.Windows.Forms.ToolStripButton();
			this.btnOpenLocal = new System.Windows.Forms.ToolStripButton();
			this.btnOpenRemote = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.m_editors = new Chameleon.GUI.EditorContainer();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// statusStrip1
			// 
			this.statusStrip1.Location = new System.Drawing.Point(0, 540);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(784, 22);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile,
            this.menuEdit,
            this.menuHelp});
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
			this.newToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
			this.newToolStripMenuItem.Text = "New";
			this.newToolStripMenuItem.Click += new System.EventHandler(this.OnNewFile);
			// 
			// menuFileOpenLocal
			// 
			this.menuFileOpenLocal.Name = "menuFileOpenLocal";
			this.menuFileOpenLocal.Size = new System.Drawing.Size(166, 22);
			this.menuFileOpenLocal.Text = "Open (Local)";
			this.menuFileOpenLocal.Click += new System.EventHandler(this.OnFileOpenLocal);
			// 
			// menuFileOpenRemote
			// 
			this.menuFileOpenRemote.Name = "menuFileOpenRemote";
			this.menuFileOpenRemote.Size = new System.Drawing.Size(166, 22);
			this.menuFileOpenRemote.Text = "Open (Remote)";
			this.menuFileOpenRemote.Click += new System.EventHandler(this.OnFileOpenRemote);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(163, 6);
			// 
			// menuFileSave
			// 
			this.menuFileSave.Name = "menuFileSave";
			this.menuFileSave.Size = new System.Drawing.Size(166, 22);
			this.menuFileSave.Text = "Save File";
			this.menuFileSave.Click += new System.EventHandler(this.OnFileSave);
			// 
			// menuFileSaveAsLocal
			// 
			this.menuFileSaveAsLocal.Name = "menuFileSaveAsLocal";
			this.menuFileSaveAsLocal.Size = new System.Drawing.Size(166, 22);
			this.menuFileSaveAsLocal.Text = "Save As (Local)";
			this.menuFileSaveAsLocal.Click += new System.EventHandler(this.OnFileSaveAsLocal);
			// 
			// menuFileSaveAsRemote
			// 
			this.menuFileSaveAsRemote.Name = "menuFileSaveAsRemote";
			this.menuFileSaveAsRemote.Size = new System.Drawing.Size(166, 22);
			this.menuFileSaveAsRemote.Text = "Save As (Remote)";
			this.menuFileSaveAsRemote.Click += new System.EventHandler(this.OnFileSaveAsRemote);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(163, 6);
			// 
			// menuFileClose
			// 
			this.menuFileClose.Name = "menuFileClose";
			this.menuFileClose.Size = new System.Drawing.Size(166, 22);
			this.menuFileClose.Text = "Close";
			this.menuFileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// menuFileCloseAll
			// 
			this.menuFileCloseAll.Name = "menuFileCloseAll";
			this.menuFileCloseAll.Size = new System.Drawing.Size(166, 22);
			this.menuFileCloseAll.Text = "Close All";
			this.menuFileCloseAll.Click += new System.EventHandler(this.OnFileCloseAll);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(163, 6);
			// 
			// menuFileExit
			// 
			this.menuFileExit.Name = "menuFileExit";
			this.menuFileExit.Size = new System.Drawing.Size(166, 22);
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
			this.menuEditUndo.Size = new System.Drawing.Size(115, 22);
			this.menuEditUndo.Text = "Undo";
			this.menuEditUndo.Click += new System.EventHandler(this.menuEditUndo_Click);
			// 
			// menuEditRedo
			// 
			this.menuEditRedo.Name = "menuEditRedo";
			this.menuEditRedo.Size = new System.Drawing.Size(115, 22);
			this.menuEditRedo.Text = "Redo";
			this.menuEditRedo.Click += new System.EventHandler(this.menuEditRedo_Click);
			// 
			// toolStripMenuItem4
			// 
			this.toolStripMenuItem4.Name = "toolStripMenuItem4";
			this.toolStripMenuItem4.Size = new System.Drawing.Size(112, 6);
			// 
			// menuEditCut
			// 
			this.menuEditCut.Name = "menuEditCut";
			this.menuEditCut.Size = new System.Drawing.Size(115, 22);
			this.menuEditCut.Text = "Cut";
			this.menuEditCut.Click += new System.EventHandler(this.menuEditCut_Click);
			// 
			// menuEditCopy
			// 
			this.menuEditCopy.Name = "menuEditCopy";
			this.menuEditCopy.Size = new System.Drawing.Size(115, 22);
			this.menuEditCopy.Text = "Copy";
			this.menuEditCopy.Click += new System.EventHandler(this.menuEditCopy_Click);
			// 
			// menuEditPaste
			// 
			this.menuEditPaste.Name = "menuEditPaste";
			this.menuEditPaste.Size = new System.Drawing.Size(115, 22);
			this.menuEditPaste.Text = "Paste";
			this.menuEditPaste.Click += new System.EventHandler(this.menuEditPaste_Click);
			// 
			// toolStripMenuItem5
			// 
			this.toolStripMenuItem5.Name = "toolStripMenuItem5";
			this.toolStripMenuItem5.Size = new System.Drawing.Size(112, 6);
			// 
			// menuEditFind
			// 
			this.menuEditFind.Name = "menuEditFind";
			this.menuEditFind.Size = new System.Drawing.Size(115, 22);
			this.menuEditFind.Text = "Find";
			// 
			// menuEditReplace
			// 
			this.menuEditReplace.Name = "menuEditReplace";
			this.menuEditReplace.Size = new System.Drawing.Size(115, 22);
			this.menuEditReplace.Text = "Replace";
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
			// toolStrip1
			// 
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnNewFile,
            this.btnOpenLocal,
            this.btnOpenRemote,
            this.toolStripSeparator1});
			this.toolStrip1.Location = new System.Drawing.Point(0, 24);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(784, 38);
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
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 38);
			// 
			// imageList1
			// 
			this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			this.imageList1.Images.SetKeyName(0, "moz.png");
			this.imageList1.Images.SetKeyName(1, "newfile.png");
			this.imageList1.Images.SetKeyName(2, "openlocal.png");
			this.imageList1.Images.SetKeyName(3, "openremote.png");
			this.imageList1.Images.SetKeyName(4, "savefile.png");
			// 
			// m_editors
			// 
			this.m_editors.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_editors.Location = new System.Drawing.Point(0, 62);
			this.m_editors.Name = "m_editors";
			this.m_editors.Size = new System.Drawing.Size(784, 478);
			this.m_editors.TabIndex = 4;
			// 
			// ChameleonForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.m_editors);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "ChameleonForm";
			this.Text = "Chameleon";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ChameleonForm_FormClosing);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

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
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.ToolStripButton btnOpenLocal;
		private System.Windows.Forms.ToolStripButton btnOpenRemote;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
	}
}

