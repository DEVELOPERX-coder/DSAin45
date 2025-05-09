/*
 * BitManipulation.cpp
 * Day 40 of #DSAin45 - Comprehensive Bit Manipulation Implementation
 *
 * This file demonstrates practical bit manipulation techniques in C++ with
 * real-world examples, benchmarks, and common interview problem solutions.
 */

#include <iostream>
#include <vector>
#include <bitset>
#include <chrono>
#include <functional>
#include <random>
#include <iomanip>
#include <cmath>
#include <unordered_set>

// Utility function to print a number in binary format
void printBinary(int num, int bits = 8)
{
    std::cout << std::bitset<32>(num).to_string().substr(32 - bits) << std::endl;
}

// Utility function to get elapsed time
template <typename TimePoint>
std::chrono::microseconds getElapsedMicroseconds(TimePoint start, TimePoint end)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// Basic Bit Operations
namespace BitOps
{
    // Check if nth bit is set
    bool isBitSet(int num, int pos)
    {
        return (num & (1 << pos)) != 0;
    }

    // Set the nth bit
    int setBit(int num, int pos)
    {
        return num | (1 << pos);
    }

    // Clear the nth bit
    int clearBit(int num, int pos)
    {
        return num & ~(1 << pos);
    }

    // Toggle the nth bit
    int toggleBit(int num, int pos)
    {
        return num ^ (1 << pos);
    }

    // Clear all bits from MSB to position (inclusive)
    int clearMSBToPos(int num, int pos)
    {
        // Create a mask with all 1's up to position pos
        return num & ((1 << pos) - 1);
    }

    // Clear all bits from position to LSB (inclusive)
    int clearPosToLSB(int num, int pos)
    {
        // Create a mask with all 1's from position pos to MSB
        return num & (~0 << (pos + 1));
    }

    // Update the nth bit to a given value (0 or 1)
    int updateBit(int num, int pos, bool value)
    {
        // First clear the bit at position
        int cleared = clearBit(num, pos);
        // Then set it to the desired value
        return cleared | ((value ? 1 : 0) << pos);
    }
}

// Common Bit Tricks
namespace BitTricks
{
    // Check if a number is power of 2
    bool isPowerOfTwo(int num)
    {
        return num > 0 && (num & (num - 1)) == 0;
    }

    // Count set bits (Brian Kernighan's algorithm)
    int countSetBits(int num)
    {
        int count = 0;
        while (num)
        {
            num &= (num - 1); // Clear the least significant set bit
            count++;
        }
        return count;
    }

    // Count set bits using lookup table (faster for multiple queries)
    int countSetBitsTable(int num)
    {
        static const int BIT_COUNT_TABLE[256] = {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

        return BIT_COUNT_TABLE[num & 0xff] +
               BIT_COUNT_TABLE[(num >> 8) & 0xff] +
               BIT_COUNT_TABLE[(num >> 16) & 0xff] +
               BIT_COUNT_TABLE[(num >> 24) & 0xff];
    }

    // Get the position of the rightmost set bit (0-indexed)
    int getRightmostSetBitPos(int num)
    {
        if (num == 0)
            return -1; // No set bit

        // Isolate the rightmost set bit
        int rightmost = num & -num;

        // Find position using log2
        return log2(rightmost);
    }

    // Swap two numbers without using a temporary variable
    void swapWithoutTemp(int &a, int &b)
    {
        a = a ^ b;
        b = a ^ b;
        a = a ^ b;
    }

    // Check if a number has alternating bits
    bool hasAlternatingBits(int num)
    {
        int xor_result = num ^ (num >> 1);
        return (xor_result & (xor_result + 1)) == 0;
    }

    // Get the next power of 2 greater than or equal to n
    int nextPowerOf2(int n)
    {
        if (n <= 0)
            return 1;
        if (isPowerOfTwo(n))
            return n;

        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return n + 1;
    }
}

// Real-world applications
namespace Applications
{

    // Application 1: Bit Flags/Options
    class FilePermissions
    {
    private:
        const static int READ = 4;    // 100 in binary
        const static int WRITE = 2;   // 010 in binary
        const static int EXECUTE = 1; // 001 in binary
        int permissions;

    public:
        FilePermissions() : permissions(0) {}

        void grantRead() { permissions |= READ; }
        void grantWrite() { permissions |= WRITE; }
        void grantExecute() { permissions |= EXECUTE; }

        void revokeRead() { permissions &= ~READ; }
        void revokeWrite() { permissions &= ~WRITE; }
        void revokeExecute() { permissions &= ~EXECUTE; }

