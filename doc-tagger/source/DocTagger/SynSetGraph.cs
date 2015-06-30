using System;
using System.Collections;
using System.Text;

namespace DocTagger
{
	class SynSetGraph
	{
		Token[] m_tokens = null;
		Hashtable m_htMap = null;
		int m_maxScore = 1;
		string m_strMax = string.Empty;

		public SynSetGraph(Token[] tokens)
		{
			Wnlib.WNCommon.path = "C:\\Program Files\\WordNet\\2.1\\dict\\";
			m_tokens = tokens;
			m_htMap = new Hashtable(1024);
		}

		public int FindSynSets()
		{
			foreach (Token t in m_tokens)
			{
				if (t.Word.Trim().Length < 2)
					continue;
				//WnLexicon.WordInfo wordinfo = WnLexicon.Lexicon.FindWordInfo(t.Word, false); // TODO: Include morphs?
				
				//Wnlib.SynSet synset = new Wnlib.SynSet(0, , t.Word, Wnlib.Search.ALLSENSES, 0);
				//int []i = wordinfo.senseCounts;
				//Stemmer stemmer = new Stemmer(t.Word.ToLower());
				//Wnlib.Index idx = Wnlib.Index.lookup(stemmer.StemPlural(), Wnlib.PartOfSpeech.of("noun"));
				//Wnlib.SynSet[] syns = idx.syns;
				Wnlib.PartsOfSpeech pos = Wnlib.PartsOfSpeech.Unknown;
				if (t.POS.Contains("NN"))
				{
					pos = Wnlib.PartsOfSpeech.Noun;
				}
				else if (t.POS.Contains("VB"))
				{
					pos = Wnlib.PartsOfSpeech.Verb;
				}
				else if (t.POS.Contains("JJ"))
				{
					pos = Wnlib.PartsOfSpeech.Adj;
				}
				//string[] st = WnLexicon.Lexicon.FindSynonyms(t.Word, pos, false);
				if (pos == Wnlib.PartsOfSpeech.Unknown)
					continue;

				Wnlib.Index index = Wnlib.Index.lookup(t.Word, Wnlib.PartOfSpeech.of(pos));
				// Word not found. Stem it and search again
				if (index == null)
				{
					string newWord = string.Empty;
					if (t.Word.EndsWith("s"))
					{
						char[] trimChars = { 's' };
						newWord = t.Word.TrimEnd(trimChars);
					}
					if (t.Word[0] < 'Z')
					{
						newWord = newWord.ToLower();
					}
					index = Wnlib.Index.lookup(newWord, Wnlib.PartOfSpeech.of(pos));
				}
				if (index != null)
				{
					// Get synset
					Wnlib.SynSet synset = new Wnlib.SynSet(index.offs[0], index.pos, index.wd, null, 0);

					// Update this entry in hashmap
					UpdateHashMap(synset.hereiam, 3);
					
					// Find hypernyms to level 2 and add them to the hashmap
					FindHypernyms(synset, 2);
				}
			}

			return m_htMap.Count;
		}

		private void FindHypernyms(Wnlib.SynSet synset, int depth)
		{
			for (int i = 0; i < synset.ptrs.Length; i++)
			{
				if (synset.ptrs[i].ptp.ident == 2) // Hyper Pointer
				{
					Wnlib.SynSet parentSynSet = new Wnlib.SynSet(synset.ptrs[i].off, synset.pos, synset);
					UpdateHashMap(parentSynSet.hereiam, depth);

					depth--;
					if (depth != 0)
					{
						FindHypernyms(parentSynSet, depth);
					}
				}
			}
		}

		private void FindMeroHolonyms(Wnlib.SynSet synset)
		{
			for (int i = 0; i < synset.ptrs.Length; i++)
			{
				if (synset.ptrs[i].ptp.ident == 12 ||
					synset.ptrs[i].ptp.ident == 13)
				{
					Wnlib.SynSet daSynSet = new Wnlib.SynSet(synset.ptrs[i].off, synset.pos, synset);

				}
			}
		}

		private void UpdateHashMap(int synId, int add)
		{
			// Add/Update in hashmap
			int score = 1;
			if (m_htMap.ContainsKey(synId))
			{
				score = (int)m_htMap[synId] + add;
				m_htMap.Remove(synId);
				if (score > m_maxScore)
				{
					m_maxScore = score;
				}
			}
			m_htMap.Add(synId, score);
		}

		public string GetTags()
		{
			StringBuilder sb = new StringBuilder();
			foreach (object key in m_htMap.Keys)
			{
				int synId = (int)key;
				int score = (int)m_htMap[synId];
				if (score == m_maxScore)
				{
					Wnlib.SynSet synset = null;
					try
					{
						synset = new Wnlib.SynSet(synId, Wnlib.PartOfSpeech.of("noun"), string.Empty, null, 0);
					}
					catch
					{
						continue;
					}
					sb.Append("<li>{ ");
					for (int i=0; i<synset.words.Length; i++)
					{	
						sb.Append(synset.words[i].word + ", ");
					}
					sb.Append(" }</li>");
				}
			}

			return sb.ToString();
		}

		public string GetLesserTags()
		{
			StringBuilder sb = new StringBuilder();
			foreach (object key in m_htMap.Keys)
			{
				int synId = (int)key;
				int score = (int)m_htMap[synId];
				if (score > 1 && score != m_maxScore)
				{
					Wnlib.SynSet synset = null;
					try
					{
						synset = new Wnlib.SynSet(synId, Wnlib.PartOfSpeech.of("noun"), string.Empty, null, 0);
					}
					catch
					{
						continue;
					}
					if (synset == null)
						continue;
					sb.Append("<li>{ ");
					for (int i = 0; i < synset.words.Length; i++)
					{
						sb.Append(synset.words[i].word + ", ");
					}
					sb.Append(" } [" + score + "]</li>");
				}
			}

			return sb.ToString();
		}
	}
}
