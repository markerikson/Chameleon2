using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Reflection;

namespace Chameleon.GUI
{
	/// <summary>
	/// Summary description for SplashForm.
	/// </summary>
	public class SplashForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label lStatusInfo;
		private System.Windows.Forms.Label label2;
		private PictureBox pictureBox1;
		private Label label1;
		private Label lblVersion;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public string AssemblyVersion
		{
			get
			{
				return Assembly.GetExecutingAssembly().GetName().Version.ToString();
			}
		}

		public SplashForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			lblVersion.Text = AssemblyVersion;
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		public string StatusInfo 
		{
			set 
			{
				_StatusInfo = value;
				ChangeStatusText();
			}
			get 
			{
				return _StatusInfo;
			}
		}

		public void ChangeStatusText() 
		{
			try 
			{
				if (this.InvokeRequired) 
				{
					this.Invoke(new MethodInvoker(this.ChangeStatusText));
					return;
				}

				lStatusInfo.Text = _StatusInfo;
			}
			catch (Exception e) 
			{
				//	do something here...
			}
		}
		private string _StatusInfo = "";

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			ControlPaint.DrawBorder(e.Graphics, this.ClientRectangle, Color.Black, ButtonBorderStyle.Solid);
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SplashForm));
			this.lStatusInfo = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.label1 = new System.Windows.Forms.Label();
			this.lblVersion = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.SuspendLayout();
			// 
			// lStatusInfo
			// 
			this.lStatusInfo.Location = new System.Drawing.Point(43, 127);
			this.lStatusInfo.Name = "lStatusInfo";
			this.lStatusInfo.Size = new System.Drawing.Size(261, 16);
			this.lStatusInfo.TabIndex = 1;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(1, 126);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(40, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "Status:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// pictureBox1
			// 
			this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
			this.pictureBox1.Location = new System.Drawing.Point(1, 1);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(480, 122);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(352, 125);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(48, 16);
			this.label1.TabIndex = 3;
			this.label1.Text = "Version:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// lblVersion
			// 
			this.lblVersion.Location = new System.Drawing.Point(397, 125);
			this.lblVersion.Name = "lblVersion";
			this.lblVersion.Size = new System.Drawing.Size(83, 16);
			this.lblVersion.TabIndex = 4;
			this.lblVersion.Text = "2.1.0.123";
			this.lblVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// SplashForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(482, 150);
			this.ControlBox = false;
			this.Controls.Add(this.lblVersion);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.lStatusInfo);
			this.Controls.Add(this.pictureBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SplashForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "SplashForm";
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion
	}
}
