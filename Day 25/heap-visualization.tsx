import React, { useState, useEffect } from 'react';

// Heap visualization component
const HeapVisualization = () => {
  // States
  const [heap, setHeap] = useState([]);
  const [inputValue, setInputValue] = useState('');
  const [operationMessage, setOperationMessage] = useState('');
  const [heapType, setHeapType] = useState('max'); // 'max' or 'min'
  const [activeTab, setActiveTab] = useState('visualization');
  const [animationState, setAnimationState] = useState({
    isAnimating: false,
    animationType: null,
    highlightedNodes: [],
    swappingNodes: []
  });
  const [codeView, setCodeView] = useState('insert');
  
  // Colors using neon theme
  const colors = {
    bgColor: "#121212",
    cyanColor: "#00FFFF",
    pinkColor: "#FF00FF",
    yellowColor: "#FFFF00",
    greenColor: "#00FF00"
  };
  
  // Function to compare elements based on heap type
  const compareHeapNodes = (a, b) => {
    if (heapType === 'max') {
      return a >= b;
    } else {
      return a <= b;
    }
  };
  
  // Helper functions for heap operations
  const getParentIndex = (index) => Math.floor((index - 1) / 2);
  const getLeftChildIndex = (index) => 2 * index + 1;
  const getRightChildIndex = (index) => 2 * index + 2;
  
  // Insert a new value into the heap
  const insertValue = async () => {
    const value = parseInt(inputValue);
    if (isNaN(value)) {
      setOperationMessage('Please enter a valid number');
      return;
    }
    
    const newHeap = [...heap, value];
    setHeap(newHeap);
    
    // Animate heapify up
    setAnimationState({
      isAnimating: true,
      animationType: 'insert',
      highlightedNodes: [newHeap.length - 1],
      swappingNodes: []
    });
    
    await heapifyUpAnimated(newHeap, newHeap.length - 1);
    
    setInputValue('');
    setOperationMessage(`Value ${value} inserted successfully`);
    setAnimationState({
      isAnimating: false,
      animationType: null,
      highlightedNodes: [],
      swappingNodes: []
    });
  };
  
  // Extract the top element from the heap
  const extractTop = async () => {
    if (heap.length === 0) {
      setOperationMessage('Heap is empty');
      return;
    }
    
    const topValue = heap[0];
    
    // Highlight the root to be extracted
    setAnimationState({
      isAnimating: true,
      animationType: 'extract',
      highlightedNodes: [0],
      swappingNodes: []
    });
    
    // Wait for visualization
    await new Promise(resolve => setTimeout(resolve, 800));
    
    // If only one element, just remove it
    if (heap.length === 1) {
      setHeap([]);
      setOperationMessage(`Extracted ${heapType === 'max' ? 'maximum' : 'minimum'}: ${topValue}`);
      setAnimationState({
        isAnimating: false,
        animationType: null,
        highlightedNodes: [],
        swappingNodes: []
      });
      return;
    }
    
    // Replace root with last element
    const newHeap = [...heap];
    newHeap[0] = newHeap[newHeap.length - 1];
    newHeap.pop();
    setHeap(newHeap);
    
    // Show swap animation
    setAnimationState({
      isAnimating: true,
      animationType: 'extract',
      highlightedNodes: [0],
      swappingNodes: [0, heap.length - 1]
    });
    
    await new Promise(resolve => setTimeout(resolve, 800));
    
    // Heapify down
    await heapifyDownAnimated(newHeap, 0);
    
    setOperationMessage(`Extracted ${heapType === 'max' ? 'maximum' : 'minimum'}: ${topValue}`);
    setAnimationState({
      isAnimating: false,
      animationType: null,
      highlightedNodes: [],
      swappingNodes: []
    });
  };
  
  // Create a new heap with random values
  const createRandomHeap = async () => {
    const size = 7; // A reasonable size for visualization
    const randomValues = Array.from({ length: size }, () => Math.floor(Math.random() * 100) + 1);
    
    // Start with empty heap
    setHeap([]);
    setOperationMessage('Creating a new heap with random values...');
    
    // Insert values one by one with animation
    for (const value of randomValues) {
      await new Promise(resolve => setTimeout(resolve, 300));
      
      const newHeap = [...heap, value];
      setHeap(newHeap);
      
      setAnimationState({
        isAnimating: true,
        animationType: 'insert',
        highlightedNodes: [newHeap.length - 1],
        swappingNodes: []
      });
      
      await heapifyUpAnimated(newHeap, newHeap.length - 1);
    }
    
    setOperationMessage('Random heap created successfully');
    setAnimationState({
      isAnimating: false,
      animationType: null,
      highlightedNodes: [],
      swappingNodes: []
    });
  };
  
  // Toggle between max and min heap
  const toggleHeapType = async () => {
    const newType = heapType === 'max' ? 'min' : 'max';
    setHeapType(newType);
    
    // Rebuild the heap with the new comparison
    if (heap.length > 0) {
      const newHeap = [...heap];
      setOperationMessage(`Converting to ${newType} heap...`);
      
      // Rebuild the heap from bottom-up
      for (let i = Math.floor(newHeap.length / 2) - 1; i >= 0; i--) {
        await heapifyDownAnimated(newHeap, i);
      }
      
      setOperationMessage(`Converted to ${newType} heap successfully`);
    } else {
      setOperationMessage(`Switched to ${newType} heap`);
    }
  };
  
  // Heapify up with animation
  const heapifyUpAnimated = async (heapArray, index) => {
    let currentIndex = index;
    let parentIndex = getParentIndex(currentIndex);
    
    while (
      currentIndex > 0 && 
      !compareHeapNodes(heapArray[parentIndex], heapArray[currentIndex])
    ) {
      // Highlight the nodes to be swapped
      setAnimationState(prev => ({
        ...prev,
        highlightedNodes: [currentIndex, parentIndex],
        swappingNodes: [currentIndex, parentIndex]
      }));
      
      await new Promise(resolve => setTimeout(resolve, 800));
      
      // Swap values
      [heapArray[currentIndex], heapArray[parentIndex]] = 
      [heapArray[parentIndex], heapArray[currentIndex]];
      
      // Update heap state for visualization
      setHeap([...heapArray]);
      
      // Move up the tree
      currentIndex = parentIndex;
      parentIndex = getParentIndex(currentIndex);
      
      // Update highlighted nodes
      setAnimationState(prev => ({
        ...prev,
        highlightedNodes: [currentIndex],
        swappingNodes: []
      }));
      
      await new Promise(resolve => setTimeout(resolve, 500));
    }
    
    return heapArray;
  };
  
  // Heapify down with animation
  const heapifyDownAnimated = async (heapArray, index) => {
    const size = heapArray.length;
    let currentIndex = index;
    let largest = currentIndex;
    
    while (true) {
      const leftChildIdx = getLeftChildIndex(currentIndex);
      const rightChildIdx = getRightChildIndex(currentIndex);
      
      // Find the largest/smallest among current, left, and right
      if (
        leftChildIdx < size && 
        !compareHeapNodes(heapArray[largest], heapArray[leftChildIdx])
      ) {
        largest = leftChildIdx;
      }
      
      if (
        rightChildIdx < size && 
        !compareHeapNodes(heapArray[largest], heapArray[rightChildIdx])
      ) {
        largest = rightChildIdx;
      }
      
      // If largest/smallest is not current, swap
      if (largest !== currentIndex) {
        // Highlight the nodes to be swapped
        setAnimationState(prev => ({
          ...prev,
          highlightedNodes: [currentIndex, largest],
          swappingNodes: [currentIndex, largest]
        }));
        
        await new Promise(resolve => setTimeout(resolve, 800));
        
        // Swap values
        [heapArray[currentIndex], heapArray[largest]] = 
        [heapArray[largest], heapArray[currentIndex]];
        
        // Update heap state for visualization
        setHeap([...heapArray]);
        
        // Continue heapifying down
        currentIndex = largest;
        
        // Update highlighted nodes
        setAnimationState(prev => ({
          ...prev,
          highlightedNodes: [currentIndex],
          swappingNodes: []
        }));
        
        await new Promise(resolve => setTimeout(resolve, 500));
      } else {
        // No more swaps needed
        break;
      }
    }
    
    return heapArray;
  };
  
  // Demo Heap Sort
  const demonstrateHeapSort = async () => {
    if (heap.length === 0) {
      setOperationMessage('Heap is empty. Please create a heap first.');
      return;
    }
    
    setOperationMessage('Demonstrating Heap Sort...');
    
    // Clone the current heap
    const heapArray = [...heap];
    const size = heapArray.length;
    
    // Step 1: Ensure the array is a valid heap (should already be the case)
    for (let i = Math.floor(size / 2) - 1; i >= 0; i--) {
      await heapifyDownAnimated(heapArray, i);
    }
    
    // Step 2: Extract elements one by one
    for (let i = size - 1; i > 0; i--) {
      // Highlight the root and last unsorted element
      setAnimationState({
        isAnimating: true,
        animationType: 'heapsort',
        highlightedNodes: [0, i],
        swappingNodes: [0, i]
      });
      
      await new Promise(resolve => setTimeout(resolve, 800));
      
      // Swap root with last unsorted element
      [heapArray[0], heapArray[i]] = [heapArray[i], heapArray[0]];
      setHeap([...heapArray]);
      
      // Show sorted part in a different color
      setAnimationState({
        isAnimating: true,
        animationType: 'heapsort',
        highlightedNodes: [0],
        swappingNodes: [],
        sortedIndices: Array.from({ length: size - i }, (_, idx) => i + idx)
      });
      
      await new Promise(resolve => setTimeout(resolve, 500));
      
      // Heapify the reduced heap
      await heapifyDownAnimated(heapArray.slice(0, i), 0).then(newHeap => {
        // Combine the heapified part with the sorted part
        setHeap([...newHeap, ...heapArray.slice(i)]);
      });
    }
    
    // Mark the entire array as sorted
    setAnimationState({
      isAnimating: true,
      animationType: 'heapsort',
      highlightedNodes: [],
      swappingNodes: [],
      sortedIndices: Array.from({ length: size }, (_, idx) => idx)
    });
    
    setOperationMessage('Heap Sort completed!');
    
    // Reset animation state after a delay
    setTimeout(() => {
      setAnimationState({
        isAnimating: false,
        animationType: null,
        highlightedNodes: [],
        swappingNodes: [],
        sortedIndices: []
      });
    }, 3000);
  };
  
  // Reset demo
  const resetDemo = () => {
    setHeap([]);
    setInputValue('');
    setOperationMessage('Heap reset. Add values to begin.');
    setAnimationState({
      isAnimating: false,
      animationType: null,
      highlightedNodes: [],
      swappingNodes: []
    });
  };
  
  // Render a node in the heap visualization
  const renderHeapNode = (value, index, x, y, radius) => {
    // Determine node color based on its state
    let fillColor = colors.cyanColor;
    
    if (animationState.swappingNodes && animationState.swappingNodes.includes(index)) {
      fillColor = colors.pinkColor;
    } else if (animationState.highlightedNodes && animationState.highlightedNodes.includes(index)) {
      fillColor = colors.yellowColor;
    } else if (animationState.sortedIndices && animationState.sortedIndices.includes(index)) {
      fillColor = colors.greenColor;
    }
    
    return (
      <g key={index}>
        <circle
          cx={x}
          cy={y}
          r={radius}
          fill={fillColor}
          stroke="#333"
          strokeWidth="2"
        />
        <text
          x={x}
          y={y}
          textAnchor="middle"
          dominantBaseline="middle"
          fill="#000"
          fontSize="18"
          fontWeight="bold"
        >
          {value}
        </text>
        <text
          x={x}
          y={y - radius - 10}
          textAnchor="middle"
          fill="#999"
          fontSize="12"
        >
          {index}
        </text>
      </g>
    );
  };
  
  // Render an edge between nodes
  const renderEdge = (parentIndex, childIndex, nodes) => {
    if (childIndex >= heap.length) return null;
    
    const parent = nodes[parentIndex];
    const child = nodes[childIndex];
    
    return (
      <line
        key={`${parentIndex}-${childIndex}`}
        x1={parent.x}
        y1={parent.y}
        x2={child.x}
        y2={child.y}
        stroke={colors.greenColor}
        strokeWidth="2"
      />
    );
  };
  
  // Calculate node positions for visualization
  const calculateNodePositions = () => {
    const nodes = [];
    const levelHeight = 80;
    const nodeRadius = 25;
    
    // Calculate positions level by level
    heap.forEach((value, index) => {
      const level = Math.floor(Math.log2(index + 1));
      const position = index - (Math.pow(2, level) - 1);
      const levelWidth = Math.pow(2, level);
      const width = 600; // SVG width
      const spacing = width / (levelWidth + 1);
      
      nodes.push({
        value,
        x: spacing * (position + 1),
        y: 60 + level * levelHeight,
        radius: nodeRadius
      });
    });
    
    return nodes;
  };
  
  // Render the heap tree visualization
  const renderHeapVisualization = () => {
    const nodes = calculateNodePositions();
    const edges = [];
    
    // Calculate edges
    for (let i = 0; i < heap.length; i++) {
      const leftChildIndex = getLeftChildIndex(i);
      const rightChildIndex = getRightChildIndex(i);
      
      if (leftChildIndex < heap.length) {
        edges.push(renderEdge(i, leftChildIndex, nodes));
      }
      
      if (rightChildIndex < heap.length) {
        edges.push(renderEdge(i, rightChildIndex, nodes));
      }
    }
    
    // Calculate height based on number of levels
    let svgHeight = 300;
    if (heap.length > 0) {
      const levels = Math.floor(Math.log2(heap.length)) + 1;
      svgHeight = 120 + levels * 80;
    }
    
    return (
      <svg width="600" height={svgHeight} style={{ backgroundColor: colors.bgColor }}>
        <text
          x="300"
          y="30"
          textAnchor="middle"
          fill={colors.pinkColor}
          fontSize="20"
          fontWeight="bold"
        >
          {heapType === 'max' ? 'Max Heap' : 'Min Heap'}
        </text>
        
        {/* Render edges first so they appear behind nodes */}
        {edges}
        
        {/* Render nodes */}
        {nodes.map((node, index) => 
          renderHeapNode(node.value, index, node.x, node.y, node.radius)
        )}
        
        {/* Show array representation */}
        <text
          x="300"
          y={svgHeight - 40}
          textAnchor="middle"
          fill="#fff"
          fontSize="14"
        >
          Array Representation:
        </text>
        <text
          x="300"
          y={svgHeight - 15}
          textAnchor="middle"
          fill={colors.cyanColor}
          fontSize="16"
        >
          [{heap.join(', ')}]
        </text>
      </svg>
    );
  };
  
  // Render the code view tab
  const renderCodeView = () => {
    let codeSnippet = '';
    
    if (codeView === 'insert') {
      codeSnippet = `// Insert into ${heapType} heap
void insert(int value) {
    // Add element to the end
    heap.push_back(value);
    int index = heap.size() - 1;
    
    // Heapify up
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        
        if (${heapType === 'max' ? 'heap[parentIndex] < heap[index]' : 'heap[parentIndex] > heap[index]'}) {
            // Swap with parent
            swap(heap[parentIndex], heap[index]);
            index = parentIndex;
        } else {
            break;
        }
    }
}`;
    } else if (codeView === 'extract') {
      codeSnippet = `// Extract ${heapType === 'max' ? 'maximum' : 'minimum'} from heap
int extract${heapType === 'max' ? 'Max' : 'Min'}() {
    if (heap.empty()) {
        throw runtime_error("Heap is empty");
    }
    
    // Get the root
    int root = heap[0];
    
    // Replace root with last element
    heap[0] = heap.back();
    heap.pop_back();
    
    // Heapify down
    int index = 0;
    while (true) {
        int largest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        
        if (leftChild < heap.size() && 
            ${heapType === 'max' ? 'heap[leftChild] > heap[largest]' : 'heap[leftChild] < heap[largest]'}) {
            largest = leftChild;
        }
        
        if (rightChild < heap.size() && 
            ${heapType === 'max' ? 'heap[rightChild] > heap[largest]' : 'heap[rightChild] < heap[largest]'}) {
            largest = rightChild;
        }
        
        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
    
    return root;
}`;
    } else if (codeView === 'heapsort') {
      codeSnippet = `// Heap Sort algorithm
void heapSort(vector<int>& arr) {
    int n = arr.size();
    
    // Build ${heapType} heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to the end
        swap(arr[0], arr[i]);
        
        // Call heapify on reduced heap
        heapify(arr, i, 0);
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int ${heapType === 'max' ? 'largest' : 'smallest'} = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // Compare with left child
    if (left < n && ${heapType === 'max' 
      ? 'arr[left] > arr[largest]' 
      : 'arr[left] < arr[smallest]'})
        ${heapType === 'max' ? 'largest' : 'smallest'} = left;
    
    // Compare with right child
    if (right < n && ${heapType === 'max' 
      ? 'arr[right] > arr[largest]' 
      : 'arr[right] < arr[smallest]'})
        ${heapType === 'max' ? 'largest' : 'smallest'} = right;
    
    // Swap if needed
    if (${heapType === 'max' ? 'largest' : 'smallest'} != i) {
        swap(arr[i], arr[${heapType === 'max' ? 'largest' : 'smallest'}]);
        heapify(arr, n, ${heapType === 'max' ? 'largest' : 'smallest'});
    }
}`;
    }
    
    return (
      <div style={{ 
        backgroundColor: '#1a1a1a', 
        padding: '15px',
        borderRadius: '5px',
        overflowX: 'auto'
      }}>
        <div style={{ display: 'flex', marginBottom: '10px' }}>
          <button
            onClick={() => setCodeView('insert')}
            style={{
              backgroundColor: codeView === 'insert' ? '#333' : 'transparent',
              color: codeView === 'insert' ? colors.cyanColor : '#999',
              border: 'none',
              padding: '8px 12px',
              marginRight: '8px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Insert
          </button>
          <button
            onClick={() => setCodeView('extract')}
            style={{
              backgroundColor: codeView === 'extract' ? '#333' : 'transparent',
              color: codeView === 'extract' ? colors.cyanColor : '#999',
              border: 'none',
              padding: '8px 12px',
              marginRight: '8px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Extract {heapType === 'max' ? 'Max' : 'Min'}
          </button>
          <button
            onClick={() => setCodeView('heapsort')}
            style={{
              backgroundColor: codeView === 'heapsort' ? '#333' : 'transparent',
              color: codeView === 'heapsort' ? colors.cyanColor : '#999',
              border: 'none',
              padding: '8px 12px',
              marginRight: '8px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Heap Sort
          </button>
        </div>
        <pre style={{ 
          color: '#fff', 
          fontSize: '14px',
          padding: '10px',
          background: '#242424',
          borderRadius: '5px',
          overflowX: 'auto'
        }}>
          <code>{codeSnippet}</code>
        </pre>
      </div>
    );
  };
  
  // Render the complexity table
  const renderComplexityView = () => {
    return (
      <div style={{ 
        padding: '20px', 
        color: '#fff',
        backgroundColor: '#1a1a1a',
        borderRadius: '5px'
      }}>
        <h3 style={{ 
          color: colors.pinkColor, 
          textAlign: 'center',
          marginBottom: '20px'
        }}>
          Heap Operations Time Complexity
        </h3>
        
        <table style={{ 
          width: '100%', 
          borderCollapse: 'collapse',
          marginBottom: '30px'
        }}>
          <thead>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <th style={{ padding: '10px', textAlign: 'left', color: colors.cyanColor }}>Operation</th>
              <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Time Complexity</th>
              <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Space Complexity</th>
            </tr>
          </thead>
          <tbody>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Insert</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
            </tr>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Extract Max/Min</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
            </tr>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Get Max/Min</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>O(1)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
            </tr>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Build Heap</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n)</td>
            </tr>
            <tr>
              <td style={{ padding: '10px' }}>Heap Sort</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
            </tr>
          </tbody>
        </table>
        
        <h3 style={{ 
          color: colors.yellowColor, 
          marginBottom: '15px'
        }}>
          Heap vs Other Data Structures
        </h3>
        
        <table style={{ 
          width: '100%', 
          borderCollapse: 'collapse'
        }}>
          <thead>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <th style={{ padding: '10px', textAlign: 'left', color: colors.cyanColor }}>Data Structure</th>
              <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Insert</th>
              <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Delete Min/Max</th>
              <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Search</th>
            </tr>
          </thead>
          <tbody>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Binary Heap</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n)</td>
            </tr>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Binary Search Tree</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)*</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)*</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)*</td>
            </tr>
            <tr style={{ borderBottom: '1px solid #333' }}>
              <td style={{ padding: '10px' }}>Sorted Array</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>O(1)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
            </tr>
            <tr>
              <td style={{ padding: '10px' }}>Unsorted Array</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>O(1)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n)</td>
              <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n)</td>
            </tr>
          </tbody>
        </table>
        
        <p style={{ fontSize: '14px', color: '#999', marginTop: '15px' }}>
          * For balanced BSTs. Worst case can be O(n) if unbalanced.
        </p>
      </div>
    );
  };
  
  // Render the applications view
  const renderApplicationsView = () => {
    return (
      <div style={{ 
        padding: '20px', 
        color: '#fff',
        backgroundColor: '#1a1a1a',
        borderRadius: '5px'
      }}>
        <h3 style={{ 
          color: colors.pinkColor, 
          textAlign: 'center',
          marginBottom: '20px'
        }}>
          Common Applications of Heaps
        </h3>
        
        <div style={{ 
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(250px, 1fr))',
          gap: '20px',
          marginBottom: '20px'
        }}>
          <div style={{ 
            border: '1px solid #333',
            borderRadius: '8px',
            padding: '15px'
          }}>
            <h4 style={{ color: colors.cyanColor, marginBottom: '10px' }}>Priority Queues</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Task scheduling in OS</li>
              <li style={{ marginBottom: '8px' }}>• Process management</li>
              <li style={{ marginBottom: '8px' }}>• Print job scheduling</li>
              <li>• Event-driven simulations</li>
            </ul>
          </div>
          
          <div style={{ 
            border: '1px solid #333',
            borderRadius: '8px',
            padding: '15px'
          }}>
            <h4 style={{ color: colors.yellowColor, marginBottom: '10px' }}>Graph Algorithms</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Dijkstra's algorithm</li>
              <li style={{ marginBottom: '8px' }}>• Prim's algorithm</li>
              <li style={{ marginBottom: '8px' }}>• A* search algorithm</li>
              <li>• Huffman coding</li>
            </ul>
          </div>
          
          <div style={{ 
            border: '1px solid #333',
            borderRadius: '8px',
            padding: '15px'
          }}>
            <h4 style={{ color: colors.greenColor, marginBottom: '10px' }}>Selection Problems</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Finding k-th smallest/largest</li>
              <li style={{ marginBottom: '8px' }}>• K nearest neighbors</li>
              <li style={{ marginBottom: '8px' }}>• Top-K frequent elements</li>
              <li>• Median of data stream</li>
            </ul>
          </div>
          
          <div style={{ 
            border: '1px solid #333',
            borderRadius: '8px',
            padding: '15px'
          }}>
            <h4 style={{ color: colors.pinkColor, marginBottom: '10px' }}>Sorting</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Heap Sort algorithm</li>
              <li style={{ marginBottom: '8px' }}>• Partial sorting</li>
              <li style={{ marginBottom: '8px' }}>• External sorting</li>
              <li>• Merge k sorted lists</li>
            </ul>
          </div>
        </div>
        
        <h3 style={{ 
          color: colors.cyanColor, 
          marginBottom: '15px',
          marginTop: '30px'
        }}>
          Example: Median Finding with Two Heaps
        </h3>
        
        <div style={{ 
          backgroundColor: '#242424',
          padding: '15px',
          borderRadius: '5px',
          marginBottom: '20px'
        }}>
          <pre style={{ color: '#fff', overflow: 'auto' }}>
            <code>{`class MedianFinder {
    // Max heap for the lower half
    priority_queue<int> maxHeap;
    
    // Min heap for the upper half
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:
    void addNum(int num) {
        // Add to appropriate heap
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }
        
        // Balance heaps
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        } else {
            return maxHeap.top();
        }
    }
};`}</code>
          </pre>
        </div>
        
        <p style={{ 
          textAlign: 'center',
          color: '#999',
          fontSize: '14px'
        }}>
          This two-heap approach efficiently finds the median from a stream of numbers in O(log n) time per insertion.
        </p>
      </div>
    );
  };
  
  // Render the visualization tab content
  const renderVisualizationTab = () => {
    return (
      <div>
        {/* Heap Visualization */}
        <div>
          {renderHeapVisualization()}
        </div>
        
        {/* Controls */}
        <div style={{ 
          padding: '15px', 
          display: 'flex',
          flexDirection: 'column',
          gap: '15px'
        }}>
          {/* Input and main operations */}
          <div style={{ 
            display: 'flex', 
            justifyContent: 'space-between',
            flexWrap: 'wrap',
            gap: '10px'
          }}>
            <div style={{ 
              display: 'flex', 
              gap: '10px',
              flexWrap: 'wrap'
            }}>
              <input
                type="text"
                value={inputValue}
                onChange={(e) => setInputValue(e.target.value)}
                placeholder="Enter a number"
                style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: '#fff',
                  border: '1px solid #444',
                  borderRadius: '4px',
                  width: '120px'
                }}
                disabled={animationState.isAnimating}
              />
              
              <button
                onClick={insertValue}
                style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: colors.greenColor,
                  border: '1px solid #444',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  opacity: animationState.isAnimating ? 0.5 : 1
                }}
                disabled={animationState.isAnimating}
              >
                Insert
              </button>
              
              <button
                onClick={extractTop}
                style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: colors.pinkColor,
                  border: '1px solid #444',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  opacity: animationState.isAnimating ? 0.5 : 1
                }}
                disabled={animationState.isAnimating}
              >
                Extract {heapType === 'max' ? 'Max' : 'Min'}
              </button>
            </div>
            
            <div style={{ 
              display: 'flex', 
              gap: '10px'
            }}>
              <button
                onClick={toggleHeapType}
                style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: colors.cyanColor,
                  border: '1px solid #444',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  opacity: animationState.isAnimating ? 0.5 : 1
                }}
                disabled={animationState.isAnimating}
              >
                Switch to {heapType === 'max' ? 'Min' : 'Max'} Heap
              </button>
              
              <button
                onClick={resetDemo}
                style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: colors.yellowColor,
                  border: '1px solid #444',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  opacity: animationState.isAnimating ? 0.5 : 1
                }}
                disabled={animationState.isAnimating}
              >
                Reset
              </button>
            </div>
          </div>
          
          {/* Operation message */}
          <div style={{
            padding: '8px 12px',
            backgroundColor: '#2a2a2a',
            color: '#fff',
            borderRadius: '4px',
            minHeight: '20px'
          }}>
            {operationMessage}
          </div>
          
          {/* Demo operations */}
          <div style={{ 
            display: 'flex', 
            justifyContent: 'space-between',
            flexWrap: 'wrap',
            gap: '10px'
          }}>
            <button
              onClick={createRandomHeap}
              style={{
                padding: '8px 12px',
                backgroundColor: '#2a2a2a',
                color: colors.greenColor,
                border: '1px solid #444',
                borderRadius: '4px',
                cursor: 'pointer',
                opacity: animationState.isAnimating ? 0.5 : 1
              }}
              disabled={animationState.isAnimating}
            >
              Create Random Heap
            </button>
            
            <button
              onClick={demonstrateHeapSort}
              style={{
                padding: '8px 12px',
                backgroundColor: '#2a2a2a',
                color: colors.cyanColor,
                border: '1px solid #444',
                borderRadius: '4px',
                cursor: 'pointer',
                opacity: animationState.isAnimating ? 0.5 : 1
              }}
              disabled={animationState.isAnimating}
            >
              Demonstrate Heap Sort
            </button>
          </div>
        </div>
      </div>
    );
  };
  
  return (
    <div style={{ 
      backgroundColor: colors.bgColor, 
      width: '100%', 
      minHeight: '700px',
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      padding: '20px',
      color: '#fff'
    }}>
      {/* Title */}
      <h2 style={{ 
        color: colors.pinkColor, 
        marginBottom: '20px',
        textAlign: 'center'
      }}>
        Heap Data Structure Visualization
      </h2>
      
      {/* Main container */}
      <div style={{ 
        width: '100%', 
        maxWidth: '800px',
        backgroundColor: '#1a1a1a',
        borderRadius: '8px',
        overflow: 'hidden',
        boxShadow: '0 4px 6px rgba(0, 0, 0, 0.3)'
      }}>
        {/* Tabs */}
        <div style={{ 
          display: 'flex', 
          borderBottom: '1px solid #333'
        }}>
          <button 
            onClick={() => setActiveTab('visualization')} 
            style={{ 
              padding: '10px 15px',
              backgroundColor: activeTab === 'visualization' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: activeTab === 'visualization' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: activeTab === 'visualization' ? 'bold' : 'normal'
            }}
          >
            Visualization
          </button>
          <button 
            onClick={() => setActiveTab('code')}
            style={{ 
              padding: '10px 15px',
              backgroundColor: activeTab === 'code' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: activeTab === 'code' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: activeTab === 'code' ? 'bold' : 'normal'
            }}
          >
            Code
          </button>
          <button 
            onClick={() => setActiveTab('complexity')}
            style={{ 
              padding: '10px 15px',
              backgroundColor: activeTab === 'complexity' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: activeTab === 'complexity' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: activeTab === 'complexity' ? 'bold' : 'normal'
            }}
          >
            Complexity
          </button>
          <button 
            onClick={() => setActiveTab('applications')}
            style={{ 
              padding: '10px 15px',
              backgroundColor: activeTab === 'applications' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: activeTab === 'applications' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: activeTab === 'applications' ? 'bold' : 'normal'
            }}
          >
            Applications
          </button>
        </div>
        
        {/* Tab content */}
        <div>
          {activeTab === 'visualization' && renderVisualizationTab()}
          {activeTab === 'code' && renderCodeView()}
          {activeTab === 'complexity' && renderComplexityView()}
          {activeTab === 'applications' && renderApplicationsView()}
        </div>
      </div>
      
      {/* Footer */}
      <div style={{ 
        marginTop: '15px',
        textAlign: 'center',
        color: '#999',
        fontSize: '14px'
      }}>
        <div style={{ color: colors.yellowColor, marginBottom: '5px' }}>
          Heap Data Structure - Day 25 of #DSAin45
        </div>
        <div>
          A specialized tree-based data structure for efficient priority management
        </div>
      </div>
    </div>
  );
};

export default HeapVisualization;