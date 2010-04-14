namespace Chameleon.GUI
{
	partial class EditorContainer
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.m_tabStrip = new FarsiLibrary.Win.FATabStrip();
			((System.ComponentModel.ISupportInitialize)(this.m_tabStrip)).BeginInit();
			this.SuspendLayout();
			// 
			// m_tabStrip
			// 
			this.m_tabStrip.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_tabStrip.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.m_tabStrip.Location = new System.Drawing.Point(0, 0);
			this.m_tabStrip.Name = "m_tabStrip";
			this.m_tabStrip.Size = new System.Drawing.Size(581, 311);
			this.m_tabStrip.TabIndex = 0;
			this.m_tabStrip.Text = "faTabStrip1";
			// 
			// EditorContainer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_tabStrip);
			this.Name = "EditorContainer";
			this.Size = new System.Drawing.Size(581, 311);
			((System.ComponentModel.ISupportInitialize)(this.m_tabStrip)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private FarsiLibrary.Win.FATabStrip m_tabStrip;
	}
}
