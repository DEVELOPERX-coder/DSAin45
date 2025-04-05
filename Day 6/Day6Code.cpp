// Day 6: Singly Linked Lists - #DSAin45
// A comprehensive implementation with detailed comments

#include <iostream>
#include <stdexcept>

// Node structure - the building block of our linked list
struct Node {
    int data;       // The value we're storing (could be any type)
    Node* next;     // Pointer to the next node in the sequence
    
    // Constructor to make node creation easier
    // Initialize data and set next to nullptr by default
    Node(int value) : data(value), next(nullptr) {}
};

// Main SinglyLinkedList class - the wrapper around our nodes
class SinglyLinkedList {
private:
    Node* head;     // Points to the first node in the list
    Node* tail;     // Points to the last node (optional but makes appending O(1))
    int size;       // Keeps track of the list size
    
public:
    // Constructor - Initialize an empty list
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {
        std::cout << "Creating a shiny new empty linked list" << std::endl;
    }
    
    // Destructor - Very important to prevent memory leaks!
    ~SinglyLinkedList() {
        std::cout << "Cleaning up the linked list..." << std::endl;
        clear();
    }
    
    // Clear the entire list and free memory
    void clear() {
        Node* current = head;
        Node* next = nullptr;
        
        // Delete each node one by one
        while (current) {
            next = current->next;  // Save next node before deleting current
            delete current;        // Free memory
            current = next;        // Move to the next node
        }
        
        // Reset member variables
        head = tail = nullptr;
        size = 0;
        std::cout << "List cleared and memory freed" << std::endl;
    }
    
    // Check if list is empty - a simple utility method
    bool isEmpty() const {
        return head == nullptr;
    }
    
    // Get the current size of the list
    int getSize() const {
        return size;
    }
    
    // Insert a node at the beginning (prepend) - O(1)
    void insertAtBeginning(int value) {
        Node* newNode = new Node(value);
        
        if (isEmpty()) {
            // If list is empty, the new node is both head and tail
            head = tail = newNode;
        } else {
            // Make the new node point to the current head
            newNode->next = head;
            // Update head to point to the new node
            head = newNode;
        }
        
        size++;
        std::cout << value << " inserted at the beginning" << std::endl;
    }
    
    // Insert a node at the end (append) - O(1) with tail pointer
    void insertAtEnd(int value) {
        Node* newNode = new Node(value);
        
        if (isEmpty()) {
            // If list is empty, the new node is both head and tail
            head = tail = newNode;
        } else {
            // Make the last node point to the new node
            tail->next = newNode;
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
        std::cout << value << " inserted at position " << position << std::endl;
        return true;
    }
    
    // Delete a node from the beginning - O(1)
    bool deleteFromBeginning() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }
        
        // Save the head node
        Node* temp = head;
        
        // Update head to point to the next node
        head = head->next;
        
        // If list becomes empty after deletion, update tail
        if (head == nullptr) {
            tail = nullptr;
        }
        
        // Store the value for reporting
        int value = temp->data;
        
        // Delete the old head node
        delete temp;
        
