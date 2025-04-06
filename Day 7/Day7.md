# 🔁 DAY 7: Linked Lists - Part 2 (Doubly & Circular Lists) #DSAin45

Welcome to Day 7! Yesterday we explored singly linked lists—the data structure equivalent of a one-way street where U-turns require complex maneuvering. Today, we're upgrading to doubly linked lists and circular linked lists—because sometimes you need to go both ways, and sometimes the end is just the beginning.

_Singly linked lists are like people who only remember the names of people they're about to introduce you to, but never remember who introduced them._

## 🤔 Doubly Linked Lists: Looking Both Ways Before Crossing Memory

Doubly linked lists solve the "but what came before?" existential crisis of singly linked lists by adding a `prev` pointer to each node. Now we can traverse in both directions, like a time traveler with commitment issues.

```
      head                                tail
       ↓                                   ↓
 nullptr ← +------+------+ ⟷ +------+------+ ⟷ +------+------+ → nullptr
           | Data | Next | → | Data | Next | → | Data | Next |
           | Prev |      | ← | Prev |      | ← | Prev |      |
           +------+------+   +------+------+   +------+------+
```

> **🧠 Fun Fact**: The Linux kernel's list implementation is a circular doubly linked list. It uses a clever "container_of" macro to embed list pointers within other structures, creating an elegant universal list mechanism used throughout the kernel.

### Node Structure for Doubly Linked Lists

```cpp
struct Node {
    int data;      // The actual value we're storing
    Node* next;    // Pointer to the next node
    Node* prev;    // Pointer to the previous node

    // Constructor
    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};
```

_A node with null prev and next pointers is like someone with amnesia who also can't make new memories—therapeutically complex but algorithmically straightforward._

### Building a Doubly Linked List Class

```cpp
class DoublyLinkedList {
private:
    Node* head;  // Points to the first node
    Node* tail;  // Points to the last node
    int size;    // Keeps track of the number of nodes

public:
    // Constructor
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Other methods will be implemented below...
};
```

_Having both head and tail in a doubly linked list is like having GPS with both "where you've been" and "where you're going" functionality—exceptionally useful but uses twice the memory._

## 🔄 Doubly Linked List Operations

### Insertion Operations

#### 1. Insertion at the Beginning

```cpp
void insertAtBeginning(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    size++;
}
```

_Inserting at the beginning of a doubly linked list is like adding a new first chapter to a book—you need to update the previous chapter's reference to it, but there's no "previous" chapter to update._

#### 2. Insertion at the End

```cpp
void insertAtEnd(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    size++;
}
```

_Inserting at the end is like adding a caboose to a train—just connect it to the current last car and update who gets to wear the "I'm the end" hat._

#### 3. Insertion at a Specific Position

```cpp
bool insertAtPosition(int value, int position) {
    // Check if position is valid
    if (position < 0 || position > size) {
        return false;
    }

    // Special cases for beginning and end
    if (position == 0) {
        insertAtBeginning(value);
        return true;
    }

    if (position == size) {
        insertAtEnd(value);
        return true;
    }

    // Create new node
    Node* newNode = new Node(value);

    // Find the position
    Node* current;

    // Optimize traversal by starting from the closest end
    if (position <= size / 2) {
        // Start from head
        current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
    } else {
        // Start from tail
        current = tail;
        for (int i = size - 1; i >= position; i--) {
            current = current->prev;
        }
    }

    // Insert the new node
    newNode->next = current->next;
    newNode->prev = current;
    current->next->prev = newNode;
    current->next = newNode;

    size++;
    return true;
}
```

> **💡 Optimization Alert**: Unlike singly linked lists, doubly linked lists can optimize insertions (and other operations) by choosing the shortest path to the position: from head if it's in the first half, from tail if it's in the second half!

_Inserting in the middle of a doubly linked list is like scheduling a lunch meeting between two busy executives—you need to update both of their calendars._

### Deletion Operations

#### 1. Deletion from the Beginning

```cpp
bool deleteFromBeginning() {
    if (isEmpty()) {
        return false;
    }

    Node* temp = head;

    if (head == tail) {
        // Only one node
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }

    delete temp;
    size--;
    return true;
}
```

