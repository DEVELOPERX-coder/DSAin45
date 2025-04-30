import React, { useState } from 'react';
import { ChevronDown, ChevronUp, Info, Play, Pause, RotateCcw } from 'lucide-react';

// Custom styled components
const Container = ({ children }) => (
  <div className="flex flex-col items-center w-full h-full p-4 bg-gray-900 text-white font-sans">
    {children}
  </div>
);

const Title = ({ children }) => (
  <h1 className="text-3xl font-bold mb-2 text-cyan-400">
    {children}
  </h1>
);

const Subtitle = ({ children }) => (
  <h2 className="text-lg mb-4 text-gray-300">
    {children}
  </h2>
);

const Button = ({ onClick, children, active, className }) => (
  <button 
    onClick={onClick} 
    className={`px-4 py-2 rounded-lg font-medium transition-colors ${
      active 
        ? 'bg-fuchsia-600 text-white' 
        : 'bg-gray-700 text-gray-200 hover:bg-gray-600'
    } ${className || ''}`}
  >
    {children}
  </button>
);

const Card = ({ title, children, isOpen, onToggle }) => (
  <div className="w-full border border-gray-700 rounded-lg mb-4 overflow-hidden">
    <div 
      className="flex justify-between items-center p-3 bg-gray-800 cursor-pointer"
      onClick={onToggle}
    >
      <h3 className="font-semibold text-lg">{title}</h3>
      {isOpen ? <ChevronUp className="h-5 w-5" /> : <ChevronDown className="h-5 w-5" />}
    </div>
    {isOpen && (
      <div className="p-4 bg-gray-800 bg-opacity-50">
        {children}
      </div>
    )}
  </div>
);

const ComplexityTable = ({ data }) => (
  <div className="overflow-x-auto w-full mb-4">
    <table className="w-full border-collapse">
      <thead>
        <tr className="bg-gray-800">
          <th className="border border-gray-600 p-2">Algorithm</th>
          <th className="border border-gray-600 p-2">Time Complexity</th>
          <th className="border border-gray-600 p-2">Space Complexity</th>
          <th className="border border-gray-600 p-2">Negative Edges</th>
          <th className="border border-gray-600 p-2">Negative Cycles</th>
          <th className="border border-gray-600 p-2">Use Case</th>
        </tr>
      </thead>
      <tbody>
        {data.map((row, i) => (
          <tr key={i} className={i % 2 === 0 ? 'bg-gray-800' : 'bg-gray-700'}>
            <td className="border border-gray-600 p-2 font-medium text-cyan-400">{row.algorithm}</td>
            <td className="border border-gray-600 p-2">{row.time}</td>
            <td className="border border-gray-600 p-2">{row.space}</td>
            <td className="border border-gray-600 p-2 text-center">
              {row.negativeEdges ? '✅' : '❌'}
            </td>
            <td className="border border-gray-600 p-2 text-center">
              {row.negativeCycles ? '✅' : '❌'}
            </td>
            <td className="border border-gray-600 p-2">{row.useCase}</td>
          </tr>
        ))}
      </tbody>
    </table>
  </div>
);

