#!/usr/bin/env python

import sys
from random import choice as C
import random

class Generator:
    """ 
    Generates,
    1. Find/Replace list
    2. Text
    """

    @staticmethod
    def cw():
        """ Create a random word """
        return reduce(lambda x,y:x+y,[chr(C(range(97,122))) for x in range(C(range(4,16)))])

    @staticmethod
    def generate():
        text = []
        # Generate 100,000 words
        words = [Generator.cw() for x in xrange(100000)]

        for number in [10, 100, 1000, 10000, 100000]:
            fp = open("%6.6i.find_replace" % number, "w")
            universal_set = random.sample(words, number)
            for w in universal_set:
                fp.write("%s %s\n" % (w, w[::-1])) # Write the find/replace file
            fp.close()

            # Write file with 10% occurrence, 50% occurrence, 100% occurrence
            for x in [0.1, 0.5, 1.0]:
                w1 = random.sample(universal_set, int(number * x))
                w2 = random.sample(universal_set, int(number * (1 - x)))

                fp = open("%6.6i-%i.txt" % (number, x * 100), "w")
                w1.extend([x[::-1] for x in w2])
                random.shuffle(w1)
                fp.write(" ".join(w1) + " ") # Extra space added at the end
                fp.close()

if __name__ == "__main__":
    g = Generator.generate()
