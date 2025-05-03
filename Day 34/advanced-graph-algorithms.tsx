import React, { useState, useEffect, useRef } from 'react';
import { Play, Pause, SkipForward, SkipBack, RotateCw, Code } from 'lucide-react';

// Main component
export default function AdvancedGraphAlgorithms() {
  // App state
  const [algorithm, setAlgorithm] = useState('scc');
  const [animationSpeed, setAnimationSpeed] = useState(1000);
  const [isRunning, setIsRunning] = useState(false);
  const [currentStep, setCurrentStep] = useState(0);
  const [steps, setSteps] = useState([]);
  const [graph, setGraph] = useState(null);
  const [showCode, setShowCode] = useState(false);
  const [result, setResult] = useState([]);
  
  // Canvas refs
  const canvasRef = useRef(null);
  const animationRef = useRef(null);
  
  // Initialize graph based on selected algorithm
  useEffect(() => {
    let initialGraph;
    
    if (algorithm === 'scc') {
      // Strongly Connected Components graph
      initialGraph = {
        nodes: [
          { id: 'A', x: 100, y: 100, color: '#00FFFF', component: -1 },
          { id: 'B', x: 200, y: 50, color: '#00FFFF', component: -1 },
          { id: 'C', x: 300, y: 100, color: '#00FFFF', component: -1 },
          { id: 'D', x: 200, y: 150, color: '#00FFFF', component: -1 },
          { id: 'E', x: 350, y: 200, color: '#00FFFF', component: -1 },
          { id: 'F', x: 250, y: 250, color: '#00FFFF', component: -1 },
          { id: 'G', x: 150, y: 250, color: '#00FFFF', component: -1 },
        ],
        edges: [
          { source: 'A', target: 'B', color: '#FFFFFF', directed: true },
          { source: 'B', target: 'C', color: '#FFFFFF', directed: true },
          { source: 'C', target: 'A', color: '#FFFFFF', directed: true },
          { source: 'C', target: 'D', color: '#FFFFFF', directed: true },
          { source: 'D', target: 'B', color: '#FFFFFF', directed: true },
          { source: 'E', target: 'C', color: '#FFFFFF', directed: true },
          { source: 'E', target: 'F', color: '#FFFFFF', directed: true },
          { source: 'F', target: 'G', color: '#FFFFFF', directed: true },
          { source: 'G', target: 'F', color: '#FFFFFF', directed: true },
          { source: 'G', target: 'D', color: '#FFFFFF', directed: true },
        ]
      };
    } else if (algorithm === 'articulation') {
      // Articulation Points and Bridges graph
      initialGraph = {
        nodes: [
          { id: 'A', x: 100, y: 100, color: '#00FFFF', isArticulation: false },
          { id: 'B', x: 200, y: 100, color: '#00FFFF', isArticulation: false },
          { id: 'C', x: 300, y: 100, color: '#00FFFF', isArticulation: false },
          { id: 'D', x: 250, y: 200, color: '#00FFFF', isArticulation: false },
          { id: 'E', x: 150, y: 200, color: '#00FFFF', isArticulation: false },
          { id: 'F', x: 50, y: 200, color: '#00FFFF', isArticulation: false },
          { id: 'G', x: 350, y: 200, color: '#00FFFF', isArticulation: false },
        ],
        edges: [
          { source: 'A', target: 'B', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'B', target: 'C', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'A', target: 'E', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'B', target: 'D', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'C', target: 'D', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'E', target: 'F', color: '#FFFFFF', isBridge: false, directed: false },
          { source: 'C', target: 'G', color: '#FFFFFF', isBridge: false, directed: false },
        ]
      };
    } else if (algorithm === 'flow') {
      // Flow Network graph
      initialGraph = {
        nodes: [
          { id: 'S', x: 50, y: 150, color: '#00FFFF', label: 'Source' },
          { id: 'A', x: 150, y: 75, color: '#00FFFF', label: '' },
          { id: 'B', x: 150, y: 225, color: '#00FFFF', label: '' },
          { id: 'C', x: 250, y: 75, color: '#00FFFF', label: '' },
          { id: 'D', x: 250, y: 225, color: '#00FFFF', label: '' },
          { id: 'T', x: 350, y: 150, color: '#00FFFF', label: 'Sink' },
        ],
        edges: [
          { source: 'S', target: 'A', color: '#FFFFFF', capacity: 10, flow: 0, directed: true },
          { source: 'S', target: 'B', color: '#FFFFFF', capacity: 8, flow: 0, directed: true },
          { source: 'A', target: 'C', color: '#FFFFFF', capacity: 6, flow: 0, directed: true },
          { source: 'A', target: 'B', color: '#FFFFFF', capacity: 5, flow: 0, directed: true },
          { source: 'B', target: 'D', color: '#FFFFFF', capacity: 7, flow: 0, directed: true },
          { source: 'C', target: 'T', color: '#FFFFFF', capacity: 8, flow: 0, directed: true },
          { source: 'C', target: 'D', color: '#FFFFFF', capacity: 3, flow: 0, directed: true },
          { source: 'D', target: 'T', color: '#FFFFFF', capacity: 9, flow: 0, directed: true },
        ]
      };
    }
    
    setGraph(initialGraph);
    generateSteps(initialGraph, algorithm);
  }, [algorithm]);
  
  // Draw the graph
  useEffect(() => {
    if (!graph) return;
    
    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw edges
    graph.edges.forEach(edge => {
      const source = graph.nodes.find(node => node.id === edge.source);
      const target = graph.nodes.find(node => node.id === edge.target);
      
      // Draw the edge line
      ctx.beginPath();
      ctx.moveTo(source.x, source.y);
      ctx.lineTo(target.x, target.y);
      ctx.strokeStyle = edge.color;
      ctx.lineWidth = 2;
      ctx.stroke();
      
      // If it's a directed edge, draw an arrow
      if (edge.directed) {
        const dx = target.x - source.x;
        const dy = target.y - source.y;
        const angle = Math.atan2(dy, dx);
        
        const headLen = 10;
        const x1 = target.x - headLen * Math.cos(angle - Math.PI / 6);
        const y1 = target.y - headLen * Math.sin(angle - Math.PI / 6);
        const x2 = target.x - headLen * Math.cos(angle + Math.PI / 6);
        const y2 = target.y - headLen * Math.sin(angle + Math.PI / 6);
        
        ctx.beginPath();
        ctx.moveTo(target.x, target.y);
        ctx.lineTo(x1, y1);
        ctx.lineTo(x2, y2);
        ctx.closePath();
        ctx.fillStyle = edge.color;
        ctx.fill();
      }
      
      // For flow networks, show capacity and flow
      if (algorithm === 'flow') {
        const midX = (source.x + target.x) / 2;
        const midY = (source.y + target.y) / 2;
        
        ctx.fillStyle = '#FFFF00';
        ctx.font = 'bold 12px Arial';
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText(`${edge.flow}/${edge.capacity}`, midX, midY);
      }
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
      
      // For articulation points, add indicator
      if (algorithm === 'articulation' && node.isArticulation) {
        ctx.beginPath();
        ctx.arc(node.x, node.y, 24, 0, 2 * Math.PI);
        ctx.strokeStyle = '#FF00FF';
        ctx.lineWidth = 2;
        ctx.stroke();
      }
      
      // For SCC, show component number
      if (algorithm === 'scc' && node.component !== -1) {
        ctx.fillStyle = '#FF00FF';
        ctx.font = '10px Arial';
        ctx.fillText(`SCC ${node.component}`, node.x, node.y - 24);
      }
      
      // For flow networks, show node label
      if (algorithm === 'flow' && node.label) {
        ctx.fillStyle = '#FF00FF';
        ctx.font = '12px Arial';
        ctx.fillText(node.label, node.x, node.y - 24);
      }
    });
  }, [graph, algorithm]);
  
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
      
      // Update node properties
      newGraph.nodes.forEach(node => {
        const stepNode = step.nodes.find(n => n.id === node.id);
        if (stepNode) {
          node.color = stepNode.color;
          
          if (algorithm === 'scc') {
            node.component = stepNode.component;
          } else if (algorithm === 'articulation') {
            node.isArticulation = stepNode.isArticulation;
          }
        }
      });
      
      // Update edge properties
      newGraph.edges.forEach(edge => {
        const stepEdge = step.edges.find(e => 
          e.source === edge.source && e.target === edge.target);
        if (stepEdge) {
          edge.color = stepEdge.color;
          
          if (algorithm === 'articulation') {
            edge.isBridge = stepEdge.isBridge;
          } else if (algorithm === 'flow') {
            edge.flow = stepEdge.flow;
          }
        }
      });
      
      return newGraph;
    });
    
    // Update result
    if (step.result) {
      setResult(step.result);
    }
  };
  
  // Generate algorithm steps
  const generateSteps = (graphData, algoType) => {
    const nodes = JSON.parse(JSON.stringify(graphData.nodes));
    const edges = JSON.parse(JSON.stringify(graphData.edges));
    const newSteps = [];
    
    // Initial state
    newSteps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `Initial graph for ${getAlgorithmName(algoType)}`,
      result: []
    });
    
    if (algoType === 'scc') {
      generateSCCSteps(nodes, edges, newSteps);
    } else if (algoType === 'articulation') {
      generateArticulationSteps(nodes, edges, newSteps);
    } else if (algoType === 'flow') {
      generateFlowNetworkSteps(nodes, edges, newSteps);
    }
    
    setSteps(newSteps);
    setCurrentStep(0);
    
    // Apply initial step
    if (newSteps.length > 0) {
      updateGraphState(newSteps[0]);
    }
  };
  
  // Get full algorithm name
  const getAlgorithmName = (algo) => {
    switch (algo) {
      case 'scc':
        return 'Strongly Connected Components (Kosaraju\'s Algorithm)';
      case 'articulation':
        return 'Articulation Points and Bridges';
      case 'flow':
        return 'Flow Networks (Ford-Fulkerson Algorithm)';
      default:
        return 'Unknown Algorithm';
    }
  };
  
  // Generate steps for Strongly Connected Components
  const generateSCCSteps = (nodes, edges, steps) => {
    // Convert graph to adjacency list
    const graph = {};
    const transposedGraph = {};
    
    nodes.forEach(node => {
      graph[node.id] = [];
      transposedGraph[node.id] = [];
    });
    
    edges.forEach(edge => {
      graph[edge.source].push(edge.target);
      transposedGraph[edge.target].push(edge.source);
    });
    
    // Step 1: First DFS to determine finishing order
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: "Step 1: First DFS to determine finishing order",
      result: []
    });
    
    const visited = {};
    const finishingOrder = [];
    
    // DFS function for first pass
    const dfs1 = (nodeId) => {
      visited[nodeId] = true;
      nodes.find(n => n.id === nodeId).color = '#FFFF00';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Visiting node ${nodeId} in first DFS`,
        result: []
      });
      
      for (const neighbor of graph[nodeId]) {
        if (!visited[neighbor]) {
          // Highlight edge being traversed
          const edgeIndex = edges.findIndex(e => e.source === nodeId && e.target === neighbor);
          edges[edgeIndex].color = '#FFFF00';
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Exploring edge ${nodeId} → ${neighbor}`,
            result: []
          });
          
          dfs1(neighbor);
          
          // Reset edge color
          edges[edgeIndex].color = '#FFFFFF';
        }
      }
      
      // Push to finishingOrder after all neighbors processed
      finishingOrder.push(nodeId);
      nodes.find(n => n.id === nodeId).color = '#FF00FF';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Finished with node ${nodeId}, adding to stack. Current stack: [${finishingOrder.join(', ')}]`,
        result: []
      });
    };
    
    // Perform first DFS
    for (const node of nodes) {
      if (!visited[node.id]) {
        dfs1(node.id);
      }
    }
    
    // Step 2: Create transposed graph
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: "Step 2: Creating transposed graph (reversing all edges)",
      result: []
    });
    
    // Reset colors
    nodes.forEach(node => node.color = '#00FFFF');
    
    // Show transposed edges
    const transposedEdges = JSON.parse(JSON.stringify(edges));
    transposedEdges.forEach(edge => {
      [edge.source, edge.target] = [edge.target, edge.source];
      edge.color = '#00FF00';
    });
    
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: transposedEdges,
      description: "Transposed graph created (all edges reversed)",
      result: []
    });
    
    // Step 3: Second DFS in order of finishing time
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: "Step 3: Second DFS in order of finishing time",
      result: []
    });
    
    // Reset visited
    Object.keys(visited).forEach(key => visited[key] = false);
    
    // For storing SCCs
    const SCCs = [];
    let currentComponent = 0;
    
    // DFS function for second pass
    const dfs2 = (nodeId, componentId) => {
      visited[nodeId] = true;
      const nodeObj = nodes.find(n => n.id === nodeId);
      nodeObj.color = getComponentColor(componentId);
      nodeObj.component = componentId;
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Visiting node ${nodeId} in second DFS, assigning to SCC ${componentId}`,
        result: SCCs
      });
      
      for (const neighbor of transposedGraph[nodeId]) {
        if (!visited[neighbor]) {
          // Highlight edge being traversed
          const edgeIndex = edges.findIndex(e => e.source === neighbor && e.target === nodeId);
          edges[edgeIndex].color = getComponentColor(componentId);
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Exploring edge ${neighbor} → ${nodeId} in transposed graph`,
            result: SCCs
          });
          
          dfs2(neighbor, componentId);
        }
      }
    };
    
    // Perform second DFS
    for (let i = finishingOrder.length - 1; i >= 0; i--) {
      const nodeId = finishingOrder[i];
      if (!visited[nodeId]) {
        SCCs.push([]);
        dfs2(nodeId, currentComponent);
        
        // Update current SCC with nodes
        SCCs[currentComponent] = nodes
          .filter(n => n.component === currentComponent)
          .map(n => n.id);
        
        currentComponent++;
      }
    }
    
    // Final state
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `Algorithm completed. Found ${SCCs.length} strongly connected components.`,
      result: SCCs
    });
  };
  
  // Get color for SCC component
  const getComponentColor = (componentId) => {
    const colors = ['#FF00FF', '#00FF00', '#FFFF00', '#FF5500', '#00FFFF'];
    return colors[componentId % colors.length];
  };
  
  // Generate steps for Articulation Points and Bridges
  const generateArticulationSteps = (nodes, edges, steps) => {
    // Convert graph to adjacency list
    const graph = {};
    
    nodes.forEach(node => {
      graph[node.id] = [];
    });
    
    edges.forEach(edge => {
      graph[edge.source].push(edge.target);
      graph[edge.target].push(edge.source);
    });
    
    // Arrays for tracking
    const disc = {};
    const low = {};
    const parent = {};
    const visited = {};
    const articPoints = new Set();
    const bridges = [];
    
    // Initialize
    nodes.forEach(node => {
      disc[node.id] = -1;
      low[node.id] = -1;
      parent[node.id] = null;
      visited[node.id] = false;
    });
    
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: "Using Tarjan's algorithm to find articulation points and bridges",
      result: []
    });
    
    let time = 0;
    
    // DFS function for articulation points and bridges
    const dfs = (u) => {
      visited[u] = true;
      disc[u] = low[u] = ++time;
      let children = 0;
      
      nodes.find(n => n.id === u).color = '#FFFF00';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Visiting node ${u}, discovery time = ${disc[u]}`,
        result: []
      });
      
      for (const v of graph[u]) {
        if (!visited[v]) {
          children++;
          parent[v] = u;
          
          // Highlight edge being explored
          const edgeIndex = edges.findIndex(e => 
            (e.source === u && e.target === v) || (e.source === v && e.target === u));
          edges[edgeIndex].color = '#FFFF00';
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Exploring edge ${u} - ${v}`,
            result: []
          });
          
          dfs(v);
          
          // Update low value based on child
          low[u] = Math.min(low[u], low[v]);
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Updated low[${u}] = ${low[u]} after processing ${v}`,
            result: []
          });
          
          // Check for articulation point
          if ((parent[u] === null && children > 1) || 
              (parent[u] !== null && low[v] >= disc[u])) {
            articPoints.add(u);
            nodes.find(n => n.id === u).isArticulation = true;
            nodes.find(n => n.id === u).color = '#FF00FF';
            
            steps.push({
              nodes: JSON.parse(JSON.stringify(nodes)),
              edges: JSON.parse(JSON.stringify(edges)),
              description: `Node ${u} is an articulation point`,
              result: Array.from(articPoints)
            });
          }
          
          // Check for bridge
          if (low[v] > disc[u]) {
            bridges.push([u, v]);
            edges[edgeIndex].color = '#FF00FF';
            edges[edgeIndex].isBridge = true;
            
            steps.push({
              nodes: JSON.parse(JSON.stringify(nodes)),
              edges: JSON.parse(JSON.stringify(edges)),
              description: `Edge ${u} - ${v} is a bridge`,
              result: bridges
            });
          } else {
            edges[edgeIndex].color = '#00FF00';
          }
        }
        else if (v !== parent[u]) {
          // Back edge, update low value
          low[u] = Math.min(low[u], disc[v]);
          
          // Highlight back edge
          const edgeIndex = edges.findIndex(e => 
            (e.source === u && e.target === v) || (e.source === v && e.target === u));
          edges[edgeIndex].color = '#00FFFF';
          
          steps.push({
            nodes: JSON.parse(JSON.stringify(nodes)),
            edges: JSON.parse(JSON.stringify(edges)),
            description: `Back edge ${u} - ${v} found, updated low[${u}] = ${low[u]}`,
            result: []
          });
        }
      }
      
      if (!nodes.find(n => n.id === u).isArticulation) {
        nodes.find(n => n.id === u).color = '#00FF00';
      }
    };
    
    // Run DFS from each unvisited node
    for (const node of nodes) {
      if (!visited[node.id]) {
        dfs(node.id);
      }
    }
    
    // Final state
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `Algorithm completed. Found ${articPoints.size} articulation points and ${bridges.length} bridges.`,
      result: {
        articulationPoints: Array.from(articPoints),
        bridges: bridges
      }
    });
  };
  
  // Generate steps for Flow Networks
  const generateFlowNetworkSteps = (nodes, edges, steps) => {
    // Find source and sink nodes
    const source = 'S';
    const sink = 'T';
    
    // Convert to residual graph
    const residualGraph = {};
    
    nodes.forEach(node => {
      residualGraph[node.id] = {};
    });
    
    edges.forEach(edge => {
      residualGraph[edge.source][edge.target] = edge.capacity;
      
      // Add reverse edge with zero capacity
      if (!residualGraph[edge.target][edge.source]) {
        residualGraph[edge.target][edge.source] = 0;
      }
    });
    
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: "Ford-Fulkerson algorithm: Find augmenting paths until no more exist",
      result: { maxFlow: 0 }
    });
    
    let maxFlow = 0;
    
    // Find an augmenting path using BFS
    const findAugmentingPath = () => {
      const visited = {};
      const parent = {};
      const queue = [];
      
      // Initialize
      nodes.forEach(node => {
        visited[node.id] = false;
        parent[node.id] = null;
      });
      
      // Start BFS from source
      queue.push(source);
      visited[source] = true;
      
      // Update node visualization
      nodes.forEach(node => {
        node.color = '#00FFFF';
      });
      nodes.find(n => n.id === source).color = '#FFFF00';
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: "Looking for an augmenting path from source to sink",
        result: { maxFlow }
      });
      
      // BFS
      while (queue.length > 0) {
        const u = queue.shift();
        
        for (const v in residualGraph[u]) {
          if (!visited[v] && residualGraph[u][v] > 0) {
            // Highlight current edge being explored
            const edgeIndex = edges.findIndex(e => e.source === u && e.target === v) || 
                             edges.findIndex(e => e.source === v && e.target === u && residualGraph[u][v] > 0);
            
            if (edgeIndex >= 0) {
              edges[edgeIndex].color = '#FFFF00';
              
              steps.push({
                nodes: JSON.parse(JSON.stringify(nodes)),
                edges: JSON.parse(JSON.stringify(edges)),
                description: `Checking edge ${u} → ${v} with remaining capacity ${residualGraph[u][v]}`,
                result: { maxFlow }
              });
              
              edges[edgeIndex].color = '#FFFFFF';
            }
            
            queue.push(v);
            parent[v] = u;
            visited[v] = true;
            
            // Update node visualization
            nodes.find(n => n.id === v).color = '#FFFF00';
            
            steps.push({
              nodes: JSON.parse(JSON.stringify(nodes)),
              edges: JSON.parse(JSON.stringify(edges)),
              description: `Visiting node ${v} from ${u}`,
              result: { maxFlow }
            });
            
            // If sink is reached, we found a path
            if (v === sink) {
              return parent;
            }
          }
        }
      }
      
      // No augmenting path found
      return null;
    };
    
    // While there is an augmenting path
    let iteration = 1;
    while (true) {
      const parent = findAugmentingPath();
      
      if (!parent) {
        steps.push({
          nodes: JSON.parse(JSON.stringify(nodes)),
          edges: JSON.parse(JSON.stringify(edges)),
          description: "No more augmenting paths found, algorithm terminates",
          result: { maxFlow }
        });
        break;
      }
      
      // Find bottleneck capacity
      let pathFlow = Infinity;
      for (let v = sink; v !== source; v = parent[v]) {
        const u = parent[v];
        pathFlow = Math.min(pathFlow, residualGraph[u][v]);
      }
      
      // Highlight the augmenting path
      let pathNodes = [sink];
      for (let v = sink; v !== source; v = parent[v]) {
        pathNodes.push(parent[v]);
      }
      pathNodes.reverse();
      
      // Highlight path nodes
      nodes.forEach(node => {
        if (pathNodes.includes(node.id)) {
          node.color = '#00FF00';
        } else {
          node.color = '#00FFFF';
        }
      });
      
      // Highlight path edges
      for (let i = 0; i < pathNodes.length - 1; i++) {
        const u = pathNodes[i];
        const v = pathNodes[i+1];
        const edgeIndex = edges.findIndex(e => e.source === u && e.target === v);
        if (edgeIndex >= 0) {
          edges[edgeIndex].color = '#00FF00';
        }
      }
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Found augmenting path: ${pathNodes.join(' → ')} with bottleneck capacity ${pathFlow}`,
        result: { maxFlow, path: pathNodes, bottleneck: pathFlow }
      });
      
      // Update residual graph and flow values
      for (let v = sink; v !== source; v = parent[v]) {
        const u = parent[v];
        residualGraph[u][v] -= pathFlow;
        residualGraph[v][u] += pathFlow;
        
        // Update flow in original edges
        const edgeIndex = edges.findIndex(e => e.source === u && e.target === v);
        if (edgeIndex >= 0) {
          edges[edgeIndex].flow += pathFlow;
        }
      }
      
      maxFlow += pathFlow;
      
      // Reset edge colors
      edges.forEach(edge => {
        edge.color = '#FFFFFF';
      });
      
      steps.push({
        nodes: JSON.parse(JSON.stringify(nodes)),
        edges: JSON.parse(JSON.stringify(edges)),
        description: `Iteration ${iteration}: Updated flow, current max flow = ${maxFlow}`,
        result: { maxFlow }
      });
      
      iteration++;
    }
    
    // Final state - show min cut
    const visited = {};
    const minCutEdges = [];
    
    // DFS to find reachable nodes in residual graph
    const dfsMinCut = (u) => {
      visited[u] = true;
      nodes.find(n => n.id === u).color = '#FF00FF';
      
      for (const v in residualGraph[u]) {
        if (!visited[v] && residualGraph[u][v] > 0) {
          dfsMinCut(v);
        }
      }
    };
    
    // Mark source side
    dfsMinCut(source);
    
    // Find min cut edges
    edges.forEach(edge => {
      if (visited[edge.source] && !visited[edge.target]) {
        edge.color = '#FF00FF';
        minCutEdges.push([edge.source, edge.target]);
      }
    });
    
    steps.push({
      nodes: JSON.parse(JSON.stringify(nodes)),
      edges: JSON.parse(JSON.stringify(edges)),
      description: `Final result: Maximum flow = ${maxFlow}. Min-cut edges shown in pink.`,
      result: { maxFlow, minCut: minCutEdges }
    });
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
    setResult([]);
  };
  
  const handleSpeedChange = (e) => {
    setAnimationSpeed(parseInt(e.target.value));
  };
  
  const toggleCodeView = () => {
    setShowCode(!showCode);
  };
  
  // Code snippets for each algorithm
  const getCodeSnippet = () => {
    if (algorithm === 'scc') {
      return `// Kosaraju's Algorithm for SCCs
vector<vector<int>> kosarajuSCC(vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    stack<int> stack;
    
    // First DFS to fill stack in order of finishing times
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            fillOrder(i, adj, visited, stack);
        }
    }
    
    // Create transpose of the graph
    vector<vector<int>> transpose(n);
    for (int i = 0; i < n; i++) {
        for (int j : adj[i]) {
            transpose[j].push_back(i);
        }
    }
    
    // Reset visited array
    fill(visited.begin(), visited.end(), false);
    
    // Process vertices in order of finishing times
    vector<vector<int>> result;
    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();
        
        if (!visited[v]) {
            vector<int> component;
            DFS(v, transpose, visited, component);
            result.push_back(component);
        }
    }
    
    return result;
}

