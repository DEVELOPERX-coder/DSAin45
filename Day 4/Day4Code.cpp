// String Operations in C++ - Day 4 of #DSAin45
// Comprehensive examples of string manipulation techniques

#include <iostream>
#include <string>
#include <cstring> // For C-style string functions
#include <algorithm>
#include <vector>
#include <regex>
using namespace std;

// ======== 1. STRING BASICS AND COMPARISON ========

void stringBasics()
{
    cout << "\n===== STRING BASICS =====\n";

    // C-style string declaration and initialization
    char cString[] = "Hello";

    // C++ string declaration and initialization
    string cppString = "Hello";

    // Getting length
    cout << "C-style string length: " << strlen(cString) << endl;
    cout << "C++ string length: " << cppString.length() << endl;
    cout << "C++ string size: " << cppString.size() << endl; // Same as length()

    // Capacity
    cout << "C++ string capacity: " << cppString.capacity() << endl;

    // Comparing strings

    // C-style comparison
    char anotherCString[] = "Hello";
    if (strcmp(cString, anotherCString) == 0)
    {
        cout << "C-style strings are equal\n";
    }

    // C++ style comparison
    string anotherCppString = "Hello";
    if (cppString == anotherCppString)
    {
        cout << "C++ strings are equal\n";
    }

    // Case-insensitive comparison (C++)
    string upper = "HELLO";
    string lower = "hello";

    // Convert both to same case for comparison
    transform(upper.begin(), upper.end(), upper.begin(), ::tolower);
    if (upper == lower)
    {
        cout << "Case-insensitive comparison: strings are equal\n";
    }
}

// ======== 2. STRING TRAVERSAL ========

