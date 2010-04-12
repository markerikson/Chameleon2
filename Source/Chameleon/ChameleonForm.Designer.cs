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
			this.scintilla1 = new ScintillaNet.Scintilla();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			((System.ComponentModel.ISupportInitialize)(this.scintilla1)).BeginInit();
			this.SuspendLayout();
			// 
			// scintilla1
			// 
			this.scintilla1.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.scintilla1.Location = new System.Drawing.Point(12, 12);
			this.scintilla1.Name = "scintilla1";
			this.scintilla1.Size = new System.Drawing.Size(600, 396);
			this.scintilla1.Styles.BraceBad.FontName = "Verdana";
			this.scintilla1.Styles.BraceLight.FontName = "Verdana";
			this.scintilla1.Styles.ControlChar.FontName = "Verdana";
			this.scintilla1.Styles.Default.FontName = "Verdana";
			this.scintilla1.Styles.IndentGuide.FontName = "Verdana";
			this.scintilla1.Styles.LastPredefined.FontName = "Verdana";
			this.scintilla1.Styles.LineNumber.FontName = "Verdana";
			this.scintilla1.Styles.Max.FontName = "Verdana";
			this.scintilla1.TabIndex = 0;
			// 
			// statusStrip1
			// 
			this.statusStrip1.Location = new System.Drawing.Point(0, 420);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(624, 22);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// ChameleonForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.scintilla1);
			this.Name = "ChameleonForm";
			this.Text = "Chameleon";
			((System.ComponentModel.ISupportInitialize)(this.scintilla1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private ScintillaNet.Scintilla scintilla1;
		private System.Windows.Forms.StatusStrip statusStrip1;
	}
}