// Helper for first DFS
void fillOrder(int v, vector<vector<int>>& adj, vector<bool>& visited, stack<int>& stack) {
    visited[v] = true;
    
    for (int u : adj[v]) {
        if (!visited[u]) {
            fillOrder(u, adj, visited, stack);
        }
    }
    
    stack.push(v);
}

// Helper for second DFS
void DFS(int v, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    
    for (int u : adj[v]) {
        if (!visited[u]) {
            DFS(u, adj, visited, component);
        }
    }
}`;
    } else if (algorithm === 'articulation') {
      return `// Find Articulation Points and Bridges
void findArticulationPoints(vector<vector<int>>& adj, set<int>& artPoints, vector<pair<int, int>>& bridges) {
    int n = adj.size();
    vector<int> disc(n, -1);   // Discovery times
    vector<int> low(n, -1);    // Earliest visited vertex reachable
    vector<int> parent(n, -1); // Parent in DFS tree
    vector<bool> visited(n, false);
    int time = 0;
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(i, adj, visited, disc, low, parent, artPoints, bridges, time);
        }
    }
}

void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited, 
         vector<int>& disc, vector<int>& low, vector<int>& parent,
         set<int>& artPoints, vector<pair<int, int>>& bridges, int& time) {
    
    visited[u] = true;
    disc[u] = low[u] = ++time;
    int children = 0;
    
    for (int v : adj[u]) {
        if (!visited[v]) {
            children++;
            parent[v] = u;
            
            dfs(v, adj, visited, disc, low, parent, artPoints, bridges, time);
            
            // Check if subtree rooted at v has a connection to ancestor of u
            low[u] = min(low[u], low[v]);
            
            // Articulation point case 1: u is root of DFS tree and has multiple children
            if (parent[u] == -1 && children > 1) {
                artPoints.insert(u);
            }
            
            // Articulation point case 2: u is not root and low value of child is >= discovery value of u
            if (parent[u] != -1 && low[v] >= disc[u]) {
                artPoints.insert(u);
            }
            
            // Bridge case: If the lowest vertex reachable from subtree is below u
            if (low[v] > disc[u]) {
                bridges.push_back({u, v});
            }
        }
        else if (v != parent[u]) {
            // Update low value of u for parent function calls
            low[u] = min(low[u], disc[v]);
        }
    }
}`;
    } else if (algorithm === 'flow') {
      return `// Ford-Fulkerson Algorithm for Max Flow
