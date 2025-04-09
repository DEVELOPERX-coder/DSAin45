# Time and Space Complexity Visual Guide

## Day 10: Deques & Priority Queues

### Visual Design Specification

**Overall Theme**: Dark background (#121212) with neon accent colors

**Title**: Use gradient color from neon blue (#00FFFF) to neon purple (#FF00FF)

**Table Borders**: Thin lines in neon green (#00FF00)

**Headers**: Bold text in neon yellow (#FFFF00) with slightly darker background (#1A1A1A)

**Best Performance**: Highlight best performance in each row with neon pink (#FF00FF) text

**Worst Performance**: Indicate worst performance with muted orange (#FF8800)

**Background Elements**: Subtle grid pattern in dark teal (#004444) at 10% opacity

**Animation Effect**: Subtle glow effect around the tables when being viewed

---

## Deque Operations Time Complexity

|                   | Doubly Linked List |   Array-Based    |   STL std::deque   |
| ----------------- | :----------------: | :--------------: | :----------------: |
| **push_front**    |      **O(1)**      |       O(n)       | **O(1)** amortized |
| **push_back**     |      **O(1)**      | O(1) amortized\* | **O(1)** amortized |
| **pop_front**     |      **O(1)**      |       O(n)       |      **O(1)**      |
| **pop_back**      |      **O(1)**      |     **O(1)**     |      **O(1)**      |
| **front/back**    |      **O(1)**      |     **O(1)**     |      **O(1)**      |
| **Random Access** |        O(n)        |     **O(1)**     |      **O(1)**      |
| **Insert Middle** |       O(n)†        |       O(n)       |        O(n)        |
| **Delete Middle** |       O(n)†        |       O(n)       |        O(n)        |

_\* O(1) amortized for vector push_back due to occasional reallocation_  
_† O(1) time to update pointers once position is found, but O(n) to find position_

---

## Priority Queue Operations Time Complexity

|               | Binary Heap | Ordered Array | Unsorted Array | STL priority_queue |
| ------------- | :---------: | :-----------: | :------------: | :----------------: |
| **push**      |  O(log n)   |     O(n)      |    **O(1)**    |      O(log n)      |
| **pop**       |  O(log n)   |   **O(1)**    |      O(n)      |      O(log n)      |
| **top**       |  **O(1)**   |   **O(1)**    |      O(n)      |      **O(1)**      |
| **search**    |    O(n)     |   O(log n)    |      O(n)      |        O(n)        |
| **update**    |  O(log n)   |     O(n)      |      O(n)      |     O(log n)\*     |
| **buildHeap** |    O(n)     |  O(n·log n)   |    **O(n)**    |        O(n)        |

_\* Requires custom implementation for efficient updates in std::priority_queue_

---

## Space Complexity

|                        | Space Usage |               Notes               |
| ---------------------- | :---------: | :-------------------------------: |
| **Linked List Deque**  |    O(n)     |    Extra overhead for pointers    |
| **Array Deque**        |    O(n)     |     May have unused capacity      |
| **STL deque**          |    O(n)     |  Implemented as multiple chunks   |
| **Binary Heap PQ**     |    O(n)     |       Typically array-based       |
| **Ordered Array PQ**   |    O(n)     | Simple but inefficient operations |
| **STL priority_queue** |    O(n)     |  Typically built on std::vector   |

---

## Implementation Trade-offs

|                        | Pros                                                                                          | Cons                                                                                                            |
| ---------------------- | :-------------------------------------------------------------------------------------------- | :-------------------------------------------------------------------------------------------------------------- |
| **Linked List Deque**  | • Stable O(1) operations at both ends<br>• Dynamic size<br>• No wasted space                  | • No random access<br>• Extra memory for pointers<br>• Cache unfriendly                                         |
| **Array Deque**        | • Random access<br>• Cache friendly<br>• Potentially less memory overhead                     | • Expensive front operations if not circular<br>• May need resizing<br>• Complex circular buffer implementation |
| **STL deque**          | • Best of both worlds<br>• Efficient operations everywhere<br>• Chunk-based memory management | • Slightly more overhead than vector<br>• Implementation complexity                                             |
| **Binary Heap PQ**     | • O(log n) modifications<br>• O(1) access to max/min<br>• In-place implementation possible    | • No efficient random access<br>• Element updates are tricky without auxiliary structures                       |
| **Ordered Array PQ**   | • Simple implementation<br>• Good for small datasets<br>• Binary search possible              | • Linear time insertions<br>• Not suitable for frequent modifications                                           |
| **STL priority_queue** | • Well-tested implementation<br>• Easy to use<br>• Customizable comparison                    | • Limited interface (no element updates)<br>• No iteration capability                                           |

---

## Deque vs Priority Queue: When to Use

| Choose Deque When...                              | Choose Priority Queue When...                          |
| ------------------------------------------------- | ------------------------------------------------------ |
| • You need operations at both ends                | • Elements have different priorities                   |
| • You're implementing a sliding window            | • You always need the highest/lowest element           |
| • You want both stack and queue behavior          | • You're implementing graph algorithms like Dijkstra's |
| • You need efficient traversal in both directions | • You're doing resource scheduling                     |
| • You're creating undo/redo functionality         | • You're implementing a heap sort                      |
| • You need random access and end operations       | • You need to find the k largest/smallest elements     |
