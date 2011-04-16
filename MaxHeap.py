#!/usr/bin/env python
#
# Pravin Paratey (February 22, 2011)
# Code released under BSD license

class MaxHeap:
    """ This class illustrates a Max-Heap and its associated functions """

    def __init__(self):
        """
        We initialize the heap with some values
                     15
                   __/\__
                  /      \
                 13       9
                _/\_      /\
               /    \    /  \
              5     12  8    7
             / \    /\  |
            4   0  6  2 1
        """
        self.heap = [15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1]

    def parent(self, index):
        """
        Parent will be at math.floor(index/2). Since integer division
        simulates the floor function, we don't explicity use it
        """
        return index / 2

    def left_child(self, index):
        return 2 * index + 1

    def right_child(self, index):
        return 2 * index + 2

    def max_heapify(self, index):
        """
        Responsible for maintaining the heap property of the heap.
        This function assumes that the subtree located at left and right
        child satisfies the max-heap property. But the tree at index
        (current node) does not. O(log n)
        """
        left_index = self.left_child(index)
        right_index = self.right_child(index)

        largest = index
        if left_index < len(self.heap) and self.heap[left_index] > self.heap[index]:
            largest = left_index
        if right_index < len(self.heap) and self.heap[right_index] > self.heap[largest]:
            largest = right_index

        if largest != index:
            self.heap[index], self.heap[largest] = self.heap[largest], self.heap[index]
            self.max_heapify(largest)

    def build_max_heap(self):
        """
        Responsible for building the heap bottom up. It starts with the lowest non-leaf nodes
        and calls heapify on them. This function is useful for initialising a heap with an
        unordered array. O(n)
        """
        for i in xrange(len(self.heap)/2, -1, -1):
            self.max_heapify(i)

    def heap_sort(self):
        """ The heap-sort algorithm with a time complexity O(n*log(n)) """
        self.build_max_heap()
        output = []
        for i in xrange(len(self.heap)-1, 0, -1):
            self.heap[0], self.heap[i] = self.heap[i], self.heap[0]
            output.append(self.heap.pop())
            self.max_heapify(0)
        output.append(self.heap.pop())
        self.heap = output

    def propagate_up(self, index):
        """ Compares index with parent and swaps node if larger O(log(n)) """
        while index != 0 and self.heap[self.parent(index)] < self.heap[index]:
            self.heap[index], self.heap[self.parent(index)] = self.heap[self.parent(index)], self.heap[index]
            index = self.parent(index)

    def add(self, key):
        """ Adds an element in the heap O(ln(n)) """
        self.heap.append(key)
        self.propagate_up(len(self.heap) - 1) # Index value is 1 less than length

    def extract_max(self):
        """
        Part of the Priority Queue, extracts the element on the top of the heap and
        then re-heapifies. O(log n)
        """
        max = self.heap[0]
        data = self.heap.pop()
        if len(self.heap) > 0:
            self.heap[0] = data
            self.max_heapify(0)
        return max

    def increment(self, key, value):
        """ Increments key by the input value. O(log n) """
        for i in xrange(len(self.heap)):
            if self.heap[i] == key:
                self.heap[i] += value
                self.propagate_up(i)
                break


if __name__ == '__main__':
    # Create the heap object
    m = MaxHeap()
    print "Initial heap:", m.heap

    # Add an element to the heap
    m.add(14)
    print "Heap after addition:", m.heap

    # Update an element in the heap
    m.increment(14, 20) # Increment by 20
    print "Heap after update:", m.heap

    # Extract the maximum element
    print "Extract maximum:", m.extract_max()
    print "Heap after extract:", m.heap

    # Randomize heap to illustrate build_max_heap
    import random
    random.shuffle(m.heap)
    print "Heap after shuffle:", m.heap
    m.build_max_heap()
    print "Build max heap:", m.heap

    # Randomize heap to illustrate heap_sort
    import random
    random.shuffle(m.heap)
    print "Heap after shuffle:", m.heap
    m.heap_sort()
    print "Heapsort:", m.heap
