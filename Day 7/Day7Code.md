// Day 7: Doubly & Circular Linked Lists - #DSAin45
// A comprehensive implementation with detailed comments

#include <iostream>
#include <stdexcept>

//========== DOUBLY LINKED LIST ==========//

// Node structure for Doubly Linked List
struct DoublyNode {
int data; // The value we're storing
DoublyNode* next; // Pointer to the next node
DoublyNode* prev; // Pointer to the previous node

    // Constructor to make node creation easier
    DoublyNode(int value) : data(value), next(nullptr), prev(nullptr) {
        std::cout << "Created node with value: " << value << std::endl;
    }

};

// Doubly Linked List class
class DoublyLinkedList {
private:
DoublyNode* head; // Points to the first node
DoublyNode* tail; // Points to the last node
int size; // Keeps track of the list size

public:
// Constructor - Initialize an empty list
DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {
std::cout << "Creating a new doubly linked list" << std::endl;
}

    // Destructor - Clean up memory
    ~DoublyLinkedList() {
        std::cout << "Destroying doubly linked list..." << std::endl;
        clear();
    }

    // Clear the entire list and free memory
    void clear() {
        DoublyNode* current = head;

        // Delete each node one by one
        while (current) {
            DoublyNode* next = current->next;
            delete current;
            current = next;
        }

        // Reset member variables
        head = tail = nullptr;
        size = 0;
        std::cout << "Doubly linked list cleared" << std::endl;
    }

    // Check if list is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Get the size of the list
    int getSize() const {
        return size;
    }

    // Insert a node at the beginning (prepend) - O(1)
    void insertAtBeginning(int value) {
        DoublyNode* newNode = new DoublyNode(value);

        if (isEmpty()) {
            // If list is empty, both head and tail point to the new node
            head = tail = newNode;
        } else {
            // Make the new node point to the current head
            newNode->next = head;
            // Current head's prev points to the new node
            head->prev = newNode;
            // Update head to point to the new node
            head = newNode;
        }

        size++;
        std::cout << value << " inserted at the beginning" << std::endl;
    }

    // Insert a node at the end (append) - O(1)
    void insertAtEnd(int value) {
        DoublyNode* newNode = new DoublyNode(value);

        if (isEmpty()) {
            // If list is empty, both head and tail point to the new node
            head = tail = newNode;
        } else {
            // Make the last node point to the new node
            tail->next = newNode;
            // New node's prev points to the current tail
            newNode->prev = tail;
            // Update tail to point to the new node
            tail = newNode;
        }

        size++;
        std::cout << value << " inserted at the end" << std::endl;
    }

    // Insert a node at a specific position - O(n)
    bool insertAtPosition(int value, int position) {
        // Check if position is valid
        if (position < 0 || position > size) {
            std::cout << "Invalid position: " << position << std::endl;
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

        // Create a new node
        DoublyNode* newNode = new DoublyNode(value);

        // Find the node just before the position
        // Optimize by starting from the closest end
        DoublyNode* current;

        if (position <= size / 2) {
            // Start from head (closer to the position)
            current = head;
            for (int i = 0; i < position - 1; i++) {
                current = current->next;
            }
        } else {
            // Start from tail (closer to the position)
            current = tail;
            for (int i = size - 1; i >= position; i--) {
                current = current->prev;
            }
        }

        // Update pointers to insert the new node
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;

        size++;
        std::cout << value << " inserted at position " << position << std::endl;
        return true;
    }

    // Delete a node from the beginning - O(1)
    bool deleteFromBeginning() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }

        DoublyNode* temp = head;

        if (head == tail) {
            // Only one node
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }

        int value = temp->data;
        delete temp;

