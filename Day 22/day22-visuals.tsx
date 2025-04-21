import React from 'react';
import { GitBranch, GitFork, GitMerge, GitCommit, CornerDownRight, ArrowDown, ChevronDown, ChevronUp, ChevronRight, ChevronLeft } from 'lucide-react';

// Main component for Binary Trees Visualizations
const BinaryTreesVisualizations = () => {
  return (
    <div className="flex flex-col items-center justify-center w-full space-y-8 bg-gray-900 p-6 rounded-lg">
      <header className="w-full text-center mb-4">
        <h1 className="text-3xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-cyan-400 via-pink-500 to-yellow-300">
          Binary Trees Basics
        </h1>
        <p className="text-gray-300 mt-2">Day 22 of #DSAin45 - Branching into Hierarchical Data Structures</p>
      </header>

      {/* Main visualization section */}
      <div className="grid grid-cols-1 gap-8 w-full">
        {/* Binary Tree Structure Visualization */}
        <BinaryTreeStructure />
        
        {/* Tree Traversal Methods */}
        <TreeTraversalMethods />
        
        {/* Tree Types Comparison */}
        <TreeTypesComparison />
        
        {/* Time & Space Complexity */}
        <ComplexityTable />
        
        {/* Binary Tree Meme */}
        <BinaryTreeMeme />
      </div>
    </div>
  );
};

// Binary Tree Structure Visualization Component
const BinaryTreeStructure = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Binary Tree Structure</h2>
      
      <div className="flex flex-col items-center">
        {/* Tree Visualization */}
        <div className="relative w-full h-64 flex items-center justify-center">
          <svg 
            viewBox="0 0 320 200" 
            className="w-full h-full max-w-lg"
            style={{ 
              strokeLinecap: "round", 
              strokeLinejoin: "round",
              fill: "none"
            }}
          >
            {/* Tree Edges */}
            <path d="M160,40 L80,80" stroke="#00FFFF" strokeWidth="2" />
            <path d="M160,40 L240,80" stroke="#00FFFF" strokeWidth="2" />
            <path d="M80,80 L40,120" stroke="#00FFFF" strokeWidth="2" />
            <path d="M80,80 L120,120" stroke="#00FFFF" strokeWidth="2" />
            <path d="M240,80 L280,120" stroke="#00FFFF" strokeWidth="2" />
            
            {/* Root Node */}
            <circle cx="160" cy="40" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="160" y="45" textAnchor="middle" fill="#FF00FF" fontWeight="bold">1</text>
            <text x="160" y="15" textAnchor="middle" fill="#FF00FF" fontSize="12">Root</text>
            
            {/* Internal Nodes */}
            <circle cx="80" cy="80" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="80" y="85" textAnchor="middle" fill="#00FFFF" fontWeight="bold">2</text>
            
            <circle cx="240" cy="80" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="240" y="85" textAnchor="middle" fill="#00FFFF" fontWeight="bold">3</text>
            <text x="160" y="100" textAnchor="middle" fill="#00FFFF" fontSize="12">Internal Nodes</text>
            
            {/* Leaf Nodes */}
            <circle cx="40" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="40" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">4</text>
            
            <circle cx="120" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="120" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">5</text>
            
            <circle cx="280" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="280" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">6</text>
            <text x="160" y="150" textAnchor="middle" fill="#FFFF00" fontSize="12">Leaf Nodes</text>
          </svg>
        </div>
        
        {/* Tree Node Structure */}
        <div className="mt-4 bg-gray-700 p-4 rounded-lg w-full max-w-md">
          <h3 className="text-lg font-bold text-pink-400 mb-2">Node Structure</h3>
          <div className="font-mono text-sm bg-gray-800 p-3 rounded">
            <div className="text-green-400">struct TreeNode {</div>
            <div className="ml-4 text-gray-300">int val; <span className="text-gray-500">// Node's value</span></div>
            <div className="ml-4 text-gray-300">TreeNode* left; <span className="text-gray-500">// Pointer to left child</span></div>
            <div className="ml-4 text-gray-300">TreeNode* right; <span className="text-gray-500">// Pointer to right child</span></div>
            <div className="text-green-400">};</div>
          </div>
        </div>
        
        {/* Tree Terminology */}
        <div className="mt-4 grid grid-cols-1 md:grid-cols-2 gap-3 w-full">
          <TerminologyCard
            term="Height"
            definition="The number of edges on the longest path from a node to a leaf"
            icon={<ChevronUp className="text-cyan-400" />}
          />
          <TerminologyCard
            term="Depth"
            definition="The number of edges from the root to a specific node"
            icon={<ChevronDown className="text-pink-400" />}
          />
          <TerminologyCard
            term="Siblings"
            definition="Nodes that share the same parent"
            icon={<GitFork className="text-yellow-300" />}
          />
          <TerminologyCard
            term="Subtree"
            definition="A tree consisting of a node and all its descendants"
            icon={<GitBranch className="text-green-400" />}
          />
        </div>
      </div>
    </div>
  );
};

