import React, { useState, useEffect } from 'react';

const SortingAlgorithmsVisualization = () => {
  // State to track which algorithm is being demonstrated
  const [activeAlgo, setActiveAlgo] = useState('bubble');
  const [arraySize, setArraySize] = useState(15);
  const [arrayType, setArrayType] = useState('random');
  const [array, setArray] = useState([]);
  const [animationStep, setAnimationStep] = useState(0);
  const [animationSteps, setAnimationSteps] = useState([]);
  const [animationPlaying, setAnimationPlaying] = useState(false);
  const [animationSpeed, setAnimationSpeed] = useState(500);
  const [comparisons, setComparisons] = useState(0);
  const [swaps, setSwaps] = useState(0);

  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    text: '#FFFFFF',
    darkCyan: 'rgba(0, 255, 255, 0.15)',
    darkPink: 'rgba(255, 0, 255, 0.15)',
    darkYellow: 'rgba(255, 255, 0, 0.15)',
    darkGreen: 'rgba(0, 255, 0, 0.15)',
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
    setSwaps(0);
  };

  // Generate bubble sort animation steps
  const generateBubbleSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let swapCount = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      comparing: [],
      swapping: [],
      sorted: [],
      compareCount,
      swapCount
    });
    
    for (let i = 0; i < copy.length - 1; i++) {
      let swapped = false;
      
      for (let j = 0; j < copy.length - i - 1; j++) {
        // Add comparison step
        compareCount++;
        steps.push({
          array: [...copy],
          comparing: [j, j + 1],
          swapping: [],
          sorted: Array.from({ length: copy.length }, (_, idx) => idx >= copy.length - i),
          compareCount,
          swapCount
        });
        
        if (copy[j] > copy[j + 1]) {
          // Swap elements
          [copy[j], copy[j + 1]] = [copy[j + 1], copy[j]];
          swapped = true;
          swapCount++;
          
          // Add swap step
          steps.push({
            array: [...copy],
            comparing: [],
            swapping: [j, j + 1],
            sorted: Array.from({ length: copy.length }, (_, idx) => idx >= copy.length - i),
            compareCount,
            swapCount
          });
        }
      }
      
      // Mark the last element as sorted
      steps.push({
        array: [...copy],
        comparing: [],
        swapping: [],
        sorted: Array.from({ length: copy.length }, (_, idx) => idx >= copy.length - i - 1),
        compareCount,
        swapCount
      });
      
      if (!swapped) break;
    }
    
    // Final sorted state
    steps.push({
      array: [...copy],
      comparing: [],
      swapping: [],
      sorted: Array.from({ length: copy.length }, () => true),
      compareCount,
      swapCount,
      complete: true
    });
    
    return steps;
  };

  // Generate selection sort animation steps
  const generateSelectionSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let swapCount = 0;
    
    // Add initial state
    steps.push({
      array: [...copy],
      comparing: [],
      swapping: [],
      sorted: [],
      minIndex: null,
      compareCount,
      swapCount
    });
    
    for (let i = 0; i < copy.length - 1; i++) {
      let minIndex = i;
      
      // Add step showing current position
      steps.push({
        array: [...copy],
        comparing: [i],
        swapping: [],
        sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
        minIndex,
        compareCount,
        swapCount
      });
      
      for (let j = i + 1; j < copy.length; j++) {
        // Add comparison step
        compareCount++;
        steps.push({
          array: [...copy],
          comparing: [minIndex, j],
          swapping: [],
          sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
          minIndex,
          compareCount,
          swapCount
        });
        
        if (copy[j] < copy[minIndex]) {
          minIndex = j;
          
          // Add step showing new minimum
          steps.push({
            array: [...copy],
            comparing: [],
            swapping: [],
            sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
            minIndex,
            compareCount,
            swapCount
          });
        }
      }
      
      // Swap elements if minimum is not already at position i
      if (minIndex !== i) {
        [copy[i], copy[minIndex]] = [copy[minIndex], copy[i]];
        swapCount++;
        
        // Add swap step
        steps.push({
          array: [...copy],
          comparing: [],
          swapping: [i, minIndex],
          sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
          minIndex: null,
          compareCount,
          swapCount
        });
      }
      
      // Mark element as sorted
      steps.push({
        array: [...copy],
        comparing: [],
        swapping: [],
        sorted: Array.from({ length: copy.length }, (_, idx) => idx <= i),
        minIndex: null,
        compareCount,
        swapCount
      });
    }
    
    // Final sorted state
    steps.push({
      array: [...copy],
      comparing: [],
      swapping: [],
      sorted: Array.from({ length: copy.length }, () => true),
      minIndex: null,
      compareCount,
      swapCount,
      complete: true
    });
    
    return steps;
  };

  // Generate insertion sort animation steps
  const generateInsertionSortSteps = (arr) => {
    const steps = [];
    const copy = [...arr];
    let compareCount = 0;
    let swapCount = 0;
    
    // Add initial state with first element considered sorted
    steps.push({
      array: [...copy],
      comparing: [],
      moving: [],
      sorted: [0],
      key: null,
      compareCount,
      swapCount
    });
    
    for (let i = 1; i < copy.length; i++) {
      const key = copy[i];
      
      // Add step showing current key
      steps.push({
        array: [...copy],
        comparing: [],
        moving: [],
        sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
        key: i,
        compareCount,
        swapCount
      });
      
      let j = i - 1;
      
      while (j >= 0) {
        // Add comparison step
        compareCount++;
        steps.push({
          array: [...copy],
          comparing: [j, i],
          moving: [],
          sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
          key: i,
          compareCount,
          swapCount
        });
        
        if (copy[j] > key) {
          // Shift element to the right
          copy[j + 1] = copy[j];
          swapCount++;
          
          // Add shifting step
          steps.push({
            array: [...copy],
            comparing: [],
            moving: [j, j + 1],
            sorted: Array.from({ length: copy.length }, (_, idx) => idx < i),
            key: i,
            compareCount,
            swapCount
          });
          
          j--;
        } else {
          break;
        }
      }
      
      // Place key in the correct position
      copy[j + 1] = key;
      
      // Add step showing key inserted
      steps.push({
        array: [...copy],
        comparing: [],
        moving: [],
        insertedAt: j + 1,
        sorted: Array.from({ length: copy.length }, (_, idx) => idx <= i),
        key: null,
        compareCount,
        swapCount
      });
    }
    
    // Final sorted state
    steps.push({
      array: [...copy],
      comparing: [],
      moving: [],
      sorted: Array.from({ length: copy.length }, () => true),
      key: null,
      compareCount,
      swapCount,
      complete: true
    });
    
    return steps;
  };

  // Generate steps for the selected algorithm
  const generateAnimationSteps = (arr) => {
    let steps;
    
    switch (activeAlgo) {
      case 'bubble':
        steps = generateBubbleSortSteps(arr);
        break;
      case 'selection':
        steps = generateSelectionSortSteps(arr);
        break;
      case 'insertion':
        steps = generateInsertionSortSteps(arr);
        break;
      default:
        steps = generateBubbleSortSteps(arr);
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
          setSwaps(animationSteps[animationStep].swapCount);
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
        setSwaps(animationSteps[animationStep].swapCount);
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
        setSwaps(animationSteps[animationStep - 1].swapCount);
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
          
          // Element is being compared
          if (step.comparing && step.comparing.includes(index)) {
            backgroundColor = colors.darkYellow;
            borderColor = colors.yellow;
          }
          
          // Element is being swapped
          if (step.swapping && step.swapping.includes(index)) {
            backgroundColor = colors.darkPink;
            borderColor = colors.pink;
          }
          
          // Element is being moved (insertion sort)
          if (step.moving && step.moving.includes(index)) {
            backgroundColor = colors.darkCyan;
            borderColor = colors.cyan;
          }
          
          // Element is the key (insertion sort)
          if (step.key === index) {
            backgroundColor = colors.darkYellow;
            borderColor = colors.yellow;
          }
          
          // Element was just inserted (insertion sort)
          if (step.insertedAt === index) {
            backgroundColor = colors.darkGreen;
            borderColor = colors.green;
          }
          
          // Element is the current minimum (selection sort)
          if (step.minIndex === index) {
            backgroundColor = colors.darkYellow;
            borderColor = colors.yellow;
          }
          
          // Element is sorted
          if (step.sorted && step.sorted[index]) {
            backgroundColor = colors.darkGreen;
            borderColor = colors.green;
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
                color: '#fff',
                position: 'relative',
                transition: 'height 0.3s ease'
              }}
            >
              {arraySize <= 20 && <span>{value}</span>}
            </div>
          );
        })}
      </div>
    );
  };

  // Algorithm info
  const algorithmInfo = {
    bubble: {
      title: "Bubble Sort",
      description: "Repeatedly steps through the list, compares adjacent elements, and swaps them if they're in the wrong order.",
      timeComplexity: {
        best: "O(n)",
        average: "O(n²)",
        worst: "O(n²)"
      },
      spaceComplexity: "O(1)",
      stable: true,
      code: `void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        
        // Last i elements are already sorted
        for (int j = 0; j < n - i - 1; j++) {
            // Compare adjacent elements
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        // If no swapping occurred, array is sorted
        if (!swapped)
            break;
    }
}`
    },
    selection: {
      title: "Selection Sort",
      description: "Repeatedly finds the minimum element from the unsorted part and puts it at the beginning.",
      timeComplexity: {
        best: "O(n²)",
        average: "O(n²)",
        worst: "O(n²)"
      },
      spaceComplexity: "O(1)",
      stable: false,
      code: `void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int i = 0; i < n - 1; i++) {
        // Find minimum element in unsorted array
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        
        // Swap found minimum with first element
        if (min_idx != i)
            std::swap(arr[i], arr[min_idx]);
    }
}`
    },
    insertion: {
      title: "Insertion Sort",
      description: "Builds a sorted array one element at a time, similar to how people sort playing cards in their hands.",
      timeComplexity: {
        best: "O(n)",
        average: "O(n²)",
        worst: "O(n²)"
      },
      spaceComplexity: "O(1)",
      stable: true,
      code: `void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int i = 1; i < n; i++) {
        int key = arr[i];  // Element to be inserted
        int j = i - 1;
        
        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        // Insert key at correct position
        arr[j + 1] = key;
    }
}`
    }
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Basic Sorting Algorithms</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Interactive Algorithm Visualization</h2>
        
        <div style={tabsContainerStyle}>
          <div style={tabStyle(activeAlgo === 'bubble')} onClick={() => setActiveAlgo('bubble')}>
            Bubble Sort
          </div>
          <div style={tabStyle(activeAlgo === 'selection')} onClick={() => setActiveAlgo('selection')}>
            Selection Sort
          </div>
          <div style={tabStyle(activeAlgo === 'insertion')} onClick={() => setActiveAlgo('insertion')}>
            Insertion Sort
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
                  color: line.includes('swap') || line.includes('key') ? colors.pink : 
                         line.includes('if') || line.includes('while') ? colors.yellow :
                         line.includes('for') ? colors.cyan : colors.text
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
          
          {renderArrayElements()}
          
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
              <div style={{fontSize: '24px', color: colors.pink}}>{swaps}</div>
              <div>Swaps</div>
            </div>
            <div style={statBoxStyle}>
              <div style={{fontSize: '24px', color: colors.green}}>
                {animationSteps.length > 0 && animationSteps[animationStep] && animationSteps[animationStep].complete ? "Yes" : "No"}
              </div>
              <div>Sorted</div>
            </div>
          </div>
          
          <div style={{textAlign: 'center', fontSize: '12px', marginTop: '10px'}}>
            {activeAlgo === 'bubble' && "Each pass bubbles the largest unsorted element to its correct position."}
            {activeAlgo === 'selection' && "Each pass selects the smallest unsorted element and places it at the beginning of the unsorted part."}
            {activeAlgo === 'insertion' && "Each step takes the next unsorted element and inserts it into its correct position within the sorted part."}
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Algorithm Comparison</h2>
        
        <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', justifyContent: 'space-between'}}>
          <div style={{flex: '1', minWidth: '270px', padding: '15px', backgroundColor: 'rgba(0, 255, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.cyan}`}}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>Advantages & Disadvantages</h3>
            <div style={{marginBottom: '10px'}}>
              <strong style={{color: colors.yellow}}>Bubble Sort</strong>
              <ul style={{paddingLeft: '20px', margin: '5px 0'}}>
                <li>+ Simple implementation</li>
                <li>+ Good for small or nearly sorted data</li>
                <li>+ Stable sorting algorithm</li>
                <li>- Inefficient for large datasets</li>
              </ul>
            </div>
            
            <div style={{marginBottom: '10px'}}>
              <strong style={{color: colors.yellow}}>Selection Sort</strong>
              <ul style={{paddingLeft: '20px', margin: '5px 0'}}>
                <li>+ Minimizes number of swaps</li>
                <li>+ Works well when memory writes are expensive</li>
                <li>+ In-place algorithm</li>
                <li>- Always O(n²), even for sorted data</li>
                <li>- Not stable</li>
              </ul>
            </div>
            
            <div>
              <strong style={{color: colors.yellow}}>Insertion Sort</strong>
              <ul style={{paddingLeft: '20px', margin: '5px 0'}}>
                <li>+ Efficient for small datasets</li>
                <li>+ Adaptive (O(n) for nearly sorted data)</li>
                <li>+ Online algorithm (processes data as it arrives)</li>
                <li>+ Stable sorting algorithm</li>
                <li>- Inefficient for large, randomly ordered datasets</li>
              </ul>
            </div>
          </div>
          
          <div style={{flex: '1', minWidth: '270px', padding: '15px', backgroundColor: 'rgba(255, 0, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.pink}`}}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Ideal Use Cases</h3>
            
            <div style={{marginBottom: '10px'}}>
              <strong style={{color: colors.cyan}}>Bubble Sort</strong>
              <p style={{marginTop: '5px'}}>Best for teaching sorting concepts and for very small arrays where implementation simplicity matters more than efficiency.</p>
            </div>
            
            <div style={{marginBottom: '10px'}}>
              <strong style={{color: colors.cyan}}>Selection Sort</strong>
              <p style={{marginTop: '5px'}}>Best for systems where writing to memory is significantly more expensive than reading, as it minimizes the number of swaps.</p>
            </div>
            
            <div>
              <strong style={{color: colors.cyan}}>Insertion Sort</strong>
              <p style={{marginTop: '5px'}}>Best for small datasets, nearly sorted arrays, and online scenarios where elements arrive incrementally. Used in V8 JavaScript engine for small arrays.</p>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Real-World Applications</h2>
        
        <div style={{display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(250px, 1fr))', gap: '15px'}}>
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.green}`
          }}>
            <h3 style={{color: colors.green, marginTop: 0}}>Card Sorting by Hand</h3>
            <p>Humans naturally use insertion sort when arranging playing cards—picking up one card at a time and inserting it into the correct position in the sorted portion of the hand.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.yellow}`
          }}>
            <h3 style={{color: colors.yellow, marginTop: 0}}>JavaScript V8 Engine</h3>
            <p>V8 (used in Chrome and Node.js) uses insertion sort for arrays with fewer than 10 elements, as the algorithm's simplicity and low overhead make it faster for small arrays.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.cyan}`
          }}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>Hybrid Sorting Algorithms</h3>
            <p>TimSort (used in Python, Java, and Android) combines merge sort with insertion sort, using insertion sort for small subarrays to improve performance.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 0, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.pink}`
          }}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Online Data Processing</h3>
            <p>Insertion sort is valuable for online scenarios where data arrives one element at a time and needs to be kept in sorted order continuously.</p>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 17: Basic Sorting Algorithms
      </div>
    </div>
  );
};

export default SortingAlgorithmsVisualization;
