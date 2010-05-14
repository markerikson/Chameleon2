using System;
using System.Collections;

namespace Granados
{
	delegate void MenuCallback();

	class Menu
	{
		private class MenuItem
		{
			public MenuCallback mc;
			public string text;

			public MenuItem(string Text, MenuCallback Mc)
			{
				mc = Mc;
				text = Text;
			}
		}

		private ArrayList m_Items = new ArrayList();

		public void Add(string text, MenuCallback mc)
		{
			m_Items.Add(new MenuItem(text, mc));
		}

		public void Show()
		{
			for(int i = 0; i < m_Items.Count; ++i)
			{
				MenuItem mi = m_Items[i] as MenuItem;
				Console.WriteLine(" [{0}] {1}", i + 1, mi.text);
			}

			Console.Write("Selection: ");

			int choosen = Int32.Parse(Console.ReadLine());

			if(choosen > m_Items.Count || choosen < 1)
			{
				Console.WriteLine("Invalid option.");
			}
			else
			{
				MenuItem mi = m_Items[choosen - 1] as MenuItem;
				MenuCallback mc = mi.mc;
				mc();
			}
		}

	}

}