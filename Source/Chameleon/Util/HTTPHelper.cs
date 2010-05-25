using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace Piccolo.Common
{
	public class HttpHelper
	{
		private string _baseUrl;
		private CookieContainer _cookieContainer = new CookieContainer();

		public HttpHelper() : this("") { }

		public HttpHelper(string baseUrl)
		{
			_baseUrl = baseUrl;
		}

		public string HttpStringGet(string relativeUrl)
		{
			HttpWebRequest req = (HttpWebRequest)WebRequest.Create(_baseUrl + relativeUrl);
			req.CookieContainer = _cookieContainer;

			return ReadBasicResponse(req.GetResponse());
		}

		public byte[] HttpBinaryGet(string relativeUrl)
		{
			HttpWebRequest req = (HttpWebRequest)WebRequest.Create(_baseUrl + relativeUrl);
			req.CookieContainer = _cookieContainer;

			byte[] result = null;
			byte[] buffer = new byte[4096];

			using(WebResponse resp = req.GetResponse())
			using(Stream responseStream = resp.GetResponseStream())
			using(MemoryStream memoryStream = new MemoryStream())
			{
				int count = 0;
				do
				{
					count = responseStream.Read(buffer, 0, buffer.Length);
					memoryStream.Write(buffer, 0, count);

				} while(count != 0);

				result = memoryStream.ToArray();
			}

			return result;
		}

		private string ReadBasicResponse(WebResponse response)
		{
			using(WebResponse resp = response)
			using(StreamReader sr = new StreamReader(resp.GetResponseStream()))
				return sr.ReadToEnd().Trim();
		}
	}
}