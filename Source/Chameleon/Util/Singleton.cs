// Copyright (c) DevInstinct Inc. All rights reserved.
// http://devinstinct.com
// Code author: Martin Lapierre
// You cannot remove this copyright and notice.
// This source file is part of the DevInstinct Application Framework.
// You can redistribute the DevInstinct Application Framework in compiled, obfuscated form.
// You cannot redistribute its source files without the written consent of DevInstinct Inc.
// You can freely use and redistribute THIS source file: Singleton.cs

using System;
using System.Reflection;

namespace DevInstinct.Patterns
{
	/// <summary>
	/// Manages the single instance of a class.
	/// </summary>
	/// <remarks>
	/// Generic variant of the strategy presented here : http://geekswithblogs.net/akraus1/articles/90803.aspx.
	/// Prefered to http://www.yoda.arachsys.com/csharp/singleton.html, where static initialization doesn't allow
	/// proper handling of exceptions, and doesn't allow retrying type initializers initialization later
	/// (once a type initializer fails to initialize in .NET, it can't be re-initialized again).
	/// </remarks>
	/// <typeparam name="T">Type of the singleton class.</typeparam>
	public static class Singleton<T>
		where T : class
	{
		#region Fields

		/// <summary>
		/// The single instance of the target class.
		/// </summary>
		/// <remarks>
		/// The volatile keyword makes sure to remove any compiler optimization that could make concurrent 
		/// threads reach a race condition with the double-checked lock pattern used in the Instance property.
		/// See http://www.bluebytesoftware.com/blog/PermaLink,guid,543d89ad-8d57-4a51-b7c9-a821e3992bf6.aspx
		/// </remarks>
		static volatile T _instance;

		/// <summary>
		/// The dummy object used for locking.
		/// </summary>
		static object _lock = new object();

		#endregion Fields


		#region Constructors

		/// <summary>
		/// Type-initializer to prevent type to be marked with beforefieldinit.
		/// </summary>
		/// <remarks>
		/// This simply makes sure that static fields initialization occurs 
		/// when Instance is called the first time and not before.
		/// </remarks>
		static Singleton()
		{
		}

		#endregion Constructors


		#region Properties

		/// <summary>
		/// Gets the single instance of the class.
		/// </summary>
		public static T Instance
		{
			get
			{
				if (_instance == null)
					lock (_lock)
					{
						if (_instance == null)
						{
							ConstructorInfo constructor = null;

							try
							{
								// Binding flags exclude public constructors.
								constructor = typeof(T).GetConstructor(BindingFlags.Instance | BindingFlags.NonPublic, null, new Type[0], null);
							}
							catch (Exception exception)
							{
								throw new SingletonException(exception);
							}

							if (constructor == null || constructor.IsAssembly) // Also exclude internal constructors.
								throw new SingletonException(string.Format("A private or protected constructor is missing for '{0}'.", typeof(T).Name));

							_instance = (T)constructor.Invoke(null);
						}
					}

				return _instance;
			}
		}

		#endregion Properties
	}
}