// Graph Visualization component
const GraphVisualization = () => {
  const [algorithm, setAlgorithm] = useState('dijkstra');
  const [playing, setPlaying] = useState(false);
  const [step, setStep] = useState(0);
  const [showInfo, setShowInfo] = useState(false);

  // Nodes data
  const nodes = [
    { id: 'A', x: 80, y: 70 },
    { id: 'B', x: 220, y: 70 },
    { id: 'C', x: 360, y: 70 },
    { id: 'D', x: 80, y: 200 },
    { id: 'E', x: 220, y: 200 },
    { id: 'F', x: 360, y: 200 },
  ];

  // Edges data with weights
  const edges = [
    { from: 'A', to: 'B', weight: 2 },
    { from: 'A', to: 'D', weight: 4 },
    { from: 'B', to: 'C', weight: 3 },
    { from: 'B', to: 'E', weight: 1 },
    { from: 'C', to: 'F', weight: 5 },
    { from: 'D', to: 'E', weight: 7 },
    { from: 'E', to: 'F', weight: 1 },
  ];

  // Algorithm visualization steps
  const visualizationSteps = {
    dijkstra: [
      { description: "Initialize: A=0, B=∞, C=∞, D=∞, E=∞, F=∞", visited: ['A'], distances: { A: 0, B: '∞', C: '∞', D: '∞', E: '∞', F: '∞' }, active: [] },
      { description: "Process A: Set B=2, D=4", visited: ['A'], distances: { A: 0, B: 2, C: '∞', D: 4, E: '∞', F: '∞' }, active: ['A-B', 'A-D'] },
      { description: "Process B (smallest distance): Set C=5, E=3", visited: ['A', 'B'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: '∞' }, active: ['B-C', 'B-E'] },
      { description: "Process E (smallest distance): Set F=4", visited: ['A', 'B', 'E'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['E-F'] },
      { description: "Process D (smallest distance): No updates", visited: ['A', 'B', 'E', 'D'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Process F (smallest distance): No updates", visited: ['A', 'B', 'E', 'D', 'F'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Process C (smallest distance): No updates", visited: ['A', 'B', 'E', 'D', 'F', 'C'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Final result: Shortest path to F is A→B→E→F with weight 4", visited: ['A', 'B', 'E', 'D', 'F', 'C'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['A-B', 'B-E', 'E-F'], showPath: true }
    ],
    bellmanFord: [
      { description: "Initialize: A=0, B=∞, C=∞, D=∞, E=∞, F=∞", visited: [], distances: { A: 0, B: '∞', C: '∞', D: '∞', E: '∞', F: '∞' }, active: [] },
      { description: "Iteration 1: Process all edges", visited: [], distances: { A: 0, B: 2, C: '∞', D: 4, E: '∞', F: '∞' }, active: ['A-B', 'A-D'] },
      { description: "Iteration 1 (cont'd): More edges", visited: [], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: '∞' }, active: ['B-C', 'B-E'] },
      { description: "Iteration 1 (cont'd): Final edges", visited: [], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['E-F'] },
      { description: "Iteration 2: No changes made", visited: [], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Check for negative cycles (none found)", visited: [], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Final result: Shortest path to F is A→B→E→F with weight 4", visited: [], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['A-B', 'B-E', 'E-F'], showPath: true }
    ],
    floydWarshall: [
      { description: "Initialize the distance matrix with direct edges", visited: [], distances: { A: { A: 0, B: 2, C: '∞', D: 4, E: '∞', F: '∞' }, B: { A: 2, B: 0, C: 3, D: '∞', E: 1, F: '∞' }, C: { A: '∞', B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: '∞', C: '∞', D: 0, E: 7, F: '∞' }, E: { A: '∞', B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: '∞', B: '∞', C: 5, D: '∞', E: 1, F: 0 } }, active: [] },
      { description: "Use A as intermediate: No improvements", visited: [], distances: { A: { A: 0, B: 2, C: '∞', D: 4, E: '∞', F: '∞' }, B: { A: 2, B: 0, C: 3, D: '∞', E: 1, F: '∞' }, C: { A: '∞', B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: '∞', C: '∞', D: 0, E: 7, F: '∞' }, E: { A: '∞', B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: '∞', B: '∞', C: 5, D: '∞', E: 1, F: 0 } }, active: [] },
      { description: "Use B as intermediate: Path A→C improves to A→B→C=5", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: '∞' }, B: { A: 2, B: 0, C: 3, D: '∞', E: 1, F: '∞' }, C: { A: 5, B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: '∞', C: '∞', D: 0, E: 7, F: '∞' }, E: { A: 3, B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: '∞', B: '∞', C: 5, D: '∞', E: 1, F: 0 } }, active: ['A-B', 'B-C'] },
      { description: "Use C as intermediate: Path B→F improves to B→C→F=8", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 10 }, B: { A: 2, B: 0, C: 3, D: '∞', E: 1, F: 8 }, C: { A: 5, B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: '∞', C: '∞', D: 0, E: 7, F: '∞' }, E: { A: 3, B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: 10, B: 8, C: 5, D: '∞', E: 1, F: 0 } }, active: ['B-C', 'C-F'] },
      { description: "Use D as intermediate: No significant improvements", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 10 }, B: { A: 2, B: 0, C: 3, D: '∞', E: 1, F: 8 }, C: { A: 5, B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: '∞', C: '∞', D: 0, E: 7, F: '∞' }, E: { A: 3, B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: 10, B: 8, C: 5, D: '∞', E: 1, F: 0 } }, active: [] },
      { description: "Use E as intermediate: Path A→F improves to A→B→E→F=4", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, B: { A: 2, B: 0, C: 3, D: 8, E: 1, F: 2 }, C: { A: 5, B: 3, C: 0, D: '∞', E: '∞', F: 5 }, D: { A: 4, B: 8, C: '∞', D: 0, E: 7, F: 8 }, E: { A: 3, B: 1, C: '∞', D: 7, E: 0, F: 1 }, F: { A: 4, B: 2, C: 5, D: 8, E: 1, F: 0 } }, active: ['A-B', 'B-E', 'E-F'] },
      { description: "Use F as intermediate: Complete matrix calculated", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, B: { A: 2, B: 0, C: 3, D: 8, E: 1, F: 2 }, C: { A: 5, B: 3, C: 0, D: 10, E: 6, F: 5 }, D: { A: 4, B: 8, C: 10, D: 0, E: 7, F: 8 }, E: { A: 3, B: 1, C: 6, D: 7, E: 0, F: 1 }, F: { A: 4, B: 2, C: 5, D: 8, E: 1, F: 0 } }, active: [] },
      { description: "Final result: Shortest path to F is A→B→E→F with weight 4", visited: [], distances: { A: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, B: { A: 2, B: 0, C: 3, D: 8, E: 1, F: 2 }, C: { A: 5, B: 3, C: 0, D: 10, E: 6, F: 5 }, D: { A: 4, B: 8, C: 10, D: 0, E: 7, F: 8 }, E: { A: 3, B: 1, C: 6, D: 7, E: 0, F: 1 }, F: { A: 4, B: 2, C: 5, D: 8, E: 1, F: 0 } }, active: ['A-B', 'B-E', 'E-F'], showPath: true }
    ],
    astar: [
      { description: "Initialize: A=0+h(A,F), open set contains A", visited: [], distances: { A: 0, B: '∞', C: '∞', D: '∞', E: '∞', F: '∞' }, active: [] },
      { description: "Process A: Add B, D to open set with f-values", visited: ['A'], distances: { A: 0, B: 2, C: '∞', D: 4, E: '∞', F: '∞' }, active: ['A-B', 'A-D'] },
      { description: "Process B (smallest f-value): Add C, E to open set", visited: ['A', 'B'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: '∞' }, active: ['B-C', 'B-E'] },
      { description: "Process E (smallest f-value): Add F to open set", visited: ['A', 'B', 'E'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['E-F'] },
      { description: "Process F: Goal reached! Algorithm terminates", visited: ['A', 'B', 'E', 'F'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: [] },
      { description: "Final result: Shortest path to F is A→B→E→F with weight 4", visited: ['A', 'B', 'E', 'F'], distances: { A: 0, B: 2, C: 5, D: 4, E: 3, F: 4 }, active: ['A-B', 'B-E', 'E-F'], showPath: true }
    ]
  };

  const currentSteps = visualizationSteps[algorithm];
  const currentStep = currentSteps[step] || currentSteps[0];

  const playAnimation = () => {
    if (playing) {
      setPlaying(false);
      return;
    }
    
    setPlaying(true);
    let currentStep = step;
    
    const interval = setInterval(() => {
      if (currentStep < currentSteps.length - 1) {
        currentStep++;
        setStep(currentStep);
      } else {
        clearInterval(interval);
        setPlaying(false);
      }
    }, 2000);
  };

  const reset = () => {
    setStep(0);
    setPlaying(false);
  };

  // Helper function to determine node color based on state
  const getNodeColor = (nodeId) => {
    if (currentStep.showPath && (nodeId === 'A' || nodeId === 'F')) {
      return '#FFFF00'; // Yellow for start/end in path
    }
    if (currentStep.visited && currentStep.visited.includes(nodeId)) {
      return '#00FF00'; // Green for visited
    }
    return '#00FFFF'; // Default cyan
  };

  // Helper function to determine edge color based on state
  const getEdgeColor = (from, to) => {
    const edgeId = `${from}-${to}`;
    const reverseEdgeId = `${to}-${from}`;
    
    if (currentStep.showPath && currentStep.active.includes(edgeId)) {
      return '#FFFF00'; // Yellow for final path
    }
    if (currentStep.active.includes(edgeId) || currentStep.active.includes(reverseEdgeId)) {
      return '#FF00FF'; // Magenta for active
    }
    return '#444444'; // Gray for inactive
  };

  const algorithmInfo = {
    dijkstra: "Dijkstra's algorithm is a greedy algorithm that finds the shortest path from a source node to all other nodes. It works by repeatedly selecting the node with the smallest known distance and updating the distances to its neighbors. It doesn't work with negative edge weights.",
    bellmanFord: "The Bellman-Ford algorithm finds the shortest paths from a source node to all other nodes, even in graphs with negative edge weights. It works by relaxing all edges V-1 times (where V is the number of vertices) and can detect negative cycles.",
    floydWarshall: "Floyd-Warshall is a dynamic programming algorithm that finds the shortest paths between all pairs of nodes. It works by considering each node as a potential intermediate point in a path and updates the distance matrix accordingly.",
    astar: "A* (A-star) is an informed search algorithm that uses a heuristic to guide its search toward the goal. It combines the advantages of Dijkstra's algorithm (completeness, optimality) with the efficiency of greedy best-first search."
  };

  return (
    <div className="w-full max-w-3xl">
      <div className="flex gap-2 mb-4 flex-wrap">
        <Button 
          onClick={() => { setAlgorithm('dijkstra'); setStep(0); setPlaying(false); }} 
          active={algorithm === 'dijkstra'}
        >
          Dijkstra's
        </Button>
        <Button 
          onClick={() => { setAlgorithm('bellmanFord'); setStep(0); setPlaying(false); }} 
          active={algorithm === 'bellmanFord'}
        >
          Bellman-Ford
        </Button>
        <Button 
          onClick={() => { setAlgorithm('floydWarshall'); setStep(0); setPlaying(false); }} 
          active={algorithm === 'floydWarshall'}
        >
          Floyd-Warshall
        </Button>
        <Button 
          onClick={() => { setAlgorithm('astar'); setStep(0); setPlaying(false); }} 
          active={algorithm === 'astar'}
        >
          A* Search
        </Button>
      </div>
      
      <div className="flex items-center gap-2 mb-2">
        <Button onClick={playAnimation} className="flex items-center gap-1">
          {playing ? <Pause className="h-4 w-4" /> : <Play className="h-4 w-4" />} 
          {playing ? "Pause" : "Animate"}
        </Button>
        <Button onClick={reset} className="flex items-center gap-1">
          <RotateCcw className="h-4 w-4" /> Reset
        </Button>
        <Button 
          onClick={() => setShowInfo(!showInfo)} 
          className="flex items-center gap-1 ml-auto"
        >
          <Info className="h-4 w-4" /> Info
        </Button>
      </div>
      
      {showInfo && (
        <div className="mb-4 p-4 bg-gray-800 rounded-lg text-sm">
          {algorithmInfo[algorithm]}
        </div>
      )}
      
      <div className="flex mb-4 bg-gray-800 rounded-lg overflow-hidden">
        <div className="relative w-full" style={{ height: '300px' }}>
          <svg width="100%" height="100%" viewBox="0 0 440 270">
            {/* Edges */}
            {edges.map((edge, i) => {
              const fromNode = nodes.find(n => n.id === edge.from);
              const toNode = nodes.find(n => n.id === edge.to);
              
              return (
                <g key={`edge-${i}`}>
                  <line 
                    x1={fromNode.x} 
                    y1={fromNode.y} 
                    x2={toNode.x} 
                    y2={toNode.y} 
                    stroke={getEdgeColor(edge.from, edge.to)} 
                    strokeWidth="3"
                  />
                  
                  {/* Edge weight */}
                  <text 
                    x={(fromNode.x + toNode.x) / 2} 
                    y={(fromNode.y + toNode.y) / 2 - 10} 
                    textAnchor="middle" 
                    fill="white" 
                    fontSize="14"
                  >
                    {edge.weight}
                  </text>
                </g>
              );
            })}
            
            {/* Nodes */}
            {nodes.map((node) => (
              <g key={`node-${node.id}`}>
                <circle 
                  cx={node.x} 
                  cy={node.y} 
                  r="20" 
                  fill={getNodeColor(node.id)} 
                />
                <text 
                  x={node.x} 
                  y={node.y} 
                  textAnchor="middle" 
                  dominantBaseline="middle" 
                  fill="black" 
                  fontSize="16" 
                  fontWeight="bold"
                >
                  {node.id}
                </text>
                
                {/* Distance label */}
                {currentStep.distances && (
                  <text 
                    x={node.x} 
                    y={node.y + 35} 
                    textAnchor="middle" 
                    fill="white" 
                    fontSize="14"
                  >
                    {typeof currentStep.distances[node.id] !== 'undefined' 
                      ? currentStep.distances[node.id] 
                      : '∞'}
                  </text>
                )}
              </g>
            ))}
          </svg>
        </div>
      </div>
      
      <div className="p-4 bg-gray-800 rounded-lg mb-4">
        <p className="font-medium">Step {step + 1}/{currentSteps.length}: {currentStep.description}</p>
      </div>
      
      <div className="flex">
        <div className="flex gap-4 items-center mb-2">
          <div className="flex items-center gap-1">
            <div className="w-4 h-4 rounded-full bg-cyan-400"></div>
            <span className="text-sm">Unvisited</span>
          </div>
          <div className="flex items-center gap-1">
            <div className="w-4 h-4 rounded-full bg-green-400"></div>
            <span className="text-sm">Visited</span>
          </div>
          <div className="flex items-center gap-1">
            <div className="w-4 h-4 rounded-full bg-yellow-400"></div>
            <span className="text-sm">Path</span>
          </div>
        </div>
      </div>
    </div>
  );
};

