#!/usr/bin/env python

import sys, time, hashlib

class Naive:
    """ Naive implementation of find/replace for a massive word list. """
    def __init__(self, filename):
        self._words = {}
        fp = open(filename)
        for line in fp:
            word, replacement = line.strip().split()
            self._words[word] = replacement
        fp.close()

    def replace_all(self, filename):
        fp = open(filename)
        text = fp.read()
        fp.close()

        for word in self._words.keys():
            text = text.replace(word + ' ', self._words[word] + ' ')
        return text

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: python %s <find-replace-list> <file1> <file2>" % sys.argv[0]
        sys.exit(1)

    n = Naive(sys.argv[1])
    for filename in sys.argv[2:]:
        start = time.time()
        print filename, hashlib.md5(n.replace_all(filename)).hexdigest(), time.time() - start
        #print n.replace_all(filename)