int fordFulkerson(vector<vector<int>>& capacity, int source, int sink) {
    int n = capacity.size();
    vector<vector<int>> residual = capacity; // Residual capacity
    vector<int> parent(n);
    int max_flow = 0;
    
    // Augment the flow while there is a path from source to sink
    while (bfs(residual, source, sink, parent)) {
        // Find minimum residual capacity along the path
        int path_flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, residual[u][v]);
        }
        
        // Update residual capacities
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow; // Reverse edge
        }
        
        // Add path flow to overall flow
        max_flow += path_flow;
    }
    
    return max_flow;
}

// Uses BFS to find path in residual graph
bool bfs(vector<vector<int>>& residual, int source, int sink, vector<int>& parent) {
    int n = residual.size();
    vector<bool> visited(n, false);
    queue<int> q;
    
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    
    return visited[sink]; // Return true if sink was reached
}`;
    }
  };
  
  // Time and space complexity info
  const getComplexityInfo = () => {
    if (algorithm === 'scc') {
      return {
        time: 'O(V + E)',
        space: 'O(V)',
        description: 'Both DFS passes traverse each vertex and edge exactly once'
      };
    } else if (algorithm === 'articulation') {
      return {
        time: 'O(V + E)',
        space: 'O(V)',
        description: 'Performs a single DFS traversal of the graph'
      };
    } else if (algorithm === 'flow') {
      return {
        time: 'O(E * maxFlow)',
        space: 'O(V²)',
        description: 'Edmonds-Karp implementation is O(V * E²)'
      };
    }
  };
  
  // Get result display information
  const getResultDisplay = () => {
    if (algorithm === 'scc' && Array.isArray(result) && result.length > 0) {
      return (
        <div>
          <h3 className="text-lg font-semibold text-cyan-400 mb-2">Strongly Connected Components</h3>
          <div className="grid grid-cols-1 gap-2">
            {result.map((component, idx) => (
              <div key={idx} className="border border-gray-700 rounded p-2">
                <span className="font-semibold" style={{color: getComponentColor(idx)}}>
                  SCC {idx}:
                </span>{' '}
                {component.join(' → ')}
              </div>
            ))}
          </div>
        </div>
      );
    } else if (algorithm === 'articulation' && result && result.articulationPoints) {
      return (
        <div>
          <h3 className="text-lg font-semibold text-pink-400 mb-2">Critical Points</h3>
          <div className="grid grid-cols-2 gap-4">
            <div className="border border-gray-700 rounded p-2">
              <span className="font-semibold text-pink-400">Articulation Points:</span>{' '}
              {result.articulationPoints.length > 0 ? result.articulationPoints.join(', ') : 'None'}
            </div>
            <div className="border border-gray-700 rounded p-2">
              <span className="font-semibold text-pink-400">Bridges:</span>{' '}
              {result.bridges && result.bridges.length > 0 
                ? result.bridges.map(b => `${b[0]}-${b[1]}`).join(', ') 
                : 'None'}
            </div>
          </div>
        </div>
      );
    } else if (algorithm === 'flow' && result && result.maxFlow !== undefined) {
      return (
        <div>
          <h3 className="text-lg font-semibold text-green-400 mb-2">Flow Results</h3>
          <div className="grid grid-cols-1 gap-2">
            <div className="border border-gray-700 rounded p-2">
              <span className="font-semibold text-green-400">Maximum Flow:</span>{' '}
              {result.maxFlow}
            </div>
            {result.path && (
              <div className="border border-gray-700 rounded p-2">
                <span className="font-semibold text-yellow-300">Current Path:</span>{' '}
                {result.path.join(' → ')} (Flow: {result.bottleneck})
              </div>
            )}
            {result.minCut && result.minCut.length > 0 && (
              <div className="border border-gray-700 rounded p-2">
                <span className="font-semibold text-pink-400">Min-Cut Edges:</span>{' '}
                {result.minCut.map(mc => `${mc[0]}-${mc[1]}`).join(', ')}
              </div>
            )}
          </div>
        </div>
      );
    }
    
    return null;
  };
  
  // Get application examples
  const getApplications = () => {
    if (algorithm === 'scc') {
      return [
        'Web page analysis (finding closely related pages)',
        'Social network analysis (finding communities)',
        'Compiler design (identifying cyclic dependencies)',
        'Circuit design (finding feedback paths)',
        'Game theory (finding stable states)'
      ];
    } else if (algorithm === 'articulation') {
      return [
        'Network reliability analysis',
        'Transport network planning',
        'Supply chain vulnerability assessment',
        'Critical infrastructure protection',
        'Circuit design and analysis'
      ];
    } else if (algorithm === 'flow') {
      return [
        'Transportation networks (traffic flow)',
        'Communication networks (data routing)',
        'Resource allocation problems',
        'Bipartite matching (assignment problems)',
        'Image segmentation',
        'Sports tournament scheduling'
      ];
    }
  };

  return (
    <div className="flex flex-col items-center w-full h-full bg-gray-900 text-white p-4">
      <h1 className="text-2xl font-bold mb-4 text-cyan-400">#DSAin45 Day 34: Advanced Graph Algorithms</h1>
      
      <div className="flex justify-between w-full mb-4">
        <div className="w-1/2 pr-2">
          <select 
            value={algorithm} 
            onChange={handleAlgorithmChange} 
            className="w-full p-2 bg-gray-800 text-cyan-400 border border-cyan-600 rounded"
          >
            <option value="scc">Strongly Connected Components</option>
            <option value="articulation">Articulation Points & Bridges</option>
            <option value="flow">Flow Networks</option>
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
          height="400" 
          className="w-full h-full bg-gray-800 rounded-lg"
        />
      </div>
      
      <div className="w-full p-4 bg-gray-800 rounded-lg mb-4">
        <h3 className="text-lg font-semibold text-green-400 mb-2">Current Status</h3>
        <p className="text-yellow-300">{steps[currentStep]?.description || 'Ready to start'}</p>
        <div className="mt-2">
          <span className="text-pink-400">Step: {currentStep + 1}/{steps.length}</span>
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
      
      {result && result.length !== 0 && getResultDisplay()}
      
      {showCode ? (
        <div className="w-full p-4 bg-gray-800 rounded-lg mb-4 overflow-auto max-h-96">
          <h3 className="text-lg font-semibold text-pink-400 mb-2">C++ Implementation</h3>
          <pre className="text-xs text-green-300 whitespace-pre-wrap">
            {getCodeSnippet()}
          </pre>
        </div>
      ) : (
        <div className="w-full grid grid-cols-1 gap-4 mb-4">
          <div className="p-4 bg-gray-800 rounded-lg">
            <h3 className="text-lg font-semibold text-pink-400 mb-2">Algorithm Analysis</h3>
            <div className="grid grid-cols-2 gap-4">
              <div>
                <p className="text-cyan-400 font-semibold">Time Complexity:</p>
                <p className="text-white">{getComplexityInfo().time}</p>
              </div>
              <div>
                <p className="text-cyan-400 font-semibold">Space Complexity:</p>
                <p className="text-white">{getComplexityInfo().space}</p>
              </div>
              <div className="col-span-2">
                <p className="text-green-400 font-semibold">Notes:</p>
                <p className="text-white">{getComplexityInfo().description}</p>
              </div>
            </div>
          </div>
          
          <div className="p-4 bg-gray-800 rounded-lg">
            <h3 className="text-lg font-semibold text-yellow-300 mb-2">Applications</h3>
            <ul className="list-disc list-inside">
              {getApplications().map((app, index) => (
                <li key={index} className="text-white mb-1">{app}</li>
              ))}
            </ul>
          </div>
        </div>
      )}
      
      <div className="w-full text-center text-sm text-gray-400">
        <p>Click the play button to animate the algorithm or use the step buttons to navigate manually.</p>
      </div>
    </div>
  );
}