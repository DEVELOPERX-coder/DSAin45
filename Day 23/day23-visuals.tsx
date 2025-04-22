import React, { useState } from 'react';
import { GitBranch, GitFork, GitMerge, GitCommit, Search, Plus, Trash2, ArrowDown, ArrowUp, SortAsc, LayoutList, BarChart4 } from 'lucide-react';

// Main component for Binary Search Tree Visualizations
const BinarySearchTreeVisualizations = () => {
  return (
    <div className="flex flex-col items-center justify-center w-full space-y-8 bg-gray-900 p-6 rounded-lg">
      <header className="w-full text-center mb-4">
        <h1 className="text-3xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-cyan-400 via-pink-500 to-yellow-300">
          Binary Search Trees
        </h1>
        <p className="text-gray-300 mt-2">Day 23 of #DSAin45 - Fast, Ordered, and Efficient</p>
      </header>

      {/* Main visualization section */}
      <div className="grid grid-cols-1 gap-8 w-full">
        {/* BST Structure and Property */}
        <BSTStructure />
        
        {/* BST Operations */}
        <BSTOperations />
        
        {/* BST vs Other Data Structures */}
        <BSTComparison />
        
        {/* Time & Space Complexity */}
        <ComplexityTable />
        
        {/* BST Meme */}
        <BSTMeme />
      </div>
    </div>
  );
};

// BST Structure and Property Visualization
const BSTStructure = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Binary Search Tree Structure & Property</h2>
      
      <div className="flex flex-col md:flex-row items-center justify-between">
        {/* BST Visualization */}
        <div className="w-full md:w-1/2 relative h-64 flex items-center justify-center">
          <svg 
            viewBox="0 0 320 200" 
            className="w-full h-full"
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
            
            {/* BST Nodes */}
            <circle cx="160" cy="40" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="160" y="45" textAnchor="middle" fill="#FF00FF" fontWeight="bold">8</text>
            
            <circle cx="80" cy="80" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="80" y="85" textAnchor="middle" fill="#00FFFF" fontWeight="bold">3</text>
            
            <circle cx="240" cy="80" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="240" y="85" textAnchor="middle" fill="#00FFFF" fontWeight="bold">10</text>
            
            <circle cx="40" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="40" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">1</text>
            
            <circle cx="120" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="120" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">6</text>
            
            <circle cx="280" cy="120" r="20" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
            <text x="280" y="125" textAnchor="middle" fill="#FFFF00" fontWeight="bold">14</text>
            
            {/* BST Property Labels */}
            <text x="60" y="60" textAnchor="middle" fill="#00FFFF" fontSize="10">Left < Parent</text>
            <text x="260" y="60" textAnchor="middle" fill="#00FFFF" fontSize="10">Right > Parent</text>
          </svg>
        </div>
        
        {/* BST Property Explanation */}
        <div className="w-full md:w-1/2 p-4">
          <h3 className="font-bold text-pink-400 mb-2">The BST Property</h3>
          <ul className="space-y-2 text-gray-300">
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-cyan-400"><ArrowDown size={16} /></div>
              <div>For any node N, all values in the <span className="text-cyan-400">left subtree</span> are <span className="text-pink-400">less than</span> N's value</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-yellow-300"><ArrowUp size={16} /></div>
              <div>For any node N, all values in the <span className="text-yellow-300">right subtree</span> are <span className="text-pink-400">greater than</span> N's value</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-green-400"><SortAsc size={16} /></div>
              <div>This ordering enables <span className="text-green-400">O(log n)</span> search, insert, and delete operations (when balanced)</div>
            </li>
          </ul>
          
          <div className="mt-4 p-3 bg-gray-700 rounded-lg text-sm">
            <div className="font-bold text-gray-200">BST vs. Basic Binary Tree:</div>
            <div className="text-gray-300 mt-1">
              A basic binary tree organizes data hierarchically, but a BST adds the 
              crucial left-less-than/right-greater-than ordering property, enabling efficient search.
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

// BST Operations Visualization
const BSTOperations = () => {
  // State for active operation
  const [activeOp, setActiveOp] = useState('search');
  
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-pink-500">
      <h2 className="text-xl font-bold text-pink-400 mb-4">BST Operations</h2>
      
      {/* Operation Selector */}
      <div className="flex flex-wrap gap-2 mb-4">
        <OperationButton 
          icon={<Search size={16} />} 
          name="Search" 
          color="cyan"
          active={activeOp === 'search'} 
          onClick={() => setActiveOp('search')} 
        />
        <OperationButton 
          icon={<Plus size={16} />} 
          name="Insert" 
          color="pink"
          active={activeOp === 'insert'} 
          onClick={() => setActiveOp('insert')} 
        />
        <OperationButton 
          icon={<Trash2 size={16} />} 
          name="Delete" 
          color="yellow"
          active={activeOp === 'delete'} 
          onClick={() => setActiveOp('delete')} 
        />
        <OperationButton 
          icon={<LayoutList size={16} />} 
          name="Traversal" 
          color="green"
          active={activeOp === 'traversal'} 
          onClick={() => setActiveOp('traversal')} 
        />
      </div>
      
      {/* Operation Visualization */}
      <div className="bg-gray-700 rounded-lg p-4">
        {activeOp === 'search' && <SearchOperation />}
        {activeOp === 'insert' && <InsertOperation />}
        {activeOp === 'delete' && <DeleteOperation />}
        {activeOp === 'traversal' && <TraversalOperation />}
      </div>
    </div>
  );
};

