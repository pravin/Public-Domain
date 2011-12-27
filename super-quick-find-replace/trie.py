#!/usr/bin/env python

import sys, time, hashlib

class Trie:
    """ 
    Trie class

    We use a tuple (string, dict) to represent a node. That is, the words 

    1. ant -> spider (replace every occurrence of ant with spider)
    2. any -> every

    will be represented as,

    (None, {
        a: (None, {
            n: (None, {
                t: ('spider', {}),
                y: ('every', {})
            }),
        })
    })
    """
    def __init__(self, dict_file):
        self._trie = (None, {})
        fp = open(dict_file)
        for line in fp:
            words = line.strip().split()
            self.add(words[0], words[1])
        fp.close()
    
    def add(self, text, replacement, node=None):
        """ Adds word/replacement pair """
        if node == None: node = self._trie

        new_replacement, children, c = None, node[1], text[0]

        # New id is the id for leaf nodes, None otherwise
        if len(text) == 1: new_replacement = replacement

        if c not in children:
            children[c] = (new_replacement, {})
        elif len(text) == 1: # Leaf node
            (old_replacement, old_node) = children[c]
            children[c] = (new_replacement, old_node)
        if len(text) > 1:
            self.add(text[1:], replacement, node[1][c])

    
    def replace_all(self, text):
        output = []
        max_len, start = len(text), 0

        while start < max_len:
            (replacement, cur_node) = self._trie
            end = start
            while end < max_len and cur_node.has_key(text[end]):
                (replacement, cur_node) = cur_node[text[end]]
                end += 1
            
            # If we are not at the beginning of the string, append prev char (whitespace)
            if start != 0: output.append(text[start-1])

            # Replacement found AND end not exceeded max-len AND the word has ended
            if replacement != None and end <= max_len and (end == max_len or not text[end].isalpha()): 
                output.append(replacement)
            else:
                output.append(text[start:end])
            
            start = end + 1
        if end != max_len: output.append(text[end])
        return ''.join(output)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: python %s <find-replace-list> <file1> <file2>" % sys.argv[0]
        sys.exit(1)

    t = Trie(sys.argv[1])
    for filename in sys.argv[2:]:
        start = time.time()
        fp = open(filename)
        data = fp.read()
        fp.close()
        print filename, hashlib.md5(t.replace_all(data)).hexdigest(), time.time() - start
        #print t.replace_all(data)
