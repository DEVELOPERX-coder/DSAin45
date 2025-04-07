// Day 8: Stacks - #DSAin45
// A comprehensive implementation with various stack applications

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <map>
#include <list>
#include <stack> // For comparison with STL stack

// ===== STACK IMPLEMENTATIONS =====

// 1. Vector-based Stack Implementation
template <typename T>
class VectorStack
{
private:
    std::vector<T> elements;

public:
    // Push - Add element to the top
    void push(const T &value)
    {
        elements.push_back(value);
        std::cout << "Pushed: " << value << std::endl;
    }

    // Pop - Remove element from the top
    T pop()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Stack underflow - Cannot pop from an empty stack");
        }

        T top_value = elements.back();
        elements.pop_back();
        std::cout << "Popped: " << top_value << std::endl;
        return top_value;
    }

    // Peek - View the top element without removing
    T peek() const
    {
        if (isEmpty())
        {
            throw std::out_of_range("Stack is empty - Cannot peek");
        }

        return elements.back();
    }

    // Check if stack is empty
    bool isEmpty() const
    {
        return elements.empty();
    }

    // Get the number of elements
    size_t size() const
    {
        return elements.size();
    }

    // Clear all elements
    void clear()
    {
        elements.clear();
        std::cout << "Stack cleared" << std::endl;
    }

    // Display the stack (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        std::cout << "Stack (bottom to top): ";
        for (const auto &element : elements)
        {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
};

// 2. Linked List-based Stack Implementation
template <typename T>
class LinkedListStack
{
private:
    struct Node
    {
        T data;
        Node *next;

        Node(const T &value) : data(value), next(nullptr) {}
    };

    Node *top_node;
    size_t stack_size;

public:
    // Constructor
    LinkedListStack() : top_node(nullptr), stack_size(0)
    {
        std::cout << "Created a new linked list stack" << std::endl;
    }

    // Destructor - clean up memory
    ~LinkedListStack()
    {
        clear();
    }

    // Push - Add element to the top
    void push(const T &value)
    {
        Node *new_node = new Node(value);
        new_node->next = top_node;
        top_node = new_node;
        stack_size++;
        std::cout << "Pushed: " << value << std::endl;
    }

    // Pop - Remove element from the top
    T pop()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Stack underflow - Cannot pop from an empty stack");
        }

        Node *temp = top_node;
        T top_value = temp->data;

        top_node = top_node->next;
        delete temp;
        stack_size--;

        std::cout << "Popped: " << top_value << std::endl;
        return top_value;
    }

    // Peek - View the top element without removing
    T peek() const
    {
        if (isEmpty())
        {
            throw std::out_of_range("Stack is empty - Cannot peek");
        }

        return top_node->data;
    }

    // Check if stack is empty
    bool isEmpty() const
    {
        return top_node == nullptr;
    }

    // Get the number of elements
    size_t size() const
    {
        return stack_size;
    }

    // Clear all elements
    void clear()
    {
        while (!isEmpty())
        {
            pop();
        }
        std::cout << "Stack cleared" << std::endl;
    }

    // Display the stack (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        std::cout << "Stack (top to bottom): ";
        Node *current = top_node;
        while (current)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

// 3. STL Stack Demo
void stlStackDemo()
{
    std::cout << "\n===== STL STACK DEMONSTRATION =====" << std::endl;

    std::stack<int> myStack;

    // Push elements
    std::cout << "Pushing elements: 10, 20, 30" << std::endl;
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);

    // View top element
    std::cout << "Top element: " << myStack.top() << std::endl;

    // Check size
    std::cout << "Stack size: " << myStack.size() << std::endl;

    // Pop elements
    std::cout << "Popping all elements:" << std::endl;
    while (!myStack.empty())
    {
        std::cout << "Popped: " << myStack.top() << std::endl;
        myStack.pop();
    }

    std::cout << "Stack is now empty: " << (myStack.empty() ? "Yes" : "No") << std::endl;
}

// ===== STACK APPLICATIONS =====