// Operation Button Component
const OperationButton = ({ icon, name, active, onClick, color }) => {
  const colorMap = {
    cyan: active ? "bg-cyan-900 border-cyan-400 text-cyan-400" : "bg-gray-700 border-gray-600 text-gray-300 hover:bg-gray-600",
    pink: active ? "bg-pink-900 border-pink-400 text-pink-400" : "bg-gray-700 border-gray-600 text-gray-300 hover:bg-gray-600",
    yellow: active ? "bg-yellow-900 border-yellow-300 text-yellow-300" : "bg-gray-700 border-gray-600 text-gray-300 hover:bg-gray-600",
    green: active ? "bg-green-900 border-green-400 text-green-400" : "bg-gray-700 border-gray-600 text-gray-300 hover:bg-gray-600"
  };
  
  return (
    <button 
      className={`flex items-center px-3 py-2 rounded-lg border ${colorMap[color]} transition-colors`}
      onClick={onClick}
    >
      <span className="mr-2">{icon}</span>
      <span>{name}</span>
    </button>
  );
};

// Search Operation Component
const SearchOperation = () => {
  return (
    <div className="space-y-4">
      <h3 className="font-bold text-cyan-400">Search Operation - O(log n)</h3>
      
      <div className="flex flex-col md:flex-row gap-4">
        <div className="w-full md:w-1/2 h-64 relative">
          <svg 
            viewBox="0 0 320 200" 
            className="w-full h-full"
            style={{ strokeLinecap: "round", strokeLinejoin: "round", fill: "none" }}
          >
            {/* Tree Edges */}
            <path d="M160,40 L80,80" stroke="#444" strokeWidth="2" />
            <path d="M160,40 L240,80" stroke="#00FFFF" strokeWidth="2" strokeDasharray="5,5" />
            <path d="M80,80 L40,120" stroke="#444" strokeWidth="2" />
            <path d="M80,80 L120,120" stroke="#00FFFF" strokeWidth="2" strokeDasharray="5,5" />
            
            {/* BST Nodes */}
            <circle cx="160" cy="40" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="160" y="45" textAnchor="middle" fill="#00FFFF" fontWeight="bold">8</text>
            
            <circle cx="80" cy="80" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="80" y="85" textAnchor="middle" fill="#00FFFF" fontWeight="bold">3</text>
            
            <circle cx="240" cy="80" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="240" y="85" textAnchor="middle" fill="#888" fontWeight="bold">10</text>
            
            <circle cx="40" cy="120" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="40" y="125" textAnchor="middle" fill="#888" fontWeight="bold">1</text>
            
            <circle cx="120" cy="120" r="20" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
            <text x="120" y="125" textAnchor="middle" fill="#00FFFF" fontWeight="bold">6</text>
            
            {/* Target Value */}
            <text x="160" y="180" textAnchor="middle" fill="#00FFFF" fontSize="14">Searching for: 6</text>
            
            {/* Search Path */}
            <text x="180" y="40" textAnchor="start" fill="#00FFFF" fontSize="12">1. Start at root (8)</text>
            <text x="180" y="55" textAnchor="start" fill="#00FFFF" fontSize="12">2. 6 &lt; 8, go left</text>
            <text x="180" y="70" textAnchor="start" fill="#00FFFF" fontSize="12">3. At node 3</text>
            <text x="180" y="85" textAnchor="start" fill="#00FFFF" fontSize="12">4. 6 &gt; 3, go right</text>
            <text x="180" y="100" textAnchor="start" fill="#00FFFF" fontSize="12">5. At node 6</text>
            <text x="180" y="115" textAnchor="start" fill="#00FFFF" fontSize="12">6. Found!</text>
          </svg>
        </div>
        
        <div className="w-full md:w-1/2">
          <div className="bg-gray-800 p-3 rounded-lg mb-4">
            <div className="font-mono text-sm text-gray-300">
              <div className="text-cyan-400">bool search(TreeNode* root, int val) {</div>
              <div className="ml-4">if (root == nullptr) {</div>
              <div className="ml-8">return false;</div>
              <div className="ml-4">}</div>
              <div className="ml-4 text-green-300">// Found the value</div>
              <div className="ml-4">if (root->val == val) {</div>
              <div className="ml-8">return true;</div>
              <div className="ml-4">}</div>
              <div className="ml-4 text-green-300">// Decide which subtree to search</div>
              <div className="ml-4">if (val < root->val) {</div>
              <div className="ml-8 text-cyan-400">return search(root->left, val);</div>
              <div className="ml-4">} else {</div>
              <div className="ml-8">return search(root->right, val);</div>
              <div className="ml-4">}</div>
              <div className="text-cyan-400">}</div>
            </div>
          </div>
          
          <ul className="space-y-2 text-gray-300 text-sm">
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-cyan-400"><GitBranch size={14} /></div>
              <div>The BST property allows us to eliminate half the remaining nodes at each step</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-cyan-400"><GitBranch size={14} /></div>
              <div>For a balanced BST, we can find any node in O(log n) time</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-pink-400"><GitBranch size={14} /></div>
              <div>Worst case: O(n) for a completely unbalanced tree (becomes like a linked list)</div>
            </li>
          </ul>
        </div>
      </div>
    </div>
  );
};