        bool canRead() const { return (permissions & READ) != 0; }
        bool canWrite() const { return (permissions & WRITE) != 0; }
        bool canExecute() const { return (permissions & EXECUTE) != 0; }

        int getUnixStyle() const { return permissions; }
    };

    // Application 2: Compact data representation
    struct RGBColor
    {
        static uint32_t packRGB(uint8_t r, uint8_t g, uint8_t b)
        {
            return (static_cast<uint32_t>(r) << 16) |
                   (static_cast<uint32_t>(g) << 8) |
                   static_cast<uint32_t>(b);
        }

        static void unpackRGB(uint32_t color, uint8_t &r, uint8_t &g, uint8_t &b)
        {
            r = (color >> 16) & 0xFF;
            g = (color >> 8) & 0xFF;
            b = color & 0xFF;
        }
    };

    // Application 3: Fast integer operations
    namespace FastOps
    {
        // Fast multiply by power of 2
        int multiplyByPowerOf2(int num, int power)
        {
            return num << power;
        }

        // Fast divide by power of 2
        int divideByPowerOf2(int num, int power)
        {
            return num >> power;
        }

        // Check if number is odd
        bool isOdd(int num)
        {
            return (num & 1) == 1;
        }

        // Fast modulo for power of 2
        int moduloPowerOf2(int num, int divisor)
        {
            // divisor must be a power of 2
            return num & (divisor - 1);
        }
    }
}

// Common Interview Problems
namespace InterviewProblems
{

    // Problem 1: Count Bits (LeetCode #338)
    std::vector<int> countBits(int n)
    {
        std::vector<int> result(n + 1, 0);
        for (int i = 1; i <= n; i++)
        {
            // Using the fact that result[i] = result[i & (i-1)] + 1
            result[i] = result[i & (i - 1)] + 1;
        }
        return result;
    }

    // Problem 2: Single Number (LeetCode #136)
    // Find the element that appears only once in an array where every other
    // element appears exactly twice
    int singleNumber(const std::vector<int> &nums)
    {
        int result = 0;
        for (const auto &num : nums)
        {
            result ^= num; // XOR cancels out pairs
        }
        return result;
    }

    // Problem 3: Single Number III (LeetCode #260)
    // Find two elements that appear only once in an array where all other
    // elements appear exactly twice
    std::vector<int> singleNumberIII(const std::vector<int> &nums)
    {
        // XOR all numbers to get XOR of the two unique numbers
        int xorResult = 0;
        for (int num : nums)
        {
            xorResult ^= num;
        }

        // Find rightmost set bit in the XOR result
        // This bit is different in the two unique numbers
        int rightmostBit = xorResult & -xorResult;

        // Partition numbers into two groups based on the rightmost bit
        // Then XOR each group separately to find the unique numbers
        int num1 = 0, num2 = 0;
        for (int num : nums)
        {
            if (num & rightmostBit)
            {
                num1 ^= num;
            }
            else
            {
                num2 ^= num;
            }
        }

        return {num1, num2};
    }

    // Problem 4: Reverse Bits (LeetCode #190)
    uint32_t reverseBits(uint32_t n)
    {
        uint32_t result = 0;
        for (int i = 0; i < 32; i++)
        {
            // Shift result left and add rightmost bit of n
            result = (result << 1) | (n & 1);
            n >>= 1;
        }
        return result;
    }

    // Problem 5: Bitwise AND of Numbers Range (LeetCode #201)
    int rangeBitwiseAnd(int left, int right)
    {
        // Find common prefix
        int shift = 0;
        while (left < right)
        {
            left >>= 1;
            right >>= 1;
            shift++;
        }
        return left << shift;
    }
}

// Benchmark functions
void runBenchmarks()
{
    std::cout << "\n=== BENCHMARKING BIT OPERATIONS ===\n";

    const int NUM_ITERATIONS = 10000000;
    const int TEST_NUMBER = 0x12345678;

    // Generate random numbers for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 0x7FFFFFFF);

    std::vector<int> testNumbers(1000);
    for (auto &num : testNumbers)
    {
        num = dis(gen);
    }

    // Benchmark different bit counting algorithms
    {
        int result = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            result += BitTricks::countSetBits(testNumbers[i % testNumbers.size()]);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto kernighan_time = getElapsedMicroseconds(start, end);

        std::cout << "Brian Kernighan's Algorithm: "
                  << kernighan_time.count() << " microseconds (Result: " << result << ")\n";

        result = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            result += BitTricks::countSetBitsTable(testNumbers[i % testNumbers.size()]);
        }
        end = std::chrono::high_resolution_clock::now();

        auto lookup_time = getElapsedMicroseconds(start, end);

        std::cout << "Lookup Table Algorithm: "
                  << lookup_time.count() << " microseconds (Result: " << result << ")\n";

        result = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            result += std::bitset<32>(testNumbers[i % testNumbers.size()]).count();
        }
        end = std::chrono::high_resolution_clock::now();

