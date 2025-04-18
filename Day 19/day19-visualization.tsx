import React, { useState, useEffect } from 'react';

const DivideAndConquerVisualization = () => {
  // State to track which algorithm is being demonstrated
  const [activeAlgo, setActiveAlgo] = useState('binary-search');
  const [array, setArray] = useState([]);
  const [arraySize, setArraySize] = useState(16);
  const [target, setTarget] = useState(42);
  const [animationStep, setAnimationStep] = useState(0);
  const [animationSteps, setAnimationSteps] = useState([]);
  const [animationPlaying, setAnimationPlaying] = useState(false);
  const [animationSpeed, setAnimationSpeed] = useState(1000);

  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    orange: '#FF8800',
    purple: '#BB00FF',
    text: '#FFFFFF',
    darkCyan: 'rgba(0, 255, 255, 0.15)',
    darkPink: 'rgba(255, 0, 255, 0.15)',
    darkYellow: 'rgba(255, 255, 0, 0.15)',
    darkGreen: 'rgba(0, 255, 0, 0.15)',
    darkOrange: 'rgba(255, 136, 0, 0.15)',
    darkPurple: 'rgba(187, 0, 255, 0.15)',
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

  const inputStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    border: `1px solid ${colors.yellow}`,
    color: colors.yellow,
    padding: '8px',
    borderRadius: '4px',
    margin: '0 10px',
    width: '60px',
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

  // Generate a sorted array
  const generateSortedArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(i * 10 + Math.floor(Math.random() * 5)); // Some randomness but still sorted
    }
    return arr;
  };

  // Generate a random array
  const generateRandomArray = (size) => {
    const arr = [];
    for (let i = 0; i < size; i++) {
      arr.push(Math.floor(Math.random() * 100) + 1);
    }
    return arr.sort((a, b) => a - b); // Sort for binary search
  };

  // Generate the initial array
  const generateArray = () => {
    let newArray = activeAlgo === 'binary-search' ? 
      generateSortedArray(arraySize) : 
      generateRandomArray(arraySize);
    
    setArray(newArray);
    
    // Set a target value that exists in the array for binary search
    if (activeAlgo === 'binary-search') {
      const randomIndex = Math.floor(Math.random() * newArray.length);
      setTarget(newArray[randomIndex]);
    }
    
    generateAnimationSteps(newArray);
  };

  // Reset animation state
  const resetAnimation = () => {
    setAnimationStep(0);
    setAnimationPlaying(false);
  };

  // Generate binary search animation steps
  const generateBinarySearchSteps = (arr) => {
    const steps = [];
    let left = 0;
    let right = arr.length - 1;
    
    // Add initial state
    steps.push({
      array: [...arr],
      left,
      right,
      mid: null,
      found: false,
      message: "Starting binary search"
    });
    
    while (left <= right) {
      const mid = Math.floor(left + (right - left) / 2);
      
      // Add step showing mid calculation
      steps.push({
        array: [...arr],
        left,
        right,
        mid,
        comparing: true,
        message: `Comparing array[${mid}] = ${arr[mid]} with target ${target}`
      });
      
      if (arr[mid] === target) {
        // Target found
        steps.push({
          array: [...arr],
          left,
          right,
          mid,
          found: true,
          message: `Found target ${target} at index ${mid}`
        });
        break;
      } else if (arr[mid] < target) {
        // Target is in the right half
        left = mid + 1;
        steps.push({
          array: [...arr],
          left,
          right,
          mid,
          direction: 'right',
          message: `${arr[mid]} < ${target}, search right half (${left} to ${right})`
        });
      } else {
        // Target is in the left half
        right = mid - 1;
        steps.push({
          array: [...arr],
          left,
          right,
          mid,
          direction: 'left',
          message: `${arr[mid]} > ${target}, search left half (${left} to ${right})`
        });
      }
    }
    
    // If target not found
    if (steps[steps.length - 1].found !== true) {
      steps.push({
        array: [...arr],
        left,
        right,
        mid: null,
        notFound: true,
        message: `Target ${target} not found in the array`
      });
    }
    
    return steps;
  };

  // Generate merge sort animation steps
  const generateMergeSortSteps = (arr) => {
    const steps = [];
    const auxArray = [...arr];
    
    // Add initial state
    steps.push({
      array: [...arr],
      aux: [...auxArray],
      dividing: false,
      merging: false,
      ranges: [],
      message: "Starting merge sort"
    });
    
    // Function to generate steps for merge sort
    const mergeSortHelper = (mainArray, aux, left, right) => {
      if (left === right) return;
      
      const mid = Math.floor(left + (right - left) / 2);
      
      // Add step showing division
      steps.push({
        array: [...mainArray],
        aux: [...aux],
        dividing: true,
        ranges: [[left, right]],
        mid,
        message: `Dividing array from index ${left} to ${right} at midpoint ${mid}`
      });
      
      // Sort left half
      mergeSortHelper(aux, mainArray, left, mid);
      
      // Sort right half
      mergeSortHelper(aux, mainArray, mid + 1, right);
      
      // Add step before merging
      steps.push({
        array: [...mainArray],
        aux: [...aux],
        merging: true,
        ranges: [[left, mid], [mid + 1, right]],
        message: `Merging subarrays from ${left} to ${mid} and ${mid + 1} to ${right}`
      });
      
      // Merge the two halves
      let i = left;
      let j = mid + 1;
      let k = left;
      
      while (i <= mid && j <= right) {
        // Add comparison step
        steps.push({
          array: [...mainArray],
          aux: [...aux],
          merging: true,
          comparing: [i, j],
          ranges: [[left, mid], [mid + 1, right]],
          k,
          message: `Comparing ${aux[i]} and ${aux[j]}`
        });
        
        if (aux[i] <= aux[j]) {
          mainArray[k] = aux[i];
          steps.push({
            array: [...mainArray],
            aux: [...aux],
            merging: true,
            ranges: [[left, mid], [mid + 1, right]],
            selected: i,
            k,
            message: `Placing ${aux[i]} at position ${k}`
          });
          i++;
        } else {
          mainArray[k] = aux[j];
          steps.push({
            array: [...mainArray],
            aux: [...aux],
            merging: true,
            ranges: [[left, mid], [mid + 1, right]],
            selected: j,
            k,
            message: `Placing ${aux[j]} at position ${k}`
          });
          j++;
        }
        k++;
      }
      
      // Copy remaining elements from left subarray
      while (i <= mid) {
        mainArray[k] = aux[i];
        steps.push({
          array: [...mainArray],
          aux: [...aux],
          merging: true,
          ranges: [[left, mid], [mid + 1, right]],
          selected: i,
          k,
          message: `Copying remaining element ${aux[i]} from left subarray to position ${k}`
        });
        i++;
        k++;
      }
      
      // Copy remaining elements from right subarray
      while (j <= right) {
        mainArray[k] = aux[j];
        steps.push({
          array: [...mainArray],
          aux: [...aux],
          merging: true,
          ranges: [[left, mid], [mid + 1, right]],
          selected: j,
          k,
          message: `Copying remaining element ${aux[j]} from right subarray to position ${k}`
        });
        j++;
        k++;
      }
      
      // Add step after merging
      steps.push({
        array: [...mainArray],
        aux: [...aux],
        merged: true,
        ranges: [[left, right]],
        message: `Merged subarray from ${left} to ${right}`
      });
    };
    
    // Start the merge sort
    mergeSortHelper(arr, auxArray, 0, arr.length - 1);
    
    // Add final state
    steps.push({
      array: [...arr],
      aux: [...auxArray],
      complete: true,
      message: "Merge sort complete"
    });
    
    return steps;
  };

  // Generate quick sort animation steps
  const generateQuickSortSteps = (arr) => {
    const steps = [];
    
    // Add initial state
    steps.push({
      array: [...arr],
      partitioning: false,
      ranges: [],
      message: "Starting quick sort"
    });
    
    // Function to simulate quick sort partitioning
    const partition = (mainArray, low, high) => {
      // Choose the rightmost element as pivot
      const pivot = mainArray[high];
      
      // Add step showing pivot selection
      steps.push({
        array: [...mainArray],
        partitioning: true,
        ranges: [[low, high]],
        pivot: high,
        message: `Selected pivot: ${pivot} at index ${high}`
      });
      
      let i = low - 1;
      
      for (let j = low; j < high; j++) {
        // Add comparison step
        steps.push({
          array: [...mainArray],
          partitioning: true,
          ranges: [[low, high]],
          pivot: high,
          comparing: [j, high],
          partitionIndex: i,
          message: `Comparing ${mainArray[j]} with pivot ${pivot}`
        });
        
        if (mainArray[j] < pivot) {
          i++;
          
          // Add swap step
          steps.push({
            array: [...mainArray],
            partitioning: true,
            ranges: [[low, high]],
            pivot: high,
            swapping: [i, j],
            partitionIndex: i,
            message: i !== j ? `Swapping ${mainArray[i]} and ${mainArray[j]}` : `Element ${mainArray[j]} is already in correct position`
          });
          
          // Perform the swap
          [mainArray[i], mainArray[j]] = [mainArray[j], mainArray[i]];
          
          // Add post-swap step
          steps.push({
            array: [...mainArray],
            partitioning: true,
            ranges: [[low, high]],
            pivot: high,
            swapped: [i, j],
            partitionIndex: i,
            message: `Updated array after swap`
          });
        }
      }
      
      // Swap pivot with the element at partition index + 1
      const pivotIndex = i + 1;
      
      steps.push({
        array: [...mainArray],
        partitioning: true,
        ranges: [[low, high]],
        pivot: high,
        swapping: [pivotIndex, high],
        message: `Placing pivot at its correct position: swapping ${mainArray[pivotIndex]} and ${pivot}`
      });
      
      [mainArray[pivotIndex], mainArray[high]] = [mainArray[high], mainArray[pivotIndex]];
      
      steps.push({
        array: [...mainArray],
        partitioning: true,
        ranges: [[low, high]],
        pivot: pivotIndex,
        pivotPlaced: true,
        message: `Pivot ${pivot} placed at index ${pivotIndex}`
      });
      
      return pivotIndex;
    };
    
    // Function to simulate quick sort
    const quickSortHelper = (mainArray, low, high) => {
      if (low < high) {
        // Add step before partitioning
        steps.push({
          array: [...mainArray],
          sorting: true,
          ranges: [[low, high]],
          message: `Sorting subarray from index ${low} to ${high}`
        });
        
        // Partition the array and get the pivot index
        const pivotIndex = partition(mainArray, low, high);
        
        // Add step after partitioning
        steps.push({
          array: [...mainArray],
          partitioned: true,
          ranges: [[low, pivotIndex - 1], [pivotIndex], [pivotIndex + 1, high]],
          pivot: pivotIndex,
          message: `Array partitioned around pivot at index ${pivotIndex}`
        });
        
        // Recursively sort the subarrays
        quickSortHelper(mainArray, low, pivotIndex - 1);
        quickSortHelper(mainArray, pivotIndex + 1, high);
      } else if (low === high) {
        // Single element array is already sorted
        steps.push({
          array: [...mainArray],
          sorted: true,
          ranges: [[low, high]],
          message: `Subarray with single element at index ${low} is already sorted`
        });
      }
    };
    
    // Create a copy of the array to sort
    const arrayCopy = [...arr];
    
    // Start the quick sort
    quickSortHelper(arrayCopy, 0, arrayCopy.length - 1);
    
    // Add final state
    steps.push({
      array: [...arrayCopy],
      complete: true,
      message: "Quick sort complete"
    });
    
    return steps;
  };

  // Generate max subarray animation steps
  const generateMaxSubarraySteps = (arr) => {
    const steps = [];
    
    // Add initial state
    steps.push({
      array: [...arr],
      dividing: false,
      ranges: [],
      message: "Starting maximum subarray problem using divide and conquer"
    });
    
    // Function to find max crossing subarray
    const findMaxCrossingSubarray = (mainArray, low, mid, high) => {
      let leftSum = -Infinity;
      let sum = 0;
      let maxLeft = mid;
      
      // Add step for finding left sum
      steps.push({
        array: [...mainArray],
        finding: 'leftSum',
        ranges: [[low, mid]],
        message: `Finding maximum subarray crossing midpoint - scanning left half from ${mid} down to ${low}`
      });
      
      // Find max sum in left half
      for (let i = mid; i >= low; i--) {
        sum += mainArray[i];
        
        steps.push({
          array: [...mainArray],
          finding: 'leftSum',
          ranges: [[low, mid]],
          currentSum: sum,
          currentIndex: i,
          message: `Current sum: ${sum}`
        });
        
        if (sum > leftSum) {
          leftSum = sum;
          maxLeft = i;
          
          steps.push({
            array: [...mainArray],
            finding: 'leftSum',
            ranges: [[low, mid]],
            maxSum: leftSum,
            maxRange: [maxLeft, mid],
            message: `New maximum left sum: ${leftSum} from index ${maxLeft} to ${mid}`
          });
        }
      }
      
      let rightSum = -Infinity;
      sum = 0;
      let maxRight = mid + 1;
      
      // Add step for finding right sum
      steps.push({
        array: [...mainArray],
        finding: 'rightSum',
        ranges: [[mid + 1, high]],
        message: `Scanning right half from ${mid + 1} up to ${high}`
      });
      
      // Find max sum in right half
      for (let i = mid + 1; i <= high; i++) {
        sum += mainArray[i];
        
        steps.push({
          array: [...mainArray],
          finding: 'rightSum',
          ranges: [[mid + 1, high]],
          currentSum: sum,
          currentIndex: i,
          message: `Current sum: ${sum}`
        });
        
        if (sum > rightSum) {
          rightSum = sum;
          maxRight = i;
          
          steps.push({
            array: [...mainArray],
            finding: 'rightSum',
            ranges: [[mid + 1, high]],
            maxSum: rightSum,
            maxRange: [mid + 1, maxRight],
            message: `New maximum right sum: ${rightSum} from index ${mid + 1} to ${maxRight}`
          });
        }
      }
      
      // Add step for combining results
      steps.push({
        array: [...mainArray],
        finding: 'cross',
        leftMaxSum: leftSum,
        rightMaxSum: rightSum,
        crossSum: leftSum + rightSum,
        maxRange: [maxLeft, maxRight],
        message: `Cross sum: ${leftSum} + ${rightSum} = ${leftSum + rightSum} from index ${maxLeft} to ${maxRight}`
      });
      
      return { maxLeft, maxRight, sum: leftSum + rightSum };
    };
    
    // Function to find maximum subarray
    const findMaxSubarray = (mainArray, low, high) => {
      // Base case: single element
      if (low === high) {
        steps.push({
          array: [...mainArray],
          base: true,
          ranges: [[low, high]],
          maxSum: mainArray[low],
          maxRange: [low, high],
          message: `Base case: single element ${mainArray[low]} at index ${low}`
        });
        
        return { maxLeft: low, maxRight: high, sum: mainArray[low] };
      }
      
      const mid = Math.floor(low + (high - low) / 2);
      
      // Add step for division
      steps.push({
        array: [...mainArray],
        dividing: true,
        ranges: [[low, high]],
        mid,
        message: `Dividing array from index ${low} to ${high} at midpoint ${mid}`
      });
      
      // Find max subarray in left half
      const leftResult = findMaxSubarray(mainArray, low, mid);
      
      // Add step for left result
      steps.push({
        array: [...mainArray],
        result: 'left',
        ranges: [[low, mid]],
        maxSum: leftResult.sum,
        maxRange: [leftResult.maxLeft, leftResult.maxRight],
        message: `Maximum subarray in left half: sum = ${leftResult.sum} from index ${leftResult.maxLeft} to ${leftResult.maxRight}`
      });
      
      // Find max subarray in right half
      const rightResult = findMaxSubarray(mainArray, mid + 1, high);
      
      // Add step for right result
      steps.push({
        array: [...mainArray],
        result: 'right',
        ranges: [[mid + 1, high]],
        maxSum: rightResult.sum,
        maxRange: [rightResult.maxLeft, rightResult.maxRight],
        message: `Maximum subarray in right half: sum = ${rightResult.sum} from index ${rightResult.maxLeft} to ${rightResult.maxRight}`
      });
      
      // Find max subarray crossing the midpoint
      const crossResult = findMaxCrossingSubarray(mainArray, low, mid, high);
      
      // Add step for comparing results
      steps.push({
        array: [...mainArray],
        comparing: true,
        leftResult,
        rightResult,
        crossResult,
        message: `Comparing left (${leftResult.sum}), right (${rightResult.sum}), and cross (${crossResult.sum}) sums`
      });
      
      // Return the maximum of the three
      if (leftResult.sum >= rightResult.sum && leftResult.sum >= crossResult.sum) {
        steps.push({
          array: [...mainArray],
          result: 'final',
          ranges: [[low, high]],
          maxSum: leftResult.sum,
          maxRange: [leftResult.maxLeft, leftResult.maxRight],
          winner: 'left',
          message: `Left subarray has maximum sum: ${leftResult.sum}`
        });
        
        return leftResult;
      } else if (rightResult.sum >= leftResult.sum && rightResult.sum >= crossResult.sum) {
        steps.push({
          array: [...mainArray],
          result: 'final',
          ranges: [[low, high]],
          maxSum: rightResult.sum,
          maxRange: [rightResult.maxLeft, rightResult.maxRight],
          winner: 'right',
          message: `Right subarray has maximum sum: ${rightResult.sum}`
        });
        
        return rightResult;
      } else {
        steps.push({
          array: [...mainArray],
          result: 'final',
          ranges: [[low, high]],
          maxSum: crossResult.sum,
          maxRange: [crossResult.maxLeft, crossResult.maxRight],
          winner: 'cross',
          message: `Crossing subarray has maximum sum: ${crossResult.sum}`
        });
        
        return crossResult;
      }
    };
    
    // Create a copy of the array
    const arrayCopy = [...arr];
    
    // Start the algorithm
    const result = findMaxSubarray(arrayCopy, 0, arrayCopy.length - 1);
    
    // Add final state
    steps.push({
      array: [...arrayCopy],
      complete: true,
      maxSum: result.sum,
      maxRange: [result.maxLeft, result.maxRight],
      message: `Maximum subarray found: sum = ${result.sum} from index ${result.maxLeft} to ${result.maxRight}`
    });
    
    return steps;
  };

  // Generate steps for the selected algorithm
  const generateAnimationSteps = (arr) => {
    let steps;
    
    switch (activeAlgo) {
      case 'binary-search':
        steps = generateBinarySearchSteps(arr);
        break;
      case 'merge-sort':
        steps = generateMergeSortSteps(arr);
        break;
      case 'quick-sort':
        steps = generateQuickSortSteps(arr);
        break;
      case 'max-subarray':
        steps = generateMaxSubarraySteps(arr);
        break;
      default:
        steps = generateBinarySearchSteps(arr);
    }
    
    setAnimationSteps(steps);
  };

  // Initialize on component mount
  useEffect(() => {
    generateArray();
  }, []);

  // Update when algorithm or array size changes
  useEffect(() => {
    generateArray();
  }, [activeAlgo, arraySize]);

  // Animation playback
  useEffect(() => {
    let timer;
    
    if (animationPlaying && animationStep < animationSteps.length - 1) {
      timer = setTimeout(() => {
        setAnimationStep(animationStep + 1);
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
    }
  };

  // Step backward
  const stepBackward = () => {
    if (animationStep > 0) {
      setAnimationStep(animationStep - 1);
    }
  };

  // Reset to initial state
  const resetState = () => {
    generateArray();
  };

  // Change target for binary search
  const handleTargetChange = (e) => {
    setTarget(parseInt(e.target.value));
    generateAnimationSteps(array);
  };

  // Render array elements
  const renderArrayElements = () => {
    if (!animationSteps.length || !animationSteps[animationStep]) {
      return null;
    }
    
    const step = animationSteps[animationStep];
    const maxValue = Math.max(...array.map(val => Math.abs(val)));
    
    return (
      <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'flex-end', height: '250px', marginTop: '20px', flexWrap: 'wrap' }}>
        {step.array.map((value, index) => {
          const height = Math.max(30, (Math.abs(value) / maxValue) * 200);
          
          let backgroundColor = 'rgba(255, 255, 255, 0.2)';
          let borderColor = 'rgba(255, 255, 255, 0.3)';
          let textColor = colors.text;
          
          // Binary Search visualization
          if (activeAlgo === 'binary-search') {
            if (index < step.left || index > step.right) {
              backgroundColor = 'rgba(100, 100, 100, 0.2)';
              borderColor = 'rgba(100, 100, 100, 0.3)';
              textColor = 'rgba(255, 255, 255, 0.5)';
            } else if (step.mid === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
              textColor = colors.background;
            } else if (step.found && step.mid === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
              textColor = colors.background;
            } else if (step.comparing && step.mid === index) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
              textColor = colors.background;
            } else if (step.direction === 'left' && index > step.mid && index <= step.right) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
              textColor = 'rgba(255, 255, 255, 0.5)';
            } else if (step.direction === 'right' && index < step.mid && index >= step.left) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
              textColor = 'rgba(255, 255, 255, 0.5)';
            }
          }
          
          // Merge Sort visualization
          if (activeAlgo === 'merge-sort') {
            if (step.ranges && step.ranges.some(range => index >= range[0] && index <= range[1])) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            if (step.comparing && step.comparing.includes(index)) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            if (step.selected === index) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            if (step.k === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            if (step.merged && step.ranges && step.ranges.some(range => index >= range[0] && index <= range[1])) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
          }
          
          // Quick Sort visualization
          if (activeAlgo === 'quick-sort') {
            if (step.ranges && step.ranges.some(range => Array.isArray(range) && index >= range[0] && index <= range[1])) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            if (step.pivot === index) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            if (step.comparing && step.comparing.includes(index)) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            if (step.swapping && step.swapping.includes(index)) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
            
            if (step.swapped && step.swapped.includes(index)) {
              backgroundColor = colors.darkOrange;
              borderColor = colors.orange;
            }
            
            if (step.pivotPlaced && step.pivot === index) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            if (step.sorted && step.ranges && step.ranges.includes(index)) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
          }
          
          // Max Subarray visualization
          if (activeAlgo === 'max-subarray') {
            if (step.ranges && step.ranges.some(range => index >= range[0] && index <= range[1])) {
              backgroundColor = colors.darkPurple;
              borderColor = colors.purple;
            }
            
            if (step.maxRange && index >= step.maxRange[0] && index <= step.maxRange[1]) {
              backgroundColor = colors.darkGreen;
              borderColor = colors.green;
            }
            
            if (step.currentIndex === index) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            if (step.finding === 'leftSum' && index <= step.mid && index >= step.currentIndex) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            if (step.finding === 'rightSum' && index >= step.mid + 1 && index <= step.currentIndex) {
              backgroundColor = colors.darkOrange;
              borderColor = colors.orange;
            }
            
            if (step.winner === 'left' && step.maxRange && index >= step.maxRange[0] && index <= step.maxRange[1]) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
            }
            
            if (step.winner === 'right' && step.maxRange && index >= step.maxRange[0] && index <= step.maxRange[1]) {
              backgroundColor = colors.darkCyan;
              borderColor = colors.cyan;
            }
            
            if (step.winner === 'cross' && step.maxRange && index >= step.maxRange[0] && index <= step.maxRange[1]) {
              backgroundColor = colors.darkOrange;
              borderColor = colors.orange;
            }
          }
          
          // Common states for all algorithms
          if (step.complete && activeAlgo !== 'binary-search') {
            backgroundColor = 'rgba(0, 255, 0, 0.1)';
            borderColor = 'rgba(0, 255, 0, 0.3)';
          }
          
          return (
            <div 
              key={index} 
              style={{
                height: `${height}px`,
                width: '30px',
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
              {arraySize <= 30 && <span>{value}</span>}
            </div>
          );
        })}
      </div>
    );
  };

  // Render auxiliary arrays for merge sort
  const renderAuxiliaryArrays = () => {
    if (activeAlgo !== 'merge-sort' || !animationSteps.length || !animationSteps[animationStep]) {
      return null;
    }
    
    const step = animationSteps[animationStep];
    
    if (!step.aux) {
      return null;
    }
    
    const maxValue = Math.max(...array.map(val => Math.abs(val)));
    
    return (
      <div style={{ marginTop: '20px' }}>
        <div style={{ color: colors.cyan, marginBottom: '5px' }}>Auxiliary Array:</div>
        <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'flex-end', height: '100px' }}>
          {step.aux.map((value, index) => {
            const height = Math.max(30, (Math.abs(value) / maxValue) * 80);
            
            let backgroundColor = 'rgba(255, 255, 255, 0.1)';
            let borderColor = 'rgba(255, 255, 255, 0.2)';
            
            if (step.comparing && (step.comparing[0] === index || step.comparing[1] === index)) {
              backgroundColor = colors.darkYellow;
              borderColor = colors.yellow;
            }
            
            if (step.selected === index) {
              backgroundColor = colors.darkPink;
              borderColor = colors.pink;
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
                  color: colors.text
                }}
              >
                {arraySize <= 30 && <span>{value}</span>}
              </div>
            );
          })}
        </div>
      </div>
    );
  };

  // Algorithm info
  const algorithmInfo = {
    'binary-search': {
      title: "Binary Search",
      description: "A classic divide and conquer algorithm that searches for a target value in a sorted array by repeatedly dividing the search space in half.",
      complexity: {
        time: "O(log n)",
        space: "O(log n) (recursive), O(1) (iterative)"
      },
      pseudocode: `function binarySearch(arr, target, left, right):
    if left > right:
        return -1  // not found
        
    mid = left + (right - left) / 2
    
    if arr[mid] == target:
        return mid  // found
    else if arr[mid] > target:
        return binarySearch(arr, target, left, mid - 1)  // search left half
    else:
        return binarySearch(arr, target, mid + 1, right)  // search right half`,
      applications: [
        "Database indexing and searching",
        "Dictionary lookups",
        "Finding elements in sorted arrays",
        "Component in other algorithms like binary search trees"
      ]
    },
    'merge-sort': {
      title: "Merge Sort",
      description: "A stable, comparison-based sorting algorithm that divides the input array into smaller subarrays, sorts them, and then merges them back together.",
      complexity: {
        time: "O(n log n) in all cases",
        space: "O(n) extra space"
      },
      pseudocode: `function mergeSort(arr, left, right):
    if left < right:
        mid = left + (right - left) / 2
        
        // Sort first and second halves
        mergeSort(arr, left, mid)
        mergeSort(arr, mid + 1, right)
        
        // Merge the sorted halves
        merge(arr, left, mid, right)`,
      applications: [
        "Stable sorting in databases",
        "External sorting when data doesn't fit in memory",
        "Sorting linked lists efficiently",
        "Counting inversions in an array"
      ]
    },
    'quick-sort': {
      title: "Quick Sort",
      description: "An efficient sorting algorithm that uses the divide and conquer approach by selecting a 'pivot' element and partitioning the array around it.",
      complexity: {
        time: "Best/Average: O(n log n), Worst: O(n²)",
        space: "O(log n) for the recursive call stack"
      },
      pseudocode: `function quickSort(arr, low, high):
    if low < high:
        // Partition the array and get the pivot index
        pivotIndex = partition(arr, low, high)
        
        // Sort the elements before and after the pivot
        quickSort(arr, low, pivotIndex - 1)
        quickSort(arr, pivotIndex + 1, high)`,
      applications: [
        "General-purpose sorting in many programming languages",
        "Systems with good cache locality",
        "When average-case performance is more important than worst-case",
        "Sorting of primitive types in most standard libraries"
      ]
    },
    'max-subarray': {
      title: "Maximum Subarray",
      description: "A problem of finding the contiguous subarray within a one-dimensional array of numbers which has the largest sum, solved efficiently using divide and conquer.",
      complexity: {
        time: "O(n log n) with divide and conquer, O(n) with Kadane's algorithm",
        space: "O(log n) for the recursive call stack"
      },
      pseudocode: `function maxSubArray(arr, low, high):
    if low == high:
        return arr[low]  // Base case: single element
        
    mid = low + (high - low) / 2
    
    // Find maximum subarray in left half
    leftMax = maxSubArray(arr, low, mid)
    
    // Find maximum subarray in right half
    rightMax = maxSubArray(arr, mid + 1, high)
    
    // Find maximum subarray crossing the midpoint
    crossMax = maxCrossingSum(arr, low, mid, high)
    
    // Return the maximum of the three
    return max(leftMax, rightMax, crossMax)`,
      applications: [
        "Stock market analysis (maximum profit calculation)",
        "Image processing (maximum brightness region)",
        "Signal processing (finding strongest signal segment)",
        "Component of many other algorithms and problems"
      ]
    }
  };

  // Status message for current step
  const getStatusMessage = () => {
    if (!animationSteps.length || !animationSteps[animationStep]) {
      return "Initializing algorithm...";
    }
    
    return animationSteps[animationStep].message || "Processing...";
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Divide and Conquer Strategy Visualization</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Interactive Algorithm Visualization</h2>
        
        <div style={tabsContainerStyle}>
          <div 
            style={tabStyle(activeAlgo === 'binary-search')} 
            onClick={() => setActiveAlgo('binary-search')}
          >
            Binary Search
          </div>
          <div 
            style={tabStyle(activeAlgo === 'merge-sort')} 
            onClick={() => setActiveAlgo('merge-sort')}
          >
            Merge Sort
          </div>
          <div 
            style={tabStyle(activeAlgo === 'quick-sort')} 
            onClick={() => setActiveAlgo('quick-sort')}
          >
            Quick Sort
          </div>
          <div 
            style={tabStyle(activeAlgo === 'max-subarray')} 
            onClick={() => setActiveAlgo('max-subarray')}
          >
            Maximum Subarray
          </div>
        </div>
        
        <div style={boxStyle}>
          <h3 style={{color: colors.yellow}}>{algorithmInfo[activeAlgo].title}</h3>
          <p>{algorithmInfo[activeAlgo].description}</p>
          
          <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', marginBottom: '15px'}}>
            <div>
              <strong style={{color: colors.cyan}}>Time Complexity:</strong>
              <span> {algorithmInfo[activeAlgo].complexity.time}</span>
            </div>
            <div>
              <strong style={{color: colors.cyan}}>Space Complexity:</strong>
              <span> {algorithmInfo[activeAlgo].complexity.space}</span>
            </div>
          </div>
          
          <div style={codeBlockStyle}>
            <code style={{color: colors.text, whiteSpace: 'pre-wrap'}}>
              {algorithmInfo[activeAlgo].pseudocode}
            </code>
          </div>
          
          <div style={{marginTop: '20px', marginBottom: '10px'}}>
            <strong style={{color: colors.pink}}>Applications:</strong>
            <ul style={{margin: '5px 0', paddingLeft: '20px'}}>
              {algorithmInfo[activeAlgo].applications.map((app, index) => (
                <li key={index}>{app}</li>
              ))}
            </ul>
          </div>
          
          <div style={{
            display: 'flex', 
            flexWrap: 'wrap', 
            justifyContent: 'space-between', 
            alignItems: 'center', 
            marginTop: '20px', 
            marginBottom: '20px',
            gap: '10px'
          }}>
            <div>
              <label>
                Array Size: 
                <select
                  style={selectStyle}
                  value={arraySize}
                  onChange={(e) => setArraySize(parseInt(e.target.value))}
                >
                  <option value="8">8</option>
                  <option value="16">16</option>
                  <option value="32">32</option>
                  <option value="64">64</option>
                </select>
              </label>
            </div>
            
            {activeAlgo === 'binary-search' && (
              <div>
                <label>
                  Target: 
                  <input
                    type="number"
                    style={inputStyle}
                    value={target}
                    onChange={handleTargetChange}
                  />
                </label>
              </div>
            )}
            
            <div>
              <label>
                Speed: 
                <select
                  style={selectStyle}
                  value={animationSpeed}
                  onChange={(e) => setAnimationSpeed(parseInt(e.target.value))}
                >
                  <option value="2000">Slow</option>
                  <option value="1000">Medium</option>
                  <option value="500">Fast</option>
                  <option value="200">Very Fast</option>
                </select>
              </label>
            </div>
          </div>
          
          <div style={{
            display: 'flex', 
            justifyContent: 'center', 
            flexWrap: 'wrap', 
            gap: '10px', 
            marginBottom: '20px'
          }}>
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
              Generate New Data
            </button>
          </div>
          
          <div style={{
            color: colors.yellow, 
            textAlign: 'center', 
            marginBottom: '10px', 
            minHeight: '20px',
            padding: '5px',
            backgroundColor: 'rgba(0, 0, 0, 0.2)',
            borderRadius: '4px'
          }}>
            {getStatusMessage()}
          </div>
          
          {renderArrayElements()}
          
          {renderAuxiliaryArrays()}
          
          <div style={statsContainerStyle}>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.yellow}}>{animationStep + 1}</div>
              <div>Step</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.cyan}}>{animationSteps.length}</div>
              <div>Total Steps</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.pink}}>
                {animationSteps.length > 0 && animationSteps[animationStep] && 
                 animationSteps[animationStep].complete ? "Complete" : "In Progress"}
              </div>
              <div>Status</div>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Divide and Conquer Strategy Explained</h2>
        
        <div style={{
          backgroundColor: 'rgba(0, 0, 0, 0.3)', 
          borderRadius: '8px', 
          padding: '15px', 
          marginBottom: '20px',
          border: `1px solid ${colors.green}`
        }}>
          <h3 style={{color: colors.green, marginTop: 0}}>Three Steps of Divide and Conquer</h3>
          
          <div style={{display: 'flex', flexWrap: 'wrap', gap: '15px'}}>
            <div style={{
              flex: '1', 
              minWidth: '200px', 
              padding: '10px', 
              backgroundColor: colors.darkCyan, 
              borderRadius: '5px', 
              border: `1px solid ${colors.cyan}`
            }}>
              <h4 style={{margin: '0 0 5px 0', color: colors.cyan}}>1. Divide</h4>
              <p style={{margin: '0'}}>Break the original problem into smaller subproblems that are similar to the original but simpler to solve.</p>
            </div>
            
            <div style={{
              flex: '1', 
              minWidth: '200px', 
              padding: '10px', 
              backgroundColor: colors.darkPink, 
              borderRadius: '5px', 
              border: `1px solid ${colors.pink}`
            }}>
              <h4 style={{margin: '0 0 5px 0', color: colors.pink}}>2. Conquer</h4>
              <p style={{margin: '0'}}>Solve each subproblem recursively. If the subproblem is small enough, solve it directly (base case).</p>
            </div>
            
            <div style={{
              flex: '1', 
              minWidth: '200px', 
              padding: '10px', 
              backgroundColor: colors.darkYellow, 
              borderRadius: '5px', 
              border: `1px solid ${colors.yellow}`
            }}>
              <h4 style={{margin: '0 0 5px 0', color: colors.yellow}}>3. Combine</h4>
              <p style={{margin: '0'}}>Merge the solutions of the subproblems to create a solution to the original problem.</p>
            </div>
          </div>
        </div>
        
        <div style={{
          backgroundColor: 'rgba(0, 0, 0, 0.3)', 
          borderRadius: '8px', 
          padding: '15px', 
          marginBottom: '20px',
          border: `1px solid ${colors.purple}`
        }}>
          <h3 style={{color: colors.purple, marginTop: 0}}>Recurrence Relations & The Master Theorem</h3>
          
          <div style={{marginBottom: '15px'}}>
            <p>Divide and conquer algorithms typically follow a pattern described by a recurrence relation:</p>
            <div style={{
              textAlign: 'center', 
              fontFamily: 'monospace', 
              fontSize: '18px', 
              padding: '10px',
              color: colors.yellow
            }}>
              T(n) = a · T(n/b) + f(n)
            </div>
            <p>Where:</p>
            <ul style={{paddingLeft: '20px'}}>
              <li><span style={{color: colors.cyan}}>a</span> is the number of subproblems</li>
              <li><span style={{color: colors.pink}}>b</span> is the factor by which the problem size is reduced</li>
              <li><span style={{color: colors.green}}>f(n)</span> is the cost of dividing and combining</li>
            </ul>
          </div>
          
          <p>The Master Theorem provides bounds for recurrences of this form:</p>
          <ol style={{paddingLeft: '20px'}}>
            <li>If f(n) = O(n<sup>log<sub>b</sub>(a)-ε</sup>) for some ε > 0, then T(n) = Θ(n<sup>log<sub>b</sub>(a)</sup>)</li>
            <li>If f(n) = Θ(n<sup>log<sub>b</sub>(a)</sup>), then T(n) = Θ(n<sup>log<sub>b</sub>(a)</sup> log n)</li>
            <li>If f(n) = Ω(n<sup>log<sub>b</sub>(a)+ε</sup>) for some ε > 0, then T(n) = Θ(f(n))</li>
          </ol>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Divide and Conquer Examples</h2>
        
        <div style={{display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(250px, 1fr))', gap: '15px'}}>
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.cyan}`
          }}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>Binary Search</h3>
            <p><strong>Divide:</strong> Compare target with middle element</p>
            <p><strong>Conquer:</strong> Search in the appropriate half</p>
            <p><strong>Combine:</strong> Return the found index or -1</p>
            <p><strong>Complexity:</strong> O(log n)</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 0, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.pink}`
          }}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Merge Sort</h3>
            <p><strong>Divide:</strong> Split array into two halves</p>
            <p><strong>Conquer:</strong> Recursively sort each half</p>
            <p><strong>Combine:</strong> Merge the sorted halves</p>
            <p><strong>Complexity:</strong> O(n log n)</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.green}`
          }}>
            <h3 style={{color: colors.green, marginTop: 0}}>Quick Sort</h3>
            <p><strong>Divide:</strong> Partition around a pivot</p>
            <p><strong>Conquer:</strong> Recursively sort subarrays</p>
            <p><strong>Combine:</strong> No explicit combine step needed</p>
            <p><strong>Complexity:</strong> O(n log n) average</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.yellow}`
          }}>
            <h3 style={{color: colors.yellow, marginTop: 0}}>Strassen's Matrix Multiplication</h3>
            <p><strong>Divide:</strong> Split matrices into quadrants</p>
            <p><strong>Conquer:</strong> Use 7 multiplications instead of 8</p>
            <p><strong>Combine:</strong> Add/subtract to get final matrix</p>
            <p><strong>Complexity:</strong> O(n<sup>2.81</sup>) vs O(n<sup>3</sup>)</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 136, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.orange}`
          }}>
            <h3 style={{color: colors.orange, marginTop: 0}}>Maximum Subarray</h3>
            <p><strong>Divide:</strong> Split array at midpoint</p>
            <p><strong>Conquer:</strong> Find max in left, right, and crossing</p>
            <p><strong>Combine:</strong> Return the maximum of three results</p>
            <p><strong>Complexity:</strong> O(n log n)</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(187, 0, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.purple}`
          }}>
            <h3 style={{color: colors.purple, marginTop: 0}}>Fast Fourier Transform</h3>
            <p><strong>Divide:</strong> Split into even and odd indices</p>
            <p><strong>Conquer:</strong> Compute FFT recursively</p>
            <p><strong>Combine:</strong> Combine using butterfly operation</p>
            <p><strong>Complexity:</strong> O(n log n)</p>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 19: Divide and Conquer Strategy
      </div>
    </div>
  );
};

export default DivideAndConquerVisualization;
