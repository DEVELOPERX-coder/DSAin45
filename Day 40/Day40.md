# Day 40: Bit Manipulation #DSAin45

## Why This Matters

Bit manipulation is essential for:

- Low-level programming and embedded systems
- Optimizing performance-critical code
- Solving specific algorithm problems efficiently
- Understanding how data is represented at the hardware level

## Bitwise Operators in C++

| Operator | Name        | Description                               |
| -------- | ----------- | ----------------------------------------- |
| `&`      | AND         | Set bit if both bits are 1                |
| `\|`     | OR          | Set bit if either bit is 1                |
| `^`      | XOR         | Set bit if bits are different             |
| `~`      | NOT         | Flips all bits                            |
| `<<`     | Left shift  | Shifts bits left, filling with 0s         |
| `>>`     | Right shift | Shifts bits right (logical or arithmetic) |

### Key Points:

- **Arithmetic Right Shift** (`>>`) preserves the sign bit for signed integers
- **Logical Right Shift** fills with 0s (only relevant distinction for signed numbers)
- C++ handles signed integers differently across platforms - be careful

## Common Bit Manipulation Techniques

### 1. Check if bit is set

```cpp
bool isBitSet(int num, int pos) {
    return (num & (1 << pos)) != 0;
}
```

### 2. Set a bit

```cpp
int setBit(int num, int pos) {
    return num | (1 << pos);
}
```

### 3. Clear a bit

```cpp
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}
```

### 4. Toggle a bit

```cpp
int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}
```

### 5. Clear rightmost set bit

```cpp
int clearRightmostSetBit(int num) {
    return num & (num - 1);
}
```

### 6. Check if power of 2

```cpp
bool isPowerOfTwo(int num) {
    return num > 0 && (num & (num - 1)) == 0;
}
```

### 7. Count set bits (Brian Kernighan's Algorithm)

```cpp
int countSetBits(int num) {
    int count = 0;
    while (num) {
        num &= (num - 1);
        count++;
    }
    return count;
}
```

### 8. Get rightmost set bit position

```cpp
int getRightmostSetBitPos(int num) {
    if (num == 0) return -1;
    return log2(num & -num);
}
```

### 9. XOR properties

- `a ^ a = 0` (XOR with self = 0)
- `a ^ 0 = a` (XOR with 0 = self)
- `a ^ b ^ a = b` (XOR is associative and can cancel out)

## Real-World Applications

### 1. Low-Level Device Control

```cpp
// Setting flags in a control register
deviceRegister |= ENABLE_FLAG;  // Enable a device feature
deviceRegister &= ~ERROR_FLAG;  // Clear an error flag
```

### 2. Memory-Efficient Data Structures

```cpp
// Using a 32-bit integer as a bitset for 32 boolean flags
int flags = 0;
flags |= (1 << USER_ADMIN_FLAG);  // Set admin flag
bool isAdmin = (flags & (1 << USER_ADMIN_FLAG)) != 0;  // Check if admin
```

### 3. Fast Arithmetic Operations

```cpp
// Multiply by 2^n
int fastMultiply = num << n;

// Divide by 2^n
int fastDivide = num >> n;
```

### 4. Compact Data Representation

```cpp
// Pack RGB values into a single integer
int color = (r << 16) | (g << 8) | b;

// Extract components
int red = (color >> 16) & 0xFF;
int green = (color >> 8) & 0xFF;
int blue = color & 0xFF;
```

### 5. Cryptographic Operations

XOR is fundamental in many encryption algorithms:

```cpp
// Simple XOR encryption (not secure, just an example)
char encryptedByte = plainTextByte ^ key;
char decryptedByte = encryptedByte ^ key;  // XOR again to decrypt
```

## Time & Space Complexity

Bit operations are O(1) time complexity operations. They don't depend on the size of the input but on the fixed size of the machine word (typically 32 or 64 bits).

The space complexity is also O(1) since we're manipulating fixed-size integers.

## Common Mistakes & Pitfalls

1. **Overflow Issues**: Be careful with shifts that might cause overflow

   ```cpp
   int x = 1 << 31;  // Might overflow on 32-bit integers
   ```

2. **Sign Extension**: Right shifts behave differently for signed vs. unsigned

   ```cpp
   int x = -8 >> 1;   // Might be -4 (arithmetic shift)
   unsigned y = 8 >> 1;  // Always 4 (logical shift)
   ```

3. **Precedence Problems**: Bitwise operators have lower precedence than arithmetic

   ```cpp
   // Wrong:
   int x = 1 << 2 + 3;  // Shifts by 5, not 2

   // Correct:
   int x = 1 << (2 + 3);
   ```

## Practice Problems

### Easy: Count Bits (LeetCode #338)

Write a function that takes an integer and returns an array containing the number of 1's in the binary representation of each number from 0 to n.

**Approach Hint**: Use the property that `countBits(i) = countBits(i & (i-1)) + 1`.

### Medium: Single Number III (LeetCode #260)

Given an array where all numbers appear twice except for two numbers, find those two numbers.

**Approach Hint**: Use XOR to find the XOR of the two unique numbers, then find a bit where they differ.

### Hard: UTF-8 Validation (LeetCode #393)

Determine if a given array of integers represents a valid UTF-8 encoding.

**Approach Hint**: Use bit manipulation to check the pattern of most significant bits according to UTF-8 rules.

## What's Next

Tomorrow we'll cover String Algorithms including pattern matching techniques like KMP and Rabin-Karp, which are essential for text processing and searching.

## Practical Takeaways

- Bit manipulation provides performance optimizations not possible with standard arithmetic
- Master the common bit tricks and think about problems in terms of individual bits
- Use bitwise operations when working with flags, masks, or needing memory-efficient solutions
- Practice visualizing numbers in binary to better understand bit manipulation

#DSAin45 #BitManipulation #CPlusPlus #Algorithms #PerformanceOptimization #Coding
