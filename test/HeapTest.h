#ifndef HEAP_TEST_H
#define HEAP_TEST_H
#include "../include/heap/Heap.h"
#include "../include/util/sampleFunc.h"
#include <assert.h>
#include <iostream>
#include <string>

// String converter for integers
string intToString(int &value) {
    return std::to_string(value);
}

void testIntHeap() {
    cout << "\n=== Testing Integer Heap (Max Heap) ===\n";

    // Create a max heap (default)
    Heap<int> maxHeap(maxHeapComparator);

    // Test empty
    assert(maxHeap.empty());
    cout << "Empty check passed\n";

    // Test push and size
    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(15);
    maxHeap.push(30);
    maxHeap.push(5);

    assert(maxHeap.size() == 5);
    cout << "Push and size check passed\n";

    // Test peek
    assert(maxHeap.peek() == 30);
    cout << "Peek check passed\n";

    // Test toString
    cout << "Heap contents: " << maxHeap.toString() << endl;
    cout << "Heap with custom toString: " << maxHeap.toString(intToString) << endl;

    // Test contains
    assert(maxHeap.contains(15));
    assert(!maxHeap.contains(100));
    cout << "Contains check passed\n";

    // Test pop
    assert(maxHeap.pop() == 30);
    assert(maxHeap.pop() == 20);
    assert(maxHeap.size() == 3);
    cout << "Pop check passed\n";

    // Test remove
    maxHeap.remove(15);
    assert(!maxHeap.contains(15));
    assert(maxHeap.size() == 2);
    cout << "Remove check passed\n";

    // Test clear
    maxHeap.clear();
    assert(maxHeap.empty());
    cout << "Clear check passed\n";

    // Test heapify
    int arr[] = {5, 10, 15, 20, 25};
    maxHeap.heapify(arr, 5);
    assert(maxHeap.size() == 5);
    assert(maxHeap.peek() == 25);
    cout << "Heapify check passed\n";

    // Test iterator
    cout << "Iterating through heap: ";
    for (Heap<int>::Iterator it = maxHeap.begin(); it != maxHeap.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Test Min Heap with custom comparator
    Heap<int> minHeap(minHeapComparator);
    minHeap.push(10);
    minHeap.push(20);
    minHeap.push(5);
    minHeap.push(15);

    assert(minHeap.peek() == 5);
    cout << "Min heap with custom comparator check passed\n";
}

void testPointerHeap() {
    cout << "\n=== Testing Pointer Heap ===\n";

    // Create a heap of Point pointers
    Heap<Point *> pointHeap(maxHeapComparator, &Heap<Point *>::free);

    // Test push with pointers
    pointHeap.push(new Point(1, 2));
    pointHeap.push(new Point(5, 6));
    pointHeap.push(new Point(3, 4));
    pointHeap.push(new Point(7, 8));

    assert(pointHeap.size() == 4);
    cout << "Push with pointers check passed\n";

    // Test peek with pointers
    Point *top = pointHeap.peek();
    assert(top->getX() == 7 && top->getY() == 8);
    cout << "Peek with pointers check passed\n";

    // Test toString
    cout << "Point heap contents: " << pointHeap.toString(point2str) << endl;

    // Test contains with pointers
    Point *searchPoint = new Point(3, 4);
    assert(pointHeap.contains(searchPoint));
    delete searchPoint; // Clean up the search point
    cout << "Contains with pointers check passed\n";

    // Test pop with pointers
    Point *popped = pointHeap.pop();
    assert(popped->getX() == 7 && popped->getY() == 8);
    delete popped; // Manually delete since we removed it from heap
    assert(pointHeap.size() == 3);
    cout << "Pop with pointers check passed\n";

    // Let the heap destructor clean up the remaining Points
}

void testHeapsort() {
    cout << "\n=== Testing Heapsort ===\n";

    XArrayList<int> list;

    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    int arrSize = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < arrSize; i++) {
        list.add(arr[i]);
    }

    cout << "Original list: ";
    for (int i = 0; i < list.size(); i++) {
        cout << list.get(i) << " ";
    }
    cout << endl;

    Heap<int> heap(maxHeapComparator);
    heap.heapsort(list);

    cout << "Sorted list: ";
    for (int i = 0; i < list.size(); i++) {
        cout << list.get(i) << " ";
    }
    cout << endl;
}

#endif // HEAP_TEST_H