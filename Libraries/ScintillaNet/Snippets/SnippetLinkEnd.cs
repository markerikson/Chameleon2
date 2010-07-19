using System;
using System.Drawing;
using System.Collections.Generic;
using System.Text;

using System.Windows.Forms;

namespace ScintillaNet
{
	public class SnippetLinkEnd : ManagedRange
	{
		internal SnippetLinkEnd(int start, Scintilla scintilla) : base(start, start, scintilla) { }

		public override void Change(int newStart, int newEnd)
		{
			Invalidate();

			//	This actually changes Start and End
			base.Change(newStart, newEnd);
		}

		public void Invalidate()
		{
			if(Scintilla != null && Start > 0)
			{
				INativeScintilla _ns = Scintilla as INativeScintilla;
				int x = _ns.PointXFromPosition(Start);
				int y = _ns.PointYFromPosition(Start) + _ns.TextHeight(0) - 2;

				//	Invalidate the old Marker Location so that we don't get "Ghosts"
				Scintilla.Invalidate(new Rectangle(x-2, y, 5, 5));
			}
		}

		//	Drop Markers are points, not a spanned range.
		public override bool IsPoint
		{
			get
			{
				return true;
			}
		}

		protected internal override void Paint(Graphics g)
		{
			base.Paint(g);

			if (IsDisposed)
				return;

			INativeScintilla _ns = Scintilla as INativeScintilla;

			int x = _ns.PointXFromPosition(Start);
			int y = _ns.PointYFromPosition(Start) + _ns.TextHeight(0) - 2;

			//	Draw a red Triangle with a dark red border at the marker position
			g.FillPolygon(Brushes.Lime, new Point[] { new Point(x-2, y+4), new Point(x, y), new Point(x+2, y+4) });
			g.DrawPolygon(Pens.Green, new Point[] { new Point(x-2, y+4), new Point(x, y), new Point(x+2, y+4) });
		}

		public override void Dispose()
		{
			if (!IsDisposed)
			{
				Invalidate();
				base.Dispose();
			}
		}
	}
}
