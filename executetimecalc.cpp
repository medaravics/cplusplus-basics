
#include <iostream>
#include <chrono>
 
int main() {
    // ... your 1000 lines of code to read buffer into structure ...
 
    auto start = std::chrono::high_resolution_clock::now();
    int x=61000, k;
    for (int i=0; i<x; i++) {
        printf("");
    }
    auto stop = std::chrono::high_resolution_clock::now();
 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "Time taken by operations: " << duration.count() << " microseconds" << std::endl;
}