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
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.editor1 = new Chameleon.GUI.ChameleonEditor();
			((System.ComponentModel.ISupportInitialize)(this.editor1)).BeginInit();
			this.SuspendLayout();
			// 
			// statusStrip1
			// 
			this.statusStrip1.Location = new System.Drawing.Point(0, 420);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(624, 22);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// editor1
			// 
			this.editor1.ConfigurationManager.CustomLocation = "ConfigCPP.xml";
			this.editor1.ConfigurationManager.Language = "cpp";
			this.editor1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.editor1.Lexing.Lexer = ScintillaNet.Lexer.Cpp;
			this.editor1.Lexing.LexerName = "cpp";
			this.editor1.Lexing.LineCommentPrefix = "";
			this.editor1.Lexing.StreamCommentPrefix = "";
			this.editor1.Lexing.StreamCommentSufix = "";
			this.editor1.Location = new System.Drawing.Point(0, 0);
			this.editor1.Name = "editor1";
			this.editor1.Size = new System.Drawing.Size(624, 420);
			this.editor1.Styles.BraceBad.FontName = "Verdana";
			this.editor1.Styles.BraceBad.Size = 10F;
			this.editor1.Styles.BraceLight.Bold = true;
			this.editor1.Styles.BraceLight.FontName = "Verdana";
			this.editor1.Styles.BraceLight.ForeColor = System.Drawing.Color.Red;
			this.editor1.Styles.BraceLight.Size = 10F;
			this.editor1.Styles.ControlChar.FontName = "Verdana";
			this.editor1.Styles.ControlChar.Size = 10F;
			this.editor1.Styles.Default.FontName = "Verdana";
			this.editor1.Styles.Default.Size = 10F;
			this.editor1.Styles.IndentGuide.FontName = "Verdana";
			this.editor1.Styles.IndentGuide.Size = 10F;
			this.editor1.Styles.LastPredefined.FontName = "Verdana";
			this.editor1.Styles.LastPredefined.Size = 10F;
			this.editor1.Styles.LineNumber.FontName = "Verdana";
			this.editor1.Styles.LineNumber.Size = 10F;
			this.editor1.Styles.Max.FontName = "Verdana";
			this.editor1.Styles.Max.Size = 10F;
			this.editor1.TabIndex = 2;
			// 
			// ChameleonForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.Controls.Add(this.editor1);
			this.Controls.Add(this.statusStrip1);
			this.Name = "ChameleonForm";
			this.Text = "Chameleon";
			((System.ComponentModel.ISupportInitialize)(this.editor1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.StatusStrip statusStrip1;
		private GUI.ChameleonEditor editor1;
	}
}

