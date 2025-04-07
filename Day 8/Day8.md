# 📚 DAY 8: Stacks - The Last-In-First-Out Champions #DSAin45

Welcome to Day 8! After two days of exploring linked lists—where the question "what comes next?" becomes a software engineering career choice—we're now tackling one of computing's simplest yet most powerful data structures: the Stack.

_A stack is like a Pringles can—you can only get to the chips from one end, and the last one you put in is the first one you'll take out. Except unlike Pringles, you can never reach that final chip at the bottom without eating all the others first._

## 🤔 What Is a Stack?

A stack is a linear data structure that follows the Last-In-First-Out (LIFO) principle. Like a stack of plates, books, or pancakes in the real world, the last item placed on top is the first item to be removed.

```
   ┌─────────┐
   │  Top    │ ← Most recently added (will be removed first)
   ├─────────┤
   │         │
   ├─────────┤
   │         │
   ├─────────┤
   │ Bottom  │ ← Least recently added (will be removed last)
   └─────────┘
```

> **🧠 Fun Fact**: The concept of a "stack" in computing dates back to 1946, when Alan Turing used the terms "bury" and "unbury" to describe what we now call "push" and "pop" operations. The first hardware implementation appeared in the UNIVAC I computer (1951), featuring a hardware stack for subroutine return addresses.

## 📋 Stack Operations

Stacks support two primary operations:

- **Push**: Add an element to the top of the stack
- **Pop**: Remove the top element from the stack

Additional common operations include:

- **Peek/Top**: View the top element without removing it
- **isEmpty**: Check if the stack is empty
- **Size**: Return the number of elements in the stack

_The simplicity of stacks is like a superhero with only two powers that somehow always saves the day—push and pop may not sound exciting, but they can solve a surprising range of complex problems._

## 🏗️ Implementing a Stack in C++

There are multiple ways to implement a stack:

### 1. Using a Dynamic Array (Vector)

```cpp
#include <iostream>
#include <vector>

template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    // Push - Add element to the top
    void push(const T& value) {
        elements.push_back(value);
    }

    // Pop - Remove element from the top
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }

        T top_value = elements.back();
        elements.pop_back();
        return top_value;
    }

    // Peek - View the top element without removing
    T peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }

        return elements.back();
    }

    // Check if stack is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Get the number of elements
    size_t size() const {
        return elements.size();
    }

    // Clear all elements
    void clear() {
        elements.clear();
    }
};
```

_Using a vector to implement a stack is like using a sports car to drive to the corner store—it's probably overkill, but hey, it gets the job done in style._

### 2. Using a Linked List

```cpp
#include <iostream>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* top_node;
    size_t stack_size;

public:
    // Constructor
    Stack() : top_node(nullptr), stack_size(0) {}

    // Destructor - clean up memory
    ~Stack() {
        clear();
    }

    // Push - Add element to the top
    void push(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = top_node;
        top_node = new_node;
        stack_size++;
    }

    // Pop - Remove element from the top
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }

        Node* temp = top_node;
        T top_value = temp->data;

        top_node = top_node->next;
        delete temp;
        stack_size--;

        return top_value;
    }

    // Peek - View the top element without removing
    T peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }

        return top_node->data;
    }

    // Check if stack is empty
    bool isEmpty() const {
        return top_node == nullptr;
    }

    // Get the number of elements
    size_t size() const {
        return stack_size;
    }

    // Clear all elements
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};
```

_A linked list implementation of a stack is like a tower of people standing on each other's shoulders—the person on top is the only one who gets to interact with the outside world, and when they jump off, the next person takes over._

### 3. Using STL's `std::stack`

C++ provides a built-in stack implementation in the Standard Template Library:

```cpp
#include <iostream>
#include <stack>

int main() {
    std::stack<int> myStack;

    // Push elements
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);

    // View top element
    std::cout << "Top element: " << myStack.top() << std::endl;

    // Pop elements
    while (!myStack.empty()) {
        std::cout << "Popping: " << myStack.top() << std::endl;
        myStack.pop();
    }

    return 0;
}
```

_Using STL's stack is like buying a pre-made sandwich versus making one yourself—less fun but more reliable, and you don't have to clean up the kitchen afterward._

