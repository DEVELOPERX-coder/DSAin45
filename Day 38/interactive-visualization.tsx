import React, { useState, useEffect } from 'react';
import { ArrowRight, Play, Pause, RotateCcw, ChevronLeft, ChevronRight, Code } from 'lucide-react';

const GreedyVisualization = () => {
  // State for controlling the visualization
  const [algorithm, setAlgorithm] = useState('activitySelection');
  const [step, setStep] = useState(0);
  const [playing, setPlaying] = useState(false);
  const [speed, setSpeed] = useState(1000); // milliseconds between steps
  const [showCode, setShowCode] = useState(false);

  // Example data for different algorithms
  const [activityData] = useState({
    items: [
      { id: 0, start: 1, finish: 4, selected: false },
      { id: 1, start: 3, finish: 5, selected: false },
      { id: 2, start: 0, finish: 6, selected: false },
      { id: 3, start: 5, finish: 7, selected: false },
      { id: 4, start: 3, finish: 9, selected: false },
      { id: 5, start: 5, finish: 9, selected: false },
      { id: 6, start: 6, finish: 10, selected: false },
      { id: 7, start: 8, finish: 11, selected: false },
      { id: 8, start: 8, finish: 12, selected: false },
      { id: 9, start: 2, finish: 14, selected: false },
      { id: 10, start: 12, finish: 16, selected: false },
    ],
    maxTime: 16,
    steps: [
      { message: "We'll select activities that don't overlap, maximizing the total number. First, we sort by finish time.", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [] },
      { message: "Select the first activity (index 0, finish time: 4)", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0] },
      { message: "Activity 1 (finish: 5) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0] },
      { message: "Activity 2 (finish: 6) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0] },
      { message: "Activity 3 (finish: 7) doesn't conflict. Select it.", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3] },
      { message: "Activity 4 (finish: 9) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3] },
      { message: "Activity 5 (finish: 9) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3] },
      { message: "Activity 6 (finish: 10) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3] },
      { message: "Activity 7 (finish: 11) doesn't conflict. Select it.", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3, 7] },
      { message: "Activity 8 (finish: 12) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3, 7] },
      { message: "Activity 9 (finish: 14) conflicts with our selection, skip it", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3, 7] },
      { message: "Activity 10 (finish: 16) doesn't conflict. Select it.", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3, 7, 10] },
      { message: "Final result: 4 activities selected using the greedy approach", sortedIds: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], selectedIds: [0, 3, 7, 10] },
    ]
  });

  const [knapsackData] = useState({
    items: [
      { id: 0, value: 60, weight: 10, ratio: 6, fraction: 0 },
      { id: 1, value: 100, weight: 20, ratio: 5, fraction: 0 },
      { id: 2, value: 120, weight: 30, ratio: 4, fraction: 0 },
    ],
    capacity: 50,
    steps: [
      { message: "We have 3 items and a knapsack with capacity 50. First, we sort items by value-to-weight ratio.", sortedIds: [0, 1, 2], selectedIds: [], fractions: [0, 0, 0], remainingCapacity: 50 },
      { message: "Select item 0 (ratio: 6) completely. Remaining capacity: 40", sortedIds: [0, 1, 2], selectedIds: [0], fractions: [1, 0, 0], remainingCapacity: 40 },
      { message: "Select item 1 (ratio: 5) completely. Remaining capacity: 20", sortedIds: [0, 1, 2], selectedIds: [0, 1], fractions: [1, 1, 0], remainingCapacity: 20 },
      { message: "Can't take all of item 2 (weight: 30). Take 20/30 = 2/3 of it.", sortedIds: [0, 1, 2], selectedIds: [0, 1, 2], fractions: [1, 1, 0.67], remainingCapacity: 0 },
      { message: "Final result: Maximum value = 60 + 100 + (2/3)*120 = 240", sortedIds: [0, 1, 2], selectedIds: [0, 1, 2], fractions: [1, 1, 0.67], remainingCapacity: 0 },
    ]
  });

  const [huffmanData] = useState({
    text: "AAABBC",
    frequency: { 'A': 3, 'B': 2, 'C': 1 },
    nodes: [
      { id: 'A', freq: 3, code: '', left: null, right: null },
      { id: 'B', freq: 2, code: '', left: null, right: null },
      { id: 'C', freq: 1, code: '', left: null, right: null },
    ],
    steps: [
      { message: "We start with frequencies: A:3, B:2, C:1", heap: ['A', 'B', 'C'], tree: null, codes: {} },
      { message: "Extract two least frequent symbols: C (1) and B (2)", heap: ['A'], tree: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } }, codes: {} },
      { message: "New internal node BC with frequency 3 is added to the heap", heap: ['A', 'BC'], tree: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } }, codes: {} },
      { message: "Extract two least frequent nodes: A (3) and BC (3)", heap: [], tree: { id: 'ABC', freq: 6, left: { id: 'A', freq: 3 }, right: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } } }, codes: {} },
      { message: "Final Huffman Tree is created with root having frequency 6", heap: [], tree: { id: 'ABC', freq: 6, left: { id: 'A', freq: 3 }, right: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } } }, codes: {} },
      { message: "Assign codes: A = 0, B = 10, C = 11", heap: [], tree: { id: 'ABC', freq: 6, left: { id: 'A', freq: 3 }, right: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } } }, codes: { 'A': '0', 'B': '10', 'C': '11' } },
      { message: "Encoded string: 000101011 (original: 6 chars × 8 bits = 48 bits, compressed: 9 bits)", heap: [], tree: { id: 'ABC', freq: 6, left: { id: 'A', freq: 3 }, right: { id: 'BC', freq: 3, left: { id: 'C', freq: 1 }, right: { id: 'B', freq: 2 } } }, codes: { 'A': '0', 'B': '10', 'C': '11' } },
    ]
  });

  // Time and space complexity data
  const complexityData = {
    activitySelection: {
      name: "Activity Selection",
      time: "O(n log n)",
      space: "O(n)",
      timeExplanation: "Dominated by the sorting step where activities are sorted by finish time",
      spaceExplanation: "Storage for the activities and selected activities"
    },
    fractionalKnapsack: {
      name: "Fractional Knapsack",
      time: "O(n log n)",
      space: "O(n)",
      timeExplanation: "Dominated by the sorting step where items are sorted by value-to-weight ratio",
      spaceExplanation: "Storage for the items and selected items"
    },
    huffmanCoding: {
      name: "Huffman Coding",
      time: "O(n log n)",
      space: "O(n)",
      timeExplanation: "Building the Huffman tree using a priority queue with n leaf nodes",
      spaceExplanation: "Storage for the Huffman tree and character-to-code mapping"
    },
    coinChange: {
      name: "Coin Change (Greedy)",
      time: "O(n log n + amount)",
      space: "O(n)",
      timeExplanation: "Sorting denominations plus iterating through amount",
      spaceExplanation: "Storage for coin denominations and result"
    },
    jobScheduling: {
      name: "Job Scheduling",
      time: "O(n log n + n*m)",
      space: "O(m)",
      timeExplanation: "Sorting jobs by profit (n log n) and finding slots (n*m), where m is max deadline",
      spaceExplanation: "Storage for slots array of size m"
    },
    kruskalMST: {
      name: "Kruskal's MST",
      time: "O(E log E)",
      space: "O(E + V)",
      timeExplanation: "Sorting edges by weight (E log E) dominates the time complexity",
      spaceExplanation: "Storage for edges and disjoint-set data structure"
    }
  };

  // All greedy algorithms in our visualization
  const algorithms = [
    { id: 'activitySelection', name: 'Activity Selection' },
    { id: 'fractionalKnapsack', name: 'Fractional Knapsack' },
    { id: 'huffmanCoding', name: 'Huffman Coding' },
  ];

  // Code snippets for each algorithm
  const codeSnippets = {
    activitySelection: `vector<int> activitySelection(vector<int>& start, vector<int>& finish) {
    // Sort activities by finish time
    vector<pair<int, int>> activities;
    for (int i = 0; i < start.size(); i++) {
        activities.push_back({finish[i], i});
    }
    sort(activities.begin(), activities.end());
    
    vector<int> result;
    result.push_back(activities[0].second); // Select first activity
    int lastFinishTime = activities[0].first;
    
    for (int i = 1; i < activities.size(); i++) {
        // If this activity starts after the last selected activity finishes
        if (start[activities[i].second] >= lastFinishTime) {
            result.push_back(activities[i].second);
            lastFinishTime = activities[i].first;
        }
    }
    
    return result;
}`,
    fractionalKnapsack: `double fractionalKnapsack(vector<int>& values, vector<int>& weights, int capacity) {
    vector<pair<double, int>> valuePerWeight;
    for (int i = 0; i < values.size(); i++) {
        valuePerWeight.push_back({(double)values[i] / weights[i], i});
    }
    
    // Sort by value-to-weight ratio in descending order
    sort(valuePerWeight.rbegin(), valuePerWeight.rend());
    
    double totalValue = 0.0;
    int remainingCapacity = capacity;
    
    for (auto [ratio, idx] : valuePerWeight) {
        if (weights[idx] <= remainingCapacity) {
            // Take the whole item
            totalValue += values[idx];
            remainingCapacity -= weights[idx];
        } else {
            // Take a fraction of the item
            totalValue += values[idx] * ((double)remainingCapacity / weights[idx]);
            break;
        }
    }
    
    return totalValue;
}`,
    huffmanCoding: `unordered_map<char, string> buildHuffmanCodes(const string& text) {
    // Count frequency of characters
    unordered_map<char, unsigned> freq;
    for (char c : text) {
        freq[c]++;
    }
    
    // Create a priority queue (min-heap)
    priority_queue<Node*, vector<Node*>, Compare> pq;
    
    // Create a leaf node for each character and add to priority queue
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }
    
    // Build Huffman Tree
    while (pq.size() > 1) {
        // Extract two nodes with lowest frequency
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        
        // Create a new internal node with these two nodes as children
        Node* top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        
        // Add this node to the priority queue
        pq.push(top);
    }
    
    // The remaining node is the root of Huffman Tree
    Node* root = pq.top();
    
    // Generate Huffman codes
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);
    
    return huffmanCode;
}`
  };

  // Animation effect when playing
  useEffect(() => {
    let interval;
    
    if (playing) {
      interval = setInterval(() => {
        setStep(prevStep => {
          const maxStep = getMaxSteps() - 1;
          return prevStep < maxStep ? prevStep + 1 : 0; // loop back to start when reaching the end
        });
      }, speed);
    }
    
    return () => clearInterval(interval);
  }, [playing, speed]);

  // Get max steps for current algorithm
  const getMaxSteps = () => {
    switch(algorithm) {
      case 'activitySelection':
        return activityData.steps.length;
      case 'fractionalKnapsack':
        return knapsackData.steps.length;
      case 'huffmanCoding':
        return huffmanData.steps.length;
      default:
        return 0;
    }
  };

  // Get current data for visualization
  const getCurrentData = () => {
    switch(algorithm) {
      case 'activitySelection':
        return activityData;
      case 'fractionalKnapsack':
        return knapsackData;
      case 'huffmanCoding':
        return huffmanData;
      default:
        return null;
    }
  };

  // Render timeline for Activity Selection
  const renderActivityTimeline = () => {
    const data = activityData;
    const currentStep = data.steps[step];
    const timelineWidth = 100; // percentage
    
    return (
      <div className="mt-4">
        {/* Timeline */}
        <div className="relative h-64 w-full bg-gray-800 rounded-lg overflow-hidden">
          {/* Time markers */}
          <div className="absolute bottom-0 w-full flex justify-between px-2 text-gray-400 text-xs">
            {Array.from({ length: data.maxTime + 1 }, (_, i) => (
              <div key={i} className="relative">
                <div className="absolute bottom-2 w-px h-2 bg-gray-600"></div>
                {i}
              </div>
            ))}
          </div>
          
          {/* Activities */}
          {data.items.map((item, idx) => {
            const isSorted = currentStep.sortedIds.indexOf(item.id);
            const isSelected = currentStep.selectedIds.includes(item.id);
            const itemTop = 10 + isSorted * 20; // Position based on sorted order
            const startPos = (item.start / data.maxTime) * timelineWidth;
            const width = ((item.finish - item.start) / data.maxTime) * timelineWidth;
            
            return (
              <div 
                key={idx}
                className={`absolute h-6 rounded-md flex items-center justify-center text-xs font-bold transition-all duration-500 ${
                  isSelected ? 'bg-pink-500 text-white' : 'bg-cyan-500 text-gray-900'
                }`}
                style={{
                  left: `${startPos}%`,
                  top: `${itemTop}px`,
                  width: `${width}%`
                }}
              >
                {item.id}
              </div>
            );
          })}
        </div>
        
        {/* Legend */}
        <div className="mt-2 flex items-center justify-center space-x-6 text-sm">
          <div className="flex items-center">
            <div className="w-4 h-4 bg-cyan-500 rounded mr-2"></div>
            <span>Available Activity</span>
          </div>
          <div className="flex items-center">
            <div className="w-4 h-4 bg-pink-500 rounded mr-2"></div>
            <span>Selected Activity</span>
          </div>
        </div>
      </div>
    );
  };

  // Render Fractional Knapsack
  const renderKnapsack = () => {
    const data = knapsackData;
    const currentStep = data.steps[step];
    
    return (
      <div className="mt-4">
        <div className="grid grid-cols-2 gap-4">
          {/* Items */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Items</h3>
            <div className="space-y-3">
              {data.items.map((item, idx) => {
                const isSelected = currentStep.selectedIds.includes(item.id);
                const fraction = currentStep.fractions ? currentStep.fractions[item.id] : 0;
                
                return (
                  <div 
                    key={idx}
                    className={`p-2 rounded-md transition-all duration-500 ${
                      isSelected ? 'bg-gray-700' : 'bg-gray-800'
                    }`}
                  >
                    <div className="flex justify-between mb-1">
                      <span>Item {item.id}</span>
                      <span className="text-green-400">Value: {item.value}</span>
                      <span className="text-yellow-400">Weight: {item.weight}</span>
                      <span className="text-cyan-400">Ratio: {item.ratio}</span>
                    </div>
                    
                    {isSelected && (
                      <div className="w-full bg-gray-600 rounded-full h-2.5">
                        <div 
                          className="bg-pink-500 h-2.5 rounded-full" 
                          style={{ width: `${fraction * 100}%` }}
                        ></div>
                      </div>
                    )}
                  </div>
                );
              })}
            </div>
          </div>
          
          {/* Knapsack */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Knapsack</h3>
            <div className="space-y-2">
              <div className="flex justify-between">
                <span>Capacity:</span>
                <span>{data.capacity}</span>
              </div>
              <div className="flex justify-between">
                <span>Remaining:</span>
                <span>{currentStep.remainingCapacity}</span>
              </div>
              <div className="w-full bg-gray-600 rounded-full h-4 mb-4">
                <div 
                  className="bg-green-400 h-4 rounded-full transition-all duration-500" 
                  style={{ width: `${((data.capacity - currentStep.remainingCapacity) / data.capacity) * 100}%` }}
                ></div>
              </div>
              
              <h4 className="text-yellow-400 mt-4">Selected Items:</h4>
              <div className="space-y-2">
                {currentStep.selectedIds.map((id, idx) => {
                  const item = data.items.find(i => i.id === id);
                  const fraction = currentStep.fractions[id];
                  const value = item.value * fraction;
                  const weight = item.weight * fraction;
                  
                  return (
                    <div key={idx} className="flex justify-between">
                      <span>Item {id} ({fraction < 1 ? `${Math.round(fraction * 100)}%` : 'complete'})</span>
                      <span className="text-green-400">Value: {value.toFixed(1)}</span>
                      <span className="text-yellow-400">Weight: {weight.toFixed(1)}</span>
                    </div>
                  );
                })}
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  };

  // Render Huffman Coding
  const renderHuffmanCoding = () => {
    const data = huffmanData;
    const currentStep = data.steps[step];
    
    // Utility function to render tree recursively
    const renderTree = (node, x, y, level = 0, isLeft = true) => {
      if (!node) return null;
      
      const gap = 200 / Math.pow(2, level);
      
      return (
        <g key={`${node.id}-${level}-${x}`}>
          {/* Node circle */}
          <circle 
            cx={x} 
            cy={y} 
            r="20" 
            fill={node.id.length === 1 ? "#FF00FF" : "#00FFFF"} 
          />
          
          {/* Node text */}
          <text 
            x={x} 
            y={y} 
            textAnchor="middle" 
            dominantBaseline="middle" 
            fill="#121212" 
            fontWeight="bold"
          >
            {node.id}
          </text>
          
          {/* Frequency text */}
          <text 
            x={x} 
            y={y + 30} 
            textAnchor="middle" 
            fill="#FFFFFF" 
            fontSize="12"
          >
            {node.freq}
          </text>
          
          {/* Left child */}
          {node.left && (
            <>
              <line 
                x1={x} 
                y1={y + 20} 
                x2={x - gap} 
                y2={y + 60} 
                stroke="#FFFF00" 
                strokeWidth="2" 
              />
              <text 
                x={x - gap/2} 
                y={y + 40} 
                textAnchor="middle" 
                fill="#FFFF00" 
                fontSize="12"
              >
                0
              </text>
              {renderTree(node.left, x - gap, y + 80, level + 1, true)}
            </>
          )}
          
          {/* Right child */}
          {node.right && (
            <>
              <line 
                x1={x} 
                y1={y + 20} 
                x2={x + gap} 
                y2={y + 60} 
                stroke="#FFFF00" 
                strokeWidth="2" 
              />
              <text 
                x={x + gap/2} 
                y={y + 40} 
                textAnchor="middle" 
                fill="#FFFF00" 
                fontSize="12"
              >
                1
              </text>
              {renderTree(node.right, x + gap, y + 80, level + 1, false)}
            </>
          )}
        </g>
      );
    };
    
    return (
      <div className="mt-4">
        <div className="grid grid-cols-2 gap-4">
          {/* Priority Queue and Tree */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Min Heap</h3>
            <div className="flex flex-wrap gap-2 mb-4">
              {currentStep.heap.map((nodeId, idx) => {
                const node = data.nodes.find(n => n.id === nodeId) || 
                             { id: nodeId, freq: nodeId.split('').reduce((acc, char) => 
                               acc + (data.frequency[char] || 0), 0)};
                
                return (
                  <div 
                    key={idx}
                    className="px-3 py-2 bg-cyan-500 rounded-md text-gray-900 font-bold"
                  >
                    {node.id} ({node.freq})
                  </div>
                );
              })}
              {currentStep.heap.length === 0 && (
                <div className="text-gray-400 italic">Heap is empty</div>
              )}
            </div>
            
            <h3 className="text-yellow-400 text-lg mb-2">Huffman Tree</h3>
            <div className="w-full h-64 overflow-auto">
              {currentStep.tree ? (
                <svg width="400" height="300" viewBox="0 0 400 300">
                  {renderTree(currentStep.tree, 200, 40)}
                </svg>
              ) : (
                <div className="text-gray-400 italic">Tree not built yet</div>
              )}
            </div>
          </div>
          
          {/* Codes and Encoding */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Huffman Codes</h3>
            {Object.keys(currentStep.codes).length > 0 ? (
              <div className="grid grid-cols-2 gap-2 mb-4">
                {Object.entries(currentStep.codes).map(([char, code], idx) => (
                  <div key={idx} className="flex justify-between">
                    <span className="font-mono bg-gray-700 px-2 rounded">{char}</span>
                    <span className="font-mono">{code}</span>
                  </div>
                ))}
              </div>
            ) : (
              <div className="text-gray-400 italic mb-4">Codes not assigned yet</div>
            )}
            
            <h3 className="text-yellow-400 text-lg mb-2">Original Text</h3>
            <div className="font-mono bg-gray-700 p-2 rounded mb-4 overflow-x-auto">
              {data.text}
            </div>
            
            {Object.keys(currentStep.codes).length > 0 && (
              <>
                <h3 className="text-yellow-400 text-lg mb-2">Encoded Text</h3>
                <div className="font-mono bg-gray-700 p-2 rounded overflow-x-auto">
                  {data.text.split('').map((char, idx) => currentStep.codes[char]).join('')}
                </div>
              </>
            )}
          </div>
        </div>
      </div>
    );
  };

  // Render the current algorithm visualization
  const renderVisualization = () => {
    switch(algorithm) {
      case 'activitySelection':
        return renderActivityTimeline();
      case 'fractionalKnapsack':
        return renderKnapsack();
      case 'huffmanCoding':
        return renderHuffmanCoding();
      default:
        return <div>Select an algorithm to visualize</div>;
    }
  };

  // Render the complexity table
  const renderComplexityTable = () => {
    return (
      <div className="mt-6 bg-gray-800 rounded-lg p-4">
        <h3 className="text-yellow-400 text-lg mb-2">Time & Space Complexity Comparison</h3>
        <div className="overflow-x-auto">
          <table className="w-full text-sm">
            <thead>
              <tr className="border-b border-gray-700">
                <th className="px-4 py-2 text-left">Algorithm</th>
                <th className="px-4 py-2 text-left">Time Complexity</th>
                <th className="px-4 py-2 text-left">Space Complexity</th>
                <th className="px-4 py-2 text-left">Notes</th>
              </tr>
            </thead>
            <tbody>
              {Object.values(complexityData).map((algo, idx) => (
                <tr key={idx} className={`border-b border-gray-700 ${algo.id === algorithm ? 'bg-gray-700' : ''}`}>
                  <td className="px-4 py-2">{algo.name}</td>
                  <td className="px-4 py-2 font-mono">{algo.time}</td>
                  <td className="px-4 py-2 font-mono">{algo.space}</td>
                  <td className="px-4 py-2 text-xs">{algo.timeExplanation}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    );
  };

  return (
    <div className="bg-gray-900 text-white min-h-screen p-4">
      <h1 className="text-2xl font-bold text-center mb-4">Greedy Algorithms Visualization</h1>
      
      {/* Algorithm selection */}
      <div className="flex justify-center mb-4 space-x-4">
        {algorithms.map(algo => (
          <button
            key={algo.id}
            className={`px-4 py-2 rounded-md ${
              algorithm === algo.id ? 'bg-cyan-500 text-gray-900' : 'bg-gray-700 text-white'
            }`}
            onClick={() => {
              setAlgorithm(algo.id);
              setStep(0);
              setPlaying(false);
            }}
          >
            {algo.name}
          </button>
        ))}
      </div>
      
      {/* Current algorithm info */}
      <div className="bg-gray-800 rounded-lg p-4 mb-4">
        <div className="flex justify-between items-center">
          <h2 className="text-xl font-bold text-cyan-400">
            {complexityData[algorithm]?.name}
          </h2>
          <div className="flex items-center space-x-2">
            <button
              className="p-2 bg-gray-700 rounded-md hover:bg-gray-600"
              onClick={() => setShowCode(!showCode)}
            >
              <Code size={20} className="text-yellow-400" />
            </button>
          </div>
        </div>
        
        {/* Algorithm description */}
        <p className="mt-2">
          {algorithm === 'activitySelection' && "Select the maximum number of non-overlapping activities."}
          {algorithm === 'fractionalKnapsack' && "Maximize value in a knapsack by taking fractions of items."}
          {algorithm === 'huffmanCoding' && "Compress data by assigning variable-length codes to characters."}
        </p>
        
        {/* Complexity summary */}
        <div className="mt-2 flex flex-col sm:flex-row sm:justify-start sm:space-x-4">
          <div className="flex items-center space-x-2">
            <span className="text-gray-400">Time:</span>
            <span className="font-mono text-green-400">{complexityData[algorithm]?.time}</span>
          </div>
          <div className="flex items-center space-x-2">
            <span className="text-gray-400">Space:</span>
            <span className="font-mono text-green-400">{complexityData[algorithm]?.space}</span>
          </div>
        </div>
        
        {/* Code view */}
        {showCode && (
          <div className="mt-4 bg-gray-900 rounded p-4 overflow-x-auto">
            <pre className="text-xs font-mono text-green-400">
              {codeSnippets[algorithm]}
            </pre>
          </div>
        )}
      </div>
      
      {/* Step information */}
      <div className="bg-gray-800 rounded-lg p-4 mb-4">
        <div className="text-yellow-400">Step {step + 1} of {getMaxSteps()}</div>
        <div className="mt-2 text-lg">
          {getCurrentData()?.steps[step]?.message}
        </div>
      </div>
      
      {/* Visualization area */}
      <div className="bg-gray-800 rounded-lg p-4 mb-4 min-h-[300px]">
        {renderVisualization()}
      </div>
      
      {/* Complexity table */}
      {renderComplexityTable()}
      
      {/* Controls */}
      <div className="bg-gray-800 rounded-lg p-4 mt-4 flex items-center justify-between">
        <div className="flex items-center space-x-2">
          <button
            className="p-2 bg-gray-700 rounded-md hover:bg-gray-600"
            onClick={() => setStep(0)}
          >
            <RotateCcw size={20} className="text-yellow-400" />
          </button>
          <button
            className="p-2 bg-gray-700 rounded-md hover:bg-gray-600"
            onClick={() => setStep(prev => (prev > 0 ? prev - 1 : prev))}
          >
            <ChevronLeft size={20} className="text-yellow-400" />
          </button>
          <button
            className="p-2 bg-gray-700 rounded-md hover:bg-gray-600"
            onClick={() => setPlaying(!playing)}
          >
            {playing ? (
              <Pause size={20} className="text-pink-400" />
            ) : (
              <Play size={20} className="text-green-400" />
            )}
          </button>
          <button
            className="p-2 bg-gray-700 rounded-md hover:bg-gray-600"
            onClick={() => setStep(prev => (prev < getMaxSteps() - 1 ? prev + 1 : prev))}
          >
            <ChevronRight size={20} className="text-yellow-400" />
          </button>
        </div>
        
        <div className="flex items-center space-x-2">
          <span className="text-gray-400">Speed:</span>
          <input
            type="range"
            min="200"
            max="2000"
            step="100"
            value={2200 - speed}
            onChange={e => setSpeed(2200 - parseInt(e.target.value))}
            className="w-32"
          />
        </div>
      </div>
    </div>
  );
};

export default GreedyVisualization;