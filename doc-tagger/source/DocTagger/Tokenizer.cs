using System;
using System.Collections;
using System.Text.RegularExpressions;

namespace DocTagger
{
	class Token
	{
		string m_word;
		string m_pos;

		public Token(string word, string pos)
		{
			m_word = word;
			m_pos = pos;
		}

		public string Word
		{
			get { return m_word; }
		}

		public string POS
		{
			get { return m_pos; }
		}
	}

	class Tokenizer
	{
		#region Member Vars
		static Tokenizer _instance = new Tokenizer();
		Regex m_splitter = null;
		Regex m_trunc = null;
		#endregion

		public static Tokenizer Instance
		{
			get { return _instance; }
		}

		private Tokenizer()
		{
			m_splitter = new Regex(@"([ \t{}():;._,\-! \""?\n])"); // TODO: Add more cases
			m_trunc = new Regex(@"([.,;])");
		}

		public Token[] Tokenize(string input)
		{
			ArrayList tokens = new ArrayList();

			//input = NormalizeCasing(input);
			input = m_trunc.Replace(input, string.Empty);
			string[] rawTokens = m_splitter.Split(input);

			for (int i = 0; i < rawTokens.Length; i++)
			{
				// Break up brill tags
				string []twoKons = rawTokens[i].Split('/');
				if (twoKons.Length == 2) // TODO: Fix this so that / is allowed in text (?)
				{
					//m_trunc.Replace(twoKons[0], "");
					if (!StopWordsHandler.Instance.IsStopWord(twoKons[0]))
					{
						Token t = new Token(twoKons[0], twoKons[1]);
						tokens.Add(t);
					}
				}
			}

			return (Token[]) tokens.ToArray(typeof(Token));
		}
	}
}