> **🧠 Fun Fact**: The STL's `std::stack` is not actually a container itself, but a container adapter that uses another container (by default, `std::deque`) as its underlying storage. You can also configure it to use `std::vector` or `std::list` instead.

## ⚡ Time & Space Complexity

A properly implemented stack should have the following complexities:

| Operation | Time Complexity | Space Complexity |
| --------- | --------------- | ---------------- |
| Push      | O(1)            | O(1)             |
| Pop       | O(1)            | O(1)             |
| Peek      | O(1)            | O(1)             |
| isEmpty   | O(1)            | O(1)             |
| Size      | O(1)            | O(1)             |

_Stack operations are like fast food—quick, efficient, and you always know exactly what you're getting._

## 🌎 Real-World Applications of Stacks

Stacks are surprisingly ubiquitous in computing:

### 1. Function Call Management

When your program calls a function, the function's parameters, local variables, and return address are pushed onto a call stack. When the function finishes, these items are popped, and execution returns to the calling function.

```cpp
void functionA() {
    int x = 5;
    functionB();  // When this is called, functionA's context is saved on the stack
    // Execution returns here after functionB completes
}

void functionB() {
    int y = 10;
    functionC();  // functionB's context is saved on the stack
    // Execution returns here after functionC completes
}

void functionC() {
    int z = 15;
    // Do something
}  // When functionC completes, its context is popped from the stack
```

_The call stack is like a stack of paperwork on your desk—you keep adding to the pile when starting new tasks and removing from the top as you complete them._

### 2. Expression Evaluation

Stacks are used to evaluate mathematical expressions, particularly for:

- Converting infix expressions (e.g., `A + B`) to postfix (e.g., `A B +`) or prefix (e.g., `+ A B`)
- Evaluating postfix expressions
- Checking expression syntax (e.g., balanced parentheses)

_Evaluating expressions with stacks is like solving a puzzle where you can only look at one piece at a time—methodical and sequential._

### 3. Undo/Redo Functionality

Many applications use a stack to implement undo/redo functionality:

- Each user action is pushed onto an "undo stack"
- When the user performs an undo, the action is popped from the undo stack and pushed onto a "redo stack"
- When the user performs a redo, the action is popped from the redo stack and reapplied

_The undo feature in software is like having a time machine that only goes backward one step at a time—limited but incredibly useful._

### 4. Browser History

Your browser's back button relies on a stack to keep track of previously visited pages:

- When you visit a new page, it's pushed onto the stack
- When you click "Back," the current page is popped off the stack

_Browser history is like breadcrumbs in a digital forest—they show you where you've been and how to retrace your steps._

### 5. Syntax Parsing

Compilers and interpreters use stacks for parsing and syntax validation:

- Parsing nested structures like loops, conditionals, and code blocks
- Checking for balanced symbols (brackets, parentheses, braces)
- Implementing recursive descent parsers

_Syntax parsing is like an ultra-strict grammar teacher checking every sentence structure, with stacks as their teaching assistant._

## 🎭 Classic Stack Problems

### 1. Balanced Parentheses

Determine if a string of parentheses is balanced:

```cpp
bool areParenthesesBalanced(const std::string& expr) {
    std::stack<char> s;

    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (s.empty()) return false;

            char top = s.top();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }

            s.pop();
        }
    }

    return s.empty();
}
```

_Checking balanced parentheses is like being a nightclub bouncer who ensures every person who enters also leaves, and in the correct order._

### 2. Infix to Postfix Conversion

Convert an infix expression (e.g., `A + B * C`) to postfix (e.g., `A B C * +`):

```cpp
std::string infixToPostfix(const std::string& infix) {
    std::stack<char> operators;
    std::string postfix;

    std::map<char, int> precedence;
    precedence['+'] = precedence['-'] = 1;
    precedence['*'] = precedence['/'] = 2;
    precedence['^'] = 3;

    for (char c : infix) {
        // If character is an operand, add it to the output
        if (isalnum(c)) {
            postfix += c;
        }
        // If character is '(', push it onto the stack
        else if (c == '(') {
            operators.push(c);
        }
        // If character is ')', pop and add to output until '(' is found
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop();  // Remove '(' from the stack
        }
        // If character is an operator
        else if (precedence.find(c) != precedence.end()) {
            while (!operators.empty() && operators.top() != '(' &&
                   precedence[operators.top()] >= precedence[c]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    // Pop all remaining operators from the stack
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}
```

