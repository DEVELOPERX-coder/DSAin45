import React, { useState, useEffect } from 'react';

const BitManipulator = () => {
  const [number, setNumber] = useState(73);
  const [binaryRep, setBinaryRep] = useState('');
  const [operation, setOperation] = useState('none');
  const [position, setPosition] = useState(3);
  const [result, setResult] = useState(0);
  const [resultBinary, setResultBinary] = useState('');
  const [showCode, setShowCode] = useState(false);
  const [speed, setSpeed] = useState(500);
  const [isAnimating, setIsAnimating] = useState(false);
  const [currentStep, setCurrentStep] = useState(0);
  const [maxStep, setMaxStep] = useState(0);
  const [highlightedBits, setHighlightedBits] = useState([]);
  const [explanation, setExplanation] = useState('Select an operation to begin');

  // Convert number to binary representation
  useEffect(() => {
    const binary = number.toString(2).padStart(8, '0');
    setBinaryRep(binary);
    handleOperation(operation, number, position);
  }, [number, position]);

  // Handle operation change
  useEffect(() => {
    handleOperation(operation, number, position);
  }, [operation]);

  const handleOperation = (op, num, pos) => {
    setCurrentStep(0);
    setHighlightedBits([]);
    
    let res = 0;
    let steps = [];
    let explanation = '';

    switch(op) {
      case 'check':
        steps = generateCheckBitSteps(num, pos);
        explanation = `Checking if bit ${pos} is set in ${num} (${num.toString(2).padStart(8, '0')})`;
        res = (num & (1 << pos)) !== 0 ? 1 : 0;
        break;
      case 'set':
        steps = generateSetBitSteps(num, pos);
        explanation = `Setting bit ${pos} in ${num} (${num.toString(2).padStart(8, '0')})`;
        res = num | (1 << pos);
        break;
      case 'clear':
        steps = generateClearBitSteps(num, pos);
        explanation = `Clearing bit ${pos} in ${num} (${num.toString(2).padStart(8, '0')})`;
        res = num & ~(1 << pos);
        break;
      case 'toggle':
        steps = generateToggleBitSteps(num, pos);
        explanation = `Toggling bit ${pos} in ${num} (${num.toString(2).padStart(8, '0')})`;
        res = num ^ (1 << pos);
        break;
      case 'count':
        steps = generateCountBitsSteps(num);
        explanation = `Counting set bits in ${num} (${num.toString(2).padStart(8, '0')})`;
        res = countBits(num);
        break;
      case 'powerof2':
        steps = generatePowerOf2Steps(num);
        explanation = `Checking if ${num} (${num.toString(2).padStart(8, '0')}) is a power of 2`;
        res = (num > 0 && (num & (num - 1)) === 0) ? 1 : 0;
        break;
      default:
        steps = [];
        explanation = 'Select an operation to begin';
    }

    setResult(res);
    setResultBinary(res.toString(2).padStart(8, '0'));
    setMaxStep(steps.length);
    setExplanation(explanation);
    
    return steps;
  };

  const playAnimation = () => {
    if (currentStep >= maxStep) {
      setCurrentStep(0);
      setHighlightedBits([]);
    }

    setIsAnimating(true);
    const steps = handleOperation(operation, number, position);
    
    let step = 0;
    const interval = setInterval(() => {
      if (step < steps.length) {
        setCurrentStep(step + 1);
        setHighlightedBits(steps[step].highlightBits || []);
        setExplanation(steps[step].explanation || explanation);
        step++;
      } else {
        clearInterval(interval);
        setIsAnimating(false);
      }
    }, speed);

    return () => clearInterval(interval);
  };

  const pauseAnimation = () => {
    setIsAnimating(false);
  };

  const resetAnimation = () => {
    setIsAnimating(false);
    setCurrentStep(0);
    setHighlightedBits([]);
    setExplanation('Select an operation to begin');
    handleOperation(operation, number, position);
  };

  const stepForward = () => {
    if (currentStep >= maxStep) return;
    
    const steps = handleOperation(operation, number, position);
    if (currentStep < steps.length) {
      setCurrentStep(currentStep + 1);
      setHighlightedBits(steps[currentStep].highlightBits || []);
      setExplanation(steps[currentStep].explanation || explanation);
    }
  };

  const stepBackward = () => {
    if (currentStep <= 1) {
      setCurrentStep(0);
      setHighlightedBits([]);
      setExplanation('Select an operation to begin');
      return;
    }
    
    const steps = handleOperation(operation, number, position);
    setCurrentStep(currentStep - 1);
    setHighlightedBits(steps[currentStep - 2].highlightBits || []);
    setExplanation(steps[currentStep - 2].explanation || explanation);
  };

  // Helper functions for generating animation steps
  function generateCheckBitSteps(num, pos) {
    const steps = [];
    const mask = 1 << pos;
    
    steps.push({
      explanation: `Step 1: Create a mask with only bit ${pos} set: ${mask.toString(2).padStart(8, '0')}`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `Step 2: Apply bitwise AND between ${num} and the mask ${mask}`,
      highlightBits: [7 - pos]
    });
    
    const result = num & mask;
    steps.push({
      explanation: `Step 3: Result of AND operation: ${result} (${result.toString(2).padStart(8, '0')})`,
      highlightBits: result > 0 ? [7 - pos] : []
    });
    
    steps.push({
      explanation: `Step 4: Check if result is non-zero: ${result !== 0 ? 'Bit is set (1)' : 'Bit is not set (0)'}`,
      highlightBits: result > 0 ? [7 - pos] : []
    });
    
    return steps;
  }

  function generateSetBitSteps(num, pos) {
    const steps = [];
    const mask = 1 << pos;
    
    steps.push({
      explanation: `Step 1: Create a mask with only bit ${pos} set: ${mask.toString(2).padStart(8, '0')}`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `Step 2: Apply bitwise OR between ${num} and the mask ${mask}`,
      highlightBits: [7 - pos]
    });
    
    const result = num | mask;
    const resultHighlights = [];
    for (let i = 0; i < 8; i++) {
      if ((result & (1 << i)) !== 0) {
        resultHighlights.push(7 - i);
      }
    }
    
    steps.push({
      explanation: `Step 3: Result of OR operation: ${result} (${result.toString(2).padStart(8, '0')})`,
      highlightBits: resultHighlights
    });
    
    return steps;
  }

  function generateClearBitSteps(num, pos) {
    const steps = [];
    const mask = 1 << pos;
    
    steps.push({
      explanation: `Step 1: Create a mask with only bit ${pos} set: ${mask.toString(2).padStart(8, '0')}`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `Step 2: Invert the mask: ~${mask.toString(2).padStart(8, '0')} = ${(~mask >>> 0).toString(2).slice(-8).padStart(8, '0')}`,
      highlightBits: []
    });
    
    const invertedMask = ~mask;
    steps.push({
      explanation: `Step 3: Apply bitwise AND between ${num} and the inverted mask`,
      highlightBits: [7 - pos]
    });
    
    const result = num & invertedMask;
    const resultHighlights = [];
    for (let i = 0; i < 8; i++) {
      if ((result & (1 << i)) !== 0) {
        resultHighlights.push(7 - i);
      }
    }
    
    steps.push({
      explanation: `Step 4: Result of AND operation: ${result} (${result.toString(2).padStart(8, '0')})`,
      highlightBits: resultHighlights
    });
    
    return steps;
  }

  function generateToggleBitSteps(num, pos) {
    const steps = [];
    const mask = 1 << pos;
    
    steps.push({
      explanation: `Step 1: Create a mask with only bit ${pos} set: ${mask.toString(2).padStart(8, '0')}`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `Step 2: Apply bitwise XOR between ${num} and the mask ${mask}`,
      highlightBits: [7 - pos]
    });
    
    const result = num ^ mask;
    const resultHighlights = [];
    for (let i = 0; i < 8; i++) {
      if ((result & (1 << i)) !== 0) {
        resultHighlights.push(7 - i);
      }
    }
    
    steps.push({
      explanation: `Step 3: Result of XOR operation: ${result} (${result.toString(2).padStart(8, '0')})`,
      highlightBits: resultHighlights
    });
    
    return steps;
  }

  function generateCountBitsSteps(num) {
    const steps = [];
    let count = 0;
    let n = num;
    
    steps.push({
      explanation: `Start with number ${num} (${num.toString(2).padStart(8, '0')}) and count = 0`,
      highlightBits: []
    });
    
    while (n > 0) {
      // Find positions of set bits
      const highlightPositions = [];
      for (let i = 0; i < 8; i++) {
        if ((n & (1 << i)) !== 0) {
          highlightPositions.push(7 - i);
        }
      }
      
      steps.push({
        explanation: `Current number: ${n} (${n.toString(2).padStart(8, '0')}), count = ${count}`,
        highlightBits: highlightPositions
      });
      
      const rightmostSetBit = n & -n;
      const rightmostPos = Math.log2(rightmostSetBit);
      
      steps.push({
        explanation: `Clear rightmost set bit at position ${rightmostPos} using n & (n-1)`,
        highlightBits: [7 - rightmostPos]
      });
      
      n &= (n - 1);
      count++;
      
      steps.push({
        explanation: `After clearing: ${n} (${n.toString(2).padStart(8, '0')}), count = ${count}`,
        highlightBits: []
      });
    }
    
    steps.push({
      explanation: `Final count: ${count}`,
      highlightBits: []
    });
    
    return steps;
  }

  function generatePowerOf2Steps(num) {
    const steps = [];
    
    steps.push({
      explanation: `Check if ${num} (${num.toString(2).padStart(8, '0')}) is a power of 2`,
      highlightBits: []
    });
    
    if (num <= 0) {
      steps.push({
        explanation: `${num} ≤ 0, not a power of 2`,
        highlightBits: []
      });
      return steps;
    }
    
    steps.push({
      explanation: `${num} > 0, proceed with the check`,
      highlightBits: []
    });
    
    const numMinus1 = num - 1;
    
    steps.push({
      explanation: `Compute ${num} - 1 = ${numMinus1} (${numMinus1.toString(2).padStart(8, '0')})`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `For powers of 2, n & (n-1) should be 0, because a power of 2 has exactly one bit set`,
      highlightBits: []
    });
    
    const result = num & numMinus1;
    steps.push({
      explanation: `${num} & ${numMinus1} = ${result} (${result.toString(2).padStart(8, '0')})`,
      highlightBits: []
    });
    
    steps.push({
      explanation: `${result === 0 ? 'Result is 0, so ' + num + ' is a power of 2' : 'Result is not 0, so ' + num + ' is not a power of 2'}`,
      highlightBits: []
    });
    
    return steps;
  }

  function countBits(num) {
    let count = 0;
    while (num > 0) {
      num &= (num - 1);
      count++;
    }
    return count;
  }

  // Code snippets for the "Show Code" toggle
  const getCodeSnippet = () => {
    switch(operation) {
      case 'check':
        return `bool isBitSet(int num, int pos) {
    return (num & (1 << pos)) != 0;
}`;
      case 'set':
        return `int setBit(int num, int pos) {
    return num | (1 << pos);
}`;
      case 'clear':
        return `int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}`;
      case 'toggle':
        return `int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}`;
      case 'count':
        return `int countSetBits(int num) {
    int count = 0;
    while (num) {
        num &= (num - 1); // Clear the least significant set bit
        count++;
    }
    return count;
}`;
      case 'powerof2':
        return `bool isPowerOfTwo(int num) {
    return num > 0 && (num & (num - 1)) == 0;
}`;
      default:
        return '// Select an operation to see its code';
    }
  };

  // Render a binary representation with optional highlighting
  const renderBinary = (binaryString, highlights = []) => {
    return (
      <div className="flex justify-center space-x-1">
        {binaryString.split('').map((bit, index) => (
          <div 
            key={index} 
            className={`w-8 h-8 flex items-center justify-center border ${
              highlights.includes(index) 
                ? 'bg-pink-500 text-white' 
                : 'bg-gray-800'
            }`}
          >
            <span className="text-lg">{bit}</span>
          </div>
        ))}
      </div>
    );
  };

  return (
    <div className="p-6 w-full bg-gray-900 text-white min-h-screen">
      <div className="max-w-4xl mx-auto">
        <h1 className="text-3xl font-bold text-center mb-6 text-cyan-400">Bit Manipulation Visualizer</h1>
        
        <div className="bg-gray-800 p-6 rounded-lg shadow-lg mb-6">
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            <div>
              <h2 className="text-xl font-semibold mb-4 text-yellow-300">Input</h2>
              
              <div className="mb-4">
                <label className="block mb-2">Number (0-255):</label>
                <input 
                  type="number" 
                  min="0" 
                  max="255" 
                  value={number} 
                  onChange={(e) => setNumber(parseInt(e.target.value) || 0)} 
                  className="w-full p-2 bg-gray-700 text-white rounded"
                />
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Binary:</label>
                {renderBinary(binaryRep)}
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Operation:</label>
                <select 
                  value={operation} 
                  onChange={(e) => setOperation(e.target.value)}
                  className="w-full p-2 bg-gray-700 text-white rounded"
                >
                  <option value="none">Select Operation</option>
                  <option value="check">Check Bit</option>
                  <option value="set">Set Bit</option>
                  <option value="clear">Clear Bit</option>
                  <option value="toggle">Toggle Bit</option>
                  <option value="count">Count Set Bits</option>
                  <option value="powerof2">Check if Power of 2</option>
                </select>
              </div>
              
              {operation !== 'count' && operation !== 'powerof2' && operation !== 'none' && (
                <div className="mb-4">
                  <label className="block mb-2">Bit Position (0-7):</label>
                  <input 
                    type="number" 
                    min="0" 
                    max="7" 
                    value={position} 
                    onChange={(e) => setPosition(parseInt(e.target.value) || 0)} 
                    className="w-full p-2 bg-gray-700 text-white rounded"
                  />
                </div>
              )}
            </div>
            
            <div>
              <h2 className="text-xl font-semibold mb-4 text-green-400">Result</h2>
              
              <div className="mb-4">
                <label className="block mb-2">Value:</label>
                <div className="p-2 bg-gray-700 rounded">{result}</div>
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Binary:</label>
                {renderBinary(resultBinary, highlightedBits)}
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Explanation:</label>
                <div className="p-2 bg-gray-700 rounded min-h-12">{explanation}</div>
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Progress:</label>
                <div className="w-full bg-gray-700 rounded-full h-2.5">
                  <div 
                    className="bg-cyan-400 h-2.5 rounded-full" 
                    style={{ width: `${maxStep > 0 ? (currentStep / maxStep) * 100 : 0}%` }}
                  ></div>
                </div>
              </div>
            </div>
          </div>
          
          <div className="mt-6">
            <div className="flex justify-center space-x-4 mb-4">
              <button 
                onClick={stepBackward}
                disabled={currentStep <= 0}
                className="px-4 py-2 bg-gray-700 rounded-md hover:bg-gray-600 disabled:opacity-50"
              >
                ⏮ Back
              </button>
              
              {isAnimating ? (
                <button 
                  onClick={pauseAnimation}
                  className="px-4 py-2 bg-pink-600 rounded-md hover:bg-pink-500"
                >
                  ⏸ Pause
                </button>
              ) : (
                <button 
                  onClick={playAnimation}
                  disabled={operation === 'none'}
                  className="px-4 py-2 bg-cyan-600 rounded-md hover:bg-cyan-500 disabled:opacity-50"
                >
                  ▶️ Play
                </button>
              )}
              
              <button 
                onClick={stepForward}
                disabled={currentStep >= maxStep}
                className="px-4 py-2 bg-gray-700 rounded-md hover:bg-gray-600 disabled:opacity-50"
              >
                ⏭ Forward
              </button>
              
              <button 
                onClick={resetAnimation}
                className="px-4 py-2 bg-yellow-600 rounded-md hover:bg-yellow-500"
              >
                🔄 Reset
              </button>
            </div>
            
            <div className="mb-4">
              <label className="block mb-2">Animation Speed:</label>
              <input 
                type="range" 
                min="100" 
                max="2000" 
                step="100" 
                value={speed} 
                onChange={(e) => setSpeed(parseInt(e.target.value))} 
                className="w-full"
              />
              <div className="flex justify-between text-xs">
                <span>Fast</span>
                <span>Slow</span>
              </div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 p-6 rounded-lg shadow-lg">
          <div className="flex justify-between items-center mb-4">
            <h2 className="text-xl font-semibold text-pink-400">Code Implementation</h2>
            <button 
              onClick={() => setShowCode(!showCode)}
              className="px-3 py-1 bg-gray-700 text-sm rounded hover:bg-gray-600"
            >
              {showCode ? 'Hide Code' : 'Show Code'}
            </button>
          </div>
          
          {showCode && (
            <div className="bg-gray-900 p-4 rounded-md overflow-x-auto">
              <pre className="text-green-400">
                {getCodeSnippet()}
              </pre>
            </div>
          )}
          
          <div className="mt-6">
            <h3 className="font-semibold text-yellow-300 mb-2">Time & Space Complexity</h3>
            <p>All bit manipulation operations run in <span className="text-green-400">O(1)</span> time complexity, as they are CPU-level instructions operating on fixed-width integers.</p>
            <p className="mt-2">The space complexity is also <span className="text-green-400">O(1)</span>, using only a constant amount of memory regardless of input size.</p>
          </div>
          
          <div className="mt-6">
            <h3 className="font-semibold text-yellow-300 mb-2">Practical Applications</h3>
            <ul className="list-disc list-inside space-y-2">
              <li>Optimizing memory usage in embedded systems and memory-constrained environments</li>
              <li>Setting and checking flags in configuration options</li>
              <li>Fast arithmetic operations (multiply/divide by powers of 2)</li>
              <li>Building efficient data structures like Bitmap indexes or Bloom filters</li>
              <li>Low-level hardware control and register manipulation</li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  );
};

export default BitManipulator;
