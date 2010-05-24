// Copyright (c) DevInstinct Inc. All rights reserved.
// http://devinstinct.com
// Code author: Martin Lapierre
// You cannot remove this copyright and notice.
// This source file is part of the DevInstinct Application Framework.
// You can redistribute the DevInstinct Application Framework in compiled, obfuscated form.
// You cannot redistribute its source files without the written consent of DevInstinct Inc.
// You can freely use and redistribute THIS source file: SingletonException.cs

using System;
using System.Runtime.Serialization;

namespace DevInstinct.Patterns
{
   /// <summary>
   /// Represents errors that occur while creating a singleton.
   /// </summary>
   /// <remarks>
   /// http://msdn.microsoft.com/en-us/library/ms229064(VS.80).aspx
   /// </remarks>
   [Serializable]
   public class SingletonException
      : Exception
   {
      /// <summary>
      /// Initializes a new instance.
      /// </summary>
      public SingletonException()
      {
      }

      /// <summary>
      /// Initializes a new instance with a specified error message.
      /// </summary>
      /// <param name="message">The message that describes the error.</param>
      public SingletonException(string message)
         : base(message)
      {
      }

      /// <summary>
      /// Initializes a new instance with a reference to the inner 
      /// exception that is the cause of this exception.
      /// </summary>
      /// <param name="innerException">
      /// The exception that is the cause of the current exception, 
      /// or a null reference if no inner exception is specified.
      /// </param>
      public SingletonException(Exception innerException)
         : base(null, innerException)
      {
      }

      /// <summary>
      /// Initializes a new instance with a specified error message and a 
      /// reference to the inner exception that is the cause of this exception.
      /// </summary>
      /// <param name="message">The message that describes the error.</param>
      /// <param name="innerException">
      /// The exception that is the cause of the current exception, 
      /// or a null reference if no inner exception is specified.
      /// </param>
      public SingletonException(string message, Exception innerException)
         : base(message, innerException)
      {
      }

#if !WindowsCE
      /// <summary>
      /// Initializes a new instance with serialized data.
      /// </summary>
      /// <param name="info">
      /// The <see cref="System.Runtime.Serialization.SerializationInfo"/> that holds the 
      /// serialized object data about the exception being thrown.
      /// </param>
      /// <param name="context">
      /// The <see cref="System.Runtime.Serialization.StreamingContext"/> that contains 
      /// contextual information about the source or destination.
      /// </param>
      /// <exception cref="System.ArgumentNullException">The info parameter is null.</exception>
      /// <exception cref="System.Runtime.Serialization.SerializationException">The class name is null or System.Exception.HResult is zero (0).</exception>
      protected SingletonException(SerializationInfo info, StreamingContext context)
         : base(info, context)
      {
      }
#endif
   }
}