// Insert Operation Component
const InsertOperation = () => {
  return (
    <div className="space-y-4">
      <h3 className="font-bold text-pink-400">Insert Operation - O(log n)</h3>
      
      <div className="flex flex-col md:flex-row gap-4">
        <div className="w-full md:w-1/2 h-64 relative">
          <svg 
            viewBox="0 0 320 200" 
            className="w-full h-full"
            style={{ strokeLinecap: "round", strokeLinejoin: "round", fill: "none" }}
          >
            {/* Tree Edges */}
            <path d="M160,40 L80,80" stroke="#444" strokeWidth="2" />
            <path d="M160,40 L240,80" stroke="#FF00FF" strokeWidth="2" strokeDasharray="5,5" />
            <path d="M240,80 L280,120" stroke="#FF00FF" strokeWidth="2" strokeDasharray="5,5" />
            
            {/* BST Nodes */}
            <circle cx="160" cy="40" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="160" y="45" textAnchor="middle" fill="#FF00FF" fontWeight="bold">8</text>
            
            <circle cx="80" cy="80" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="80" y="85" textAnchor="middle" fill="#888" fontWeight="bold">3</text>
            
            <circle cx="240" cy="80" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="240" y="85" textAnchor="middle" fill="#FF00FF" fontWeight="bold">10</text>
            
            <circle cx="280" cy="120" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="280" y="125" textAnchor="middle" fill="#FF00FF" fontWeight="bold">14</text>
            
            {/* New Node to Insert */}
            <circle cx="260" cy="160" r="20" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
            <text x="260" y="165" textAnchor="middle" fill="#FF00FF" fontWeight="bold">12</text>
            
            {/* Insertion Path */}
            <path d="M280,120 L260,160" stroke="#FF00FF" strokeWidth="2" strokeDasharray="5,5" />
            
            {/* Target Value */}
            <text x="160" y="190" textAnchor="middle" fill="#FF00FF" fontSize="14">Inserting: 12</text>
            
            {/* Insert Path */}
            <text x="30" y="40" textAnchor="start" fill="#FF00FF" fontSize="12">1. Start at root (8)</text>
            <text x="30" y="55" textAnchor="start" fill="#FF00FF" fontSize="12">2. 12 > 8, go right</text>
            <text x="30" y="70" textAnchor="start" fill="#FF00FF" fontSize="12">3. At node 10</text>
            <text x="30" y="85" textAnchor="start" fill="#FF00FF" fontSize="12">4. 12 > 10, go right</text>
            <text x="30" y="100" textAnchor="start" fill="#FF00FF" fontSize="12">5. At node 14</text>
            <text x="30" y="115" textAnchor="start" fill="#FF00FF" fontSize="12">6. 12 < 14, go left</text>
            <text x="30" y="130" textAnchor="start" fill="#FF00FF" fontSize="12">7. Empty spot, insert 12</text>
          </svg>
        </div>
        
        <div className="w-full md:w-1/2">
          <div className="bg-gray-800 p-3 rounded-lg mb-4">
            <div className="font-mono text-sm text-gray-300">
              <div className="text-pink-400">TreeNode* insert(TreeNode* root, int val) {</div>
              <div className="ml-4 text-green-300">// Found insertion point</div>
              <div className="ml-4">if (root == nullptr) {</div>
              <div className="ml-8">return new TreeNode(val);</div>
              <div className="ml-4">}</div>
              <div className="ml-4 text-green-300">// Decide which subtree to insert into</div>
              <div className="ml-4">if (val < root->val) {</div>
              <div className="ml-8">root->left = insert(root->left, val);</div>
              <div className="ml-4">} else if (val > root->val) {</div>
              <div className="ml-8 text-pink-400">root->right = insert(root->right, val);</div>
              <div className="ml-4">}</div>
              <div className="ml-4 text-green-300">// Handle duplicates (if needed)</div>
              <div className="ml-4">return root;</div>
              <div className="text-pink-400">}</div>
            </div>
          </div>
          
          <ul className="space-y-2 text-gray-300 text-sm">
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-pink-400"><GitBranch size={14} /></div>
              <div>Insertion follows the same search path until finding an empty spot</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-pink-400"><GitBranch size={14} /></div>
              <div>The BST property is maintained by the insertion logic</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-yellow-300"><GitBranch size={14} /></div>
              <div>No rebalancing in a standard BST - can lead to unbalanced trees</div>
            </li>
          </ul>
        </div>
      </div>
    </div>
  );
};

