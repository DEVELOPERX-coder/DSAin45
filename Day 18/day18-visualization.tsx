import React, { useState, useEffect } from 'react';

const EfficientSortingVisualization = () => {
  // State to track which algorithm is being demonstrated
  const [activeAlgo, setActiveAlgo] = useState('merge');
  const [arraySize, setArraySize] = useState(15);
  const [arrayType, setArrayType] = useState('random');
  const [array, setArray] = useState([]);
  const [animationStep, setAnimationStep] = useState(0);
  const [animationSteps, setAnimationSteps] = useState([]);
  const [animationPlaying, setAnimationPlaying] = useState(false);
  const [animationSpeed, setAnimationSpeed] = useState(500);
  const [comparisons, setComparisons] = useState(0);
  const [operations, setOperations] = useState(0);
  const [extraSpace, setExtraSpace] = useState(0);

  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    purple: '#BB00FF',
    orange: '#FF8800',
    text: '#FFFFFF',
    darkCyan: 'rgba(0, 255, 255, 0.15)',
    darkPink: 'rgba(255, 0, 255, 0.15)',
    darkYellow: 'rgba(255, 255, 0, 0.15)',
    darkGreen: 'rgba(0, 255, 0, 0.15)',
    darkPurple: 'rgba(187, 0, 255, 0.15)',
    darkOrange: 'rgba(255, 136, 0, 0.15)',
  };

  // Styles
  const containerStyle = {
    backgroundColor: colors.background,
    padding: '20px',
    fontFamily: 'monospace',
    color: colors.text,
    borderRadius: '8px',
    width: '100%',
    maxWidth: '900px',
    margin: '0 auto'
  };

  const headerStyle = {
    textAlign: 'center',
    color: colors.yellow,
    marginBottom: '20px',
    padding: '10px',
    borderBottom: `2px solid ${colors.yellow}`
  };

  const sectionStyle = {
    marginBottom: '30px',
  };

  const sectionHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    marginBottom: '15px'
  };

  const boxStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.3)',
    borderRadius: '8px',
    padding: '15px',
    marginBottom: '20px',
    border: `1px solid ${colors.pink}`
  };

  const codeBlockStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    padding: '15px',
    borderRadius: '4px',
    fontFamily: 'monospace',
    overflowX: 'auto',
    border: `1px solid ${colors.green}`,
    marginTop: '10px',
    marginBottom: '10px',
  };

  const footerStyle = {
    textAlign: 'center',
    marginTop: '30px',
    color: colors.yellow,
    fontStyle: 'italic'
  };

  const tabsContainerStyle = {
    display: 'flex',
    justifyContent: 'space-around',
    marginBottom: '20px',
    flexWrap: 'wrap',
    gap: '10px'
  };

  const tabStyle = (isActive) => ({
    padding: '10px 15px',
    cursor: 'pointer',
    backgroundColor: isActive ? 'rgba(0, 255, 255, 0.2)' : 'rgba(0, 0, 0, 0.3)',
    borderRadius: '5px',
    color: isActive ? colors.cyan : colors.text,
    border: isActive ? `1px solid ${colors.cyan}` : '1px solid rgba(255, 255, 255, 0.1)',
    transition: 'all 0.3s ease',
  });

  const buttonStyle = {
    backgroundColor: 'rgba(0, 255, 0, 0.2)',
    border: `1px solid ${colors.green}`,
    color: colors.green,
    padding: '8px 15px',
    borderRadius: '4px',
    cursor: 'pointer',
    margin: '0 5px',
    transition: 'all 0.3s ease',
  };

  const selectStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    border: `1px solid ${colors.yellow}`,
    color: colors.yellow,
    padding: '8px',
    borderRadius: '4px',
    margin: '0 10px',
    cursor: 'pointer',
  };

  const statsContainerStyle = {
    display: 'flex',
    justifyContent: 'space-around',
    marginTop: '20px',
    marginBottom: '20px',
    flexWrap: 'wrap',
    gap: '10px'
  };

  const statBoxStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.3)',
    border: `1px solid ${colors.cyan}`,
    borderRadius: '5px',
    padding: '10px',
    minWidth: '120px',
    textAlign: 'center'
  };

  // Generate a random array
  const generateRandomArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(Math.floor(Math.random() * 100) + 1);
    }
    return arr;
  };

  // Generate a nearly sorted array
  const generateNearlySortedArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(i + 1);
    }
    
    // Swap a few elements to make it nearly sorted
    const swaps = Math.max(2, Math.floor(size * 0.1)); // Swap about 10% of elements
    for (let i = 0; i < swaps; i++) {
      const idx1 = Math.floor(Math.random() * size);
      const idx2 = Math.floor(Math.random() * size);
      const temp = arr[idx1];
      arr[idx1] = arr[idx2];
      arr[idx2] = temp;
    }
    
    return arr;
  };

  // Generate a reversed array
  const generateReversedArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(size - i);
    }
    return arr;
  };

  // Generate an array with many duplicates
  const generateDuplicatesArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(Math.floor(Math.random() * 10) + 1); // Only 10 different values
    }
    return arr;
  };

  // Generate the initial array based on type and size
  const generateArray = () => {
    let newArray;
    switch (arrayType) {
      case 'nearly-sorted':
        newArray = generateNearlySortedArray(arraySize);
        break;
      case 'reversed':
        newArray = generateReversedArray(arraySize);
        break;
      case 'duplicates':
        newArray = generateDuplicatesArray(arraySize);
        break;
      case 'random':
      default:
        newArray = generateRandomArray(arraySize);
        break;
    }
    
    setArray(newArray);
    generateAnimationSteps(newArray);
    resetAnimation();
  };

  // Reset animation state
  const resetAnimation = () => {
    setAnimationStep(0);
    setAnimationPlaying(false);
    setComparisons(0);
    setOperations(0);
    setExtraSpace(0);
  };

  // Generate merge sort animation steps
  const generateMergeSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let opCount = 0;
    let maxExtraSpace = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      mergeRange: [],
      left: [],
      right: [],
      auxiliaryArray: [],
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    // Recursive function to simulate merge sort steps
    const mergeSortHelper = (arr, left, right) => {
      if (left >= right) return;
      
      const mid = Math.floor((left + right) / 2);
      
      // Add step showing the division
      steps.push({
        array: [...arr],
        mergeRange: [left, right],
        divideAt: mid,
        left: [],
        right: [],
        auxiliaryArray: [],
        compareCount,
        opCount,
        extraSpace: maxExtraSpace
      });
      
      // Recursive sort first half
      mergeSortHelper(arr, left, mid);
      
      // Recursive sort second half
      mergeSortHelper(arr, mid + 1, right);
      
      // Merge the sorted halves
      const leftSize = mid - left + 1;
      const rightSize = right - mid;
      
      // Create temporary arrays
      const leftArr = new Array(leftSize);
      const rightArr = new Array(rightSize);
      
      // Update max extra space used
      maxExtraSpace = Math.max(maxExtraSpace, leftSize + rightSize);
      
      // Copy data to temp arrays
      for (let i = 0; i < leftSize; i++) {
        leftArr[i] = arr[left + i];
        opCount++;
      }
      
      for (let j = 0; j < rightSize; j++) {
        rightArr[j] = arr[mid + 1 + j];
        opCount++;
      }
      
      // Add step showing the subarrays to be merged
      steps.push({
        array: [...arr],
        mergeRange: [left, right],
        left: [...leftArr],
        right: [...rightArr],
        auxiliaryArray: [],
        compareCount,
        opCount,
        extraSpace: maxExtraSpace
      });
      
      // Merge the arrays
      let i = 0, j = 0, k = left;
      const auxiliaryArray = new Array(right - left + 1).fill(null);
      let auxIndex = 0;
      
      while (i < leftSize && j < rightSize) {
        compareCount++;
        
        if (leftArr[i] <= rightArr[j]) {
          arr[k] = leftArr[i];
          auxiliaryArray[auxIndex] = leftArr[i];
          i++;
        } else {
          arr[k] = rightArr[j];
          auxiliaryArray[auxIndex] = rightArr[j];
          j++;
        }
        
        k++;
        auxIndex++;
        opCount++;
        
        // Add step showing the merge progress
        steps.push({
          array: [...arr],
          mergeRange: [left, right],
          left: [...leftArr],
          right: [...rightArr],
          leftPos: i,
          rightPos: j,
          mergePos: k - 1,
          auxiliaryArray: [...auxiliaryArray],
          compareCount,
          opCount,
          extraSpace: maxExtraSpace
        });
      }
      
      // Copy remaining elements from left array
      while (i < leftSize) {
        arr[k] = leftArr[i];
        auxiliaryArray[auxIndex] = leftArr[i];
        i++;
        k++;
        auxIndex++;
        opCount++;
        
        // Add step showing the remainder copy
        steps.push({
          array: [...arr],
          mergeRange: [left, right],
          left: [...leftArr],
          right: [...rightArr],
          leftPos: i,
          rightPos: j,
          mergePos: k - 1,
          auxiliaryArray: [...auxiliaryArray],
          compareCount,
          opCount,
          extraSpace: maxExtraSpace
        });
      }
      
      // Copy remaining elements from right array
      while (j < rightSize) {
        arr[k] = rightArr[j];
        auxiliaryArray[auxIndex] = rightArr[j];
        j++;
        k++;
        auxIndex++;
        opCount++;
        
        // Add step showing the remainder copy
        steps.push({
          array: [...arr],
          mergeRange: [left, right],
          left: [...leftArr],
          right: [...rightArr],
          leftPos: i,
          rightPos: j,
          mergePos: k - 1,
          auxiliaryArray: [...auxiliaryArray],
          compareCount,
          opCount,
          extraSpace: maxExtraSpace
        });
      }
      
      // Add step showing merged result
      steps.push({
        array: [...arr],
        mergeRange: [left, right],
        left: [],
        right: [],
        merged: true,
        auxiliaryArray: [],
        compareCount,
        opCount,
        extraSpace: maxExtraSpace
      });
    };
    
    // Start the merge sort simulation
    mergeSortHelper(copy, 0, copy.length - 1);
    
    // Final sorted state
    steps.push({
      array: [...copy],
      mergeRange: [],
      left: [],
      right: [],
      auxiliaryArray: [],
      complete: true,
      compareCount,
      opCount,
      extraSpace: maxExtraSpace
    });
    
    return steps;
  };

  // Generate quick sort animation steps
  const generateQuickSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let opCount = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      sortRange: [],
      pivot: null,
      partitioning: false,
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    // Recursive function to simulate quick sort steps
    const quickSortHelper = (arr, low, high) => {
      if (low < high) {
        // Add step showing the range to be sorted
        steps.push({
          array: [...arr],
          sortRange: [low, high],
          pivot: null,
          partitioning: false,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        // Choose pivot (last element)
        const pivot = arr[high];
        
        // Add step showing pivot selection
        steps.push({
          array: [...arr],
          sortRange: [low, high],
          pivot: high,
          partitioning: true,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        // Partition the array
        let i = low - 1;
        
        for (let j = low; j < high; j++) {
          // Add step showing comparison with pivot
          compareCount++;
          steps.push({
            array: [...arr],
            sortRange: [low, high],
            pivot: high,
            comparing: [j, high],
            pivotValue: pivot,
            partitionIndex: i,
            currentIndex: j,
            partitioning: true,
            compareCount,
            opCount,
            extraSpace: 0
          });
          
          if (arr[j] < pivot) {
            i++;
            
            // Swap arr[i] and arr[j]
            [arr[i], arr[j]] = [arr[j], arr[i]];
            opCount++;
            
            // Add step showing the swap
            steps.push({
              array: [...arr],
              sortRange: [low, high],
              pivot: high,
              swapping: [i, j],
              pivotValue: pivot,
              partitionIndex: i,
              partitioning: true,
              compareCount,
              opCount,
              extraSpace: 0
            });
          }
        }
        
        // Swap pivot into correct position
        [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
        opCount++;
        
        // Add step showing pivot placement
        steps.push({
          array: [...arr],
          sortRange: [low, high],
          pivot: i + 1,
          pivotPlaced: true,
          swapping: [i + 1, high],
          partitioning: true,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        const pivotIndex = i + 1;
        
        // Add step showing partition complete
        steps.push({
          array: [...arr],
          sortRange: [low, high],
          pivot: pivotIndex,
          partitionComplete: true,
          leftPartition: low < pivotIndex - 1 ? [low, pivotIndex - 1] : null,
          rightPartition: pivotIndex + 1 < high ? [pivotIndex + 1, high] : null,
          partitioning: false,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        // Recursively sort the subarrays
        quickSortHelper(arr, low, pivotIndex - 1);
        quickSortHelper(arr, pivotIndex + 1, high);
      }
    };
    
    // Start the quick sort simulation
    quickSortHelper(copy, 0, copy.length - 1);
    
    // Final sorted state
    steps.push({
      array: [...copy],
      sortRange: [],
      pivot: null,
      partitioning: false,
      complete: true,
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    return steps;
  };

  // Generate heap sort animation steps
  const generateHeapSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let opCount = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      heapifying: false,
      heapRange: null,
      comparing: [],
      swapping: [],
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    // Function to simulate heapify
    const heapify = (arr, n, i) => {
      let largest = i;
      const left = 2 * i + 1;
      const right = 2 * i + 2;
      
      // Add step showing heapify start
      steps.push({
        array: [...arr],
        heapifying: true,
        heapIndex: i,
        heapRange: n,
        heapRoot: i,
        leftChild: left < n ? left : null,
        rightChild: right < n ? right : null,
        largest: largest,
        compareCount,
        opCount,
        extraSpace: 0
      });
      
      // Check if left child is larger than root
      if (left < n) {
        compareCount++;
        
        // Add step showing left child comparison
        steps.push({
          array: [...arr],
          heapifying: true,
          heapRange: n,
          heapRoot: i,
          comparing: [largest, left],
          leftChild: left,
          rightChild: right < n ? right : null,
          largest: largest,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        if (arr[left] > arr[largest]) {
          largest = left;
          
          // Add step showing largest update
          steps.push({
            array: [...arr],
            heapifying: true,
            heapRange: n,
            heapRoot: i,
            leftChild: left,
            rightChild: right < n ? right : null,
            largest: largest,
            compareCount,
            opCount,
            extraSpace: 0
          });
        }
      }
      
      // Check if right child is larger than largest so far
      if (right < n) {
        compareCount++;
        
        // Add step showing right child comparison
        steps.push({
          array: [...arr],
          heapifying: true,
          heapRange: n,
          heapRoot: i,
          comparing: [largest, right],
          leftChild: left < n ? left : null,
          rightChild: right,
          largest: largest,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        if (arr[right] > arr[largest]) {
          largest = right;
          
          // Add step showing largest update
          steps.push({
            array: [...arr],
            heapifying: true,
            heapRange: n,
            heapRoot: i,
            leftChild: left < n ? left : null,
            rightChild: right,
            largest: largest,
            compareCount,
            opCount,
            extraSpace: 0
          });
        }
      }
      
      // If largest is not root
      if (largest !== i) {
        // Swap
        [arr[i], arr[largest]] = [arr[largest], arr[i]];
        opCount++;
        
        // Add step showing the swap
        steps.push({
          array: [...arr],
          heapifying: true,
          heapRange: n,
          heapRoot: i,
          swapping: [i, largest],
          leftChild: left < n ? left : null,
          rightChild: right < n ? right : null,
          largest: largest,
          compareCount,
          opCount,
          extraSpace: 0
        });
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
      }
    };
    
    // Build max heap
    const n = copy.length;
    
    // Add step showing heap building phase
    steps.push({
      array: [...copy],
      buildingHeap: true,
      heapRange: n,
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
      heapify(copy, n, i);
    }
    
    // Add step showing heap built
    steps.push({
      array: [...copy],
      heapBuilt: true,
      heapRange: n,
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    // Extract elements from heap one by one
    for (let i = n - 1; i > 0; i--) {
      // Move current root to end
      [copy[0], copy[i]] = [copy[i], copy[0]];
      opCount++;
      
      // Add step showing the swap
      steps.push({
        array: [...copy],
        extractingMax: true,
        heapRange: i,
        swapping: [0, i],
        extracted: i,
        compareCount,
        opCount,
        extraSpace: 0
      });
      
      // Call heapify on the reduced heap
      heapify(copy, i, 0);
      
      // Add step showing reduced heap after heapify
      steps.push({
        array: [...copy],
        heapified: true,
        heapRange: i,
        sorted: Array.from({ length: n }, (_, idx) => idx >= i),
        compareCount,
        opCount,
        extraSpace: 0
      });
    }
    
    // Final sorted state
    steps.push({
      array: [...copy],
      heapifying: false,
      heapRange: null,
      complete: true,
      compareCount,
      opCount,
      extraSpace: 0
    });
    
    return steps;
  };

  // Generate steps for Introsort (hybrid of QuickSort, HeapSort, and InsertionSort)
  const generateIntroSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let opCount = 0;
    let extraSpaceUsed = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      algorithm: 'intro',
      phase: 'start',
      compareCount,
      opCount,
      extraSpace: extraSpaceUsed
    });
    
    // Since Introsort is a hybrid algorithm, we'll simulate a simplified version
    // that shows the key aspects of its behavior
    
    // Calculate max recursion depth
    const maxDepth = Math.floor(2 * Math.log2(copy.length));
    
    // Add step showing depth calculation
    steps.push({
      array: [...copy],
      algorithm: 'intro',
      phase: 'depth-calculation',
      maxDepth,
      message: `Max depth limit: ${maxDepth}`,
      compareCount,
      opCount,
      extraSpace: extraSpaceUsed
    });
    
    // Function to simulate introsort steps
    const introSortHelper = (arr, low, high, depth) => {
      // If array segment is small, use insertion sort
      if (high - low < 16) {
        // Add step showing switch to insertion sort
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'insertion',
          sortRange: [low, high],
          message: `Range size ${high - low + 1} < 16, using insertion sort`,
          compareCount,
          opCount,
          extraSpace: extraSpaceUsed
        });
        
        // Insertion sort
        for (let i = low + 1; i <= high; i++) {
          let key = arr[i];
          let j = i - 1;
          
          // Add step showing key selection
          steps.push({
            array: [...arr],
            algorithm: 'intro',
            phase: 'insertion',
            sortRange: [low, high],
            key: i,
            compareCount,
            opCount,
            extraSpace: extraSpaceUsed
          });
          
          while (j >= low && arr[j] > key) {
            compareCount++;
            arr[j + 1] = arr[j];
            opCount++;
            
            // Add step showing shift
            steps.push({
              array: [...arr],
              algorithm: 'intro',
              phase: 'insertion',
              sortRange: [low, high],
              key: i,
              comparing: [j, i],
              shifting: j,
              compareCount,
              opCount,
              extraSpace: extraSpaceUsed
            });
            
            j--;
          }
          
          compareCount++; // For the comparison that exits the loop
          
          if (j + 1 !== i) {
            arr[j + 1] = key;
            opCount++;
            
            // Add step showing key placement
            steps.push({
              array: [...arr],
              algorithm: 'intro',
              phase: 'insertion',
              sortRange: [low, high],
              keyPlaced: j + 1,
              compareCount,
              opCount,
              extraSpace: extraSpaceUsed
            });
          }
        }
        
        // Add step showing insertion sort complete
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'insertion-complete',
          sortRange: [low, high],
          message: 'Insertion sort complete for this range',
          compareCount,
          opCount,
          extraSpace: extraSpaceUsed
        });
        
        return;
      }
      
      // If recursion depth exceeds limit, switch to heap sort
      if (depth === 0) {
        // Add step showing switch to heap sort
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'heapsort',
          sortRange: [low, high],
          message: 'Depth limit reached, switching to heap sort',
          compareCount,
          opCount,
          extraSpace: extraSpaceUsed
        });
        
        // Create a subarray for heap sort (simulate)
        const subarray = arr.slice(low, high + 1);
        
        // Add heapsort simulation steps for this subarray
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'heapsort-running',
          sortRange: [low, high],
          message: 'Running heap sort on subarray',
          compareCount: compareCount + Math.floor(subarray.length * Math.log2(subarray.length)),
          opCount: opCount + Math.floor(subarray.length * Math.log2(subarray.length)),
          extraSpace: extraSpaceUsed
        });
        
        // Sort the subarray with heap sort
        subarray.sort((a, b) => a - b);
        
        // Copy back to original array
        for (let i = 0; i < subarray.length; i++) {
          arr[low + i] = subarray[i];
        }
        
        compareCount += Math.floor(subarray.length * Math.log2(subarray.length));
        opCount += Math.floor(subarray.length * Math.log2(subarray.length));
        
        // Add step showing heap sort complete
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'heapsort-complete',
          sortRange: [low, high],
          message: 'Heap sort complete for this range',
          compareCount,
          opCount,
          extraSpace: extraSpaceUsed
        });
        
        return;
      }
      
      // Otherwise, use quicksort
      // Add step showing quicksort phase
      steps.push({
        array: [...arr],
        algorithm: 'intro',
        phase: 'quicksort',
        sortRange: [low, high],
        depthRemaining: depth,
        message: `Using quick sort (depth: ${depth})`,
        compareCount,
        opCount,
        extraSpace: extraSpaceUsed
      });
      
      // Choose pivot (median of three)
      const mid = Math.floor((low + high) / 2);
      
      // Sort low, mid, high
      if (arr[mid] < arr[low]) {
        [arr[mid], arr[low]] = [arr[low], arr[mid]];
        opCount++;
      }
      compareCount++;
      
      if (arr[high] < arr[low]) {
        [arr[high], arr[low]] = [arr[low], arr[high]];
        opCount++;
      }
      compareCount++;
      
      if (arr[high] < arr[mid]) {
        [arr[high], arr[mid]] = [arr[mid], arr[high]];
        opCount++;
      }
      compareCount++;
      
      // Use mid as pivot
      const pivot = arr[mid];
      
      // Add step showing pivot selection
      steps.push({
        array: [...arr],
        algorithm: 'intro',
        phase: 'quicksort-pivot',
        sortRange: [low, high],
        pivot: mid,
        pivotValue: pivot,
        message: 'Selected pivot (median of three)',
        compareCount,
        opCount,
        extraSpace: extraSpaceUsed
      });
      
      // Move pivot to end for standard partitioning
      [arr[mid], arr[high]] = [arr[high], arr[mid]];
      opCount++;
      
      // Partition
      let i = low - 1;
      
      for (let j = low; j < high; j++) {
        compareCount++;
        
        // Add step showing comparison
        steps.push({
          array: [...arr],
          algorithm: 'intro',
          phase: 'quicksort-partition',
          sortRange: [low, high],
          pivotValue: pivot,
          comparing: [j, high],
          partitionIndex: i,
          compareCount,
          opCount,
          extraSpace: extraSpaceUsed
        });
        
        if (arr[j] <= pivot) {
          i++;
          
          // Swap arr[i] and arr[j]
          [arr[i], arr[j]] = [arr[j], arr[i]];
          opCount++;
          
          // Add step showing swap
          steps.push({
            array: [...arr],
            algorithm: 'intro',
            phase: 'quicksort-partition',
            sortRange: [low, high],
            pivotValue: pivot,
            swapping: [i, j],
            partitionIndex: i,
            compareCount,
            opCount,
            extraSpace: extraSpaceUsed
          });
        }
      }
      
      // Put pivot in correct position
      [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
      opCount++;
      
      const pivotIndex = i + 1;
      
      // Add step showing pivot placement
      steps.push({
        array: [...arr],
        algorithm: 'intro',
        phase: 'quicksort-pivot-placed',
        sortRange: [low, high],
        pivot: pivotIndex,
        pivotPlaced: true,
        message: 'Pivot placed in final position',
        compareCount,
        opCount,
        extraSpace: extraSpaceUsed
      });
      
      // Recursively sort the partitions
      if (low < pivotIndex - 1) {
        introSortHelper(arr, low, pivotIndex - 1, depth - 1);
      }
      
      if (pivotIndex + 1 < high) {
        introSortHelper(arr, pivotIndex + 1, high, depth - 1);
      }
    };
    
    // Start the introsort simulation
    introSortHelper(copy, 0, copy.length - 1, maxDepth);
    
    // Final sorted state
    steps.push({
      array: [...copy],
      algorithm: 'intro',
      phase: 'complete',
      complete: true,
      message: 'Introsort complete',
      compareCount,
      opCount,
      extraSpace: extraSpaceUsed
    });
    
    return steps;
  };

  // Generate steps for the selected algorithm
  const generateAnimationSteps = (arr) => {
    let steps;
    
    switch (activeAlgo) {
      case 'merge':
        steps = generateMergeSortSteps(arr);
        break;
      case 'quick':
        steps = generateQuickSortSteps(arr);
        break;
      case 'heap':
        steps = generateHeapSortSteps(arr);
        break;
      case 'intro':
        steps = generateIntroSortSteps(arr);
        break;
      default:
        steps = generateMergeSortSteps(arr);
    }
    
    setAnimationSteps(steps);
  };

  // Initialize on component mount
  useEffect(() => {
    generateArray();
  }, []);

  // Update when algorithm, array size, or array type changes
  useEffect(() => {
    generateArray();
  }, [activeAlgo, arraySize, arrayType]);

  // Animation playback
  useEffect(() => {
    let timer;
    
    if (animationPlaying && animationStep < animationSteps.length - 1) {
      timer = setTimeout(() => {
        setAnimationStep(animationStep + 1);
        
        // Update statistics
        if (animationSteps[animationStep]) {
          setComparisons(animationSteps[animationStep].compareCount);
          setOperations(animationSteps[animationStep].opCount);
          if (animationSteps[animationStep].extraSpace !== undefined) {
            setExtraSpace(animationSteps[animationStep].extraSpace);
          }
        }
      }, animationSpeed);
    } else if (animationPlaying && animationStep >= animationSteps.length - 1) {
      setAnimationPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [animationPlaying, animationStep, animationSteps, animationSpeed]);

  // Start animation
  const startAnimation = () => {
    if (animationStep >= animationSteps.length - 1) {
      setAnimationStep(0);
    }
    setAnimationPlaying(true);
  };

  // Pause animation
  const pauseAnimation = () => {
    setAnimationPlaying(false);
  };

  // Step forward
  const stepForward = () => {
    if (animationStep < animationSteps.length - 1) {
      setAnimationStep(animationStep + 1);
      
      // Update statistics
      if (animationSteps[animationStep]) {
        setComparisons(animationSteps[animationStep].compareCount);
        setOperations(animationSteps[animationStep].opCount);
        if (animationSteps[animationStep].extraSpace !== undefined) {
          setExtraSpace(animationSteps[animationStep].extraSpace);
        }
      }
    }
  };

  // Step backward
  const stepBackward = () => {
    if (animationStep > 0) {
      setAnimationStep(animationStep - 1);
      
      // Update statistics
      if (animationSteps[animationStep - 1]) {
        setComparisons(animationSteps[animationStep - 1].compareCount);
        setOperations(animationSteps[animationStep - 1].opCount);
        if (animationSteps[animationStep - 1].extraSpace !== undefined) {
          setExtraSpace(animationSteps[animationStep - 1].extraSpace);
        }
      }
    }
  };

  // Reset to initial state
  const resetState = () => {
    generateArray();
  };

  // Render array elements
  const renderArrayElements = () => {
    if (!animationSteps.length || !animationSteps[animationStep]) {
      return null;
    }
    
    const step = animationSteps[animationStep];
    const maxValue = Math.max(...array);
    
    return (
      <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'flex-end', height: '250px', marginTop: '20px' }}>
        {step.array.map((value, index) => {
          const height = (value / maxValue) * 200;
          
          let backgroundColor = 'rgba(255, 255, 255, 0.2)';
          let borderColor = 'rgba(255, 255, 255, 0.3)';
          let textColor = colors.text;
          
          // Merge Sort specific visualization
          if (activeAlgo === 'merge') {
            // Element is in the merge range
            if (step.mergeRange && index >= step.mergeRange[0] && index <= step.mergeRange[1]) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            // Element is being compared during merge
            if (step.mergePos === index) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
            
            // Element was just placed during merge
            if (step.mergePos === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            // The dividing point
            if (step.divideAt === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            // Element is merged
            if (step.merged && index >= step.mergeRange[0] && index <= step.mergeRange[1]) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
          }
          
          // Quick Sort specific visualization
          if (activeAlgo === 'quick') {
            // Element is in the sort range
            if (step.sortRange && index >= step.sortRange[0] && index <= step.sortRange[1]) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            // Element is the pivot
            if (step.pivot === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
              textColor = colors.background;
            }
            
            // Elements being compared
            if (step.comparing && step.comparing.includes(index)) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            // Elements being swapped
            if (step.swapping && step.swapping.includes(index)) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
            
            // Pivot placed in final position
            if (step.pivotPlaced && step.pivot === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
          }
          
          // Heap Sort specific visualization
          if (activeAlgo === 'heap') {
            // Element is in the heap
            if (step.heapRange !== null && index < step.heapRange) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            // Element is the heap root
            if (step.heapRoot === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            // Element is left or right child
            if ((step.leftChild === index || step.rightChild === index) && 
                step.heapifying) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            // Elements being compared
            if (step.comparing && step.comparing.includes(index)) {
              backgroundColor = colors.darkOrange;
              borderColor = colors.orange;
            }
            
            // Elements being swapped
            if (step.swapping && step.swapping.includes(index)) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
            
            // Element is extracted and placed in final position
            if (step.extracted === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            // Element is in final sorted position
            if (step.sorted && step.sorted[index]) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
          }
          
          // Introsort specific visualization
          if (activeAlgo === 'intro') {
            // Element is in the sort range
            if (step.sortRange && index >= step.sortRange[0] && index <= step.sortRange[1]) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            // Different phases have different colors
            if (step.phase === 'insertion' || step.phase === 'insertion-complete') {
              if (step.sortRange && index >= step.sortRange[0] && index <= step.sortRange[1]) {
                backgroundColor = colors.darkCyan;
                borderColor = colors.cyan;
              }
            } else if (step.phase.startsWith('heapsort')) {
              if (step.sortRange && index >= step.sortRange[0] && index <= step.sortRange[1]) {
                backgroundColor = colors.darkOrange;
                borderColor = colors.orange;
              }
            } else if (step.phase.startsWith('quicksort')) {
              if (step.sortRange && index >= step.sortRange[0] && index <= step.sortRange[1]) {
                backgroundColor = colors.darkPurple;
                borderColor = colors.purple;
              }
            }
            
            // Key element in insertion sort
            if (step.key === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            // Element was just placed
            if (step.keyPlaced === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            // Pivot element in quicksort
            if (step.pivot === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            // Elements being compared
            if (step.comparing && step.comparing.includes(index)) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            // Elements being swapped
            if (step.swapping && step.swapping.includes(index)) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
          }
          
          return (
            <div 
              key={index} 
              style={{
                height: `${height}px`,
                width: '20px',
                margin: '0 2px',
                backgroundColor,
                border: `1px solid ${borderColor}`,
                display: 'flex',
                justifyContent: 'center',
                alignItems: 'flex-end',
                padding: '2px',
                fontSize: '10px',
                color: textColor,
                position: 'relative',
                transition: 'height 0.3s ease, background-color 0.3s ease'
              }}
            >
              {arraySize <= 20 && <span>{value}</span>}
            </div>
          );
        })}
      </div>
    );
  };

  // Render auxiliary elements for merge sort
  const renderAuxiliaryElements = () => {
    if (activeAlgo !== 'merge' || !animationSteps.length || !animationSteps[animationStep]) {
      return null;
    }
    
    const step = animationSteps[animationStep];
    
    if ((!step.left || step.left.length === 0) && 
        (!step.right || step.right.length === 0) && 
        (!step.auxiliaryArray || step.auxiliaryArray.every(item => item === null))) {
      return null;
    }
    
    const maxValue = Math.max(...array);
    
    return (
      <div style={{ marginTop: '20px' }}>
        <div style={{ color: colors.cyan, marginBottom: '5px' }}>Auxiliary Arrays:</div>
        
        <div style={{ display: 'flex', flexDirection: 'column', gap: '10px' }}>
          {/* Left array */}
          {step.left && step.left.length > 0 && (
            <div>
              <div style={{ color: colors.pink, marginBottom: '2px' }}>Left:</div>
              <div style={{ display: 'flex' }}>
                {step.left.map((value, index) => {
                  const height = (value / maxValue) * 100;
                  const isSelected = step.leftPos === index;
                  
                  return (
                    <div 
                      key={index} 
                      style={{
                        height: `${height}px`,
                        width: '20px',
                        margin: '0 2px',
                        backgroundColor: isSelected ? colors.darkPink : colors.darkCyan,
                        border: `1px solid ${isSelected ? colors.pink : colors.cyan}`,
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'flex-end',
                        padding: '2px',
                        fontSize: '10px',
                        color: colors.text
                      }}
                    >
                      {value}
                    </div>
                  );
                })}
              </div>
            </div>
          )}
          
          {/* Right array */}
          {step.right && step.right.length > 0 && (
            <div>
              <div style={{ color: colors.green, marginBottom: '2px' }}>Right:</div>
              <div style={{ display: 'flex' }}>
                {step.right.map((value, index) => {
                  const height = (value / maxValue) * 100;
                  const isSelected = step.rightPos === index;
                  
                  return (
                    <div 
                      key={index} 
                      style={{
                        height: `${height}px`,
                        width: '20px',
                        margin: '0 2px',
                        backgroundColor: isSelected ? colors.darkPink : colors.darkGreen,
                        border: `1px solid ${isSelected ? colors.pink : colors.green}`,
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'flex-end',
                        padding: '2px',
                        fontSize: '10px',
                        color: colors.text
                      }}
                    >
                      {value}
                    </div>
                  );
                })}
              </div>
            </div>
          )}
          
          {/* Auxiliary array (merged result) */}
          {step.auxiliaryArray && step.auxiliaryArray.some(item => item !== null) && (
            <div>
              <div style={{ color: colors.yellow, marginBottom: '2px' }}>Merged:</div>
              <div style={{ display: 'flex' }}>
                {step.auxiliaryArray.map((value, index) => {
                  if (value === null) return null;
                  
                  const height = (value / maxValue) * 100;
                  
                  return (
                    <div 
                      key={index} 
                      style={{
                        height: `${height}px`,
                        width: '20px',
                        margin: '0 2px',
                        backgroundColor: colors.darkYellow,
                        border: `1px solid ${colors.yellow}`,
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'flex-end',
                        padding: '2px',
                        fontSize: '10px',
                        color: colors.text
                      }}
                    >
                      {value}
                    </div>
                  );
                })}
              </div>
            </div>
          )}
        </div>
      </div>
    );
  };

  // Algorithm info
  const algorithmInfo = {
    merge: {
      title: "Merge Sort",
      description: "A classic divide-and-conquer algorithm that divides the input array into smaller subarrays, sorts them, and then merges them back together.",
      timeComplexity: {
        best: "O(n log n)",
        average: "O(n log n)",
        worst: "O(n log n)"
      },
      spaceComplexity: "O(n)",
      stable: true,
      code: `void merge(std::vector<int>& arr, int left, int mid, int right) {
    // Calculate sizes of two subarrays to be merged
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    std::vector<int> leftArray(n1);
    std::vector<int> rightArray(n2);
    
    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArray[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArray[j] = arr[mid + 1 + j];
    
    // Merge the temporary arrays back into arr[left..right]
    int i = 0;  // Initial index of first subarray
    int j = 0;  // Initial index of second subarray
    int k = left;  // Initial index of merged array
    
    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            arr[k] = leftArray[i];
            i++;
        } else {
            arr[k] = rightArray[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements
    while (i < n1) {
        arr[k] = leftArray[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}`
    },
    quick: {
      title: "Quick Sort",
      description: "An efficient divide-and-conquer algorithm that works by selecting a 'pivot' element and partitioning the array around it.",
      timeComplexity: {
        best: "O(n log n)",
        average: "O(n log n)",
        worst: "O(n²)"
      },
      spaceComplexity: "O(log n)",
      stable: false,
      code: `int partition(std::vector<int>& arr, int low, int high) {
    // Select the rightmost element as pivot
    int pivot = arr[high];
    
    // Index of smaller element
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            // Increment index of smaller element
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    
    // Place the pivot in its correct position
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pivotIndex = partition(arr, low, high);
        
        // Recursively sort elements before and after the pivot
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}`
    },
    heap: {
      title: "Heap Sort",
      description: "A comparison-based sorting algorithm that uses a binary heap data structure to build a max-heap and then repeatedly extract the maximum element.",
      timeComplexity: {
        best: "O(n log n)",
        average: "O(n log n)",
        worst: "O(n log n)"
      },
      spaceComplexity: "O(1)",
      stable: false,
      code: `void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;        // Initialize largest as root
    int leftChild = 2 * i + 1;  // Left child
    int rightChild = 2 * i + 2; // Right child
    
    // If left child is larger than root
    if (leftChild < n && arr[leftChild] > arr[largest])
        largest = leftChild;
    
    // If right child is larger than largest so far
    if (rightChild < n && arr[rightChild] > arr[largest])
        largest = rightChild;
    
    // If largest is not root
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    
    // Build a max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        std::swap(arr[0], arr[i]);
        
        // Heapify the reduced heap
        heapify(arr, i, 0);
    }
}`
    },
    intro: {
      title: "Introsort",
      description: "A hybrid sorting algorithm that combines the strengths of Quick Sort, Heap Sort, and Insertion Sort to guarantee good performance in all cases.",
      timeComplexity: {
        best: "O(n log n)",
        average: "O(n log n)",
        worst: "O(n log n)"
      },
      spaceComplexity: "O(log n)",
      stable: false,
      code: `void introSort(std::vector<int>& arr) {
    int n = arr.size();
    int maxDepth = 2 * log2(n);
    introSortUtil(arr, 0, n - 1, maxDepth);
}

void introSortUtil(std::vector<int>& arr, int low, int high, int depthLimit) {
    // If partition size is small, use insertion sort
    if (high - low < 16) {
        insertionSort(arr, low, high);
        return;
    }
    
    // If depth limit is 0, switch to heap sort
    if (depthLimit == 0) {
        // Create a max heap for elements from low to high
        heapSortRange(arr, low, high);
        return;
    }
    
    // Otherwise, use quick sort
    int pivotIndex = partition(arr, low, high);
    introSortUtil(arr, low, pivotIndex - 1, depthLimit - 1);
    introSortUtil(arr, pivotIndex + 1, high, depthLimit - 1);
}`
    }
  };

  // Status message for current step
  const getStatusMessage = () => {
    if (!animationSteps.length || !animationSteps[animationStep]) {
      return "Initialize algorithm";
    }
    
    const step = animationSteps[animationStep];
    
    if (step.complete) {
      return "Sorting complete!";
    }
    
    if (activeAlgo === 'merge') {
      if (step.mergeRange && step.mergeRange.length === 2) {
        if (step.divideAt !== undefined) {
          return `Dividing array at index ${step.divideAt} (range: ${step.mergeRange[0]}-${step.mergeRange[1]})`;
        } else if (step.left && step.left.length > 0 && step.right && step.right.length > 0) {
          return `Merging subarrays (range: ${step.mergeRange[0]}-${step.mergeRange[1]})`;
        } else if (step.merged) {
          return `Subarray merged (range: ${step.mergeRange[0]}-${step.mergeRange[1]})`;
        }
      }
      return "Processing merge sort";
    }
    
    if (activeAlgo === 'quick') {
      if (step.sortRange && step.sortRange.length === 2) {
        if (step.pivot !== null) {
          if (step.pivotPlaced) {
            return `Pivot (${step.array[step.pivot]}) placed at index ${step.pivot}`;
          } else if (step.partitioning) {
            if (step.comparing) {
              return `Comparing element at index ${step.comparing[0]} with pivot value ${step.array[step.pivot]}`;
            } else if (step.swapping) {
              return `Swapping elements at indices ${step.swapping[0]} and ${step.swapping[1]}`;
            }
            return `Partitioning around pivot (index ${step.pivot}, value ${step.array[step.pivot]})`;
          }
        }
        return `Processing subarray (range: ${step.sortRange[0]}-${step.sortRange[1]})`;
      }
      return "Processing quick sort";
    }
    
    if (activeAlgo === 'heap') {
      if (step.buildingHeap) {
        return "Building max heap from the array";
      } else if (step.heapBuilt) {
        return "Max heap built, ready to extract elements";
      } else if (step.heapifying) {
        return `Heapifying subtree rooted at index ${step.heapRoot}`;
      } else if (step.extractingMax) {
        return `Extracting maximum element (moving from index 0 to ${step.extracted})`;
      } else if (step.heapified) {
        return `Heapified reduced heap (size: ${step.heapRange})`;
      }
      return "Processing heap sort";
    }
    
    if (activeAlgo === 'intro') {
      if (step.phase) {
        if (step.message) {
          return step.message;
        }
        
        switch (step.phase) {
          case 'start':
            return "Starting Introsort";
          case 'depth-calculation':
            return `Calculating max recursion depth: ${step.maxDepth}`;
          case 'insertion':
            return `Using insertion sort for small subarray (range: ${step.sortRange[0]}-${step.sortRange[1]})`;
          case 'insertion-complete':
            return `Insertion sort complete for range ${step.sortRange[0]}-${step.sortRange[1]}`;
          case 'heapsort':
          case 'heapsort-running':
            return `Using heap sort (depth limit reached) for range ${step.sortRange[0]}-${step.sortRange[1]}`;
          case 'heapsort-complete':
            return `Heap sort complete for range ${step.sortRange[0]}-${step.sortRange[1]}`;
          case 'quicksort':
            return `Using quick sort for range ${step.sortRange[0]}-${step.sortRange[1]} (depth: ${step.depthRemaining})`;
          case 'quicksort-pivot':
            return `Selected pivot at index ${step.pivot} (value: ${step.pivotValue})`;
          case 'quicksort-partition':
            if (step.comparing) {
              return `Comparing element at index ${step.comparing[0]} with pivot value ${step.pivotValue}`;
            } else if (step.swapping) {
              return `Swapping elements at indices ${step.swapping[0]} and ${step.swapping[1]}`;
            }
            return `Partitioning around pivot value ${step.pivotValue}`;
          case 'quicksort-pivot-placed':
            return `Pivot placed at index ${step.pivot}`;
          case 'complete':
            return "Introsort complete";
          default:
            return `Introsort phase: ${step.phase}`;
        }
      }
      return "Processing introsort";
    }
    
    return "Processing...";
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Efficient Sorting Algorithms Visualization</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Interactive Algorithm Visualization</h2>
        
        <div style={tabsContainerStyle}>
          <div style={tabStyle(activeAlgo === 'merge')} onClick={() => setActiveAlgo('merge')}>
            Merge Sort
          </div>
          <div style={tabStyle(activeAlgo === 'quick')} onClick={() => setActiveAlgo('quick')}>
            Quick Sort
          </div>
          <div style={tabStyle(activeAlgo === 'heap')} onClick={() => setActiveAlgo('heap')}>
            Heap Sort
          </div>
          <div style={tabStyle(activeAlgo === 'intro')} onClick={() => setActiveAlgo('intro')}>
            Introsort
          </div>
        </div>
        
        <div style={boxStyle}>
          <h3 style={{color: colors.yellow}}>{algorithmInfo[activeAlgo].title}</h3>
          <p>{algorithmInfo[activeAlgo].description}</p>
          
          <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', marginBottom: '15px'}}>
            <div>
              <strong style={{color: colors.cyan}}>Time Complexity:</strong><br />
              Best: {algorithmInfo[activeAlgo].timeComplexity.best}<br />
              Average: {algorithmInfo[activeAlgo].timeComplexity.average}<br />
              Worst: {algorithmInfo[activeAlgo].timeComplexity.worst}
            </div>
            <div>
              <strong style={{color: colors.cyan}}>Space Complexity:</strong><br />
              {algorithmInfo[activeAlgo].spaceComplexity}
            </div>
            <div>
              <strong style={{color: colors.cyan}}>Stability:</strong><br />
              {algorithmInfo[activeAlgo].stable ? "Stable" : "Not Stable"}
            </div>
          </div>
          
          <div style={codeBlockStyle}>
            <code style={{color: colors.text, whiteSpace: 'pre-wrap'}}>
              {algorithmInfo[activeAlgo].code.split('\n').map((line, i) => (
                <div key={i} style={{
                  color: line.includes('swap') ? colors.pink : 
                         line.includes('if') || line.includes('while') ? colors.yellow :
                         line.includes('for') || line.includes('void') || line.includes('int') ? colors.cyan : colors.text
                }}>
                  {line}
                </div>
              ))}
            </code>
          </div>
          
          <div style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'space-between', alignItems: 'center', marginTop: '20px', marginBottom: '20px'}}>
            <div>
              <label>
                Array Size: 
                <select
                  style={selectStyle}
                  value={arraySize}
                  onChange={(e) => setArraySize(parseInt(e.target.value))}
                >
                  <option value="8">8</option>
                  <option value="15">15</option>
                  <option value="20">20</option>
                  <option value="30">30</option>
                  <option value="50">50</option>
                </select>
              </label>
            </div>
            
            <div>
              <label>
                Array Type: 
                <select
                  style={selectStyle}
                  value={arrayType}
                  onChange={(e) => setArrayType(e.target.value)}
                >
                  <option value="random">Random</option>
                  <option value="nearly-sorted">Nearly Sorted</option>
                  <option value="reversed">Reversed</option>
                  <option value="duplicates">Many Duplicates</option>
                </select>
              </label>
            </div>
            
            <div>
              <label>
                Speed: 
                <select
                  style={selectStyle}
                  value={animationSpeed}
                  onChange={(e) => setAnimationSpeed(parseInt(e.target.value))}
                >
                  <option value="1000">Slow</option>
                  <option value="500">Medium</option>
                  <option value="200">Fast</option>
                  <option value="50">Very Fast</option>
                </select>
              </label>
            </div>
          </div>
          
          <div style={{display: 'flex', justifyContent: 'center', flexWrap: 'wrap', gap: '10px', marginBottom: '20px'}}>
            <button style={buttonStyle} onClick={startAnimation}>
              {animationPlaying ? 'Restart' : 'Start Animation'}
            </button>
            <button style={buttonStyle} onClick={pauseAnimation}>
              Pause
            </button>
            <button style={buttonStyle} onClick={stepBackward}>
              ← Step Back
            </button>
            <button style={buttonStyle} onClick={stepForward}>
              Step Forward →
            </button>
            <button style={buttonStyle} onClick={resetState}>
              Generate New Array
            </button>
          </div>
          
          <div style={{color: colors.yellow, textAlign: 'center', marginBottom: '10px', height: '20px'}}>
            {getStatusMessage()}
          </div>
          
          {renderArrayElements()}
          
          {renderAuxiliaryElements()}
          
          <div style={statsContainerStyle}>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.yellow}}>{animationStep}</div>
              <div>Step</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.cyan}}>{comparisons}</div>
              <div>Comparisons</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.pink}}>{operations}</div>
              <div>Operations</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.green}}>{extraSpace}</div>
              <div>Extra Space</div>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Algorithm Comparison</h2>
        
        <div style={{backgroundColor: 'rgba(0, 0, 0, 0.3)', borderRadius: '8px', padding: '15px', marginBottom: '20px', border: `1px solid ${colors.yellow}`}}>
          <table style={{width: '100%', borderCollapse: 'collapse', color: colors.text}}>
            <thead>
              <tr>
                <th style={{padding: '8px', textAlign: 'left', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Algorithm</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Best Case</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Average Case</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Worst Case</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Space</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>Stable</th>
                <th style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.yellow}`, color: colors.yellow}}>In-Place</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td style={{padding: '8px', borderBottom: `1px solid ${colors.darkCyan}`, color: colors.cyan}}>Merge Sort</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>O(n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>Yes</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkCyan}`}}>No</td>
              </tr>
              <tr>
                <td style={{padding: '8px', borderBottom: `1px solid ${colors.darkPink}`, color: colors.pink}}>Quick Sort</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>O(n²)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>O(log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>No</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkPink}`}}>Yes</td>
              </tr>
              <tr>
                <td style={{padding: '8px', borderBottom: `1px solid ${colors.darkGreen}`, color: colors.green}}>Heap Sort</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>O(1)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>No</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkGreen}`}}>Yes</td>
              </tr>
              <tr>
                <td style={{padding: '8px', borderBottom: `1px solid ${colors.darkOrange}`, color: colors.orange}}>Introsort</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>O(log n)</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>No</td>
                <td style={{padding: '8px', textAlign: 'center', borderBottom: `1px solid ${colors.darkOrange}`}}>Mostly</td>
              </tr>
              <tr>
                <td style={{padding: '8px', color: colors.purple}}>Tim Sort</td>
                <td style={{padding: '8px', textAlign: 'center'}}>O(n)</td>
                <td style={{padding: '8px', textAlign: 'center'}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center'}}>O(n log n)</td>
                <td style={{padding: '8px', textAlign: 'center'}}>O(n)</td>
                <td style={{padding: '8px', textAlign: 'center'}}>Yes</td>
                <td style={{padding: '8px', textAlign: 'center'}}>No</td>
              </tr>
            </tbody>
          </table>
        </div>
        
        <div style={{display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(270px, 1fr))', gap: '15px'}}>
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.cyan}`
          }}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>When to Use Merge Sort</h3>
            <ul style={{paddingLeft: '20px', marginTop: '5px'}}>
              <li>When stability is required (preserving order of equal elements)</li>
              <li>When predictable O(n log n) performance is needed regardless of input data</li>
              <li>When sorting linked lists (works great with sequential access)</li>
              <li>When working with external sorting (large datasets that don't fit in memory)</li>
            </ul>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 0, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.pink}`
          }}>
            <h3 style={{color: colors.pink, marginTop: 0}}>When to Use Quick Sort</h3>
            <ul style={{paddingLeft: '20px', marginTop: '5px'}}>
              <li>When average performance matters more than worst-case scenarios</li>
              <li>When working with arrays (great cache locality)</li>
              <li>When memory usage is a concern (uses less than merge sort)</li>
              <li>When implemented well, it's typically the fastest for large random datasets</li>
            </ul>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.green}`
          }}>
            <h3 style={{color: colors.green, marginTop: 0}}>When to Use Heap Sort</h3>
            <ul style={{paddingLeft: '20px', marginTop: '5px'}}>
              <li>When memory usage is highly constrained (uses O(1) extra space)</li>
              <li>When guaranteed O(n log n) performance is needed</li>
              <li>When finding the k largest/smallest elements</li>
              <li>When stability is not required but constant extra space is important</li>
            </ul>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 136, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.orange}`
          }}>
            <h3 style={{color: colors.orange, marginTop: 0}}>When to Use Introsort/Tim Sort</h3>
            <ul style={{paddingLeft: '20px', marginTop: '5px'}}>
              <li>Introsort: For general-purpose sorting in production code</li>
              <li>Introsort: When you want quick sort speed with heap sort guarantees</li>
              <li>Tim Sort: When dealing with real-world data that may be partially sorted</li>
              <li>Tim Sort: When stability is required for complex datasets</li>
            </ul>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 18: Efficient Sorting Algorithms
      </div>
    </div>
  );
};

export default EfficientSortingVisualization;
