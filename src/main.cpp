#include "test/tc_compressor.h"
#include "test/tc_heap.h"
#include "test/tc_xmap.h"
#include "../test/HeapTest.h"
#include "../test/xMapTest.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void (*func_ptr[17])() = {
    // hashDemo1,
    // hashDemo2,
    // hashDemo3,
    // hashDemo4,
    // hashDemo5,
    // hashDemo6,
    // hashDemo7,
    // heapDemo1,
    // heapDemo2,
    // heapDemo3,
    // tc_huffman1001,
    // tc_huffman1002,
    // tc_huffman1003,
    // tc_huffman1004,
    // tc_huffman1005,
    tc_compressor1001,
    tc_compressor1002
};

    void run(int func_idx){
        func_ptr[func_idx]();
}

int main(int argc, char **argv) {

    testCustomObjects();
    // for (int i = 0; i < 2; ++i) {
    //     cout << "Running test case tc_compressor100" << i + 1<< "..." << endl;
    //     run(i);
    //     cout << "--------------------------" << endl;
    // }

    return 0;
}