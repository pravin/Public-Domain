#!/usr/bin/env python
# Pravin Paratey (April 22, 2011)

import copy

class Diet:
    @staticmethod
    def compute(array):
        neg = 1
        pos = 1
        for n in array:
            if n < 0:
                neg = neg * Diet.get_representation(abs(n))
            else:
                pos = pos * Diet.get_representation(n)
        count = 0
        pos_str = str(pos)
        neg_str = str(neg)
        print pos_str
        print neg_str
        min_len = min(len(pos_str), len(neg_str))
        for i in xrange(1, min_len):
            if pos_str[i] != '0' and neg_str[i] != '0':
                return True
        return False

    @staticmethod
    def get_representation(num):
        """ Returns a representation of a num in bits """
        return 10 ** num + 1


if __name__ == '__main__':
    import random
    array = [random.randint(-500, 500) for x in xrange(50)]
    print array
    import time
    start = time.time()
    print Diet.compute(array)
    end = time.time()
    print "Time taken: ", end - start
