# 🔗 DAY 6: Linked Lists - Part 1 (Singly Linked Lists) #DSAin45

Welcome to Day 6! Yesterday we explored STL containers—C++'s ready-made furniture for your code home. Today, we're diving into linked lists—data structures that give pointers a purpose beyond making C++ developers cry.

_A linked list is like a treasure hunt where each clue tells you where to find the next one. Perfect if you hate your data being stored together like some happy family reunion._

## 🤔 What Even Is a Linked List?

Unlike arrays—where elements huddle together in memory like nervous passengers on a crowded subway—linked lists embrace social distancing. Each element (node) contains both data and a pointer to the next node, scattered throughout memory like my motivation throughout the workday.

```
+------+------+    +------+------+    +------+------+
| Data | Next |--->| Data | Next |--->| Data | Next |---> nullptr
+------+------+    +------+------+    +------+------+
   Head                                   Tail
```

_Pointers in linked lists are like that friend who always knows where the next party is. Without them, you'd just have a bunch of disconnected data sitting alone at home on a Saturday night._

## 🏗️ Node Structure: The Building Block

Let's start with the fundamental building block of a linked list—the node:

```cpp
struct Node {
    int data;       // The actual value we're storing
    Node* next;     // Pointer to the next node (or nullptr if we're at the end)

    // Constructor to make our lives easier
    Node(int value) : data(value), next(nullptr) {}
};
```

_A node without a next pointer is like me without coffee—technically functional but likely to end abruptly._

## 🚗 Why Linked Lists Instead of Arrays?

You might be thinking: "I already have arrays and vectors. Why do I need another list type?" Well, dear reader:

1. **Dynamic Size**: Linked lists grow organically, one node at a time—no dramatic reallocation tantrums
2. **Insertion/Deletion Efficiency**: Adding or removing elements is O(1) when you're already at the position—no need to shuffle other elements like a deck of cards
3. **No Memory Wastage**: They use exactly the memory they need—they're the minimalists of the data structure world

4. **No Need to Relocate**: Elements can stay at their memory addresses forever—they have tenure

_Using an array when you need constant insertions/deletions is like using scissors to mow your lawn. It'll work eventually, but there are better tools for the job._

## 🏗️ Building a Linked List Class

Let's implement a basic singly linked list class that gives us a nice wrapper around our nodes:

```cpp
class LinkedList {
private:
    Node* head;  // Points to the first node
    Node* tail;  // Points to the last node (optional but useful)
    int size;    // Keeps track of the number of nodes

public:
    // Constructor
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor (very important to avoid memory leaks!)
    ~LinkedList() {
        clear();  // Call our clear method to free memory
    }

    // Clear all nodes from the list
    void clear() {
        Node* current = head;
        while (current) {
            Node* next = current->next;  // Save the next node
            delete current;              // Delete the current node
            current = next;              // Move to the next node
        }
        head = tail = nullptr;
        size = 0;
    }

    // Other methods will go here...
};
```

_A linked list without a proper destructor is like leaving your campsite without putting out the fire—there will be consequences, and they won't be pretty._

## 🔄 Basic Operations: The Essential Toolkit

### 🔍 Traversal: The Grand Tour

To visit each node in the list, we need to traverse it:

```cpp
void display() const {
    if (isEmpty()) {
        std::cout << "List is empty" << std::endl;
        return;
    }

    Node* current = head;
    while (current) {
        std::cout << current->data;
        if (current->next) {
            std::cout << " -> ";
        }
        current = current->next;
    }
    std::cout << std::endl;
}
```

_Traversing a linked list is like following a trail of breadcrumbs—except the birds are memory leaks, and they'll eat your program if you're not careful._

### ➕ Insertion: Adding New Nodes

There are three typical places to insert: beginning, end, and middle.

#### 1. Insertion at the Beginning (Prepend)

```cpp
void insertAtBeginning(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = tail = newNode;  // If list is empty, the new node is both head and tail
    } else {
        newNode->next = head;   // Point the new node to the current head
        head = newNode;         // Update head to the new node
    }

    size++;
}
```

_Adding to the beginning of a linked list is like cutting in line—quick, efficient, and only frowned upon in polite society._

#### 2. Insertion at the End (Append)

```cpp
void insertAtEnd(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = tail = newNode;  // If list is empty, the new node is both head and tail
    } else {
        tail->next = newNode;   // Link the current tail to the new node
        tail = newNode;         // Update tail to the new node
    }

    size++;
}
```

_With a tail pointer, adding to the end is O(1). Without it, it's like finding the last person in a conga line by starting at the front—O(n) and increasingly awkward the longer it gets._

#### 3. Insertion at a Specific Position

```cpp
bool insertAtPosition(int value, int position) {
    // Check if position is valid
    if (position < 0 || position > size) {
        std::cout << "Invalid position" << std::endl;
        return false;
    }

    // If position is 0, insert at the beginning
    if (position == 0) {
        insertAtBeginning(value);
        return true;
    }

    // If position is at the end, insert at the end
    if (position == size) {
        insertAtEnd(value);
        return true;
    }

    // Create a new node
    Node* newNode = new Node(value);

    // Traverse to the node just before the position
    Node* current = head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }

    // Insert the new node
    newNode->next = current->next;
    current->next = newNode;

    size++;
    return true;
}
```

