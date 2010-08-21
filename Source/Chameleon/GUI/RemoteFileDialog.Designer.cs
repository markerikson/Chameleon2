namespace Chameleon.GUI
{
	partial class RemoteFileDialog
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RemoteFileDialog));
			this.label1 = new System.Windows.Forms.Label();
			this.txtCurrentPath = new System.Windows.Forms.TextBox();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolUpFolder = new System.Windows.Forms.ToolStripButton();
			this.toolHomeFolder = new System.Windows.Forms.ToolStripButton();
			this.toolRefresh = new System.Windows.Forms.ToolStripButton();
			this.listView1 = new System.Windows.Forms.ListView();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.txtFilename = new System.Windows.Forms.TextBox();
			this.cbFileType = new System.Windows.Forms.ComboBox();
			this.btnAccept = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.m_images = new System.Windows.Forms.ImageList(this.components);
			this.m_toolbarImages = new System.Windows.Forms.ImageList(this.components);
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.Location = new System.Drawing.Point(12, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(84, 17);
			this.label1.TabIndex = 0;
			this.label1.Text = "Current path:";
			// 
			// txtCurrentPath
			// 
			this.txtCurrentPath.BackColor = System.Drawing.Color.White;
			this.txtCurrentPath.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtCurrentPath.Location = new System.Drawing.Point(102, 9);
			this.txtCurrentPath.Name = "txtCurrentPath";
			this.txtCurrentPath.ReadOnly = true;
			this.txtCurrentPath.Size = new System.Drawing.Size(427, 25);
			this.txtCurrentPath.TabIndex = 1;
			// 
			// toolStrip1
			// 
			this.toolStrip1.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip1.GripMargin = new System.Windows.Forms.Padding(0);
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolUpFolder,
            this.toolHomeFolder,
            this.toolRefresh});
			this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.toolStrip1.Location = new System.Drawing.Point(512, 9);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(103, 25);
			this.toolStrip1.TabIndex = 2;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// toolUpFolder
			// 
			this.toolUpFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolUpFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolUpFolder.Name = "toolUpFolder";
			this.toolUpFolder.Size = new System.Drawing.Size(23, 22);
			this.toolUpFolder.Text = "Parent Folder";
			this.toolUpFolder.Click += new System.EventHandler(this.OnButtonUpFolderClick);
			// 
			// toolHomeFolder
			// 
			this.toolHomeFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolHomeFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolHomeFolder.Name = "toolHomeFolder";
			this.toolHomeFolder.Size = new System.Drawing.Size(23, 22);
			this.toolHomeFolder.Text = "Home Folder";
			this.toolHomeFolder.Click += new System.EventHandler(this.OnButtonHomeFolderClick);
			// 
			// toolRefresh
			// 
			this.toolRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolRefresh.Name = "toolRefresh";
			this.toolRefresh.Size = new System.Drawing.Size(23, 22);
			this.toolRefresh.Text = "Refresh Folder";
			this.toolRefresh.Click += new System.EventHandler(this.OnButtonRefreshFolderClick);
			// 
			// listView1
			// 
			this.listView1.Location = new System.Drawing.Point(12, 40);
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(603, 321);
			this.listView1.TabIndex = 3;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.View = System.Windows.Forms.View.List;
			this.listView1.ItemActivate += new System.EventHandler(this.OnItemActivated);
			this.listView1.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.OnItemSelected);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label2.Location = new System.Drawing.Point(12, 375);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(66, 17);
			this.label2.TabIndex = 4;
			this.label2.Text = "File name:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label3.Location = new System.Drawing.Point(12, 408);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(81, 17);
			this.label3.TabIndex = 5;
			this.label3.Text = "Files of type:";
			// 
			// txtFilename
			// 
			this.txtFilename.BackColor = System.Drawing.Color.White;
			this.txtFilename.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtFilename.Location = new System.Drawing.Point(102, 372);
			this.txtFilename.Name = "txtFilename";
			this.txtFilename.Size = new System.Drawing.Size(427, 25);
			this.txtFilename.TabIndex = 6;
			this.txtFilename.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.OnFilenameKeyDown);
			// 
			// cbFileType
			// 
			this.cbFileType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbFileType.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.cbFileType.FormattingEnabled = true;
			this.cbFileType.Location = new System.Drawing.Point(102, 405);
			this.cbFileType.Name = "cbFileType";
			this.cbFileType.Size = new System.Drawing.Size(427, 25);
			this.cbFileType.TabIndex = 7;
			this.cbFileType.SelectedIndexChanged += new System.EventHandler(this.OnFileTypeSelected);
			// 
			// btnAccept
			// 
			this.btnAccept.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnAccept.Location = new System.Drawing.Point(540, 374);
			this.btnAccept.Name = "btnAccept";
			this.btnAccept.Size = new System.Drawing.Size(75, 23);
			this.btnAccept.TabIndex = 8;
			this.btnAccept.Text = "Open";
			this.btnAccept.UseVisualStyleBackColor = true;
			this.btnAccept.Click += new System.EventHandler(this.OnButtonAcceptClick);
			// 
			// btnCancel
			// 
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(540, 406);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 9;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			this.btnCancel.Click += new System.EventHandler(this.OnButtonCancelClick);
			// 
			// m_images
			// 
			this.m_images.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("m_images.ImageStream")));
			this.m_images.TransparentColor = System.Drawing.Color.Transparent;
			this.m_images.Images.SetKeyName(0, "XPfolder_closed.png");
			// 
			// m_toolbarImages
			// 
			this.m_toolbarImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("m_toolbarImages.ImageStream")));
			this.m_toolbarImages.TransparentColor = System.Drawing.Color.Transparent;
			this.m_toolbarImages.Images.SetKeyName(0, "upfolder.png");
			this.m_toolbarImages.Images.SetKeyName(1, "HomeHS.png");
			this.m_toolbarImages.Images.SetKeyName(2, "refresh.png");
			// 
			// RemoteFileDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.ControlBox = false;
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnAccept);
			this.Controls.Add(this.cbFileType);
			this.Controls.Add(this.txtFilename);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.listView1);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.txtCurrentPath);
			this.Controls.Add(this.label1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "RemoteFileDialog";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "RemoteFileDialog";
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtCurrentPath;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton toolUpFolder;
		private System.Windows.Forms.ToolStripButton toolHomeFolder;
		private System.Windows.Forms.ToolStripButton toolRefresh;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtFilename;
		private System.Windows.Forms.ComboBox cbFileType;
		private System.Windows.Forms.Button btnAccept;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.ImageList m_images;
		private System.Windows.Forms.ImageList m_toolbarImages;
	}
}