_Removing the first node is like firing your oldest employee—there's no one before them to notify, but someone else suddenly becomes the most senior._

#### 2. Deletion from the End

```cpp
bool deleteFromEnd() {
    if (isEmpty()) {
        return false;
    }

    Node* temp = tail;

    if (head == tail) {
        // Only one node
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }

    delete temp;
    size--;
    return true;
}
```

_Deleting from the end in a doubly linked list is O(1)—unlike singly linked lists, where it's more like trying to remember the name of the second-to-last person who left the party._

#### 3. Deletion from a Specific Position

```cpp
bool deleteFromPosition(int position) {
    // Check if position is valid
    if (position < 0 || position >= size) {
        return false;
    }

    // Special cases for beginning and end
    if (position == 0) {
        return deleteFromBeginning();
    }

    if (position == size - 1) {
        return deleteFromEnd();
    }

    // Find the node to delete
    Node* current;

    // Optimize traversal
    if (position <= size / 2) {
        // Start from head
        current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
    } else {
        // Start from tail
        current = tail;
        for (int i = size - 1; i > position; i--) {
            current = current->prev;
        }
    }

    // Update pointers to bypass this node
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // Delete the node
    delete current;
    size--;
    return true;
}
```

_Deleting a node from a doubly linked list is like disappearing from a conversation between two friends—they just start talking to each other directly like you were never there._

### Traversals: Forward and Backward

```cpp
// Forward traversal
void displayForward() const {
    Node* current = head;

    while (current) {
        std::cout << current->data << " ⟷ ";
        current = current->next;
    }

    std::cout << "nullptr" << std::endl;
}

// Backward traversal
void displayBackward() const {
    Node* current = tail;

    while (current) {
        std::cout << current->data << " ⟷ ";
        current = current->prev;
    }

    std::cout << "nullptr" << std::endl;
}
```

_Being able to traverse both ways is like having a rewind button for your data—incredibly useful when you need to check if you missed something, or just want to reminisce about the nodes you've visited._

## 🔄 Circular Linked Lists: Where the End Meets the Beginning

Circular linked lists take the concept of linked lists and add a dash of "Ouroboros"—the mythical serpent eating its own tail. Instead of a nullptr at the end, the last node points back to the first, creating an endless loop.

### Singly Circular Linked List

```
    +------+    +------+    +------+
    |      ↓    |      ↓    |      ↓
+-> | Data | -> | Data | -> | Data | -+
|   +------+    +------+    +------+  |
|                                     |
+-------------------------------------+
```

_A singly circular linked list is like a merry-go-round—once you're on, you'll keep going around unless you explicitly decide to get off._

### Doubly Circular Linked List

```
    +------+------+    +------+------+    +------+------+
    |      ↓      |    |      ↓      |    |      ↓      |
+-> | Data | Next | -> | Data | Next | -> | Data | Next | -+
|   | Prev |      | <- | Prev |      | <- | Prev |      |  |
|   +------+------+    +------+------+    +------+------+  |
|      ↑                                                    |
+------+----------------------------------------------------+
```

> **🧠 Fun Fact**: Circular lists are particularly useful in operating systems for round-robin scheduling, where processes are given CPU time in a circular fashion.

_A doubly circular linked list is like a time-loop movie where the characters can move both forward and backward in time, but always end up at the same spot eventually._

## 🏗️ Implementing a Circular Linked List

Let's implement a singly circular linked list, with the understanding that adding the 'prev' pointers would make it a doubly circular list.

```cpp
class CircularLinkedList {
private:
    Node* head;  // We only need one pointer, even for the "end"
    int size;

public:
    CircularLinkedList() : head(nullptr), size(0) {}

    bool isEmpty() const {
        return head == nullptr;
    }

    // Other methods will be implemented below...
};
```

_In a circular linked list, the concept of "head" and "tail" becomes philosophical—like asking "where does a circle begin and end?" The answer: wherever we arbitrarily decide._

### Key Operations for Circular Lists

