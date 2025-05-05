import React, { useState, useEffect, useCallback } from 'react';

// Main component for the Dynamic Programming Visualization
const DynamicProgrammingVisualization = () => {
  // State for the selected algorithm and parameters
  const [algorithm, setAlgorithm] = useState('fibonacci');
  const [params, setParams] = useState({
    fibonacci: { n: 6 },
    coinChange: { amount: 11, coins: [1, 2, 5] },
    climbStairs: { n: 5 },
    knapsack: { 
      weights: [1, 3, 4, 5], 
      values: [1, 4, 5, 7], 
      capacity: 7 
    }
  });
  
  // State for visualization
  const [showMemoizationTable, setShowMemoizationTable] = useState(true);
  const [showCode, setShowCode] = useState(false);
  const [speed, setSpeed] = useState(500); // Animation speed in ms
  const [playing, setPlaying] = useState(false);
  const [currentStep, setCurrentStep] = useState(0);
  const [steps, setSteps] = useState([]);
  const [maxSteps, setMaxSteps] = useState(0);
  
  // Colors (using required neon on dark theme)
  const colors = {
    background: '#121212',
    text: '#FFFFFF',
    primary: '#00FFFF', // Cyan
    secondary: '#FF00FF', // Pink
    highlight: '#FFFF00', // Yellow
    success: '#00FF00', // Green
    border: '#444444'
  };
  
  // Initialize visualization based on selected algorithm
  useEffect(() => {
    resetVisualization();
  }, [algorithm, params]);
  
  // Function to reset visualization state
  const resetVisualization = () => {
    setCurrentStep(0);
    setPlaying(false);
    
    // Generate steps based on selected algorithm
    let visualizationSteps = [];
    
    switch(algorithm) {
      case 'fibonacci':
        visualizationSteps = generateFibonacciSteps(params.fibonacci.n);
        break;
      case 'coinChange':
        visualizationSteps = generateCoinChangeSteps(params.coinChange.coins, params.coinChange.amount);
        break;
      case 'climbStairs':
        visualizationSteps = generateClimbStairsSteps(params.climbStairs.n);
        break;
      case 'knapsack':
        visualizationSteps = generateKnapsackSteps(
          params.knapsack.weights, 
          params.knapsack.values, 
          params.knapsack.capacity
        );
        break;
      default:
        visualizationSteps = [];
    }
    
    setSteps(visualizationSteps);
    setMaxSteps(visualizationSteps.length - 1);
  };
  
  // Generate steps for Fibonacci visualization
  const generateFibonacciSteps = (n) => {
    const steps = [];
    const memo = Array(n + 1).fill(null);
    
    // Initial state
    steps.push({
      description: `Starting to compute Fibonacci(${n})`,
      callStack: [`fib(${n})`],
      memoTable: [...memo],
      result: null,
      activeNodes: [n],
      completedNodes: []
    });
    
    // Base cases
    memo[0] = 0;
    memo[1] = 1;
    
    steps.push({
      description: 'Initialize base cases',
      callStack: [`fib(${n})`],
      memoTable: [...memo],
      result: null,
      activeNodes: [n],
      completedNodes: [0, 1]
    });
    
    // Function to recursively generate steps
    const generateSteps = (num, callStack) => {
      // Base cases
      if (num <= 1) {
        return num;
      }
      
      // If already computed, return from memo
      if (memo[num] !== null) {
        steps.push({
          description: `fib(${num}) already computed, returning ${memo[num]} from memo`,
          callStack: [...callStack],
          memoTable: [...memo],
          result: null,
          activeNodes: [num],
          completedNodes: Array.from({length: num}, (_, i) => i).filter(i => memo[i] !== null)
        });
        return memo[num];
      }
      
      // Recursive calls
      steps.push({
        description: `Computing fib(${num}) = fib(${num-1}) + fib(${num-2})`,
        callStack: [...callStack, `fib(${num})`],
        memoTable: [...memo],
        result: null,
        activeNodes: [num, num-1],
        completedNodes: Array.from({length: num}, (_, i) => i).filter(i => memo[i] !== null)
      });
      
      const result1 = generateSteps(num - 1, [...callStack, `fib(${num})`]);
      
      steps.push({
        description: `Computed fib(${num-1}) = ${result1}, now computing fib(${num-2})`,
        callStack: [...callStack, `fib(${num})`],
        memoTable: [...memo],
        result: null,
        activeNodes: [num, num-2],
        completedNodes: Array.from({length: num}, (_, i) => i).filter(i => memo[i] !== null)
      });
      
      const result2 = generateSteps(num - 2, [...callStack, `fib(${num})`]);
      
      // Store result in memo
      memo[num] = result1 + result2;
      
      steps.push({
        description: `Computed fib(${num}) = ${result1} + ${result2} = ${memo[num]}, storing in memo`,
        callStack: [...callStack],
        memoTable: [...memo],
        result: null,
        activeNodes: [num],
        completedNodes: Array.from({length: num+1}, (_, i) => i).filter(i => memo[i] !== null)
      });
      
      return memo[num];
    };
    
    // Generate all steps
    const result = generateSteps(n, []);
    
    // Final result
    steps.push({
      description: `Final result: fib(${n}) = ${result}`,
      callStack: [],
      memoTable: [...memo],
      result: result,
      activeNodes: [],
      completedNodes: Array.from({length: n+1}, (_, i) => i)
    });
    
    return steps;
  };
  
  // Generate steps for Coin Change visualization
  const generateCoinChangeSteps = (coins, amount) => {
    const steps = [];
    
    // dp[i] = min coins needed to make amount i
    const dp = Array(amount + 1).fill(Infinity);
    dp[0] = 0; // Base case: no coins needed to make amount 0
    
    // Initial state
    steps.push({
      description: `Initializing DP table for coin denominations [${coins.join(', ')}] and amount ${amount}`,
      dpTable: [...dp],
      coins,
      amount,
      activeCell: null,
      activeCoin: null,
      currentAmount: 0
    });
    
    steps.push({
      description: `Base case: dp[0] = 0 (zero coins needed to make amount 0)`,
      dpTable: [...dp],
      coins,
      amount,
      activeCell: 0,
      activeCoin: null,
      currentAmount: 0
    });
    
    // Fill the dp table
    for (let currentAmount = 1; currentAmount <= amount; currentAmount++) {
      steps.push({
        description: `Computing minimum coins needed for amount ${currentAmount}`,
        dpTable: [...dp],
        coins,
        amount,
        activeCell: currentAmount,
        activeCoin: null,
        currentAmount
      });
      
      for (const coin of coins) {
        if (coin <= currentAmount) {
          steps.push({
            description: `Considering coin ${coin} for amount ${currentAmount}`,
            dpTable: [...dp],
            coins,
            amount,
            activeCell: currentAmount,
            activeCoin: coin,
            currentAmount
          });
          
          const potentialValue = dp[currentAmount - coin] + 1;
          
          steps.push({
            description: `dp[${currentAmount}] = min(dp[${currentAmount}], dp[${currentAmount} - ${coin}] + 1) = min(${dp[currentAmount]}, ${dp[currentAmount - coin]} + 1) = min(${dp[currentAmount]}, ${potentialValue})`,
            dpTable: [...dp],
            coins,
            amount,
            activeCell: currentAmount,
            activeCoin: coin,
            subCell: currentAmount - coin,
            currentAmount
          });
          
          if (potentialValue < dp[currentAmount]) {
            dp[currentAmount] = potentialValue;
            
            steps.push({
              description: `Updated dp[${currentAmount}] = ${dp[currentAmount]}`,
              dpTable: [...dp],
              coins,
              amount,
              activeCell: currentAmount,
              activeCoin: coin,
              currentAmount
            });
          } else {
            steps.push({
              description: `No improvement with coin ${coin}, dp[${currentAmount}] remains ${dp[currentAmount]}`,
              dpTable: [...dp],
              coins,
              amount,
              activeCell: currentAmount,
              activeCoin: coin,
              currentAmount
            });
          }
        }
      }
    }
    
    // Final result
    const finalResult = dp[amount] === Infinity ? -1 : dp[amount];
    
    steps.push({
      description: `Final result: Minimum coins needed for amount ${amount} is ${finalResult}`,
      dpTable: [...dp],
      coins,
      amount,
      activeCell: amount,
      activeCoin: null,
      currentAmount: amount,
      finalResult
    });
    
    return steps;
  };
  
  // Generate steps for Climb Stairs visualization
  const generateClimbStairsSteps = (n) => {
    const steps = [];
    
    // dp[i] = number of ways to climb to step i
    const dp = Array(n + 1).fill(0);
    dp[0] = 1; // Base case: one way to climb 0 steps (stay where you are)
    dp[1] = 1; // Base case: one way to climb 1 step
    
    // Initial state
    steps.push({
      description: `Initializing DP table for climbing ${n} stairs`,
      dpTable: [...dp],
      n,
      activeStep: null
    });
    
    steps.push({
      description: `Base cases: dp[0] = 1, dp[1] = 1`,
      dpTable: [...dp],
      n,
      activeStep: 0,
      previousSteps: []
    });
    
    // Fill the dp table
    for (let i = 2; i <= n; i++) {
      steps.push({
        description: `Computing ways to reach step ${i}`,
        dpTable: [...dp],
        n,
        activeStep: i,
        previousSteps: [i-1, i-2]
      });
      
      dp[i] = dp[i-1] + dp[i-2];
      
      steps.push({
        description: `dp[${i}] = dp[${i-1}] + dp[${i-2}] = ${dp[i-1]} + ${dp[i-2]} = ${dp[i]}`,
        dpTable: [...dp],
        n,
        activeStep: i,
        previousSteps: [i-1, i-2]
      });
    }
    
    // Final result
    steps.push({
      description: `Final result: There are ${dp[n]} distinct ways to climb to the top`,
      dpTable: [...dp],
      n,
      activeStep: n,
      previousSteps: []
    });
    
    return steps;
  };
  
  // Generate steps for Knapsack visualization
  const generateKnapsackSteps = (weights, values, capacity) => {
    const steps = [];
    const n = weights.length;
    
    // Initialize dp table: dp[i][w] = max value with first i items and weight limit w
    const dp = Array(n + 1).fill().map(() => Array(capacity + 1).fill(0));
    
    // Initial state
    steps.push({
      description: `Initializing DP table for 0/1 Knapsack with ${n} items and capacity ${capacity}`,
      dpTable: [...dp.map(row => [...row])],
      weights,
      values,
      capacity,
      activeItem: null,
      activeWeight: null,
      currentItem: 0,
      itemIncluded: null
    });
    
    steps.push({
      description: `Base case: dp[0][w] = 0 for all w (no items means zero value)`,
      dpTable: [...dp.map(row => [...row])],
      weights,
      values,
      capacity,
      activeItem: 0,
      activeWeight: null,
      currentItem: 0,
      itemIncluded: null
    });
    
    // Fill the dp table
    for (let i = 1; i <= n; i++) {
      const itemIndex = i - 1; // Index of current item in original arrays
      
      steps.push({
        description: `Considering item ${i} (weight: ${weights[itemIndex]}, value: ${values[itemIndex]})`,
        dpTable: [...dp.map(row => [...row])],
        weights,
        values,
        capacity,
        activeItem: i,
        activeWeight: null,
        currentItem: i,
        itemIncluded: null
      });
      
      for (let w = 0; w <= capacity; w++) {
        steps.push({
          description: `Computing maximum value for items 1 to ${i} with weight limit ${w}`,
          dpTable: [...dp.map(row => [...row])],
          weights,
          values,
          capacity,
          activeItem: i,
          activeWeight: w,
          currentItem: i,
          itemIncluded: null
        });
        
        // If item is too heavy, skip it
        if (weights[itemIndex] > w) {
          dp[i][w] = dp[i-1][w];
          
          steps.push({
            description: `Item ${i} is too heavy (${weights[itemIndex]} > ${w}), so dp[${i}][${w}] = dp[${i-1}][${w}] = ${dp[i-1][w]}`,
            dpTable: [...dp.map(row => [...row])],
            weights,
            values,
            capacity,
            activeItem: i,
            activeWeight: w,
            currentItem: i,
            itemIncluded: false,
            previousState: {item: i-1, weight: w}
          });
        } else {
          // Either include the item or exclude it
          const includeValue = values[itemIndex] + dp[i-1][w - weights[itemIndex]];
          const excludeValue = dp[i-1][w];
          
          steps.push({
            description: `Comparing: exclude item ${i} (${excludeValue}) vs include item ${i} (${values[itemIndex]} + dp[${i-1}][${w - weights[itemIndex]}] = ${values[itemIndex]} + ${dp[i-1][w - weights[itemIndex]]} = ${includeValue})`,
            dpTable: [...dp.map(row => [...row])],
            weights,
            values,
            capacity,
            activeItem: i,
            activeWeight: w,
            currentItem: i,
            excludeValue,
            includeValue,
            previousStates: [
              {item: i-1, weight: w},
              {item: i-1, weight: w - weights[itemIndex]}
            ]
          });
          
          dp[i][w] = Math.max(includeValue, excludeValue);
          
          const itemIncluded = includeValue > excludeValue;
          
          steps.push({
            description: `dp[${i}][${w}] = max(${excludeValue}, ${includeValue}) = ${dp[i][w]} (${itemIncluded ? 'include' : 'exclude'} item ${i})`,
            dpTable: [...dp.map(row => [...row])],
            weights,
            values,
            capacity,
            activeItem: i,
            activeWeight: w,
            currentItem: i,
            itemIncluded
          });
        }
      }
    }
    
    // Final result
    steps.push({
      description: `Final result: Maximum value = ${dp[n][capacity]}`,
      dpTable: [...dp.map(row => [...row])],
      weights,
      values,
      capacity,
      activeItem: n,
      activeWeight: capacity,
      currentItem: n,
      itemIncluded: null
    });
    
    return steps;
  };
  
  // Handle visualization playback
  useEffect(() => {
    let timer;
    if (playing && currentStep < maxSteps) {
      timer = setTimeout(() => {
        setCurrentStep(prev => prev + 1);
      }, speed);
    } else if (currentStep >= maxSteps) {
      setPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [playing, currentStep, maxSteps, speed]);
  
  // Playback controls
  const handlePlay = () => setPlaying(true);
  const handlePause = () => setPlaying(false);
  const handleStepForward = () => currentStep < maxSteps && setCurrentStep(prev => prev + 1);
  const handleStepBack = () => currentStep > 0 && setCurrentStep(prev => prev - 1);
  const handleReset = () => {
    setCurrentStep(0);
    setPlaying(false);
  };
  
  // Update algorithm params
  const handleParamChange = (algorithmName, paramName, value) => {
    setParams(prev => ({
      ...prev,
      [algorithmName]: {
        ...prev[algorithmName],
        [paramName]: value
      }
    }));
  };
  
  // Update array parameters
  const handleArrayChange = (algorithmName, paramName, value) => {
    try {
      const array = value.split(',').map(item => parseInt(item.trim(), 10));
      if (array.some(isNaN)) {
        return; // Invalid input
      }
      handleParamChange(algorithmName, paramName, array);
    } catch (e) {
      // Invalid input, ignore
    }
  };
  
  // Render Fibonacci visualization
  const renderFibonacciVisualization = () => {
    if (!steps.length || currentStep >= steps.length) return null;
    
    const step = steps[currentStep];
    const n = params.fibonacci.n;
    
    return (
      <div style={{ marginTop: '20px' }}>
        <h3 style={{ color: colors.primary }}>Fibonacci Recursion Tree</h3>
        <div style={{ marginBottom: '20px' }}>
          {step.description}
        </div>
        
        <div style={{ display: 'flex', flexWrap: 'wrap', gap: '20px' }}>
          {/* Recursion tree */}
          <div style={{ 
            flex: '1', 
            minWidth: '300px', 
            padding: '15px', 
            backgroundColor: 'rgba(0, 0, 0, 0.2)', 
            borderRadius: '8px' 
          }}>
            <div style={{ 
              display: 'flex', 
              justifyContent: 'center',
              marginBottom: '20px'
            }}>
              <svg width="600" height={Math.max(200, n * 35)} style={{ maxWidth: '100%' }}>
                {renderFibonacciTree(n, step.activeNodes, step.completedNodes)}
              </svg>
            </div>
          </div>
          
          {/* Memoization table */}
          {showMemoizationTable && (
            <div style={{ 
              flex: '1', 
              minWidth: '250px', 
              padding: '15px', 
              backgroundColor: 'rgba(0, 0, 0, 0.2)', 
              borderRadius: '8px' 
            }}>
              <h4 style={{ color: colors.secondary }}>Memoization Table</h4>
              <div style={{ 
                display: 'flex', 
                flexWrap: 'wrap', 
                gap: '5px', 
                justifyContent: 'center' 
              }}>
                {step.memoTable.map((value, index) => (
                  <div 
                    key={index}
                    style={{ 
                      width: '40px', 
                      height: '40px', 
                      display: 'flex', 
                      alignItems: 'center', 
                      justifyContent: 'center', 
                      border: `1px solid ${colors.border}`, 
                      borderRadius: '4px',
                      backgroundColor: step.activeNodes.includes(index) 
                        ? 'rgba(255, 255, 0, 0.2)' 
                        : step.completedNodes.includes(index) 
                          ? 'rgba(0, 255, 0, 0.2)' 
                          : 'transparent'
                    }}
                  >
                    <div>
                      <div style={{ fontSize: '12px', opacity: 0.7 }}>fib({index})</div>
                      <div style={{ textAlign: 'center' }}>{value !== null ? value : '?'}</div>
                    </div>
                  </div>
                ))}
              </div>
              
              {step.callStack.length > 0 && (
                <>
                  <h4 style={{ color: colors.secondary, marginTop: '20px' }}>Call Stack</h4>
                  <div style={{ 
                    display: 'flex', 
                    flexDirection: 'column-reverse', 
                    gap: '5px', 
                    border: `1px solid ${colors.border}`, 
                    borderRadius: '4px', 
                    padding: '10px',
                    maxHeight: '150px',
                    overflow: 'auto'
                  }}>
                    {step.callStack.map((call, index) => (
                      <div 
                        key={index}
                        style={{ 
                          padding: '5px', 
                          borderRadius: '4px',
                          backgroundColor: index === step.callStack.length - 1 
                            ? 'rgba(255, 0, 255, 0.2)' 
                            : 'transparent',
                          borderBottom: index !== 0 ? `1px solid ${colors.border}` : 'none'
                        }}
                      >
                        {call}
                      </div>
                    ))}
                  </div>
                </>
              )}
              
              {step.result !== null && (
                <div style={{ 
                  marginTop: '20px', 
                  padding: '10px', 
                  backgroundColor: 'rgba(0, 255, 0, 0.2)', 
                  borderRadius: '4px', 
                  textAlign: 'center', 
                  fontWeight: 'bold' 
                }}>
                  Final Result: {step.result}
                </div>
              )}
            </div>
          )}
        </div>
      </div>
    );
  };
  
  // Render Coin Change visualization
  const renderCoinChangeVisualization = () => {
    if (!steps.length || currentStep >= steps.length) return null;
    
    const step = steps[currentStep];
    const amount = params.coinChange.amount;
    
    return (
      <div style={{ marginTop: '20px' }}>
        <h3 style={{ color: colors.primary }}>Coin Change (Min Coins)</h3>
        <div style={{ marginBottom: '20px' }}>
          {step.description}
        </div>
        
        <div style={{ 
          padding: '15px', 
          backgroundColor: 'rgba(0, 0, 0, 0.2)', 
          borderRadius: '8px',
          overflowX: 'auto'
        }}>
          <div style={{ marginBottom: '15px' }}>
            <strong>Coins:</strong> [{step.coins.join(', ')}]
          </div>
          
          <div style={{ 
            display: 'flex', 
            marginBottom: '10px' 
          }}>
            <div style={{ 
              width: '60px', 
              padding: '8px', 
              textAlign: 'center', 
              fontWeight: 'bold', 
              color: colors.primary 
            }}>
              Amount
            </div>
            
            {step.dpTable.map((_, index) => (
              <div 
                key={index}
                style={{ 
                  width: '60px', 
                  padding: '8px', 
                  textAlign: 'center', 
                  fontWeight: 'bold',
                  color: colors.primary,
                  backgroundColor: step.activeCell === index ? 'rgba(255, 255, 0, 0.2)' : 'transparent'
                }}
              >
                {index}
              </div>
            ))}
          </div>
          
          <div style={{ 
            display: 'flex'
          }}>
            <div style={{ 
              width: '60px', 
              padding: '8px', 
              textAlign: 'center', 
              fontWeight: 'bold', 
              color: colors.primary 
            }}>
              Min Coins
            </div>
            
            {step.dpTable.map((value, index) => (
              <div 
                key={index}
                style={{ 
                  width: '60px', 
                  padding: '8px', 
                  textAlign: 'center', 
                  border: `1px solid ${colors.border}`, 
                  backgroundColor: (() => {
                    if (step.activeCell === index) {
                      return 'rgba(255, 255, 0, 0.2)';
                    }
                    if (step.subCell === index) {
                      return 'rgba(0, 255, 255, 0.2)';
                    }
                    return index <= step.currentAmount ? 'rgba(0, 255, 0, 0.1)' : 'transparent';
                  })()
                }}
              >
                {value === Infinity ? '∞' : value}
              </div>
            ))}
          </div>
          
          {step.activeCoin !== null && (
            <div style={{ 
              marginTop: '15px', 
              padding: '10px', 
              backgroundColor: 'rgba(255, 0, 255, 0.2)', 
              borderRadius: '4px' 
            }}>
              <strong>Current Coin:</strong> {step.activeCoin}
            </div>
          )}
          
          {step.finalResult !== undefined && (
            <div style={{ 
              marginTop: '15px', 
              padding: '10px', 
              backgroundColor: 'rgba(0, 255, 0, 0.2)', 
              borderRadius: '4px', 
              textAlign: 'center', 
              fontWeight: 'bold' 
            }}>
              Final Result: {step.finalResult}
            </div>
          )}
        </div>
      </div>
    );
  };
  
  // Render Climb Stairs visualization
  const renderClimbStairsVisualization = () => {
    if (!steps.length || currentStep >= steps.length) return null;
    
    const step = steps[currentStep];
    const n = params.climbStairs.n;
    
    return (
      <div style={{ marginTop: '20px' }}>
        <h3 style={{ color: colors.primary }}>Climbing Stairs</h3>
        <div style={{ marginBottom: '20px' }}>
          {step.description}
        </div>
        
        <div style={{ 
          display: 'flex', 
          flexWrap: 'wrap', 
          gap: '20px' 
        }}>
          {/* DP Table */}
          <div style={{ 
            flex: '1', 
            minWidth: '300px', 
            padding: '15px', 
            backgroundColor: 'rgba(0, 0, 0, 0.2)', 
            borderRadius: '8px',
            overflowX: 'auto'
          }}>
            <div style={{ display: 'flex', marginBottom: '10px' }}>
              <div style={{ 
                width: '60px', 
                padding: '8px', 
                textAlign: 'center', 
                fontWeight: 'bold', 
                color: colors.primary 
              }}>
                Step
              </div>
              
              {step.dpTable.map((_, index) => (
                <div 
                  key={index}
                  style={{ 
                    width: '60px', 
                    padding: '8px', 
                    textAlign: 'center', 
                    fontWeight: 'bold',
                    color: colors.primary
                  }}
                >
                  {index}
                </div>
              ))}
            </div>
            
            <div style={{ display: 'flex' }}>
              <div style={{ 
                width: '60px', 
                padding: '8px', 
                textAlign: 'center', 
                fontWeight: 'bold', 
                color: colors.primary 
              }}>
                Ways
              </div>
              
              {step.dpTable.map((value, index) => (
                <div 
                  key={index}
                  style={{ 
                    width: '60px', 
                    padding: '8px', 
                    textAlign: 'center', 
                    border: `1px solid ${colors.border}`, 
                    backgroundColor: (() => {
                      if (step.activeStep === index) {
                        return 'rgba(255, 255, 0, 0.2)';
                      }
                      if (step.previousSteps?.includes(index)) {
                        return 'rgba(0, 255, 255, 0.2)';
                      }
                      return index <= step.activeStep ? 'rgba(0, 255, 0, 0.1)' : 'transparent';
                    })()
                  }}
                >
                  {value}
                </div>
              ))}
            </div>
          </div>
          
          {/* Staircase Visualization */}
          <div style={{ 
            flex: '1', 
            minWidth: '300px', 
            padding: '15px', 
            backgroundColor: 'rgba(0, 0, 0, 0.2)', 
            borderRadius: '8px' 
          }}>
            <svg width="100%" height={n * 40 + 50} style={{ maxHeight: '300px' }}>
              {/* Draw stairs */}
              {Array.from({ length: n + 1 }).map((_, i) => (
                <g key={i}>
                  <rect 
                    x={10 + i * 30} 
                    y={50 + (n - i) * 30} 
                    width="30" 
                    height="30" 
                    fill={step.activeStep === i ? colors.highlight : '#333'} 
                    stroke="#555"
                  />
                  <text 
                    x={25 + i * 30} 
                    y={40 + (n - i) * 30} 
                    textAnchor="middle" 
                    fill={colors.text} 
                    fontSize="12"
                  >
                    {i}
                  </text>
                  <text 
                    x={25 + i * 30} 
                    y={70 + (n - i) * 30} 
                    textAnchor="middle" 
                    fill={colors.text} 
                    fontSize="14"
                  >
                    {i <= step.activeStep ? step.dpTable[i] : '?'}
                  </text>
                </g>
              ))}
              
              {/* Draw step paths if active */}
              {step.previousSteps?.map((prevStep, idx) => {
                if (prevStep < 0 || prevStep >= n) return null;
                return (
                  <path 
                    key={idx}
                    d={`M${25 + step.activeStep * 30} ${65 + (n - step.activeStep) * 30} L${25 + prevStep * 30} ${65 + (n - prevStep) * 30}`}
                    stroke={colors.secondary}
                    strokeWidth="2"
                    strokeDasharray="5,5"
                    fill="none"
                  />
                );
              })}
            </svg>
          </div>
        </div>
      </div>
    );
  };
  
  // Render Knapsack visualization
  const renderKnapsackVisualization = () => {
    if (!steps.length || currentStep >= steps.length) return null;
    
    const step = steps[currentStep];
    const { weights, values, capacity } = params.knapsack;
    
    return (
      <div style={{ marginTop: '20px' }}>
        <h3 style={{ color: colors.primary }}>0/1 Knapsack Problem</h3>
        <div style={{ marginBottom: '20px' }}>
          {step.description}
        </div>
        
        <div style={{ 
          display: 'flex', 
          flexWrap: 'wrap', 
          gap: '20px' 
        }}>
          {/* Items Information */}
          <div style={{ 
            flex: '1', 
            minWidth: '200px', 
            padding: '15px', 
            backgroundColor: 'rgba(0, 0, 0, 0.2)', 
            borderRadius: '8px' 
          }}>
            <h4 style={{ color: colors.secondary }}>Items</h4>
            <table style={{ width: '100%', borderCollapse: 'collapse' }}>
              <thead>
                <tr>
                  <th style={{ padding: '8px', textAlign: 'left', borderBottom: `1px solid ${colors.border}` }}>Item</th>
                  <th style={{ padding: '8px', textAlign: 'left', borderBottom: `1px solid ${colors.border}` }}>Weight</th>
                  <th style={{ padding: '8px', textAlign: 'left', borderBottom: `1px solid ${colors.border}` }}>Value</th>
                </tr>
              </thead>
              <tbody>
                {weights.map((weight, index) => (
                  <tr 
                    key={index}
                    style={{ 
                      backgroundColor: step.currentItem - 1 === index 
                        ? 'rgba(255, 255, 0, 0.2)' 
                        : 'transparent' 
                    }}
                  >
                    <td style={{ padding: '8px', borderBottom: `1px solid ${colors.border}` }}>{index + 1}</td>
                    <td style={{ padding: '8px', borderBottom: `1px solid ${colors.border}` }}>{weight}</td>
                    <td style={{ padding: '8px', borderBottom: `1px solid ${colors.border}` }}>{values[index]}</td>
                  </tr>
                ))}
              </tbody>
            </table>
            <div style={{ marginTop: '15px' }}>
              <strong>Knapsack Capacity:</strong> {capacity}
            </div>
            
            {step.itemIncluded !== null && (
              <div style={{ 
                marginTop: '15px', 
                padding: '10px', 
                backgroundColor: step.itemIncluded 
                  ? 'rgba(0, 255, 0, 0.2)' 
                  : 'rgba(255, 0, 0, 0.2)', 
                borderRadius: '4px' 
              }}>
                Decision: {step.itemIncluded ? 'Include' : 'Exclude'} item {step.currentItem}
              </div>
            )}
          </div>
          
          {/* DP Table */}
          <div style={{ 
            flex: '2', 
            minWidth: '300px', 
            padding: '15px', 
            backgroundColor: 'rgba(0, 0, 0, 0.2)', 
            borderRadius: '8px',
            overflowX: 'auto'
          }}>
            <h4 style={{ color: colors.secondary }}>DP Table</h4>
            <div style={{ display: 'flex', marginBottom: '10px' }}>
              <div style={{ 
                width: '60px', 
                padding: '8px', 
                textAlign: 'center', 
                fontWeight: 'bold', 
                color: colors.primary 
              }}>
                Item / W
              </div>
              
              {Array.from({ length: capacity + 1 }).map((_, index) => (
                <div 
                  key={index}
                  style={{ 
                    width: '60px', 
                    padding: '8px', 
                    textAlign: 'center', 
                    fontWeight: 'bold',
                    color: colors.primary,
                    backgroundColor: step.activeWeight === index ? 'rgba(255, 255, 0, 0.1)' : 'transparent'
                  }}
                >
                  {index}
                </div>
              ))}
            </div>
            
            {step.dpTable.map((row, rowIndex) => (
              <div key={rowIndex} style={{ display: 'flex' }}>
                <div style={{ 
                  width: '60px', 
                  padding: '8px', 
                  textAlign: 'center', 
                  fontWeight: 'bold', 
                  color: colors.primary,
                  backgroundColor: step.activeItem === rowIndex ? 'rgba(255, 255, 0, 0.1)' : 'transparent'
                }}>
                  {rowIndex}
                </div>
                
                {row.map((value, colIndex) => (
                  <div 
                    key={colIndex}
                    style={{ 
                      width: '60px', 
                      padding: '8px', 
                      textAlign: 'center', 
                      border: `1px solid ${colors.border}`, 
                      backgroundColor: (() => {
                        if (step.activeItem === rowIndex && step.activeWeight === colIndex) {
                          return 'rgba(255, 255, 0, 0.2)';
                        }
                        if (step.previousStates?.some(state => state.item === rowIndex && state.weight === colIndex)) {
                          return 'rgba(0, 255, 255, 0.2)';
                        }
                        return 'transparent';
                      })()
                    }}
                  >
                    {value}
                  </div>
                ))}
              </div>
            ))}
          </div>
        </div>
      </div>
    );
  };
  
  // Helper function to recursively render Fibonacci tree
  const renderFibonacciTree = (n, activeNodes, completedNodes, x = 300, y = 50, depth = 0) => {
    const nodeSize = 30;
    const vertSpace = 60;
    const horizSpace = Math.max(15, 150 / (depth + 1));
    
    if (n <= 1) {
      return (
        <g key={`node-${n}-${x}-${y}`}>
          <circle 
            cx={x} 
            cy={y} 
            r={nodeSize / 2} 
            fill={activeNodes.includes(n) ? colors.highlight : completedNodes.includes(n) ? colors.success : colors.primary} 
            strokeWidth="2" 
            stroke={colors.border}
          />
          <text 
            x={x} 
            y={y} 
            textAnchor="middle" 
            dominantBaseline="middle" 
            fill={colors.text} 
            fontSize="14"
          >
            {n}
          </text>
        </g>
      );
    }
    
    // Current node
    const elements = [
      <g key={`node-${n}-${x}-${y}`}>
        <circle 
          cx={x} 
          cy={y} 
          r={nodeSize / 2} 
          fill={activeNodes.includes(n) ? colors.highlight : completedNodes.includes(n) ? colors.success : colors.primary} 
          strokeWidth="2" 
          stroke={colors.border}
        />
        <text 
          x={x} 
          y={y} 
          textAnchor="middle" 
          dominantBaseline="middle" 
          fill={colors.text} 
          fontSize="14"
        >
          {n}
        </text>
      </g>
    ];
    
    // Only render children if this node is active or has been completed
    if (activeNodes.includes(n) || completedNodes.includes(n)) {
      // Left child: fib(n-1)
      const leftX = x - horizSpace * Math.pow(1.5, Math.min(3, n/2));
      const leftY = y + vertSpace;
      
      elements.push(
        <line 
          key={`edge-${n}-${n-1}`} 
          x1={x} 
          y1={y + nodeSize / 2} 
          x2={leftX} 
          y2={leftY - nodeSize / 2} 
          stroke={colors.secondary} 
          strokeWidth="2"
        />
      );
      
      elements.push(...renderFibonacciTree(n - 1, activeNodes, completedNodes, leftX, leftY, depth + 1));
      
      // Right child: fib(n-2)
      const rightX = x + horizSpace * Math.pow(1.5, Math.min(3, n/2));
      const rightY = y + vertSpace;
      
      elements.push(
        <line 
          key={`edge-${n}-${n-2}`} 
          x1={x} 
          y1={y + nodeSize / 2} 
          x2={rightX} 
          y2={rightY - nodeSize / 2} 
          stroke={colors.secondary} 
          strokeWidth="2"
        />
      );
      
      elements.push(...renderFibonacciTree(n - 2, activeNodes, completedNodes, rightX, rightY, depth + 1));
    }
    
    return elements;
  };
  
  // Render algorithm code
  const getAlgorithmCode = () => {
    switch(algorithm) {
      case 'fibonacci':
        return `// Top-down memoization approach
int fibMemo(int n, vector<int>& memo) {
  // Base cases
  if (n <= 1) return n;
  
  // If already computed, return stored result
  if (memo[n] != -1) return memo[n];
  
  // Compute and store result
  memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
  return memo[n];
}

// Bottom-up tabulation approach
int fibTabulation(int n) {
  if (n <= 1) return n;
  
  vector<int> dp(n+1);
  dp[0] = 0;
  dp[1] = 1;
  
  for (int i = 2; i <= n; i++) {
    dp[i] = dp[i-1] + dp[i-2];
  }
  
  return dp[n];
}`;

      case 'coinChange':
        return `// Minimum coins needed to make amount
int coinChange(vector<int>& coins, int amount) {
  // Initialize dp array with a value larger than any valid answer
  vector<int> dp(amount + 1, amount + 1);
  
  // Base case: 0 coins needed to make amount 0
  dp[0] = 0;
  
  // Fill the dp array bottom-up
  for (int i = 1; i <= amount; i++) {
    // Try each coin denomination
    for (int coin : coins) {
      if (coin <= i) {
        dp[i] = min(dp[i], dp[i - coin] + 1);
      }
    }
  }
  
  // If dp[amount] is still amount+1, it means no solution exists
  return dp[amount] > amount ? -1 : dp[amount];
}`;

      case 'climbStairs':
        return `// Number of distinct ways to climb stairs
int climbStairs(int n) {
  if (n <= 2) return n;
  
  vector<int> dp(n+1);
  dp[0] = 1;  // One way to climb 0 steps (base case)
  dp[1] = 1;  // One way to climb 1 step
  
  for (int i = 2; i <= n; i++) {
    dp[i] = dp[i-1] + dp[i-2];
  }
  
  return dp[n];
}

// Space-optimized version
int climbStairsOptimized(int n) {
  if (n <= 2) return n;
  
  int prev = 1;  // ways to climb 1 step
  int curr = 2;  // ways to climb 2 steps
  
  for (int i = 3; i <= n; i++) {
    int next = prev + curr;
    prev = curr;
    curr = next;
  }
  
  return curr;
}`;

      case 'knapsack':
        return `// 0/1 Knapsack problem (maximize value within weight constraint)
int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
  int n = weights.size();
  // dp[i][w] = max value that can be obtained using first i items and weight limit w
  vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
  
  for (int i = 1; i <= n; i++) {
    for (int w = 0; w <= capacity; w++) {
      // If current item is too heavy, skip it
      if (weights[i-1] > w) {
        dp[i][w] = dp[i-1][w];  // Take value without this item
      } else {
        // Max of: (1) exclude this item, (2) include this item
        dp[i][w] = max(
          dp[i-1][w],
          values[i-1] + dp[i-1][w - weights[i-1]]
        );
      }
    }
  }
  
  return dp[n][capacity];
}

// Space-optimized version
int knapsackOptimized(vector<int>& weights, vector<int>& values, int capacity) {
  int n = weights.size();
  vector<int> dp(capacity + 1, 0);
  
  for (int i = 0; i < n; i++) {
    for (int w = capacity; w >= weights[i]; w--) {
      dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
    }
  }
  
  return dp[capacity];
}`;

      default:
        return '';
    }
  };
  
  // Render visualization based on selected algorithm
  const renderVisualization = () => {
    switch(algorithm) {
      case 'fibonacci':
        return renderFibonacciVisualization();
      case 'coinChange':
        return renderCoinChangeVisualization();
      case 'climbStairs':
        return renderClimbStairsVisualization();
      case 'knapsack':
        return renderKnapsackVisualization();
      default:
        return null;
    }
  };
  
  // Render algorithm parameters input
  const renderAlgorithmParams = () => {
    switch(algorithm) {
      case 'fibonacci':
        return (
          <div style={{ marginTop: '10px' }}>
            <label style={{ marginRight: '10px' }}>
              n:
              <input 
                type="number" 
                min="1" 
                max="10" 
                value={params.fibonacci.n} 
                onChange={e => handleParamChange('fibonacci', 'n', parseInt(e.target.value, 10))}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px' 
                }}
              />
            </label>
          </div>
        );
        
      case 'coinChange':
        return (
          <div style={{ marginTop: '10px', display: 'flex', flexWrap: 'wrap', gap: '10px' }}>
            <label>
              Coins:
              <input 
                type="text" 
                value={params.coinChange.coins.join(', ')} 
                onChange={e => handleArrayChange('coinChange', 'coins', e.target.value)}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px',
                  width: '120px'
                }}
              />
            </label>
            <label>
              Amount:
              <input 
                type="number" 
                min="1" 
                max="20" 
                value={params.coinChange.amount} 
                onChange={e => handleParamChange('coinChange', 'amount', parseInt(e.target.value, 10))}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px',
                  width: '60px'
                }}
              />
            </label>
          </div>
        );
        
      case 'climbStairs':
        return (
          <div style={{ marginTop: '10px' }}>
            <label style={{ marginRight: '10px' }}>
              Stairs:
              <input 
                type="number" 
                min="1" 
                max="10" 
                value={params.climbStairs.n} 
                onChange={e => handleParamChange('climbStairs', 'n', parseInt(e.target.value, 10))}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px' 
                }}
              />
            </label>
          </div>
        );
        
      case 'knapsack':
        return (
          <div style={{ marginTop: '10px', display: 'flex', flexWrap: 'wrap', gap: '10px' }}>
            <label>
              Weights:
              <input 
                type="text" 
                value={params.knapsack.weights.join(', ')} 
                onChange={e => handleArrayChange('knapsack', 'weights', e.target.value)}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px',
                  width: '120px'
                }}
              />
            </label>
            <label>
              Values:
              <input 
                type="text" 
                value={params.knapsack.values.join(', ')} 
                onChange={e => handleArrayChange('knapsack', 'values', e.target.value)}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px',
                  width: '120px'
                }}
              />
            </label>
            <label>
              Capacity:
              <input 
                type="number" 
                min="1" 
                max="20" 
                value={params.knapsack.capacity} 
                onChange={e => handleParamChange('knapsack', 'capacity', parseInt(e.target.value, 10))}
                style={{ 
                  marginLeft: '5px', 
                  backgroundColor: colors.background, 
                  color: colors.text, 
                  border: `1px solid ${colors.border}`, 
                  borderRadius: '4px', 
                  padding: '5px',
                  width: '60px'
                }}
              />
            </label>
          </div>
        );
        
      default:
        return null;
    }
  };
  
  return (
    <div style={{ 
      fontFamily: 'Arial, sans-serif', 
      backgroundColor: colors.background, 
      color: colors.text, 
      padding: '20px', 
      borderRadius: '8px', 
      maxWidth: '100%' 
    }}>
      <h1 style={{ color: colors.primary, textAlign: 'center' }}>Dynamic Programming Visualization</h1>
      
      {/* Algorithm selection */}
      <div style={{ marginBottom: '20px' }}>
        <div style={{ marginBottom: '10px' }}>
          <label style={{ marginRight: '10px' }}>
            Algorithm:
            <select 
              value={algorithm} 
              onChange={e => setAlgorithm(e.target.value)}
              style={{ 
                marginLeft: '10px', 
                backgroundColor: colors.background, 
                color: colors.text, 
                border: `1px solid ${colors.border}`, 
                borderRadius: '4px', 
                padding: '5px' 
              }}
            >
              <option value="fibonacci">Fibonacci</option>
              <option value="coinChange">Coin Change</option>
              <option value="climbStairs">Climbing Stairs</option>
              <option value="knapsack">0/1 Knapsack</option>
            </select>
          </label>
          
          <button 
            onClick={() => setShowCode(!showCode)}
            style={{ 
              marginLeft: '10px', 
              backgroundColor: showCode ? colors.primary : colors.background, 
              color: showCode ? colors.background : colors.primary, 
              border: `1px solid ${colors.primary}`, 
              borderRadius: '4px', 
              padding: '5px 10px', 
              cursor: 'pointer' 
            }}
          >
            {showCode ? 'Hide Code' : 'Show Code'}
          </button>
          
          {algorithm === 'fibonacci' && (
            <button 
              onClick={() => setShowMemoizationTable(!showMemoizationTable)}
              style={{ 
                marginLeft: '10px', 
                backgroundColor: showMemoizationTable ? colors.primary : colors.background, 
                color: showMemoizationTable ? colors.background : colors.primary, 
                border: `1px solid ${colors.primary}`, 
                borderRadius: '4px', 
                padding: '5px 10px', 
                cursor: 'pointer' 
              }}
            >
              {showMemoizationTable ? 'Hide Memo Table' : 'Show Memo Table'}
            </button>
          )}
        </div>
        
        {renderAlgorithmParams()}
      </div>
      
      {/* Code Display */}
      {showCode && (
        <div style={{ 
          backgroundColor: '#0D0D0D', 
          padding: '15px', 
          borderRadius: '8px', 
          marginBottom: '20px',
          overflowX: 'auto'
        }}>
          <pre style={{ 
            margin: 0, 
            color: colors.text, 
            fontFamily: 'monospace', 
            fontSize: '14px' 
          }}>
            {getAlgorithmCode()}
          </pre>
        </div>
      )}
      
      {/* Complexity Information */}
      <div style={{ 
        marginBottom: '20px', 
        padding: '15px', 
        backgroundColor: 'rgba(0, 0, 0, 0.2)', 
        borderRadius: '8px' 
      }}>
        <h3 style={{ color: colors.secondary, marginTop: 0 }}>Complexity Analysis</h3>
        {algorithm === 'fibonacci' && (
          <div>
            <p><strong>Naive Recursive:</strong> O(2ⁿ) time, O(n) space</p>
            <p><strong>Memoization:</strong> O(n) time, O(n) space</p>
            <p><strong>Tabulation:</strong> O(n) time, O(n) space</p>
            <p><strong>Space-Optimized:</strong> O(n) time, O(1) space</p>
          </div>
        )}
        {algorithm === 'coinChange' && (
          <div>
            <p><strong>Naive Recursive:</strong> O(amount^coins.size()) time, O(amount) space</p>
            <p><strong>Memoization:</strong> O(amount * coins.size()) time, O(amount) space</p>
            <p><strong>Tabulation:</strong> O(amount * coins.size()) time, O(amount) space</p>
          </div>
        )}
        {algorithm === 'climbStairs' && (
          <div>
            <p><strong>Naive Recursive:</strong> O(2ⁿ) time, O(n) space</p>
            <p><strong>Memoization:</strong> O(n) time, O(n) space</p>
            <p><strong>Tabulation:</strong> O(n) time, O(n) space</p>
            <p><strong>Space-Optimized:</strong> O(n) time, O(1) space</p>
          </div>
        )}
        {algorithm === 'knapsack' && (
          <div>
            <p><strong>Naive Recursive:</strong> O(2ⁿ) time, O(n) space</p>
            <p><strong>Memoization:</strong> O(n * capacity) time, O(n * capacity) space</p>
            <p><strong>Tabulation:</strong> O(n * capacity) time, O(n * capacity) space</p>
            <p><strong>Space-Optimized:</strong> O(n * capacity) time, O(capacity) space</p>
          </div>
        )}
      </div>
      
      {/* Visualization area */}
      <div style={{ marginBottom: '20px' }}>
        {renderVisualization()}
      </div>
      
      {/* Playback controls */}
      <div style={{ 
        display: 'flex', 
        justifyContent: 'center', 
        gap: '10px', 
        marginBottom: '20px',
        flexWrap: 'wrap'
      }}>
        <button 
          onClick={handleReset}
          style={{ 
            backgroundColor: colors.background, 
            color: colors.primary, 
            border: `1px solid ${colors.primary}`, 
            borderRadius: '4px', 
            padding: '8px 15px', 
            cursor: 'pointer' 
          }}
        >
          Reset
        </button>
        <button 
          onClick={handleStepBack}
          disabled={currentStep === 0}
          style={{ 
            backgroundColor: colors.background, 
            color: colors.primary, 
            border: `1px solid ${colors.primary}`, 
            borderRadius: '4px', 
            padding: '8px 15px', 
            cursor: currentStep === 0 ? 'not-allowed' : 'pointer',
            opacity: currentStep === 0 ? 0.5 : 1 
          }}
        >
          ← Step
        </button>
        <button 
          onClick={playing ? handlePause : handlePlay}
          disabled={currentStep === maxSteps}
          style={{ 
            backgroundColor: playing ? colors.background : colors.primary, 
            color: playing ? colors.primary : colors.background, 
            border: `1px solid ${colors.primary}`, 
            borderRadius: '4px', 
            padding: '8px 15px', 
            cursor: currentStep === maxSteps ? 'not-allowed' : 'pointer',
            opacity: currentStep === maxSteps ? 0.5 : 1 
          }}
        >
          {playing ? 'Pause' : 'Play'}
        </button>
        <button 
          onClick={handleStepForward}
          disabled={currentStep === maxSteps}
          style={{ 
            backgroundColor: colors.background, 
            color: colors.primary, 
            border: `1px solid ${colors.primary}`, 
            borderRadius: '4px', 
            padding: '8px 15px', 
            cursor: currentStep === maxSteps ? 'not-allowed' : 'pointer',
            opacity: currentStep === maxSteps ? 0.5 : 1 
          }}
        >
          Step →
        </button>
        <select 
          value={speed} 
          onChange={e => setSpeed(parseInt(e.target.value, 10))}
          style={{ 
            backgroundColor: colors.background, 
            color: colors.text, 
            border: `1px solid ${colors.border}`, 
            borderRadius: '4px', 
            padding: '8px 15px', 
            cursor: 'pointer' 
          }}
        >
          <option value={2000}>Slow</option>
          <option value={1000}>Medium</option>
          <option value={500}>Fast</option>
          <option value={200}>Very Fast</option>
        </select>
      </div>
      
      {/* Step indicator */}
      <div style={{ 
        display: 'flex', 
        justifyContent: 'center', 
        gap: '10px',
        marginBottom: '20px',
        padding: '10px',
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        borderRadius: '8px'
      }}>
        <div>Step: {currentStep + 1} / {maxSteps + 1}</div>
        <div>
          <input 
            type="range" 
            min="0" 
            max={maxSteps} 
            value={currentStep}
            onChange={e => setCurrentStep(parseInt(e.target.value, 10))}
            style={{ width: '200px' }}
          />
        </div>
      </div>
      
      {/* Info box */}
      <div style={{
        padding: '15px',
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        borderRadius: '8px'
      }}>
        <h3 style={{ color: colors.primary, marginTop: 0 }}>About Dynamic Programming</h3>
        <p>
          Dynamic Programming (DP) is a method for solving complex problems by breaking them down into simpler subproblems. 
          It is applicable when the problem has <strong style={{ color: colors.secondary }}>overlapping subproblems</strong> and 
          <strong style={{ color: colors.secondary }}> optimal substructure</strong>.
        </p>
        
        <ul style={{ paddingLeft: '20px' }}>
          <li><strong style={{ color: colors.highlight }}>Overlapping Subproblems:</strong> Same subproblems are solved multiple times.</li>
          <li><strong style={{ color: colors.highlight }}>Optimal Substructure:</strong> Optimal solution to the problem contains optimal solutions to its subproblems.</li>
        </ul>
        
        <p>
          There are two main approaches to implement dynamic programming:
        </p>
        
        <ul style={{ paddingLeft: '20px' }}>
          <li><strong style={{ color: colors.success }}>Top-Down (Memoization):</strong> Recursive approach with a "memory" to avoid redundant calculations.</li>
          <li><strong style={{ color: colors.success }}>Bottom-Up (Tabulation):</strong> Iterative approach that builds solutions for all subproblems starting from the smallest.</li>
        </ul>
        
        <p>
          This visualization demonstrates both approaches for classic DP problems. Use the controls to step through the algorithm execution and observe how solutions to subproblems are reused to build the final solution.
        </p>
      </div>
    </div>
  );
};

export default DynamicProgrammingVisualization;