void stringTraversal()
{
    cout << "\n===== STRING TRAVERSAL =====\n";

    // C-style string traversal
    char cString[] = "Hello";
    cout << "C-style traversal: ";
    for (int i = 0; cString[i] != '\0'; i++)
    {
        cout << cString[i] << " ";
    }
    cout << endl;

    // C++ string traversal methods

    // 1. Traditional indexing
    string cppString = "Hello";
    cout << "C++ traversal with indexing: ";
    for (size_t i = 0; i < cppString.length(); i++)
    {
        cout << cppString[i] << " ";
    }
    cout << endl;

    // 2. Iterator-based traversal
    cout << "C++ traversal with iterators: ";
    for (string::iterator it = cppString.begin(); it != cppString.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

    // 3. Range-based for loop (C++11)
    cout << "C++ traversal with range-based for: ";
    for (char c : cppString)
    {
        cout << c << " ";
    }
    cout << endl;

    // 4. Using at() with bounds checking
    cout << "C++ traversal with at(): ";
    try
    {
        for (size_t i = 0; i < cppString.length(); i++)
        {
            cout << cppString.at(i) << " ";
        }
        // This would throw an exception:
        // cout << cppString.at(cppString.length());
    }
    catch (const out_of_range &e)
    {
        cout << "\nCaught exception: " << e.what();
    }
    cout << endl;
}

// ======== 3. STRING MODIFICATION ========

void stringModification()
{
    cout << "\n===== STRING MODIFICATION =====\n";

    // C-style string modification (risky and needs manual buffer management)
    char cString[20] = "Hello";
    strcat(cString, " World"); // Append - dangerous if buffer too small!
    cout << "C-style concatenation: " << cString << endl;

    // C++ string modification (safe and automatic)
    string cppString = "Hello";

    // Append to string
    cppString += " World"; // Operator overloading
    cout << "C++ concatenation with +=: " << cppString << endl;

    // Append with append()
    cppString.append("!");
    cout << "C++ append() method: " << cppString << endl;

    // Insert in the middle
    cppString.insert(5, " Beautiful");
    cout << "C++ insert() method: " << cppString << endl;

    // Replace part of string
    cppString.replace(6, 9, "Amazing");
    cout << "C++ replace() method: " << cppString << endl;

    // Erase part of string
    cppString.erase(6, 7); // Remove "Amazing"
    cout << "C++ erase() method: " << cppString << endl;

    // Clear entire string
    string tempString = "Temporary";
    tempString.clear();
    cout << "After clear(), string is empty: " << (tempString.empty() ? "true" : "false") << endl;

    // Resize
    cppString.resize(5); // Truncate to "Hello"
    cout << "After resize(5): " << cppString << endl;
}

// ======== 4. STRING TRANSFORMATION ========

void stringTransformation()
{
    cout << "\n===== STRING TRANSFORMATION =====\n";

    string text = "Hello World";

    // To lowercase (copy)
    string lowerText = text;
    transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    cout << "Lowercase: " << lowerText << endl;

    // To uppercase (copy)
    string upperText = text;
    transform(upperText.begin(), upperText.end(), upperText.begin(), ::toupper);
    cout << "Uppercase: " << upperText << endl;

    // Substring extraction
    string substr = text.substr(6, 5); // Start at index 6, take 5 chars
    cout << "Substring: " << substr << endl;

    // Find and extract all words
    string sentence = "This is a sample sentence with multiple words";
    size_t pos = 0;
    string delimiter = " ";

    cout << "Words extracted from sentence:\n";
    while ((pos = sentence.find(delimiter)) != string::npos)
    {
        string word = sentence.substr(0, pos);
        cout << word << endl;
        sentence.erase(0, pos + delimiter.length());
    }
    cout << sentence << endl; // Print the last word
}

// ======== 5. STRING SEARCHING ========

void stringSearching()
{
    cout << "\n===== STRING SEARCHING =====\n";

    string haystack = "The quick brown fox jumps over the lazy dog";

    // Find first occurrence
    string needle = "the";
    size_t found = haystack.find(needle);
    if (found != string::npos)
    {
        cout << "First 'the' found at: " << found << endl;
    }

    // Find last occurrence
    found = haystack.rfind(needle);
    if (found != string::npos)
    {
        cout << "Last 'the' found at: " << found << endl;
    }

    // Find first of any character
    found = haystack.find_first_of("aeiou");
    if (found != string::npos)
    {
        cout << "First vowel found at: " << found << " ('" << haystack[found] << "')" << endl;
    }

    // Find first not of any character
    found = haystack.find_first_not_of("The quick");
    if (found != string::npos)
    {
        cout << "First char not in 'The quick' found at: " << found << " ('" << haystack[found] << "')" << endl;
    }

    // Case-insensitive search using regex
    regex pattern("the", regex_constants::icase);
    sregex_iterator it(haystack.begin(), haystack.end(), pattern);
    sregex_iterator end;

    cout << "Case-insensitive 'the' matches:\n";
    while (it != end)
    {
        smatch match = *it;
        cout << "  Match at position " << match.position() << ": " << match.str() << endl;
        ++it;
    }
}

// ======== 6. ADVANCED STRING ALGORITHMS ========

// 6.1 Check if a string is a palindrome
bool isPalindrome(const string &s)
{
    int left = 0;
    int right = s.length() - 1;

    while (left < right)
    {
        if (s[left] != s[right])
        {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

// 6.2 Improved palindrome check (ignores non-alphanumeric and case)
bool isPalindromeIgnoreNonAlphanumeric(const string &s)
{
    int left = 0;
    int right = s.length() - 1;

    while (left < right)
    {
        // Skip non-alphanumeric characters
        if (!isalnum(s[left]))
        {
            left++;
            continue;
        }
        if (!isalnum(s[right]))
        {
            right--;
            continue;
        }

        // Compare characters (case-insensitive)
        if (tolower(s[left]) != tolower(s[right]))
        {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

// 6.3 Brute force pattern matching
int bruteForcePatternMatch(const string &text, const string &pattern)
{
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++)
    {
        int j;
        for (j = 0; j < m; j++)
        {
            if (text[i + j] != pattern[j])
            {
                break;
            }
        }
        if (j == m)
        {
            return i; // Pattern found at position i
        }
    }

    return -1; // Pattern not found
}

// 6.4 KMP Pattern Matching Algorithm
vector<int> computeLPSArray(const string &pattern)
{
    int m = pattern.length();
    vector<int> lps(m, 0); // Longest Prefix Suffix values

    int len = 0; // Length of previous longest prefix & suffix
    int i = 1;

    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                // Try matching with shorter prefix
                len = lps[len - 1];
            }
            else
            {
                // No match found
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

int kmpSearch(const string &text, const string &pattern)
{
    int n = text.length();
    int m = pattern.length();

    if (m == 0)
        return 0; // Empty pattern matches at position 0

    // Preprocess pattern to compute LPS array
    vector<int> lps = computeLPSArray(pattern);

    int i = 0; // Index for text
    int j = 0; // Index for pattern

    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            i++;
            j++;
        }

        if (j == m)
        {
            // Found pattern at index i-j
            return i - j;
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }

    return -1; // Pattern not found
}

// 6.5 Simple String Compression (Run-length encoding)
string compressString(const string &s)
{
    if (s.empty())
        return "";

    string compressed;
    char currentChar = s[0];
    int count = 1;

    for (size_t i = 1; i < s.length(); i++)
    {
        if (s[i] == currentChar)
        {
            count++;
        }
        else
        {
            compressed += currentChar;
            if (count > 1)
            {
                compressed += to_string(count);
            }
            currentChar = s[i];
            count = 1;
        }
    }

    // Add the last character and its count
    compressed += currentChar;
    if (count > 1)
    {
        compressed += to_string(count);
    }

    // Return original string if compression didn't help
    return (compressed.length() < s.length()) ? compressed : s;
}

// 6.6 Solution to practice problem: Reverse words in a string
string reverseWords(const string &s)
{
    string result;
    string currentWord;

    for (char c : s)
    {
        if (c == ' ')
        {
            // Reverse the current word and add to result
            reverse(currentWord.begin(), currentWord.end());
            result += currentWord + " ";
            currentWord.clear();
        }
        else
        {
            currentWord += c;
        }
    }

    // Handle the last word
    reverse(currentWord.begin(), currentWord.end());
    result += currentWord;

    return result;
}

// ======== MAIN FUNCTION WITH DEMONSTRATIONS ========

int main()
{
    // Basic string operations
    stringBasics();

    // String traversal methods
    stringTraversal();

    // String modification
    stringModification();

    // String transformation
    stringTransformation();

    // String searching
    stringSearching();

    // Advanced string algorithms
    cout << "\n===== ADVANCED STRING ALGORITHMS =====\n";

    // Palindrome checking
    vector<string> palindromeTests = {
        "racecar",
        "A man, a plan, a canal: Panama",
        "Was it a car or a cat I saw?",
        "Not a palindrome"};

    cout << "Palindrome tests:\n";
    for (const string &test : palindromeTests)
    {
        cout << "\"" << test << "\" is "
             << (isPalindromeIgnoreNonAlphanumeric(test) ? "" : "not ")
             << "a palindrome\n";
    }

    // Pattern matching
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    cout << "\nPattern matching:\n";
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;

    int bruteForcePos = bruteForcePatternMatch(text, pattern);
    cout << "Brute force: " << (bruteForcePos != -1 ? "Pattern found at position " + to_string(bruteForcePos) : "Pattern not found") << endl;

    int kmpPos = kmpSearch(text, pattern);
    cout << "KMP: " << (kmpPos != -1 ? "Pattern found at position " + to_string(kmpPos) : "Pattern not found") << endl;

    // String compression
    vector<string> compressionTests = {
        "aabcccccaaa",
        "abcdef",
        "aaaaaaaaaaaaaabbbbbbbbbbbbbbb"};

    cout << "\nString compression tests:\n";
    for (const string &test : compressionTests)
    {
        string compressed = compressString(test);
        cout << "Original: \"" << test << "\" (" << test.length() << " chars)\n";
        cout << "Compressed: \"" << compressed << "\" (" << compressed.length() << " chars)\n";
    }

    // Practice problem solution
    string practiceInput = "Hello World of C++";
    string practiceOutput = reverseWords(practiceInput);

    cout << "\nPractice problem - Reverse words in a string:\n";
    cout << "Input: \"" << practiceInput << "\"\n";
    cout << "Output: \"" << practiceOutput << "\"\n";

    return 0;
}