// Terminology Card Component
const TerminologyCard = ({ term, definition, icon }) => {
  return (
    <div className="bg-gray-800 border border-gray-700 rounded-lg p-3 flex items-start">
      <div className="mr-3 mt-1">{icon}</div>
      <div>
        <h4 className="font-bold text-gray-200">{term}</h4>
        <p className="text-gray-400 text-sm">{definition}</p>
      </div>
    </div>
  );
};

// Tree Traversal Methods Visualization
const TreeTraversalMethods = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-pink-500">
      <h2 className="text-xl font-bold text-pink-400 mb-4">Tree Traversal Methods</h2>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        {/* Depth-First Traversals */}
        <div className="bg-gray-700 p-4 rounded-lg">
          <h3 className="text-lg font-bold text-cyan-400 mb-2">Depth-First Traversals</h3>
          
          <div className="space-y-4">
            <TraversalMethod
              name="Pre-order (NLR)"
              example="1 → 2 → 4 → 5 → 3 → 6"
              illustration={
                <div className="flex items-center space-x-1 text-cyan-400">
                  <span className="font-bold">N</span>
                  <ChevronRight size={16} />
                  <span>L</span>
                  <ChevronRight size={16} />
                  <span>R</span>
                </div>
              }
              code="visit(node); traverse(left); traverse(right);"
              applications="Creating copies of trees, Expression trees (Polish notation)"
            />
            
            <TraversalMethod
              name="In-order (LNR)"
              example="4 → 2 → 5 → 1 → 3 → 6"
              illustration={
                <div className="flex items-center space-x-1 text-pink-400">
                  <span>L</span>
                  <ChevronRight size={16} />
                  <span className="font-bold">N</span>
                  <ChevronRight size={16} />
                  <span>R</span>
                </div>
              }
              code="traverse(left); visit(node); traverse(right);"
              applications="BST traversal (gives nodes in sorted order)"
            />
            
            <TraversalMethod
              name="Post-order (LRN)"
              example="4 → 5 → 2 → 6 → 3 → 1"
              illustration={
                <div className="flex items-center space-x-1 text-yellow-300">
                  <span>L</span>
                  <ChevronRight size={16} />
                  <span>R</span>
                  <ChevronRight size={16} />
                  <span className="font-bold">N</span>
                </div>
              }
              code="traverse(left); traverse(right); visit(node);"
              applications="Deleting trees, Evaluating expressions"
            />
          </div>
        </div>
        
        {/* Level-Order Traversal & Animation */}
        <div className="bg-gray-700 p-4 rounded-lg">
          <h3 className="text-lg font-bold text-green-400 mb-2">Breadth-First Traversal</h3>
          
          <TraversalMethod
            name="Level-order (BFS)"
            example="1 → 2 → 3 → 4 → 5 → 6"
            illustration={
              <div className="flex items-center space-x-1 text-green-400">
                <span>Level 0</span>
                <ChevronRight size={16} />
                <span>Level 1</span>
                <ChevronRight size={16} />
                <span>Level 2</span>
              </div>
            }
            code="queue.push(root); while(!queue.empty()) { node = queue.pop(); visit(node); queue.push(node->left); queue.push(node->right); }"
            applications="Level-order processing, Finding shortest path"
          />
          
          <div className="mt-4">
            <h4 className="font-bold text-gray-300 mb-2">Traversal Visualization</h4>
            <div className="relative w-full h-36 flex items-center justify-center bg-gray-800 rounded-lg">
              <svg 
                viewBox="0 0 320 120" 
                className="w-full h-full"
                style={{ 
                  strokeLinecap: "round", 
                  strokeLinejoin: "round",
                  fill: "none"
                }}
              >
                {/* Tree Edges */}
                <path d="M160,20 L80,50" stroke="#444" strokeWidth="2" />
                <path d="M160,20 L240,50" stroke="#444" strokeWidth="2" />
                <path d="M80,50 L40,80" stroke="#444" strokeWidth="2" />
                <path d="M80,50 L120,80" stroke="#444" strokeWidth="2" />
                <path d="M240,50 L280,80" stroke="#444" strokeWidth="2" />
                
                {/* Nodes */}
                <circle cx="160" cy="20" r="15" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
                <text x="160" y="25" textAnchor="middle" fill="#FF00FF" fontWeight="bold">1</text>
                
                <circle cx="80" cy="50" r="15" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <text x="80" y="55" textAnchor="middle" fill="#00FFFF" fontWeight="bold">2</text>
                
                <circle cx="240" cy="50" r="15" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <text x="240" y="55" textAnchor="middle" fill="#00FFFF" fontWeight="bold">3</text>
                
                <circle cx="40" cy="80" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
                <text x="40" y="85" textAnchor="middle" fill="#FFFF00" fontWeight="bold">4</text>
                
                <circle cx="120" cy="80" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
                <text x="120" y="85" textAnchor="middle" fill="#FFFF00" fontWeight="bold">5</text>
                
                <circle cx="280" cy="80" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
                <text x="280" y="85" textAnchor="middle" fill="#FFFF00" fontWeight="bold">6</text>
                
                {/* Traversal Order Labels */}
                <text x="185" y="20" textAnchor="middle" fill="#FF00FF" fontWeight="bold">①</text>
                <text x="55" y="50" textAnchor="middle" fill="#00FFFF" fontWeight="bold">②</text>
                <text x="265" y="50" textAnchor="middle" fill="#00FFFF" fontWeight="bold">③</text>
                <text x="15" y="80" textAnchor="middle" fill="#FFFF00" fontWeight="bold">④</text>
                <text x="145" y="80" textAnchor="middle" fill="#FFFF00" fontWeight="bold">⑤</text>
                <text x="305" y="80" textAnchor="middle" fill="#FFFF00" fontWeight="bold">⑥</text>
              </svg>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