_Inserting in the middle is like adding a new person to a human chain where everyone is holding hands—you need to break one link and create two new ones._

### ➖ Deletion: Removing Nodes

Similarly, we can delete from the beginning, end, or middle.

#### 1. Deletion from the Beginning

```cpp
bool deleteFromBeginning() {
    if (isEmpty()) {
        std::cout << "List is empty" << std::endl;
        return false;
    }

    Node* temp = head;       // Save the head node
    head = head->next;       // Update head to the next node

    // If list becomes empty, update tail
    if (head == nullptr) {
        tail = nullptr;
    }

    delete temp;             // Free the memory
    size--;
    return true;
}
```

_Removing from the beginning is like firing the team leader—quick, but now someone else has to step up._

#### 2. Deletion from the End

```cpp
bool deleteFromEnd() {
    if (isEmpty()) {
        std::cout << "List is empty" << std::endl;
        return false;
    }

    // If there's only one node
    if (head == tail) {
        delete head;
        head = tail = nullptr;
        size = 0;
        return true;
    }

    // Traverse to the second-to-last node
    Node* current = head;
    while (current->next != tail) {
        current = current->next;
    }

    delete tail;          // Delete the last node
    tail = current;       // Update tail to the second-to-last node
    tail->next = nullptr; // The new tail points to nullptr

    size--;
    return true;
}
```

_Removing from the end without a tail pointer requires finding the second-to-last node—it's like trying to find the person who was second-to-last to arrive at a party where nobody remembers when they got there._

#### 3. Deletion from a Specific Position

```cpp
bool deleteFromPosition(int position) {
    // Check if position is valid
    if (position < 0 || position >= size) {
        std::cout << "Invalid position" << std::endl;
        return false;
    }

    // If position is 0, delete from the beginning
    if (position == 0) {
        return deleteFromBeginning();
    }

    // If position is at the end, delete from the end
    if (position == size - 1) {
        return deleteFromEnd();
    }

    // Traverse to the node just before the position
    Node* current = head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }

    // Save the node to be deleted
    Node* temp = current->next;

    // Update the next pointer to skip the node to be deleted
    current->next = temp->next;

    // Delete the node
    delete temp;

    size--;
    return true;
}
```

_Removing a node from the middle is like extracting someone from the middle of a conga line—you need to connect the people before and after, and hope nobody trips during the transition._

## 🔍 Searching: Finding an Element

```cpp
bool search(int value) const {
    Node* current = head;

    while (current) {
        if (current->data == value) {
            return true;  // Found it!
        }
        current = current->next;
    }

    return false;  // Not found
}
```

_Searching in a linked list is like looking for your keys in a series of rooms where each room tells you which room to check next—O(n) and hopefully they're not in the last place you look (but they always are)._

## ⚡ Time & Space Complexity: The Numbers Game

Let's analyze the time and space complexity of linked list operations:

| Operation               | Time Complexity | Space Complexity | Note                             |
| ----------------------- | --------------- | ---------------- | -------------------------------- |
| Access by index         | O(n)            | O(1)             | Need to traverse from head       |
| Insertion at beginning  | O(1)            | O(1)             | Just update head                 |
| Insertion at end        | O(1)\*          | O(1)             | \*O(1) with tail pointer         |
| Insertion at position   | O(n)            | O(1)             | Need to find the position first  |
| Deletion from beginning | O(1)            | O(1)             | Just update head                 |
| Deletion from end       | O(n)            | O(1)             | Need to find second-to-last node |
| Deletion from position  | O(n)            | O(1)             | Need to find the position first  |
| Search                  | O(n)            | O(1)             | May need to traverse entire list |

_A linked list is like having a sports car (fast insertions) with a terrible GPS (slow lookups)—amazing for some trips, terrible for others._

## 🧠 Memory Management: Don't Leak Your Memories

One of the trickiest aspects of linked lists in C++ is memory management. Every `new` must eventually be balanced with a `delete`, or you'll leak memory faster than a sieve leaks water.

```cpp
// Destructor - vital for cleanup
~LinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;  // Save next before deleting current
        delete current;
        current = next;
    }
}
```

_Forgetting to delete nodes is like inviting guests to a party but never showing them the door—eventually, you'll run out of space and crash._

## 🌎 Real-World Applications: Beyond the Textbook

Linked lists aren't just theoretical constructs for torturing CS students. They're used in real systems:

1. **Browser History**: Ever clicked the back button? You're traversing a linked list of web pages you've visited.

2. **Music Players**: Your playlist is often implemented as a linked list, especially if it supports features like "play next" or "add to queue."

3. **Chess Game Moves**: Chess engines use linked lists to track the sequence of moves in a game.

4. **Image Viewers**: When you're flipping through a series of images, many image viewers use a linked list to manage the sequence.

