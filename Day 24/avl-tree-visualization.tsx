import React, { useState, useEffect } from 'react';

// AVL Tree Node class
class AVLNode {
  constructor(value) {
    this.value = value;
    this.left = null;
    this.right = null;
    this.height = 1;
    this.x = 0;
    this.y = 0;
    this.balanceFactor = 0;
  }
}

// AVL Tree implementation
class AVLTree {
  constructor() {
    this.root = null;
  }
  
  // Get height of a node
  height(node) {
    return node ? node.height : 0;
  }
  
  // Calculate balance factor
  getBalanceFactor(node) {
    return node ? this.height(node.left) - this.height(node.right) : 0;
  }
  
  // Update height and balance factor
  updateHeightAndBF(node) {
    if (!node) return;
    
    node.height = 1 + Math.max(this.height(node.left), this.height(node.right));
    node.balanceFactor = this.getBalanceFactor(node);
  }
  
  // Right rotation
  rightRotate(y) {
    const x = y.left;
    const T2 = x.right;
    
    // Perform rotation
    x.right = y;
    y.left = T2;
    
    // Update heights
    this.updateHeightAndBF(y);
    this.updateHeightAndBF(x);
    
    // Return new root
    return x;
  }
  
  // Left rotation
  leftRotate(x) {
    const y = x.right;
    const T2 = y.left;
    
    // Perform rotation
    y.left = x;
    x.right = T2;
    
    // Update heights
    this.updateHeightAndBF(x);
    this.updateHeightAndBF(y);
    
    // Return new root
    return y;
  }
  
  // Insert a value
  insert(value) {
    this.root = this._insert(this.root, value);
    return this.root;
  }
  
  _insert(node, value) {
    // Standard BST insertion
    if (!node) {
      return new AVLNode(value);
    }
    
    if (value < node.value) {
      node.left = this._insert(node.left, value);
    } else if (value > node.value) {
      node.right = this._insert(node.right, value);
    } else {
      // Duplicate values not allowed
      return node;
    }
    
    // Update height and balance factor
    this.updateHeightAndBF(node);
    
    // Balance the tree if needed
    const balance = node.balanceFactor;
    
    // Left Left Case
    if (balance > 1 && value < node.left.value) {
      return this.rightRotate(node);
    }
    
    // Right Right Case
    if (balance < -1 && value > node.right.value) {
      return this.leftRotate(node);
    }
    
    // Left Right Case
    if (balance > 1 && value > node.left.value) {
      node.left = this.leftRotate(node.left);
      return this.rightRotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && value < node.right.value) {
      node.right = this.rightRotate(node.right);
      return this.leftRotate(node);
    }
    
    // Return unchanged node pointer
    return node;
  }
  
  // Find minimum value node
  minValueNode(node) {
    let current = node;
    
    // Find the leftmost leaf
    while (current && current.left) {
      current = current.left;
    }
    
    return current;
  }
  
  // Delete a node
  delete(value) {
    this.root = this._delete(this.root, value);
    return this.root;
  }
  
  _delete(node, value) {
    // Standard BST delete
    if (!node) {
      return null;
    }
    
    if (value < node.value) {
      node.left = this._delete(node.left, value);
    } else if (value > node.value) {
      node.right = this._delete(node.right, value);
    } else {
      // Node with the value found
      
      // Case 1: Node with no or one child
      if (!node.left || !node.right) {
        const temp = node.left ? node.left : node.right;
        
        // No child case
        if (!temp) {
          node = null;
        } else {
          // One child case
          node = temp;
        }
      } else {
        // Case 2: Node with two children
        const temp = this.minValueNode(node.right);
        
        // Copy the inorder successor's data
        node.value = temp.value;
        
        // Delete the inorder successor
        node.right = this._delete(node.right, temp.value);
      }
    }
    
    // If the tree had only one node then return
    if (!node) {
      return null;
    }
    
    // Update height and balance factor
    this.updateHeightAndBF(node);
    
    // Balance the tree if needed
    const balance = node.balanceFactor;
    
    // Left Left Case
    if (balance > 1 && this.getBalanceFactor(node.left) >= 0) {
      return this.rightRotate(node);
    }
    
    // Left Right Case
    if (balance > 1 && this.getBalanceFactor(node.left) < 0) {
      node.left = this.leftRotate(node.left);
      return this.rightRotate(node);
    }
    
    // Right Right Case
    if (balance < -1 && this.getBalanceFactor(node.right) <= 0) {
      return this.leftRotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && this.getBalanceFactor(node.right) > 0) {
      node.right = this.rightRotate(node.right);
      return this.leftRotate(node);
    }
    
    return node;
  }
  