// Traversal Method Component
const TraversalMethod = ({ name, example, illustration, code, applications }) => {
  return (
    <div className="bg-gray-800 p-3 rounded-lg">
      <h4 className="font-bold text-gray-200 flex items-center">
        <GitCommit size={16} className="mr-2" />
        {name}
      </h4>
      
      <div className="mt-1">
        <div className="text-gray-400 text-sm">{illustration}</div>
        <div className="mt-1 text-gray-300">Result: <span className="font-mono text-yellow-300">{example}</span></div>
        <div className="mt-1 text-xs text-pink-400 font-mono">{code}</div>
        <div className="mt-1 text-gray-400 text-sm">Applications: {applications}</div>
      </div>
    </div>
  );
};

// Tree Types Comparison Visualization
const TreeTypesComparison = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-yellow-300">
      <h2 className="text-xl font-bold text-yellow-300 mb-4">Types of Binary Trees</h2>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        <TreeTypeCard
          type="Full Binary Tree"
          definition="Every node has 0 or 2 children"
          visualization={
            <svg viewBox="0 0 160 120" className="w-full h-32">
              {/* Edges */}
              <path d="M80,20 L40,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M80,20 L120,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L20,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L60,80" stroke="#00FFFF" strokeWidth="2" />
              
              {/* Nodes */}
              <circle cx="80" cy="20" r="12" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
              <text x="80" y="24" textAnchor="middle" fill="#FF00FF">1</text>
              
              <circle cx="40" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="40" y="54" textAnchor="middle" fill="#00FFFF">2</text>
              
              <circle cx="120" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="120" y="54" textAnchor="middle" fill="#00FFFF">3</text>
              
              <circle cx="20" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="20" y="84" textAnchor="middle" fill="#FFFF00">4</text>
              
              <circle cx="60" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="60" y="84" textAnchor="middle" fill="#FFFF00">5</text>
            </svg>
          }
        />
        
        <TreeTypeCard
          type="Complete Binary Tree"
          definition="All levels filled except possibly the last, which is filled from left to right"
          visualization={
            <svg viewBox="0 0 160 120" className="w-full h-32">
              {/* Edges */}
              <path d="M80,20 L40,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M80,20 L120,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L20,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L60,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M120,50 L100,80" stroke="#00FFFF" strokeWidth="2" />
              
              {/* Nodes */}
              <circle cx="80" cy="20" r="12" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
              <text x="80" y="24" textAnchor="middle" fill="#FF00FF">1</text>
              
              <circle cx="40" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="40" y="54" textAnchor="middle" fill="#00FFFF">2</text>
              
              <circle cx="120" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="120" y="54" textAnchor="middle" fill="#00FFFF">3</text>
              
              <circle cx="20" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="20" y="84" textAnchor="middle" fill="#FFFF00">4</text>
              
              <circle cx="60" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="60" y="84" textAnchor="middle" fill="#FFFF00">5</text>
              
              <circle cx="100" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="100" y="84" textAnchor="middle" fill="#FFFF00">6</text>
            </svg>
          }
        />
        
        <TreeTypeCard
          type="Perfect Binary Tree"
          definition="All internal nodes have 2 children and all leaf nodes are at the same level"
          visualization={
            <svg viewBox="0 0 160 120" className="w-full h-32">
              {/* Edges */}
              <path d="M80,20 L40,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M80,20 L120,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L20,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L60,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M120,50 L100,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M120,50 L140,80" stroke="#00FFFF" strokeWidth="2" />
              
              {/* Nodes */}
              <circle cx="80" cy="20" r="12" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
              <text x="80" y="24" textAnchor="middle" fill="#FF00FF">1</text>
              
              <circle cx="40" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="40" y="54" textAnchor="middle" fill="#00FFFF">2</text>
              
              <circle cx="120" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="120" y="54" textAnchor="middle" fill="#00FFFF">3</text>
              
              <circle cx="20" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="20" y="84" textAnchor="middle" fill="#FFFF00">4</text>
              
              <circle cx="60" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="60" y="84" textAnchor="middle" fill="#FFFF00">5</text>
              
              <circle cx="100" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="100" y="84" textAnchor="middle" fill="#FFFF00">6</text>
              
              <circle cx="140" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="140" y="84" textAnchor="middle" fill="#FFFF00">7</text>
            </svg>
          }
        />
        
        <TreeTypeCard
          type="Balanced Binary Tree"
          definition="Height of left and right subtrees of any node differ by at most 1"
          visualization={
            <svg viewBox="0 0 160 120" className="w-full h-32">
              {/* Edges */}
              <path d="M80,20 L40,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M80,20 L120,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M40,50 L20,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M120,50 L100,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M120,50 L140,80" stroke="#00FFFF" strokeWidth="2" />
              
              {/* Nodes */}
              <circle cx="80" cy="20" r="12" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
              <text x="80" y="24" textAnchor="middle" fill="#FF00FF">1</text>
              
              <circle cx="40" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="40" y="54" textAnchor="middle" fill="#00FFFF">2</text>
              
              <circle cx="120" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="120" y="54" textAnchor="middle" fill="#00FFFF">3</text>
              
              <circle cx="20" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="20" y="84" textAnchor="middle" fill="#FFFF00">4</text>
              
              <circle cx="100" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="100" y="84" textAnchor="middle" fill="#FFFF00">5</text>
              
              <circle cx="140" cy="80" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="140" y="84" textAnchor="middle" fill="#FFFF00">6</text>
            </svg>
          }
        />
      </div>
      
      {/* Degenerate Tree */}
      <div className="mt-6 bg-gray-700 p-4 rounded-lg">
        <h3 className="text-lg font-bold text-pink-400 mb-2">Degenerate / Pathological Tree</h3>
        <div className="flex items-center">
          <div className="w-1/3">
            <svg viewBox="0 0 100 200" className="w-full h-40">
              {/* Edges */}
              <path d="M50,20 L60,50" stroke="#00FFFF" strokeWidth="2" />
              <path d="M60,50 L70,80" stroke="#00FFFF" strokeWidth="2" />
              <path d="M70,80 L80,110" stroke="#00FFFF" strokeWidth="2" />
              <path d="M80,110 L90,140" stroke="#00FFFF" strokeWidth="2" />
              
              {/* Nodes */}
              <circle cx="50" cy="20" r="12" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
              <text x="50" y="24" textAnchor="middle" fill="#FF00FF">1</text>
              
              <circle cx="60" cy="50" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="60" y="54" textAnchor="middle" fill="#00FFFF">2</text>
              
              <circle cx="70" cy="80" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="70" y="84" textAnchor="middle" fill="#00FFFF">3</text>
              
              <circle cx="80" cy="110" r="12" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
              <text x="80" y="114" textAnchor="middle" fill="#00FFFF">4</text>
              
              <circle cx="90" cy="140" r="12" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
              <text x="90" y="144" textAnchor="middle" fill="#FFFF00">5</text>
            </svg>
          </div>
          <div className="w-2/3 pl-4">
            <p className="text-gray-300">
              A degenerate or pathological tree is a tree where each parent node has only one child, effectively becoming a linked list.
            </p>
            <div className="mt-2 text-yellow-300 text-sm">
              <p>⚠️ Performance Warning:</p>
              <p className="text-gray-400">Search operations degrade to O(n) time complexity instead of O(log n)</p>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