// Delete Operation Component
const DeleteOperation = () => {
  return (
    <div className="space-y-4">
      <h3 className="font-bold text-yellow-300">Delete Operation - O(log n)</h3>
      
      <div className="flex flex-col gap-4">
        <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
          <DeleteCase
            title="Case 1: Leaf Node"
            description="Simply remove the node, set parent's pointer to null"
            color="green"
            svgContent={
              <>
                {/* Tree Edges */}
                <path d="M50,30 L20,60" stroke="#444" strokeWidth="2" />
                <path d="M50,30 L80,60" stroke="#444" strokeWidth="2" />
                <path d="M20,60 L35,90" stroke="#FFFF00" strokeWidth="2" strokeDasharray="5,5" />
                
                {/* Nodes */}
                <circle cx="50" cy="30" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="50" y="34" textAnchor="middle" fill="#888" fontWeight="bold">8</text>
                
                <circle cx="20" cy="60" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
                <text x="20" y="64" textAnchor="middle" fill="#FFFF00" fontWeight="bold">3</text>
                
                <circle cx="80" cy="60" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="80" y="64" textAnchor="middle" fill="#888" fontWeight="bold">10</text>
                
                <circle cx="35" cy="90" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="3" strokeDasharray="2,2" />
                <text x="35" y="94" textAnchor="middle" fill="#FFFF00" fontWeight="bold">6</text>
                
                {/* X Mark on the node to delete */}
                <path d="M25,80 L45,100" stroke="#FF5555" strokeWidth="2" />
                <path d="M25,100 L45,80" stroke="#FF5555" strokeWidth="2" />
              </>
            }
          />
          
          <DeleteCase
            title="Case 2: One Child"
            description="Replace node with its child"
            color="cyan"
            svgContent={
              <>
                {/* Tree Edges */}
                <path d="M50,30 L20,60" stroke="#444" strokeWidth="2" />
                <path d="M50,30 L80,60" stroke="#444" strokeWidth="2" />
                <path d="M80,60 L65,90" stroke="#FFFF00" strokeWidth="2" strokeDasharray="5,5" />
                <path d="M65,90 L80,120" stroke="#FFFF00" strokeWidth="2" />
                
                {/* Nodes */}
                <circle cx="50" cy="30" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="50" y="34" textAnchor="middle" fill="#888" fontWeight="bold">8</text>
                
                <circle cx="20" cy="60" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="20" y="64" textAnchor="middle" fill="#888" fontWeight="bold">3</text>
                
                <circle cx="80" cy="60" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="2" />
                <text x="80" y="64" textAnchor="middle" fill="#FFFF00" fontWeight="bold">10</text>
                
                <circle cx="65" cy="90" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="3" strokeDasharray="2,2" />
                <text x="65" y="94" textAnchor="middle" fill="#FFFF00" fontWeight="bold">9</text>
                
                <circle cx="80" cy="120" r="15" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <text x="80" y="124" textAnchor="middle" fill="#00FFFF" fontWeight="bold">12</text>
                
                {/* X Mark on the node to delete */}
                <path d="M55,80 L75,100" stroke="#FF5555" strokeWidth="2" />
                <path d="M55,100 L75,80" stroke="#FF5555" strokeWidth="2" />
                
                {/* Arrow showing replacement */}
                <path d="M100,105 L115,105" stroke="#00FFFF" strokeWidth="2" markerEnd="url(#arrowhead)" />
              </>
            }
          />
          
          <DeleteCase
            title="Case 3: Two Children"
            description="Replace with in-order successor (smallest in right subtree), then delete successor"
            color="pink"
            svgContent={
              <>
                {/* Tree Edges */}
                <path d="M50,30 L20,60" stroke="#444" strokeWidth="2" />
                <path d="M50,30 L80,60" stroke="#FFFF00" strokeWidth="2" strokeDasharray="5,5" />
                <path d="M80,60 L65,90" stroke="#FFFF00" strokeWidth="2" />
                <path d="M80,60 L95,90" stroke="#FFFF00" strokeWidth="2" />
                
                {/* Nodes */}
                <circle cx="50" cy="30" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="50" y="34" textAnchor="middle" fill="#888" fontWeight="bold">8</text>
                
                <circle cx="20" cy="60" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="20" y="64" textAnchor="middle" fill="#888" fontWeight="bold">3</text>
                
                <circle cx="80" cy="60" r="15" fill="#121212" stroke="#FFFF00" strokeWidth="3" strokeDasharray="2,2" />
                <text x="80" y="64" textAnchor="middle" fill="#FFFF00" fontWeight="bold">10</text>
                
                <circle cx="65" cy="90" r="15" fill="#121212" stroke="#FF00FF" strokeWidth="2" />
                <text x="65" y="94" textAnchor="middle" fill="#FF00FF" fontWeight="bold">9</text>
                
                <circle cx="95" cy="90" r="15" fill="#121212" stroke="#444" strokeWidth="2" />
                <text x="95" y="94" textAnchor="middle" fill="#888" fontWeight="bold">14</text>
                
                {/* X Mark on the node to delete */}
                <path d="M70,50 L90,70" stroke="#FF5555" strokeWidth="2" />
                <path d="M70,70 L90,50" stroke="#FF5555" strokeWidth="2" />
                
                {/* Arrow showing successor replacement */}
                <path d="M60,105 L80,75" stroke="#FF00FF" strokeWidth="2" markerEnd="url(#arrowhead)" />
              </>
            }
          />
        </div>
        
        <div className="bg-gray-800 p-3 rounded-lg">
          <div className="font-mono text-sm text-gray-300 max-h-48 overflow-y-auto">
            <div className="text-yellow-300">TreeNode* remove(TreeNode* root, int val) {</div>
            <div className="ml-4">if (root == nullptr) return nullptr;</div>
            <div className="ml-4 text-green-300">// Find the node to delete</div>
            <div className="ml-4">if (val < root->val) {</div>
            <div className="ml-8">root->left = remove(root->left, val);</div>
            <div className="ml-4">} else if (val > root->val) {</div>
            <div className="ml-8">root->right = remove(root->right, val);</div>
            <div className="ml-4">} else {</div>
            <div className="ml-8 text-green-300">// Node found - handle deletion cases</div>
            <div className="ml-8 text-green-300">// Case 1: Leaf node</div>
            <div className="ml-8">if (!root->left && !root->right) {</div>
            <div className="ml-12">delete root; return nullptr;</div>
            <div className="ml-8">}</div>
            <div className="ml-8 text-green-300">// Case 2: One child</div>
            <div className="ml-8">if (!root->left) {</div>
            <div className="ml-12">TreeNode* temp = root->right;</div>
            <div className="ml-12">delete root; return temp;</div>
            <div className="ml-8">} else if (!root->right) {</div>
            <div className="ml-12">TreeNode* temp = root->left;</div>
            <div className="ml-12">delete root; return temp;</div>
            <div className="ml-8">}</div>
            <div className="ml-8 text-green-300">// Case 3: Two children</div>
            <div className="ml-8 text-yellow-300">TreeNode* successor = findMin(root->right);</div>
            <div className="ml-8 text-yellow-300">root->val = successor->val;</div>
            <div className="ml-8 text-yellow-300">root->right = remove(root->right, successor->val);</div>
            <div className="ml-4">}</div>
            <div className="ml-4">return root;</div>
            <div className="text-yellow-300">}</div>
          </div>
        </div>
      </div>
    </div>
  );
};