#### 1. Insertion at the Beginning

```cpp
void insertAtBeginning(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = newNode;
        newNode->next = head;  // Points to itself
    } else {
        // Find the last node (which points to head)
        Node* last = head;
        while (last->next != head) {
            last = last->next;
        }

        // Insert new node
        newNode->next = head;
        head = newNode;

        // Update the last node to point to the new head
        last->next = head;
    }

    size++;
}
```

_Inserting a node at the beginning of a circular list is like joining a circular conga line as the leader—someone has to tell the last person in line that you're the new head of the party._

#### 2. Insertion at the End

```cpp
void insertAtEnd(int value) {
    Node* newNode = new Node(value);

    if (isEmpty()) {
        head = newNode;
        newNode->next = head;  // Points to itself
    } else {
        // Find the last node
        Node* last = head;
        while (last->next != head) {
            last = last->next;
        }

        // Insert new node
        last->next = newNode;
        newNode->next = head;
    }

    size++;
}
```

_Inserting at the end of a circular list is like being the last person to join a circular conga line—you're both the "end" and the one who connects back to the "beginning"._

#### 3. Deletion from the Beginning

```cpp
bool deleteFromBeginning() {
    if (isEmpty()) {
        return false;
    }

    if (head->next == head) {
        // Only one node
        delete head;
        head = nullptr;
    } else {
        // Find the last node
        Node* last = head;
        while (last->next != head) {
            last = last->next;
        }

        // Store old head
        Node* oldHead = head;

        // Update head
        head = head->next;

        // Update last node to point to new head
        last->next = head;

        // Delete old head
        delete oldHead;
    }

    size--;
    return true;
}
```

_Removing the first node from a circular list is like the lead dancer leaving the circle—someone else becomes the new lead, and the last dancer needs to hold their hand instead._

#### 4. Traversal (with Cycle Detection)

```cpp
void display() const {
    if (isEmpty()) {
        std::cout << "List is empty" << std::endl;
        return;
    }

    Node* current = head;

    // Use do-while to handle the circular nature
    do {
        std::cout << current->data << " -> ";
        current = current->next;
    } while (current != head);

    std::cout << "(back to head)" << std::endl;
}
```

_Traversing a circular list is like walking on a track—you need to know when you've completed a lap, or you'll keep going forever._

## 🤜 Linked List Interview Problems 🤛

Now that we've covered the fundamentals of various linked list types, let's tackle some classic interview problems. These are the problems that separate the linked list padawans from the linked list Jedi.

> **🧠 Fun Fact**: Linked list problems are so common in coding interviews that a Google engineer once joked, "If you can reverse a linked list in your sleep, you're halfway to getting hired."

### 1. Reversing a Linked List (Singly or Doubly)

```cpp
// For singly linked list
void reverse() {
    if (isEmpty() || size == 1) {
        return; // Nothing to reverse
    }

    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current) {
        next = current->next;  // Store next
        current->next = prev;  // Reverse link
        prev = current;        // Move prev forward
        current = next;        // Move current forward
    }

    // Update head (and tail for efficiency)
    tail = head;
    head = prev;
}

// For doubly linked list
void reverse() {
    if (isEmpty() || size == 1) {
        return; // Nothing to reverse
    }

    Node* current = head;
    Node* temp = nullptr;

    // Swap next and prev pointers for all nodes
    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; // Move to next node (which is now prev)
    }

    // Swap head and tail
    temp = head;
    head = tail;
    tail = temp;
}
```

_Reversing a linked list is like convincing an entire line of people to turn around and hold hands with the person who was behind them—fundamentally simple but requires careful coordination to avoid dropped connections._

### 2. Detecting a Cycle

