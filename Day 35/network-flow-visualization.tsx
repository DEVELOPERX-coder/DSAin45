import React, { useState, useEffect, useCallback } from 'react';

// Main component for the Network Flow Visualization
const NetworkFlowVisualization = () => {
  // State for the flow network
  const [graph, setGraph] = useState({
    nodes: [
      { id: 's', label: 'Source', x: 100, y: 300 },
      { id: 'a', label: 'A', x: 250, y: 150 },
      { id: 'b', label: 'B', x: 250, y: 450 },
      { id: 'c', label: 'C', x: 400, y: 225 },
      { id: 'd', label: 'D', x: 400, y: 375 },
      { id: 't', label: 'Sink', x: 550, y: 300 }
    ],
    edges: [
      { from: 's', to: 'a', capacity: 10, flow: 0 },
      { from: 's', to: 'b', capacity: 8, flow: 0 },
      { from: 'a', to: 'c', capacity: 6, flow: 0 },
      { from: 'a', to: 'b', capacity: 4, flow: 0 },
      { from: 'b', to: 'd', capacity: 7, flow: 0 },
      { from: 'c', to: 't', capacity: 8, flow: 0 },
      { from: 'c', to: 'd', capacity: 2, flow: 0 },
      { from: 'd', to: 't', capacity: 10, flow: 0 }
    ]
  });

  // State for algorithm visualization
  const [currentStep, setCurrentStep] = useState(0);
  const [maxSteps, setMaxSteps] = useState(0);
  const [playbackSpeed, setPlaybackSpeed] = useState(1000);
  const [isPlaying, setIsPlaying] = useState(false);
  const [currentPath, setCurrentPath] = useState([]);
  const [algorithmState, setAlgorithmState] = useState({
    description: 'Initial network with zero flow',
    maxFlow: 0,
    residualGraph: [],
  });

  // State for algorithm steps
  const [steps, setSteps] = useState([]);
  
  // State for UI
  const [showCode, setShowCode] = useState(false);
  const [showInfo, setShowInfo] = useState(true);
  
  // Colors
  const colors = {
    nodeBackground: '#121212',
    nodeBorder: '#00FFFF', // Cyan
    nodeText: '#FFFFFF',
    edgeBackground: '#FF00FF', // Pink
    activePath: '#FFFF00', // Yellow
    flowText: '#00FF00', // Green
    buttonBackground: '#121212',
    buttonText: '#00FFFF'
  };

  // Precompute algorithm steps
  useEffect(() => {
    // Clone the initial graph for the algorithm
    const initialGraph = JSON.parse(JSON.stringify(graph));
    
    // Reset all flows to 0
    initialGraph.edges.forEach(edge => edge.flow = 0);
    
    // Prepare the algorithm steps
    const algorithmSteps = [];
    
    // Add initial state
    algorithmSteps.push({
      graph: JSON.parse(JSON.stringify(initialGraph)),
      description: 'Initial network with zero flow',
      maxFlow: 0,
      path: [],
      residualGraph: computeResidualGraph(initialGraph)
    });
    
    // Run Ford-Fulkerson algorithm
    const result = fordFulkerson(initialGraph, 's', 't');
    
    setSteps(result.steps);
    setMaxSteps(result.steps.length - 1);
    
    // Set initial algorithm state
    setAlgorithmState(result.steps[0]);
    
  }, []);

  // Function to compute the residual graph
  function computeResidualGraph(graph) {
    const residualGraph = [];
    
    // Add forward and backward edges
    graph.edges.forEach(edge => {
      // Forward edge with remaining capacity
      const remainingCapacity = edge.capacity - edge.flow;
      if (remainingCapacity > 0) {
        residualGraph.push({
          from: edge.from,
          to: edge.to,
          capacity: remainingCapacity,
          isBackward: false
        });
      }
      
      // Backward edge with flow capacity
      if (edge.flow > 0) {
        residualGraph.push({
          from: edge.to,
          to: edge.from,
          capacity: edge.flow,
          isBackward: true
        });
      }
    });
    
    return residualGraph;
  }
  
  // Ford-Fulkerson algorithm implementation
  function fordFulkerson(graph, source, sink) {
    const steps = [];
    let currentGraph = JSON.parse(JSON.stringify(graph));
    let maxFlow = 0;
    
    // Add initial state
    steps.push({
      graph: JSON.parse(JSON.stringify(currentGraph)),
      description: 'Initial network with zero flow',
      maxFlow: 0,
      path: [],
      residualGraph: computeResidualGraph(currentGraph)
    });
    
    // Find augmenting paths until none remain
    let path;
    let iteration = 1;
    while ((path = findAugmentingPath(currentGraph, source, sink)) !== null) {
      // Find the minimum residual capacity along the path
      let minResidualCapacity = Infinity;
      for (let i = 0; i < path.length - 1; i++) {
        const edge = findEdge(currentGraph, path[i], path[i+1]);
        const residualCapacity = edge.capacity - edge.flow;
        minResidualCapacity = Math.min(minResidualCapacity, residualCapacity);
      }
      
      // Add step for found path
      steps.push({
        graph: JSON.parse(JSON.stringify(currentGraph)),
        description: `Augmenting Path ${iteration}: Found path with capacity ${minResidualCapacity}`,
        maxFlow,
        path,
        minCapacity: minResidualCapacity,
        residualGraph: computeResidualGraph(currentGraph)
      });
      
      // Augment flow along the path
      for (let i = 0; i < path.length - 1; i++) {
        const edge = findEdge(currentGraph, path[i], path[i+1]);
        edge.flow += minResidualCapacity;
        
        // If there's a reverse edge, update its flow as well
        const reverseEdge = findEdge(currentGraph, path[i+1], path[i]);
        if (reverseEdge) {
          reverseEdge.flow -= minResidualCapacity;
        }
      }
      
      maxFlow += minResidualCapacity;
      
      // Add step for updated flow
      steps.push({
        graph: JSON.parse(JSON.stringify(currentGraph)),
        description: `Augmenting Path ${iteration}: Augmented flow by ${minResidualCapacity}`,
        maxFlow,
        path: [],
        residualGraph: computeResidualGraph(currentGraph)
      });
      
      iteration++;
    }
    
    // Add final state
    steps.push({
      graph: JSON.parse(JSON.stringify(currentGraph)),
      description: `Algorithm complete: Maximum flow = ${maxFlow}`,
      maxFlow,
      path: [],
      residualGraph: computeResidualGraph(currentGraph)
    });
    
    return { maxFlow, steps };
  }
  
  // Find an edge in the graph
  function findEdge(graph, from, to) {
    return graph.edges.find(edge => edge.from === from && edge.to === to);
  }
  
  // Find an augmenting path using BFS
  function findAugmentingPath(graph, source, sink) {
    const queue = [source];
    const visited = { [source]: true };
    const parent = {};
    
    while (queue.length > 0) {
      const current = queue.shift();
      
      // If we reached the sink, reconstruct the path
      if (current === sink) {
        const path = [sink];
        let node = sink;
        while (node !== source) {
          node = parent[node];
          path.unshift(node);
        }
        return path;
      }
      
      // Find all potential next nodes
      for (const edge of graph.edges) {
        if (edge.from === current && edge.flow < edge.capacity && !visited[edge.to]) {
          visited[edge.to] = true;
          parent[edge.to] = current;
          queue.push(edge.to);
        }
      }
    }
    
    // No augmenting path found
    return null;
  }
  
  // Update visualization based on current step
  useEffect(() => {
    if (steps.length > 0 && currentStep >= 0 && currentStep < steps.length) {
      const step = steps[currentStep];
      setGraph(step.graph);
      setAlgorithmState({
        description: step.description,
        maxFlow: step.maxFlow,
        residualGraph: step.residualGraph
      });
      setCurrentPath(step.path || []);
    }
  }, [currentStep, steps]);
  
  // Handle playback
  useEffect(() => {
    let timer;
    if (isPlaying && currentStep < maxSteps) {
      timer = setTimeout(() => {
        setCurrentStep(prev => Math.min(prev + 1, maxSteps));
      }, playbackSpeed);
    } else if (currentStep >= maxSteps) {
      setIsPlaying(false);
    }
    
    return () => clearTimeout(timer);
  }, [isPlaying, currentStep, maxSteps, playbackSpeed]);
  
  // Handle playback controls
  const handlePlay = () => setIsPlaying(true);
  const handlePause = () => setIsPlaying(false);
  const handleStepForward = () => setCurrentStep(prev => Math.min(prev + 1, maxSteps));
  const handleStepBack = () => setCurrentStep(prev => Math.max(prev - 1, 0));
  const handleReset = () => {
    setCurrentStep(0);
    setIsPlaying(false);
  };
  
  // Render the graph
  const renderGraph = () => {
    return (
      <svg width="700" height="500" viewBox="0 0 700 500" style={{ backgroundColor: '#121212' }}>
        {/* Render edges */}
        {graph.edges.map((edge, index) => {
          // Find the node positions
          const fromNode = graph.nodes.find(node => node.id === edge.from);
          const toNode = graph.nodes.find(node => node.id === edge.to);
          
          // Check if this edge is part of the current path
          const isInPath = currentPath.length > 1 && 
                          currentPath.includes(edge.from) && 
                          currentPath.includes(edge.to) &&
                          currentPath.indexOf(edge.from) === currentPath.indexOf(edge.to) - 1;
                          
          // Calculate midpoint for the label
          const midX = (fromNode.x + toNode.x) / 2;
          const midY = (fromNode.y + toNode.y) / 2;
          
          // Adjust for label offset
          const dx = toNode.x - fromNode.x;
          const dy = toNode.y - fromNode.y;
          const len = Math.sqrt(dx * dx + dy * dy);
          const offsetX = -dy * 15 / len;
          const offsetY = dx * 15 / len;
          
          return (
            <g key={`edge-${index}`}>
              {/* Edge line */}
              <line 
                x1={fromNode.x} 
                y1={fromNode.y}
                x2={toNode.x}
                y2={toNode.y}
                stroke={isInPath ? colors.activePath : colors.edgeBackground}
                strokeWidth={isInPath ? 3 : 2}
              />
              
              {/* Edge label */}
              <text 
                x={midX + offsetX} 
                y={midY + offsetY}
                fill={colors.flowText}
                textAnchor="middle"
                dominantBaseline="middle"
                fontSize="14"
              >
                {edge.flow}/{edge.capacity}
              </text>
              
              {/* Edge direction marker (arrow) */}
              <marker
                id={`arrow-${index}`}
                viewBox="0 0 10 10"
                refX="5"
                refY="5"
                markerWidth="6"
                markerHeight="6"
                orient="auto-start-reverse"
              >
                <path d="M 0 0 L 10 5 L 0 10 z" fill={isInPath ? colors.activePath : colors.edgeBackground} />
              </marker>
            </g>
          );
        })}
        
        {/* Render nodes */}
        {graph.nodes.map((node, index) => (
          <g key={`node-${index}`}>
            <circle
              cx={node.x}
              cy={node.y}
              r={30}
              fill={colors.nodeBackground}
              stroke={currentPath.includes(node.id) ? colors.activePath : colors.nodeBorder}
              strokeWidth={currentPath.includes(node.id) ? 3 : 2}
            />
            <text
              x={node.x}
              y={node.y}
              textAnchor="middle"
              dominantBaseline="middle"
              fill={colors.nodeText}
              fontSize="16"
              fontWeight={currentPath.includes(node.id) ? "bold" : "normal"}
            >
              {node.label}
            </text>
          </g>
        ))}
      </svg>
    );
  };
  
  // Ford-Fulkerson pseudocode
  const codeSnippet = `function fordFulkerson(graph, source, sink):
    // Initialize flow to 0 for all edges
    for each edge (u,v) in graph:
        flow[u,v] = 0
    
    maxFlow = 0
    
    // While there exists an augmenting path
    while (path = findAugmentingPath(graph, source, sink)):
        // Find minimum residual capacity along the path
        minCapacity = Infinity
        for each edge (u,v) in path:
            minCapacity = min(minCapacity, capacity[u,v] - flow[u,v])
        
        // Augment flow along the path
        for each edge (u,v) in path:
            flow[u,v] += minCapacity
            flow[v,u] -= minCapacity  // Update reverse edge
        
        maxFlow += minCapacity
    
    return maxFlow

function findAugmentingPath(graph, source, sink):
    // Use BFS to find a path with available capacity
    queue = [source]
    visited = {source}
    parent = {}
    
    while queue is not empty:
        current = queue.dequeue()
        
        if current == sink:
            return reconstructPath(parent, source, sink)
        
        for each neighbor v of current:
            if v not visited and residualCapacity(current,v) > 0:
                visited.add(v)
                parent[v] = current
                queue.enqueue(v)
    
    return null  // No augmenting path found`;
  
  return (
    <div className="network-flow-visualization" style={{ fontFamily: 'Arial, sans-serif', color: 'white', backgroundColor: '#121212', padding: '20px', borderRadius: '8px', maxWidth: '100%' }}>
      <h1 style={{ color: '#00FFFF', textAlign: 'center' }}>Network Flow Visualization</h1>
      
      <div style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
        {/* Main visualization area */}
        <div style={{ display: 'flex', flexWrap: 'wrap', gap: '20px' }}>
          {/* Graph visualization */}
          <div style={{ flex: '1', minWidth: '300px' }}>
            <h2 style={{ color: '#FF00FF' }}>Flow Network</h2>
            {renderGraph()}
          </div>
          
          {/* Information panel */}
          {showInfo && (
            <div style={{ flex: '1', minWidth: '300px', backgroundColor: '#1A1A1A', padding: '15px', borderRadius: '8px' }}>
              <h2 style={{ color: '#FF00FF' }}>Algorithm State</h2>
              <div style={{ marginBottom: '15px' }}>
                <h3 style={{ color: '#00FFFF' }}>Current Step</h3>
                <p>{algorithmState.description}</p>
                <p style={{ color: '#00FF00', fontWeight: 'bold' }}>
                  Maximum Flow: {algorithmState.maxFlow}
                </p>
              </div>
              
              {currentPath.length > 0 && (
                <div style={{ marginBottom: '15px' }}>
                  <h3 style={{ color: '#00FFFF' }}>Current Augmenting Path</h3>
                  <p>{currentPath.join(' → ')}</p>
                  {steps[currentStep].minCapacity && (
                    <p>Minimum capacity along path: <span style={{ color: '#00FF00' }}>{steps[currentStep].minCapacity}</span></p>
                  )}
                </div>
              )}
              
              <div>
                <h3 style={{ color: '#00FFFF' }}>Complexity Analysis</h3>
                <p><strong>Time Complexity:</strong> O(E · |f|) where E is the number of edges and |f| is the maximum flow value.</p>
                <p><strong>With Edmonds-Karp:</strong> O(V · E²) where V is the number of vertices.</p>
                <p><strong>Space Complexity:</strong> O(V + E) for the residual graph.</p>
              </div>
            </div>
          )}
        </div>
        
        {/* Code view toggle */}
        {showCode && (
          <div style={{ backgroundColor: '#1A1A1A', padding: '15px', borderRadius: '8px', marginTop: '20px' }}>
            <h2 style={{ color: '#FF00FF' }}>Ford-Fulkerson Algorithm</h2>
            <pre style={{ 
              backgroundColor: '#0D0D0D', 
              padding: '15px', 
              borderRadius: '5px',
              overflow: 'auto',
              color: '#FFFFFF',
              fontFamily: 'monospace',
              fontSize: '14px',
              whiteSpace: 'pre-wrap'
            }}>
              {codeSnippet}
            </pre>
          </div>
        )}
        
        {/* Playback controls */}
        <div style={{ 
          display: 'flex', 
          justifyContent: 'center', 
          gap: '10px', 
          flexWrap: 'wrap',
          marginTop: '20px'
        }}>
          <button 
            onClick={handleReset}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Reset
          </button>
          <button 
            onClick={handleStepBack}
            disabled={currentStep === 0}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: currentStep === 0 ? 'not-allowed' : 'pointer',
              opacity: currentStep === 0 ? 0.5 : 1
            }}
          >
            ← Step
          </button>
          {isPlaying ? (
            <button 
              onClick={handlePause}
              style={{ 
                backgroundColor: colors.buttonBackground, 
                color: colors.buttonText,
                border: `1px solid ${colors.buttonText}`,
                padding: '8px 15px',
                borderRadius: '4px',
                cursor: 'pointer'
              }}
            >
              Pause
            </button>
          ) : (
            <button 
              onClick={handlePlay}
              disabled={currentStep === maxSteps}
              style={{ 
                backgroundColor: colors.buttonBackground, 
                color: colors.buttonText,
                border: `1px solid ${colors.buttonText}`,
                padding: '8px 15px',
                borderRadius: '4px',
                cursor: currentStep === maxSteps ? 'not-allowed' : 'pointer',
                opacity: currentStep === maxSteps ? 0.5 : 1
              }}
            >
              Play
            </button>
          )}
          <button 
            onClick={handleStepForward}
            disabled={currentStep === maxSteps}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: currentStep === maxSteps ? 'not-allowed' : 'pointer',
              opacity: currentStep === maxSteps ? 0.5 : 1
            }}
          >
            Step →
          </button>
          <select 
            value={playbackSpeed}
            onChange={(e) => setPlaybackSpeed(Number(e.target.value))}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            <option value={2000}>Slow</option>
            <option value={1000}>Normal</option>
            <option value={500}>Fast</option>
          </select>
          <button 
            onClick={() => setShowCode(!showCode)}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            {showCode ? 'Hide Code' : 'Show Code'}
          </button>
          <button 
            onClick={() => setShowInfo(!showInfo)}
            style={{ 
              backgroundColor: colors.buttonBackground, 
              color: colors.buttonText,
              border: `1px solid ${colors.buttonText}`,
              padding: '8px 15px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            {showInfo ? 'Hide Info' : 'Show Info'}
          </button>
        </div>
        
        {/* Status bar */}
        <div style={{ 
          backgroundColor: '#1A1A1A', 
          padding: '10px', 
          borderRadius: '4px',
          display: 'flex',
          justifyContent: 'space-between',
          marginTop: '10px'
        }}>
          <span>Step: {currentStep + 1} / {maxSteps + 1}</span>
          <span>Max Flow: {algorithmState.maxFlow}</span>
        </div>
      </div>
      
      {/* Tooltip container */}
      <div style={{ marginTop: '20px', backgroundColor: '#1A1A1A', padding: '10px', borderRadius: '8px' }}>
        <h3 style={{ color: '#FFFF00' }}>About Network Flow</h3>
        <p>
          Network flow algorithms solve the problem of finding the maximum flow in a flow network - 
          a directed graph where each edge has a capacity, from a source node to a sink node.
        </p>
        <p>
          The Ford-Fulkerson algorithm works by finding augmenting paths from source to sink 
          in the residual graph and sending flow along these paths until no more augmenting paths exist.
        </p>
        <h4 style={{ color: '#00FFFF' }}>Applications:</h4>
        <ul>
          <li>Transportation networks and traffic flow optimization</li>
          <li>Bipartite matching problems (job assignments, resource allocation)</li>
          <li>Network reliability and capacity planning</li>
          <li>Image segmentation in computer vision</li>
        </ul>
      </div>
    </div>
  );
};

export default NetworkFlowVisualization;