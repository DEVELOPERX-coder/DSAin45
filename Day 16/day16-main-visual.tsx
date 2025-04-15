import React, { useState } from 'react';

const SearchingAlgorithmsVisualization = () => {
  // State to track which algorithm is being demonstrated
  const [activeDemo, setActiveDemo] = useState('binary');
  const [searchValue, setSearchValue] = useState(42);
  const [animationStep, setAnimationStep] = useState(0);
  const [animationPlaying, setAnimationPlaying] = useState(false);
  const [animationSpeed, setAnimationSpeed] = useState(1000);

  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    text: '#FFFFFF'
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
    width: '60px',
    textAlign: 'center',
    margin: '0 10px',
  };

  // Binary search visualization data
  const binarySearchArray = [2, 5, 8, 12, 16, 23, 38, 42, 56, 72, 91, 95, 102, 121, 145, 167];
  
  // Linear search visualization data
  const linearSearchArray = [34, 12, 78, 56, 45, 23, 65, 87, 32, 11, 90, 42, 100, 76, 33, 50];
  
  // Jump search visualization data
  const jumpSearchArray = [2, 5, 8, 12, 16, 23, 38, 42, 56, 72, 91, 95, 102, 121, 145, 167];
  
  // Interpolation search visualization data 
  const interpolationSearchArray = [2, 5, 8, 12, 16, 23, 38, 42, 56, 72, 91, 95, 102, 121, 145, 167];

  const getArrayByAlgorithm = () => {
    switch(activeDemo) {
      case 'linear': return linearSearchArray;
      case 'binary': return binarySearchArray;
      case 'jump': return jumpSearchArray;
      case 'interpolation': return interpolationSearchArray;
      default: return binarySearchArray;
    }
  };

  // Binary search steps
  const getBinarySearchSteps = (array, target) => {
    const steps = [];
    let left = 0;
    let right = array.length - 1;
    
    while(left <= right) {
      const mid = Math.floor(left + (right - left) / 2);
      steps.push({
        left,
        right,
        mid,
        comparison: array[mid] === target ? 'equal' : (array[mid] < target ? 'less' : 'greater')
      });
      
      if(array[mid] === target) {
        break;
      } else if(array[mid] < target) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
    
    return steps;
  };

  // Linear search steps
  const getLinearSearchSteps = (array, target) => {
    const steps = [];
    
    for(let i = 0; i < array.length; i++) {
      steps.push({
        index: i,
        comparison: array[i] === target ? 'equal' : (array[i] < target ? 'less' : 'greater')
      });
      
      if(array[i] === target) {
        break;
      }
    }
    
    return steps;
  };

  // Jump search steps
  const getJumpSearchSteps = (array, target) => {
    const steps = [];
    const n = array.length;
    const step = Math.floor(Math.sqrt(n));
    
    let prev = 0;
    steps.push({
      type: 'jump',
      index: prev,
      jumpTo: Math.min(step, n) - 1,
      text: `Jump to index ${Math.min(step, n) - 1}`
    });
    
    // Finding the block where element might be present
    let jumpIndex = Math.min(step, n) - 1;
    while(prev < n && array[jumpIndex] < target) {
      prev = jumpIndex + 1;
      jumpIndex = Math.min(prev + step, n) - 1;
      steps.push({
        type: 'jump',
        index: prev,
        jumpTo: jumpIndex,
        text: `Jump to index ${jumpIndex}`
      });
    }
    
    // Linear search in identified block
    for(let i = prev; i <= Math.min(jumpIndex, n-1); i++) {
      steps.push({
        type: 'linear',
        index: i,
        comparison: array[i] === target ? 'equal' : (array[i] < target ? 'less' : 'greater')
      });
      
      if(array[i] === target) {
        break;
      }
    }
    
    return steps;
  };

  // Interpolation search steps
  const getInterpolationSearchSteps = (array, target) => {
    const steps = [];
    let low = 0;
    let high = array.length - 1;
    
    while(low <= high && target >= array[low] && target <= array[high]) {
      if(low === high) {
        steps.push({
          low,
          high,
          pos: low,
          text: `low = high = ${low}, checking if target found`
        });
        break;
      }
      
      // Calculate pos using the interpolation formula
      const pos = Math.floor(low + ((target - array[low]) * (high - low)) / (array[high] - array[low]));
      
      steps.push({
        low,
        high,
        pos,
        text: `Calculated position: ${pos}`,
        formula: `pos = low + ((target - A[low]) * (high - low)) / (A[high] - A[low])`,
        comparison: array[pos] === target ? 'equal' : (array[pos] < target ? 'less' : 'greater')
      });
      
      if(array[pos] === target) {
        break;
      } else if(array[pos] < target) {
        low = pos + 1;
      } else {
        high = pos - 1;
      }
    }
    
    return steps;
  };

  const getSearchStepsByAlgorithm = () => {
    const array = getArrayByAlgorithm();
    
    switch(activeDemo) {
      case 'linear': return getLinearSearchSteps(array, searchValue);
      case 'binary': return getBinarySearchSteps(array, searchValue);
      case 'jump': return getJumpSearchSteps(array, searchValue);
      case 'interpolation': return getInterpolationSearchSteps(array, searchValue);
      default: return getBinarySearchSteps(array, searchValue);
    }
  };

  const steps = getSearchStepsByAlgorithm();
  const currentStep = steps[Math.min(animationStep, steps.length - 1)];

  const startAnimation = () => {
    setAnimationStep(0);
    setAnimationPlaying(true);
  };

  const pauseAnimation = () => {
    setAnimationPlaying(false);
  };

  const stepForward = () => {
    if(animationStep < steps.length - 1) {
      setAnimationStep(animationStep + 1);
    }
  };

  const stepBackward = () => {
    if(animationStep > 0) {
      setAnimationStep(animationStep - 1);
    }
  };

  // Effect for animation playback
  React.useEffect(() => {
    let timer;
    if(animationPlaying && animationStep < steps.length - 1) {
      timer = setTimeout(() => {
        setAnimationStep(animationStep + 1);
      }, animationSpeed);
    } else if(animationPlaying && animationStep >= steps.length - 1) {
      setAnimationPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [animationPlaying, animationStep, steps.length, animationSpeed]);

  const renderArrayCell = (value, index, highlightType) => {
    let backgroundColor = 'rgba(0, 0, 0, 0.3)';
    let borderColor = 'rgba(255, 255, 255, 0.1)';
    let textColor = colors.text;
    
    if(highlightType === 'search') {
      backgroundColor = 'rgba(255, 255, 0, 0.3)';
      borderColor = colors.yellow;
      textColor = colors.yellow;
    } else if(highlightType === 'found') {
      backgroundColor = 'rgba(0, 255, 0, 0.3)';
      borderColor = colors.green;
      textColor = colors.green;
    } else if(highlightType === 'jump') {
      backgroundColor = 'rgba(255, 0, 255, 0.3)';
      borderColor = colors.pink;
      textColor = colors.pink;
    } else if(highlightType === 'range') {
      backgroundColor = 'rgba(0, 255, 255, 0.15)';
      borderColor = colors.cyan;
    } else if(highlightType === 'excluded') {
      backgroundColor = 'rgba(255, 255, 255, 0.05)';
      textColor = 'rgba(255, 255, 255, 0.3)';
    }
    
    return (
      <div 
        key={index}
        style={{
          width: '36px',
          height: '36px',
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'center',
          backgroundColor,
          border: `1px solid ${borderColor}`,
          borderRadius: '4px',
          margin: '2px',
          color: textColor,
          fontSize: '14px',
          position: 'relative',
        }}
      >
        {value}
        <div style={{
          position: 'absolute',
          bottom: '-20px',
          fontSize: '10px',
          color: 'rgba(255, 255, 255, 0.5)',
        }}>
          {index}
        </div>
      </div>
    );
  };

  const renderBinarySearchVisualization = () => {
    const step = currentStep;
    
    return (
      <div style={{padding: '20px 0'}}>
        <div style={{marginBottom: '20px'}}>
          <p>Searching for: <span style={{color: colors.yellow}}>{searchValue}</span></p>
          <p>Step {animationStep + 1} of {steps.length}:</p>
          {step && (
            <p>
              Left: {step.left}, Right: {step.right}, Middle: {step.mid}, 
              Value at middle: {binarySearchArray[step.mid]}
              {step.comparison === 'equal' && <span style={{color: colors.green}}> (Found!)</span>}
              {step.comparison === 'less' && <span style={{color: colors.cyan}}> (Too small, search right half)</span>}
              {step.comparison === 'greater' && <span style={{color: colors.pink}}> (Too large, search left half)</span>}
            </p>
          )}
        </div>
        
        <div style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'center', margin: '10px 0 30px'}}>
          {binarySearchArray.map((value, index) => {
            let highlightType = 'normal';
            
            if(step) {
              if(index === step.mid) {
                highlightType = step.comparison === 'equal' ? 'found' : 'search';
              } else if(index >= step.left && index <= step.right) {
                highlightType = 'range';
              } else {
                highlightType = 'excluded';
              }
            }
            
            return renderArrayCell(value, index, highlightType);
          })}
        </div>
      </div>
    );
  };

  const renderLinearSearchVisualization = () => {
    const step = currentStep;
    
    return (
      <div style={{padding: '20px 0'}}>
        <div style={{marginBottom: '20px'}}>
          <p>Searching for: <span style={{color: colors.yellow}}>{searchValue}</span></p>
          <p>Step {animationStep + 1} of {steps.length}:</p>
          {step && (
            <p>
              Checking index: {step.index}, Value: {linearSearchArray[step.index]}
              {step.comparison === 'equal' && <span style={{color: colors.green}}> (Found!)</span>}
              {step.comparison !== 'equal' && <span style={{color: colors.pink}}> (Not a match, continue)</span>}
            </p>
          )}
        </div>
        
        <div style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'center', margin: '10px 0 30px'}}>
          {linearSearchArray.map((value, index) => {
            let highlightType = 'normal';
            
            if(step && index === step.index) {
              highlightType = step.comparison === 'equal' ? 'found' : 'search';
            } else if(step && index < step.index) {
              highlightType = 'excluded';
            }
            
            return renderArrayCell(value, index, highlightType);
          })}
        </div>
      </div>
    );
  };

  const renderJumpSearchVisualization = () => {
    const step = currentStep;
    
    return (
      <div style={{padding: '20px 0'}}>
        <div style={{marginBottom: '20px'}}>
          <p>Searching for: <span style={{color: colors.yellow}}>{searchValue}</span></p>
          <p>Step {animationStep + 1} of {steps.length}:</p>
          {step && step.type === 'jump' && (
            <p>
              <span style={{color: colors.pink}}>Jump step:</span> From index {step.index} to {step.jumpTo}
            </p>
          )}
          {step && step.type === 'linear' && (
            <p>
              <span style={{color: colors.cyan}}>Linear search step:</span> Checking index {step.index}, 
              Value: {jumpSearchArray[step.index]}
              {step.comparison === 'equal' && <span style={{color: colors.green}}> (Found!)</span>}
            </p>
          )}
        </div>
        
        <div style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'center', margin: '10px 0 30px'}}>
          {jumpSearchArray.map((value, index) => {
            let highlightType = 'normal';
            
            if(step && step.type === 'jump') {
              if(index === step.jumpTo) {
                highlightType = 'jump';
              } else if(index >= step.index && index < step.jumpTo) {
                highlightType = 'range';
              } else if(index < step.index) {
                highlightType = 'excluded';
              }
            } else if(step && step.type === 'linear') {
              if(index === step.index) {
                highlightType = step.comparison === 'equal' ? 'found' : 'search';
              } else if(index < step.index) {
                highlightType = 'excluded';
              }
            }
            
            return renderArrayCell(value, index, highlightType);
          })}
        </div>
      </div>
    );
  };

  const renderInterpolationSearchVisualization = () => {
    const step = currentStep;
    
    return (
      <div style={{padding: '20px 0'}}>
        <div style={{marginBottom: '20px'}}>
          <p>Searching for: <span style={{color: colors.yellow}}>{searchValue}</span></p>
          <p>Step {animationStep + 1} of {steps.length}:</p>
          {step && (
            <>
              <p>
                Low: {step.low}, High: {step.high}, Calculated Position: {step.pos}
              </p>
              <p>
                Value at position: {interpolationSearchArray[step.pos]}
                {step.comparison === 'equal' && <span style={{color: colors.green}}> (Found!)</span>}
                {step.comparison === 'less' && <span style={{color: colors.cyan}}> (Too small, search right half)</span>}
                {step.comparison === 'greater' && <span style={{color: colors.pink}}> (Too large, search left half)</span>}
              </p>
              <p style={{color: colors.yellow, fontSize: '12px'}}>
                {step.formula}
              </p>
            </>
          )}
        </div>
        
        <div style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'center', margin: '10px 0 30px'}}>
          {interpolationSearchArray.map((value, index) => {
            let highlightType = 'normal';
            
            if(step) {
              if(index === step.pos) {
                highlightType = step.comparison === 'equal' ? 'found' : 'search';
              } else if(index >= step.low && index <= step.high) {
                highlightType = 'range';
              } else {
                highlightType = 'excluded';
              }
            }
            
            return renderArrayCell(value, index, highlightType);
          })}
        </div>
      </div>
    );
  };

  const renderActiveVisualization = () => {
    switch(activeDemo) {
      case 'linear': return renderLinearSearchVisualization();
      case 'binary': return renderBinarySearchVisualization();
      case 'jump': return renderJumpSearchVisualization();
      case 'interpolation': return renderInterpolationSearchVisualization();
      default: return renderBinarySearchVisualization();
    }
  };

  const algorithmInfo = {
    linear: {
      title: "Linear Search",
      description: "Checks each element sequentially until the target is found or the array ends.",
      timeComplexity: "O(n)",
      spaceComplexity: "O(1)",
      bestFor: "Unsorted arrays, small datasets",
      code: `int linearSearch(const vector<int>& arr, int target) {
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i] == target) {
      return i;  // Found the target at index i
    }
  }
  return -1;  // Target not found
}`
    },
    binary: {
      title: "Binary Search",
      description: "Repeatedly divides the search interval in half until the target is found.",
      timeComplexity: "O(log n)",
      spaceComplexity: "O(1) iterative, O(log n) recursive",
      bestFor: "Sorted arrays, large datasets",
      code: `int binarySearch(const vector<int>& arr, int target) {
  int left = 0;
  int right = arr.size() - 1;
  
  while (left <= right) {
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
      return mid;  // Found the target
    }
    
    if (arr[mid] < target) {
      left = mid + 1;  // Target is in the right half
    } else {
      right = mid - 1;  // Target is in the left half
    }
  }
  
  return -1;  // Target not found
}`
    },
    jump: {
      title: "Jump Search",
      description: "Jumps ahead by fixed steps, then performs linear search when target is bracketed.",
      timeComplexity: "O(√n)",
      spaceComplexity: "O(1)",
      bestFor: "Sorted arrays of medium size",
      code: `int jumpSearch(const vector<int>& arr, int target) {
  int n = arr.size();
  int step = sqrt(n);  // Optimal step size
  
  // Find the block where element might be present
  int prev = 0;
  while (arr[min(step, n) - 1] < target) {
    prev = step;
    step += sqrt(n);
    if (prev >= n) return -1;
  }
  
  // Linear search within the identified block
  while (arr[prev] < target) {
    prev++;
    if (prev == min(step, n)) return -1;
  }
  
  if (arr[prev] == target) return prev;
  
  return -1;  // Target not found
}`
    },
    interpolation: {
      title: "Interpolation Search",
      description: "Estimates target position based on distributed values, like finding a name in a phone book.",
      timeComplexity: "O(log log n) average, O(n) worst",
      spaceComplexity: "O(1)",
      bestFor: "Sorted, uniformly distributed arrays",
      code: `int interpolationSearch(const vector<int>& arr, int target) {
  int low = 0;
  int high = arr.size() - 1;
  
  while (low <= high && target >= arr[low] && target <= arr[high]) {
    if (low == high) {
      if (arr[low] == target) return low;
      return -1;
    }
    
    // Estimate position with probe formula
    int pos = low + ((target - arr[low]) * (high - low)) / 
                     (arr[high] - arr[low]);
    
    if (arr[pos] == target) return pos;
    
    if (arr[pos] < target) low = pos + 1;
    else high = pos - 1;
  }
  
  return -1;  // Target not found
}`
    }
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Searching Algorithms</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Interactive Algorithm Visualizations</h2>
        
        <div style={tabsContainerStyle}>
          <div style={tabStyle(activeDemo === 'linear')} onClick={() => setActiveDemo('linear')}>
            Linear Search
          </div>
          <div style={tabStyle(activeDemo === 'binary')} onClick={() => setActiveDemo('binary')}>
            Binary Search
          </div>
          <div style={tabStyle(activeDemo === 'jump')} onClick={() => setActiveDemo('jump')}>
            Jump Search
          </div>
          <div style={tabStyle(activeDemo === 'interpolation')} onClick={() => setActiveDemo('interpolation')}>
            Interpolation Search
          </div>
        </div>
        
        <div style={boxStyle}>
          <h3 style={{color: colors.yellow}}>{algorithmInfo[activeDemo].title}</h3>
          <p>{algorithmInfo[activeDemo].description}</p>
          
          <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', marginBottom: '15px'}}>
            <div>
              <strong style={{color: colors.cyan}}>Time Complexity:</strong> {algorithmInfo[activeDemo].timeComplexity}
            </div>
            <div>
              <strong style={{color: colors.cyan}}>Space Complexity:</strong> {algorithmInfo[activeDemo].spaceComplexity}
            </div>
            <div>
              <strong style={{color: colors.cyan}}>Best For:</strong> {algorithmInfo[activeDemo].bestFor}
            </div>
          </div>
          
          <div style={codeBlockStyle}>
            <code style={{color: colors.text}}>
              {algorithmInfo[activeDemo].code.split('\n').map((line, i) => (
                <div key={i} style={{
                  color: line.includes('return') ? colors.green : 
                         line.includes('if') ? colors.yellow :
                         line.includes('while') ? colors.pink : colors.text
                }}>
                  {line}
                </div>
              ))}
            </code>
          </div>
          
          <div style={{marginTop: '20px', marginBottom: '10px'}}>
            <label>
              Search for value: 
              <input 
                type="number" 
                style={inputStyle}
                value={searchValue}
                onChange={(e) => {
                  setSearchValue(parseInt(e.target.value, 10) || 0);
                  setAnimationStep(0);
                }}
              />
            </label>
            <label style={{marginLeft: '20px'}}>
              Animation Speed: 
              <select 
                style={{...inputStyle, width: '100px'}}
                value={animationSpeed}
                onChange={(e) => setAnimationSpeed(parseInt(e.target.value, 10))}
              >
                <option value={2000}>Slow</option>
                <option value={1000}>Medium</option>
                <option value={500}>Fast</option>
              </select>
            </label>
          </div>
          
          <div style={{display: 'flex', justifyContent: 'center', gap: '10px', marginBottom: '20px'}}>
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
          </div>
          
          {renderActiveVisualization()}
          
          <div style={{textAlign: 'center', fontSize: '12px', marginTop: '10px'}}>
            {activeDemo === 'linear' && "Linear search checks each element one by one."}
            {activeDemo === 'binary' && "Binary search divides the array in half with each step."}
            {activeDemo === 'jump' && "Jump search makes √n jumps, then linear search."}
            {activeDemo === 'interpolation' && "Interpolation search estimates position based on value distribution."}
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Algorithm Comparison</h2>
        
        <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', justifyContent: 'space-between'}}>
          <div style={{flex: '1', minWidth: '270px', padding: '15px', backgroundColor: 'rgba(0, 255, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.cyan}`}}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>Time Complexity</h3>
            <table style={{width: '100%', borderCollapse: 'collapse'}}>
              <thead>
                <tr>
                  <th style={{textAlign: 'left', borderBottom: `1px solid ${colors.cyan}`, padding: '5px'}}>Algorithm</th>
                  <th style={{textAlign: 'left', borderBottom: `1px solid ${colors.cyan}`, padding: '5px'}}>Best</th>
                  <th style={{textAlign: 'left', borderBottom: `1px solid ${colors.cyan}`, padding: '5px'}}>Average</th>
                  <th style={{textAlign: 'left', borderBottom: `1px solid ${colors.cyan}`, padding: '5px'}}>Worst</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td style={{padding: '5px'}}>Linear</td>
                  <td style={{padding: '5px'}}>O(1)</td>
                  <td style={{padding: '5px'}}>O(n)</td>
                  <td style={{padding: '5px'}}>O(n)</td>
                </tr>
                <tr>
                  <td style={{padding: '5px'}}>Binary</td>
                  <td style={{padding: '5px'}}>O(1)</td>
                  <td style={{padding: '5px'}}>O(log n)</td>
                  <td style={{padding: '5px'}}>O(log n)</td>
                </tr>
                <tr>
                  <td style={{padding: '5px'}}>Jump</td>
                  <td style={{padding: '5px'}}>O(1)</td>
                  <td style={{padding: '5px'}}>O(√n)</td>
                  <td style={{padding: '5px'}}>O(√n)</td>
                </tr>
                <tr>
                  <td style={{padding: '5px'}}>Interpolation</td>
                  <td style={{padding: '5px'}}>O(1)</td>
                  <td style={{padding: '5px'}}>O(log log n)*</td>
                  <td style={{padding: '5px'}}>O(n)</td>
                </tr>
              </tbody>
            </table>
            <p style={{fontSize: '12px', marginTop: '10px'}}>* For uniformly distributed data</p>
          </div>
          
          <div style={{flex: '1', minWidth: '270px', padding: '15px', backgroundColor: 'rgba(255, 0, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.pink}`}}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Practical Comparisons</h3>
            <p>Number of comparisons for array of size 1,000,000:</p>
            <ul style={{paddingLeft: '20px'}}>
              <li><strong>Linear Search:</strong> 500,000 (average)</li>
              <li><strong>Binary Search:</strong> ~20</li>
              <li><strong>Jump Search:</strong> ~1,000</li>
              <li><strong>Interpolation:</strong> ~7 (uniform data)</li>
            </ul>
            <p style={{marginTop: '15px'}}>When to use each:</p>
            <ul style={{paddingLeft: '20px'}}>
              <li><strong>Linear:</strong> Unsorted data, small arrays</li>
              <li><strong>Binary:</strong> Sorted arrays, general purpose</li>
              <li><strong>Jump:</strong> Sorted arrays, memory constraints</li>
              <li><strong>Interpolation:</strong> Sorted, uniform data</li>
            </ul>
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
            <h3 style={{color: colors.green, marginTop: 0}}>Database Indexing</h3>
            <p>B-Trees and variants power database indexes, enabling fast record lookups with O(log n) performance.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 255, 0, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.yellow}`
          }}>
            <h3 style={{color: colors.yellow, marginTop: 0}}>Search Engines</h3>
            <p>Advanced search algorithms process billions of documents with sophisticated ranking mechanisms.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(0, 255, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.cyan}`
          }}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>IP Routing</h3>
            <p>Routers use specialized search algorithms to quickly find the next hop for an IP packet.</p>
          </div>
          
          <div style={{
            padding: '15px', 
            backgroundColor: 'rgba(255, 0, 255, 0.1)', 
            borderRadius: '8px', 
            border: `1px solid ${colors.pink}`
          }}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Spell Checkers</h3>
            <p>Spell checking combines efficient dictionary searches with edit distance algorithms.</p>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 16: Searching Algorithms
      </div>
    </div>
  );
};

export default SearchingAlgorithmsVisualization;