        size--;
        std::cout << value << " deleted from the beginning" << std::endl;
        return true;
    }
    
    // Delete a node from the end - O(n)
    bool deleteFromEnd() {
        if (isEmpty()) {
            std::cout << "Cannot delete: List is empty" << std::endl;
            return false;
        }
        
        // If there's only one node
        if (head == tail) {
            int value = head->data;
            delete head;
            head = tail = nullptr;
            size = 0;
            std::cout << value << " deleted from the end (was the only element)" << std::endl;
            return true;
        }
        
        // Traverse to the second-to-last node
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        
        // Store the value for reporting
        int value = tail->data;
        
        // Delete the last node
        delete tail;
        
        // Update tail to point to the second-to-last node
        tail = current;
        tail->next = nullptr;
        
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
        
        // Traverse to the node just before the position
        Node* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        
        // Save the node to be deleted
        Node* temp = current->next;
        
        // Store the value for reporting
        int value = temp->data;
        
        // Update the next pointer to skip the node to be deleted
        current->next = temp->next;
        
        // Delete the node
        delete temp;
        
        size--;
        std::cout << value << " deleted from position " << position << std::endl;
        return true;
    }
    
    // Search for a value in the list - O(n)
    bool search(int value) const {
        Node* current = head;
        int position = 0;
        
        while (current) {
            if (current->data == value) {
                std::cout << value << " found at position " << position << std::endl;
                return true;
            }
            current = current->next;
            position++;
        }
        
        std::cout << value << " not found in the list" << std::endl;
        return false;
    }
    
    // Get value at a specific position - O(n)
    bool getValueAt(int position, int& value) const {
        if (position < 0 || position >= size) {
            std::cout << "Invalid position: " << position << std::endl;
            return false;
        }
        
        Node* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        
        value = current->data;
        return true;
    }
    
    // Update value at a specific position - O(n)
    bool updateValueAt(int position, int value) {
        if (position < 0 || position >= size) {
            std::cout << "Invalid position: " << position << std::endl;
            return false;
        }
        
        Node* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        
        int oldValue = current->data;
        current->data = value;
        std::cout << "Updated position " << position << " from " << oldValue << " to " << value << std::endl;
        return true;
    }
    
    // Display the linked list - O(n)
    void display() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }
        
        Node* current = head;
        std::cout << "List contents: ";
        
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        
        std::cout << std::endl;
        std::cout << "Size: " << size << std::endl;
    }
    
    // Find the middle node using the runner technique (fast & slow pointers)
    void findMiddle() const {
        if (isEmpty()) {
            std::cout << "Cannot find middle: List is empty" << std::endl;
            return;
        }
        
        Node* slow = head;
        Node* fast = head;
        
        // Fast moves twice as fast as slow
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;       // Move one step
            fast = fast->next->next; // Move two steps
        }
        
        std::cout << "Middle element: " << slow->data << std::endl;
    }
    
    // Reverse the linked list - O(n)
    void reverse() {
        if (isEmpty() || size == 1) {
            return; // Nothing to reverse
        }
        
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
        std::cout << "List reversed" << std::endl;
    }
    
    // Detect if there's a cycle in the list using Floyd's cycle-finding algorithm
    bool hasCycle() const {
        if (isEmpty() || size == 1) {
            return false; // No cycle possible
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
};

// Example usage with advanced operations
int main() {
    std::cout << "===== SINGLY LINKED LIST DEMONSTRATION =====" << std::endl;
    
    // Create a new linked list
    SinglyLinkedList list;
    
    // Test isEmpty and size on empty list
    std::cout << "Is list empty? " << (list.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "List size: " << list.getSize() << std::endl;
    
    // Insert elements at different positions
    list.insertAtBeginning(10);
    list.display();
    
    list.insertAtEnd(30);
    list.display();
    
    list.insertAtPosition(20, 1);
    list.display();
    
    list.insertAtBeginning(5);
    list.display();
    
    list.insertAtEnd(40);
    list.display();
    
    // Find the middle element
    list.findMiddle();
    
    // Search for elements
    list.search(20);
    list.search(100);
    
    // Get and update values
    int value;
    if (list.getValueAt(2, value)) {
        std::cout << "Value at position 2: " << value << std::endl;
    }
    
    list.updateValueAt(1, 15);
    list.display();
    
    // Delete elements
    list.deleteFromBeginning();
    list.display();
    
    list.deleteFromEnd();
    list.display();
    
    list.deleteFromPosition(1);
    list.display();
    
    // Add more elements for testing reversal
    list.insertAtEnd(50);
    list.insertAtEnd(60);
    list.display();
    
    // Reverse the list
    list.reverse();
    list.display();
    
    // Check for cycles
    std::cout << "Does list have a cycle? " << (list.hasCycle() ? "Yes" : "No") << std::endl;
    
    // Clear the list
    list.clear();
    list.display();
    
    // Final message
    std::cout << "===== END OF DEMONSTRATION =====" << std::endl;
    
    return 0;
}