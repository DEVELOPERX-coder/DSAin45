import React, { useState, useEffect } from 'react';
import { Play, Pause, RotateCcw, ChevronLeft, ChevronRight, Code } from 'lucide-react';

const BacktrackingVisualization = () => {
  // State for controlling the visualization
  const [algorithm, setAlgorithm] = useState('nqueens');
  const [step, setStep] = useState(0);
  const [playing, setPlaying] = useState(false);
  const [speed, setSpeed] = useState(1000); // milliseconds between steps
  const [boardSize, setBoardSize] = useState(4); // For N-Queens
  const [showCode, setShowCode] = useState(false);
  
  // Time and space complexity data
  const complexityData = {
    nqueens: {
      name: "N-Queens",
      time: "O(N!)",
      space: "O(N)",
      timeExplanation: "We try N positions for first row, N-1 for second row, and so on",
      spaceExplanation: "We need O(N) space for the board and the recursion stack"
    },
    sudoku: {
      name: "Sudoku Solver",
      time: "O(9^(N²))",
      space: "O(N²)",
      timeExplanation: "We try up to 9 digits for each empty cell in the 9×9 grid",
      spaceExplanation: "We need O(N²) space for the board and the recursion stack"
    },
    subsetsum: {
      name: "Subset Sum",
      time: "O(2^N)",
      space: "O(N)",
      timeExplanation: "We have 2 choices (include/exclude) for each element in the set",
      spaceExplanation: "We need O(N) space for the current subset and recursion stack"
    },
    permutations: {
      name: "Permutations",
      time: "O(N!)",
      space: "O(N)",
      timeExplanation: "We have N! different permutations of N elements",
      spaceExplanation: "We need O(N) space for the current permutation and recursion stack"
    },
    combinations: {
      name: "Combinations",
      time: "O(N choose K)",
      space: "O(K)",
      timeExplanation: "We have C(n,k) = n!/(k!(n-k)!) possible combinations",
      spaceExplanation: "We need O(K) space for the current combination and recursion stack"
    },
    wordsearch: {
      name: "Word Search",
      time: "O(M×N×4^L)",
      space: "O(L)",
      timeExplanation: "We try each cell as starting point and explore in 4 directions for a word of length L",
      spaceExplanation: "We need O(L) space for the recursion stack and visited cells"
    }
  };

  // Example data for different algorithms
  const [nQueensData, setNQueensData] = useState({
    n: boardSize,
    steps: generateNQueensSteps(boardSize),
  });
  
  // Update N-Queens data when board size changes
  useEffect(() => {
    setNQueensData({
      n: boardSize,
      steps: generateNQueensSteps(boardSize),
    });
    setStep(0);
  }, [boardSize]);

  const [sudokuData] = useState({
    board: [
      [5, 3, 0, 0, 7, 0, 0, 0, 0],
      [6, 0, 0, 1, 9, 5, 0, 0, 0],
      [0, 9, 8, 0, 0, 0, 0, 6, 0],
      [8, 0, 0, 0, 6, 0, 0, 0, 3],
      [4, 0, 0, 8, 0, 3, 0, 0, 1],
      [7, 0, 0, 0, 2, 0, 0, 0, 6],
      [0, 6, 0, 0, 0, 0, 2, 8, 0],
      [0, 0, 0, 4, 1, 9, 0, 0, 5],
      [0, 0, 0, 0, 8, 0, 0, 7, 9]
    ],
    steps: [
      { message: "Starting with the initial Sudoku puzzle", row: -1, col: -1, value: 0, states: [] },
      { message: "Looking for an empty cell", row: 0, col: 2, value: 0, states: [] },
      { message: "Trying 1 at position (0,2)", row: 0, col: 2, value: 1, states: [] },
      { message: "1 is not valid at (0,2) because it conflicts with row or column or box", row: 0, col: 2, value: 0, states: [] },
      { message: "Trying 2 at position (0,2)", row: 0, col: 2, value: 2, states: [] },
      { message: "Valid placement, moving to next cell", row: 0, col: 2, value: 2, states: [] },
      { message: "Looking for an empty cell", row: 0, col:.3, value: 0, states: [] },
      { message: "Trying 4 at position (0,3)", row: 0, col: 3, value: 4, states: [] },
      { message: "Valid placement, moving to next cell", row: 0, col: 3, value: 4, states: [] },
      { message: "...many steps later...", row: -1, col: -1, value: 0, states: [] },
      { message: "Solution found! All cells are filled according to Sudoku rules", row: -1, col: -1, value: 0, states: [] }
    ]
  });

  function generateNQueensSteps(n) {
    const initialBoard = Array(n).fill().map(() => Array(n).fill(0));
    const steps = [];
    
    // Initial state
    steps.push({
      board: JSON.parse(JSON.stringify(initialBoard)),
      message: `Start with an empty ${n}×${n} board`,
      row: -1,
      col: -1
    });
    
    // Example steps for N-Queens (simplified for visualization)
    const simulateBacktracking = (board, row) => {
      if (row >= n) return true;
      
      for (let col = 0; col < n; col++) {
        const newBoard = JSON.parse(JSON.stringify(board));
        
        // Check if this position is safe
        let isSafe = true;
        for (let prevRow = 0; prevRow < row; prevRow++) {
          // Check column
          if (board[prevRow][col] === 1) {
            isSafe = false;
            break;
          }
          
          // Check diagonals
          const leftDiagCol = col - (row - prevRow);
          const rightDiagCol = col + (row - prevRow);
          
          if ((leftDiagCol >= 0 && board[prevRow][leftDiagCol] === 1) || 
              (rightDiagCol < n && board[prevRow][rightDiagCol] === 1)) {
            isSafe = false;
            break;
          }
        }
        
        // Try placing queen
        steps.push({
          board: JSON.parse(JSON.stringify(board)),
          message: `Trying to place queen at row ${row}, column ${col}`,
          row,
          col
        });
        
        if (isSafe) {
          // Place queen
          newBoard[row][col] = 1;
          steps.push({
            board: JSON.parse(JSON.stringify(newBoard)),
            message: `Safe position found at row ${row}, column ${col}`,
            row,
            col
          });
          
          // Recur for next row
          if (simulateBacktracking(newBoard, row + 1)) {
            return true;
          }
          
          // If placing queen doesn't lead to a solution, backtrack
          steps.push({
            board: JSON.parse(JSON.stringify(newBoard)),
            message: `Placing queen at row ${row}, column ${col} doesn't lead to a solution, backtracking`,
            row,
            col
          });
          
          // Backtrack
          newBoard[row][col] = 0;
        } else {
          steps.push({
            board: JSON.parse(JSON.stringify(board)),
            message: `Position at row ${row}, column ${col} is under attack, trying next column`,
            row,
            col
          });
        }
      }
      
      return false;
    };
    
    // Generate example solutions for 4-Queens
    const simulatedBoard = JSON.parse(JSON.stringify(initialBoard));
    simulateBacktracking(simulatedBoard, 0);
    
    // Add final solution
    if (n === 4) {
      const solutionBoard = [
        [0, 0, 1, 0],
        [1, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 1, 0, 0]
      ];
      steps.push({
        board: solutionBoard,
        message: `Solution found for ${n}-Queens problem!`,
        row: -1,
        col: -1
      });
    }
    
    return steps;
  }

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
      case 'nqueens':
        return nQueensData.steps.length;
      case 'sudoku':
        return sudokuData.steps.length;
      default:
        return 0;
    }
  };

  // Code snippets for each algorithm
  const codeSnippets = {
    nqueens: `void solveNQueensUtil(vector<string>& board, int row) {
    // Base case: All queens are placed
    if (row == n) {
        solutions.push_back(board);
        return;
    }
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col)) {
            // Place queen
            board[row][col] = 'Q';
            
            // Recur to place queens in remaining rows
            solveNQueensUtil(board, row + 1);
            
            // Backtrack and remove queen
            board[row][col] = '.';
        }
    }
}

bool isSafe(vector<string>& board, int row, int col) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 'Q') {
            return false;
        }
    }
    
    // Check upper-left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 'Q') {
            return false;
        }
    }
    
    // Check upper-right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 'Q') {
            return false;
        }
    }
    
    return true;
}`,
    sudoku: `bool solveSudokuUtil(vector<vector<char>>& board) {
    int row, col;
    
    // If no empty cell is found, we've solved the puzzle
    if (!findEmptyCell(board, row, col)) {
        return true;
    }
    
    // Try digits 1-9 for the empty cell
    for (char digit = '1'; digit <= '9'; digit++) {
        if (isValid(board, row, col, digit)) {
            // Place digit
            board[row][col] = digit;
            
            // Recur to solve rest of the board
            if (solveSudokuUtil(board)) {
                return true;
            }
            
            // If placing digit doesn't lead to a solution, backtrack
            board[row][col] = '.';
        }
    }
    
    // Trigger backtracking
    return false;
}

bool isValid(vector<vector<char>>& board, int row, int col, char digit) {
    // Check row
    for (int j = 0; j < 9; j++) {
        if (board[row][j] == digit) {
            return false;
        }
    }
    
    // Check column
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == digit) {
            return false;
        }
    }
    
    // Check 3x3 subgrid
    int subgridRow = 3 * (row / 3);
    int subgridCol = 3 * (col / 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[subgridRow + i][subgridCol + j] == digit) {
                return false;
            }
        }
    }
    
    return true;
}`
  };

  // Render N-Queens Board
  const renderNQueensBoard = () => {
    const { n, steps } = nQueensData;
    const currentStep = steps[step];
    const board = currentStep.board;
    
    return (
      <div className="mt-4">
        <div className="grid grid-cols-[auto_1fr] gap-4">
          {/* Board */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">{n}×{n} Chessboard</h3>
            <div className={`grid grid-cols-${n} gap-px bg-gray-600`} style={{ gridTemplateColumns: `repeat(${n}, minmax(0, 1fr))` }}>
              {board.map((row, rowIndex) => (
                row.map((cell, colIndex) => {
                  const isHighlighted = currentStep.row === rowIndex && currentStep.col === colIndex;
                  const isQueen = cell === 1;
                  const isDarkSquare = (rowIndex + colIndex) % 2 === 1;
                  
                  return (
                    <div
                      key={`${rowIndex}-${colIndex}`}
                      className={`w-12 h-12 flex items-center justify-center transition-all duration-300 ${
                        isDarkSquare ? 'bg-gray-700' : 'bg-gray-800'
                      } ${isHighlighted ? 'ring-2 ring-yellow-400' : ''}`}
                    >
                      {isQueen && (
                        <div className="text-2xl text-pink-500">♛</div>
                      )}
                    </div>
                  );
                })
              ))}
            </div>
          </div>
          
          {/* State Information */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Backtracking Process</h3>
            <p className="mb-4">{currentStep.message}</p>
            
            <div className="mt-4">
              <h4 className="text-cyan-400 mb-2">State Space Exploration</h4>
              <p className="text-sm">
                Backtracking builds a state space tree where each node represents a partial solution.
                We expand nodes that might lead to a solution and prune those that violate constraints.
              </p>
              
              <div className="mt-4 flex items-center">
                <div className="w-4 h-4 bg-yellow-400 rounded-full mr-2"></div>
                <span>Current position being evaluated</span>
              </div>
              <div className="mt-2 flex items-center">
                <div className="w-4 h-4 bg-pink-500 rounded-full mr-2"></div>
                <span>Placed queen</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  };

  // Render Sudoku Board
  const renderSudokuBoard = () => {
    const currentStep = sudokuData.steps[step];
    const board = sudokuData.board; // Using the initial board for simplicity
    
    return (
      <div className="mt-4">
        <div className="grid grid-cols-[auto_1fr] gap-4">
          {/* Board */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Sudoku Puzzle</h3>
            <div className="grid grid-cols-9 gap-px bg-gray-600">
              {board.map((row, rowIndex) => (
                row.map((cell, colIndex) => {
                  const isHighlighted = currentStep.row === rowIndex && currentStep.col === colIndex;
                  const isFixed = cell !== 0;
                  const isTrying = isHighlighted && currentStep.value !== 0;
                  
                  return (
                    <div
                      key={`${rowIndex}-${colIndex}`}
                      className={`w-8 h-8 flex items-center justify-center transition-all duration-300 bg-gray-800 
                                 ${isHighlighted ? 'ring-2 ring-yellow-400' : ''}
                                 ${(Math.floor(rowIndex / 3) + Math.floor(colIndex / 3)) % 2 === 0 ? 'bg-gray-700' : 'bg-gray-800'}`}
                    >
                      {isFixed ? (
                        <span className="text-cyan-400 font-bold">{cell}</span>
                      ) : isTrying ? (
                        <span className="text-pink-500 font-bold">{currentStep.value}</span>
                      ) : (
                        <span></span>
                      )}
                    </div>
                  );
                })
              ))}
            </div>
          </div>
          
          {/* State Information */}
          <div className="bg-gray-800 rounded-lg p-4">
            <h3 className="text-yellow-400 text-lg mb-2">Backtracking Process</h3>
            <p className="mb-4">{currentStep.message}</p>
            
            <div className="mt-4">
              <h4 className="text-cyan-400 mb-2">Constraint Checking</h4>
              <p className="text-sm">
                Sudoku solving uses backtracking with constraint checking:
              </p>
              <ul className="list-disc ml-5 mt-2 text-sm">
                <li>Row constraint: Each number appears once per row</li>
                <li>Column constraint: Each number appears once per column</li>
                <li>Box constraint: Each number appears once per 3×3 box</li>
              </ul>
              
              <div className="mt-4 flex items-center">
                <div className="w-4 h-4 bg-yellow-400 rounded-full mr-2"></div>
                <span>Current cell being evaluated</span>
              </div>
              <div className="mt-2 flex items-center">
                <div className="w-4 h-4 bg-cyan-400 rounded-full mr-2"></div>
                <span>Fixed numbers (from original puzzle)</span>
              </div>
              <div className="mt-2 flex items-center">
                <div className="w-4 h-4 bg-pink-500 rounded-full mr-2"></div>
                <span>Number being tried</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  };

  // All backtracking algorithms in our visualization
  const algorithms = [
    { id: 'nqueens', name: 'N-Queens Problem' },
    { id: 'sudoku', name: 'Sudoku Solver' },
  ];

  // Render the current algorithm visualization
  const renderVisualization = () => {
    switch(algorithm) {
      case 'nqueens':
        return renderNQueensBoard();
      case 'sudoku':
        return renderSudokuBoard();
      default:
        return <div>Select an algorithm to visualize</div>;
    }
  };

  // Render the complexity table
  const renderComplexityTable = () => {
    return (
      <div className="mt-6 bg-gray-800 rounded-lg p-4">
        <h3 className="text-yellow-400 text-lg mb-2">Time & Space Complexity Analysis</h3>
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
      <h1 className="text-2xl font-bold text-center mb-4">Backtracking Algorithms Visualization</h1>
      
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
      
      {/* Board size selection for N-Queens */}
      {algorithm === 'nqueens' && (
        <div className="flex justify-center mb-4 space-x-4">
          <span className="text-gray-400">Board Size:</span>
          {[4, 5, 6, 8].map(size => (
            <button
              key={size}
              className={`px-3 py-1 rounded-md ${
                boardSize === size ? 'bg-pink-500 text-white' : 'bg-gray-700 text-white'
              }`}
              onClick={() => {
                setBoardSize(size);
                setStep(0);
                setPlaying(false);
              }}
            >
              {size}×{size}
            </button>
          ))}
        </div>
      )}
      
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
          {algorithm === 'nqueens' && "Place N queens on an N×N chessboard so that no two queens threaten each other."}
          {algorithm === 'sudoku' && "Fill a 9×9 grid with digits so that each row, column, and 3×3 subgrid contains all digits from 1-9."}
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
          {algorithm === 'nqueens' && nQueensData.steps[step]?.message}
          {algorithm === 'sudoku' && sudokuData.steps[step]?.message}
        </div>
      </div>
      
      {/* Visualization area */}
      <div className="bg-gray-800 rounded-lg p-4 mb-4 min-h-[350px]">
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
      
      {/* Key Features of Backtracking */}
      <div className="bg-gray-800 rounded-lg p-4 mt-4">
        <h3 className="text-yellow-400 text-lg mb-2">Key Principles of Backtracking</h3>
        <ul className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <li className="bg-gray-700 p-3 rounded-lg">
            <h4 className="text-cyan-400 font-bold">Incremental Construction</h4>
            <p className="text-sm mt-1">Build solutions step by step, adding one component at a time</p>
          </li>
          <li className="bg-gray-700 p-3 rounded-lg">
            <h4 className="text-cyan-400 font-bold">Constraint Checking</h4>
            <p className="text-sm mt-1">Verify if partial solutions satisfy problem constraints</p>
          </li>
          <li className="bg-gray-700 p-3 rounded-lg">
            <h4 className="text-cyan-400 font-bold">Early Pruning</h4>
            <p className="text-sm mt-1">Abandon paths that cannot lead to valid solutions</p>
          </li>
          <li className="bg-gray-700 p-3 rounded-lg">
            <h4 className="text-pink-400 font-bold">State Reversion</h4>
            <p className="text-sm mt-1">Undo choices and explore alternatives when backtracking</p>
          </li>
        </ul>
      </div>
    </div>
  );
};

export default BacktrackingVisualization;