        size--;
        std::cout << value << " deleted from the beginning" << std::endl;
        return true;
    }

    // Delete a node from the end - O(1)
    bool deleteFromEnd() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }

        DoublyNode* temp = tail;

        if (head == tail) {
            // Only one node
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }

        int value = temp->data;
        delete temp;

        size--;
        std::cout << value << " deleted from the end" << std::endl;
        return true;
    }

    // Delete a node from a specific position - O(n)
    bool deleteFromPosition(int position) {
        // Check if position is valid
        if (position < 0 || position >= size) {
            std::cout << "Invalid position: " << position << std::endl;
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
        DoublyNode* current;

        if (position <= size / 2) {
            // Start from head (closer to position)
            current = head;
            for (int i = 0; i < position; i++) {
                current = current->next;
            }
        } else {
            // Start from tail (closer to position)
            current = tail;
            for (int i = size - 1; i > position; i--) {
                current = current->prev;
            }
        }

        // Update pointers to bypass this node
        current->prev->next = current->next;
        current->next->prev = current->prev;

        // Store value for reporting
        int value = current->data;

        // Delete the node
        delete current;

        size--;
        std::cout << value << " deleted from position " << position << std::endl;
        return true;
    }

    // Display the list from front to back
    void displayForward() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        DoublyNode* current = head;
        std::cout << "List (forward): nullptr ⟷ ";

        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " ⟷ ";
            }
            current = current->next;
        }

        std::cout << " ⟷ nullptr" << std::endl;
    }

    // Display the list from back to front
    void displayBackward() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        DoublyNode* current = tail;
        std::cout << "List (backward): nullptr ⟷ ";

        while (current) {
            std::cout << current->data;
            if (current->prev) {
                std::cout << " ⟷ ";
            }
            current = current->prev;
        }

        std::cout << " ⟷ nullptr" << std::endl;
    }

    // Reverse the list - O(n)
    void reverse() {
        if (isEmpty() || size == 1) {
            std::cout << "No need to reverse (empty or single element)" << std::endl;
            return;
        }

        DoublyNode* current = head;
        DoublyNode* temp = nullptr;

        // Swap next and prev pointers for all nodes
        while (current) {
            // Store current->prev in temp
            temp = current->prev;

            // Swap prev and next pointers
            current->prev = current->next;
            current->next = temp;

            // Move to next node (which is now prev)
            current = current->prev;
        }

        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;

        std::cout << "List has been reversed" << std::endl;
    }

    // Find the middle node (using runner technique)
    void findMiddle() const {
        if (isEmpty()) {
            std::cout << "List is empty, no middle element" << std::endl;
            return;
        }

        DoublyNode* slow = head;
        DoublyNode* fast = head;

        // Fast pointer moves twice as fast as slow
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        std::cout << "Middle element: " << slow->data << std::endl;
    }

    // Remove duplicates from a sorted list
    void removeDuplicates() {
        if (isEmpty() || size == 1) {
            std::cout << "No duplicates to remove (empty or single element)" << std::endl;
            return;
        }

        DoublyNode* current = head;

        while (current != nullptr && current->next != nullptr) {
            if (current->data == current->next->data) {
                // Duplicate found - remove next node
                DoublyNode* duplicate = current->next;

                current->next = duplicate->next;

                // Update prev pointer of the node after duplicate
                if (duplicate->next != nullptr) {
                    duplicate->next->prev = current;
                }

                // If we removed the tail, update it
                if (duplicate == tail) {
                    tail = current;
                }

                delete duplicate;
                size--;
                std::cout << "Removed a duplicate value: " << current->data << std::endl;
            } else {
                // Move to next node
                current = current->next;
            }
        }
    }

};

//========== CIRCULAR LINKED LIST ==========//

// Node structure for Circular Linked List
struct CircularNode {
int data;
CircularNode\* next;

    // Constructor
    CircularNode(int value) : data(value), next(nullptr) {
        std::cout << "Created circular node with value: " << value << std::endl;
    }

};

// Circular Linked List class
class CircularLinkedList {
private:
CircularNode\* head; // Points to the first node
int size; // Keeps track of the number of nodes

public:
// Constructor
CircularLinkedList() : head(nullptr), size(0) {
std::cout << "Creating a new circular linked list" << std::endl;
}

    // Destructor
    ~CircularLinkedList() {
        std::cout << "Destroying circular linked list..." << std::endl;
        clear();
    }

    // Clear the list and free memory
    void clear() {
        if (isEmpty()) {
            return;
        }

        CircularNode* current = head->next;

        // Handle circular nature - stop when we get back to head
        while (current != head) {
            CircularNode* next = current->next;
            delete current;
            current = next;
        }

        // Delete the head node
        delete head;

        head = nullptr;
        size = 0;
        std::cout << "Circular linked list cleared" << std::endl;
    }

    // Check if list is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Get size of the list
    int getSize() const {
        return size;
    }