// 1. Balanced Parentheses Checker
bool areParenthesesBalanced(const std::string &expr)
{
    std::stack<char> s;

    for (char c : expr)
    {
        if (c == '(' || c == '[' || c == '{')
        {
            // Opening bracket, push to stack
            s.push(c);
        }
        else if (c == ')' || c == ']' || c == '}')
        {
            // Closing bracket

            // If stack is empty, no matching opening bracket
            if (s.empty())
            {
                return false;
            }

            // Check if matches with top of stack
            char top = s.top();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
            {
                return false;
            }

            // Matching bracket found, pop from stack
            s.pop();
        }
    }

    // If stack is empty, all brackets were matched
    return s.empty();
}

// 2. Infix to Postfix Conversion
std::string infixToPostfix(const std::string &infix)
{
    std::stack<char> operators;
    std::string postfix;

    // Define operator precedence
    std::map<char, int> precedence;
    precedence['+'] = precedence['-'] = 1;
    precedence['*'] = precedence['/'] = 2;
    precedence['^'] = 3;

    for (char c : infix)
    {
        // If character is an operand, add it to the output
        if (isalnum(c))
        {
            postfix += c;
        }
        // If character is '(', push it onto the stack
        else if (c == '(')
        {
            operators.push(c);
        }
        // If character is ')', pop and add to output until '(' is found
        else if (c == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty())
            {
                operators.pop(); // Remove '(' from the stack
            }
        }
        // If character is an operator
        else if (precedence.find(c) != precedence.end())
        {
            while (!operators.empty() && operators.top() != '(' &&
                   precedence[operators.top()] >= precedence[c])
            {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    // Pop all remaining operators from the stack
    while (!operators.empty())
    {
        if (operators.top() != '(')
        {
            postfix += operators.top();
        }
        operators.pop();
    }

    return postfix;
}

// 3. Evaluating Postfix Expressions
int evaluatePostfix(const std::string &postfix)
{
    std::stack<int> s;

    for (char c : postfix)
    {
        // If character is an operand, push it to the stack
        if (isdigit(c))
        {
            s.push(c - '0'); // Convert char to integer
        }
        // If character is an operator
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (s.size() < 2)
            {
                throw std::invalid_argument("Invalid postfix expression");
            }

            int val2 = s.top();
            s.pop();
            int val1 = s.top();
            s.pop();

            switch (c)
            {
            case '+':
                s.push(val1 + val2);
                break;
            case '-':
                s.push(val1 - val2);
                break;
            case '*':
                s.push(val1 * val2);
                break;
            case '/':
                if (val2 == 0)
                {
                    throw std::invalid_argument("Division by zero");
                }
                s.push(val1 / val2);
                break;
            }
        }
    }

    if (s.size() != 1)
    {
        throw std::invalid_argument("Invalid postfix expression");
    }

    return s.top();
}

// 4. Next Greater Element Finder
std::vector<int> nextGreaterElements(const std::vector<int> &nums)
{
    int n = nums.size();
    std::vector<int> result(n, -1); // Initialize with -1 (no greater element)
    std::stack<int> s;              // Stack to store indices

    for (int i = 0; i < n; i++)
    {
        // While stack is not empty and current element is greater than element at stack top
        while (!s.empty() && nums[i] > nums[s.top()])
        {
            result[s.top()] = nums[i]; // Update result for the index at stack top
            s.pop();                   // Remove the index from stack
        }
        s.push(i); // Push current index to stack
    }

    return result;
}

// 5. Min Stack Implementation
class MinStack
{
private:
    std::stack<int> dataStack; // Stores actual values
    std::stack<int> minStack;  // Keeps track of minimum values

public:
    MinStack()
    {
        std::cout << "Created a new min stack" << std::endl;
    }

    void push(int val)
    {
        dataStack.push(val);

        // If minStack is empty or val is less than or equal to the current minimum
        if (minStack.empty() || val <= minStack.top())
        {
            minStack.push(val);
        }

        std::cout << "Pushed: " << val << std::endl;
    }

    int pop()
    {
        if (dataStack.empty())
        {
            throw std::out_of_range("Stack is empty");
        }

        int top_val = dataStack.top();

        // If top element is the current minimum, also pop from minStack
        if (dataStack.top() == minStack.top())
        {
            minStack.pop();
        }

        dataStack.pop();
        std::cout << "Popped: " << top_val << std::endl;
        return top_val;
    }

    int top()
    {
        if (dataStack.empty())
        {
            throw std::out_of_range("Stack is empty");
        }
        return dataStack.top();
    }

    int getMin()
    {
        if (minStack.empty())
        {
            throw std::out_of_range("Stack is empty");
        }
        return minStack.top();
    }

    bool isEmpty() const
    {
        return dataStack.empty();
    }

    size_t size() const
    {
        return dataStack.size();
    }
};

// 6. Two Stacks in One Array Implementation
class TwoStacks
{
private:
    int *arr;
    int size;
    int top1, top2;

public:
    TwoStacks(int n)
    {
        size = n;
        arr = new int[n];
        top1 = -1;   // First stack starts from the beginning
        top2 = size; // Second stack starts from the end

        std::cout << "Created a two-stack array of size " << n << std::endl;
    }

    ~TwoStacks()
    {
        delete[] arr;
        std::cout << "Destroyed two-stack array" << std::endl;
    }

    // Operations for first stack
    void push1(int x)
    {
        if (top1 < top2 - 1)
        {
            arr[++top1] = x;
            std::cout << "Pushed " << x << " to stack 1" << std::endl;
        }
        else
        {
            throw std::overflow_error("Stack 1 Overflow");
        }
    }

    int pop1()
    {
        if (top1 >= 0)
        {
            int val = arr[top1--];
            std::cout << "Popped " << val << " from stack 1" << std::endl;
            return val;
        }
        else
        {
            throw std::underflow_error("Stack 1 Underflow");
        }
    }

    int peek1() const
    {
        if (top1 >= 0)
        {
            return arr[top1];
        }
        else
        {
            throw std::underflow_error("Stack 1 is empty");
        }
    }

    // Operations for second stack
    void push2(int x)
    {
        if (top1 < top2 - 1)
        {
            arr[--top2] = x;
            std::cout << "Pushed " << x << " to stack 2" << std::endl;
        }
        else
        {
            throw std::overflow_error("Stack 2 Overflow");
        }
    }

    int pop2()
    {
        if (top2 < size)
        {
            int val = arr[top2++];
            std::cout << "Popped " << val << " from stack 2" << std::endl;
            return val;
        }
        else
        {
            throw std::underflow_error("Stack 2 Underflow");
        }
    }

    int peek2() const
    {
        if (top2 < size)
        {
            return arr[top2];
        }
        else
        {
            throw std::underflow_error("Stack 2 is empty");
        }
    }

    // Check if respective stacks are empty
    bool isEmpty1() const
    {
        return top1 == -1;
    }

    bool isEmpty2() const
    {
        return top2 == size;
    }

    // Get sizes of respective stacks
    int size1() const
    {
        return top1 + 1;
    }

    int size2() const
    {
        return size - top2;
    }

    // Display both stacks
    void display() const
    {
        std::cout << "Stack 1 (bottom to top): ";
        for (int i = 0; i <= top1; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Stack 2 (bottom to top): ";
        for (int i = size - 1; i >= top2; i--)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
};

// 7. Stack with GetMiddle Operation
class MiddleStack
{
private:
    std::list<int> dll;           // Doubly linked list
    std::list<int>::iterator mid; // Iterator to middle element
    int size;

public:
    MiddleStack() : size(0)
    {
        std::cout << "Created a new middle stack" << std::endl;
    }

    void push(int x)
    {
        if (size == 0)
        {
            dll.push_back(x);
            mid = dll.begin();
        }
        else
        {
            dll.push_back(x);

            if (size % 2 == 1)
            {
                // Adjust middle pointer for odd size
                mid++;
            }
        }

        size++;
        std::cout << "Pushed: " << x << std::endl;
    }

    int pop()
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }

        int value = dll.back();
        dll.pop_back();

        if (size > 1 && size % 2 == 0)
        {
            // Adjust middle pointer for even size
            mid--;
        }

        size--;
        std::cout << "Popped: " << value << std::endl;
        return value;
    }

    int top() const
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }

        return dll.back();
    }

    int getMiddle() const
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }

        return *mid;
    }

    void deleteMiddle()
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }

        auto it = mid;

        if (size == 1)
        {
            dll.erase(it);
            size = 0;
            return;
        }

        if (size % 2 == 0)
        {
            // Even size, move middle forward after deletion
            auto next = mid;
            next++;
            dll.erase(it);
            mid = next;
        }
        else
        {
            // Odd size, move middle backward after deletion
            auto prev = mid;
            prev--;
            dll.erase(it);
            mid = prev;
        }

        size--;
        std::cout << "Deleted middle element" << std::endl;
    }

    bool isEmpty() const
    {
        return size == 0;
    }

    int getSize() const
    {
        return size;
    }

    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        std::cout << "Stack (bottom to top): ";
        for (const auto &item : dll)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;

        std::cout << "Middle element: " << getMiddle() << std::endl;
    }
};