        auto bitset_time = getElapsedMicroseconds(start, end);

        std::cout << "std::bitset::count(): "
                  << bitset_time.count() << " microseconds (Result: " << result << ")\n";

        std::cout << "Speedup Lookup vs Kernighan: "
                  << std::fixed << std::setprecision(2)
                  << static_cast<double>(kernighan_time.count()) / lookup_time.count() << "x\n";

        std::cout << "Speedup Lookup vs std::bitset: "
                  << std::fixed << std::setprecision(2)
                  << static_cast<double>(bitset_time.count()) / lookup_time.count() << "x\n";
    }

    std::cout << "\n=== BENCHMARKING MODULO OPERATIONS ===\n";

    // Benchmark modulo operations
    {
        int result = 0;
        const int DIVISOR = 16; // A power of 2

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            result += testNumbers[i % testNumbers.size()] % DIVISOR;
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto normal_mod_time = getElapsedMicroseconds(start, end);

        std::cout << "Normal Modulo: "
                  << normal_mod_time.count() << " microseconds (Result: " << result << ")\n";

        result = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            result += Applications::FastOps::moduloPowerOf2(
                testNumbers[i % testNumbers.size()], DIVISOR);
        }
        end = std::chrono::high_resolution_clock::now();

        auto bit_mod_time = getElapsedMicroseconds(start, end);

        std::cout << "Bit Trick Modulo: "
                  << bit_mod_time.count() << " microseconds (Result: " << result << ")\n";

        std::cout << "Speedup: "
                  << std::fixed << std::setprecision(2)
                  << static_cast<double>(normal_mod_time.count()) / bit_mod_time.count() << "x\n";
    }
}

