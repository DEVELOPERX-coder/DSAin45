import React, { useState, useEffect, useRef } from 'react';
import { Play, Pause, SkipForward, SkipBack, RotateCw, Code } from 'lucide-react';

// Main component
export default function TopologicalSortVisualizer() {
  // Graph state
  const [graph, setGraph] = useState(null);
  const [algorithm, setAlgorithm] = useState('dfs');
  const [animationSpeed, setAnimationSpeed] = useState(1000);
  const [isRunning, setIsRunning] = useState(false);
  const [currentStep, setCurrentStep] = useState(0);
  const [steps, setSteps] = useState([]);
  const [showCode, setShowCode] = useState(false);
  const [topologicalOrder, setTopologicalOrder] = useState([]);
  
  // Canvas refs
  const canvasRef = useRef(null);
  const animationRef = useRef(null);
  
  // Initialize graph
  useEffect(() => {
    // Create a DAG for topological sorting
    const initialGraph = {
      nodes: [
        { id: 'A', x: 100, y: 100, color: '#00FFFF', status: 'unvisited' },
        { id: 'B', x: 200, y: 50, color: '#00FFFF', status: 'unvisited' },
        { id: 'C', x: 300, y: 100, color: '#00FFFF', status: 'unvisited' },
        { id: 'D', x: 400, y: 50, color: '#00FFFF', status: 'unvisited' },
        { id: 'E', x: 400, y: 150, color: '#00FFFF', status: 'unvisited' },
        { id: 'F', x: 300, y: 200, color: '#00FFFF', status: 'unvisited' },
        { id: 'G', x: 200, y: 150, color: '#00FFFF', status: 'unvisited' },
        { id: 'H', x: 100, y: 200, color: '#00FFFF', status: 'unvisited' },
      ],
      edges: [
        { source: 'A', target: 'B', color: '#FFFFFF' },
        { source: 'A', target: 'C', color: '#FFFFFF' },
        { source: 'B', target: 'D', color: '#FFFFFF' },
        { source: 'C', target: 'D', color: '#FFFFFF' },
        { source: 'C', target: 'E', color: '#FFFFFF' },
        { source: 'D', target: 'F', color: '#FFFFFF' },
        { source: 'E', target: 'F', color: '#FFFFFF' },
        { source: 'F', target: 'G', color: '#FFFFFF' },
        { source: 'G', target: 'H', color: '#FFFFFF' },
      ]
    };
    
    setGraph(initialGraph);
    
    // Generate algorithm steps
    generateSteps(initialGraph, algorithm);
    
  }, [algorithm]);
  
  // Draw the graph
  useEffect(() => {
    if (!graph) return;
    
    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw edges with arrows
    graph.edges.forEach(edge => {
      const source = graph.nodes.find(node => node.id === edge.source);
      const target = graph.nodes.find(node => node.id === edge.target);
      
      // Calculate direction vector
      const dx = target.x - source.x;
      const dy = target.y - source.y;
      const length = Math.sqrt(dx * dx + dy * dy);
      
      // Normalize direction
      const ndx = dx / length;
      const ndy = dy / length;
      
      // Calculate start and end points (adjusted for node radius)
      const nodeRadius = 20;
      const startX = source.x + ndx * nodeRadius;
      const startY = source.y + ndy * nodeRadius;
      const endX = target.x - ndx * nodeRadius;
      const endY = target.y - ndy * nodeRadius;
      
      // Draw the line
      ctx.beginPath();
      ctx.moveTo(startX, startY);
      ctx.lineTo(endX, endY);
      ctx.strokeStyle = edge.color;
      ctx.lineWidth = 2;
      ctx.stroke();
      
      // Draw arrowhead
      const arrowLength = 10;
      const arrowWidth = 5;
      
      // Calculate perpendicular vector
      const perpX = -ndy;
      const perpY = ndx;
      
      // Calculate arrow points
      const arrowTip = { x: endX, y: endY };
      const arrowLeft = { 
        x: endX - arrowLength * ndx + arrowWidth * perpX, 
        y: endY - arrowLength * ndy + arrowWidth * perpY 
      };
      const arrowRight = { 
        x: endX - arrowLength * ndx - arrowWidth * perpX, 
        y: endY - arrowLength * ndy - arrowWidth * perpY 
      };
      
      // Draw arrowhead
      ctx.beginPath();
      ctx.moveTo(arrowTip.x, arrowTip.y);
      ctx.lineTo(arrowLeft.x, arrowLeft.y);
      ctx.lineTo(arrowRight.x, arrowRight.y);
      ctx.closePath();
      ctx.fillStyle = edge.color;
      ctx.fill();
    });
    
    // Draw nodes
    graph.nodes.forEach(node => {
      // Node circle
      ctx.beginPath();
      ctx.arc(node.x, node.y, 20, 0, 2 * Math.PI);
      ctx.fillStyle = '#121212';
      ctx.fill();
      ctx.strokeStyle = node.color;
      ctx.lineWidth = 3;
      ctx.stroke();
      
      // Node label
      ctx.fillStyle = node.color;
      ctx.font = 'bold 16px Arial';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(node.id, node.x, node.y);
      
      // Node status indicator (small dot)
      if (node.status === 'processing') {
        ctx.beginPath();
        ctx.arc(node.x + 14, node.y - 14, 5, 0, 2 * Math.PI);
        ctx.fillStyle = '#FFFF00';
        ctx.fill();
      } else if (node.status === 'visited') {
        ctx.beginPath();
        ctx.arc(node.x + 14, node.y - 14, 5, 0, 2 * Math.PI);
        ctx.fillStyle = '#00FF00';
        ctx.fill();
      }
    });
    
  }, [graph]);
  
  // Animation control
  useEffect(() => {
    if (isRunning && currentStep < steps.length - 1) {
      animationRef.current = setTimeout(() => {
        setCurrentStep(prev => prev + 1);
        updateGraphState(steps[currentStep + 1]);
      }, animationSpeed);
      
      return () => clearTimeout(animationRef.current);
    } else if (currentStep >= steps.length - 1) {
      setIsRunning(false);
    }
  }, [isRunning, currentStep, steps, animationSpeed]);
  
  // Update graph based on current step
  const updateGraphState = (step) => {
    if (!step) return;
    
    setGraph(prevGraph => {
      const newGraph = JSON.parse(JSON.stringify(prevGraph));
      
      // Update node colors and status
      newGraph.nodes.forEach(node => {
        const stepNode = step.nodes.find(n => n.id === node.id);
        if (stepNode) {
          node.color = stepNode.color;
          node.status = stepNode.status;
        }
      });
      
      // Update edge colors
      newGraph.edges.forEach(edge => {
        const stepEdge = step.edges.find(e => 
          e.source === edge.source && e.target === edge.target);
        if (stepEdge) {
          edge.color = stepEdge.color;
        }
      });
      
      return newGraph;
    });
    
    // Update topological order
    setTopologicalOrder(step.topologicalOrder || []);
  };
  
  // Generate algorithm steps
  const generateSteps = (graphData, algoType) => {
    // Create deep copies to work with
    const nodes = JSON.parse(JSON.stringify(graphData.nodes));
    const edges = JSON.parse(JSON.stringify(graphData.edges));
    
    const newSteps = [];
    
    // Initial state
    newSteps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: 'Initial directed acyclic graph (DAG)',
      topologicalOrder: []
    });
    
    if (algoType === 'dfs') {
      generateDFSTopologicalSort(nodes, edges, newSteps);
    } else {
      generateKahnTopologicalSort(nodes, edges, newSteps);
    }
    
    setSteps(newSteps);
    setCurrentStep(0);
    
    // Apply initial step
    if (newSteps.length > 0) {
      updateGraphState(newSteps[0]);
    }
  };
  
  // Generate DFS-based topological sort steps
  const generateDFSTopologicalSort = (nodes, edges, steps) => {
    // Create adjacency list
    const adjList = {};
    nodes.forEach(node => {
      adjList[node.id] = [];
    });
    
    edges.forEach(edge => {
      adjList[edge.source].push(edge.target);
    });
    
    // Track visited status
    const visited = {};
    const beingVisited = {};
    const topOrder = [];
    
    // Helper function for DFS
    const dfs = (nodeId) => {
      // Mark node as being visited
      beingVisited[nodeId] = true;
      const node = nodes.find(n => n.id === nodeId);
      node.status = 'processing';
      node.color = '#FFFF00';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Starting DFS visit for node ${nodeId}`,
        topologicalOrder: [...topOrder]
      });
      
      // Visit all neighbors
      for (const neighbor of adjList[nodeId]) {
        // Highlight current edge
        const edgeIndex = edges.findIndex(e => e.source === nodeId && e.target === neighbor);
        edges[edgeIndex].color = '#FFFF00';
        
        steps.push({
          nodes: JSON.parse(JSON.stringify(nodes)),
          edges: JSON.parse(JSON.stringify(edges)),
          description: `Checking edge ${nodeId} → ${neighbor}`,
          topologicalOrder: [...topOrder]
        });
        
        if (!visited[neighbor] && !beingVisited[neighbor]) {
          dfs(neighbor);
        }
        
        // Reset edge color
        edges[edgeIndex].color = '#FFFFFF';
      }
      
      // Mark as visited and add to topological order
      visited[nodeId] = true;
      beingVisited[nodeId] = false;
      node.status = 'visited';
      node.color = '#FF00FF';
      topOrder.unshift(nodeId);
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Finished processing node ${nodeId}, adding to topological order`,
        topologicalOrder: [...topOrder]
      });
    };
    
    // Run DFS for each unvisited node
    for (const node of nodes) {
      if (!visited[node.id] && !beingVisited[node.id]) {
        dfs(node.id);
      }
    }
    
    // Highlight final order
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `DFS-based topological sort completed: [${topOrder.join(' → ')}]`,
      topologicalOrder: [...topOrder]
    });
  };
  
  // Generate Kahn's algorithm steps (BFS-based)
  const generateKahnTopologicalSort = (nodes, edges, steps) => {
    // Create adjacency list and calculate in-degrees
    const adjList = {};
    const inDegree = {};
    
    nodes.forEach(node => {
      adjList[node.id] = [];
      inDegree[node.id] = 0;
    });
    
    edges.forEach(edge => {
      adjList[edge.source].push(edge.target);
      inDegree[edge.target] = (inDegree[edge.target] || 0) + 1;
    });
    
    // Create queue with all nodes that have in-degree of 0
    const queue = [];
    nodes.forEach(node => {
      if (inDegree[node.id] === 0) {
        queue.push(node.id);
        node.color = '#FFFF00';
        node.status = 'processing';
      }
    });
    
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `Kahn's algorithm: Starting with nodes that have no incoming edges`,
      topologicalOrder: []
    });
    
    const topOrder = [];
    
    // Process queue
    while (queue.length > 0) {
      const currentId = queue.shift();
      const currentNode = nodes.find(n => n.id === currentId);
      
      // Add to topological order
      topOrder.push(currentId);
      currentNode.color = '#FF00FF';
      currentNode.status = 'visited';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Adding node ${currentId} to topological order (no dependencies)`,
        topologicalOrder: [...topOrder]
      });
      
      // Process all outgoing edges
      for (const neighborId of adjList[currentId]) {
        // Highlight current edge
        const edgeIndex = edges.findIndex(e => e.source === currentId && e.target === neighborId);
        edges[edgeIndex].color = '#FFFF00';
        
        steps.push({
          nodes: JSON.parse(JSON.stringify(nodes)),
          edges: JSON.parse(JSON.stringify(edges)),
          description: `Removing edge ${currentId} → ${neighborId}`,
          topologicalOrder: [...topOrder]
        });
        
        // Decrease in-degree of neighbor
        inDegree[neighborId]--;
        
        // If in-degree becomes 0, add to queue
        if (inDegree[neighborId] === 0) {
          queue.push(neighborId);
          const neighborNode = nodes.find(n => n.id === neighborId);
          neighborNode.color = '#FFFF00';
          neighborNode.status = 'processing';
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Node ${neighborId} has no more dependencies, adding to queue`,
            topologicalOrder: [...topOrder]
          });
        }
        
        // Mark edge as processed
        edges[edgeIndex].color = '#00FF00';
      }
    }
    
    // Check if topological sort was successful
    if (topOrder.length !== nodes.length) {
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Graph contains a cycle, topological sort not possible`,
        topologicalOrder: [...topOrder]
      });
    } else {
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Kahn's algorithm completed: [${topOrder.join(' → ')}]`,
        topologicalOrder: [...topOrder]
      });
    }
  };
  
  // UI Control handlers
  const handlePlay = () => {
    setIsRunning(true);
  };
  
  const handlePause = () => {
    setIsRunning(false);
  };
  
  const handleReset = () => {
    setIsRunning(false);
    setCurrentStep(0);
    if (steps.length > 0) {
      updateGraphState(steps[0]);
    }
  };
  
  const handleNext = () => {
    if (currentStep < steps.length - 1) {
      setCurrentStep(prev => prev + 1);
      updateGraphState(steps[currentStep + 1]);
    }
  };
  
  const handlePrev = () => {
    if (currentStep > 0) {
      setCurrentStep(prev => prev - 1);
      updateGraphState(steps[currentStep - 1]);
    }
  };
  
  const handleAlgorithmChange = (e) => {
    setIsRunning(false);
    setAlgorithm(e.target.value);
  };
  
  const handleSpeedChange = (e) => {
    setAnimationSpeed(parseInt(e.target.value));
  };
  
  const toggleCodeView = () => {
    setShowCode(!showCode);
  };
  
  // Code snippets
  const dfsCode = `// DFS-based Topological Sort in C++
vector<int> topologicalSortDFS(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> result;
    vector<bool> visited(n, false);
    vector<bool> inStack(n, false); // For cycle detection
    
    function<bool(int)> dfs = [&](int node) {
        if (inStack[node]) return false; // Cycle detected
        if (visited[node]) return true;
        
        visited[node] = true;
        inStack[node] = true;
        
        for (int neighbor : graph[node]) {
            if (!dfs(neighbor)) return false;
        }
        
        inStack[node] = false;
        result.push_back(node);
        return true;
    };
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (!dfs(i)) {
                // Graph has a cycle, topological sort not possible
                return {};
            }
        }
    }
    
    reverse(result.begin(), result.end());
    return result;
}`;
  
  const kahnCode = `// Kahn's Algorithm (BFS-based) for Topological Sort in C++
vector<int> topologicalSortKahn(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> result;
    vector<int> inDegree(n, 0);
    
    // Calculate in-degrees
    for (int i = 0; i < n; i++) {
        for (int neighbor : graph[i]) {
            inDegree[neighbor]++;
        }
    }
    
    // Queue nodes with 0 in-degree
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }
    
    // Process queue
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);
        
        for (int neighbor : graph[node]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    // Check if topological sort was successful
    if (result.size() != n) {
        // Graph has a cycle, topological sort not possible
        return {};
    }
    
    return result;
}`;

  // Time and space complexity info
  const complexityInfo = {
    dfs: {
      time: 'O(V + E)',
      space: 'O(V)',
      pros: ['Can detect cycles in the graph', 'Simple recursive implementation'],
      cons: ['Recursive calls may lead to stack overflow for very large graphs']
    },
    kahn: {
      time: 'O(V + E)',
      space: 'O(V)',
      pros: ['Iterative approach, no recursion needed', 'Can easily detect cycles'],
      cons: ['Requires tracking in-degrees of all vertices']
    }
  };

  // Real-world applications info
  const applications = [
    'Task scheduling and build systems',
    'Course prerequisites planning in academic systems',
    'Dependency resolution in package managers',
    'Determining order of operations in compilers',
    'Data processing pipelines and workflows',
    'Critical path analysis in project management'
  ];

  // DAG properties
  const dagProperties = [
    'No directed cycles allowed (no way to follow edges and return to starting point)',
    'At least one node with no incoming edges (source)',
    'At least one node with no outgoing edges (sink)',
    'Always allows for a valid topological ordering',
    'Used to represent dependencies between items'
  ];

  return (
    <div className="flex flex-col items-center w-full h-full bg-gray-900 text-white p-4">
      <h1 className="text-2xl font-bold mb-4 text-cyan-400">#DSAin45 Day 33: Topological Sort & DAGs</h1>
      
      <div className="flex justify-between w-full mb-4">
        <div className="w-1/2 pr-2">
          <select 
            value={algorithm} 
            onChange={handleAlgorithmChange} 
            className="w-full p-2 bg-gray-800 text-cyan-400 border border-cyan-600 rounded"
          >
            <option value="dfs">DFS-based Topological Sort</option>
            <option value="kahn">Kahn's Algorithm (BFS-based)</option>
          </select>
        </div>
        
        <div className="w-1/2 pl-2 flex items-center">
          <span className="mr-2 text-yellow-300">Speed:</span>
          <input 
            type="range" 
            min="200" 
            max="2000" 
            step="100" 
            value={animationSpeed} 
            onChange={handleSpeedChange} 
            className="w-full"
          />
        </div>
      </div>
      
      <div className="relative w-full h-80 mb-4">
        <canvas 
          ref={canvasRef} 
          width="500" 
          height="300" 
          className="w-full h-full bg-gray-800 rounded-lg"
        />
      </div>
      
      <div className="w-full p-4 bg-gray-800 rounded-lg mb-4">
        <h3 className="text-lg font-semibold text-green-400 mb-2">Current Status</h3>
        <p className="text-yellow-300">{steps[currentStep]?.description || 'Ready to start'}</p>
        <div className="mt-2">
          <span className="text-pink-400">Step: {currentStep + 1}/{steps.length}</span>
          {topologicalOrder.length > 0 && (
            <div className="mt-2">
              <span className="text-cyan-400">Current Topological Order:</span>
              <div className="flex items-center justify-center space-x-2 mt-1">
                {topologicalOrder.map((nodeId, index) => (
                  <div key={index} className="flex items-center">
                    <span className="bg-gray-700 px-3 py-1 rounded-md text-pink-400 font-bold">{nodeId}</span>
                    {index < topologicalOrder.length - 1 && (
                      <span className="text-white">→</span>
                    )}
                  </div>
                ))}
              </div>
            </div>
          )}
        </div>
      </div>
      
      <div className="flex justify-center space-x-4 mb-4">
        <button 
          onClick={handleReset} 
          className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
          title="Reset"
        >
          <RotateCw className="w-6 h-6 text-yellow-300" />
        </button>
        <button 
          onClick={handlePrev} 
          className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
          disabled={currentStep === 0}
          title="Previous Step"
        >
          <SkipBack className="w-6 h-6 text-cyan-400" />
        </button>
        {isRunning ? (
          <button 
            onClick={handlePause} 
            className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
            title="Pause"
          >
            <Pause className="w-6 h-6 text-pink-400" />
          </button>
        ) : (
          <button 
            onClick={handlePlay} 
            className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
            disabled={currentStep === steps.length - 1}
            title="Play"
          >
            <Play className="w-6 h-6 text-green-400" />
          </button>
        )}
        <button 
          onClick={handleNext} 
          className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
          disabled={currentStep === steps.length - 1}
          title="Next Step"
        >
          <SkipForward className="w-6 h-6 text-cyan-400" />
        </button>
        <button 
          onClick={toggleCodeView} 
          className="p-2 bg-gray-700 rounded-full hover:bg-gray-600"
          title="View Code"
        >
          <Code className="w-6 h-6 text-yellow-300" />
        </button>
      </div>
      
      {showCode ? (
        <div className="w-full p-4 bg-gray-800 rounded-lg mb-4 overflow-auto max-h-96">
          <h3 className="text-lg font-semibold text-pink-400 mb-2">C++ Implementation</h3>
          <pre className="text-xs text-green-300 whitespace-pre-wrap">
            {algorithm === 'dfs' ? dfsCode : kahnCode}
          </pre>
        </div>
      ) : (
        <div className="w-full grid grid-cols-1 gap-4 mb-4">
          <div className="p-4 bg-gray-800 rounded-lg">
            <h3 className="text-lg font-semibold text-pink-400 mb-2">Algorithm Analysis</h3>
            <div className="grid grid-cols-2 gap-4">
              <div>
                <p className="text-cyan-400 font-semibold">Time Complexity:</p>
                <p className="text-white">{complexityInfo[algorithm].time}</p>
              </div>
              <div>
                <p className="text-cyan-400 font-semibold">Space Complexity:</p>
                <p className="text-white">{complexityInfo[algorithm].space}</p>
              </div>
              <div>
                <p className="text-green-400 font-semibold">Pros:</p>
                <ul className="list-disc list-inside">
                  {complexityInfo[algorithm].pros.map((pro, index) => (
                    <li key={index} className="text-white">{pro}</li>
                  ))}
                </ul>
              </div>
              <div>
                <p className="text-pink-400 font-semibold">Cons:</p>
                <ul className="list-disc list-inside">
                  {complexityInfo[algorithm].cons.map((con, index) => (
                    <li key={index} className="text-white">{con}</li>
                  ))}
                </ul>
              </div>
            </div>
          </div>
          
          <div className="grid grid-cols-2 gap-4">
            <div className="p-4 bg-gray-800 rounded-lg">
              <h3 className="text-lg font-semibold text-yellow-300 mb-2">DAG Properties</h3>
              <ul className="list-disc list-inside">
                {dagProperties.map((property, index) => (
                  <li key={index} className="text-white mb-1">{property}</li>
                ))}
              </ul>
            </div>
            
            <div className="p-4 bg-gray-800 rounded-lg">
              <h3 className="text-lg font-semibold text-yellow-300 mb-2">Applications</h3>
              <ul className="list-disc list-inside">
                {applications.map((app, index) => (
                  <li key={index} className="text-white mb-1">{app}</li>
                ))}
              </ul>
            </div>
          </div>
        </div>
      )}
      
      <div className="w-full text-center text-sm text-gray-400">
        <p>Color guide: <span className="text-cyan-400">Unvisited Nodes</span> | <span className="text-yellow-300">Processing Nodes</span> | <span className="text-pink-400">Completed Nodes</span> | <span className="text-green-400">Used Edges</span></p>
      </div>
    </div>
  );
}