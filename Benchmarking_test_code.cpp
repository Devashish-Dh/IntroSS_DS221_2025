#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>  // getrusage
#include <unistd.h>        // getpid

// Helper to get current memory usage in KB (resident set size)
long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#if defined(__APPLE__) && defined(__MACH__)
    return usage.ru_maxrss / 1024; // macOS reports bytes
#else
    return usage.ru_maxrss;        // Linux reports KB
#endif
}


// Utility: Run a function and measure time + memory
template <typename Func, typename... Args>
void profileFunction(const std::string &name, Func f, Args... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    long mem_before = getMemoryUsageKB();

    f(args...);

    auto end_time = std::chrono::high_resolution_clock::now();
    long mem_after = getMemoryUsageKB();

    double elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();

    std::cout << "Function [" << name << "] took " 
              << elapsed_ms << " ms, "
              << "Memory usage before: " << mem_before << " KB, "
              << "after: " << mem_after << " KB\n";
}


// long getMemoryUsageKB() {
//     struct rusage usage;
//     getrusage(RUSAGE_SELF, &usage);
//     return usage.ru_maxrss; // in kilobytes
// }



// Function 1: CPU-intensive (matrix multiplication)
void matrixMultiply(int n) {
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> B(n, std::vector<double>(n, 2.0));
    std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int 
// Helper to get current memory usage in KB (resident set size)
long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#if defined(__APPLE__) && defined(__MACH__)
    return usage.ru_maxrss / 1024; // macOS reports bytes
#else
    return usage.ru_maxrss;        // Linux reports KB
#endif
}


// long getMemoryUsageKB() {
//     struct rusage usage;
//     getrusage(RUSAGE_SELF, &usage);
//     return usage.ru_maxrss; // in kilobytes
// }

k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function 2: Memory-intensive (large allocation)
void memoryHeavy(int n) {
    std::vector<int> bigArray(n, 42);
    // Touch the array so it’s actually allocated in memory
    long long sum = 0;
    for (int x : bigArray) sum += x;
    std::cout << "Sum: " << sum << "\n";
}


int main() {
    std::cout << "Starting profiling demo (PID: " << getpid() << ")\n";

    profileFunction("matrixMultiply", matrixMultiply, 300);   // Adjust size for runtime
    profileFunction("memoryHeavy", memoryHeavy, 50'000'000);  // Adjust size for memory

    return 0;
}