    // Insert at the beginning - O(n) due to finding the last node
    void insertAtBeginning(int value) {
        CircularNode* newNode = new CircularNode(value);

        if (isEmpty()) {
            // First node points to itself
            head = newNode;
            newNode->next = head;
        } else {
            // Find the last node (which points back to head)
            CircularNode* last = head;
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
        std::cout << value << " inserted at the beginning" << std::endl;
    }

    // Insert at the end - O(n) due to finding the last node
    void insertAtEnd(int value) {
        CircularNode* newNode = new CircularNode(value);

        if (isEmpty()) {
            // First node points to itself
            head = newNode;
            newNode->next = head;
        } else {
            // Find the last node
            CircularNode* last = head;
            while (last->next != head) {
                last = last->next;
            }

            // Insert new node
            last->next = newNode;
            newNode->next = head;
        }

        size++;
        std::cout << value << " inserted at the end" << std::endl;
    }

    // Insert at a specific position
    bool insertAtPosition(int value, int position) {
        // Check if position is valid
        if (position < 0 || position > size) {
            std::cout << "Invalid position: " << position << std::endl;
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

        // Create a new node
        CircularNode* newNode = new CircularNode(value);

        // Find the node just before the position
        CircularNode* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        // Insert the new node
        newNode->next = current->next;
        current->next = newNode;

        size++;
        std::cout << value << " inserted at position " << position << std::endl;
        return true;
    }

    // Delete from the beginning
    bool deleteFromBeginning() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }

        int value = head->data;

        if (head->next == head) {
            // Only one node
            delete head;
            head = nullptr;
        } else {
            // Find the last node
            CircularNode* last = head;
            while (last->next != head) {
                last = last->next;
            }

            // Update last node to point to the new head
            CircularNode* newHead = head->next;
            last->next = newHead;

            // Delete the old head
            delete head;

            // Update head
            head = newHead;
        }

        size--;
        std::cout << value << " deleted from the beginning" << std::endl;
        return true;
    }

    // Delete from the end
    bool deleteFromEnd() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }

        if (head->next == head) {
            // Only one node
            int value = head->data;
            delete head;
            head = nullptr;
            size = 0;
            std::cout << value << " deleted from the end (was the only element)" << std::endl;
            return true;
        }

        // Find the second-to-last node
        CircularNode* current = head;
        while (current->next->next != head) {
            current = current->next;
        }

        // Store value for reporting
        int value = current->next->data;

        // Delete the last node
        delete current->next;

        // Update second-to-last to point to head
        current->next = head;

        size--;
        std::cout << value << " deleted from the end" << std::endl;
        return true;
    }

    // Delete from a specific position
    bool deleteFromPosition(int position) {
        // Check if position is valid
        if (position < 0 || position >= size) {
            std::cout << "Invalid position: " << position << std::endl;
            return false;
        }

        // Special cases for beginning and end
        if (position == 0) {
            return deleteFromBeginning();
        }

        if (position == size - 1) {
            return deleteFromEnd();
        }

        // Find the node just before the one to delete
        CircularNode* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        // Store node to be deleted and its value
        CircularNode* nodeToDelete = current->next;
        int value = nodeToDelete->data;

        // Update pointer to skip the node to delete
        current->next = nodeToDelete->next;

        // Delete the node
        delete nodeToDelete;

        size--;
        std::cout << value << " deleted from position " << position << std::endl;
        return true;
    }

    // Display the list
    void display() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        CircularNode* current = head;
        std::cout << "Circular list: ";

        // Use do-while to handle the circular nature
        do {
            std::cout << current->data;
            current = current->next;

            if (current != head) {
                std::cout << " -> ";
            }
        } while (current != head);

        std::cout << " -> (back to " << head->data << ")" << std::endl;
    }

    // Search for a value
    bool search(int value) const {
        if (isEmpty()) {
            return false;
        }

        CircularNode* current = head;

        // Use do-while to handle the circular nature
        do {
            if (current->data == value) {
                std::cout << value << " found in the list" << std::endl;
                return true;
            }
            current = current->next;
        } while (current != head);

        std::cout << value << " not found in the list" << std::endl;
        return false;
    }

    // Find the middle element using runner technique
    void findMiddle() const {
        if (isEmpty()) {
            std::cout << "List is empty, no middle element" << std::endl;
            return;
        }

        if (head->next == head) {
            std::cout << "Middle element (only one): " << head->data << std::endl;
            return;
        }

        CircularNode* slow = head;
        CircularNode* fast = head;

        // For circular list, need additional check to avoid infinite loop
        do {
            slow = slow->next;
            fast = fast->next->next;
        } while (fast != head && fast->next != head);

        std::cout << "Middle element: " << slow->data << std::endl;
    }

    // Josephus problem solution
    int josephus(int k) {
        if (isEmpty()) {
            std::cout << "List is empty, cannot solve Josephus problem" << std::endl;
            return -1;
        }

        // Start at the head
        CircularNode* current = head;
        CircularNode* prev = nullptr;

        // Continue until only one node remains
        while (size > 1) {
            // Find the k-th node (and the one before it)
            for (int i = 0; i < k - 1; i++) {
                prev = current;
                current = current->next;
            }

            // Store the value to be eliminated
            int eliminated = current->data;
            std::cout << "Josephus elimination: " << eliminated << std::endl;

            // Store the next node
            CircularNode* nextNode = current->next;

            // Update pointers to remove current node
            if (current == head) {
                // Find the last node to update its next pointer
                CircularNode* last = head;
                while (last->next != head) {
                    last = last->next;
                }
                head = nextNode;
                last->next = head;
            } else {
                prev->next = nextNode;
            }

            // Delete the node
            delete current;

            // Move to the next node
            current = nextNode;

            size--;
        }

        // Return the last remaining value
        int survivor = head->data;
        std::cout << "Josephus survivor: " << survivor << std::endl;
        return survivor;
    }

};