_Converting infix to postfix is like translating from English to Polish notation—it looks strange at first, but it's much easier for computers to understand without the ambiguity of precedence._

### 3. Evaluating Postfix Expressions

Evaluate a postfix expression (e.g., `5 3 + 2 *`):

```cpp
int evaluatePostfix(const std::string& postfix) {
    std::stack<int> s;

    for (char c : postfix) {
        if (isdigit(c)) {
            s.push(c - '0');  // Convert char to integer
        } else {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();

            switch (c) {
                case '+': s.push(val1 + val2); break;
                case '-': s.push(val1 - val2); break;
                case '*': s.push(val1 * val2); break;
                case '/': s.push(val1 / val2); break;
            }
        }
    }

    return s.top();
}
```

_Evaluating postfix expressions is like following a recipe where all the measurements come before the instructions—strange at first, but incredibly efficient once you get used to it._

### 4. Next Greater Element

Find the next greater element for each element in an array:

```cpp
std::vector<int> nextGreaterElements(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> result(n, -1);  // Initialize with -1 (no greater element)
    std::stack<int> s;  // Stack to store indices

    for (int i = 0; i < n; i++) {
        // While stack is not empty and current element is greater than element at stack top
        while (!s.empty() && nums[i] > nums[s.top()]) {
            result[s.top()] = nums[i];  // Update result for the index at stack top
            s.pop();  // Remove the index from stack
        }
        s.push(i);  // Push current index to stack
    }

    return result;
}
```

_Finding the next greater element is like being at a concert and trying to spot the taller person behind you—you need to look ahead and remember who you've already checked._

### 5. Min Stack

Design a stack that supports push, pop, top, and retrieving the minimum element in constant time:

```cpp
class MinStack {
private:
    std::stack<int> dataStack;      // Stores actual values
    std::stack<int> minStack;       // Keeps track of minimum values

public:
    void push(int val) {
        dataStack.push(val);

        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }

    void pop() {
        if (dataStack.empty()) return;

        if (dataStack.top() == minStack.top()) {
            minStack.pop();
        }

        dataStack.pop();
    }

    int top() {
        return dataStack.top();
    }

    int getMin() {
        return minStack.top();
    }
};
```

_A MinStack is like having a regular stack but with a small notepad where you jot down the smallest value seen so far—simple but effective._

## 🚀 Advanced Stack Techniques

### 1. Two Stacks in One Array

Implement two stacks in a single array:

```cpp
class TwoStacks {
private:
    int* arr;
    int size;
    int top1, top2;

public:
    TwoStacks(int n) {
        size = n;
        arr = new int[n];
        top1 = -1;  // First stack starts from the beginning
        top2 = size;  // Second stack starts from the end
    }

    ~TwoStacks() {
        delete[] arr;
    }

    // Operations for first stack
    void push1(int x) {
        if (top1 < top2 - 1) {
            arr[++top1] = x;
        } else {
            throw std::overflow_error("Stack 1 Overflow");
        }
    }

    int pop1() {
        if (top1 >= 0) {
            return arr[top1--];
        } else {
            throw std::underflow_error("Stack 1 Underflow");
        }
    }

    // Operations for second stack
    void push2(int x) {
        if (top1 < top2 - 1) {
            arr[--top2] = x;
        } else {
            throw std::overflow_error("Stack 2 Overflow");
        }
    }

    int pop2() {
        if (top2 < size) {
            return arr[top2++];
        } else {
            throw std::underflow_error("Stack 2 Underflow");
        }
    }
};
```

_Using two stacks in one array is like having roommates who share a closet—one uses the left side, one uses the right, and trouble starts when they meet in the middle._

### 2. Stack with GetMiddle Operation

Design a stack with a `getMiddle()` operation:

