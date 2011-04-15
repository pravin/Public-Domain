#!/usr/bin/env python
#
# Pravin Paratey (April 15, 2011)
# Code released under BSD license

class PriorityQueue:
    """ This class illustrates a PriorityQueue and its associated functions """

    def __init__(self):
        self.heap = [
            (15, 'http://google.co.uk'), 
            (13, 'http://yahoo.co.uk'),
            (9, 'http://insanitybegins.com')
            ]

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
        (current node) does not.
        """
        left_index = self.left_child(index)
        right_index = self.right_child(index)

        largest = index
        if left_index < len(self.heap) and self.heap[left_index][0] > self.heap[index][0]:
            largest = left_index
        if right_index < len(self.heap) and self.heap[right_index][0] > self.heap[largest][0]:
            largest = right_index

        if largest != index:
            self.heap[index], self.heap[largest] = self.heap[largest], self.heap[index]
            self.max_heapify(largest)

    def build_max_heap(self):
        """
        Responsible for building the heap bottom up. It starts with the lowest
        non-leaf nodes and calls heapify on them
        """
        for i in xrange(len(self.heap)/2, -1, -1):
            self.max_heapify(i)

    def add(self, obj):
        """ Adds an element in the heap """
        value, key = obj
        self.heap.append(obj)
        i = len(self.heap) - 1
        while i != 0 and self.heap[self.parent(i)][0] < self.heap[i][0]:
            self.heap[i], self.heap[self.parent(i)] = self.heap[self.parent(i)], self.heap[i]
            i = self.parent(i)

    def extract_max(self):
        """
        Part of the Priority Queue, extracts the element on the top of the heap
        and then re-heapifies
        """
        max = self.heap[0]
        data = self.heap.pop()
        if len(self.heap) > 0:
            self.heap[0] = data
            self.max_heapify(0)
        return max

    def increment(self, key, value):
        """
        Increments key by value
        """
        for i in xrange(len(self.heap)):
            if self.heap[i][1] == key:
                self.heap[i] = (value + self.heap[i][0], key)
                # Key increased, heapify parent
                while i != 0 and self.heap[self.parent(i)] < self.heap[i]:
                    self.heap[i], self.heap[self.parent(i)] = self.heap[self.parent(i)], self.heap[i]
                    i = self.parent(i)
                break

    def heap_sort(self):
        """
        The heap-sort algorithm with a time complexity O(n log n)
        """
        self.build_max_heap()
        output = []
        for i in xrange(len(self.heap)-1, 0, -1):
            self.heap[0], self.heap[i] = self.heap[i], self.heap[0]
            output.append(self.heap.pop())
            self.max_heapify(0)
        output.append(self.heap.pop())
        self.heap = output

if __name__ == '__main__':
    # Create the heap object
    m = PriorityQueue()
    print "Initial heap:", m.heap

    # Add an element to the heap
    m.add((14, 'http://msn.co.uk'))
    print "Heap after addition:", m.heap

    # Update an element in the heap
    m.increment('http://msn.co.uk', 20) # Increment by 20
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
