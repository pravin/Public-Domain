#!/usr/bin/env python

import sys, re, time, hashlib

class Regex:
    """ Regex implementation of find/replace for a massive word list. """
    def __init__(self, filename):
        self._words = {}
        fp = open(filename)
        for line in fp:
            word, replacement = line.strip().split()
            self._words[word] = replacement
        fp.close()

    def replace_func(self, matchObj):
        key = matchObj.group(0)
        if self._words.has_key(key):
            return self._words[key]
        else:
            return key

    def replace_all(self, filename):
        fp = open(filename)
        text = fp.read()
        fp.close()
        return re.sub("[a-zA-Z]+", self.replace_func, text)
        

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: python %s <find-replace-list> <file1> <file2>" % sys.argv[0]
        sys.exit(1)

    r = Regex(sys.argv[1])
    for filename in sys.argv[2:]:
        start = time.time()
        print filename, hashlib.md5(r.replace_all(filename)).hexdigest(), time.time() - start
        #print r.replace_all(filename)