// Delete Case Component
const DeleteCase = ({ title, description, color, svgContent }) => {
  const colorMap = {
    green: "text-green-400",
    cyan: "text-cyan-400",
    pink: "text-pink-400",
    yellow: "text-yellow-300"
  };
  
  return (
    <div className="bg-gray-700 p-3 rounded-lg">
      <h4 className={`font-bold ${colorMap[color]} mb-2`}>{title}</h4>
      <div className="text-gray-300 text-sm mb-2">{description}</div>
      
      <svg 
        viewBox="0 0 120 140" 
        className="w-full h-32"
        style={{ strokeLinecap: "round", strokeLinejoin: "round", fill: "none" }}
      >
        {/* Define arrowhead marker */}
        <defs>
          <marker
            id="arrowhead"
            markerWidth="10"
            markerHeight="7"
            refX="9"
            refY="3.5"
            orient="auto"
          >
            <polygon points="0 0, 10 3.5, 0 7" fill="#00FFFF" />
          </marker>
        </defs>
        
        {svgContent}
      </svg>
    </div>
  );
};

// Traversal Operation Component
const TraversalOperation = () => {
  return (
    <div className="space-y-4">
      <h3 className="font-bold text-green-400">BST Traversals</h3>
      
      <div className="flex flex-col md:flex-row gap-4">
        <div className="w-full md:w-1/2 h-64 relative">
          <svg 
            viewBox="0 0 320 200" 
            className="w-full h-full"
            style={{ strokeLinecap: "round", strokeLinejoin: "round", fill: "none" }}
          >
            {/* Tree Edges */}
            <path d="M160,40 L80,80" stroke="#444" strokeWidth="2" />
            <path d="M160,40 L240,80" stroke="#444" strokeWidth="2" />
            <path d="M80,80 L40,120" stroke="#444" strokeWidth="2" />
            <path d="M80,80 L120,120" stroke="#444" strokeWidth="2" />
            <path d="M240,80 L280,120" stroke="#444" strokeWidth="2" />
            
            {/* BST Nodes */}
            <circle cx="160" cy="40" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="160" y="45" textAnchor="middle" fill="#888" fontWeight="bold">8</text>
            <text x="140" y="35" textAnchor="end" fill="#00FF00" fontSize="10" fontWeight="bold">②</text>
            
            <circle cx="80" cy="80" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="80" y="85" textAnchor="middle" fill="#888" fontWeight="bold">3</text>
            <text x="60" y="75" textAnchor="end" fill="#00FF00" fontSize="10" fontWeight="bold">①</text>
            
            <circle cx="240" cy="80" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="240" y="85" textAnchor="middle" fill="#888" fontWeight="bold">10</text>
            <text x="260" y="75" textAnchor="start" fill="#00FF00" fontSize="10" fontWeight="bold">③</text>
            
            <circle cx="40" cy="120" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="40" y="125" textAnchor="middle" fill="#888" fontWeight="bold">1</text>
            
            <circle cx="120" cy="120" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="120" y="125" textAnchor="middle" fill="#888" fontWeight="bold">6</text>
            
            <circle cx="280" cy="120" r="20" fill="#121212" stroke="#444" strokeWidth="2" />
            <text x="280" y="125" textAnchor="middle" fill="#888" fontWeight="bold">14</text>
            
            {/* Traversal Order Labels */}
            <text x="160" y="180" textAnchor="middle" fill="#00FF00" fontSize="14">In-Order (LNR): 1, 3, 6, 8, 10, 14</text>
          </svg>
        </div>
        
        <div className="w-full md:w-1/2">
          <div className="grid grid-cols-1 gap-2">
            <TraversalMethod
              name="In-Order (LNR)"
              order="Left → Node → Right"
              result="1, 3, 6, 8, 10, 14"
              color="green"
              note="Gives nodes in sorted order - a key BST property!"
            />
            
            <TraversalMethod
              name="Pre-Order (NLR)"
              order="Node → Left → Right"
              result="8, 3, 1, 6, 10, 14"
              color="cyan"
              note="Useful for creating a copy of the tree"
            />
            
            <TraversalMethod
              name="Post-Order (LRN)"
              order="Left → Right → Node"
              result="1, 6, 3, 14, 10, 8"
              color="yellow"
              note="Useful for deleting a tree (process children before parent)"
            />
            
            <TraversalMethod
              name="Level-Order (BFS)"
              order="Level by level, left to right"
              result="8, 3, 10, 1, 6, 14"
              color="pink"
              note="Visits nodes by their distance from the root"
            />
          </div>
        </div>
      </div>
    </div>
  );
};

