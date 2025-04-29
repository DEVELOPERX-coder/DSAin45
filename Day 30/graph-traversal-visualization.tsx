import React, { useState, useEffect, useRef } from 'react';
import { Play, SkipForward, RefreshCw, Code, Settings } from 'lucide-react';

const GraphTraversalVisualization = () => {
  // State for graph data
  const [vertices, setVertices] = useState(['A', 'B', 'C', 'D', 'E', 'F']);
  const [adjacencyList, setAdjacencyList] = useState({
    'A': ['B', 'C'],
    'B': ['A', 'D', 'E'],
    'C': ['A', 'F'],
    'D': ['B'],
    'E': ['B', 'F'],
    'F': ['C', 'E']
  });
  
  // State for visualization
  const [startVertex, setStartVertex] = useState('A');
  const [algorithm, setAlgorithm] = useState('bfs');
  const [visited, setVisited] = useState([]);
  const [queue, setQueue] = useState([]);
  const [stack, setStack] = useState([]);
  const [currentStep, setCurrentStep] = useState(0);
  const [isRunning, setIsRunning] = useState(false);
  const [isCompleted, setIsCompleted] = useState(false);
  const [showCode, setShowCode] = useState(false);
  const [animationSpeed, setAnimationSpeed] = useState(1000);
  const [history, setHistory] = useState([]);
  const [currentExplanation, setCurrentExplanation] = useState('');
  
  // Refs for animation
  const animationRef = useRef(null);
  const canvasRef = useRef(null);
  
  // Function to reset the visualization
  const resetVisualization = () => {
    setVisited([]);
    setQueue([]);
    setStack([]);
    setCurrentStep(0);
    setIsRunning(false);
    setIsCompleted(false);
    setHistory([]);
    setCurrentExplanation('Click "Start" to begin the traversal.');
  };
  
  // Function to get next vertex in BFS
  const getNextBFSVertex = () => {
    if (queue.length === 0) return null;
    
    const current = queue[0];
    const newQueue = [...queue.slice(1)];
    
    // Get unvisited neighbors
    const neighbors = adjacencyList[current] || [];
    const unvisitedNeighbors = neighbors.filter(v => !visited.includes(v) && !newQueue.includes(v));
    
    return {
      current,
      newQueue,
      newVisited: [...visited, current],
      toEnqueue: unvisitedNeighbors,
      finalQueue: [...newQueue, ...unvisitedNeighbors]
    };
  };
  
  // Function to get next vertex in DFS
  const getNextDFSVertex = () => {
    if (stack.length === 0) return null;
    
    const current = stack[stack.length - 1];
    const newStack = [...stack.slice(0, -1)];
    
    // Get unvisited neighbors
    const neighbors = adjacencyList[current] || [];
    const unvisitedNeighbors = neighbors.filter(v => !visited.includes(v) && !stack.includes(v));
    
    // For DFS, we need to reverse the neighbors to match the recursive implementation order
    const reversedNeighbors = [...unvisitedNeighbors].reverse();
    
    return {
      current,
      newStack,
      newVisited: [...visited, current],
      toPush: reversedNeighbors,
      finalStack: [...newStack, ...reversedNeighbors]
    };
  };

  // Function to run one step of the traversal
  const runStep = () => {
    if (isCompleted) return;
    
    // First step: initialize
    if (currentStep === 0) {
      if (algorithm === 'bfs') {
        setQueue([startVertex]);
        setCurrentExplanation(`Starting BFS from vertex ${startVertex}. Add ${startVertex} to the queue.`);
        setHistory([...history, { visited: [], queue: [startVertex], stack: [], explanation: `Starting BFS from vertex ${startVertex}. Add ${startVertex} to the queue.` }]);
      } else {
        setStack([startVertex]);
        setCurrentExplanation(`Starting DFS from vertex ${startVertex}. Add ${startVertex} to the stack.`);
        setHistory([...history, { visited: [], queue: [], stack: [startVertex], explanation: `Starting DFS from vertex ${startVertex}. Add ${startVertex} to the stack.` }]);
      }
      setCurrentStep(currentStep + 1);
      return;
    }
    
    // BFS traversal
    if (algorithm === 'bfs') {
      const next = getNextBFSVertex();
      
      if (!next) {
        setIsCompleted(true);
        setIsRunning(false);
        setCurrentExplanation('Traversal completed! All vertices have been visited.');
        setHistory([...history, { visited, queue, stack, explanation: 'Traversal completed! All vertices have been visited.' }]);
        return;
      }
      
      const { current, newQueue, newVisited, toEnqueue, finalQueue } = next;
      
      let explanation = `Dequeue ${current} and mark as visited.`;
      if (toEnqueue.length > 0) {
        explanation += ` Enqueue unvisited neighbors: ${toEnqueue.join(', ')}.`;
      } else {
        explanation += ` No unvisited neighbors to enqueue.`;
      }
      
      setVisited(newVisited);
      setQueue(finalQueue);
      setCurrentExplanation(explanation);
      setHistory([...history, { visited: newVisited, queue: finalQueue, stack: [], explanation }]);
      
    } 
    // DFS traversal
    else {
      const next = getNextDFSVertex();
      
      if (!next) {
        setIsCompleted(true);
        setIsRunning(false);
        setCurrentExplanation('Traversal completed! All vertices have been visited.');
        setHistory([...history, { visited, queue, stack, explanation: 'Traversal completed! All vertices have been visited.' }]);
        return;
      }
      
      const { current, newStack, newVisited, toPush, finalStack } = next;
      
      let explanation = `Pop ${current} from the stack and mark as visited.`;
      if (toPush.length > 0) {
        explanation += ` Push unvisited neighbors onto the stack: ${toPush.join(', ')}.`;
      } else {
        explanation += ` No unvisited neighbors to push.`;
      }
      
      setVisited(newVisited);
      setStack(finalStack);
      setCurrentExplanation(explanation);
      setHistory([...history, { visited: newVisited, queue: [], stack: finalStack, explanation }]);
    }
    
    setCurrentStep(currentStep + 1);
    
    // Check if traversal is complete
    if ((algorithm === 'bfs' && queue.length === 0) || 
        (algorithm === 'dfs' && stack.length === 0)) {
      setIsCompleted(true);
      setIsRunning(false);
    }
  };
  
  // Function to start/pause the visualization
  const toggleRunning = () => {
    if (isCompleted) {
      resetVisualization();
      return;
    }
    
    setIsRunning(!isRunning);
  };
  
  // Function to run to completion
  const runToCompletion = () => {
    if (isCompleted) {
      resetVisualization();
      return;
    }
    
    // Run each step manually
    if (currentStep === 0) {
      // Initialize
      if (algorithm === 'bfs') {
        setQueue([startVertex]);
      } else {
        setStack([startVertex]);
      }
      
      // Recursively run all steps
      const runAllSteps = (step, visited, queue, stack) => {
        if (step === 0) {
          if (algorithm === 'bfs') {
            return runAllSteps(1, visited, [startVertex], stack);
          } else {
            return runAllSteps(1, visited, queue, [startVertex]);
          }
        }
        
        if (algorithm === 'bfs') {
          if (queue.length === 0) return { visited, queue, stack };
          
          const current = queue[0];
          const newQueue = [...queue.slice(1)];
          const newVisited = [...visited, current];
          
          const neighbors = adjacencyList[current] || [];
          const unvisitedNeighbors = neighbors.filter(v => !newVisited.includes(v) && !newQueue.includes(v));
          const finalQueue = [...newQueue, ...unvisitedNeighbors];
          
          return runAllSteps(step + 1, newVisited, finalQueue, stack);
        } else {
          if (stack.length === 0) return { visited, queue, stack };
          
          const current = stack[stack.length - 1];
          const newStack = [...stack.slice(0, -1)];
          const newVisited = [...visited, current];
          
          const neighbors = adjacencyList[current] || [];
          const unvisitedNeighbors = neighbors.filter(v => !newVisited.includes(v) && !newStack.includes(v));
          const reversedNeighbors = [...unvisitedNeighbors].reverse();
          const finalStack = [...newStack, ...reversedNeighbors];
          
          return runAllSteps(step + 1, newVisited, queue, finalStack);
        }
      };
      
      // Run all steps and get final state
      const finalState = runAllSteps(0, [], [], []);
      
      // Update the state
      setVisited(finalState.visited);
      setQueue(finalState.queue);
      setStack(finalState.stack);
      setCurrentStep(finalState.visited.length + 1);
      setIsCompleted(true);
      setCurrentExplanation(`Traversal completed! Visited order: ${finalState.visited.join(', ')}`);
      
      // Create a simplified history for animation purposes
      const simpleHistory = finalState.visited.map((v, i) => ({
        visited: finalState.visited.slice(0, i + 1),
        queue: algorithm === 'bfs' ? finalState.visited.slice(i + 1) : [],
        stack: algorithm === 'dfs' ? finalState.visited.slice(i + 1).reverse() : [],
        explanation: `Visited ${v}`
      }));
      
      setHistory([
        { visited: [], queue: algorithm === 'bfs' ? [startVertex] : [], stack: algorithm === 'dfs' ? [startVertex] : [], explanation: `Starting ${algorithm.toUpperCase()} from vertex ${startVertex}` },
        ...simpleHistory
      ]);
    } else {
      // Just show the final result
      const allVertices = [...vertices];
      const finalVisited = [...new Set([...visited, ...queue, ...stack])].filter(v => allVertices.includes(v));
      
      setVisited(finalVisited);
      setQueue([]);
      setStack([]);
      setIsCompleted(true);
      setCurrentExplanation(`Traversal completed! Visited order: ${finalVisited.join(', ')}`);
    }
  };
  
  // Function to draw the graph
  const drawGraph = () => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const width = canvas.width;
    const height = canvas.height;
    
    // Clear canvas
    ctx.clearRect(0, 0, width, height);
    
    // Calculate vertex positions in a circle
    const centerX = width / 2;
    const centerY = height / 2;
    const radius = Math.min(width, height) / 2.5;
    
    const vertexPositions = {};
    vertices.forEach((vertex, index) => {
      const angle = (2 * Math.PI * index) / vertices.length - Math.PI / 2; // Start from top
      vertexPositions[vertex] = {
        x: centerX + radius * Math.cos(angle),
        y: centerY + radius * Math.sin(angle)
      };
    });
    
    // Draw edges
    ctx.strokeStyle = '#888888';
    ctx.lineWidth = 2;
    
    vertices.forEach(vertex => {
      const neighbors = adjacencyList[vertex] || [];
      neighbors.forEach(neighbor => {
        if (vertex < neighbor) { // Draw each edge only once for undirected graph
          const fromPos = vertexPositions[vertex];
          const toPos = vertexPositions[neighbor];
          
          ctx.beginPath();
          ctx.moveTo(fromPos.x, fromPos.y);
          ctx.lineTo(toPos.x, toPos.y);
          ctx.stroke();
        }
      });
    });
    
    // Draw vertices
    vertices.forEach(vertex => {
      const pos = vertexPositions[vertex];
      
      // Determine vertex status
      let fillColor = '#333333'; // Default color
      let strokeColor = '#000000'; // Default border
      let textColor = '#FFFFFF'; // Default text color
      
      if (vertex === startVertex) {
        strokeColor = '#00FF00'; // Start vertex has green border
      }
      
      if (visited.includes(vertex)) {
        fillColor = '#FF00FF'; // Visited vertices are pink
        textColor = '#FFFFFF';
      } else if (algorithm === 'bfs' && queue.includes(vertex)) {
        fillColor = '#00FFFF'; // Queue vertices are cyan
        textColor = '#000000';
      } else if (algorithm === 'dfs' && stack.includes(vertex)) {
        fillColor = '#FFFF00'; // Stack vertices are yellow
        textColor = '#000000';
      }
      
      // Draw vertex circle
      ctx.beginPath();
      ctx.arc(pos.x, pos.y, 25, 0, 2 * Math.PI);
      ctx.fillStyle = fillColor;
      ctx.fill();
      ctx.strokeStyle = strokeColor;
      ctx.lineWidth = 3;
      ctx.stroke();
      
      // Draw vertex label
      ctx.fillStyle = textColor;
      ctx.font = 'bold 16px Arial';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(vertex, pos.x, pos.y);
    });
  };
  
  // Effect to run animation when isRunning changes
  useEffect(() => {
    if (isRunning && !isCompleted) {
      animationRef.current = setTimeout(() => {
        runStep();
      }, animationSpeed);
    } else {
      clearTimeout(animationRef.current);
    }
    
    return () => {
      clearTimeout(animationRef.current);
    };
  }, [isRunning, currentStep, isCompleted]);
  
  // Effect to draw the graph when anything changes
  useEffect(() => {
    drawGraph();
  }, [vertices, adjacencyList, visited, queue, stack, startVertex, algorithm]);
  
  // Effect to reset visualization when algorithm or start vertex changes
  useEffect(() => {
    resetVisualization();
  }, [algorithm, startVertex, adjacencyList]);
  
  // Generate explanation text for the traversal order
  const getTraversalOrderText = () => {
    if (visited.length === 0) return '';
    return `Traversal Order: ${visited.join(' → ')}`;
  };
  
  // Generate explanation text for the current data structure
  const getCurrentDataStructureText = () => {
    if (algorithm === 'bfs') {
      return `Queue: ${queue.length > 0 ? queue.join(', ') : '(empty)'}`;
    } else {
      return `Stack: ${stack.length > 0 ? stack.join(', ') : '(empty)'}`;
    }
  };
  
  // BFS code
  const bfsCode = `void BFS(const Graph& graph, int startVertex) {
  // Create a queue for BFS
  std::queue<int> queue;
  
  // Create a set to keep track of visited vertices
  std::unordered_set<int> visited;
  
  // Mark the start vertex as visited and enqueue it
  visited.insert(startVertex);
  queue.push(startVertex);
  
  while (!queue.empty()) {
    // Dequeue a vertex from queue and print it
    int currentVertex = queue.front();
    queue.pop();
    std::cout << currentVertex << " ";
    
    // Get all adjacent vertices of the dequeued vertex
    // If an adjacent vertex hasn't been visited, mark it
    // visited and enqueue it
    for (int neighbor : graph.getNeighbors(currentVertex)) {
      if (visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        queue.push(neighbor);
      }
    }
  }
}`;

  // DFS code
  const dfsCode = `void DFS(const Graph& graph, int startVertex) {
  // Create a stack for DFS
  std::stack<int> stack;
  
  // Create a set to keep track of visited vertices
  std::unordered_set<int> visited;
  
  // Push the start vertex
  stack.push(startVertex);
  
  while (!stack.empty()) {
    // Pop a vertex from stack
    int currentVertex = stack.top();
    stack.pop();
    
    // If the vertex is not visited, mark it as visited and print it
    if (visited.find(currentVertex) == visited.end()) {
      visited.insert(currentVertex);
      std::cout << currentVertex << " ";
      
      // Push all adjacent vertices of the popped vertex
      // (in reverse order to match recursive DFS)
      std::vector<int> neighbors = graph.getNeighbors(currentVertex);
      for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
        if (visited.find(*it) == visited.end()) {
          stack.push(*it);
        }
      }
    }
  }
}`;

  return (
    <div 
      style={{
        fontFamily: 'Arial, sans-serif',
        maxWidth: '1000px',
        margin: '0 auto',
        padding: '20px',
        backgroundColor: '#121212',
        color: '#ffffff',
        borderRadius: '8px',
      }}
    >
      <h1 
        style={{
          textAlign: 'center',
          color: '#00FFFF',
          marginBottom: '20px',
          borderBottom: '2px solid #00FFFF',
          paddingBottom: '10px',
        }}
      >
        Day 30: Graph Traversal Algorithms
      </h1>
      
      <div 
        style={{
          display: 'flex',
          justifyContent: 'space-between',
          marginBottom: '20px',
          gap: '20px',
          flexWrap: 'wrap',
        }}
      >
        <div 
          style={{
            flex: '1',
            minWidth: '300px',
            backgroundColor: '#222222',
            padding: '15px',
            borderRadius: '8px',
            border: '1px solid #444444',
          }}
        >
          <h2 
            style={{
              color: '#FFFF00',
              marginTop: '0',
              marginBottom: '15px',
              borderBottom: '1px solid #444444',
              paddingBottom: '5px',
            }}
          >
            Graph Configuration
          </h2>
          
          <div 
            style={{
              marginBottom: '15px',
            }}
          >
            <label 
              style={{
                display: 'block',
                marginBottom: '5px',
                color: '#DDDDDD',
              }}
            >
              Start Vertex:
            </label>
            <select 
              value={startVertex}
              onChange={(e) => setStartVertex(e.target.value)}
              style={{
                width: '100%',
                padding: '8px',
                backgroundColor: '#333333',
                color: '#FFFFFF',
                border: '1px solid #555555',
                borderRadius: '4px',
              }}
            >
              {vertices.map(vertex => (
                <option key={vertex} value={vertex}>{vertex}</option>
              ))}
            </select>
          </div>
          
          <div 
            style={{
              marginBottom: '15px',
            }}
          >
            <label 
              style={{
                display: 'block',
                marginBottom: '5px',
                color: '#DDDDDD',
              }}
            >
              Algorithm:
            </label>
            <div 
              style={{
                display: 'flex',
                gap: '10px',
              }}
            >
              <button 
                onClick={() => setAlgorithm('bfs')}
                style={{
                  flex: '1',
                  padding: '8px',
                  backgroundColor: algorithm === 'bfs' ? '#00FFFF' : '#333333',
                  color: algorithm === 'bfs' ? '#000000' : '#FFFFFF',
                  border: 'none',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  fontWeight: algorithm === 'bfs' ? 'bold' : 'normal',
                }}
              >
                BFS
              </button>
              <button 
                onClick={() => setAlgorithm('dfs')}
                style={{
                  flex: '1',
                  padding: '8px',
                  backgroundColor: algorithm === 'dfs' ? '#FFFF00' : '#333333',
                  color: algorithm === 'dfs' ? '#000000' : '#FFFFFF',
                  border: 'none',
                  borderRadius: '4px',
                  cursor: 'pointer',
                  fontWeight: algorithm === 'dfs' ? 'bold' : 'normal',
                }}
              >
                DFS
              </button>
            </div>
          </div>
          
          <div 
            style={{
              marginBottom: '15px',
            }}
          >
            <label 
              style={{
                display: 'block',
                marginBottom: '5px',
                color: '#DDDDDD',
              }}
            >
              Animation Speed:
            </label>
            <div 
              style={{
                display: 'flex',
                alignItems: 'center',
              }}
            >
              <span 
                style={{
                  marginRight: '10px',
                  color: '#BBBBBB',
                }}
              >
                Fast
              </span>
              <input 
                type="range"
                min="200"
                max="2000"
                step="100"
                value={2200 - animationSpeed}
                onChange={(e) => setAnimationSpeed(2200 - parseInt(e.target.value))}
                style={{
                  flex: '1',
                }}
              />
              <span 
                style={{
                  marginLeft: '10px',
                  color: '#BBBBBB',
                }}
              >
                Slow
              </span>
            </div>
          </div>
          
          <div 
            style={{
              display: 'flex',
              gap: '10px',
              marginTop: '20px',
            }}
          >
            <button 
              onClick={toggleRunning}
              style={{
                flex: '1',
                padding: '10px',
                backgroundColor: isRunning ? '#FF5555' : '#00FF00',
                color: '#000000',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                fontWeight: 'bold',
              }}
            >
              {isRunning ? (
                <>Pause</>
              ) : isCompleted ? (
                <>Reset</>
              ) : (
                <><Play size={16} style={{ marginRight: '5px' }} /> Start</>
              )}
            </button>
            
            <button 
              onClick={runStep}
              disabled={isRunning || isCompleted}
              style={{
                flex: '1',
                padding: '10px',
                backgroundColor: isRunning || isCompleted ? '#555555' : '#FF00FF',
                color: isRunning || isCompleted ? '#888888' : '#000000',
                border: 'none',
                borderRadius: '4px',
                cursor: isRunning || isCompleted ? 'not-allowed' : 'pointer',
                fontWeight: 'bold',
              }}
            >
              Step
            </button>
            
            <button 
              onClick={runToCompletion}
              disabled={isRunning || isCompleted}
              style={{
                flex: '1',
                padding: '10px',
                backgroundColor: isRunning || isCompleted ? '#555555' : '#FFFF00',
                color: isRunning || isCompleted ? '#888888' : '#000000',
                border: 'none',
                borderRadius: '4px',
                cursor: isRunning || isCompleted ? 'not-allowed' : 'pointer',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                fontWeight: 'bold',
              }}
            >
              <SkipForward size={16} style={{ marginRight: '5px' }} /> Finish
            </button>
          </div>
          
          <div 
            style={{
              marginTop: '20px',
              display: 'flex',
              gap: '10px',
            }}
          >
            <button 
              onClick={() => setShowCode(!showCode)}
              style={{
                flex: '1',
                padding: '10px',
                backgroundColor: showCode ? '#FFFF00' : '#333333',
                color: showCode ? '#000000' : '#FFFFFF',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
              }}
            >
              <Code size={16} style={{ marginRight: '5px' }} /> {showCode ? 'Hide Code' : 'Show Code'}
            </button>
            
            <button 
              onClick={resetVisualization}
              style={{
                flex: '1',
                padding: '10px',
                backgroundColor: '#333333',
                color: '#FFFFFF',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
              }}
            >
              <RefreshCw size={16} style={{ marginRight: '5px' }} /> Reset
            </button>
          </div>
        </div>
        
        <div 
          style={{
            flex: '2',
            minWidth: '400px',
            backgroundColor: '#222222',
            padding: '15px',
            borderRadius: '8px',
            border: '1px solid #444444',
            display: 'flex',
            flexDirection: 'column',
          }}
        >
          <h2 
            style={{
              color: algorithm === 'bfs' ? '#00FFFF' : '#FFFF00',
              marginTop: '0',
              marginBottom: '15px',
              textAlign: 'center',
              borderBottom: '1px solid #444444',
              paddingBottom: '5px',
            }}
          >
            {algorithm === 'bfs' ? 'Breadth-First Search (BFS)' : 'Depth-First Search (DFS)'}
          </h2>
          
          <div 
            style={{
              flex: '1',
              position: 'relative',
              backgroundColor: '#111111',
              borderRadius: '4px',
              marginBottom: '15px',
              overflow: 'hidden',
            }}
          >
            <canvas 
              ref={canvasRef}
              width={600}
              height={400}
              style={{
                width: '100%',
                height: '100%',
              }}
            />
          </div>
          
          <div 
            style={{
              marginBottom: '15px',
              backgroundColor: '#333333',
              padding: '10px',
              borderRadius: '4px',
              color: '#FFFFFF',
            }}
          >
            <div style={{ marginBottom: '8px' }}>
              <strong>Legend:</strong>
            </div>
            <div style={{ display: 'flex', flexWrap: 'wrap', gap: '15px' }}>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{
                    width: '20px',
                    height: '20px',
                    backgroundColor: '#333333',
                    borderRadius: '50%',
                    marginRight: '5px',
                    border: '2px solid #000000',
                  }}
                ></div>
                <span>Unvisited</span>
              </div>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{
                    width: '20px',
                    height: '20px',
                    backgroundColor: '#FF00FF',
                    borderRadius: '50%',
                    marginRight: '5px',
                    border: '2px solid #000000',
                  }}
                ></div>
                <span>Visited</span>
              </div>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{
                    width: '20px',
                    height: '20px',
                    backgroundColor: algorithm === 'bfs' ? '#00FFFF' : '#FFFF00',
                    borderRadius: '50%',
                    marginRight: '5px',
                    border: '2px solid #000000',
                  }}
                ></div>
                <span>{algorithm === 'bfs' ? 'In Queue' : 'In Stack'}</span>
              </div>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{
                    width: '20px',
                    height: '20px',
                    backgroundColor: '#333333',
                    borderRadius: '50%',
                    marginRight: '5px',
                    border: '2px solid #00FF00',
                  }}
                ></div>
                <span>Start Vertex</span>
              </div>
            </div>
          </div>
          
          <div 
            style={{
              backgroundColor: '#333333',
              padding: '15px',
              borderRadius: '4px',
              marginBottom: '15px',
            }}
          >
            <div 
              style={{
                fontSize: '16px',
                marginBottom: '10px',
                color: algorithm === 'bfs' ? '#00FFFF' : '#FFFF00',
              }}
            >
              {currentExplanation}
            </div>
            <div 
              style={{
                display: 'flex',
                justifyContent: 'space-between',
                color: '#FFFFFF',
              }}
            >
              <div>{getTraversalOrderText()}</div>
              <div>{getCurrentDataStructureText()}</div>
            </div>
          </div>
        </div>
      </div>
      
      {showCode && (
        <div 
          style={{
            backgroundColor: '#222222',
            padding: '15px',
            borderRadius: '8px',
            border: '1px solid #444444',
            marginTop: '20px',
            overflow: 'auto',
          }}
        >
          <h2 
            style={{
              color: algorithm === 'bfs' ? '#00FFFF' : '#FFFF00',
              marginTop: '0',
              marginBottom: '15px',
              borderBottom: '1px solid #444444',
              paddingBottom: '5px',
            }}
          >
            {algorithm === 'bfs' ? 'BFS Implementation (C++)' : 'DFS Implementation (C++)'}
          </h2>
          
          <pre 
            style={{
              backgroundColor: '#111111',
              padding: '15px',
              borderRadius: '4px',
              overflow: 'auto',
              fontSize: '14px',
              lineHeight: '1.4',
              color: '#FFFFFF',
            }}
          >
            {algorithm === 'bfs' ? bfsCode : dfsCode}
          </pre>
          
          <div 
            style={{
              marginTop: '15px',
              fontSize: '14px',
              color: '#BBBBBB',
            }}
          >
            <h3 style={{ color: '#FFFFFF', marginTop: '0' }}>Key Characteristics:</h3>
            
            {algorithm === 'bfs' ? (
              <ul>
                <li>Uses a <strong style={{ color: '#00FFFF' }}>queue</strong> (First In, First Out) data structure</li>
                <li>Visits all neighbors at the current depth before moving to the next level</li>
                <li>Guarantees the <strong style={{ color: '#00FFFF' }}>shortest path</strong> in unweighted graphs</li>
                <li>Time Complexity: O(V + E)</li>
                <li>Space Complexity: O(V)</li>
              </ul>
            ) : (
              <ul>
                <li>Uses a <strong style={{ color: '#FFFF00' }}>stack</strong> (Last In, First Out) data structure</li>
                <li>Explores as far as possible along each branch before backtracking</li>
                <li>Useful for <strong style={{ color: '#FFFF00' }}>topological sorting</strong> and <strong style={{ color: '#FFFF00' }}>cycle detection</strong></li>
                <li>Time Complexity: O(V + E)</li>
                <li>Space Complexity: O(V)</li>
              </ul>
            )}
          </div>
        </div>
      )}
      
      <div 
        style={{
          marginTop: '20px',
          textAlign: 'center',
          color: '#888888',
          fontSize: '14px',
        }}
      >
        #DSAin45 Day 30: Graph Traversal Algorithms
      </div>
    </div>
  );
};

export default GraphTraversalVisualization;