  // Check if a value exists
  search(value) {
    return this._search(this.root, value);
  }
  
  _search(node, value) {
    if (!node) {
      return false;
    }
    
    if (value === node.value) {
      return true;
    }
    
    if (value < node.value) {
      return this._search(node.left, value);
    } else {
      return this._search(node.right, value);
    }
  }
  
  // In-order traversal
  inorderTraversal() {
    const result = [];
    this._inorderTraversal(this.root, result);
    return result;
  }
  
  _inorderTraversal(node, result) {
    if (node) {
      this._inorderTraversal(node.left, result);
      result.push(node.value);
      this._inorderTraversal(node.right, result);
    }
  }
  
  // Calculate positions for visualization
  calculatePositions(width, height) {
    const setPositions = (node, x, y, level, leftBound, rightBound) => {
      if (!node) return;
      
      node.x = x;
      node.y = y;
      
      const verticalGap = height / 7;
      const nextY = y + verticalGap;
      
      const leftX = (x + leftBound) / 2;
      const rightX = (x + rightBound) / 2;
      
      setPositions(node.left, leftX, nextY, level + 1, leftBound, x);
      setPositions(node.right, rightX, nextY, level + 1, x, rightBound);
    };
    
    if (this.root) {
      setPositions(this.root, width / 2, 80, 0, 0, width);
    }
  }
}