// Traversal Method Component
const TraversalMethod = ({ name, order, result, color, note }) => {
  const colorMap = {
    green: "border-green-400 text-green-400",
    cyan: "border-cyan-400 text-cyan-400",
    yellow: "border-yellow-300 text-yellow-300",
    pink: "border-pink-400 text-pink-400"
  };
  
  return (
    <div className={`p-2 border rounded-lg ${colorMap[color]}`}>
      <div className="flex justify-between">
        <div className="font-bold">{name}</div>
        <div className="text-sm">{order}</div>
      </div>
      <div className="mt-1 text-gray-300 text-sm">Result: {result}</div>
      <div className="mt-1 text-gray-400 text-xs italic">{note}</div>
    </div>
  );
};

// BST vs Other Data Structures Comparison
const BSTComparison = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-yellow-300">
      <h2 className="text-xl font-bold text-yellow-300 mb-4">BST vs. Other Data Structures</h2>
      
      <div className="overflow-x-auto">
        <table className="w-full text-left border-collapse">
          <thead>
            <tr className="border-b border-gray-700">
              <th className="py-2 px-4 text-cyan-400">Data Structure</th>
              <th className="py-2 px-4 text-pink-400">Search</th>
              <th className="py-2 px-4 text-yellow-300">Insert</th>
              <th className="py-2 px-4 text-green-400">Delete</th>
              <th className="py-2 px-4 text-gray-300">Ordered?</th>
              <th className="py-2 px-4 text-gray-300">Key Advantage</th>
            </tr>
          </thead>
          <tbody>
            <tr className="border-b border-gray-700">
              <td className="py-2 px-4 text-cyan-400">Sorted Array</td>
              <td className="py-2 px-4">O(log n)</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">✓</td>
              <td className="py-2 px-4">Continuous memory, good cache locality</td>
            </tr>
            <tr className="border-b border-gray-700 bg-gray-700">
              <td className="py-2 px-4 text-cyan-400">Linked List</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">O(1)*</td>
              <td className="py-2 px-4">O(1)*</td>
              <td className="py-2 px-4">Optional</td>
              <td className="py-2 px-4">*Fast insertions/deletions at known positions</td>
            </tr>
            <tr className="border-b border-gray-700">
              <td className="py-2 px-4 text-pink-400 font-bold">BST (balanced)</td>
              <td className="py-2 px-4">O(log n)</td>
              <td className="py-2 px-4">O(log n)</td>
              <td className="py-2 px-4">O(log n)</td>
              <td className="py-2 px-4">✓</td>
              <td className="py-2 px-4">Efficiently maintains sorted order with fast operations</td>
            </tr>
            <tr className="border-b border-gray-700 bg-gray-700">
              <td className="py-2 px-4 text-pink-400">BST (unbalanced)</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">O(n)</td>
              <td className="py-2 px-4">✓</td>
              <td className="py-2 px-4">Degrades to linked list performance in worst case</td>
            </tr>
            <tr className="border-b border-gray-700">
              <td className="py-2 px-4 text-yellow-300">Hash Table</td>
              <td className="py-2 px-4">O(1)*</td>
              <td className="py-2 px-4">O(1)*</td>
              <td className="py-2 px-4">O(1)*</td>
              <td className="py-2 px-4">✗</td>
              <td className="py-2 px-4">*Amortized average case; Fastest for exact match lookups</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div className="mt-6 grid grid-cols-1 md:grid-cols-2 gap-4">
        <div className="bg-gray-700 p-3 rounded-lg">
          <h3 className="font-bold text-cyan-400 mb-2">When to Choose BSTs</h3>
          <ul className="space-y-1 text-gray-300 text-sm">
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-green-400">✓</div>
              <div>Need to maintain sorted order</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-green-400">✓</div>
              <div>Require range queries (find all between x and y)</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-green-400">✓</div>
              <div>Need to find successor/predecessor elements</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-green-400">✓</div>
              <div>Regular insertions and deletions in sorted data</div>
            </li>
          </ul>
        </div>
        
        <div className="bg-gray-700 p-3 rounded-lg">
          <h3 className="font-bold text-pink-400 mb-2">BST Limitations</h3>
          <ul className="space-y-1 text-gray-300 text-sm">
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-red-400">✗</div>
              <div>Standard BSTs can become unbalanced</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-red-400">✗</div>
              <div>More complex than arrays or linked lists</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-red-400">✗</div>
              <div>Higher constant factors than arrays (pointer overhead)</div>
            </li>
            <li className="flex items-start">
              <div className="mt-1 mr-2 text-red-400">✗</div>
              <div>Slower than hash tables for simple lookups</div>
            </li>
          </ul>
        </div>
      </div>
    </div>
  );
};

