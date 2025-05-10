import React, { useState, useEffect, useRef } from 'react';

const AlgorithmIntegrationVisualizer = () => {
  const [activeTab, setActiveTab] = useState('fileIndexing');
  const [searchResults, setSearchResults] = useState([]);
  const [searchQuery, setSearchQuery] = useState('');
  const [isSearching, setIsSearching] = useState(false);
  const [documents, setDocuments] = useState([]);
  const [indexStats, setIndexStats] = useState({});
  
  const [routeProblem, setRouteProblem] = useState({ locations: [], vehicleCapacity: 50 });
  const [currentRoute, setCurrentRoute] = useState({ path: [], distance: 0, feasible: false });
  const [isOptimizing, setIsOptimizing] = useState(false);
  const canvasRef = useRef(null);
  
  const [packetFlow, setPacketFlow] = useState([]);
  const [suspiciousIPs, setSuspiciousIPs] = useState([]);
  const [networkStats, setNetworkStats] = useState({});
  const [isAnalyzing, setIsAnalyzing] = useState(false);
  
  const [cacheNodes, setCacheNodes] = useState([]);
  const [cachePolicy, setCachePolicy] = useState('LRU');
  const [cacheKeys, setCacheKeys] = useState([]);
  const [requestStats, setRequestStats] = useState({ hits: 0, misses: 0 });
  const [isProcessing, setIsProcessing] = useState(false);

  // ===== FILE INDEXING SYSTEM =====
  
  // Initialize sample documents
  useEffect(() => {
    const sampleDocs = [
      {
        id: 1,
        title: "Introduction to Algorithms",
        content: "Algorithms are step-by-step procedures for calculations. Algorithm design is a specific method to create a mathematical process in solving problems. Algorithms are widely used throughout all areas of IT."
      },
      {
        id: 2,
        title: "Data Structures Explained",
        content: "Data structures are specialized formats for organizing and storing data. General data structure types include arrays, files, linked lists, stacks, queues, trees, graphs and more. Data structures are used in almost every program or software system."
      },
      {
        id: 3,
        title: "The Art of Programming",
        content: "Programming is the process of creating a set of instructions that tell a computer how to perform a task. Programming can be done using a variety of computer programming languages, such as JavaScript, Python, and C++."
      },
      {
        id: 4,
        title: "Algorithms and Data Structures",
        content: "Algorithms and data structures form the core of computer science. An algorithm is a sequence of steps designed to accomplish a specific task. Data structures are specialized formats used to organize and store data."
      },
      {
        id: 5,
        title: "Machine Learning Basics",
        content: "Machine learning is a method of data analysis that automates analytical model building. It is a branch of artificial intelligence based on the idea that systems can learn from data, identify patterns and make decisions with minimal human intervention."
      }
    ];
    
    setDocuments(sampleDocs);
    
    // Calculate index statistics
    const stats = {
      totalDocuments: sampleDocs.length,
      totalTerms: sampleDocs.reduce((sum, doc) => sum + doc.content.split(/\s+/).length, 0),
      uniqueTerms: new Set(sampleDocs.flatMap(doc => doc.content.toLowerCase().split(/\W+/).filter(t => t))).size,
      avgDocLength: Math.round(sampleDocs.reduce((sum, doc) => sum + doc.content.length, 0) / sampleDocs.length)
    };
    
    setIndexStats(stats);
  }, []);
  
  // Search function
  const searchDocuments = (query) => {
    if (!query.trim()) {
      setSearchResults([]);
      return;
    }
    
    setIsSearching(true);
    
    // Simulate searching delay
    setTimeout(() => {
      const terms = query.toLowerCase().split(/\s+/);
      
      const results = documents.map(doc => {
        // Calculate a simple TF-IDF-like score
        let score = 0;
        const docTerms = doc.content.toLowerCase().split(/\W+/).filter(t => t);
        
        for (const term of terms) {
          const termFreq = docTerms.filter(t => t === term).length;
          if (termFreq > 0) {
            // Simple TF-IDF approximation
            const tf = termFreq / docTerms.length;
            const docFreq = documents.filter(d => d.content.toLowerCase().includes(term)).length;
            const idf = Math.log(documents.length / docFreq);
            score += tf * idf;
          }
        }
        
        return {
          id: doc.id,
          title: doc.title,
          snippet: highlightTerms(doc.content, terms),
          score
        };
      }).filter(result => result.score > 0)
        .sort((a, b) => b.score - a.score);
      
      setSearchResults(results);
      setIsSearching(false);
    }, 500);
  };
  
  // Highlight search terms in text
  const highlightTerms = (text, terms) => {
    let snippet = text;
    if (text.length > 150) {
      // Find a good starting position for the snippet
      const firstTerm = terms[0];
      const index = text.toLowerCase().indexOf(firstTerm);
      
      if (index >= 0) {
        const start = Math.max(0, index - 50);
        snippet = (start > 0 ? '...' : '') + 
                 text.substring(start, start + 150) + 
                 (start + 150 < text.length ? '...' : '');
      } else {
        snippet = text.substring(0, 150) + '...';
      }
    }
    
    return snippet;
  };
  
  // ===== ROUTE OPTIMIZATION =====
  
  // Initialize route problem
  useEffect(() => {
    if (activeTab === 'routeOptimization') {
      // Create random locations
      const numLocations = 10;
      const locations = [];
      
      // Add depot at center
      locations.push({
        id: 0,
        x: 250,
        y: 250,
        packageWeight: 0,
        earliestArrival: 0,
        latestArrival: 480,
        serviceTime: 0,
        isDepot: true
      });
      
      // Add customer locations
      for (let i = 1; i <= numLocations; i++) {
        locations.push({
          id: i,
          x: Math.random() * 450 + 25,
          y: Math.random() * 450 + 25,
          packageWeight: Math.floor(Math.random() * 10) + 1,
          earliestArrival: Math.floor(Math.random() * 240),
          latestArrival: Math.floor(Math.random() * 240) + 240,
          serviceTime: Math.floor(Math.random() * 15) + 5,
          isDepot: false
        });
      }
      
      setRouteProblem({
        locations,
        vehicleCapacity: 50
      });
      
      // Initial solution
      const initialRoute = createInitialSolution(locations, 50);
      setCurrentRoute(initialRoute);
      
      // Draw the route
      drawRoute();
    }
  }, [activeTab]);
  
  // Update canvas when route changes
  useEffect(() => {
    if (activeTab === 'routeOptimization') {
      drawRoute();
    }
  }, [currentRoute, routeProblem]);
  
  // Draw the route on canvas
  const drawRoute = () => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const { locations } = routeProblem;
    const { path, feasible } = currentRoute;
    
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw locations
    for (const location of locations) {
      ctx.beginPath();
      ctx.arc(location.x, location.y, location.isDepot ? 12 : 8, 0, Math.PI * 2);
      
      if (location.isDepot) {
        ctx.fillStyle = '#00FFFF';
      } else {
        // Color based on time window
        const timeRatio = location.earliestArrival / 480;
        const hue = timeRatio * 240; // Blue to red
        ctx.fillStyle = `hsl(${hue}, 100%, 50%)`;
      }
      
      ctx.fill();
      ctx.stroke();
      
      // Draw ID
      ctx.fillStyle = '#FFFFFF';
      ctx.font = '10px Arial';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(location.id, location.x, location.y);
      
      // Draw package weight
      if (!location.isDepot) {
        ctx.fillStyle = '#FFFF00';
        ctx.font = '8px Arial';
        ctx.fillText(location.packageWeight, location.x, location.y - 15);
      }
    }
    
    // Draw route
    if (path.length > 0) {
      ctx.beginPath();
      
      // Start at depot
      ctx.moveTo(locations[0].x, locations[0].y);
      
      // Draw lines to each location in path
      for (const locId of path) {
        const loc = locations.find(l => l.id === locId);
        if (loc) {
          ctx.lineTo(loc.x, loc.y);
        }
      }
      
      // Return to depot
      ctx.lineTo(locations[0].x, locations[0].y);
      
      // Style based on feasibility
      ctx.strokeStyle = feasible ? '#00FF00' : '#FF00FF';
      ctx.lineWidth = 2;
      ctx.stroke();
    }
  };
  
  // Create initial solution (greedy)
  const createInitialSolution = (locations, capacity) => {
    // Simple nearest neighbor solution
    const path = [];
    const visited = new Set([0]); // Start at depot
    let currentLocation = 0;
    let currentLoad = 0;
    
    while (visited.size < locations.length) {
      let bestLocation = null;
      let bestDistance = Infinity;
      
      for (let i = 1; i < locations.length; i++) {
        if (!visited.has(locations[i].id)) {
          // Check capacity constraint
          if (currentLoad + locations[i].packageWeight <= capacity) {
            const dist = distance(
              locations[currentLocation], 
              locations[i]
            );
            
            if (dist < bestDistance) {
              bestDistance = dist;
              bestLocation = locations[i];
            }
          }
        }
      }
      
      if (bestLocation) {
        path.push(bestLocation.id);
        visited.add(bestLocation.id);
        currentLocation = bestLocation.id;
        currentLoad += bestLocation.packageWeight;
      } else {
        break; // No more locations can be visited
      }
    }
    
    // Calculate total distance
    let totalDistance = 0;
    let prevLocation = 0; // Start at depot
    
    for (const locId of path) {
      const loc = locations.find(l => l.id === locId);
      totalDistance += distance(
        locations[prevLocation], 
        loc
      );
      prevLocation = locId;
    }
    
    // Return to depot
    totalDistance += distance(
      locations[prevLocation], 
      locations[0]
    );
    
    return {
      path,
      distance: Math.round(totalDistance),
      feasible: visited.size === locations.length
    };
  };
  
  // Distance between two locations
  const distance = (a, b) => {
    return Math.sqrt(Math.pow(a.x - b.x, 2) + Math.pow(a.y - b.y, 2));
  };
  
  // Optimize route
  const optimizeRoute = () => {
    setIsOptimizing(true);
    
    // Simulate optimization with simulated annealing
    setTimeout(() => {
      const { locations, vehicleCapacity } = routeProblem;
      let currentPath = [...currentRoute.path];
      let currentDistance = currentRoute.distance;
      let bestPath = [...currentPath];
      let bestDistance = currentDistance;
      
      // Simulated annealing parameters
      let temperature = 1000;
      const coolingRate = 0.95;
      const iterations = 100;
      
      for (let i = 0; i < iterations; i++) {
        // Create a neighbor by applying a random move
        let newPath = [...currentPath];
        
        // Choose a random move type: swap, insert, or reverse
        const moveType = Math.floor(Math.random() * 3);
        
        if (moveType === 0 && newPath.length >= 2) {
          // Swap move
          const i = Math.floor(Math.random() * newPath.length);
          const j = Math.floor(Math.random() * newPath.length);
          [newPath[i], newPath[j]] = [newPath[j], newPath[i]];
        } else if (moveType === 1 && newPath.length >= 2) {
          // Insert move
          const i = Math.floor(Math.random() * newPath.length);
          const j = Math.floor(Math.random() * newPath.length);
          
          if (i !== j) {
            const value = newPath[i];
            newPath.splice(i, 1);
            newPath.splice(j, 0, value);
          }
        } else if (moveType === 2 && newPath.length >= 2) {
          // Reverse move
          const i = Math.floor(Math.random() * newPath.length);
          const j = Math.floor(Math.random() * newPath.length);
          
          const start = Math.min(i, j);
          const end = Math.max(i, j);
          
          const segment = newPath.slice(start, end + 1).reverse();
          newPath.splice(start, segment.length, ...segment);
        }
        
        // Evaluate new path
        let newDistance = 0;
        let prevLocation = 0; // Start at depot
        
        for (const locId of newPath) {
          const loc = locations.find(l => l.id === locId);
          newDistance += distance(
            locations[prevLocation], 
            loc
          );
          prevLocation = locId;
        }
        
        // Return to depot
        newDistance += distance(
          locations[prevLocation], 
          locations[0]
        );
        
        // Decide whether to accept new solution
        let accept = false;
        
        if (newDistance < currentDistance) {
          // Always accept better solutions
          accept = true;
        } else {
          // Accept worse solutions with probability based on temperature
          const acceptanceProbability = Math.exp((currentDistance - newDistance) / temperature);
          if (Math.random() < acceptanceProbability) {
            accept = true;
          }
        }
        
        if (accept) {
          currentPath = newPath;
          currentDistance = newDistance;
          
          if (newDistance < bestDistance) {
            bestPath = [...newPath];
            bestDistance = newDistance;
          }
        }
        
        // Cool down
        temperature *= coolingRate;
      }
      
      // Update route
      setCurrentRoute({
        path: bestPath,
        distance: Math.round(bestDistance),
        feasible: true
      });
      
      setIsOptimizing(false);
    }, 1500);
  };
  
  // ===== NETWORK TRAFFIC ANALYSIS =====
  
  // Initialize network traffic
  useEffect(() => {
    if (activeTab === 'networkAnalysis') {
      generateTraffic();
    }
  }, [activeTab]);
  
  // Generate sample network traffic
  const generateTraffic = () => {
    setIsAnalyzing(true);
    
    // Generate IPs
    const normalIPs = Array(5).fill().map(() => generateRandomIP());
    const serverIPs = Array(2).fill().map(() => generateRandomIP());
    const attackerIPs = Array(2).fill().map(() => generateRandomIP());
    
    // Generate traffic
    const packets = [];
    const numPackets = 50;
    
    // Normal traffic
    for (let i = 0; i < numPackets * 0.7; i++) {
      const srcIP = normalIPs[Math.floor(Math.random() * normalIPs.length)];
      const dstIP = serverIPs[Math.floor(Math.random() * serverIPs.length)];
      
      packets.push({
        id: i,
        sourceIP: srcIP,
        sourcePort: 1024 + Math.floor(Math.random() * 64000),
        destIP: dstIP,
        destPort: Math.random() < 0.8 ? 80 : 443, // HTTP or HTTPS
        protocol: Math.random() < 0.8 ? 'TCP' : 'UDP',
        size: Math.floor(Math.random() * 1000) + 64,
        isSuspicious: false
      });
    }
    
    // Attack traffic - port scan
    for (let i = 0; i < numPackets * 0.15; i++) {
      packets.push({
        id: numPackets * 0.7 + i,
        sourceIP: attackerIPs[0],
        sourcePort: 1024 + Math.floor(Math.random() * 64000),
        destIP: serverIPs[0],
        destPort: i + 1, // Sequential port scan
        protocol: 'TCP',
        size: 64,
        isSuspicious: true
      });
    }
    
    // Attack traffic - DDoS
    for (let i = 0; i < numPackets * 0.15; i++) {
      packets.push({
        id: numPackets * 0.85 + i,
        sourceIP: attackerIPs[1],
        sourcePort: 1024 + Math.floor(Math.random() * 64000),
        destIP: serverIPs[1],
        destPort: 80,
        protocol: 'UDP',
        size: 1024,
        isSuspicious: true
      });
    }
    
    // Shuffle packets
    packets.sort(() => Math.random() - 0.5);
    
    setPacketFlow(packets);
    setSuspiciousIPs(attackerIPs);
    
    // Calculate statistics
    const totalBytes = packets.reduce((sum, p) => sum + p.size, 0);
    const tcpPackets = packets.filter(p => p.protocol === 'TCP').length;
    const udpPackets = packets.filter(p => p.protocol === 'UDP').length;
    
    setNetworkStats({
      totalPackets: packets.length,
      totalBytes,
      tcpPackets,
      udpPackets,
      avgPacketSize: Math.round(totalBytes / packets.length)
    });
    
    setIsAnalyzing(false);
  };
  
  // Generate a random IP
  const generateRandomIP = () => {
    return `${Math.floor(Math.random() * 255)}.${Math.floor(Math.random() * 255)}.${Math.floor(Math.random() * 255)}.${Math.floor(Math.random() * 255)}`;
  };
  
  // Analyze traffic for suspicious patterns
  const analyzeTraffic = () => {
    setIsAnalyzing(true);
    
    setTimeout(() => {
      // Count packets per IP
      const ipCounts = {};
      const connCounts = {};
      
      packetFlow.forEach(packet => {
        // Count source IPs
        ipCounts[packet.sourceIP] = (ipCounts[packet.sourceIP] || 0) + 1;
        
        // Count connections
        const conn = `${packet.sourceIP}:${packet.sourcePort}->${packet.destIP}:${packet.destPort}`;
        connCounts[conn] = (connCounts[conn] || 0) + 1;
      });
      
      // Mark suspicious packets
      const updatedPackets = packetFlow.map(packet => {
        // Check for port scanning (many connections from same source)
        const isSuspicious = 
          ipCounts[packet.sourceIP] > packetFlow.length * 0.1 || // High packet count
          suspiciousIPs.includes(packet.sourceIP);
        
        return {
          ...packet,
          isSuspicious
        };
      });
      
      setPacketFlow(updatedPackets);
      setIsAnalyzing(false);
    }, 1000);
  };
  
  // ===== DISTRIBUTED CACHE =====
  
  // Initialize cache
  useEffect(() => {
    if (activeTab === 'distributedCache') {
      initializeCache();
    }
  }, [activeTab]);
  
  // Initialize the distributed cache
  const initializeCache = () => {
    const numNodes = 4;
    const nodeCapacity = 5; // Maximum items per node
    
    // Create nodes
    const nodes = Array(numNodes).fill().map((_, i) => ({
      id: i,
      items: [],
      capacity: nodeCapacity,
      hits: 0,
      misses: 0
    }));
    
    setCacheNodes(nodes);
    setCacheKeys([]);
    setRequestStats({ hits: 0, misses: 0 });
  };
  
  // Hash function (simplified consistent hashing)
  const getNodeForKey = (key) => {
    // Simple hash function
    let hash = 0;
    for (let i = 0; i < key.length; i++) {
      hash = (hash * 31 + key.charCodeAt(i)) % 1000;
    }
    return hash % cacheNodes.length;
  };
  
  // Put a key-value pair in the cache
  const putInCache = (key, value) => {
    if (!key || !value) return;
    
    setIsProcessing(true);
    
    setTimeout(() => {
      const nodeId = getNodeForKey(key);
      
      setCacheNodes(prev => {
        const newNodes = [...prev];
        const node = {...newNodes[nodeId]};
        
        // Check if key already exists
        const existingIndex = node.items.findIndex(item => item.key === key);
        
        if (existingIndex >= 0) {
          // Update existing item
          const updatedItems = [...node.items];
          updatedItems[existingIndex] = {
            key,
            value,
            frequency: updatedItems[existingIndex].frequency + 1,
            lastAccess: Date.now()
          };
          node.items = updatedItems;
        } else {
          // Add new item
          const newItem = {
            key,
            value,
            frequency: 1,
            lastAccess: Date.now()
          };
          
          // Check if eviction is needed
          if (node.items.length >= node.capacity) {
            // Evict based on policy
            if (cachePolicy === 'LRU') {
              // LRU: Evict least recently used
              node.items.sort((a, b) => a.lastAccess - b.lastAccess);
              node.items.shift(); // Remove oldest
              node.items.push(newItem);
            } else {
              // LFU: Evict least frequently used
              node.items.sort((a, b) => a.frequency - b.frequency);
              node.items.shift(); // Remove least frequent
              node.items.push(newItem);
            }
          } else {
            node.items.push(newItem);
          }
        }
        
        newNodes[nodeId] = node;
        return newNodes;
      });
      
      // Update keys list
      setCacheKeys(prev => {
        if (!prev.includes(key)) {
          return [...prev, key];
        }
        return prev;
      });
      
      setIsProcessing(false);
    }, 300);
  };
  
  // Get a value from the cache
  const getFromCache = (key) => {
    if (!key) return;
    
    setIsProcessing(true);
    
    setTimeout(() => {
      const nodeId = getNodeForKey(key);
      
      setCacheNodes(prev => {
        const newNodes = [...prev];
        const node = {...newNodes[nodeId]};
        
        // Look for the key
        const itemIndex = node.items.findIndex(item => item.key === key);
        
        if (itemIndex >= 0) {
          // Cache hit
          const updatedItems = [...node.items];
          updatedItems[itemIndex] = {
            ...updatedItems[itemIndex],
            frequency: updatedItems[itemIndex].frequency + 1,
            lastAccess: Date.now()
          };
          node.items = updatedItems;
          node.hits++;
          
          setRequestStats(prev => ({
            ...prev,
            hits: prev.hits + 1
          }));
        } else {
          // Cache miss
          node.misses++;
          
          setRequestStats(prev => ({
            ...prev,
            misses: prev.misses + 1
          }));
        }
        
        newNodes[nodeId] = node;
        return newNodes;
      });
      
      setIsProcessing(false);
    }, 300);
  };
  
  // Add new cache node
  const addCacheNode = () => {
    setCacheNodes(prev => {
      const newNodes = [...prev];
      newNodes.push({
        id: newNodes.length,
        items: [],
        capacity: newNodes[0].capacity,
        hits: 0,
        misses: 0
      });
      return newNodes;
    });
  };
  
  // Generate random cache operations
  const runCacheWorkload = () => {
    setIsProcessing(true);
    
    const operations = 20;
    let counter = 0;
    
    const runOperation = () => {
      if (counter >= operations) {
        setIsProcessing(false);
        return;
      }
      
      // Decide operation type
      const isPut = Math.random() < 0.7;
      
      if (isPut) {
        // Put operation
        const key = `key_${Math.floor(Math.random() * 20)}`;
        const value = `value_${Math.floor(Math.random() * 1000)}`;
        putInCache(key, value);
      } else {
        // Get operation
        const keyIndex = Math.floor(Math.random() * 20);
        const key = `key_${keyIndex}`;
        getFromCache(key);
      }
      
      counter++;
      setTimeout(runOperation, 300);
    };
    
    runOperation();
  };

  return (
    <div className="flex flex-col w-full min-h-screen bg-gray-900 text-white">
      <div className="bg-gray-800 p-4 shadow-lg">
        <h1 className="text-3xl font-bold text-center text-cyan-400">Algorithm Integration Visualizer</h1>
        <p className="text-center text-gray-300 mt-2">Day 42: Weekly Challenge & Advanced Problems</p>
      </div>
      
      <div className="flex p-4">
        <div className="flex flex-col space-y-2 pr-4">
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'fileIndexing' ? 'bg-cyan-600' : 'bg-gray-700'} hover:bg-cyan-500`}
            onClick={() => setActiveTab('fileIndexing')}
          >
            File Indexing
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'routeOptimization' ? 'bg-pink-600' : 'bg-gray-700'} hover:bg-pink-500`}
            onClick={() => setActiveTab('routeOptimization')}
          >
            Route Optimization
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'networkAnalysis' ? 'bg-yellow-600' : 'bg-gray-700'} hover:bg-yellow-500`}
            onClick={() => setActiveTab('networkAnalysis')}
          >
            Network Analysis
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'distributedCache' ? 'bg-green-600' : 'bg-gray-700'} hover:bg-green-500`}
            onClick={() => setActiveTab('distributedCache')}
          >
            Distributed Cache
          </button>
        </div>
        
        <div className="flex-1 bg-gray-800 rounded-lg p-4">
          {/* FILE INDEXING SYSTEM */}
          {activeTab === 'fileIndexing' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-cyan-400">File Indexing System</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <div className="mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Search Documents</h3>
                    <div className="flex">
                      <input
                        type="text"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Enter search query..."
                        className="flex-1 p-2 bg-gray-700 text-white rounded-l"
                      />
                      <button
                        onClick={() => searchDocuments(searchQuery)}
                        disabled={isSearching}
                        className="px-4 py-2 bg-cyan-600 text-white rounded-r hover:bg-cyan-500 disabled:opacity-50"
                      >
                        {isSearching ? 'Searching...' : 'Search'}
                      </button>
                    </div>
                  </div>
                  
                  <div className="mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Search Results</h3>
                    {searchResults.length === 0 ? (
                      <div className="bg-gray-700 p-4 rounded">
                        {searchQuery ? 'No results found' : 'Enter a search query'}
                      </div>
                    ) : (
                      <div className="space-y-4">
                        {searchResults.map(result => (
                          <div key={result.id} className="bg-gray-700 p-4 rounded">
                            <h4 className="text-lg font-semibold text-cyan-400">{result.title}</h4>
                            <p className="text-sm text-gray-300 mt-1">{result.snippet}</p>
                            <div className="text-xs text-yellow-400 mt-2">Score: {result.score.toFixed(4)}</div>
                          </div>
                        ))}
                      </div>
                    )}
                  </div>
                </div>
                
                <div>
                  <div className="mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Document Collection</h3>
                    <div className="bg-gray-700 p-4 rounded max-h-60 overflow-y-auto">
                      {documents.map(doc => (
                        <div key={doc.id} className="mb-3 pb-3 border-b border-gray-600">
                          <h4 className="font-medium text-white">{doc.title}</h4>
                          <p className="text-sm text-gray-300 mt-1">{doc.content.length > 100 ? doc.content.substring(0, 100) + '...' : doc.content}</p>
                        </div>
                      ))}
                    </div>
                  </div>
                  
                  <div>
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Index Statistics</h3>
                    <div className="bg-gray-700 p-4 rounded">
                      <div className="grid grid-cols-2 gap-4">
                        <div>
                          <p className="text-sm text-gray-400">Total Documents</p>
                          <p className="text-xl font-bold text-pink-400">{indexStats.totalDocuments}</p>
                        </div>
                        <div>
                          <p className="text-sm text-gray-400">Total Terms</p>
                          <p className="text-xl font-bold text-cyan-400">{indexStats.totalTerms}</p>
                        </div>
                        <div>
                          <p className="text-sm text-gray-400">Unique Terms</p>
                          <p className="text-xl font-bold text-yellow-400">{indexStats.uniqueTerms}</p>
                        </div>
                        <div>
                          <p className="text-sm text-gray-400">Avg. Document Length</p>
                          <p className="text-xl font-bold text-green-400">{indexStats.avgDocLength} chars</p>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Integrated Algorithms</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Inverted Index</h4>
                      <p className="text-sm">Maps terms to document IDs and positions for efficient search.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">TF-IDF Ranking</h4>
                      <p className="text-sm">Ranks search results by term frequency and inverse document frequency.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Trie Data Structure</h4>
                      <p className="text-sm">Enables efficient prefix searches and auto-completion.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
          
          {/* ROUTE OPTIMIZATION */}
          {activeTab === 'routeOptimization' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-pink-400">Route Optimization with Constraints</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                <div className="md:col-span-2">
                  <div className="bg-gray-700 p-2 rounded">
                    <canvas 
                      ref={canvasRef} 
                      width="500" 
                      height="500" 
                      className="w-full h-full bg-gray-900"
                    />
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Route Details</h3>
                    <div className="space-y-2">
                      <div>
                        <span className="text-gray-400">Locations:</span>
                        <span className="ml-2 font-semibold">{routeProblem.locations.length}</span>
                      </div>
                      <div>
                        <span className="text-gray-400">Vehicle Capacity:</span>
                        <span className="ml-2 font-semibold">{routeProblem.vehicleCapacity} units</span>
                      </div>
                      <div>
                        <span className="text-gray-400">Current Route:</span>
                        <span className="ml-2 font-semibold">{currentRoute.path.length} stops</span>
                      </div>
                      <div>
                        <span className="text-gray-400">Total Distance:</span>
                        <span className="ml-2 font-semibold">{currentRoute.distance} units</span>
                      </div>
                      <div>
                        <span className="text-gray-400">Feasible:</span>
                        <span className={`ml-2 font-semibold ${currentRoute.feasible ? 'text-green-400' : 'text-red-400'}`}>
                          {currentRoute.feasible ? 'Yes' : 'No'}
                        </span>
                      </div>
                    </div>
                  </div>
                  
                  <div className="flex flex-col space-y-2">
                    <button
                      onClick={optimizeRoute}
                      disabled={isOptimizing}
                      className="px-4 py-2 bg-pink-600 text-white rounded hover:bg-pink-500 disabled:opacity-50"
                    >
                      {isOptimizing ? 'Optimizing...' : 'Optimize Route'}
                    </button>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Problem Constraints</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Time Windows</h4>
                      <p className="text-sm">Each location has earliest and latest arrival times that must be respected.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Vehicle Capacity</h4>
                      <p className="text-sm">The vehicle has a limited capacity for packages.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Distance Minimization</h4>
                      <p className="text-sm">The total travel distance should be minimized.</p>
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-4">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Integrated Algorithms</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-green-600 p-3 rounded">
                      <h4 className="font-semibold text-green-400">Simulated Annealing</h4>
                      <p className="text-sm">Metaheuristic for finding near-optimal solutions in large search spaces.</p>
                    </div>
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Constraint Satisfaction</h4>
                      <p className="text-sm">Ensures routes meet time windows and capacity constraints.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Nearest Neighbor</h4>
                      <p className="text-sm">Greedy algorithm for initial solution construction.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
          
          {/* NETWORK TRAFFIC ANALYSIS */}
          {activeTab === 'networkAnalysis' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-yellow-400">Real-Time Network Traffic Analysis</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Traffic Flow</h3>
                    <div className="max-h-80 overflow-y-auto">
                      {packetFlow.map(packet => (
                        <div 
                          key={packet.id} 
                          className={`mb-1 p-2 text-xs rounded ${packet.isSuspicious ? 'bg-red-900 border border-pink-500' : 'bg-gray-800'}`}
                        >
                          <div className="flex justify-between">
                            <span className={packet.isSuspicious ? 'text-pink-400' : 'text-gray-300'}>
                              {packet.sourceIP}:{packet.sourcePort}
                            </span>
                            <span className="text-cyan-400">→</span>
                            <span className={packet.isSuspicious ? 'text-pink-400' : 'text-gray-300'}>
                              {packet.destIP}:{packet.destPort}
                            </span>
                          </div>
                          <div className="flex justify-between mt-1">
                            <span>{packet.protocol}</span>
                            <span>{packet.size} bytes</span>
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                  
                  <div className="flex flex-col space-y-2">
                    <button
                      onClick={analyzeTraffic}
                      disabled={isAnalyzing}
                      className="px-4 py-2 bg-yellow-600 text-white rounded hover:bg-yellow-500 disabled:opacity-50"
                    >
                      {isAnalyzing ? 'Analyzing...' : 'Analyze Traffic'}
                    </button>
                    <button
                      onClick={generateTraffic}
                      disabled={isAnalyzing}
                      className="px-4 py-2 bg-gray-600 text-white rounded hover:bg-gray-500 disabled:opacity-50"
                    >
                      Generate New Traffic
                    </button>
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Traffic Statistics</h3>
                    <div className="grid grid-cols-2 gap-4">
                      <div>
                        <p className="text-sm text-gray-400">Total Packets</p>
                        <p className="text-xl font-bold text-yellow-400">{networkStats.totalPackets}</p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">Total Bytes</p>
                        <p className="text-xl font-bold text-cyan-400">{networkStats.totalBytes}</p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">TCP Packets</p>
                        <p className="text-xl font-bold text-green-400">{networkStats.tcpPackets}</p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">UDP Packets</p>
                        <p className="text-xl font-bold text-pink-400">{networkStats.udpPackets}</p>
                      </div>
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Suspicious IPs</h3>
                    <div className="space-y-2">
                      {suspiciousIPs.length === 0 ? (
                        <p className="text-gray-400">No suspicious IPs detected</p>
                      ) : (
                        suspiciousIPs.map((ip, index) => (
                          <div key={index} className="bg-red-900 border border-pink-500 p-2 rounded flex justify-between">
                            <span className="text-pink-400">{ip}</span>
                            <span className="text-yellow-400">Potential Attacker</span>
                          </div>
                        ))
                      )}
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Integrated Algorithms</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Count-Min Sketch</h4>
                      <p className="text-sm">Probabilistic data structure for frequency estimation in streaming data.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Bloom Filter</h4>
                      <p className="text-sm">Space-efficient probabilistic structure for set membership testing.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Sliding Window Analysis</h4>
                      <p className="text-sm">Maintains recent history for time-based pattern detection.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
          
          {/* DISTRIBUTED CACHE */}
          {activeTab === 'distributedCache' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-green-400">Distributed Cache with Smart Eviction</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                <div className="md:col-span-2">
                  <div className="grid grid-cols-2 gap-4 mb-4">
                    {cacheNodes.map(node => (
                      <div key={node.id} className="bg-gray-700 p-4 rounded">
                        <h3 className="text-lg font-semibold text-green-400 mb-2">Node {node.id}</h3>
                        <div className="text-xs mb-2">
                          <span className="text-gray-400">Items: {node.items.length}/{node.capacity}</span>
                          <span className="ml-2 text-green-400">Hits: {node.hits}</span>
                          <span className="ml-2 text-pink-400">Misses: {node.misses}</span>
                        </div>
                        <div className="max-h-32 overflow-y-auto">
                          {node.items.length === 0 ? (
                            <div className="text-gray-500 text-center p-2">Empty</div>
                          ) : (
                            node.items.map((item, index) => (
                              <div key={index} className="bg-gray-800 p-2 rounded mb-1 text-xs">
                                <div className="flex justify-between">
                                  <span className="text-cyan-400">{item.key}</span>
                                  <span className="text-gray-400">{item.value}</span>
                                </div>
                                <div className="flex justify-between mt-1">
                                  <span className="text-yellow-400">Freq: {item.frequency}</span>
                                  <span className="text-pink-400">
                                    {Math.round((Date.now() - item.lastAccess) / 1000)}s ago
                                  </span>
                                </div>
                              </div>
                            ))
                          )}
                        </div>
                      </div>
                    ))}
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Cache Operations</h3>
                    <div className="grid grid-cols-2 gap-4 mb-4">
                      <div>
                        <label className="block mb-1 text-sm">Key</label>
                        <input
                          type="text"
                          placeholder="Enter key..."
                          className="w-full p-2 bg-gray-800 text-white rounded"
                          id="cacheKey"
                        />
                      </div>
                      <div>
                        <label className="block mb-1 text-sm">Value</label>
                        <input
                          type="text"
                          placeholder="Enter value..."
                          className="w-full p-2 bg-gray-800 text-white rounded"
                          id="cacheValue"
                        />
                      </div>
                    </div>
                    
                    <div className="flex space-x-2">
                      <button
                        onClick={() => {
                          const keyEl = document.getElementById("cacheKey");
                          const valueEl = document.getElementById("cacheValue");
                          if (keyEl && valueEl) {
                            putInCache(keyEl.value, valueEl.value);
                            keyEl.value = '';
                            valueEl.value = '';
                          }
                        }}
                        disabled={isProcessing}
                        className="px-4 py-2 bg-green-600 text-white rounded hover:bg-green-500 disabled:opacity-50 flex-1"
                      >
                        Put
                      </button>
                      <button
                        onClick={() => {
                          const keyEl = document.getElementById("cacheKey");
                          if (keyEl) {
                            getFromCache(keyEl.value);
                            keyEl.value = '';
                          }
                        }}
                        disabled={isProcessing}
                        className="px-4 py-2 bg-cyan-600 text-white rounded hover:bg-cyan-500 disabled:opacity-50 flex-1"
                      >
                        Get
                      </button>
                      <button
                        onClick={runCacheWorkload}
                        disabled={isProcessing}
                        className="px-4 py-2 bg-yellow-600 text-white rounded hover:bg-yellow-500 disabled:opacity-50 flex-1"
                      >
                        Run Workload
                      </button>
                    </div>
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Cache Configuration</h3>
                    <div className="space-y-4">
                      <div>
                        <label className="block mb-1 text-sm">Eviction Policy</label>
                        <select
                          value={cachePolicy}
                          onChange={(e) => setCachePolicy(e.target.value)}
                          className="w-full p-2 bg-gray-800 text-white rounded"
                        >
                          <option value="LRU">LRU (Least Recently Used)</option>
                          <option value="LFU">LFU (Least Frequently Used)</option>
                        </select>
                      </div>
                      
                      <div>
                        <label className="block mb-1 text-sm">Nodes: {cacheNodes.length}</label>
                        <button
                          onClick={addCacheNode}
                          className="w-full px-4 py-2 bg-gray-600 text-white rounded hover:bg-gray-500"
                        >
                          Add Node
                        </button>
                      </div>
                      
                      <button
                        onClick={initializeCache}
                        className="w-full px-4 py-2 bg-pink-600 text-white rounded hover:bg-pink-500"
                      >
                        Reset Cache
                      </button>
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Cache Statistics</h3>
                    <div className="grid grid-cols-2 gap-4">
                      <div>
                        <p className="text-sm text-gray-400">Total Hits</p>
                        <p className="text-xl font-bold text-green-400">{requestStats.hits}</p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">Total Misses</p>
                        <p className="text-xl font-bold text-pink-400">{requestStats.misses}</p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">Hit Rate</p>
                        <p className="text-xl font-bold text-cyan-400">
                          {requestStats.hits + requestStats.misses > 0
                            ? Math.round((requestStats.hits / (requestStats.hits + requestStats.misses)) * 100)
                            : 0}%
                        </p>
                      </div>
                      <div>
                        <p className="text-sm text-gray-400">Unique Keys</p>
                        <p className="text-xl font-bold text-yellow-400">{cacheKeys.length}</p>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Integrated Algorithms</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Consistent Hashing</h4>
                      <p className="text-sm">Distributes keys across nodes with minimal redistribution when topology changes.</p>
                    </div>
                    <div className="border border-green-600 p-3 rounded">
                      <h4 className="font-semibold text-green-400">LRU/LFU Eviction</h4>
                      <p className="text-sm">Smart cache eviction policies based on recency or frequency of access.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Distributed Hash Table</h4>
                      <p className="text-sm">Scales horizontally across multiple nodes with load balancing.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
        </div>
      </div>
      
      <div className="p-4 bg-gray-800 mt-4">
        <h2 className="text-xl font-bold text-center text-white mb-2">Time & Space Complexity</h2>
        <div className="overflow-x-auto">
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-gray-700">
                <th className="border border-gray-600 p-3 text-left text-cyan-400">Challenge</th>
                <th className="border border-gray-600 p-3 text-left text-pink-400">Algorithm</th>
                <th className="border border-gray-600 p-3 text-left text-yellow-400">Time Complexity</th>
                <th className="border border-gray-600 p-3 text-left text-green-400">Space Complexity</th>
              </tr>
            </thead>
            <tbody>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">File Indexing</td>
                <td className="border border-gray-600 p-3">Inverted Index Construction</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(n)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(n)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">TF-IDF Search</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(q + r log r)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(r)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Phrase Search</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(k × m)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(k)</td>
              </tr>
              
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Route Optimization</td>
                <td className="border border-gray-600 p-3">Initial Solution (Greedy)</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(n²)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(n)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Simulated Annealing</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(I × n²)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(n)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Constraint Checking</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(n)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(1)</td>
              </tr>
              
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Network Analysis</td>
                <td className="border border-gray-600 p-3">Count-Min Sketch</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(d) per update</td>
                <td className="border border-gray-600 p-3 text-green-300">O(w × d)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Bloom Filter</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(k) per operation</td>
                <td className="border border-gray-600 p-3 text-green-300">O(m)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Sliding Window Analysis</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1) amortized</td>
                <td className="border border-gray-600 p-3 text-green-300">O(w)</td>
              </tr>
              
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Distributed Cache</td>
                <td className="border border-gray-600 p-3">Consistent Hashing</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(log n) lookup</td>
                <td className="border border-gray-600 p-3 text-green-300">O(n log n)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">LRU Cache</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1) per operation</td>
                <td className="border border-gray-600 p-3 text-green-300">O(k)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">LFU Cache</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1) per operation</td>
                <td className="border border-gray-600 p-3 text-green-300">O(k)</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div className="text-xs text-gray-400 mt-2">
          <p>n = input size, q = query size, r = result size, k = key count, I = iterations, w = window size, d = sketch depth, m = bloom filter size</p>
        </div>
      </div>
    </div>
  );
};

export default AlgorithmIntegrationVisualizer;
