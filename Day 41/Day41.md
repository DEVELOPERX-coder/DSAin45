# Day 41: String Algorithms #DSAin45

## Why This Matters

String algorithms are crucial for:

- Text processing and document analysis
- Web search engines and information retrieval
- Bioinformatics (DNA sequence analysis)
- Compilers and interpreters
- Natural language processing
- Network packet inspection and security

## Pattern Matching Problem

The fundamental pattern matching problem:

- **Input**: Text T of length n, Pattern P of length m
- **Output**: All positions in T where P occurs as a substring

Naive approach: Check every possible position in T for P.

- Time complexity: O((n-m+1) × m) → O(n×m) in worst case
- Works but inefficient for large texts or multiple searches

## Knuth-Morris-Pratt (KMP) Algorithm

KMP avoids redundant comparisons by leveraging previously matched characters.

### Key Insight

When a mismatch occurs after matching several characters, we already know some information about the text that we can use instead of starting over.

### LPS Array (Longest Prefix Suffix)

- For each position i in pattern P, LPS[i] = length of longest proper prefix that is also a suffix of P[0...i]
- This array helps us skip comparisons and avoid backtracking in the text

### Algorithm Steps

1. Preprocess pattern to build LPS array
2. Scan text with pattern, using LPS to decide how far to slide pattern after a mismatch

### LPS Array Construction

```cpp
void computeLPSArray(const string& pattern, vector<int>& lps) {
    int m = pattern.length();
    int len = 0;  // Length of previous longest prefix suffix

    lps[0] = 0;  // LPS of first char is always 0

    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Try to find a shorter prefix that is also a suffix
                len = lps[len-1];
            } else {
                // No proper prefix is also a suffix here
                lps[i] = 0;
                i++;
            }
        }
    }
}
```

### KMP Search

```cpp
vector<int> KMPSearch(const string& text, const string& pattern) {
    vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    if (m == 0) return matches;

    // Preprocess the pattern
    vector<int> lps(m);
    computeLPSArray(pattern, lps);

    int i = 0;  // Index for text
    int j = 0;  // Index for pattern

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            // Found a match
            matches.push_back(i - j);
            // Look for the next match
            j = lps[j-1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j-1];
            } else {
                i++;
            }
        }
    }

    return matches;
}
```

### Time & Space Complexity

- Time: O(n + m) - Linear time, optimal for single pattern search
- Space: O(m) for the LPS array

## Rabin-Karp Algorithm

Rabin-Karp uses hashing to find matches, allowing fast skipping of non-matches.

### Key Insight

Instead of checking character by character, compute a hash value for the pattern and for each possible substring of the text. Only compare strings with matching hash values.

### Hash Function

A good hash function for strings is the polynomial rolling hash:
h(s) = s[0] + s[1]×d + s[2]×d² + ... + s[n-1]×d^(n-1) mod q

Where:

- d is the number of characters in the alphabet
- q is a large prime number

### Efficient Hash Calculation

When sliding the window, we can update the hash in O(1) time instead of recalculating:

- Remove contribution of outgoing character: text[i-m]
- Add contribution of incoming character: text[i]
- Multiply existing hash by d

### Algorithm Steps

1. Compute hash value for pattern
2. Compute hash for first m characters of text
3. For each position in text:
   a. If hash values match, verify character by character
   b. Update rolling hash for next position

### Implementation

```cpp
vector<int> rabinKarp(const string& text, const string& pattern) {
    vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    if (m > n || m == 0) return matches;

    // Parameters for the hash function
    const int d = 256;  // Number of possible characters
    const int q = 101;  // A prime number

    // Calculate hash for pattern and first window of text
    int patternHash = 0;
    int textHash = 0;
    int h = 1;  // d^(m-1) % q

    // Calculate h = d^(m-1) % q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // Calculate initial hash values
    for (int i = 0; i < m; i++) {
        patternHash = (d * patternHash + pattern[i]) % q;
        textHash = (d * textHash + text[i]) % q;
    }

    // Slide pattern over text
    for (int i = 0; i <= n - m; i++) {
        // Check if hash values match
        if (patternHash == textHash) {
            // Verify character by character
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                matches.push_back(i);
            }
        }

        // Calculate hash for next window
        if (i < n - m) {
            textHash = (d * (textHash - text[i] * h) + text[i + m]) % q;
            // Handle negative hash values
            if (textHash < 0) {
                textHash += q;
            }
        }
    }

    return matches;
}
```

### Time & Space Complexity

- Time: O(n×m) worst case (all hash values match), but O(n+m) expected case
- Space: O(1) excluding output array

## Z Algorithm

The Z Algorithm is another linear-time string matching algorithm that builds a Z array.

### Z Array

For a string S, Z[i] = length of the longest substring starting at S[i] that is also a prefix of S.

### Algorithm Steps

1. Concatenate pattern + '$' + text (where '$' is a character not in the alphabet)
2. Compute Z array for the concatenated string
3. Wherever Z[i] = pattern.length(), we have a match at position i - (pattern.length() + 1)

### Z Array Construction