// 8. Stock Span Calculator
std::vector<int> calculateSpan(const std::vector<int> &prices)
{
    int n = prices.size();
    std::vector<int> spans(n, 1); // Initialize with 1 (span is at least 1)
    std::stack<int> s;            // Stack to store indices

    for (int i = 0; i < n; i++)
    {
        // Pop until we find a price greater than the current price
        while (!s.empty() && prices[s.top()] <= prices[i])
        {
            s.pop();
        }

        // Calculate span
        spans[i] = s.empty() ? (i + 1) : (i - s.top());

        // Push current index to stack
        s.push(i);
    }

    return spans;
}

// Main function with examples
int main()
{
    std::cout << "===== DAY 8: STACKS DEMONSTRATION =====" << std::endl;

    // ===== VECTOR STACK DEMO =====
    std::cout << "\n===== VECTOR STACK DEMO =====" << std::endl;
    VectorStack<int> vstack;

    std::cout << "Is empty? " << (vstack.isEmpty() ? "Yes" : "No") << std::endl;

    vstack.push(10);
    vstack.push(20);
    vstack.push(30);

    vstack.display();

    std::cout << "Stack size: " << vstack.size() << std::endl;
    std::cout << "Top element: " << vstack.peek() << std::endl;

    vstack.pop();
    vstack.display();

    vstack.clear();
    std::cout << "Is empty after clear? " << (vstack.isEmpty() ? "Yes" : "No") << std::endl;

    // ===== LINKED LIST STACK DEMO =====
    std::cout << "\n===== LINKED LIST STACK DEMO =====" << std::endl;
    LinkedListStack<int> lstack;

    std::cout << "Is empty? " << (lstack.isEmpty() ? "Yes" : "No") << std::endl;

    lstack.push(10);
    lstack.push(20);
    lstack.push(30);

    lstack.display();

    std::cout << "Stack size: " << lstack.size() << std::endl;
    std::cout << "Top element: " << lstack.peek() << std::endl;

    lstack.pop();
    lstack.display();

    lstack.clear();
    std::cout << "Is empty after clear? " << (lstack.isEmpty() ? "Yes" : "No") << std::endl;

    // ===== STL STACK DEMO =====
    stlStackDemo();

    // ===== STACK APPLICATIONS =====

    // 1. Balanced Parentheses Checker
    std::cout << "\n===== BALANCED PARENTHESES CHECKER =====" << std::endl;
    std::vector<std::string> expressions = {
        "{}",
        "()[]{}",
        "([{}])",
        "([)]",
        "}{"};

    for (const auto &expr : expressions)
    {
        bool isBalanced = areParenthesesBalanced(expr);
        std::cout << "Expression \"" << expr << "\" is "
                  << (isBalanced ? "balanced" : "not balanced") << std::endl;
    }

    // 2. Infix to Postfix Conversion
    std::cout << "\n===== INFIX TO POSTFIX CONVERSION =====" << std::endl;
    std::vector<std::string> infixExpressions = {
        "A+B",
        "A+B*C",
        "(A+B)*C",
        "A+B*C+D",
        "A*(B+C*D)+E"};

    for (const auto &expr : infixExpressions)
    {
        std::string postfix = infixToPostfix(expr);
        std::cout << "Infix: " << expr << " -> Postfix: " << postfix << std::endl;
    }

    // 3. Evaluating Postfix Expressions
    std::cout << "\n===== POSTFIX EXPRESSION EVALUATION =====" << std::endl;
    std::vector<std::string> postfixExpressions = {
        "23+",
        "23*5+",
        "23+5*",
        "23+45*+"};

    try
    {
        for (const auto &expr : postfixExpressions)
        {
            int result = evaluatePostfix(expr);
            std::cout << "Postfix: " << expr << " = " << result << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // 4. Next Greater Element
    std::cout << "\n===== NEXT GREATER ELEMENT =====" << std::endl;
    std::vector<int> numbers = {4, 5, 2, 25, 7, 8};

    std::vector<int> nextGreater = nextGreaterElements(numbers);

    std::cout << "Element -> Next Greater Element" << std::endl;
    for (size_t i = 0; i < numbers.size(); i++)
    {
        std::cout << numbers[i] << " -> " << (nextGreater[i] == -1 ? -1 : nextGreater[i]) << std::endl;
    }

    // 5. Min Stack
    std::cout << "\n===== MIN STACK DEMO =====" << std::endl;
    MinStack minStack;

    minStack.push(3);
    minStack.push(5);
    minStack.push(2);
    minStack.push(1);
    minStack.push(4);

    std::cout << "Top element: " << minStack.top() << std::endl;
    std::cout << "Minimum element: " << minStack.getMin() << std::endl;

    minStack.pop();
    std::cout << "After pop - Top: " << minStack.top() << ", Min: " << minStack.getMin() << std::endl;

    minStack.pop();
    std::cout << "After pop - Top: " << minStack.top() << ", Min: " << minStack.getMin() << std::endl;

    // 6. Two Stacks in One Array
    std::cout << "\n===== TWO STACKS IN ONE ARRAY =====" << std::endl;
    TwoStacks twoStacks(10);

    try
    {
        // Pushing to first stack
        twoStacks.push1(1);
        twoStacks.push1(2);
        twoStacks.push1(3);

        // Pushing to second stack
        twoStacks.push2(10);
        twoStacks.push2(9);
        twoStacks.push2(8);

        twoStacks.display();

        std::cout << "Stack 1 - Top element: " << twoStacks.peek1() << std::endl;
        std::cout << "Stack 2 - Top element: " << twoStacks.peek2() << std::endl;

        twoStacks.pop1();
        twoStacks.pop2();

        twoStacks.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // 7. Middle Stack
    std::cout << "\n===== MIDDLE STACK DEMO =====" << std::endl;
    MiddleStack midStack;

    try
    {
        midStack.push(1);
        std::cout << "Middle after pushing 1: " << midStack.getMiddle() << std::endl;

        midStack.push(2);
        std::cout << "Middle after pushing 2: " << midStack.getMiddle() << std::endl;

        midStack.push(3);
        std::cout << "Middle after pushing 3: " << midStack.getMiddle() << std::endl;

        midStack.push(4);
        std::cout << "Middle after pushing 4: " << midStack.getMiddle() << std::endl;

        midStack.push(5);
        std::cout << "Middle after pushing 5: " << midStack.getMiddle() << std::endl;

        midStack.display();

        midStack.deleteMiddle();
        std::cout << "After deleting middle: ";
        midStack.display();

        midStack.pop();
        std::cout << "After popping: ";
        midStack.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // 8. Stock Span Problem
    std::cout << "\n===== STOCK SPAN PROBLEM =====" << std::endl;
    std::vector<int> stockPrices = {100, 80, 60, 70, 60, 75, 85};

    std::vector<int> spans = calculateSpan(stockPrices);

    std::cout << "Day | Price | Span" << std::endl;
    std::cout << "----------------" << std::endl;
    for (size_t i = 0; i < stockPrices.size(); i++)
    {
        std::cout << i + 1 << "   | " << stockPrices[i] << "    | " << spans[i] << std::endl;
    }

    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;

    return 0;
}