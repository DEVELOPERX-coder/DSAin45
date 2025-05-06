import React, { useState, useEffect } from 'react';

const DynamicProgrammingVisualization = () => {
  // State for algorithm selection
  const [algorithm, setAlgorithm] = useState('knapsack');
  
  // State for parameters
  const [params, setParams] = useState({
    knapsack: {
      weights: [1, 3, 4, 5],
      values: [1, 4, 5, 7],
      capacity: 7
    },
    lcs: {
      str1: 'ABCBDAB',
      str2: 'BDCABA'
    },
    editDistance: {
      str1: 'intention',
      str2: 'execution'
    },
    matrixChain: {
      dimensions: [40, 20, 30, 10, 30]
    }
  });
  
  // State for visualization
  const [step, setStep] = useState(0);
  const [maxSteps, setMaxSteps] = useState(0);
  const [steps, setSteps] = useState([]);
  const [isPlaying, setIsPlaying] = useState(false);
  const [playbackSpeed, setPlaybackSpeed] = useState(1000);
  const [showComplexityTable, setShowComplexityTable] = useState(true);
  
  // Colors (using neon on dark theme as required)
  const colors = {
    background: '#121212',
    primary: '#00FFFF',   // Cyan
    secondary: '#FF00FF', // Pink
    highlight: '#FFFF00', // Yellow
    success: '#00FF00',   // Green
    text: '#FFFFFF',
    border: '#444444'
  };
  
  // Generate steps for Knapsack visualization
  const generateKnapsackSteps = () => {
    const { weights, values, capacity } = params.knapsack;
    
    const steps = [];
    const n = weights.length;
    
    // Initial state - create empty DP table
    const dpTable = Array(n + 1).fill().map(() => Array(capacity + 1).fill(0));
    
    steps.push({
      description: 'Initialize a DP table with rows representing items and columns representing capacity',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: []
    });
    
    // Fill the DP table
    for (let i = 1; i <= n; i++) {
      steps.push({
        description: `Considering item ${i} (Weight: ${weights[i-1]}, Value: ${values[i-1]})`,
        dpTable: JSON.parse(JSON.stringify(dpTable)),
        activeCell: null,
        activeRow: i,
        activeCol: null,
        highlightCells: []
      });
      
      for (let w = 0; w <= capacity; w++) {
        if (w === 0) {
          // Base case: with 0 capacity, value is 0
          dpTable[i][w] = 0;
          steps.push({
            description: `Base case: with 0 capacity, maximum value is 0`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: w },
            activeRow: i,
            activeCol: w,
            highlightCells: []
          });
          continue;
        }
        
        // If item is too heavy to fit
        if (weights[i-1] > w) {
          dpTable[i][w] = dpTable[i-1][w];
          steps.push({
            description: `Item ${i} is too heavy (weight ${weights[i-1]} > capacity ${w}), so we take the value from excluding this item: ${dpTable[i-1][w]}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: w },
            activeRow: i,
            activeCol: w,
            highlightCells: [{ row: i-1, col: w }]
          });
        } else {
          // Decide whether to include or exclude the item
          const includeValue = values[i-1] + dpTable[i-1][w - weights[i-1]];
          const excludeValue = dpTable[i-1][w];
          
          steps.push({
            description: `For item ${i} and capacity ${w}, we can either:
              1. Include it: Value ${values[i-1]} + dp[${i-1}][${w - weights[i-1]}] = ${values[i-1]} + ${dpTable[i-1][w - weights[i-1]]} = ${includeValue}
              2. Exclude it: dp[${i-1}][${w}] = ${excludeValue}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: w },
            activeRow: i,
            activeCol: w,
            highlightCells: [
              { row: i-1, col: w - weights[i-1] },
              { row: i-1, col: w }
            ]
          });
          
          dpTable[i][w] = Math.max(includeValue, excludeValue);
          
          steps.push({
            description: `We choose the maximum: ${Math.max(includeValue, excludeValue)}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: w },
            activeRow: i,
            activeCol: w,
            highlightCells: [
              includeValue > excludeValue 
                ? { row: i-1, col: w - weights[i-1] } 
                : { row: i-1, col: w }
            ]
          });
        }
      }
    }
    
    // Final result
    steps.push({
      description: `Final result: The maximum value we can achieve with capacity ${capacity} is ${dpTable[n][capacity]}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: { row: n, col: capacity },
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      isFinal: true
    });
    
    // Backtrack to find selected items
    let w = capacity;
    const selectedItems = [];
    
    for (let i = n; i > 0; i--) {
      if (dpTable[i][w] !== dpTable[i-1][w]) {
        selectedItems.push(i-1); // Using 0-indexed for items
        w -= weights[i-1];
        
        steps.push({
          description: `Backtracking: Item ${i} (Weight: ${weights[i-1]}, Value: ${values[i-1]}) is included in the optimal solution`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: w + weights[i-1] },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: w }],
          selectedItems: [...selectedItems],
          isFinal: true
        });
      } else {
        steps.push({
          description: `Backtracking: Item ${i} is not included in the optimal solution`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: w },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: w }],
          selectedItems: [...selectedItems],
          isFinal: true
        });
      }
    }
    
    steps.push({
      description: `Final solution: Select items ${selectedItems.map(i => i+1).join(', ')} for a total value of ${dpTable[n][capacity]} and weight ${selectedItems.reduce((sum, i) => sum + weights[i], 0)}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      selectedItems: [...selectedItems],
      isFinal: true
    });
    
    return steps;
  };
  
  // Generate steps for LCS visualization
  const generateLCSSteps = () => {
    const { str1, str2 } = params.lcs;
    
    const steps = [];
    const m = str1.length;
    const n = str2.length;
    
    // Initial state - create empty DP table
    const dpTable = Array(m + 1).fill().map(() => Array(n + 1).fill(0));
    
    steps.push({
      description: 'Initialize a DP table with rows representing characters of first string and columns representing characters of second string',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: []
    });
    
    // Base cases
    steps.push({
      description: 'Base case: LCS of an empty string with any string is 0',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: 0,
      activeCol: null,
      highlightCells: []
    });
    
    // Fill the DP table
    for (let i = 1; i <= m; i++) {
      for (let j = 1; j <= n; j++) {
        steps.push({
          description: `Comparing characters: '${str1[i-1]}' and '${str2[j-1]}'`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [],
          charactersCompared: [
            { string: 1, index: i-1 },
            { string: 2, index: j-1 }
          ]
        });
        
        if (str1[i-1] === str2[j-1]) {
          dpTable[i][j] = dpTable[i-1][j-1] + 1;
          steps.push({
            description: `Characters match! Add 1 to the diagonal value: dp[${i-1}][${j-1}] + 1 = ${dpTable[i-1][j-1]} + 1 = ${dpTable[i][j]}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [{ row: i-1, col: j-1 }],
            charactersCompared: [
              { string: 1, index: i-1 },
              { string: 2, index: j-1 }
            ]
          });
        } else {
          dpTable[i][j] = Math.max(dpTable[i-1][j], dpTable[i][j-1]);
          steps.push({
            description: `Characters don't match. Take maximum of left (${dpTable[i][j-1]}) and top (${dpTable[i-1][j]}) values: ${dpTable[i][j]}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [
              { row: i-1, col: j },
              { row: i, col: j-1 }
            ],
            charactersCompared: [
              { string: 1, index: i-1 },
              { string: 2, index: j-1 }
            ]
          });
        }
      }
    }
    
    // Final result
    steps.push({
      description: `Final result: The length of the longest common subsequence is ${dpTable[m][n]}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: { row: m, col: n },
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      isFinal: true
    });
    
    // Backtrack to find the LCS
    let i = m, j = n;
    const lcs = [];
    
    while (i > 0 && j > 0) {
      if (str1[i-1] === str2[j-1]) {
        lcs.unshift(str1[i-1]);
        steps.push({
          description: `Backtracking: Characters '${str1[i-1]}' and '${str2[j-1]}' match, so '${str1[i-1]}' is part of the LCS`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: j-1 }],
          lcs: [...lcs],
          isFinal: true
        });
        i--; j--;
      } else if (dpTable[i-1][j] > dpTable[i][j-1]) {
        steps.push({
          description: `Backtracking: Move up because dp[${i-1}][${j}] > dp[${i}][${j-1}]`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: j }],
          lcs: [...lcs],
          isFinal: true
        });
        i--;
      } else {
        steps.push({
          description: `Backtracking: Move left because dp[${i-1}][${j}] <= dp[${i}][${j-1}]`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i, col: j-1 }],
          lcs: [...lcs],
          isFinal: true
        });
        j--;
      }
    }
    
    steps.push({
      description: `Final solution: The longest common subsequence is '${lcs.join('')}'`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      lcs: [...lcs],
      isFinal: true
    });
    
    return steps;
  };
  
  // Generate steps for Edit Distance visualization
  const generateEditDistanceSteps = () => {
    const { str1, str2 } = params.editDistance;
    
    const steps = [];
    const m = str1.length;
    const n = str2.length;
    
    // Initial state - create empty DP table
    const dpTable = Array(m + 1).fill().map(() => Array(n + 1).fill(0));
    
    steps.push({
      description: 'Initialize a DP table where dp[i][j] represents the minimum number of operations to convert the first i characters of str1 to the first j characters of str2',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: []
    });
    
    // Base cases
    for (let i = 0; i <= m; i++) {
      dpTable[i][0] = i;
    }
    
    steps.push({
      description: 'Base case: Converting a string to an empty string requires deleting all characters',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: 0,
      highlightCells: []
    });
    
    for (let j = 0; j <= n; j++) {
      dpTable[0][j] = j;
    }
    
    steps.push({
      description: 'Base case: Converting an empty string to a string requires inserting all characters',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: 0,
      activeCol: null,
      highlightCells: []
    });
    
    // Fill the DP table
    for (let i = 1; i <= m; i++) {
      for (let j = 1; j <= n; j++) {
        steps.push({
          description: `Comparing characters: '${str1[i-1]}' and '${str2[j-1]}'`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [],
          charactersCompared: [
            { string: 1, index: i-1 },
            { string: 2, index: j-1 }
          ]
        });
        
        if (str1[i-1] === str2[j-1]) {
          dpTable[i][j] = dpTable[i-1][j-1];
          steps.push({
            description: `Characters match! No operation needed, take the diagonal value: dp[${i-1}][${j-1}] = ${dpTable[i-1][j-1]}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [{ row: i-1, col: j-1 }],
            charactersCompared: [
              { string: 1, index: i-1 },
              { string: 2, index: j-1 }
            ]
          });
        } else {
          // Min of three operations: insert, delete, replace
          const insert = dpTable[i][j-1];
          const del = dpTable[i-1][j];
          const replace = dpTable[i-1][j-1];
          
          steps.push({
            description: `Characters don't match. Consider three operations:
              1. Insert '${str2[j-1]}': dp[${i}][${j-1}] + 1 = ${insert} + 1 = ${insert + 1}
              2. Delete '${str1[i-1]}': dp[${i-1}][${j}] + 1 = ${del} + 1 = ${del + 1}
              3. Replace '${str1[i-1]}' with '${str2[j-1]}': dp[${i-1}][${j-1}] + 1 = ${replace} + 1 = ${replace + 1}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [
              { row: i, col: j-1 },
              { row: i-1, col: j },
              { row: i-1, col: j-1 }
            ],
            charactersCompared: [
              { string: 1, index: i-1 },
              { string: 2, index: j-1 }
            ]
          });
          
          dpTable[i][j] = 1 + Math.min(insert, del, replace);
          
          let minOp = "replace";
          let minCell = { row: i-1, col: j-1 };
          
          if (insert <= del && insert <= replace) {
            minOp = "insert";
            minCell = { row: i, col: j-1 };
          } else if (del <= insert && del <= replace) {
            minOp = "delete";
            minCell = { row: i-1, col: j };
          }
          
          steps.push({
            description: `Choose the minimum: ${minOp} with value ${dpTable[i][j]}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [minCell],
            charactersCompared: [
              { string: 1, index: i-1 },
              { string: 2, index: j-1 }
            ]
          });
        }
      }
    }
    
    // Final result
    steps.push({
      description: `Final result: The minimum edit distance between '${str1}' and '${str2}' is ${dpTable[m][n]}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: { row: m, col: n },
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      isFinal: true
    });
    
    // Backtrack to find the operations
    let i = m, j = n;
    const operations = [];
    
    while (i > 0 || j > 0) {
      if (i > 0 && j > 0 && str1[i-1] === str2[j-1]) {
        operations.unshift(`Keep '${str1[i-1]}'`);
        steps.push({
          description: `Backtracking: Characters match, no operation needed`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: j-1 }],
          operations: [...operations],
          isFinal: true
        });
        i--; j--;
      } else if (i > 0 && j > 0 && dpTable[i][j] === dpTable[i-1][j-1] + 1) {
        operations.unshift(`Replace '${str1[i-1]}' with '${str2[j-1]}'`);
        steps.push({
          description: `Backtracking: Replace '${str1[i-1]}' with '${str2[j-1]}'`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: j-1 }],
          operations: [...operations],
          isFinal: true
        });
        i--; j--;
      } else if (j > 0 && dpTable[i][j] === dpTable[i][j-1] + 1) {
        operations.unshift(`Insert '${str2[j-1]}'`);
        steps.push({
          description: `Backtracking: Insert '${str2[j-1]}'`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i, col: j-1 }],
          operations: [...operations],
          isFinal: true
        });
        j--;
      } else {
        operations.unshift(`Delete '${str1[i-1]}'`);
        steps.push({
          description: `Backtracking: Delete '${str1[i-1]}'`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [{ row: i-1, col: j }],
          operations: [...operations],
          isFinal: true
        });
        i--;
      }
    }
    
    steps.push({
      description: `Final solution: The operations to convert '${str1}' to '${str2}' are: ${operations.join(', ')}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      operations: [...operations],
      isFinal: true
    });
    
    return steps;
  };
  
  // Generate steps for Matrix Chain Multiplication visualization
  const generateMatrixChainSteps = () => {
    const { dimensions } = params.matrixChain;
    
    const steps = [];
    const n = dimensions.length - 1; // Number of matrices
    
    // Generate matrices info for visualization
    const matrices = [];
    for (let i = 0; i < n; i++) {
      matrices.push({
        name: `M${i+1}`,
        rows: dimensions[i],
        cols: dimensions[i+1]
      });
    }
    
    // Initial state - create empty DP table
    const dpTable = Array(n).fill().map(() => Array(n).fill(0));
    const splitTable = Array(n).fill().map(() => Array(n).fill(0));
    
    steps.push({
      description: 'Initialize a table where dp[i][j] represents the minimum number of scalar multiplications needed to compute the matrix product M[i]...M[j]',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      matrices,
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: []
    });
    
    // Base case: single matrices have 0 cost
    for (let i = 0; i < n; i++) {
      dpTable[i][i] = 0;
    }
    
    steps.push({
      description: 'Base case: Multiplying a single matrix requires 0 scalar multiplications',
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      matrices,
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: []
    });
    
    // Fill the table for chains of increasing length
    for (let len = 2; len <= n; len++) {
      steps.push({
        description: `Computing optimal multiplication cost for chains of length ${len}`,
        dpTable: JSON.parse(JSON.stringify(dpTable)),
        matrices,
        activeCell: null,
        activeRow: null,
        activeCol: null,
        highlightCells: []
      });
      
      for (let i = 0; i <= n - len; i++) {
        const j = i + len - 1;
        dpTable[i][j] = Number.MAX_SAFE_INTEGER;
        
        steps.push({
          description: `Considering chain from matrix M${i+1} to M${j+1}`,
          dpTable: JSON.parse(JSON.stringify(dpTable)),
          matrices,
          activeCell: { row: i, col: j },
          activeRow: null,
          activeCol: null,
          highlightCells: [],
          chainIndices: [i, j]
        });
        
        // Try different split positions
        for (let k = i; k < j; k++) {
          const costLeft = dpTable[i][k];
          const costRight = dpTable[k+1][j];
          const costMult = dimensions[i] * dimensions[k+1] * dimensions[j+1];
          const cost = costLeft + costRight + costMult;
          
          steps.push({
            description: `Trying split at k=${k} (between M${k+1} and M${k+2}):
              Left part: M${i+1}...M${k+1} with cost ${costLeft}
              Right part: M${k+2}...M${j+1} with cost ${costRight}
              Multiplication cost: ${dimensions[i]} × ${dimensions[k+1]} × ${dimensions[j+1]} = ${costMult}
              Total cost: ${costLeft} + ${costRight} + ${costMult} = ${cost}`,
            dpTable: JSON.parse(JSON.stringify(dpTable)),
            matrices,
            activeCell: { row: i, col: j },
            activeRow: null,
            activeCol: null,
            highlightCells: [
              { row: i, col: k },
              { row: k+1, col: j }
            ],
            chainIndices: [i, j],
            splitIndex: k
          });
          
          if (cost < dpTable[i][j]) {
            dpTable[i][j] = cost;
            splitTable[i][j] = k;
            
            steps.push({
              description: `Found better cost: ${cost}. Updated dp[${i}][${j}] and recorded split position k=${k}`,
              dpTable: JSON.parse(JSON.stringify(dpTable)),
              matrices,
              activeCell: { row: i, col: j },
              activeRow: null,
              activeCol: null,
              highlightCells: [
                { row: i, col: k },
                { row: k+1, col: j }
              ],
              chainIndices: [i, j],
              splitIndex: k
            });
          }
        }
      }
    }
    
    // Final result
    steps.push({
      description: `Final result: The minimum number of scalar multiplications needed is ${dpTable[0][n-1]}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      matrices,
      activeCell: { row: 0, col: n-1 },
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      isFinal: true
    });
    
    // Construct the optimal parenthesization
    const getParenthesization = (i, j, splitTable, steps) => {
      if (i === j) {
        return `M${i+1}`;
      }
      
      const k = splitTable[i][j];
      const left = getParenthesization(i, k, splitTable, steps);
      const right = getParenthesization(k+1, j, splitTable, steps);
      
      steps.push({
        description: `Constructing optimal parenthesization: Matrices M${i+1}...M${j+1} should be multiplied by first computing ${left} and ${right}, then multiplying the results`,
        dpTable: JSON.parse(JSON.stringify(dpTable)),
        matrices,
        activeCell: { row: i, col: j },
        activeRow: null,
        activeCol: null,
        highlightCells: [
          { row: i, col: k },
          { row: k+1, col: j }
        ],
        isFinal: true
      });
      
      return `(${left} × ${right})`;
    };
    
    const parenthesization = getParenthesization(0, n-1, splitTable, steps);
    
    steps.push({
      description: `Final optimal parenthesization: ${parenthesization}`,
      dpTable: JSON.parse(JSON.stringify(dpTable)),
      matrices,
      activeCell: null,
      activeRow: null,
      activeCol: null,
      highlightCells: [],
      parenthesization,
      isFinal: true
    });
    
    return steps;
  };
  
  // Initialize steps based on selected algorithm
  useEffect(() => {
    let newSteps = [];
    
    switch (algorithm) {
      case 'knapsack':
        newSteps = generateKnapsackSteps();
        break;
      case 'lcs':
        newSteps = generateLCSSteps();
        break;
      case 'editDistance':
        newSteps = generateEditDistanceSteps();
        break;
      case 'matrixChain':
        newSteps = generateMatrixChainSteps();
        break;
      default:
        newSteps = [];
    }
    
    setSteps(newSteps);
    setMaxSteps(newSteps.length - 1);
    setStep(0);
    setIsPlaying(false);
  }, [algorithm, params]);
  
  // Auto-play functionality
  useEffect(() => {
    let timer;
    if (isPlaying && step < maxSteps) {
      timer = setTimeout(() => {
        setStep(prev => prev + 1);
      }, playbackSpeed);
    } else if (step >= maxSteps) {
      setIsPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [isPlaying, step, maxSteps, playbackSpeed]);
  
  // Handle parameter change
  const handleParamChange = (algoName, paramName, value) => {
    setParams(prev => ({
      ...prev,
      [algoName]: {
        ...prev[algoName],
        [paramName]: value
      }
    }));
  };
  
  // Handle array parameter change
  const handleArrayParamChange = (algoName, paramName, index, value) => {
    const newArray = [...params[algoName][paramName]];
    newArray[index] = parseInt(value, 10);
    
    setParams(prev => ({
      ...prev,
      [algoName]: {
        ...prev[algoName],
        [paramName]: newArray
      }
    }));
  };
  
  // Render Knapsack parameters
  const renderKnapsackParams = () => (
    <div style={{ marginBottom: '20px' }}>
      <h3 style={{ color: colors.secondary, marginBottom: '10px' }}>Parameters</h3>
      
      <div style={{ display: 'flex', flexWrap: 'wrap', gap: '20px' }}>
        <div>
          <div style={{ marginBottom: '5px' }}>Weights:</div>
          <div style={{ display: 'flex', gap: '5px' }}>
            {params.knapsack.weights.map((weight, idx) => (
              <input
                key={idx}
                type="number"
                value={weight}
                onChange={(e) => handleArrayParamChange('knapsack', 'weights', idx, e.target.value)}
                style={{ 
                  width: '50px',
                  background: colors.background,
                  color: colors.text,
                  border: `1px solid ${colors.border}`,
                  borderRadius: '4px',
                  padding: '5px'
                }}
              />
            ))}
          </div>
        </div>
        
        <div>
          <div style={{ marginBottom: '5px' }}>Values:</div>
          <div style={{ display: 'flex', gap: '5px' }}>
            {params.knapsack.values.map((value, idx) => (
              <input
                key={idx}
                type="number"
                value={value}
                onChange={(e) => handleArrayParamChange('knapsack', 'values', idx, e.target.value)}
                style={{ 
                  width: '50px',
                  background: colors.background,
                  color: colors.text,
                  border: `1px solid ${colors.border}`,
                  borderRadius: '4px',
                  padding: '5px'
                }}
              />
            ))}
          </div>
        </div>
        
        <div>
          <div style={{ marginBottom: '5px' }}>Capacity:</div>
          <input
            type="number"
            value={params.knapsack.capacity}
            onChange={(e) => handleParamChange('knapsack', 'capacity', parseInt(e.target.value, 10))}
            style={{ 
              width: '70px',
              background: colors.background,
              color: colors.text,
              border: `1px solid ${colors.border}`,
              borderRadius: '4px',
              padding: '5px'
            }}
          />
        </div>
      </div>
    </div>
  );
  
  // Render LCS parameters
  const renderLCSParams = () => (
    <div style={{ marginBottom: '20px' }}>
      <h3 style={{ color: colors.secondary, marginBottom: '10px' }}>Parameters</h3>
      
      <div style={{ display: 'flex', flexWrap: 'wrap', gap: '20px' }}>
        <div>
          <div style={{ marginBottom: '5px' }}>String 1:</div>
          <input
            type="text"
            value={params.lcs.str1}
            onChange={(e) => handleParamChange('lcs', 'str1', e.target.value)}
            style={{ 
              width: '200px',
              background: colors.background,
              color: colors.text,
              border: `1px solid ${colors.border}`,
              borderRadius: '4px',
              padding: '5px'
            }}
          />
        </div>
        
        <div>
          <div style={{ marginBottom: '5px' }}>String 2:</div>
          <input
            type="text"
            value={params.lcs.str2}
            onChange={(e) => handleParamChange('lcs', 'str2', e.target.value)}
            style={{ 
              width: '200px',
              background: colors.background,
              color: colors.text,
              border: `1px solid ${colors.border}`,
              borderRadius: '4px',
              padding: '5px'
            }}
          />
        </div>
      </div>
    </div>
  );
  
  // Render Edit Distance parameters
  const renderEditDistanceParams = () => (
    <div style={{ marginBottom: '20px' }}>
      <h3 style={{ color: colors.secondary, marginBottom: '10px' }}>Parameters</h3>
      
      <div style={{ display: 'flex', flexWrap: 'wrap', gap: '20px' }}>
        <div>
          <div style={{ marginBottom: '5px' }}>String 1:</div>
          <input
            type="text"
            value={params.editDistance.str1}
            onChange={(e) => handleParamChange('editDistance', 'str1', e.target.value)}
            style={{ 
              width: '200px',
              background: colors.background,
              color: colors.text,
              border: `1px solid ${colors.border}`,
              borderRadius: '4px',
              padding: '5px'
            }}
          />
        </div>
        
        <div>
          <div style={{ marginBottom: '5px' }}>String 2:</div>
          <input
            type="text"
            value={params.editDistance.str2}
            onChange={(e) => handleParamChange('editDistance', 'str2', e.target.value)}
            style={{ 
              width: '200px',
              background: colors.background,
              color: colors.text,
              border: `1px solid ${colors.border}`,
              borderRadius: '4px',
              padding: '5px'
            }}
          />
        </div>
      </div>
    </div>
  );
  
  // Render Matrix Chain Multiplication parameters
  const renderMatrixChainParams = () => (
    <div style={{ marginBottom: '20px' }}>
      <h3 style={{ color: colors.secondary, marginBottom: '10px' }}>Parameters</h3>
      
      <div>
        <div style={{ marginBottom: '5px' }}>Matrix Dimensions:</div>
        <div style={{ display: 'flex', gap: '5px', flexWrap: 'wrap' }}>
          {params.matrixChain.dimensions.map((dim, idx) => (
            <div key={idx} style={{ display: 'flex', alignItems: 'center' }}>
              <input
                type="number"
                value={dim}
                onChange={(e) => handleArrayParamChange('matrixChain', 'dimensions', idx, e.target.value)}
                style={{ 
                  width: '60px',
                  background: colors.background,
                  color: colors.text,
                  border: `1px solid ${colors.border}`,
                  borderRadius: '4px',
                  padding: '5px'
                }}
              />
              {idx < params.matrixChain.dimensions.length - 1 && (
                <span style={{ margin: '0 5px' }}>×</span>
              )}
            </div>
          ))}
        </div>
        <div style={{ marginTop: '10px', fontSize: '14px' }}>
          This represents {params.matrixChain.dimensions.length - 1} matrices with dimensions:
          {params.matrixChain.dimensions.slice(0, -1).map((dim, idx) => (
            <span key={idx}> M{idx+1}({dim}×{params.matrixChain.dimensions[idx+1]}){idx < params.matrixChain.dimensions.length - 2 ? ',' : ''}</span>
          ))}
        </div>
      </div>
    </div>
  );
  
  // Render parameters based on selected algorithm
  const renderParameters = () => {
    switch (algorithm) {
      case 'knapsack':
        return renderKnapsackParams();
      case 'lcs':
        return renderLCSParams();
      case 'editDistance':
        return renderEditDistanceParams();
      case 'matrixChain':
        return renderMatrixChainParams();
      default:
        return null;
    }
  };
  
  // Render DP table for Knapsack
  const renderKnapsackTable = () => {
    if (!steps.length || step >= steps.length) return null;
    
    const currentStep = steps[step];
    const { weights, values } = params.knapsack;
    
    return (
      <div style={{ overflowX: 'auto', marginTop: '20px' }}>
        <table style={{ 
          borderCollapse: 'collapse',
          width: '100%',
          textAlign: 'center'
        }}>
          <thead>
            <tr>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Item/Weight</th>
              {Array.from({ length: currentStep.dpTable[0].length }, (_, i) => (
                <th 
                  key={i} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    backgroundColor: currentStep.activeCol === i ? 'rgba(255, 255, 0, 0.2)' : 'transparent',
                    color: colors.primary
                  }}
                >
                  {i}
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            {currentStep.dpTable.map((row, rowIdx) => (
              <tr key={rowIdx} style={{ 
                backgroundColor: currentStep.activeRow === rowIdx ? 'rgba(255, 255, 0, 0.1)' : 'transparent'
              }}>
                <td style={{ 
                  padding: '8px', 
                  border: `1px solid ${colors.border}`,
                  color: colors.primary,
                  fontWeight: 'bold'
                }}>
                  {rowIdx === 0 ? 'Base' : (
                    <>
                      {rowIdx} (W:{weights[rowIdx-1]}, V:{values[rowIdx-1]})
                    </>
                  )}
                </td>
                {row.map((cell, colIdx) => (
                  <td 
                    key={colIdx} 
                    style={{ 
                      padding: '8px', 
                      border: `1px solid ${colors.border}`,
                      backgroundColor: 
                        currentStep.activeCell?.row === rowIdx && currentStep.activeCell?.col === colIdx
                          ? 'rgba(255, 255, 0, 0.3)'
                          : currentStep.highlightCells?.some(c => c.row === rowIdx && c.col === colIdx)
                            ? 'rgba(0, 255, 255, 0.2)'
                            : 'transparent'
                    }}
                  >
                    {cell}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
        
        {currentStep.selectedItems && (
          <div style={{ marginTop: '20px', padding: '10px', backgroundColor: 'rgba(0, 255, 0, 0.1)', borderRadius: '5px' }}>
            <h4 style={{ color: colors.success, marginTop: 0 }}>Selected Items:</h4>
            <div style={{ display: 'flex', flexWrap: 'wrap', gap: '10px' }}>
              {currentStep.selectedItems.map(itemIdx => (
                <div key={itemIdx} style={{ 
                  padding: '5px 10px',
                  backgroundColor: 'rgba(0, 255, 0, 0.2)',
                  borderRadius: '3px',
                  display: 'flex',
                  flexDirection: 'column',
                  alignItems: 'center'
                }}>
                  <div>Item {itemIdx + 1}</div>
                  <div>Weight: {weights[itemIdx]}, Value: {values[itemIdx]}</div>
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    );
  };
  
  // Render DP table for LCS
  const renderLCSTable = () => {
    if (!steps.length || step >= steps.length) return null;
    
    const currentStep = steps[step];
    const { str1, str2 } = params.lcs;
    
    return (
      <div style={{ overflowX: 'auto', marginTop: '20px' }}>
        <table style={{ 
          borderCollapse: 'collapse',
          width: '100%',
          textAlign: 'center'
        }}>
          <thead>
            <tr>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}></th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>ε</th>
              {str2.split('').map((char, idx) => (
                <th 
                  key={idx} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    backgroundColor: currentStep.charactersCompared?.some(c => c.string === 2 && c.index === idx)
                      ? 'rgba(255, 0, 255, 0.3)'
                      : 'transparent',
                    color: colors.primary
                  }}
                >
                  {char}
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={{ 
                padding: '8px', 
                border: `1px solid ${colors.border}`,
                color: colors.primary,
                fontWeight: 'bold'
              }}>
                ε
              </td>
              {currentStep.dpTable[0].map((cell, colIdx) => (
                <td 
                  key={colIdx} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    backgroundColor: 
                      currentStep.activeCell?.row === 0 && currentStep.activeCell?.col === colIdx
                        ? 'rgba(255, 255, 0, 0.3)'
                        : currentStep.highlightCells?.some(c => c.row === 0 && c.col === colIdx)
                          ? 'rgba(0, 255, 255, 0.2)'
                          : 'transparent'
                  }}
                >
                  {cell}
                </td>
              ))}
            </tr>
            {str1.split('').map((char, rowIdx) => (
              <tr key={rowIdx}>
                <td style={{ 
                  padding: '8px', 
                  border: `1px solid ${colors.border}`,
                  backgroundColor: currentStep.charactersCompared?.some(c => c.string === 1 && c.index === rowIdx)
                    ? 'rgba(255, 0, 255, 0.3)'
                    : 'transparent',
                  color: colors.primary,
                  fontWeight: 'bold'
                }}>
                  {char}
                </td>
                {currentStep.dpTable[rowIdx+1].map((cell, colIdx) => (
                  <td 
                    key={colIdx} 
                    style={{ 
                      padding: '8px', 
                      border: `1px solid ${colors.border}`,
                      backgroundColor: 
                        currentStep.activeCell?.row === rowIdx+1 && currentStep.activeCell?.col === colIdx
                          ? 'rgba(255, 255, 0, 0.3)'
                          : currentStep.highlightCells?.some(c => c.row === rowIdx+1 && c.col === colIdx)
                            ? 'rgba(0, 255, 255, 0.2)'
                            : 'transparent'
                    }}
                  >
                    {cell}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
        
        {currentStep.lcs && (
          <div style={{ marginTop: '20px', padding: '10px', backgroundColor: 'rgba(0, 255, 0, 0.1)', borderRadius: '5px' }}>
            <h4 style={{ color: colors.success, marginTop: 0 }}>Longest Common Subsequence:</h4>
            <div style={{ 
              padding: '5px 10px',
              backgroundColor: 'rgba(0, 255, 0, 0.2)',
              borderRadius: '3px',
              fontSize: '18px',
              fontWeight: 'bold'
            }}>
              {currentStep.lcs.join('')}
            </div>
          </div>
        )}
      </div>
    );
  };
  
  // Render DP table for Edit Distance
  const renderEditDistanceTable = () => {
    if (!steps.length || step >= steps.length) return null;
    
    const currentStep = steps[step];
    const { str1, str2 } = params.editDistance;
    
    return (
      <div style={{ overflowX: 'auto', marginTop: '20px' }}>
        <table style={{ 
          borderCollapse: 'collapse',
          width: '100%',
          textAlign: 'center'
        }}>
          <thead>
            <tr>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}></th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>ε</th>
              {str2.split('').map((char, idx) => (
                <th 
                  key={idx} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    backgroundColor: currentStep.charactersCompared?.some(c => c.string === 2 && c.index === idx)
                      ? 'rgba(255, 0, 255, 0.3)'
                      : 'transparent',
                    color: colors.primary
                  }}
                >
                  {char}
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={{ 
                padding: '8px', 
                border: `1px solid ${colors.border}`,
                color: colors.primary,
                fontWeight: 'bold'
              }}>
                ε
              </td>
              {currentStep.dpTable[0].map((cell, colIdx) => (
                <td 
                  key={colIdx} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    backgroundColor: 
                      currentStep.activeCell?.row === 0 && currentStep.activeCell?.col === colIdx
                        ? 'rgba(255, 255, 0, 0.3)'
                        : currentStep.highlightCells?.some(c => c.row === 0 && c.col === colIdx)
                          ? 'rgba(0, 255, 255, 0.2)'
                          : 'transparent'
                  }}
                >
                  {cell}
                </td>
              ))}
            </tr>
            {str1.split('').map((char, rowIdx) => (
              <tr key={rowIdx}>
                <td style={{ 
                  padding: '8px', 
                  border: `1px solid ${colors.border}`,
                  backgroundColor: currentStep.charactersCompared?.some(c => c.string === 1 && c.index === rowIdx)
                    ? 'rgba(255, 0, 255, 0.3)'
                    : 'transparent',
                  color: colors.primary,
                  fontWeight: 'bold'
                }}>
                  {char}
                </td>
                {currentStep.dpTable[rowIdx+1].map((cell, colIdx) => (
                  <td 
                    key={colIdx} 
                    style={{ 
                      padding: '8px', 
                      border: `1px solid ${colors.border}`,
                      backgroundColor: 
                        currentStep.activeCell?.row === rowIdx+1 && currentStep.activeCell?.col === colIdx
                          ? 'rgba(255, 255, 0, 0.3)'
                          : currentStep.highlightCells?.some(c => c.row === rowIdx+1 && c.col === colIdx)
                            ? 'rgba(0, 255, 255, 0.2)'
                            : 'transparent'
                    }}
                  >
                    {cell}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
        
        {currentStep.operations && (
          <div style={{ marginTop: '20px', padding: '10px', backgroundColor: 'rgba(0, 255, 0, 0.1)', borderRadius: '5px' }}>
            <h4 style={{ color: colors.success, marginTop: 0 }}>Edit Operations:</h4>
            <ol style={{ margin: '0', paddingLeft: '20px' }}>
              {currentStep.operations.map((op, idx) => (
                <li key={idx} style={{ margin: '5px 0' }}>{op}</li>
              ))}
            </ol>
          </div>
        )}
      </div>
    );
  };
  
  // Render DP table for Matrix Chain Multiplication
  const renderMatrixChainTable = () => {
    if (!steps.length || step >= steps.length) return null;
    
    const currentStep = steps[step];
    
    return (
      <div style={{ overflowX: 'auto', marginTop: '20px' }}>
        <div style={{ marginBottom: '15px' }}>
          <h4 style={{ color: colors.secondary, marginTop: 0 }}>Matrices:</h4>
          <div style={{ display: 'flex', flexWrap: 'wrap', gap: '10px' }}>
            {currentStep.matrices.map((matrix, idx) => (
              <div key={idx} style={{ 
                padding: '5px 10px',
                backgroundColor: currentStep.chainIndices?.[0] <= idx && idx <= currentStep.chainIndices?.[1]
                  ? 'rgba(255, 255, 0, 0.2)'
                  : 'rgba(0, 0, 0, 0.3)',
                borderRadius: '3px',
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center'
              }}>
                <div>{matrix.name}</div>
                <div>{matrix.rows}×{matrix.cols}</div>
              </div>
            ))}
          </div>
        </div>
        
        <table style={{ 
          borderCollapse: 'collapse',
          width: '100%',
          textAlign: 'center'
        }}>
          <thead>
            <tr>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}></th>
              {currentStep.matrices.map((matrix, idx) => (
                <th 
                  key={idx} 
                  style={{ 
                    padding: '8px', 
                    border: `1px solid ${colors.border}`,
                    color: colors.primary
                  }}
                >
                  {matrix.name}
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            {currentStep.dpTable.map((row, rowIdx) => (
              <tr key={rowIdx}>
                <td style={{ 
                  padding: '8px', 
                  border: `1px solid ${colors.border}`,
                  color: colors.primary,
                  fontWeight: 'bold'
                }}>
                  {currentStep.matrices[rowIdx].name}
                </td>
                {row.map((cell, colIdx) => (
                  <td 
                    key={colIdx} 
                    style={{ 
                      padding: '8px', 
                      border: `1px solid ${colors.border}`,
                      backgroundColor: 
                        colIdx < rowIdx
                          ? 'rgba(0, 0, 0, 0.3)'
                          : currentStep.activeCell?.row === rowIdx && currentStep.activeCell?.col === colIdx
                            ? 'rgba(255, 255, 0, 0.3)'
                            : currentStep.highlightCells?.some(c => c.row === rowIdx && c.col === colIdx)
                              ? 'rgba(0, 255, 255, 0.2)'
                              : 'transparent'
                    }}
                  >
                    {colIdx < rowIdx ? '' : cell === Number.MAX_SAFE_INTEGER ? '∞' : cell}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
        
        {currentStep.parenthesization && (
          <div style={{ marginTop: '20px', padding: '10px', backgroundColor: 'rgba(0, 255, 0, 0.1)', borderRadius: '5px' }}>
            <h4 style={{ color: colors.success, marginTop: 0 }}>Optimal Parenthesization:</h4>
            <div style={{ 
              padding: '5px 10px',
              backgroundColor: 'rgba(0, 255, 0, 0.2)',
              borderRadius: '3px',
              fontSize: '16px',
              fontWeight: 'bold'
            }}>
              {currentStep.parenthesization}
            </div>
          </div>
        )}
      </div>
    );
  };
  
  // Render visualization based on selected algorithm
  const renderVisualization = () => {
    switch (algorithm) {
      case 'knapsack':
        return renderKnapsackTable();
      case 'lcs':
        return renderLCSTable();
      case 'editDistance':
        return renderEditDistanceTable();
      case 'matrixChain':
        return renderMatrixChainTable();
      default:
        return null;
    }
  };
  
  // Render complexity table
  const renderComplexityTable = () => {
    return (
      <div style={{ marginTop: '20px', overflowX: 'auto' }}>
        <h3 style={{ color: colors.secondary }}>Time & Space Complexity Analysis</h3>
        <table style={{ 
          borderCollapse: 'collapse',
          width: '100%',
          textAlign: 'left'
        }}>
          <thead>
            <tr>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Algorithm</th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Approach</th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Time Complexity</th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Space Complexity</th>
              <th style={{ padding: '8px', border: `1px solid ${colors.border}`, color: colors.primary }}>Notes</th>
            </tr>
          </thead>
          <tbody>
            <tr style={{ backgroundColor: algorithm === 'knapsack' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}`, fontWeight: 'bold' }}>0/1 Knapsack</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Naive Recursive</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(2^n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Exponential time due to overlapping subproblems</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'knapsack' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Memoization</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n * W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n * W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>n = number of items, W = capacity</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'knapsack' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Tabulation</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n * W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n * W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Bottom-up approach</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'knapsack' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Space Optimized</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n * W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(W)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Uses 1D array instead of 2D</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'lcs' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}`, fontWeight: 'bold' }}>Longest Common Subsequence</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Naive Recursive</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(2^(m+n))</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m+n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Exponential time</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'lcs' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Memoization</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>m, n = lengths of strings</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'lcs' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Tabulation</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Bottom-up approach</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'lcs' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Space Optimized</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(min(m, n))</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Uses two rows instead of matrix</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'editDistance' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}`, fontWeight: 'bold' }}>Edit Distance</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Naive Recursive</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(3^(m+n))</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m+n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Three choices for each character</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'editDistance' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Memoization</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Top-down with cache</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'editDistance' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Tabulation</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Bottom-up approach</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'editDistance' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Space Optimized</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(m * n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(min(m, n))</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Uses two rows instead of matrix</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'matrixChain' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}`, fontWeight: 'bold' }}>Matrix Chain Multiplication</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Naive Recursive</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(2^n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Exponential due to many recursion paths</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'matrixChain' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Memoization</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n^3)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n^2)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Top-down with cache</td>
            </tr>
            <tr style={{ backgroundColor: algorithm === 'matrixChain' ? 'rgba(0, 255, 255, 0.1)' : 'transparent' }}>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}></td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>Tabulation</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n^3)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>O(n^2)</td>
              <td style={{ padding: '8px', border: `1px solid ${colors.border}` }}>n^3 from nested loop with n^2 subproblems</td>
            </tr>
          </tbody>
        </table>
      </div>
    );
  };
  
  return (
    <div style={{ 
      fontFamily: 'Arial, sans-serif',
      color: colors.text,
      backgroundColor: colors.background,
      padding: '20px',
      borderRadius: '10px'
    }}>
      <h1 style={{ color: colors.primary, textAlign: 'center' }}>Dynamic Programming Visualization - Part 2</h1>
      
      {/* Algorithm selection */}
      <div style={{ marginBottom: '20px' }}>
        <h3 style={{ color: colors.secondary }}>Select Algorithm</h3>
        <div style={{ display: 'flex', gap: '10px' }}>
          {['knapsack', 'lcs', 'editDistance', 'matrixChain'].map(algo => (
            <button
              key={algo}
              onClick={() => setAlgorithm(algo)}
              style={{
                padding: '8px 15px',
                backgroundColor: algorithm === algo ? colors.primary : 'transparent',
                color: algorithm === algo ? colors.background : colors.primary,
                border: `1px solid ${colors.primary}`,
                borderRadius: '5px',
                cursor: 'pointer'
              }}
            >
              {algo === 'knapsack' && '0/1 Knapsack'}
              {algo === 'lcs' && 'Longest Common Subsequence'}
              {algo === 'editDistance' && 'Edit Distance'}
              {algo === 'matrixChain' && 'Matrix Chain Multiplication'}
            </button>
          ))}
        </div>
      </div>
      
      {/* Algorithm parameters */}
      {renderParameters()}
      
      {/* Description */}
      {steps.length > 0 && step < steps.length && (
        <div style={{ 
          marginBottom: '20px', 
          padding: '15px', 
          backgroundColor: 'rgba(255, 255, 255, 0.05)', 
          borderRadius: '5px',
          border: `1px solid ${colors.border}`
        }}>
          <p>{steps[step].description}</p>
        </div>
      )}
      
      {/* Visualization */}
      {renderVisualization()}
      
      {/* Time & Space Complexity Table */}
      {showComplexityTable && renderComplexityTable()}
      
      {/* Playback controls */}
      <div style={{ 
        marginTop: '20px',
        display: 'flex',
        justifyContent: 'center',
        gap: '10px',
        flexWrap: 'wrap'
      }}>
        <button
          onClick={() => setStep(0)}
          style={{
            padding: '8px 15px',
            backgroundColor: 'transparent',
            color: colors.primary,
            border: `1px solid ${colors.primary}`,
            borderRadius: '5px',
            cursor: 'pointer'
          }}
        >
          Reset
        </button>
        <button
          onClick={() => setStep(prev => Math.max(0, prev - 1))}
          disabled={step === 0}
          style={{
            padding: '8px 15px',
            backgroundColor: 'transparent',
            color: colors.primary,
            border: `1px solid ${colors.primary}`,
            borderRadius: '5px',
            cursor: step === 0 ? 'not-allowed' : 'pointer',
            opacity: step === 0 ? 0.5 : 1
          }}
        >
          Previous
        </button>
        {isPlaying ? (
          <button
            onClick={() => setIsPlaying(false)}
            style={{
              padding: '8px 15px',
              backgroundColor: colors.highlight,
              color: colors.background,
              border: 'none',
              borderRadius: '5px',
              cursor: 'pointer'
            }}
          >
            Pause
          </button>
        ) : (
          <button
            onClick={() => setIsPlaying(true)}
            disabled={step >= maxSteps}
            style={{
              padding: '8px 15px',
              backgroundColor: colors.primary,
              color: colors.background,
              border: 'none',
              borderRadius: '5px',
              cursor: step >= maxSteps ? 'not-allowed' : 'pointer',
              opacity: step >= maxSteps ? 0.5 : 1
            }}
          >
            Play
          </button>
        )}
        <button
          onClick={() => setStep(prev => Math.min(maxSteps, prev + 1))}
          disabled={step >= maxSteps}
          style={{
            padding: '8px 15px',
            backgroundColor: 'transparent',
            color: colors.primary,
            border: `1px solid ${colors.primary}`,
            borderRadius: '5px',
            cursor: step >= maxSteps ? 'not-allowed' : 'pointer',
            opacity: step >= maxSteps ? 0.5 : 1
          }}
        >
          Next
        </button>
        <select
          value={playbackSpeed}
          onChange={(e) => setPlaybackSpeed(parseInt(e.target.value, 10))}
          style={{
            padding: '8px 15px',
            backgroundColor: 'transparent',
            color: colors.text,
            border: `1px solid ${colors.border}`,
            borderRadius: '5px'
          }}
        >
          <option value={2000}>Slow</option>
          <option value={1000}>Normal</option>
          <option value={500}>Fast</option>
        </select>
        <button
          onClick={() => setShowComplexityTable(!showComplexityTable)}
          style={{
            padding: '8px 15px',
            backgroundColor: 'transparent',
            color: colors.secondary,
            border: `1px solid ${colors.secondary}`,
            borderRadius: '5px',
            cursor: 'pointer'
          }}
        >
          {showComplexityTable ? 'Hide Complexity Table' : 'Show Complexity Table'}
        </button>
      </div>
      
      {/* Step progress */}
      <div style={{ 
        marginTop: '20px',
        padding: '10px',
        backgroundColor: 'rgba(255, 255, 255, 0.05)',
        borderRadius: '5px',
        textAlign: 'center'
      }}>
        Step {step + 1} of {maxSteps + 1}
        <div style={{ 
          width: '100%',
          height: '10px',
          backgroundColor: 'rgba(255, 255, 255, 0.1)',
          borderRadius: '5px',
          marginTop: '10px',
          position: 'relative'
        }}>
          <div style={{ 
            position: 'absolute',
            top: 0,
            left: 0,
            height: '100%',
            width: `${((step + 1) / (maxSteps + 1)) * 100}%`,
            backgroundColor: colors.primary,
            borderRadius: '5px'
          }} />
        </div>
      </div>
      
      {/* Info box */}
      <div style={{ 
        marginTop: '30px',
        padding: '15px',
        backgroundColor: 'rgba(255, 255, 255, 0.05)',
        borderRadius: '5px',
        border: `1px solid ${colors.border}`
      }}>
        <h3 style={{ color: colors.highlight, marginTop: 0 }}>About Advanced Dynamic Programming</h3>
        <p>
          Dynamic Programming (DP) is a powerful method for solving complex problems by breaking them down into simpler subproblems and storing the results to avoid redundant computations. This visualization demonstrates several classic DP problems:
        </p>
        <ul style={{ paddingLeft: '20px' }}>
          <li><strong style={{ color: colors.primary }}>0/1 Knapsack:</strong> The classic resource allocation problem where you select items to maximize value within a weight constraint.</li>
          <li><strong style={{ color: colors.primary }}>Longest Common Subsequence:</strong> Finding the longest sequence that appears in the same order (not necessarily contiguous) in two strings.</li>
          <li><strong style={{ color: colors.primary }}>Edit Distance:</strong> Finding the minimum number of operations (insert, delete, replace) to transform one string into another.</li>
          <li><strong style={{ color: colors.primary }}>Matrix Chain Multiplication:</strong> Finding the optimal way to multiply a chain of matrices to minimize the number of scalar multiplications.</li>
        </ul>
        <p>
          These algorithms demonstrate the key principles of dynamic programming: optimal substructure and overlapping subproblems. Use the controls above to step through each algorithm and understand how DP efficiently solves these complex problems.
        </p>
      </div>
    </div>
  );
};

export default DynamicProgrammingVisualization;