// Tree Type Card Component
const TreeTypeCard = ({ type, definition, visualization }) => {
  return (
    <div className="bg-gray-700 p-4 rounded-lg">
      <h3 className="font-bold text-green-400 flex items-center">
        <GitBranch size={16} className="mr-2" />
        {type}
      </h3>
      <p className="text-gray-300 text-sm mt-1">{definition}</p>
      <div className="mt-2">
        {visualization}
      </div>
    </div>
  );
};

// Time & Space Complexity Table
const ComplexityTable = () => {
  const operations = [
    { name: "Insertion", time: "O(h) or O(log n)", space: "O(1)" },
    { name: "Deletion", time: "O(h) or O(log n)", space: "O(1)" },
    { name: "Search", time: "O(h) or O(log n)", space: "O(1)" },
    { name: "Traversal", time: "O(n)", space: "O(h) or O(log n)" },
    { name: "Height Calculation", time: "O(n)", space: "O(h)" },
    { name: "Diameter Calculation", time: "O(n)", space: "O(h)" }
  ];

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-green-400">
      <h2 className="text-xl font-bold text-green-400 mb-4">Time & Space Complexity</h2>
      
      <div className="overflow-x-auto">
        <table className="w-full text-left border-collapse">
          <thead>
            <tr className="border-b border-gray-700">
              <th className="py-2 px-4 text-cyan-400">Operation</th>
              <th className="py-2 px-4 text-pink-400">Time Complexity</th>
              <th className="py-2 px-4 text-yellow-300">Space Complexity</th>
              <th className="py-2 px-4 text-green-400">Notes</th>
            </tr>
          </thead>
          <tbody>
            {operations.map((op, index) => (
              <tr key={index} className={`${index % 2 === 0 ? 'bg-gray-800' : 'bg-gray-700'} hover:bg-gray-600`}>
                <td className="py-2 px-4 text-gray-300">{op.name}</td>
                <td className="py-2 px-4 text-pink-400 font-mono">{op.time}</td>
                <td className="py-2 px-4 text-yellow-300 font-mono">{op.space}</td>
                <td className="py-2 px-4 text-gray-400 text-sm">
                  {op.name === "Insertion" && "h = height of tree, log n if balanced"}
                  {op.name === "Deletion" && "Includes finding the node"}
                  {op.name === "Search" && "Worst case = O(n) for degenerate trees"}
                  {op.name === "Traversal" && "Space for recursion stack"}
                  {op.name === "Height Calculation" && "Requires visiting all nodes"}
                  {op.name === "Diameter Calculation" && "Can be optimized with post-order traversal"}
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      
      <div className="mt-4 grid grid-cols-1 md:grid-cols-2 gap-4">
        <div className="bg-gray-700 p-3 rounded-lg">
          <h3 className="font-bold text-gray-200 flex items-center">
            <GitMerge size={16} className="mr-2 text-cyan-400" />
            Balanced vs. Unbalanced
          </h3>
          <p className="text-gray-300 text-sm mt-1">
            In a balanced tree, operations achieve O(log n) complexity. Unbalanced/degenerate trees degrade to O(n).
          </p>
        </div>
        
        <div className="bg-gray-700 p-3 rounded-lg">
          <h3 className="font-bold text-gray-200 flex items-center">
            <CornerDownRight size={16} className="mr-2 text-pink-400" />
            Recursive Implementation Impact
          </h3>
          <p className="text-gray-300 text-sm mt-1">
            Recursive implementations use O(h) extra space for the call stack, where h is the height of the tree.
          </p>
        </div>
      </div>
    </div>
  );
};

// Binary Tree Meme Component
const BinaryTreeMeme = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Binary Tree Meme</h2>
      
      <div className="bg-gray-900 p-4 rounded-lg max-w-xl mx-auto">
        <div className="text-center font-bold text-lg text-white mb-4">
          The Tree Traversal Dating Service
        </div>
        
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <div className="bg-gray-800 p-3 rounded-lg border border-red-500">
            <div className="text-red-400 font-bold flex items-center">
              <ChevronLeft className="mr-1" size={16} />
              <span>SWIPE LEFT</span>
              <ChevronLeft className="ml-1" size={16} />
            </div>
            <div className="mt-2 text-gray-300">
              "Swiping randomly through dating profiles and never finding the right match"
            </div>
            <div className="mt-2 text-gray-500 text-sm">
              Time Complexity: O(forever)
            </div>
          </div>
          
          <div className="bg-gray-800 p-3 rounded-lg border border-green-500">
            <div className="text-green-400 font-bold flex items-center">
              <ChevronRight className="mr-1" size={16} />
              <span>SWIPE RIGHT</span>
              <ChevronRight className="ml-1" size={16} />
            </div>
            <div className="mt-2 text-gray-300">
              "Using tree traversal algorithms to methodically search for your perfect match"
            </div>
            <div className="mt-2 text-gray-500 text-sm">
              Time Complexity: O(log n)
            </div>
          </div>
        </div>
        
        <div className="mt-4 grid grid-cols-2 md:grid-cols-4 gap-2 text-xs">
          <div className="bg-gray-800 p-2 rounded text-center border border-cyan-500">
            <div className="text-cyan-500 font-bold">In-Order</div>
            <div className="text-gray-400">For romance</div>
          </div>
          
          <div className="bg-gray-800 p-2 rounded text-center border border-pink-500">
            <div className="text-pink-500 font-bold">Pre-Order</div>
            <div className="text-gray-400">For adventure</div>
          </div>
          
          <div className="bg-gray-800 p-2 rounded text-center border border-yellow-300">
            <div className="text-yellow-300 font-bold">Post-Order</div>
            <div className="text-gray-400">For commitment</div>
          </div>
          
          <div className="bg-gray-800 p-2 rounded text-center border border-green-500">
            <div className="text-green-500 font-bold">Level-Order</div>
            <div className="text-gray-400">For variety</div>
          </div>
        </div>
        
        <div className="mt-4 text-center text-gray-500 text-sm">
          #DSAin45 #BinaryTreeHumor
        </div>
      </div>
    </div>
  );
};

export default BinaryTreesVisualizations;