5. **Text Editors**: The undo/redo functionality in text editors often uses a linked list to keep track of changes.

6. **Memory Allocation**: Operating systems use linked lists to track blocks of free memory.

7. **Hash Tables with Chaining**: Linked lists are used to resolve collisions in hash tables.

_Using a linked list for your music playlist is why adding a song to "play next" is instant, but finding that one song you added three hours ago requires scrolling through your entire queue._

## 🎭 Common Pitfalls: The Tragedy in Three Acts

### Act I: The Null Pointer Dereference

```cpp
Node* head = nullptr;
head->data = 42;  // CRASH! Dereferencing nullptr
```

_Dereferencing a null pointer is like trying to open a door that doesn't exist—you're not going to like what happens next._

### Act II: The Memory Leak

```cpp
void insertNode(int value) {
    Node* newNode = new Node(value);
    // Oops, forgot to link newNode to the list or update pointers
    // newNode is now lost in memory, but still taking up space
}
```

_A memory leak is like a ghost that haunts your program—invisible, consuming resources, and gradually slowing everything down._

### Act III: The Dangling Pointer

```cpp
Node* current = head;
delete current;  // Freed the memory
// ... later ...
std::cout << current->data;  // CRASH! current is now a dangling pointer
```

_Using a dangling pointer is like trying to visit a friend who moved away—the address exists, but what you find there won't be what you expect._

## 👑 Best Practices: The Wisdom of the Ancients

1. **Always Check for nullptr** before dereferencing pointers

2. **Maintain a Tail Pointer** for O(1) insertions at the end

3. **Keep Track of Size** to avoid having to count nodes

4. **Free Memory** in the destructor to prevent leaks

5. **Consider Using Smart Pointers** in modern C++ to manage memory automatically

6. **Handle Edge Cases** (empty list, single node) explicitly

_Not using smart pointers for your linked list in 2024 is like still using a flip phone—it works, but are you really living your best life?_

## 🏄‍♂️ Advanced Techniques: Level Up Your List Game

### The Runner Technique (Fast & Slow Pointers)

This technique uses two pointers moving at different speeds to solve problems like finding the middle element or detecting cycles.

```cpp
Node* findMiddle() {
    if (!head) return nullptr;

    Node* slow = head;
    Node* fast = head;

    // Fast moves twice as fast as slow
    while (fast && fast->next) {
        slow = slow->next;       // Move one step
        fast = fast->next->next; // Move two steps
    }

    // When fast reaches the end, slow is at the middle
    return slow;
}
```

_The runner technique is like having a faster friend scout ahead on a hike—by the time they reach the end, you're exactly in the middle._

### Reversing a Linked List

```cpp
void reverse() {
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    // Also update tail
    tail = head;

    while (current) {
        next = current->next;  // Store next node
        current->next = prev;  // Reverse the link
        prev = current;        // Move prev forward
        current = next;        // Move current forward
    }

    head = prev;  // Update head to the new front (old end)
}
```

_Reversing a linked list is like turning a one-way street into the opposite direction—you have to pick up each road sign, turn it around, and point it the other way._

## 🧩 Practice Problems: Test Your Skills

1. **Detect a Cycle**: Determine if a linked list has a cycle. Can you do it with O(1) space?

2. **Remove Duplicates**: Remove duplicate values from a sorted linked list.

3. **Find the Nth Node from the End**: Find the nth node from the end in a single pass.

4. **Merge Two Sorted Lists**: Merge two sorted linked lists into one sorted list.

5. **Palindrome Check**: Check if a linked list is a palindrome (reads the same forward and backward).

_These practice problems are like CrossFit for your coding muscles—painful during execution, but you'll be stronger afterward._

## 📚 LeetCode Practice Problems

To reinforce today's concepts, try these LeetCode problems:

**Easy:**

- #206: Reverse Linked List
- #21: Merge Two Sorted Lists
- #83: Remove Duplicates from Sorted List
- #141: Linked List Cycle
- #160: Intersection of Two Linked Lists

**Medium:**

- #19: Remove Nth Node From End of List
- #2: Add Two Numbers (represented as linked lists)
- #143: Reorder List
- #82: Remove Duplicates from Sorted List II
- #86: Partition List

**Hard:**

- #25: Reverse Nodes in k-Group
- #23: Merge k Sorted Lists
- #138: Copy List with Random Pointer

_LeetCode is like a gym membership for your algorithm skills—expensive in time, painful to start, but worth it in the long run._

## 🔮 Tomorrow's Preview:

Tomorrow we'll continue our linked list exploration with doubly linked lists and circular linked lists. We'll see how adding a `prev` pointer brings bidirectional movement but doubles the pointer maintenance headaches. Plus, we'll tackle some classic linked list interview problems that have made candidates sweat for decades.

_Doubly linked lists are like having a GPS with a "recalculate route" button—you can change direction at any time, but the system complexity doubles._

#DSAin45 #Day6 #LinkedLists #CPlusPlus #DataStructures #CodingInterview #pointers