//========== LINKED LIST INTERVIEW PROBLEMS ==========//

// Helper class for standalone interview problem demonstrations
class LinkedListProblems {
public:
// Example of reversing a linked list (simplified node structure)
static void demonstrateReverse() {
std::cout << "\n=== DEMONSTRATION: REVERSING A LINKED LIST ===\n";

        // Create some nodes for demonstration
        struct Node {
            int data;
            Node* next;
            Node(int val) : data(val), next(nullptr) {}
        };

        // Create a sample list: 1 -> 2 -> 3 -> 4 -> 5
        Node* head = new Node(1);
        head->next = new Node(2);
        head->next->next = new Node(3);
        head->next->next->next = new Node(4);
        head->next->next->next->next = new Node(5);

        // Display original list
        std::cout << "Original list: ";
        Node* current = head;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;

        // Reverse the list
        Node* prev = nullptr;
        current = head;
        Node* next = nullptr;

        while (current) {
            next = current->next;    // Store next
            current->next = prev;    // Reverse link
            prev = current;          // Move prev forward
            current = next;          // Move current forward
        }

        head = prev;  // Update head to new front

        // Display reversed list
        std::cout << "Reversed list: ";
        current = head;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;

        // Clean up memory
        current = head;
        while (current) {
            next = current->next;
            delete current;
            current = next;
        }
    }

    // Cycle detection using Floyd's algorithm
    static void demonstrateCycleDetection() {
        std::cout << "\n=== DEMONSTRATION: CYCLE DETECTION ===\n";

        // Create some nodes for demonstration
        struct Node {
            int data;
            Node* next;
            Node(int val) : data(val), next(nullptr) {}
        };

        // Create a list with a cycle: 1 -> 2 -> 3 -> 4 -> 5 -> 3 (cycle)
        Node* head = new Node(1);
        head->next = new Node(2);
        head->next->next = new Node(3);
        head->next->next->next = new Node(4);
        head->next->next->next->next = new Node(5);

        // Create a cycle for demonstration (5 points back to 3)
        Node* cycleNode = head->next->next;  // Node with value 3
        head->next->next->next->next->next = cycleNode;

        std::cout << "Created list with cycle: 1 -> 2 -> 3 -> 4 -> 5 -> (back to 3)" << std::endl;

        // Detect cycle using Floyd's algorithm
        Node* slow = head;
        Node* fast = head;
        bool hasCycle = false;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;          // Move one step
            fast = fast->next->next;    // Move two steps

            if (slow == fast) {
                hasCycle = true;
                break;
            }
        }

        if (hasCycle) {
            std::cout << "Cycle detected!" << std::endl;

            // Find the start of the cycle
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }

            std::cout << "Cycle starts at node with value: " << slow->data << std::endl;
        } else {
            std::cout << "No cycle detected" << std::endl;
        }

        // Break the cycle before freeing memory
        head->next->next->next->next->next = nullptr;

        // Clean up memory
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // Merge two sorted lists
    static void demonstrateMergeSortedLists() {
        std::cout << "\n=== DEMONSTRATION: MERGING SORTED LISTS ===\n";

        // Create some nodes for demonstration
        struct Node {
            int data;
            Node* next;
            Node(int val) : data(val), next(nullptr) {}
        };

        // Create first sorted list: 1 -> 3 -> 5 -> 7
        Node* list1 = new Node(1);
        list1->next = new Node(3);
        list1->next->next = new Node(5);
        list1->next->next->next = new Node(7);

        // Create second sorted list: 2 -> 4 -> 6 -> 8 -> 10
        Node* list2 = new Node(2);
        list2->next = new Node(4);
        list2->next->next = new Node(6);
        list2->next->next->next = new Node(8);
        list2->next->next->next->next = new Node(10);

        // Display the original lists
        std::cout << "List 1: ";
        Node* current = list1;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;

        std::cout << "List 2: ";
        current = list2;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;

        // Merge the lists
        Node dummy(0);  // Dummy node to simplify code
        Node* tail = &dummy;

        while (list1 && list2) {
            if (list1->data <= list2->data) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        // Attach remaining elements
        if (list1) tail->next = list1;
        if (list2) tail->next = list2;

        // Display merged list
        std::cout << "Merged list: ";
        current = dummy.next;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << " -> ";
            current = current->next;
        }
        std::cout << std::endl;

        // Clean up memory - more complex here as we've rearranged pointers
        // This is a simplified cleanup that might skip some nodes in this demonstration
        current = dummy.next;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

};

//========== MAIN FUNCTION ==========//

int main() {
std::cout << "===== DOUBLY LINKED LIST DEMONSTRATION =====" << std::endl;

    // Create a doubly linked list
    DoublyLinkedList dlist;

    // Test insertion operations
    dlist.insertAtBeginning(10);
    dlist.insertAtEnd(30);
    dlist.insertAtPosition(20, 1);
    dlist.displayForward();
    dlist.displayBackward();

    // Add more elements
    dlist.insertAtBeginning(5);
    dlist.insertAtEnd(40);
    dlist.displayForward();

    // Test deletion operations
    dlist.deleteFromBeginning();
    dlist.displayForward();

    dlist.deleteFromEnd();
    dlist.displayForward();

    dlist.deleteFromPosition(1);
    dlist.displayForward();

    // Find the middle element
    dlist.findMiddle();

    // Reverse the list
    dlist.reverse();
    dlist.displayForward();
    dlist.displayBackward();

    // Test with duplicates (create a sorted list with duplicates)
    dlist.clear();
    dlist.insertAtEnd(10);
    dlist.insertAtEnd(10);
    dlist.insertAtEnd(20);
    dlist.insertAtEnd(20);
    dlist.insertAtEnd(20);
    dlist.insertAtEnd(30);
    dlist.displayForward();

    // Remove duplicates
    dlist.removeDuplicates();
    dlist.displayForward();

    std::cout << "\n===== CIRCULAR LINKED LIST DEMONSTRATION =====" << std::endl;

    // Create a circular linked list
    CircularLinkedList clist;

    // Test insertion operations
    clist.insertAtBeginning(10);
    clist.insertAtEnd(30);
    clist.insertAtPosition(20, 1);
    clist.display();

    // Add more elements
    clist.insertAtBeginning(5);
    clist.insertAtEnd(40);
    clist.display();

    // Test deletion operations
    clist.deleteFromBeginning();
    clist.display();

    clist.deleteFromEnd();
    clist.display();

    clist.deleteFromPosition(1);
    clist.display();

    // Find the middle element
    clist.findMiddle();

    // Test search
    clist.search(10);
    clist.search(50);

    // Josephus problem demonstration
    CircularLinkedList josephusList;
    for (int i = 1; i <= 7; i++) {
        josephusList.insertAtEnd(i);
    }
    josephusList.display();
    josephusList.josephus(3);  // Every 3rd person is eliminated

    std::cout << "\n===== LINKED LIST INTERVIEW PROBLEMS =====" << std::endl;

    // Demonstrate interview problems
    LinkedListProblems::demonstrateReverse();
    LinkedListProblems::demonstrateCycleDetection();
    LinkedListProblems::demonstrateMergeSortedLists();

    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;

    return 0;

}