// Demos for bit manipulation
void runDemos()
{
    std::cout << "\n=== BIT MANIPULATION DEMOS ===\n";

    // Basic operations demo
    {
        std::cout << "\n--- Basic Bit Operations ---\n";

        int num = 73; // 01001001 in binary
        std::cout << "Original number: " << num << " (binary: ";
        printBinary(num, 8);
        std::cout << ")\n";

        std::cout << "Check if bit 3 is set: "
                  << (BitOps::isBitSet(num, 3) ? "Yes" : "No") << "\n";

        int withBit6Set = BitOps::setBit(num, 6);
        std::cout << "After setting bit 6: " << withBit6Set << " (binary: ";
        printBinary(withBit6Set, 8);
        std::cout << ")\n";

        int withBit3Cleared = BitOps::clearBit(num, 3);
        std::cout << "After clearing bit 3: " << withBit3Cleared << " (binary: ";
        printBinary(withBit3Cleared, 8);
        std::cout << ")\n";

        int withBit0Toggled = BitOps::toggleBit(num, 0);
        std::cout << "After toggling bit 0: " << withBit0Toggled << " (binary: ";
        printBinary(withBit0Toggled, 8);
        std::cout << ")\n";
    }

    // Common bit tricks demo
    {
        std::cout << "\n--- Common Bit Tricks ---\n";

        // Power of 2 check
        std::vector<int> nums = {1, 2, 4, 7, 8, 16, 31, 32, 64};
        std::cout << "Powers of 2 check:\n";
        for (int num : nums)
        {
            std::cout << num << " is "
                      << (BitTricks::isPowerOfTwo(num) ? "" : "not ")
                      << "a power of 2\n";
        }

        // Counting bits
        int num = 0xABC; // 101010111100 in binary
        std::cout << "\nNumber 0x" << std::hex << num << std::dec << " has "
                  << BitTricks::countSetBits(num) << " set bits\n";

        // Rightmost set bit
        std::cout << "The rightmost set bit is at position "
                  << BitTricks::getRightmostSetBitPos(num) << "\n";

        // Swapping
        int a = 5, b = 7;
        std::cout << "\nBefore swap: a = " << a << ", b = " << b << "\n";
        BitTricks::swapWithoutTemp(a, b);
        std::cout << "After swap: a = " << a << ", b = " << b << "\n";

        // Next power of 2
        std::vector<int> testNums = {5, 10, 16, 33, 64, 100};
        std::cout << "\nNext power of 2:\n";
        for (int n : testNums)
        {
            std::cout << "Next power of 2 >= " << n << " is "
                      << BitTricks::nextPowerOf2(n) << "\n";
        }
    }

    // Applications demo
    {
        std::cout << "\n--- Real-world Applications ---\n";

        // File permissions
        std::cout << "File Permissions Example:\n";
        Applications::FilePermissions perm;
        perm.grantRead();
        perm.grantWrite();

        std::cout << "Permissions: "
                  << (perm.canRead() ? "r" : "-")
                  << (perm.canWrite() ? "w" : "-")
                  << (perm.canExecute() ? "x" : "-")
                  << " (" << perm.getUnixStyle() << ")\n";

        perm.grantExecute();
        std::cout << "After granting execute: "
                  << (perm.canRead() ? "r" : "-")
                  << (perm.canWrite() ? "w" : "-")
                  << (perm.canExecute() ? "x" : "-")
                  << " (" << perm.getUnixStyle() << ")\n";

        perm.revokeWrite();
        std::cout << "After revoking write: "
                  << (perm.canRead() ? "r" : "-")
                  << (perm.canWrite() ? "w" : "-")
                  << (perm.canExecute() ? "x" : "-")
                  << " (" << perm.getUnixStyle() << ")\n";

        // RGB color packing
        std::cout << "\nRGB Color Packing Example:\n";
        uint8_t r = 255, g = 128, b = 64;
        uint32_t packed = Applications::RGBColor::packRGB(r, g, b);

        std::cout << "Original: R=" << static_cast<int>(r)
                  << ", G=" << static_cast<int>(g)
                  << ", B=" << static_cast<int>(b) << "\n";

        std::cout << "Packed: 0x" << std::hex << packed << std::dec << "\n";

        uint8_t r2, g2, b2;
        Applications::RGBColor::unpackRGB(packed, r2, g2, b2);

        std::cout << "Unpacked: R=" << static_cast<int>(r2)
                  << ", G=" << static_cast<int>(g2)
                  << ", B=" << static_cast<int>(b2) << "\n";

        // Fast operations
        std::cout << "\nFast Operations Example:\n";
        int testNum = 15;
        std::cout << testNum << " * 4 = "
                  << Applications::FastOps::multiplyByPowerOf2(testNum, 2) << "\n";

        testNum = 32;
        std::cout << testNum << " / 8 = "
                  << Applications::FastOps::divideByPowerOf2(testNum, 3) << "\n";

        std::cout << "17 % 8 (normal) = " << 17 % 8 << "\n";
        std::cout << "17 % 8 (bit trick) = "
                  << Applications::FastOps::moduloPowerOf2(17, 8) << "\n";
    }

    // Interview Problems demo
    {
        std::cout << "\n--- Interview Problems ---\n";

        // Count Bits
        std::cout << "Count Bits Problem:\n";
        std::vector<int> bitCounts = InterviewProblems::countBits(5);
        std::cout << "Number of bits in 0-5: ";
        for (int count : bitCounts)
        {
            std::cout << count << " ";
        }
        std::cout << "\n";

        // Single Number
        std::cout << "\nSingle Number Problem:\n";
        std::vector<int> nums1 = {4, 1, 2, 1, 2};
        std::cout << "Array: [4, 1, 2, 1, 2]\n";
        std::cout << "Single number: " << InterviewProblems::singleNumber(nums1) << "\n";

        // Single Number III
        std::cout << "\nSingle Number III Problem:\n";
        std::vector<int> nums2 = {1, 2, 1, 3, 2, 5};
        std::cout << "Array: [1, 2, 1, 3, 2, 5]\n";
        std::vector<int> result = InterviewProblems::singleNumberIII(nums2);
        std::cout << "Two single numbers: " << result[0] << " and " << result[1] << "\n";

        // Reverse Bits
        std::cout << "\nReverse Bits Problem:\n";
        uint32_t num = 0x12345678;
        uint32_t reversed = InterviewProblems::reverseBits(num);
        std::cout << "Original: 0x" << std::hex << num << "\n";
        std::cout << "Reversed: 0x" << reversed << std::dec << "\n";
    }
}

// Main function
int main()
{
    std::cout << "===================================\n";
    std::cout << "Day 40: Bit Manipulation Techniques\n";
    std::cout << "===================================\n";

    // Run demonstrations
    runDemos();

    // Run benchmarks
    runBenchmarks();

    std::cout << "\nAll demonstrations and benchmarks completed successfully.\n";
    return 0;
}