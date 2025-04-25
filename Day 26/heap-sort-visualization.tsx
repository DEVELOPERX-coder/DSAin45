import React, { useState, useEffect } from 'react';
import { ArrowDownUp, Activity, Award, Clock, ChevronsUp } from 'lucide-react';

const HeapSortVisualization = () => {
  // States for the application
  const [array, setArray] = useState([]);
  const [sortingSteps, setSortingSteps] = useState([]);
  const [currentStep, setCurrentStep] = useState(0);
  const [isPlaying, setIsPlaying] = useState(false);
  const [playbackSpeed, setPlaybackSpeed] = useState(500);
  const [activeTab, setActiveTab] = useState('heapsort');
  const [topKValue, setTopKValue] = useState(3);
  const [medianArray, setMedianArray] = useState([]);
  const [medianValue, setMedianValue] = useState(null);
  const [newNumber, setNewNumber] = useState('');
  const [isMedianHeapified, setIsMedianHeapified] = useState(false);
  const [maxHeap, setMaxHeap] = useState([]);
  const [minHeap, setMinHeap] = useState([]);
  
  // Colors using neon theme
  const colors = {
    bgColor: "#121212",
    cyanColor: "#00FFFF",
    pinkColor: "#FF00FF",
    yellowColor: "#FFFF00",
    greenColor: "#00FF00",
    grayColor: "#666666"
  };
  
  // Generate a new random array
  const generateArray = () => {
    const size = 8; // Reasonable size for visualization
    const newArray = Array.from({ length: size }, () => 
      Math.floor(Math.random() * 90) + 10
    );
    setArray(newArray);
    setSortingSteps([]);
    setCurrentStep(0);
    setIsPlaying(false);
  };
  
  // Heapify a subtree rooted at index i
  const heapify = (arr, n, i, steps) => {
    let largest = i;
    const left = 2 * i + 1;
    const right = 2 * i + 2;
    
    // Record the current state before any changes
    steps.push({
      array: [...arr],
      heapifyIndex: i,
      compareIndices: [left, right].filter(idx => idx < n),
      message: `Heapifying at index ${i}`
    });
    
    // Compare with left child
    if (left < n && arr[left] > arr[largest]) {
      largest = left;
      steps.push({
        array: [...arr],
        heapifyIndex: i,
        compareIndices: [left],
        message: `Left child ${arr[left]} is larger than ${arr[i]}`
      });
    }
    
    // Compare with right child
    if (right < n && arr[right] > arr[largest]) {
      largest = right;
      steps.push({
        array: [...arr],
        heapifyIndex: i,
        compareIndices: [right],
        message: `Right child ${arr[right]} is larger than ${arr[largest === left ? left : i]}`
      });
    }
    
    // If largest is not the root
    if (largest !== i) {
      // Swap elements
      [arr[i], arr[largest]] = [arr[largest], arr[i]];
      
      // Record the swap
      steps.push({
        array: [...arr],
        heapifyIndex: i,
        swapIndices: [i, largest],
        message: `Swapped ${arr[i]} and ${arr[largest]}`
      });
      
      // Recursively heapify the affected subtree
      heapify(arr, n, largest, steps);
    } else {
      steps.push({
        array: [...arr],
        heapifyIndex: i,
        message: `No need to swap. Heap property satisfied at index ${i}`
      });
    }
  };
  
  // Perform heap sort with visualization steps
  const heapSort = () => {
    if (array.length === 0) return;
    
    const steps = [];
    const arr = [...array];
    const n = arr.length;
    
    // Build max heap (rearrange array)
    steps.push({
      array: [...arr],
      message: "Building max heap from the array"
    });
    
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
      heapify(arr, n, i, steps);
    }
    
    steps.push({
      array: [...arr],
      message: "Max heap built successfully",
      heapBuilt: true
    });
    
    // Extract elements from heap one by one
    for (let i = n - 1; i > 0; i--) {
      // Move current root to end
      steps.push({
        array: [...arr],
        message: `Moving root element ${arr[0]} to final position ${i}`,
        extractIndex: i
      });
      
      [arr[0], arr[i]] = [arr[i], arr[0]];
      
      steps.push({
        array: [...arr],
        swapIndices: [0, i],
        message: `Swapped root ${arr[i]} with last unsorted element ${arr[0]}`,
        sortedIndices: Array.from({ length: n - i }, (_, idx) => i + idx)
      });
      
      // Call heapify on the reduced heap
      heapify(arr, i, 0, steps);
      
      steps.push({
        array: [...arr],
        message: `Heap restored. Elements in positions ${i} to ${n-1} are in their final sorted positions`,
        sortedIndices: Array.from({ length: n - i }, (_, idx) => i + idx)
      });
    }
    
    // Final state - array is sorted
    steps.push({
      array: [...arr],
      message: "Array is now sorted!",
      sortedIndices: Array.from({ length: n }, (_, i) => i)
    });
    
    setSortingSteps(steps);
    setCurrentStep(0);
  };
  
  // Play the animation
  useEffect(() => {
    let timer;
    if (isPlaying && sortingSteps.length > 0 && currentStep < sortingSteps.length - 1) {
      timer = setTimeout(() => {
        setCurrentStep(prevStep => prevStep + 1);
      }, playbackSpeed);
    } else if (currentStep >= sortingSteps.length - 1) {
      setIsPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [isPlaying, currentStep, sortingSteps, playbackSpeed]);
  
  // Top-K Elements functionality
  const findTopK = () => {
    if (array.length === 0) return;
    
    const k = Math.min(topKValue, array.length);
    const minHeap = [];
    
    // Helper function to sift up
    const siftUp = (heap, idx) => {
      const parent = Math.floor((idx - 1) / 2);
      if (parent >= 0 && heap[parent].value > heap[idx].value) {
        [heap[parent], heap[idx]] = [heap[idx], heap[parent]];
        siftUp(heap, parent);
      }
    };
    
    // Helper function to sift down
    const siftDown = (heap, idx) => {
      const left = 2 * idx + 1;
      const right = 2 * idx + 2;
      let smallest = idx;
      
      if (left < heap.length && heap[left].value < heap[smallest].value) {
        smallest = left;
      }
      
      if (right < heap.length && heap[right].value < heap[smallest].value) {
        smallest = right;
      }
      
      if (smallest !== idx) {
        [heap[smallest], heap[idx]] = [heap[idx], heap[smallest]];
        siftDown(heap, smallest);
      }
    };
    
    // Process each element
    for (let i = 0; i < array.length; i++) {
      if (minHeap.length < k) {
        // Add element to min heap
        minHeap.push({ value: array[i], originalIndex: i });
        siftUp(minHeap, minHeap.length - 1);
      } else if (array[i] > minHeap[0].value) {
        // Replace root with new element and heapify
        minHeap[0] = { value: array[i], originalIndex: i };
        siftDown(minHeap, 0);
      }
    }
    
    return minHeap.map(item => item.originalIndex);
  };
  
  // Median finding functionality
  const addNumberToMedian = () => {
    const num = parseInt(newNumber);
    if (isNaN(num)) return;
    
    const newMedianArray = [...medianArray, num];
    setMedianArray(newMedianArray);
    setNewNumber('');
    
    // Only compute median when there are numbers
    if (newMedianArray.length > 0) {
      findMedianTwoHeaps(newMedianArray);
    }
  };
  
  // Find median using two heaps method
  const findMedianTwoHeaps = (nums) => {
    // Reset heaps
    let maxHeapNew = [];
    let minHeapNew = [];
    
    // Helper functions for max heap
    const maxHeapPush = (heap, val) => {
      heap.push(val);
      let idx = heap.length - 1;
      while (idx > 0) {
        const parent = Math.floor((idx - 1) / 2);
        if (heap[parent] >= heap[idx]) break;
        [heap[parent], heap[idx]] = [heap[idx], heap[parent]];
        idx = parent;
      }
    };
    
    const maxHeapPop = (heap) => {
      const result = heap[0];
      const last = heap.pop();
      if (heap.length === 0) return result;
      
      heap[0] = last;
      let idx = 0;
      while (true) {
        const left = 2 * idx + 1;
        const right = 2 * idx + 2;
        let largest = idx;
        
        if (left < heap.length && heap[left] > heap[largest]) {
          largest = left;
        }
        
        if (right < heap.length && heap[right] > heap[largest]) {
          largest = right;
        }
        
        if (largest === idx) break;
        
        [heap[idx], heap[largest]] = [heap[largest], heap[idx]];
        idx = largest;
      }
      
      return result;
    };
    
    // Helper functions for min heap
    const minHeapPush = (heap, val) => {
      heap.push(val);
      let idx = heap.length - 1;
      while (idx > 0) {
        const parent = Math.floor((idx - 1) / 2);
        if (heap[parent] <= heap[idx]) break;
        [heap[parent], heap[idx]] = [heap[idx], heap[parent]];
        idx = parent;
      }
    };
    
    const minHeapPop = (heap) => {
      const result = heap[0];
      const last = heap.pop();
      if (heap.length === 0) return result;
      
      heap[0] = last;
      let idx = 0;
      while (true) {
        const left = 2 * idx + 1;
        const right = 2 * idx + 2;
        let smallest = idx;
        
        if (left < heap.length && heap[left] < heap[smallest]) {
          smallest = left;
        }
        
        if (right < heap.length && heap[right] < heap[smallest]) {
          smallest = right;
        }
        
        if (smallest === idx) break;
        
        [heap[idx], heap[smallest]] = [heap[smallest], heap[idx]];
        idx = smallest;
      }
      
      return result;
    };
    
    // Build the heaps
    for (const num of nums) {
      // Add to appropriate heap
      if (maxHeapNew.length === 0 || num <= maxHeapNew[0]) {
        maxHeapPush(maxHeapNew, num);
      } else {
        minHeapPush(minHeapNew, num);
      }
      
      // Balance heaps
      if (maxHeapNew.length > minHeapNew.length + 1) {
        minHeapPush(minHeapNew, maxHeapPop(maxHeapNew));
      } else if (minHeapNew.length > maxHeapNew.length) {
        maxHeapPush(maxHeapNew, minHeapPop(minHeapNew));
      }
    }
    
    // Calculate median
    const median = maxHeapNew.length > minHeapNew.length 
      ? maxHeapNew[0] 
      : (maxHeapNew[0] + minHeapNew[0]) / 2;
    
    setMaxHeap(maxHeapNew);
    setMinHeap(minHeapNew);
    setMedianValue(median);
    setIsMedianHeapified(true);
  };
  
  // Reset median calculation
  const resetMedian = () => {
    setMedianArray([]);
    setMedianValue(null);
    setMaxHeap([]);
    setMinHeap([]);
    setIsMedianHeapified(false);
  };
  
  // Generate random data for median
  const generateRandomMedianData = () => {
    resetMedian();
    const size = 9; // Odd size so median is clear
    const newArray = Array.from({ length: size }, () => 
      Math.floor(Math.random() * 90) + 10
    );
    setMedianArray(newArray);
    findMedianTwoHeaps(newArray);
  };
  
  // Render a bar with specified height and color
  const renderBar = (value, index, maxValue, highlightColor = null) => {
    const height = (value / maxValue) * 200;
    const width = 40;
    const margin = 5;
    
    // Determine the color based on the state
    let color = colors.cyanColor;
    
    if (highlightColor) {
      color = highlightColor;
    }
    
    return (
      <div 
        key={index} 
        style={{
          height: `${height}px`,
          width: `${width}px`,
          backgroundColor: color,
          margin: `0 ${margin}px`,
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'flex-end',
          position: 'relative',
          borderRadius: '4px 4px 0 0'
        }}
      >
        <div style={{ 
          position: 'absolute', 
          bottom: '-25px',
          fontSize: '14px',
          color: '#fff' 
        }}>
          {value}
        </div>
        <div style={{ 
          position: 'absolute', 
          top: '-20px',
          fontSize: '12px',
          color: '#aaa' 
        }}>
          {index}
        </div>
      </div>
    );
  };
  
  // Render the current state of the heap sort process
  const renderCurrentState = () => {
    if (sortingSteps.length === 0 || currentStep >= sortingSteps.length) {
      return (
        <div style={{
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'flex-end',
          height: '250px',
          marginTop: '20px'
        }}>
          {array.map((value, index) => 
            renderBar(value, index, Math.max(...array))
          )}
        </div>
      );
    }
    
    const step = sortingSteps[currentStep];
    const maxValue = Math.max(...step.array);
    
    return (
      <div>
        <div style={{
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'flex-end',
          height: '250px',
          marginTop: '20px'
        }}>
          {step.array.map((value, index) => {
            let color = colors.cyanColor;
            
            // Nodes being compared during heapify
            if (step.compareIndices && step.compareIndices.includes(index)) {
              color = colors.yellowColor;
            }
            
            // Nodes being swapped
            if (step.swapIndices && step.swapIndices.includes(index)) {
              color = colors.pinkColor;
            }
            
            // Heapify index
            if (index === step.heapifyIndex) {
              color = colors.greenColor;
            }
            
            // Sorted indices
            if (step.sortedIndices && step.sortedIndices.includes(index)) {
              color = colors.greenColor;
            }
            
            // Extract index
            if (index === step.extractIndex) {
              color = colors.pinkColor;
            }
            
            return renderBar(value, index, maxValue, color);
          })}
        </div>
        <div style={{
          textAlign: 'center',
          margin: '40px 20px 10px',
          padding: '10px',
          backgroundColor: '#222',
          borderRadius: '4px',
          color: '#fff'
        }}>
          {step.message}
        </div>
      </div>
    );
  };
  
  // Render controls for heap sort visualization
  const renderHeapSortControls = () => {
    return (
      <div style={{
        display: 'flex',
        justifyContent: 'center',
        flexWrap: 'wrap',
        gap: '10px',
        marginTop: '20px'
      }}>
        <button 
          onClick={generateArray}
          style={{
            padding: '8px 16px',
            backgroundColor: '#333',
            color: colors.cyanColor,
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer'
          }}
        >
          Generate Array
        </button>
        
        <button 
          onClick={heapSort}
          disabled={array.length === 0 || sortingSteps.length > 0}
          style={{
            padding: '8px 16px',
            backgroundColor: '#333',
            color: colors.greenColor,
            border: 'none',
            borderRadius: '4px',
            cursor: array.length === 0 || sortingSteps.length > 0 ? 'not-allowed' : 'pointer',
            opacity: array.length === 0 || sortingSteps.length > 0 ? 0.6 : 1
          }}
        >
          Sort
        </button>
        
        <button 
          onClick={() => setIsPlaying(!isPlaying)}
          disabled={sortingSteps.length === 0 || currentStep >= sortingSteps.length - 1}
          style={{
            padding: '8px 16px',
            backgroundColor: '#333',
            color: isPlaying ? colors.yellowColor : colors.pinkColor,
            border: 'none',
            borderRadius: '4px',
            cursor: sortingSteps.length === 0 || currentStep >= sortingSteps.length - 1 ? 'not-allowed' : 'pointer',
            opacity: sortingSteps.length === 0 || currentStep >= sortingSteps.length - 1 ? 0.6 : 1
          }}
        >
          {isPlaying ? 'Pause' : 'Play'}
        </button>
        
        <button 
          onClick={() => setCurrentStep(0)}
          disabled={sortingSteps.length === 0}
          style={{
            padding: '8px 16px',
            backgroundColor: '#333',
            color: colors.yellowColor,
            border: 'none',
            borderRadius: '4px',
            cursor: sortingSteps.length === 0 ? 'not-allowed' : 'pointer',
            opacity: sortingSteps.length === 0 ? 0.6 : 1
          }}
        >
          Restart
        </button>
        
        <div style={{
          display: 'flex',
          alignItems: 'center',
          gap: '5px',
          color: '#fff'
        }}>
          <label>Speed:</label>
          <select 
            value={playbackSpeed}
            onChange={(e) => setPlaybackSpeed(Number(e.target.value))}
            style={{
              padding: '8px',
              backgroundColor: '#333',
              color: '#fff',
              border: 'none',
              borderRadius: '4px'
            }}
          >
            <option value={1000}>Slow</option>
            <option value={500}>Medium</option>
            <option value={200}>Fast</option>
          </select>
        </div>
      </div>
    );
  };
  
  // Render progress bar for steps
  const renderProgressBar = () => {
    if (sortingSteps.length === 0) return null;
    
    return (
      <div style={{
        margin: '20px 0',
        backgroundColor: '#333',
        height: '30px',
        borderRadius: '15px',
        overflow: 'hidden',
        position: 'relative'
      }}>
        <div style={{
          width: `${(currentStep / (sortingSteps.length - 1)) * 100}%`,
          height: '100%',
          backgroundColor: colors.greenColor,
          transition: 'width 0.3s ease'
        }} />
        <div style={{
          position: 'absolute',
          top: 0,
          left: 0,
          right: 0,
          bottom: 0,
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'center',
          color: '#fff',
          fontSize: '14px'
        }}>
          Step {currentStep + 1} of {sortingSteps.length}
        </div>
      </div>
    );
  };
  
  // Render Top-K elements visualization
  const renderTopKElements = () => {
    if (array.length === 0) {
      return (
        <div style={{ 
          color: '#fff', 
          textAlign: 'center',
          marginTop: '50px' 
        }}>
          Generate an array first to find top-K elements
        </div>
      );
    }
    
    const topKIndices = findTopK() || [];
    const maxValue = Math.max(...array);
    
    return (
      <div>
        <div style={{ 
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          gap: '20px',
          marginTop: '20px'
        }}>
          <div style={{
            display: 'flex',
            alignItems: 'center',
            gap: '10px'
          }}>
            <label style={{ color: '#fff' }}>K value:</label>
            <input 
              type="number" 
              min={1} 
              max={array.length}
              value={topKValue}
              onChange={(e) => setTopKValue(Math.min(Math.max(1, parseInt(e.target.value) || 1), array.length))}
              style={{
                width: '50px',
                padding: '8px',
                backgroundColor: '#333',
                color: '#fff',
                border: 'none',
                borderRadius: '4px',
                textAlign: 'center'
              }}
            />
          </div>
          
          <div style={{
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'flex-end',
            height: '250px'
          }}>
            {array.map((value, index) => {
              const isTopK = topKIndices.includes(index);
              const color = isTopK ? colors.pinkColor : colors.grayColor;
              return renderBar(value, index, maxValue, color);
            })}
          </div>
          
          <div style={{
            padding: '15px',
            backgroundColor: '#222',
            borderRadius: '4px',
            color: '#fff',
            maxWidth: '600px',
            textAlign: 'center'
          }}>
            Top {topKValue} largest elements highlighted in pink.
            <div style={{ marginTop: '10px' }}>
              <span style={{ color: colors.pinkColor, fontWeight: 'bold' }}>
                [{topKIndices.map(idx => array[idx]).sort((a, b) => b - a).join(', ')}]
              </span>
            </div>
          </div>
          
          <div style={{
            padding: '15px',
            backgroundColor: '#222',
            borderRadius: '4px',
            maxWidth: '600px',
            color: '#ddd',
            fontSize: '14px'
          }}>
            <div style={{ marginBottom: '10px', color: colors.cyanColor }}>How it works:</div>
            <ol style={{ paddingLeft: '20px', marginTop: '0' }}>
              <li>Use a min-heap to track the K largest elements.</li>
              <li>Process each element in the array.</li>
              <li>If the heap size is less than K, add the element.</li>
              <li>If the element is larger than the smallest in the heap, replace the root.</li>
              <li>At the end, the heap contains the K largest elements.</li>
            </ol>
            <div>Time Complexity: O(n log k) where n is the array size</div>
          </div>
        </div>
      </div>
    );
  };
  
  // Render Median Finding visualization
  const renderMedianFinding = () => {
    return (
      <div style={{
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        gap: '20px',
        marginTop: '20px'
      }}>
        <div style={{
          display: 'flex',
          gap: '10px',
          alignItems: 'center'
        }}>
          <input 
            type="text" 
            value={newNumber}
            onChange={(e) => setNewNumber(e.target.value)}
            placeholder="Enter a number"
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: '#fff',
              border: 'none',
              borderRadius: '4px',
              width: '120px'
            }}
          />
          
          <button 
            onClick={addNumberToMedian}
            style={{
              padding: '8px 16px',
              backgroundColor: '#333',
              color: colors.greenColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Add Number
          </button>
          
          <button 
            onClick={generateRandomMedianData}
            style={{
              padding: '8px 16px',
              backgroundColor: '#333',
              color: colors.cyanColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Generate Random Data
          </button>
          
          <button 
            onClick={resetMedian}
            style={{
              padding: '8px 16px',
              backgroundColor: '#333',
              color: colors.yellowColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Reset
          </button>
        </div>
        
        {medianArray.length > 0 && (
          <div>
            <div style={{
              padding: '15px',
              backgroundColor: '#222',
              borderRadius: '4px',
              color: '#fff',
              textAlign: 'center',
              marginBottom: '20px'
            }}>
              <div>Numbers: [{medianArray.join(', ')}]</div>
              <div style={{ marginTop: '10px' }}>
                Median: <span style={{ color: colors.pinkColor, fontWeight: 'bold' }}>
                  {medianValue !== null ? (
                    Number.isInteger(medianValue) ? medianValue : medianValue.toFixed(1)
                  ) : 'N/A'}
                </span>
              </div>
            </div>
            
            {isMedianHeapified && (
              <div style={{
                display: 'flex',
                justifyContent: 'center',
                gap: '40px',
                marginBottom: '20px'
              }}>
                <div style={{
                  display: 'flex',
                  flexDirection: 'column',
                  alignItems: 'center'
                }}>
                  <div style={{ color: colors.yellowColor, marginBottom: '10px' }}>
                    Max Heap (Lower Half):
                  </div>
                  <div style={{
                    display: 'flex',
                    justifyContent: 'center',
                    alignItems: 'flex-end',
                    height: '150px'
                  }}>
                    {maxHeap.map((value, index) => {
                      return renderBar(
                        value, 
                        index, 
                        Math.max(...medianArray, 1), 
                        index === 0 ? colors.pinkColor : colors.yellowColor
                      );
                    })}
                  </div>
                </div>
                
                <div style={{
                  display: 'flex',
                  flexDirection: 'column',
                  alignItems: 'center'
                }}>
                  <div style={{ color: colors.cyanColor, marginBottom: '10px' }}>
                    Min Heap (Upper Half):
                  </div>
                  <div style={{
                    display: 'flex',
                    justifyContent: 'center',
                    alignItems: 'flex-end',
                    height: '150px'
                  }}>
                    {minHeap.map((value, index) => {
                      return renderBar(
                        value, 
                        index, 
                        Math.max(...medianArray, 1), 
                        index === 0 ? colors.pinkColor : colors.cyanColor
                      );
                    })}
                  </div>
                </div>
              </div>
            )}
            
            <div style={{
              padding: '15px',
              backgroundColor: '#222',
              borderRadius: '4px',
              maxWidth: '600px',
              color: '#ddd',
              fontSize: '14px'
            }}>
              <div style={{ marginBottom: '10px', color: colors.cyanColor }}>
                How finding median with two heaps works:
              </div>
              <ol style={{ paddingLeft: '20px', marginTop: '0' }}>
                <li>Use two heaps: a max heap for the lower half and a min heap for the upper half.</li>
                <li>Add each number to the appropriate heap, keeping them balanced.</li>
                <li>If the max heap has one more element, its root is the median.</li>
                <li>If both heaps have the same size, the median is the average of their roots.</li>
              </ol>
              <div>Time Complexity: O(log n) per insertion, O(1) to find median</div>
            </div>
          </div>
        )}
        
        {medianArray.length === 0 && (
          <div style={{ color: '#fff', marginTop: '30px' }}>
            Add numbers to find the median using two heaps
          </div>
        )}
      </div>
    );
  };
  
  // Set up the initial array
  useEffect(() => {
    generateArray();
  }, []);
  
  // Tab content renderer
  const renderTabContent = () => {
    switch (activeTab) {
      case 'heapsort':
        return (
          <div>
            <div style={{ color: colors.pinkColor, fontSize: '24px', textAlign: 'center', margin: '20px 0' }}>
              <ArrowDownUp size={24} style={{ verticalAlign: 'middle', marginRight: '10px' }} />
              Heap Sort Visualization
            </div>
            {renderCurrentState()}
            {renderProgressBar()}
            {renderHeapSortControls()}
          </div>
        );
      case 'topk':
        return (
          <div>
            <div style={{ color: colors.cyanColor, fontSize: '24px', textAlign: 'center', margin: '20px 0' }}>
              <ChevronsUp size={24} style={{ verticalAlign: 'middle', marginRight: '10px' }} />
              Top-K Elements
            </div>
            {renderTopKElements()}
            <div style={{ margin: '20px 0' }}>
              {renderHeapSortControls()}
            </div>
          </div>
        );
      case 'median':
        return (
          <div>
            <div style={{ color: colors.greenColor, fontSize: '24px', textAlign: 'center', margin: '20px 0' }}>
              <Activity size={24} style={{ verticalAlign: 'middle', marginRight: '10px' }} />
              Median Finding with Two Heaps
            </div>
            {renderMedianFinding()}
          </div>
        );
      case 'complexity':
        return (
          <div>
            <div style={{ color: colors.yellowColor, fontSize: '24px', textAlign: 'center', margin: '20px 0' }}>
              <Clock size={24} style={{ verticalAlign: 'middle', marginRight: '10px' }} />
              Time & Space Complexity
            </div>
            
            <div style={{ 
              padding: '20px', 
              backgroundColor: '#222',
              borderRadius: '8px',
              margin: '20px auto',
              maxWidth: '800px'
            }}>
              <h3 style={{ color: colors.pinkColor, marginTop: 0 }}>Heap Sort Complexity</h3>
              <table style={{ 
                width: '100%', 
                borderCollapse: 'collapse',
                marginBottom: '30px'
              }}>
                <thead>
                  <tr style={{ borderBottom: '1px solid #444' }}>
                    <th style={{ padding: '10px', textAlign: 'left', color: colors.cyanColor }}>Case</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Time Complexity</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Space Complexity</th>
                  </tr>
                </thead>
                <tbody>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Best Case</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                  </tr>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Average Case</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                  </tr>
                  <tr>
                    <td style={{ padding: '10px', color: '#fff' }}>Worst Case</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                  </tr>
                </tbody>
              </table>
              
              <h3 style={{ color: colors.pinkColor }}>Heap Applications Complexity</h3>
              <table style={{ 
                width: '100%', 
                borderCollapse: 'collapse',
                marginBottom: '30px'
              }}>
                <thead>
                  <tr style={{ borderBottom: '1px solid #444' }}>
                    <th style={{ padding: '10px', textAlign: 'left', color: colors.cyanColor }}>Application</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Time Complexity</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Space Complexity</th>
                  </tr>
                </thead>
                <tbody>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Find Top-K Elements</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n log k)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(k)</td>
                  </tr>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Find Median (Insert)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n)</td>
                  </tr>
                  <tr>
                    <td style={{ padding: '10px', color: '#fff' }}>Find Median (Query)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>O(1)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                  </tr>
                </tbody>
              </table>
              
              <h3 style={{ color: colors.pinkColor }}>Heap Sort vs. Other Sorting Algorithms</h3>
              <table style={{ 
                width: '100%', 
                borderCollapse: 'collapse'
              }}>
                <thead>
                  <tr style={{ borderBottom: '1px solid #444' }}>
                    <th style={{ padding: '10px', textAlign: 'left', color: colors.cyanColor }}>Algorithm</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Best Case</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Average Case</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Worst Case</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Space</th>
                    <th style={{ padding: '10px', textAlign: 'center', color: colors.cyanColor }}>Stable</th>
                  </tr>
                </thead>
                <tbody>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Heap Sort</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>No</td>
                  </tr>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Quick Sort</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n²)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>No</td>
                  </tr>
                  <tr style={{ borderBottom: '1px solid #333' }}>
                    <td style={{ padding: '10px', color: '#fff' }}>Merge Sort</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.yellowColor }}>O(n log n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>Yes</td>
                  </tr>
                  <tr>
                    <td style={{ padding: '10px', color: '#fff' }}>Bubble Sort</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(n)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n²)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.pinkColor }}>O(n²)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>O(1)</td>
                    <td style={{ padding: '10px', textAlign: 'center', color: colors.greenColor }}>Yes</td>
                  </tr>
                </tbody>
              </table>
              
              <div style={{ marginTop: '20px', color: '#aaa', fontSize: '14px' }}>
                <div style={{ marginBottom: '5px' }}>Notes:</div>
                <ul style={{ paddingLeft: '20px', margin: 0 }}>
                  <li>Heap Sort's consistent O(n log n) performance makes it reliable for large datasets.</li>
                  <li>Though not stable, Heap Sort requires no extra memory unlike Merge Sort.</li>
                  <li>Heap Sort is especially useful when you need partial sorting (top-k elements).</li>
                  <li>Heap Sort combines the reliability of Merge Sort with the space efficiency of Quick Sort.</li>
                </ul>
              </div>
            </div>
          </div>
        );
      default:
        return null;
    }
  };
  
  return (
    <div style={{ 
      backgroundColor: colors.bgColor, 
      color: '#fff',
      fontFamily: 'Arial, sans-serif',
      padding: '20px',
      borderRadius: '8px',
      maxWidth: '1000px',
      margin: '0 auto'
    }}>
      <div style={{ 
        textAlign: 'center',
        marginBottom: '20px'
      }}>
        <h1 style={{ 
          color: colors.cyanColor,
          marginBottom: '10px',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center'
        }}>
          <Award size={32} style={{ marginRight: '10px' }} />
          Heap Sort & Applications
        </h1>
        <div style={{ 
          backgroundColor: 'rgba(255, 0, 255, 0.1)',
          padding: '10px',
          borderRadius: '4px',
          maxWidth: '600px',
          margin: '0 auto'
        }}>
          Explore heap sort algorithm and powerful heap applications 
          like finding top-K elements and median in a data stream.
        </div>
      </div>
      
      <div style={{ 
        display: 'flex',
        backgroundColor: '#222',
        borderRadius: '8px',
        overflow: 'hidden',
        marginBottom: '20px'
      }}>
        <button 
          onClick={() => setActiveTab('heapsort')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'heapsort' ? '#333' : 'transparent',
            color: activeTab === 'heapsort' ? colors.pinkColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'heapsort' ? 'bold' : 'normal'
          }}
        >
          Heap Sort
        </button>
        
        <button 
          onClick={() => setActiveTab('topk')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'topk' ? '#333' : 'transparent',
            color: activeTab === 'topk' ? colors.cyanColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'topk' ? 'bold' : 'normal'
          }}
        >
          Top-K Elements
        </button>
        
        <button 
          onClick={() => setActiveTab('median')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'median' ? '#333' : 'transparent',
            color: activeTab === 'median' ? colors.greenColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'median' ? 'bold' : 'normal'
          }}
        >
          Median Finding
        </button>
        
        <button 
          onClick={() => setActiveTab('complexity')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'complexity' ? '#333' : 'transparent',
            color: activeTab === 'complexity' ? colors.yellowColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'complexity' ? 'bold' : 'normal'
          }}
        >
          Complexity
        </button>
      </div>
      
      <div style={{ 
        backgroundColor: '#1a1a1a',
        padding: '20px',
        borderRadius: '8px'
      }}>
        {renderTabContent()}
      </div>
      
      <div style={{ 
        textAlign: 'center',
        marginTop: '20px',
        fontSize: '14px',
        color: '#888'
      }}>
        Day 26 of #DSAin45 - Heap Sort & Applications
      </div>
    </div>
  );
};

export default HeapSortVisualization;