The famous "tortoise and hare" algorithm (Floyd's Cycle-Finding Algorithm):

```cpp
bool hasCycle() {
    if (isEmpty() || size == 1) {
        return false;
    }

    Node* slow = head;
    Node* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;       // Move one step
        fast = fast->next->next; // Move two steps

        if (slow == fast) {
            return true; // Cycle detected
        }
    }

    return false; // No cycle
}
```

_The cycle detection algorithm is like having two runners on a track—if it's a regular track (with a finish line), the faster runner will finish first. But if it's a circular track, the faster runner will eventually lap the slower one, and they'll meet again._

### 3. Finding the Middle Element

```cpp
Node* findMiddle() {
    if (isEmpty()) {
        return nullptr;
    }

    Node* slow = head;
    Node* fast = head;

    // Fast moves twice as fast as slow
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow; // Middle node (or right middle for even length)
}
```

_Finding the middle of a linked list with the "runner technique" is like having a sprinter run twice as fast as a jogger—when the sprinter reaches the finish line, the jogger will be exactly halfway._

### 4. Removing Duplicates from a Sorted List

```cpp
void removeDuplicates() {
    if (isEmpty() || size == 1) {
        return; // No duplicates possible
    }

    Node* current = head;

    while (current != nullptr && current->next != nullptr) {
        if (current->data == current->next->data) {
            // Duplicate found - remove next node
            Node* duplicate = current->next;
            current->next = duplicate->next;

            // Update prev pointer if it's a doubly linked list
            if (duplicate->next != nullptr) {
                duplicate->next->prev = current;
            }

            // If we removed the tail, update it
            if (duplicate == tail) {
                tail = current;
            }

            delete duplicate;
            size--;
        } else {
            // Move to next node
            current = current->next;
        }
    }
}
```

_Removing duplicates from a sorted list is like having a bouncer check IDs—"You look exactly like the person who just went in, so you can't enter!"_

### 5. Merging Two Sorted Lists

```cpp
SinglyLinkedList mergeSortedLists(const SinglyLinkedList& list1, const SinglyLinkedList& list2) {
    SinglyLinkedList result;
    Node* current1 = list1.head;
    Node* current2 = list2.head;

    // Traverse both lists and add smaller element to result
    while (current1 != nullptr && current2 != nullptr) {
        if (current1->data <= current2->data) {
            result.insertAtEnd(current1->data);
            current1 = current1->next;
        } else {
            result.insertAtEnd(current2->data);
            current2 = current2->next;
        }
    }

    // Add remaining elements from list1
    while (current1 != nullptr) {
        result.insertAtEnd(current1->data);
        current1 = current1->next;
    }

    // Add remaining elements from list2
    while (current2 != nullptr) {
        result.insertAtEnd(current2->data);
        current2 = current2->next;
    }

    return result;
}
```

_Merging sorted lists is like "zippering" two pre-sorted lines of people into a single sorted line—always take the shorter person from the front of either line._

## 💪 Advanced Techniques and Variations

### 1. XOR Linked List

A memory-efficient variation where instead of storing direct pointers, each node stores the XOR of addresses of previous and next nodes.

```cpp
struct XORNode {
    int data;
    XORNode* npx;  // XOR of next and previous node addresses

    XORNode(int value) : data(value), npx(nullptr) {}
};
```

_An XOR linked list is like a spy who only remembers the combined face of the person in front and behind them in line—theoretically efficient but practically confusing._

> **🧠 Fun Fact**: XOR linked lists are considered a "clever trick" but are rarely used in practice because they make debugging extremely difficult and are incompatible with garbage collection.

### 2. Skip Lists

A probabilistic data structure that allows O(log n) search complexity within a linked list structure.

```
Level 3 -->  1 -------------------------> 9
Level 2 -->  1 ------------> 6 --------> 9
Level 1 -->  1 -----> 4 ----> 6 -----> 9
Level 0 -->  1 -> 3 -> 4 -> 5 -> 6 -> 7 -> 9
```

_Skip lists are like express elevators in a tall building—they let you skip several floors to get closer to your destination faster, then take local stops for the final approach._

## 📊 Comparing Linked List Types

| Feature                      | Singly Linked | Doubly Linked | Circular Singly | Circular Doubly |
| ---------------------------- | ------------- | ------------- | --------------- | --------------- |
| Memory per Node              | data + 1 ptr  | data + 2 ptrs | data + 1 ptr    | data + 2 ptrs   |
| Backward Traversal           | O(n)          | O(1)          | O(n)            | O(1)            |
| Insertion at Beginning       | O(1)          | O(1)          | O(n)\*          | O(1)            |
| Insertion at End (with tail) | O(1)          | O(1)          | O(1)            | O(1)            |
| Insertion at End (no tail)   | O(n)          | O(n)          | O(n)            | O(n)            |
| Deletion from End            | O(n)          | O(1)          | O(n)            | O(1)            |
| Find Middle Element          | O(n)          | O(n)          | O(n)            | O(n)            |
| Join Two Lists               | O(1)          | O(1)          | O(1)            | O(1)            |

_\* O(n) because we need to find the last node to update its pointer to the new head_

_This comparison table is like a dating profile for linked lists—choose the one with the features that match your specific needs, but remember they all have some baggage._

## 🌎 Real-World Applications

### Doubly Linked Lists

1. **Browser History**: Forward and backward navigation in web browsers
2. **Music Players**: Previous/next song functionality
3. **Text Editors**: Undo/redo functionality
4. **LRU Caches**: Most Recently Used item tracking
5. **Operating Systems**: Process scheduling

_The browser's back and forward buttons are the most relatable example of a doubly linked list in action—your browsing history is just a chain of web pages linked together._

### Circular Linked Lists

1. **Round-Robin Scheduling**: CPU scheduling algorithm
2. **Multiplayer Games**: Taking turns in a fixed order
3. **Circular Buffers**: Audio streaming, keyboard buffers
4. **Josephus Problem**: Algorithm for counting out people standing in a circle
5. **Closed Loop Control Systems**: Feedback control systems

> **🧠 Fun Fact**: The Josephus problem (deciding where to stand in a circle to be the last survivor) is a famous problem solved using circular linked lists. Legend has it that Flavius Josephus used this mathematical insight to save himself during the Roman siege of Jotapata.

_Circular linked lists are perfect for anything that needs to loop back to the beginning, like my attempts to stick to a diet._

## 🧩 Advanced Interview Problems

1. **LRU Cache Implementation**: Implement a Least Recently Used cache with O(1) operations
2. **Clone a Linked List with Random Pointers**: Deep copy a list where each node has an additional random pointer
3. **Flatten a Multilevel Doubly Linked List**: Convert a multilevel list to a single-level list
4. **Add Two Numbers Represented by Linked Lists**: Add two numbers stored in reverse order in linked lists
5. **Reorder List**: Reorder a list to L₀ → Lₙ → L₁ → Lₙ₋₁ → ...

_These advanced problems are like the boss levels in a linked list video game—they combine multiple techniques and really test your understanding of pointers and memory manipulation._

## 📚 LeetCode Practice Problems

To reinforce today's concepts, try these LeetCode problems:

**Easy:**

- #21: Merge Two Sorted Lists
- #83: Remove Duplicates from Sorted List
- #141: Linked List Cycle
- #160: Intersection of Two Linked Lists
- #876: Middle of the Linked List

**Medium:**

- #19: Remove Nth Node From End of List
- #92: Reverse Linked List II (reverse a sublist)
- #61: Rotate List
- #82: Remove Duplicates from Sorted List II
- #86: Partition List

**Hard:**

- #23: Merge k Sorted Lists
- #25: Reverse Nodes in k-Group
- #138: Copy List with Random Pointer
- #146: LRU Cache (using doubly linked list)
- #460: LFU Cache

_These LeetCode problems are like gym equipment for your linked list muscles—regular workouts will make you stronger, but there will be some soreness along the way._

## 🔮 Tomorrow's Preview:

Tomorrow we'll shift gears and dive into Stacks—linked lists' more disciplined cousins who follow strict LIFO (Last In, First Out) protocol. We'll see how these simple structures solve surprisingly complex problems and enable some of the most fundamental algorithms in computer science.

_Stacks are like linked lists that went to military school—highly disciplined and surprisingly effective despite their simplicity._

#DSAin45 #Day7 #LinkedLists #DoublyLinkedLists #CircularLinkedLists #CPlusPlus #DataStructures #CodingInterview