// AVL Tree Visualization Component
const AVLTreeVisualization = () => {
  const [avlTree] = useState(new AVLTree());
  const [inputValue, setInputValue] = useState('');
  const [message, setMessage] = useState('Welcome to AVL Tree Visualization!');
  const [currentTab, setCurrentTab] = useState('visualization');
  const [animationHistory, setAnimationHistory] = useState([]);
  const [currentAnimationStep, setCurrentAnimationStep] = useState(-1);
  const [isAnimating, setIsAnimating] = useState(false);
  const [rotationType, setRotationType] = useState(null);
  const [showBalanceFactors, setShowBalanceFactors] = useState(true);
  const [traversalResult, setTraversalResult] = useState([]);
  
  // Canvas dimensions
  const canvasWidth = 600;
  const canvasHeight = 400;
  
  // Colors
  const bgColor = "#121212";
  const cyanColor = "#00FFFF";
  const pinkColor = "#FF00FF";
  const yellowColor = "#FFFF00";
  const greenColor = "#00FF00";
  
  // Initialize tree with sample values
  useEffect(() => {
    const sampleValues = [10, 20, 30, 5, 15, 25, 35];
    sampleValues.forEach(value => avlTree.insert(value));
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setTraversalResult(avlTree.inorderTraversal());
  }, []);
  
  const handleInsert = () => {
    const value = parseInt(inputValue);
    if (isNaN(value)) {
      setMessage('Please enter a valid number');
      return;
    }
    
    if (avlTree.search(value)) {
      setMessage(`Value ${value} already exists in the tree`);
      return;
    }
    
    avlTree.insert(value);
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setInputValue('');
    setMessage(`Value ${value} inserted successfully`);
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  const handleDelete = () => {
    const value = parseInt(inputValue);
    if (isNaN(value)) {
      setMessage('Please enter a valid number');
      return;
    }
    
    if (!avlTree.search(value)) {
      setMessage(`Value ${value} not found in the tree`);
      return;
    }
    
    avlTree.delete(value);
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setInputValue('');
    setMessage(`Value ${value} deleted successfully`);
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  const resetTree = () => {
    const newTree = new AVLTree();
    const sampleValues = [10, 20, 30, 5, 15, 25, 35];
    sampleValues.forEach(value => newTree.insert(value));
    
    // Update the reference
    Object.assign(avlTree, newTree);
    
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setInputValue('');
    setMessage('Tree reset to default');
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  const showLeftRightRotation = () => {
    // Create a tree for the LR rotation example
    resetTree();
    const rotationTree = new AVLTree();
    rotationTree.insert(30);
    rotationTree.insert(20);
    rotationTree.insert(25);
    
    // Update the reference
    Object.assign(avlTree, rotationTree);
    
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setRotationType('LR');
    setMessage('Demonstrating Left-Right Rotation');
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  const showRightLeftRotation = () => {
    // Create a tree for the RL rotation example
    resetTree();
    const rotationTree = new AVLTree();
    rotationTree.insert(10);
    rotationTree.insert(20);
    rotationTree.insert(15);
    
    // Update the reference
    Object.assign(avlTree, rotationTree);
    
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setRotationType('RL');
    setMessage('Demonstrating Right-Left Rotation');
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  const createUnbalancedTree = () => {
    // Create a tree that will need balancing
    resetTree();
    const unbalancedTree = new AVLTree();
    
    // Insert values in ascending order (which would make a regular BST skewed)
    [1, 2, 3, 4, 5, 6, 7].forEach(value => {
      unbalancedTree.insert(value);
    });
    
    // Update the reference
    Object.assign(avlTree, unbalancedTree);
    
    avlTree.calculatePositions(canvasWidth, canvasHeight);
    setMessage('Created a balanced tree from sequential insertions');
    setTraversalResult(avlTree.inorderTraversal());
  };
  
  // Draw the tree on the canvas
  useEffect(() => {
    const canvas = document.getElementById('avl-canvas');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Fill background
    ctx.fillStyle = bgColor;
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    
    // Draw title
    ctx.font = "24px Arial";
    ctx.fillStyle = pinkColor;
    ctx.textAlign = "center";
    ctx.fillText("AVL Tree Visualization", canvas.width / 2, 30);
    
    // Draw nodes and connections
    const drawNode = (node) => {
      if (!node) return;
      
      // Draw connections to children
      if (node.left) {
        ctx.beginPath();
        ctx.moveTo(node.x, node.y);
        ctx.lineTo(node.left.x, node.left.y);
        ctx.strokeStyle = greenColor;
        ctx.lineWidth = 2;
        ctx.stroke();
      }
      
      if (node.right) {
        ctx.beginPath();
        ctx.moveTo(node.x, node.y);
        ctx.lineTo(node.right.x, node.right.y);
        ctx.strokeStyle = greenColor;
        ctx.lineWidth = 2;
        ctx.stroke();
      }
      
      // Determine node color based on balance factor
      let nodeColor = cyanColor;
      
      if (node.balanceFactor > 1 || node.balanceFactor < -1) {
        // Unbalanced node
        nodeColor = pinkColor;
      } else if (node.balanceFactor === 1 || node.balanceFactor === -1) {
        // Slightly unbalanced but within AVL constraints
        nodeColor = yellowColor;
      } else {
        // Perfectly balanced
        nodeColor = cyanColor;
      }
      
      // Draw node circle
      ctx.beginPath();
      ctx.arc(node.x, node.y, 25, 0, Math.PI * 2);
      ctx.fillStyle = nodeColor;
      ctx.fill();
      ctx.strokeStyle = "#333";
      ctx.lineWidth = 2;
      ctx.stroke();
      
      // Draw node value
      ctx.font = "16px Arial";
      ctx.fillStyle = "#000";
      ctx.textAlign = "center";
      ctx.textBaseline = "middle";
      ctx.fillText(node.value.toString(), node.x, node.y);
      
      // Draw balance factor if enabled
      if (showBalanceFactors) {
        ctx.font = "12px Arial";
        ctx.fillStyle = "#000";
        ctx.textAlign = "center";
        ctx.fillText(`BF: ${node.balanceFactor}`, node.x, node.y + 30);
      }
      
      // Continue with children
      drawNode(node.left);
      drawNode(node.right);
    };
    
    // Start drawing from root
    drawNode(avlTree.root);
    
  });
  
  const renderComplexityTab = () => {
    return (
      <div style={{ color: '#fff', padding: '15px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree Time Complexity
        </h3>
        
        <div style={{ 
          display: 'grid', 
          gridTemplateColumns: '1fr 1fr',
          gap: '15px',
          marginBottom: '20px'
        }}>
          <div style={{ 
            border: '1px solid #333', 
            borderRadius: '6px', 
            padding: '15px'
          }}>
            <h4 style={{ color: cyanColor, marginBottom: '10px' }}>AVL Trees</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Search: O(log n)</li>
              <li style={{ marginBottom: '8px' }}>• Insert: O(log n)</li>
              <li style={{ marginBottom: '8px' }}>• Delete: O(log n)</li>
              <li>• Traversal: O(n)</li>
            </ul>
          </div>
          
          <div style={{ 
            border: '1px solid #333', 
            borderRadius: '6px', 
            padding: '15px'
          }}>
            <h4 style={{ color: yellowColor, marginBottom: '10px' }}>Regular BST (Worst Case)</h4>
            <ul style={{ listStyleType: 'none', padding: 0 }}>
              <li style={{ marginBottom: '8px' }}>• Search: O(n)</li>
              <li style={{ marginBottom: '8px' }}>• Insert: O(n)</li>
              <li style={{ marginBottom: '8px' }}>• Delete: O(n)</li>
              <li>• Traversal: O(n)</li>
            </ul>
          </div>
        </div>
        
        <div style={{ marginBottom: '20px' }}>
          <h4 style={{ color: greenColor, marginBottom: '10px' }}>AVL Tree Height</h4>
          <p>
            The height of an AVL tree with n nodes is guaranteed to be O(log n).
            More precisely: height ≤ 1.44 × log₂(n + 2) - 0.328
          </p>
        </div>
        
        <div style={{ marginBottom: '20px' }}>
          <h4 style={{ color: pinkColor, marginBottom: '10px' }}>Balance Factor</h4>
          <p>
            For any node, Balance Factor = Height(Left Subtree) - Height(Right Subtree)
          </p>
          <p>
            In an AVL tree, this value must always be in {-1, 0, 1} for every node.
          </p>
        </div>
        
        <div>
          <h4 style={{ color: cyanColor, marginBottom: '10px' }}>Rotation Types</h4>
          <ul style={{ listStyleType: 'none', padding: 0 }}>
            <li style={{ marginBottom: '8px' }}>• Left Rotation (RR Imbalance)</li>
            <li style={{ marginBottom: '8px' }}>• Right Rotation (LL Imbalance)</li>
            <li style={{ marginBottom: '8px' }}>• Left-Right Rotation (LR Imbalance)</li>
            <li>• Right-Left Rotation (RL Imbalance)</li>
          </ul>
        </div>
      </div>
    );
  };
  
  const renderInfoTab = () => {
    return (
      <div style={{ color: '#fff', padding: '15px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree: Key Concepts
        </h3>
        
        <div style={{ marginBottom: '20px' }}>
          <h4 style={{ color: cyanColor, marginBottom: '10px' }}>What are AVL Trees?</h4>
          <p>
            AVL trees are self-balancing binary search trees where the height difference between left and right 
            subtrees (balance factor) of any node cannot exceed 1. They were the first self-balancing BST structure 
            invented, named after their creators Adelson-Velsky and Landis.
          </p>
        </div>
        
        <div style={{ marginBottom: '20px' }}>
          <h4 style={{ color: yellowColor, marginBottom: '10px' }}>Why Use AVL Trees?</h4>
          <p>
            Regular BSTs can become skewed with certain insertion patterns, degrading to O(n) performance.
            AVL trees guarantee O(log n) operations through automatic rotations that maintain balance.
          </p>
        </div>
        
        <div style={{ marginBottom: '20px' }}>
          <h4 style={{ color: greenColor, marginBottom: '10px' }}>How AVL Trees Work</h4>
          <ul style={{ listStyleType: 'none', padding: 0 }}>
            <li style={{ marginBottom: '8px' }}>• Each node stores its height or balance factor</li>
            <li style={{ marginBottom: '8px' }}>• After insertion/deletion, balance is checked</li>
            <li style={{ marginBottom: '8px' }}>• If imbalanced, appropriate rotations are performed</li>
            <li>• Different rotation types handle different imbalance patterns</li>
          </ul>
        </div>
        
        <div>
          <h4 style={{ color: pinkColor, marginBottom: '10px' }}>Applications</h4>
          <ul style={{ listStyleType: 'none', padding: 0 }}>
            <li style={{ marginBottom: '8px' }}>• Database indexing</li>
            <li style={{ marginBottom: '8px' }}>• Memory management systems</li>
            <li style={{ marginBottom: '8px' }}>• File systems</li>
            <li>• Network routing tables</li>
          </ul>
        </div>
      </div>
    );
  };
  
  return (
    <div style={{ 
      backgroundColor: bgColor, 
      width: '100%', 
      minHeight: '700px',
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      padding: '20px'
    }}>
      {/* Main container */}
      <div style={{ 
        width: '100%', 
        maxWidth: '700px',
        backgroundColor: '#1a1a1a',
        borderRadius: '8px',
        overflow: 'hidden',
        boxShadow: '0 4px 6px rgba(0, 0, 0, 0.3)'
      }}>
        {/* Tabs */}
        <div style={{ 
          display: 'flex', 
          borderBottom: '1px solid #333'
        }}>
          <button 
            onClick={() => setCurrentTab('visualization')} 
            style={{ 
              padding: '10px 15px',
              backgroundColor: currentTab === 'visualization' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: currentTab === 'visualization' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: currentTab === 'visualization' ? 'bold' : 'normal'
            }}
          >
            Visualization
          </button>
          <button 
            onClick={() => setCurrentTab('complexity')}
            style={{ 
              padding: '10px 15px',
              backgroundColor: currentTab === 'complexity' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: currentTab === 'complexity' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: currentTab === 'complexity' ? 'bold' : 'normal'
            }}
          >
            Complexity
          </button>
          <button 
            onClick={() => setCurrentTab('info')}
            style={{ 
              padding: '10px 15px',
              backgroundColor: currentTab === 'info' ? '#2a2a2a' : 'transparent',
              border: 'none',
              color: currentTab === 'info' ? '#fff' : '#999',
              cursor: 'pointer',
              fontWeight: currentTab === 'info' ? 'bold' : 'normal'
            }}
          >
            Info
          </button>
        </div>
        
        {/* Content area */}
        <div>
          {currentTab === 'visualization' ? (
            <div>
              {/* Canvas for visualization */}
              <canvas 
                id="avl-canvas" 
                width={canvasWidth} 
                height={canvasHeight}
                style={{ display: 'block', margin: '0 auto' }}
              ></canvas>
              
              {/* Controls */}
              <div style={{ padding: '15px' }}>
                {/* Input and operation buttons */}
                <div style={{ 
                  display: 'flex', 
                  justifyContent: 'space-between',
                  marginBottom: '15px'
                }}>
                  <div style={{ 
                    display: 'flex', 
                    gap: '10px'
                  }}>
                    <input
                      type="text"
                      value={inputValue}
                      onChange={(e) => setInputValue(e.target.value)}
                      placeholder="Enter a number"
                      style={{
                        padding: '8px 12px',
                        backgroundColor: '#2a2a2a',
                        color: '#fff',
                        border: '1px solid #444',
                        borderRadius: '4px',
                        width: '120px'
                      }}
                    />
                    
                    <button
                      onClick={handleInsert}
                      style={{
                        padding: '8px 12px',
                        backgroundColor: '#2a2a2a',
                        color: greenColor,
                        border: '1px solid #444',
                        borderRadius: '4px',
                        cursor: 'pointer'
                      }}
                    >
                      Insert
                    </button>
                    
                    <button
                      onClick={handleDelete}
                      style={{
                        padding: '8px 12px',
                        backgroundColor: '#2a2a2a',
                        color: pinkColor,
                        border: '1px solid #444',
                        borderRadius: '4px',
                        cursor: 'pointer'
                      }}
                    >
                      Delete
                    </button>
                  </div>
                  
                  <button
                    onClick={resetTree}
                    style={{
                      padding: '8px 12px',
                      backgroundColor: '#2a2a2a',
                      color: yellowColor,
                      border: '1px solid #444',
                      borderRadius: '4px',
                      cursor: 'pointer'
                    }}
                  >
                    Reset Tree
                  </button>
                </div>
                
                {/* Message display */}
                <div style={{
                  padding: '8px 12px',
                  backgroundColor: '#2a2a2a',
                  color: '#fff',
                  borderRadius: '4px',
                  marginBottom: '15px',
                  minHeight: '20px'
                }}>
                  {message}
                </div>
                
                {/* Demonstration controls */}
                <div style={{ 
                  display: 'flex', 
                  justifyContent: 'space-between',
                  marginBottom: '15px'
                }}>
                  <div style={{ 
                    display: 'flex', 
                    gap: '10px'
                  }}>
                    <button
                      onClick={showLeftRightRotation}
                      style={{
                        padding: '8px 12px',
                        backgroundColor: '#2a2a2a',
                        color: cyanColor,
                        border: '1px solid #444',
                        borderRadius: '4px',
                        cursor: 'pointer'
                      }}
                    >
                      LR Rotation
                    </button>
                    
                    <button
                      onClick={showRightLeftRotation}
                      style={{
                        padding: '8px 12px',
                        backgroundColor: '#2a2a2a',
                        color: cyanColor,
                        border: '1px solid #444',
                        borderRadius: '4px',
                        cursor: 'pointer'
                      }}
                    >
                      RL Rotation
                    </button>
                  </div>
                  
                  <button
                    onClick={createUnbalancedTree}
                    style={{
                      padding: '8px 12px',
                      backgroundColor: '#2a2a2a',
                      color: yellowColor,
                      border: '1px solid #444',
                      borderRadius: '4px',
                      cursor: 'pointer'
                    }}
                  >
                    Sequential Insertions
                  </button>
                </div>
                
                {/* Traversal and options */}
                <div style={{ 
                  display: 'flex', 
                  justifyContent: 'space-between',
                  alignItems: 'center'
                }}>
                  <div style={{
                    flex: 1,
                    padding: '8px 12px',
                    backgroundColor: '#2a2a2a',
                    color: '#fff',
                    borderRadius: '4px',
                    overflowX: 'auto',
                    whiteSpace: 'nowrap',
                    marginRight: '10px'
                  }}>
                    <span style={{ color: '#999', marginRight: '10px' }}>Inorder:</span>
                    {traversalResult.join(' → ')}
                  </div>
                  
                  <div style={{ 
                    display: 'flex', 
                    alignItems: 'center',
                    gap: '5px'
                  }}>
                    <input
                      type="checkbox"
                      checked={showBalanceFactors}
                      onChange={() => setShowBalanceFactors(!showBalanceFactors)}
                      id="show-bf"
                    />
                    <label 
                      htmlFor="show-bf"
                      style={{ color: '#fff' }}
                    >
                      Show Balance Factors
                    </label>
                  </div>
                </div>
              </div>
            </div>
          ) : currentTab === 'complexity' ? (
            renderComplexityTab()
          ) : (
            renderInfoTab()
          )}
        </div>
      </div>
      
      {/* Footer */}
      <div style={{ 
        marginTop: '15px',
        textAlign: 'center',
        color: '#999',
        fontSize: '14px'
      }}>
        <div style={{ color: yellowColor, marginBottom: '5px' }}>
          AVL Trees - Day 24 of #DSAin45
        </div>
        <div>
          Self-balancing Binary Search Trees with guaranteed O(log n) operations
        </div>
      </div>
    </div>
  );
};

export default AVLTreeVisualization;