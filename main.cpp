#include <iostream>
#include <cstdlib>  // for malloc
using namespace std;

int globalVar = 100;
static int staticVar = 200;

int uninitGlobal;
int uninitGlobal2;

// Stack check function: receives address from caller (parent frame)
// and compares with a local variable (child frame)
void checkStack(int* parentAddr) {
    int childVar = 0;
    cout << "--- STACK SEGMENT (Cross-function comparison) ---" << endl;
    cout << (void*)parentAddr << endl;
    cout << (void*)&parentAddr << endl;
    cout << (void*)&childVar << endl;
    cout << endl;
}

int main() {

    int myVar = 10;

    char* p1 = (char*)malloc(1024);
    char* p2 = (char*)malloc(1024);

    cout << "=== MEMORY SEGMENT BOUNDARIES ===" << endl;
    cout << endl;

    cout << "--- TEXT SEGMENT (Code) ---" << endl;
    cout << (void*)&main << endl;
    cout << (void*)&checkStack << endl;
    cout << endl;

    cout << "--- DATA SEGMENT (Initialized Globals) ---" << endl;
    cout << (void*)&globalVar << " " << globalVar << endl;
    cout << (void*)&staticVar << " " << staticVar << endl;
    cout << endl;

    cout << "--- BSS SEGMENT (Uninitialized Globals) ---" << endl;
    cout << (void*)&uninitGlobal << " " << uninitGlobal << endl;
    cout << (void*)&uninitGlobal2 << " " << uninitGlobal2 << endl;
    cout << endl;

    checkStack(&myVar);

    cout << "--- HEAP SEGMENT (Dynamic Allocation) ---" << endl;
    cout << (void*)p1 << endl;
    cout << (void*)p2 << endl;
    cout << "Heap grows: " << (p1 < p2 ? "UP" : "DOWN") << endl;
    cout << endl;

    cout << "=== RELATIVE POSITION SUMMARY ===" << endl;
    unsigned long long textAddr = reinterpret_cast<unsigned long long>(&main);
    unsigned long long dataAddr = reinterpret_cast<unsigned long long>(&globalVar);
    unsigned long long bssAddr = reinterpret_cast<unsigned long long>(&uninitGlobal);
    unsigned long long heapAddr = reinterpret_cast<unsigned long long>(p1);
    unsigned long long stackAddr = reinterpret_cast<unsigned long long>(&myVar);

    cout << "Text vs Data: " << (textAddr < dataAddr ? "TEXT below DATA" : "TEXT above DATA") << endl;
    cout << "Data vs BSS: " << (dataAddr < bssAddr ? "DATA below BSS" : "DATA above BSS") << endl;
    cout << "BSS vs Heap: " << (bssAddr < heapAddr ? "BSS below HEAP" : "BSS above HEAP") << endl;
    cout << "Heap vs Stack: " << (heapAddr < stackAddr ? "HEAP below STACK" : "HEAP above STACK") << endl;
    cout << "Gap (stack - heap): " << (stackAddr > heapAddr ? stackAddr - heapAddr : heapAddr - stackAddr) << " bytes" << endl;

    free(p1);
    free(p2);

    return 0;
}

/*
 * EXPERIMENTAL RESULTS:
 * TODO: After running your program, explain what you observed:
 
 * - Which segment has the lowest addresses?
 Lowest addresses are in the TEXT segment.

 * - Which has the highest?
  Highest addresses are in the DATA segment

 * - Does stack grow down? How did you verify this?
Stack grows downward as verified by comparing addresses:
0x5ffe94 // address of myVar
0x5ffe70 // address of parentAddr
0x5ffe5c // address of childVar
Addresses are decreasing (9>7>5)

 * - Does heap grow up? How did you verify this?
 Heap grows up as verified by comparing addresses:
0x10cfd0
0x10d400
The second address is higher than the first: c<d
 * - What is the gap between HEAP and STACK?
 5189316 bytes
 */
