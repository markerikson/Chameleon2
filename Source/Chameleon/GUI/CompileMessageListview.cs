using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Chameleon.GUI
{
	public class CompileMessageListView : ListView
	{
		private string m_compileResultMessage;

		public string CompileResultMessage
		{
			get { return m_compileResultMessage; }
			set { m_compileResultMessage = value; }
		}

		public CompileMessageListView()
		{
			SetStyle(ControlStyles.ResizeRedraw, true);
			m_compileResultMessage = "";
		}

		const int WM_PAINT = 0xF;
		const int WM_ERASEBKGND = 0x14;

		protected override void WndProc(ref Message m)
		{
			base.WndProc(ref m);

			if(m.Msg == WM_PAINT)
			{
				if(Items.Count > 0)
				{
					ListViewGroup compileResult = Groups["groupCompileResult"];

					if(compileResult.Items.Count > 0)
					{
						Rectangle lvArea = compileResult.Items[0].GetBounds(ItemBoundsPortion.Entire);
						lvArea.Offset(20, 0);
						StringFormat sf = new StringFormat();
						sf.Alignment = StringAlignment.Near;

						using(Graphics g = this.CreateGraphics())
						{
							g.FillRectangle(SystemBrushes.Window, lvArea);
							g.DrawString(CompileResultMessage, this.Font, SystemBrushes.ControlText, lvArea, sf);
						}
					}					
				}				
			}
			else if(m.Msg == WM_ERASEBKGND)
			{
				if(this.View == View.Details && this.Columns.Count > 0)
				{
					// auto-resize the last column to fit to the width of the control
					this.Columns[this.Columns.Count - 1].Width = -1;
				}
			}
		}
	}
}