// Main component with tabs
const ShortestPathVisualization = () => {
  const [activeTab, setActiveTab] = useState('visualization');
  const [openCards, setOpenCards] = useState({
    dijkstra: true,
    bellmanFord: false,
    floydWarshall: false,
    johnsonAlgorithm: false
  });

  const toggleCard = (card) => {
    setOpenCards(prev => ({
      ...prev,
      [card]: !prev[card]
    }));
  };

  const complexityData = [
    { 
      algorithm: "Dijkstra's", 
      time: "O(E log V)", 
      space: "O(V)", 
      negativeEdges: false, 
      negativeCycles: false,
      useCase: "Single-source shortest path, GPS navigation"
    },
    { 
      algorithm: "Bellman-Ford", 
      time: "O(V·E)", 
      space: "O(V)", 
      negativeEdges: true, 
      negativeCycles: true,
      useCase: "Negative edge weights, financial transactions"
    },
    { 
      algorithm: "Floyd-Warshall", 
      time: "O(V³)", 
      space: "O(V²)", 
      negativeEdges: true, 
      negativeCycles: false,
      useCase: "All-pairs shortest paths, small dense graphs"
    },
    { 
      algorithm: "Johnson's", 
      time: "O(V² log V + V·E)", 
      space: "O(V²)", 
      negativeEdges: true, 
      negativeCycles: false,
      useCase: "All-pairs shortest paths, sparse graphs"
    },
    { 
      algorithm: "A* Search", 
      time: "O(E)", 
      space: "O(V)", 
      negativeEdges: false, 
      negativeCycles: false,
      useCase: "Informed search, path finding, games"
    }
  ];

  const MemeSVG = () => (
    <svg viewBox="0 0 600 400" className="w-full h-full max-w-md mx-auto">
      <rect width="600" height="400" fill="#121212" />
      
      {/* Top Panel */}
      <rect x="10" y="10" width="580" height="190" fill="#333" rx="5" ry="5" />
      <text x="300" y="40" fontSize="20" fill="#00FFFF" textAnchor="middle" fontWeight="bold">
        SHORTEST PATH ALGORITHMS
      </text>
      <text x="300" y="70" fontSize="16" fill="white" textAnchor="middle">
        EXPECTATION
      </text>
      
      <line x1="170" y1="100" x2="430" y2="100" stroke="#00FF00" strokeWidth="3" />
      <circle cx="170" cy="100" r="15" fill="#00FFFF" />
      <circle cx="430" cy="100" r="15" fill="#00FFFF" />
      <text x="170" cy="100" textAnchor="middle" dominantBaseline="middle" fill="black" fontWeight="bold">A</text>
      <text x="430" cy="100" textAnchor="middle" dominantBaseline="middle" fill="black" fontWeight="bold">B</text>
      
      <text x="300" y="170" fontSize="14" fill="white" textAnchor="middle">
        One straight line from A to B
      </text>
      
      {/* Bottom Panel */}
      <rect x="10" y="210" width="580" height="180" fill="#333" rx="5" ry="5" />
      <text x="300" y="240" fontSize="16" fill="white" textAnchor="middle">
        REALITY
      </text>
      
      {/* Crazy path */}
      <path d="M 100,280 C 130,250 160,320 190,270 S 250,350 280,290 S 340,350 370,270 S 430,340 460,280" 
        stroke="#FF00FF" strokeWidth="3" fill="none" />
      
      <circle cx="100" cy="280" r="15" fill="#00FFFF" />
      <circle cx="460" cy="280" r="15" fill="#00FFFF" />
      <text x="100" cy="280" textAnchor="middle" dominantBaseline="middle" fill="black" fontWeight="bold">A</text>
      <text x="460" cy="280" textAnchor="middle" dominantBaseline="middle" fill="black" fontWeight="bold">B</text>
      
      <text x="300" y="360" fontSize="14" fill="white" textAnchor="middle">
        Negative edge weights, cycles, and one-way streets
      </text>
    </svg>
  );

  const ApplicationsSVG = () => (
    <svg viewBox="0 0 600 400" className="w-full h-full max-w-md mx-auto">
      <rect width="600" height="400" fill="#121212" />
      
      {/* Grid lines */}
      {[0, 1, 2, 3, 4].map(i => (
        <line 
          key={`vline-${i}`}
          x1={100 + i * 100} 
          y1="50" 
          x2={100 + i * 100} 
          y2="350" 
          stroke="#333" 
          strokeWidth="1" 
        />
      ))}
      
      {[0, 1, 2, 3].map(i => (
        <line 
          key={`hline-${i}`}
          x1="100" 
          y1={50 + i * 100} 
          x2="500" 
          y2={50 + i * 100} 
          stroke="#333" 
          strokeWidth="1" 
        />
      ))}
      
      <text x="300" y="30" fontSize="20" fill="#00FFFF" textAnchor="middle" fontWeight="bold">
        REAL-WORLD APPLICATIONS
      </text>
      
      {/* Application Boxes */}
      <g transform="translate(150, 100)">
        <rect width="100" height="60" fill="#FF00FF" rx="5" ry="5" />
        <text x="50" y="35" fontSize="14" fill="white" textAnchor="middle">
          GPS Navigation
        </text>
      </g>
      
      <g transform="translate(350, 100)">
        <rect width="100" height="60" fill="#00FFFF" rx="5" ry="5" />
        <text x="50" y="35" fontSize="14" fill="black" textAnchor="middle">
          Network Routing
        </text>
      </g>
      
      <g transform="translate(150, 250)">
        <rect width="100" height="60" fill="#00FF00" rx="5" ry="5" />
        <text x="50" y="35" fontSize="14" fill="black" textAnchor="middle">
          Game Pathfinding
        </text>
      </g>
      
      <g transform="translate(350, 250)">
        <rect width="100" height="60" fill="#FFFF00" rx="5" ry="5" />
        <text x="50" y="35" fontSize="14" fill="black" textAnchor="middle">
          Circuit Design
        </text>
      </g>
      
      {/* Connecting Lines */}
      <line x1="200" y1="160" x2="350" y2="250" stroke="#FF00FF" strokeWidth="2" />
      <line x1="400" y1="160" x2="250" y2="250" stroke="#00FFFF" strokeWidth="2" />
      <line x1="250" y1="100" x2="350" y2="100" stroke="#FFFF00" strokeWidth="2" />
      <line x1="200" y1="250" x2="350" y2="250" stroke="#00FF00" strokeWidth="2" />
    </svg>
  );

  return (
    <Container>
      <Title>Shortest Path Algorithms</Title>
      <Subtitle>Finding the Most Efficient Routes in Graphs</Subtitle>
      
      <div className="flex gap-2 mb-4">
        <Button 
          onClick={() => setActiveTab('visualization')} 
          active={activeTab === 'visualization'}
        >
          Interactive Demo
        </Button>
        <Button 
          onClick={() => setActiveTab('complexity')} 
          active={activeTab === 'complexity'}
        >
          Complexity Analysis
        </Button>
        <Button 
          onClick={() => setActiveTab('applications')} 
          active={activeTab === 'applications'}
        >
          Applications
        </Button>
        <Button 
          onClick={() => setActiveTab('meme')} 
          active={activeTab === 'meme'}
        >
          Meme
        </Button>
      </div>
      
      {activeTab === 'visualization' && (
        <GraphVisualization />
      )}
      
      {activeTab === 'complexity' && (
        <div className="w-full">
          <Card 
            title="Time & Space Complexity Comparison" 
            isOpen={true} 
            onToggle={() => {}}
          >
            <ComplexityTable data={complexityData} />
            <p className="text-gray-300 text-sm">
              V = number of vertices, E = number of edges
            </p>
          </Card>
          
          <Card 
            title="Dijkstra's Algorithm" 
            isOpen={openCards.dijkstra} 
            onToggle={() => toggleCard('dijkstra')}
          >
            <p className="mb-2">A greedy algorithm that finds the shortest path from a source vertex to all other vertices in a graph with non-negative edge weights.</p>
            <ul className="list-disc pl-5 space-y-1">
              <li>Works by maintaining a priority queue of vertices, ordered by their current distance from the source</li>
              <li>Repeatedly selects the vertex with minimum distance and explores its neighbors</li>
              <li><span className="text-cyan-400">Advantage:</span> Efficient for sparse graphs</li>
              <li><span className="text-pink-400">Limitation:</span> Cannot handle negative edge weights</li>
            </ul>
          </Card>
          
          <Card 
            title="Bellman-Ford Algorithm" 
            isOpen={openCards.bellmanFord} 
            onToggle={() => toggleCard('bellmanFord')}
          >
            <p className="mb-2">Finds the shortest paths from a source vertex to all other vertices, even in graphs with negative edge weights.</p>
            <ul className="list-disc pl-5 space-y-1">
              <li>Works by relaxing all edges V-1 times (where V is the number of vertices)</li>
              <li>Can detect negative cycles (cycles with negative total weight)</li>
              <li><span className="text-cyan-400">Advantage:</span> Works with negative edge weights</li>
              <li><span className="text-pink-400">Limitation:</span> Slower than Dijkstra's algorithm</li>
            </ul>
          </Card>
          
          <Card 
            title="Floyd-Warshall Algorithm" 
            isOpen={openCards.floydWarshall} 
            onToggle={() => toggleCard('floydWarshall')}
          >
            <p className="mb-2">A dynamic programming algorithm that finds the shortest paths between all pairs of vertices.</p>
            <ul className="list-disc pl-5 space-y-1">
              <li>Works by considering each vertex as a potential intermediate point</li>
              <li>Builds up solutions for all pairs incrementally</li>
              <li><span className="text-cyan-400">Advantage:</span> Simple implementation, handles negative edges</li>
              <li><span className="text-pink-400">Limitation:</span> O(V³) time complexity makes it impractical for large graphs</li>
            </ul>
          </Card>
          
          <Card 
            title="Johnson's Algorithm" 
            isOpen={openCards.johnsonAlgorithm} 
            onToggle={() => toggleCard('johnsonAlgorithm')}
          >
            <p className="mb-2">Finds all-pairs shortest paths, particularly efficient for sparse graphs.</p>
            <ul className="list-disc pl-5 space-y-1">
              <li>Uses Bellman-Ford to reweight the graph to remove negative edges</li>
              <li>Then runs Dijkstra's algorithm from each vertex</li>
              <li><span className="text-cyan-400">Advantage:</span> More efficient than Floyd-Warshall for sparse graphs</li>
              <li><span className="text-pink-400">Limitation:</span> More complex implementation</li>
            </ul>
          </Card>
        </div>
      )}
      
      {activeTab === 'applications' && (
        <div className="w-full">
          <div className="mb-6">
            <ApplicationsSVG />
          </div>
          
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div className="bg-gray-800 p-4 rounded-lg">
              <h3 className="text-lg font-semibold text-cyan-400 mb-2">GPS & Navigation Systems</h3>
              <p>Shortest path algorithms power the route planning in GPS devices and navigation apps, finding optimal routes based on distance, time, or other factors.</p>
            </div>
            
            <div className="bg-gray-800 p-4 rounded-lg">
              <h3 className="text-lg font-semibold text-pink-400 mb-2">Network Routing</h3>
              <p>Internet packets are routed through networks using shortest path algorithms to minimize delay and maximize throughput.</p>
            </div>
            
            <div className="bg-gray-800 p-4 rounded-lg">
              <h3 className="text-lg font-semibold text-yellow-400 mb-2">Social Network Analysis</h3>
              <p>Finding the shortest path between users to study the "six degrees of separation" phenomenon and recommend connections.</p>
            </div>
            
            <div className="bg-gray-800 p-4 rounded-lg">
              <h3 className="text-lg font-semibold text-green-400 mb-2">Robotics & Autonomous Vehicles</h3>
              <p>Path planning algorithms help robots and self-driving cars navigate through environments efficiently while avoiding obstacles.</p>
            </div>
          </div>
        </div>
      )}
      
      {activeTab === 'meme' && (
        <div className="w-full">
          <MemeSVG />
        </div>
      )}
    </Container>
  );
};

export default ShortestPathVisualization;