```cpp
class MiddleStack {
private:
    std::list<int> dll;  // Doubly linked list
    std::list<int>::iterator mid;  // Iterator to middle element
    int size;

public:
    MiddleStack() : size(0) {}

    void push(int x) {
        if (size == 0) {
            dll.push_back(x);
            mid = dll.begin();
        } else {
            dll.push_back(x);

            if (size % 2 == 1) {
                // Adjust middle pointer for odd size
                mid++;
            }
        }

        size++;
    }

    int pop() {
        if (size == 0) {
            throw std::underflow_error("Stack is empty");
        }

        int value = dll.back();
        dll.pop_back();

        if (size % 2 == 0) {
            // Adjust middle pointer for even size
            mid--;
        }

        size--;
        return value;
    }

    int getMiddle() {
        if (size == 0) {
            throw std::underflow_error("Stack is empty");
        }

        return *mid;
    }

    void deleteMiddle() {
        if (size == 0) {
            throw std::underflow_error("Stack is empty");
        }

        auto it = mid;

        if (size % 2 == 0) {
            // Even size, move middle forward after deletion
            mid++;
        } else {
            // Odd size, move middle backward after deletion
            auto prev = mid;
            prev--;
            mid = prev;
        }

        dll.erase(it);
        size--;
    }
};
```

_A stack with a getMiddle operation is like having a book with a bookmark that always stays in the middle, no matter how many pages you add or remove—a neat trick that requires some careful pointer juggling._

### 3. The Stock Span Problem

Calculate the span of a stock's price for each day:

```cpp
std::vector<int> calculateSpan(const std::vector<int>& prices) {
    int n = prices.size();
    std::vector<int> spans(n, 1);  // Initialize with 1 (span is at least 1)
    std::stack<int> s;  // Stack to store indices

    for (int i = 0; i < n; i++) {
        // Pop until we find a price greater than the current price
        while (!s.empty() && prices[s.top()] <= prices[i]) {
            s.pop();
        }

        // Calculate span
        spans[i] = s.empty() ? (i + 1) : (i - s.top());

        // Push current index to stack
        s.push(i);
    }

    return spans;
}
```

_The stock span problem is like asking how many days you need to look backward to find a higher stock price—a surprisingly common question in finance, efficiently solved with a stack._

> **🧠 Fun Fact**: Many programming languages use a stack-based execution model for their virtual machines. For example, the Java Virtual Machine (JVM) and the .NET Common Language Runtime (CLR) are stack-based, while others like the WebAssembly virtual machine are register-based.

## 📚 LeetCode Practice Problems

To reinforce today's stack concepts, try these LeetCode problems:

**Easy:**

- #20: Valid Parentheses
- #155: Min Stack
- #225: Implement Stack using Queues
- #232: Implement Queue using Stacks
- #496: Next Greater Element I

**Medium:**

- #71: Simplify Path
- #150: Evaluate Reverse Polish Notation
- #856: Score of Parentheses
- #946: Validate Stack Sequences
- #1249: Minimum Remove to Make Valid Parentheses

**Hard:**

- #42: Trapping Rain Water
- #84: Largest Rectangle in Histogram
- #85: Maximal Rectangle
- #224: Basic Calculator
- #726: Number of Atoms

_LeetCode stack problems are like mental gymnastics—they seem impossible at first, but with practice, you'll find yourself solving them automatically, much like how a stack naturally processes its elements._

## 😂 Stack Jokes

- Why don't programmers like to pop too many items from their stack? They're afraid of stack underflow—it's like trying to take more pancakes than there are on the plate.

- I used to think recursion was better than iteration, but then I got a stack overflow error. Turns out there's a limit to how deep your thoughts can go.

- A stack walks into a bar and asks for a beer. The bartender says, "Sorry, we only serve queues here." The stack replies, "That's fine, I can implement a queue with two of me!"

- What's a stack's favorite song? "LIFO the Party" by Pitbull.

## 🔮 Tomorrow's Preview:

Tomorrow we'll explore Queues—stacks' more patient siblings who believe in fairness and process items in the order they arrived (FIFO: First-In-First-Out). We'll see how these orderly data structures manage everything from print jobs to web server requests.

_If stacks are like a stack of plates, queues are like a line at a movie theater—the first person to arrive is the first one served, no pushing or cutting allowed!_

#DSAin45 #Day8 #Stacks #CPlusPlus #DataStructures #CodingInterview #LIFO