```cpp
vector<int> computeZArray(const string& s) {
    int n = s.length();
    vector<int> z(n, 0);

    int L = 0, R = 0;
    for (int i = 1; i < n; i++) {
        if (i > R) {
            // Outside the Z-box, compute Z[i] naively
            L = R = i;
            while (R < n && s[R - L] == s[R]) {
                R++;
            }
            z[i] = R - L;
            R--;
        } else {
            // Inside the Z-box, use previously computed values
            int k = i - L;
            if (z[k] < R - i + 1) {
                // Z[i] will be equal to Z[k]
                z[i] = z[k];
            } else {
                // We need to do more comparisons
                L = i;
                while (R < n && s[R - L] == s[R]) {
                    R++;
                }
                z[i] = R - L;
                R--;
            }
        }
    }

    return z;
}

vector<int> zAlgorithm(const string& text, const string& pattern) {
    vector<int> matches;
    if (pattern.empty()) return matches;

    // Concatenate pattern, special character, and text
    string s = pattern + "$" + text;

    // Compute Z array
    vector<int> z = computeZArray(s);

    // Find matches
    for (int i = 0; i < z.size(); i++) {
        if (z[i] == pattern.length()) {
            // Convert to original text position
            int pos = i - pattern.length() - 1;
            if (pos >= 0) {
                matches.push_back(pos);
            }
        }
    }

    return matches;
}
```

### Time & Space Complexity

- Time: O(n + m)
- Space: O(n + m)

## Suffix Trees and Arrays

For applications requiring multiple pattern searches in the same text, preprocessing the text into more advanced data structures can be more efficient.

### Suffix Tree

A compressed trie containing all suffixes of a string.

- Construction time: O(n) using Ukkonen's algorithm
- Query time: O(m) for finding pattern of length m
- Space: O(n) - but with a large constant factor

### Suffix Array

A sorted array of all suffixes of a string, represented by their starting positions.

- Construction time: O(n log n) using efficient algorithms
- Query time: O(m log n) for finding pattern
- Space: O(n) - more space-efficient than suffix trees

Suffix arrays are often preferred in practice due to their simplicity and space efficiency.

## Aho-Corasick Algorithm

For matching multiple patterns simultaneously against a text.

- Preprocess patterns into a finite automaton with failure links
- Scan text once to find all occurrences of all patterns
- Time complexity: O(n + m + z) where n is text length, m is sum of pattern lengths, z is number of matches

## Real-World Applications

### 1. DNA Sequence Alignment

```cpp
// Simplified example of finding DNA patterns
bool findGeneSequence(const string& dna, const string& gene) {
    vector<int> matches = KMPSearch(dna, gene);
    return !matches.empty();
}
```

### 2. Plagiarism Detection

```cpp
// Check for substring matches between documents
double calculateSimilarity(const string& doc1, const string& doc2) {
    // Break documents into chunks and find matches
    // Return similarity score based on matching chunks
}
```

### 3. Code Editors and IDEs

```cpp
// Implement search and replace functionality
void searchAndReplace(string& text, const string& pattern,
                      const string& replacement) {
    vector<int> matches = KMPSearch(text, pattern);

    // Replace in reverse order to avoid index shifting
    for (int i = matches.size() - 1; i >= 0; i--) {
        int pos = matches[i];
        text.replace(pos, pattern.length(), replacement);
    }
}
```

### 4. Data Compression

```cpp
// Simplified Lempel-Ziv compression concept
void compressLZ(const string& text) {
    // Find repeated substrings using string matching
    // Replace with references to previous occurrences
}
```

### 5. Spell Checkers

```cpp
// Check if a word is within edit distance of dictionary entries
vector<string> findSimilarWords(const string& word,
                              const vector<string>& dictionary) {
    vector<string> suggestions;
    for (const string& dict_word : dictionary) {
        if (editDistance(word, dict_word) <= 2) {
            suggestions.push_back(dict_word);
        }
    }
    return suggestions;
}
```

## Practice Problems

### Easy: Implement strStr() (LeetCode #28)

Implement the strStr() function to find the first occurrence of a pattern in a text.

**Approach Hint**: Use KMP algorithm for optimal performance.

### Medium: Repeated String Match (LeetCode #686)

Find the minimum number of times string A needs to be repeated to make B a substring.

**Approach Hint**: Concatenate A maximally twice more than necessary, then use Rabin-Karp.

### Hard: Shortest Palindrome (LeetCode #214)

Find the shortest string that can be added in front of a given string to make it a palindrome.

**Approach Hint**: Use KMP to find the longest prefix that is also a suffix of the reversed string.

## What's Next

Tomorrow we'll tackle Weekly Challenge & Advanced Problems, bringing together multiple algorithms we've studied to solve complex real-world problems.

## Practical Takeaways

- KMP is optimal for single pattern matching with O(n+m) time complexity
- Rabin-Karp shines when searching for multiple patterns of the same length
- Z Algorithm provides a simpler alternative to KMP with the same complexity
- For multiple pattern searches in the same text, consider suffix trees/arrays
- String algorithms are fundamental to numerous applications in computer science and bioinformatics

#DSAin45 #StringAlgorithms #CPlusPlus #PatternMatching #KMP #RabinKarp
