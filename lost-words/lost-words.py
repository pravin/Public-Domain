#!/usr/bin/env python
"""
You are given a list of words with some letters missing. The missing 
letters are denoted by a "?". You are also given a dictionary 
containing a list of valid words. Your job is to return a list of 
possible words, given a partially formed word.

Ex.

c?t -> cat, cot
c?? -> cat, cot, can, car

"""

class Trie:
	""" 
	Data structure contains

	data = {
		'a': {
			'ENDS': 'True',
			'a': {
				'ENDS': False,
				'b': ...
			}
		}
		'b': {
			...
		}
	}
	"""

	def __init__(self, file):
		""" file: input file with words """
		self.data = {}
		with open(file) as fp:
			for word in fp: 
				self.add_word(word.strip())

	def add_word(self, word):
		""" Add word to the data structure """
		d, l = self.data, len(word)
		for i in xrange(l):
			if not d.has_key(word[i]): d[word[i]] = {'ENDS': False}
			d = d[word[i]]
		d['ENDS'] = True

	def find_matches(self, word, d):
		""" Find matches """
		result, c = [], word[0]

		if len(word) == 1:
			if c == '?': 
				return filter(lambda k: k != 'ENDS' and d[k]['ENDS'], d.keys())
			elif d.has_key(c) and d[c]['ENDS']:
				return [c] 
		else:
			if c == '?': # Pick all children
				for child in d.keys():
					if child != 'ENDS': 
						result.extend([child + n for n in self.find_matches(word[1:], d[child])])
			elif d.has_key(c): # Go one level down
				result = map(lambda x: c + x, self.find_matches(word[1:], d[c]))

		return result

if __name__ == '__main__':
	t = Trie('english-words.20')
	for w in ['al???', 'allege', 'ye?i', '???', 'xyzw']:
		print w, t.find_matches(w, t.data)