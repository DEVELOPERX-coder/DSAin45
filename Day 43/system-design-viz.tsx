import React, { useState, useEffect, useRef } from 'react';

const SystemDesignVisualizer = () => {
  const [activeTab, setActiveTab] = useState('consistentHashing');
  const [nodes, setNodes] = useState([]);
  const [keys, setKeys] = useState([]);
  const [selectedKey, setSelectedKey] = useState('');
  const [newKey, setNewKey] = useState('');
  const [newNode, setNewNode] = useState('');
  const canvasRef = useRef(null);
  
  // Rate limiter state
  const [rateLimiter, setRateLimiter] = useState({
    type: 'token-bucket',
    capacity: 100,
    refillRate: 10,
    currentTokens: 100,
    requests: []
  });
  
  // URL shortener state
  const [urlShortener, setUrlShortener] = useState({
    urls: [],
    longUrl: '',
    analytics: {}
  });
  
  // Sharding state
  const [shardingStrategy, setShardingStrategy] = useState('hash');
  const [shards, setShards] = useState([]);
  const [shardedData, setShardedData] = useState({});

  // Initialize data
  useEffect(() => {
    if (activeTab === 'consistentHashing') {
      const initialNodes = ['Node1', 'Node2', 'Node3'];
      const initialKeys = ['key1', 'key2', 'key3', 'key4', 'key5'];
      setNodes(initialNodes);
      setKeys(initialKeys);
      drawConsistentHash(initialNodes, initialKeys);
    } else if (activeTab === 'sharding') {
      const initialShards = [
        { id: 0, name: 'Shard-A', range: 'A-H', load: 0 },
        { id: 1, name: 'Shard-B', range: 'I-P', load: 0 },
        { id: 2, name: 'Shard-C', range: 'Q-Z', load: 0 }
      ];
      setShards(initialShards);
    }
  }, [activeTab]);

  // Simulate hash function
  const simpleHash = (str) => {
    let hash = 0;
    for (let i = 0; i < str.length; i++) {
      const char = str.charCodeAt(i);
      hash = ((hash << 5) - hash) + char;
      hash = hash & hash;
    }
    return Math.abs(hash);
  };

  // Get node for key using consistent hashing
  const getNodeForKey = (key) => {
    if (nodes.length === 0) return null;
    
    const keyHash = simpleHash(key);
    const nodeHashes = nodes.map(node => ({
      node,
      hash: simpleHash(node + '#virtual')
    })).sort((a, b) => a.hash - b.hash);
    
    // Find first node with hash greater than or equal to key hash
    for (const { node } of nodeHashes) {
      if (simpleHash(node + '#virtual') >= keyHash) {
        return node;
      }
    }
    
    // Wrap around to first node
    return nodeHashes[0].node;
  };

  // Draw consistent hash ring
  const drawConsistentHash = (currentNodes, currentKeys) => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const centerX = canvas.width / 2;
    const centerY = canvas.height / 2;
    const radius = Math.min(centerX, centerY) - 50;
    
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw ring
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
    ctx.strokeStyle = '#00FFFF';
    ctx.lineWidth = 2;
    ctx.stroke();
    
    // Draw nodes
    const nodePositions = [];
    currentNodes.forEach((node, i) => {
      const angle = (simpleHash(node + '#virtual') % 360) * (Math.PI / 180);
      const x = centerX + radius * Math.cos(angle);
      const y = centerY + radius * Math.sin(angle);
      
      nodePositions.push({ node, x, y });
      
      // Draw node
      ctx.beginPath();
      ctx.arc(x, y, 12, 0, Math.PI * 2);
      ctx.fillStyle = '#FF00FF';
      ctx.fill();
      ctx.strokeStyle = '#FFFFFF';
      ctx.stroke();
      
      // Draw node label
      ctx.fillStyle = '#FFFFFF';
      ctx.font = '12px Arial';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(node, x, y);
    });
    
    // Draw keys
    currentKeys.forEach((key, i) => {
      const node = getNodeForKey(key);
      const angle = (simpleHash(key) % 360) * (Math.PI / 180);
      const x = centerX + (radius - 30) * Math.cos(angle);
      const y = centerY + (radius - 30) * Math.sin(angle);
      
      // Draw key
      ctx.beginPath();
      ctx.arc(x, y, 6, 0, Math.PI * 2);
      ctx.fillStyle = key === selectedKey ? '#FFFF00' : '#00FF00';
      ctx.fill();
      
      // Draw connection to node
      const nodePos = nodePositions.find(n => n.node === node);
      if (nodePos) {
        ctx.beginPath();
        ctx.moveTo(x, y);
        ctx.lineTo(nodePos.x, nodePos.y);
        ctx.strokeStyle = key === selectedKey ? '#FFFF00' : '#666666';
        ctx.lineWidth = 1;
        ctx.stroke();
      }
      
      // Draw key label
      ctx.fillStyle = '#FFFFFF';
      ctx.font = '10px Arial';
      ctx.textAlign = 'center';
      ctx.fillText(key, x - 20, y);
    });
  };

  // Add new node
  const addNode = () => {
    if (newNode.trim() && !nodes.includes(newNode)) {
      const updatedNodes = [...nodes, newNode];
      setNodes(updatedNodes);
      drawConsistentHash(updatedNodes, keys);
      setNewNode('');
    }
  };

  // Remove node
  const removeNode = (nodeToRemove) => {
    const updatedNodes = nodes.filter(node => node !== nodeToRemove);
    setNodes(updatedNodes);
    drawConsistentHash(updatedNodes, keys);
  };

  // Add new key
  const addKey = () => {
    if (newKey.trim() && !keys.includes(newKey)) {
      const updatedKeys = [...keys, newKey];
      setKeys(updatedKeys);
      drawConsistentHash(nodes, updatedKeys);
      setNewKey('');
    }
  };

  // Rate limiter simulation
  const makeRequest = () => {
    const now = Date.now();
    setRateLimiter(prev => {
      const newState = { ...prev };
      
      if (prev.type === 'token-bucket') {
        // Refill tokens
        const elapsed = now - (prev.lastRefill || now);
        const tokensToAdd = Math.floor((elapsed / 1000) * prev.refillRate);
        newState.currentTokens = Math.min(prev.capacity, prev.currentTokens + tokensToAdd);
        newState.lastRefill = now;
        
        // Try to consume token
        if (newState.currentTokens > 0) {
          newState.currentTokens--;
          newState.requests = [
            ...prev.requests,
            { time: now, accepted: true }
          ].slice(-100); // Keep last 100 requests
        } else {
          newState.requests = [
            ...prev.requests,
            { time: now, accepted: false }
          ].slice(-100);
        }
      }
      
      return newState;
    });
  };

  // URL shortener operations
  const createShortUrl = () => {
    if (!urlShortener.longUrl.trim()) return;
    
    const shortCode = Math.random().toString(36).substring(2, 8);
    
    setUrlShortener(prev => ({
      ...prev,
      urls: [
        ...prev.urls,
        {
          longUrl: prev.longUrl,
          shortCode,
          createdAt: new Date(),
          clicks: 0
        }
      ],
      analytics: {
        ...prev.analytics,
        [shortCode]: { clicks: 0, createdAt: new Date() }
      },
      longUrl: ''
    }));
  };

  // Redirect short URL
  const redirectShortUrl = (shortCode) => {
    setUrlShortener(prev => {
      const updatedAnalytics = { ...prev.analytics };
      if (updatedAnalytics[shortCode]) {
        updatedAnalytics[shortCode].clicks++;
      }
      
      const updatedUrls = prev.urls.map(url => 
        url.shortCode === shortCode ? { ...url, clicks: url.clicks + 1 } : url
      );
      
      return {
        ...prev,
        urls: updatedUrls,
        analytics: updatedAnalytics
      };
    });
  };

  // Sharding operations
  const getShardForKey = (key) => {
    switch (shardingStrategy) {
      case 'hash':
        const hash = simpleHash(key);
        return hash % shards.length;
      case 'range':
        const firstChar = key.charAt(0).toUpperCase();
        if (firstChar >= 'A' && firstChar <= 'H') return 0;
        if (firstChar >= 'I' && firstChar <= 'P') return 1;
        return 2;
      case 'directory':
        // Simple directory-based sharding
        return Math.abs(simpleHash(key)) % shards.length;
      default:
        return 0;
    }
  };

  const addShardedData = () => {
    if (!newKey.trim()) return;
    
    const shardId = getShardForKey(newKey);
    
    setShardedData(prev => ({
      ...prev,
      [shardId]: [...(prev[shardId] || []), newKey]
    }));
    
    setShards(prev => prev.map(shard => 
      shard.id === shardId ? { ...shard, load: shard.load + 1 } : shard
    ));
    
    setNewKey('');
  };

  return (
    <div className="flex flex-col w-full min-h-screen bg-gray-900 text-white">
      <div className="bg-gray-800 p-4 shadow-lg">
        <h1 className="text-3xl font-bold text-center text-cyan-400">System Design Fundamentals</h1>
        <p className="text-center text-gray-300 mt-2">Day 43: Distributed Systems Concepts</p>
      </div>
      
      <div className="flex p-4">
        <div className="flex flex-col space-y-2 pr-4">
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'consistentHashing' ? 'bg-cyan-600' : 'bg-gray-700'} hover:bg-cyan-500`}
            onClick={() => setActiveTab('consistentHashing')}
          >
            Consistent Hashing
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'rateLimiting' ? 'bg-pink-600' : 'bg-gray-700'} hover:bg-pink-500`}
            onClick={() => setActiveTab('rateLimiting')}
          >
            Rate Limiting
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'urlShortener' ? 'bg-yellow-600' : 'bg-gray-700'} hover:bg-yellow-500`}
            onClick={() => setActiveTab('urlShortener')}
          >
            URL Shortener
          </button>
          <button 
            className={`px-4 py-2 rounded-lg ${activeTab === 'sharding' ? 'bg-green-600' : 'bg-gray-700'} hover:bg-green-500`}
            onClick={() => setActiveTab('sharding')}
          >
            Sharding
          </button>
        </div>
        
        <div className="flex-1 bg-gray-800 rounded-lg p-4">
          {/* CONSISTENT HASHING */}
          {activeTab === 'consistentHashing' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-cyan-400">Consistent Hashing</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                <div className="md:col-span-2">
                  <div className="bg-gray-700 p-2 rounded">
                    <canvas 
                      ref={canvasRef} 
                      width="600" 
                      height="600" 
                      className="w-full h-full bg-gray-900"
                    />
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Add Nodes</h3>
                    <div className="flex mb-2">
                      <input
                        type="text"
                        value={newNode}
                        onChange={(e) => setNewNode(e.target.value)}
                        placeholder="Node name"
                        className="flex-1 p-2 bg-gray-800 text-white rounded"
                      />
                      <button
                        onClick={addNode}
                        className="ml-2 px-4 py-2 bg-cyan-600 text-white rounded hover:bg-cyan-500"
                      >
                        Add
                      </button>
                    </div>
                    <div className="space-y-1">
                      {nodes.map(node => (
                        <div key={node} className="flex justify-between items-center bg-gray-800 p-2 rounded">
                          <span>{node}</span>
                          <button
                            onClick={() => removeNode(node)}
                            className="px-2 py-1 bg-red-600 text-white rounded text-sm hover:bg-red-500"
                          >
                            Remove
                          </button>
                        </div>
                      ))}
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Add Keys</h3>
                    <div className="flex mb-2">
                      <input
                        type="text"
                        value={newKey}
                        onChange={(e) => setNewKey(e.target.value)}
                        placeholder="Key name"
                        className="flex-1 p-2 bg-gray-800 text-white rounded"
                      />
                      <button
                        onClick={addKey}
                        className="ml-2 px-4 py-2 bg-green-600 text-white rounded hover:bg-green-500"
                      >
                        Add
                      </button>
                    </div>
                    <div className="space-y-1">
                      {keys.map(key => (
                        <div 
                          key={key} 
                          className={`p-2 rounded cursor-pointer ${selectedKey === key ? 'bg-yellow-700' : 'bg-gray-800'}`}
                          onClick={() => setSelectedKey(key === selectedKey ? '' : key)}
                        >
                          <div className="flex justify-between items-center">
                            <span className="text-green-400">{key}</span>
                            <span className="text-sm text-gray-400">→ {getNodeForKey(key)}</span>
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">How It Works</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <ul className="list-disc list-inside space-y-2">
                    <li>Nodes are placed on a consistent hash ring using their hash values</li>
                    <li>Keys are mapped to the first node in clockwise direction from their hash position</li>
                    <li>Adding/removing nodes minimally affects key distribution</li>
                    <li>Virtual nodes (replicas) ensure better load distribution</li>
                  </ul>
                </div>
              </div>
            </div>
          )}
          
          {/* RATE LIMITING */}
          {activeTab === 'rateLimiting' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-pink-400">Rate Limiting</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Configuration</h3>
                    <div className="space-y-4">
                      <div>
                        <label className="block mb-1">Algorithm</label>
                        <select
                          value={rateLimiter.type}
                          onChange={(e) => setRateLimiter(prev => ({ ...prev, type: e.target.value }))}
                          className="w-full p-2 bg-gray-800 text-white rounded"
                        >
                          <option value="token-bucket">Token Bucket</option>
                          <option value="leaky-bucket">Leaky Bucket</option>
                          <option value="sliding-window">Sliding Window</option>
                        </select>
                      </div>
                      
                      <div>
                        <label className="block mb-1">Capacity: {rateLimiter.capacity}</label>
                        <input
                          type="range"
                          min="10"
                          max="200"
                          value={rateLimiter.capacity}
                          onChange={(e) => setRateLimiter(prev => ({ ...prev, capacity: parseInt(e.target.value) }))}
                          className="w-full"
                        />
                      </div>
                      
                      <div>
                        <label className="block mb-1">Refill Rate: {rateLimiter.refillRate}/sec</label>
                        <input
                          type="range"
                          min="1"
                          max="50"
                          value={rateLimiter.refillRate}
                          onChange={(e) => setRateLimiter(prev => ({ ...prev, refillRate: parseInt(e.target.value) }))}
                          className="w-full"
                        />
                      </div>
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Current State</h3>
                    <div className="space-y-2">
                      <div>
                        <div className="flex justify-between text-sm text-gray-400">
                          <span>Available Tokens</span>
                          <span>{rateLimiter.currentTokens}/{rateLimiter.capacity}</span>
                        </div>
                        <div className="w-full bg-gray-600 rounded-full h-2">
                          <div 
                            className="bg-green-600 h-2 rounded-full" 
                            style={{ width: `${(rateLimiter.currentTokens / rateLimiter.capacity) * 100}%` }}
                          ></div>
                        </div>
                      </div>
                      
                      <button
                        onClick={makeRequest}
                        className="w-full px-4 py-2 bg-pink-600 text-white rounded hover:bg-pink-500"
                      >
                        Make Request
                      </button>
                    </div>
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded h-full">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Request History</h3>
                    <div className="max-h-96 overflow-y-auto">
                      {rateLimiter.requests.slice().reverse().map((req, index) => (
                        <div 
                          key={index} 
                          className={`p-2 mb-1 rounded text-sm ${req.accepted ? 'bg-green-900' : 'bg-red-900'}`}
                        >
                          <div className="flex justify-between">
                            <span>{new Date(req.time).toLocaleTimeString()}</span>
                            <span className={req.accepted ? 'text-green-400' : 'text-red-400'}>
                              {req.accepted ? 'Accepted' : 'Rejected'}
                            </span>
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Rate Limiting Algorithms</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Token Bucket</h4>
                      <p className="text-sm">Maintains a bucket of tokens that refill at a fixed rate. Allows bursts up to bucket capacity.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Leaky Bucket</h4>
                      <p className="text-sm">Processes requests at a fixed rate. Smooths out bursts but may introduce latency.</p>
                    </div>
                    <div className="border border-green-600 p-3 rounded">
                      <h4 className="font-semibold text-green-400">Sliding Window</h4>
                      <p className="text-sm">Tracks requests in a time window. Provides precise rate limiting but requires more memory.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
          
          {/* URL SHORTENER */}
          {activeTab === 'urlShortener' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-yellow-400">URL Shortener</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Create Short URL</h3>
                    <div className="flex">
                      <input
                        type="text"
                        value={urlShortener.longUrl}
                        onChange={(e) => setUrlShortener(prev => ({ ...prev, longUrl: e.target.value }))}
                        placeholder="Enter long URL"
                        className="flex-1 p-2 bg-gray-800 text-white rounded"
                      />
                      <button
                        onClick={createShortUrl}
                        className="ml-2 px-4 py-2 bg-yellow-600 text-white rounded hover:bg-yellow-500"
                      >
                        Shorten
                      </button>
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Short URLs</h3>
                    <div className="space-y-2">
                      {urlShortener.urls.map((url, index) => (
                        <div key={url.shortCode} className="bg-gray-800 p-3 rounded">
                          <div className="flex justify-between items-center">
                            <div>
                              <div className="text-yellow-400">short.ly/{url.shortCode}</div>
                              <div className="text-xs text-gray-400 truncate">{url.longUrl}</div>
                            </div>
                            <button
                              onClick={() => redirectShortUrl(url.shortCode)}
                              className="px-3 py-1 bg-green-600 text-white rounded text-sm hover:bg-green-500"
                            >
                              Visit
                            </button>
                          </div>
                          <div className="mt-2 text-xs text-gray-400">
                            Clicks: {url.clicks} | Created: {url.createdAt.toLocaleString()}
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded h-full">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Analytics</h3>
                    {Object.keys(urlShortener.analytics).length === 0 ? (
                      <p className="text-gray-400">No analytics data yet.</p>
                    ) : (
                      <div className="space-y-4">
                        <div className="grid grid-cols-2 gap-4">
                          <div className="bg-gray-800 p-3 rounded">
                            <div className="text-sm text-gray-400">Total URLs</div>
                            <div className="text-2xl font-bold text-yellow-400">{urlShortener.urls.length}</div>
                          </div>
                          <div className="bg-gray-800 p-3 rounded">
                            <div className="text-sm text-gray-400">Total Clicks</div>
                            <div className="text-2xl font-bold text-green-400">
                              {urlShortener.urls.reduce((sum, url) => sum + url.clicks, 0)}
                            </div>
                          </div>
                        </div>
                        
                        <div>
                          <h4 className="text-sm font-semibold text-gray-400 mb-2">Top Performing URLs</h4>
                          <div className="space-y-2">
                            {urlShortener.urls
                              .sort((a, b) => b.clicks - a.clicks)
                              .slice(0, 3)
                              .map(url => (
                                <div key={url.shortCode} className="bg-gray-800 p-2 rounded text-sm">
                                  <div className="flex justify-between">
                                    <span className="text-yellow-400">{url.shortCode}</span>
                                    <span className="text-green-400">{url.clicks} clicks</span>
                                  </div>
                                  <div className="text-xs text-gray-400 truncate">{url.longUrl}</div>
                                </div>
                              ))}
                          </div>
                        </div>
                      </div>
                    )}
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">System Components</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">ID Generation</h4>
                      <p className="text-sm">Snowflake algorithm generates unique IDs across distributed nodes.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Base62 Encoding</h4>
                      <p className="text-sm">Converts numeric IDs to alphanumeric strings for short URLs.</p>
                    </div>
                    <div className="border border-green-600 p-3 rounded">
                      <h4 className="font-semibold text-green-400">Analytics Store</h4>
                      <p className="text-sm">Time-series database tracks clicks and user behavior patterns.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
          
          {/* SHARDING */}
          {activeTab === 'sharding' && (
            <div className="h-full">
              <h2 className="text-2xl font-bold mb-4 text-green-400">Database Sharding</h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <div className="bg-gray-700 p-4 rounded mb-4">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Sharding Strategy</h3>
                    <select
                      value={shardingStrategy}
                      onChange={(e) => setShardingStrategy(e.target.value)}
                      className="w-full p-2 bg-gray-800 text-white rounded mb-4"
                    >
                      <option value="hash">Hash-based Sharding</option>
                      <option value="range">Range-based Sharding</option>
                      <option value="directory">Directory-based Sharding</option>
                    </select>
                    
                    <div className="flex">
                      <input
                        type="text"
                        value={newKey}
                        onChange={(e) => setNewKey(e.target.value)}
                        placeholder="Enter key to shard"
                        className="flex-1 p-2 bg-gray-800 text-white rounded"
                      />
                      <button
                        onClick={addShardedData}
                        className="ml-2 px-4 py-2 bg-green-600 text-white rounded hover:bg-green-500"
                      >
                        Add Data
                      </button>
                    </div>
                  </div>
                  
                  <div className="bg-gray-700 p-4 rounded">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Distribution Stats</h3>
                    <div className="space-y-4">
                      <div>
                        <h4 className="text-sm text-gray-400">Load Balance</h4>
                        <div className="w-full bg-gray-600 rounded-full h-4 flex overflow-hidden">
                          {shards.map((shard, index) => (
                            <div
                              key={shard.id}
                              style={{
                                width: `${(shard.load / Math.max(1, shards.reduce((sum, s) => sum + s.load, 0))) * 100}%`,
                                backgroundColor: `hsl(${index * 120}, 70%, 50%)`
                              }}
                              className="h-full"
                            />
                          ))}
                        </div>
                      </div>
                      
                      <div className="grid grid-cols-3 gap-2">
                        {shards.map(shard => (
                          <div key={shard.id} className="bg-gray-800 p-2 rounded text-center">
                            <div className="text-xs text-gray-400">{shard.name}</div>
                            <div className="text-sm font-bold">{shard.load} items</div>
                          </div>
                        ))}
                      </div>
                    </div>
                  </div>
                </div>
                
                <div>
                  <div className="bg-gray-700 p-4 rounded h-full">
                    <h3 className="text-lg font-semibold text-gray-300 mb-2">Shard Contents</h3>
                    <div className="space-y-4">
                      {shards.map(shard => (
                        <div key={shard.id} className="bg-gray-800 p-3 rounded">
                          <div className="flex justify-between items-center mb-2">
                            <h4 className="font-semibold text-green-400">{shard.name}</h4>
                            <span className="text-sm text-gray-400">{shard.range}</span>
                          </div>
                          <div className="flex flex-wrap gap-1">
                            {(shardedData[shard.id] || []).map((key, index) => (
                              <span key={index} className="px-2 py-1 bg-gray-700 rounded text-xs">
                                {key}
                              </span>
                            ))}
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
              </div>
              
              <div className="mt-8">
                <h3 className="text-lg font-semibold text-gray-300 mb-2">Sharding Strategies</h3>
                <div className="bg-gray-700 p-4 rounded">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div className="border border-cyan-600 p-3 rounded">
                      <h4 className="font-semibold text-cyan-400">Hash-based</h4>
                      <p className="text-sm">Uses hash function to distribute data evenly across shards. Simple but inflexible.</p>
                    </div>
                    <div className="border border-pink-600 p-3 rounded">
                      <h4 className="font-semibold text-pink-400">Range-based</h4>
                      <p className="text-sm">Divides data by key ranges. Allows range queries but may create hotspots.</p>
                    </div>
                    <div className="border border-yellow-600 p-3 rounded">
                      <h4 className="font-semibold text-yellow-400">Directory-based</h4>
                      <p className="text-sm">Uses a lookup table to map keys to shards. Flexible but requires extra lookups.</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          )}
        </div>
      </div>
      
      <div className="p-4 bg-gray-800 mt-4">
        <h2 className="text-xl font-bold text-center text-white mb-2">System Design Complexity Analysis</h2>
        <div className="overflow-x-auto">
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-gray-700">
                <th className="border border-gray-600 p-3 text-left text-cyan-400">Component</th>
                <th className="border border-gray-600 p-3 text-left text-pink-400">Operation</th>
                <th className="border border-gray-600 p-3 text-left text-yellow-400">Time Complexity</th>
                <th className="border border-gray-600 p-3 text-left text-green-400">Space Complexity</th>
              </tr>
            </thead>
            <tbody>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Consistent Hashing</td>
                <td className="border border-gray-600 p-3">Node Lookup</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(log N)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N×R)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Add/Remove Node</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(R log N)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N×R)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Key Redistribution</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(K/N)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(K)</td>
              </tr>
              
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Rate Limiting</td>
                <td className="border border-gray-600 p-3">Token Bucket</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(1)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Sliding Window Log</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(log N)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Sliding Window Counter</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(log W)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(W)</td>
              </tr>
              
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="2">URL Shortener</td>
                <td className="border border-gray-600 p-3">Create URL</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1) amortized</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Redirect</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N)</td>
              </tr>
              
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3 font-medium" rowSpan="3">Sharding</td>
                <td className="border border-gray-600 p-3">Hash-based</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(N)</td>
              </tr>
              <tr className="bg-gray-900 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Range-based</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(log S)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(S)</td>
              </tr>
              <tr className="bg-gray-800 hover:bg-gray-700 transition-colors">
                <td className="border border-gray-600 p-3">Directory-based</td>
                <td className="border border-gray-600 p-3 text-yellow-300">O(1)</td>
                <td className="border border-gray-600 p-3 text-green-300">O(K)</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div className="text-xs text-gray-400 mt-2">
          <p>N = number of nodes, R = replicas per node, K = number of keys, W = window size, S = number of shards</p>
        </div>
      </div>
    </div>
  );
};

export default SystemDesignVisualizer;