// Time & Space Complexity Table
const ComplexityTable = () => {
  const operations = [
    { name: "Search", average: "O(log n)", worst: "O(n)", space: "O(h)" },
    { name: "Insert", average: "O(log n)", worst: "O(n)", space: "O(h)" },
    { name: "Delete", average: "O(log n)", worst: "O(n)", space: "O(h)" },
    { name: "In-Order Traversal", average: "O(n)", worst: "O(n)", space: "O(h)" },
    { name: "Find Min/Max", average: "O(log n)", worst: "O(n)", space: "O(h)" },
    { name: "Successor/Predecessor", average: "O(log n)", worst: "O(n)", space: "O(h)" }
  ];

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-green-400">
      <h2 className="text-xl font-bold text-green-400 mb-4">Time & Space Complexity</h2>
      
      <div className="overflow-x-auto">
        <table className="w-full text-left border-collapse">
          <thead>
            <tr className="border-b border-gray-700">
              <th className="py-2 px-4 text-cyan-400">Operation</th>
              <th className="py-2 px-4 text-pink-400">Average Case</th>
              <th className="py-2 px-4 text-yellow-300">Worst Case</th>
              <th className="py-2 px-4 text-green-400">Space</th>
              <th className="py-2 px-4 text-gray-300">Notes</th>
            </tr>
          </thead>
          <tbody>
            {operations.map((op, index) => (
              <tr key={index} className={index % 2 === 0 ? 'border-b border-gray-700' : 'border-b border-gray-700 bg-gray-700'}>
                <td className="py-2 px-4 text-gray-300">{op.name}</td>
                <td className="py-2 px-4 text-pink-400 font-mono">{op.average}</td>
                <td className="py-2 px-4 text-yellow-300 font-mono">{op.worst}</td>
                <td className="py-2 px-4 text-green-400 font-mono">{op.space}</td>
                <td className="py-2 px-4 text-gray-400 text-sm">
                  {op.name === "Search" && "Logarithmic for balanced trees"}
                  {op.name === "Insert" && "Follows the same path as search"}
                  {op.name === "Delete" && "Most complex operation due to restructuring"}
                  {op.name === "In-Order Traversal" && "Always visits all nodes"}
                  {op.name === "Find Min/Max" && "Just follow left/right pointers"}
                  {op.name === "Successor/Predecessor" && "May need to traverse up from a leaf"}
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      
      <div className="mt-4 bg-gray-700 p-3 rounded-lg">
        <h3 className="font-bold text-pink-400 mb-2">Key Complexity Factors</h3>
        <ul className="space-y-2 text-gray-300 text-sm">
          <li className="flex items-start">
            <div className="mt-1 mr-2"><BarChart4 size={16} className="text-yellow-300" /></div>
            <div>
              <span className="text-yellow-300 font-bold">h</span> represents the height of the tree:
              <ul className="ml-6 mt-1 space-y-1">
                <li>h = log₂(n) for balanced trees</li>
                <li>h = n for completely unbalanced (degenerate) trees</li>
              </ul>
            </div>
          </li>
          <li className="flex items-start">
            <div className="mt-1 mr-2"><GitMerge size={16} className="text-green-400" /></div>
            <div>
              <span className="text-green-400 font-bold">Space complexity</span> comes from:
              <ul className="ml-6 mt-1 space-y-1">
                <li>O(h) for the recursion stack during operations</li>
                <li>O(n) for storing the tree structure itself</li>
              </ul>
            </div>
          </li>
        </ul>
      </div>
    </div>
  );
};

// BST Meme Component
const BSTMeme = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">The BST Efficiency Meme</h2>
      
      <div className="bg-gray-900 p-4 rounded-lg max-w-xl mx-auto">
        <div className="text-center font-bold text-lg text-white mb-4">
          Binary Search Tree: Expectations vs. Reality
        </div>
        
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <div className="bg-gray-800 p-3 rounded-lg">
            <div className="text-center text-cyan-400 font-bold mb-2">Expectation</div>
            <div className="w-full h-36 relative">
              <svg viewBox="0 0 160 120" className="w-full h-full">
                {/* Perfect BST */}
                <path d="M80,20 L40,40" stroke="#00FFFF" strokeWidth="2" />
                <path d="M80,20 L120,40" stroke="#00FFFF" strokeWidth="2" />
                <path d="M40,40 L20,60" stroke="#00FFFF" strokeWidth="2" />
                <path d="M40,40 L60,60" stroke="#00FFFF" strokeWidth="2" />
                <path d="M120,40 L100,60" stroke="#00FFFF" strokeWidth="2" />
                <path d="M120,40 L140,60" stroke="#00FFFF" strokeWidth="2" />
                <path d="M20,60 L10,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M20,60 L30,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M60,60 L50,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M60,60 L70,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M100,60 L90,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M100,60 L110,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M140,60 L130,80" stroke="#00FFFF" strokeWidth="2" />
                <path d="M140,60 L150,80" stroke="#00FFFF" strokeWidth="2" />
                
                {/* Nodes */}
                <circle cx="80" cy="20" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="40" cy="40" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="120" cy="40" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="20" cy="60" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="60" cy="60" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="100" cy="60" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="140" cy="60" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="10" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="30" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="50" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="70" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="90" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="110" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="130" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                <circle cx="150" cy="80" r="8" fill="#121212" stroke="#00FFFF" strokeWidth="2" />
                
                <text x="80" y="104" textAnchor="middle" fill="#00FFFF" fontSize="12">Perfect Balance: O(log n)</text>
              </svg>
            </div>
          </div>
          
          <div className="bg-gray-800 p-3 rounded-lg">
            <div className="text-center text-red-400 font-bold mb-2">Reality</div>
            <div className="w-full h-36 relative">
              <svg viewBox="0 0 160 120" className="w-full h-full">
                {/* Degenerate BST (essentially a linked list) */}
                <path d="M20,20 L40,40" stroke="#FF5555" strokeWidth="2" />
                <path d="M40,40 L60,60" stroke="#FF5555" strokeWidth="2" />
                <path d="M60,60 L80,80" stroke="#FF5555" strokeWidth="2" />
                <path d="M80,80 L100,100" stroke="#FF5555" strokeWidth="2" />
                
                {/* Nodes */}
                <circle cx="20" cy="20" r="8" fill="#121212" stroke="#FF5555" strokeWidth="2" />
                <circle cx="40" cy="40" r="8" fill="#121212" stroke="#FF5555" strokeWidth="2" />
                <circle cx="60" cy="60" r="8" fill="#121212" stroke="#FF5555" strokeWidth="2" />
                <circle cx="80" cy="80" r="8" fill="#121212" stroke="#FF5555" strokeWidth="2" />
                <circle cx="100" cy="100" r="8" fill="#121212" stroke="#FF5555" strokeWidth="2" />
                
                <text x="80" y="20" textAnchor="middle" fill="#FF5555" fontSize="12">After sequential inserts:</text>
                <text x="80" y="104" textAnchor="middle" fill="#FF5555" fontSize="12">Degenerate Tree: O(n)</text>
              </svg>
            </div>
          </div>
        </div>
        
        <div className="mt-4 text-center text-gray-300">
          <p>
            Without balancing, a BST can degenerate into a linked list if data is inserted in sorted order.
          </p>
          <p className="mt-2 text-yellow-300">
            Coming tomorrow: How AVL trees solve this problem! 🌲
          </p>
        </div>
        
        <div className="mt-4 text-center text-gray-500 text-sm">
          #DSAin45 #BinarySearchTrees #BalanceMatters
        </div>
      </div>
    </div>
  );
};

export default BinarySearchTreeVisualizations;
