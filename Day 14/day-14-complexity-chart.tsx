import React from 'react';

const ComplexityComparison = () => {
  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    text: '#FFFFFF',
    darkGlass: 'rgba(0, 0, 0, 0.5)'
  };

  // Styles
  const containerStyle = {
    backgroundColor: colors.background,
    padding: '20px',
    fontFamily: 'monospace',
    color: colors.text,
    borderRadius: '8px',
    width: '100%',
    maxWidth: '900px',
    margin: '0 auto'
  };

  const headerStyle = {
    textAlign: 'center',
    color: colors.yellow,
    marginBottom: '20px',
    padding: '10px',
    borderBottom: `2px solid ${colors.yellow}`
  };

  const tableContainerStyle = {
    backgroundColor: colors.darkGlass,
    borderRadius: '8px',
    padding: '15px',
    marginBottom: '20px',
    overflowX: 'auto'
  };

  const tableStyle = {
    width: '100%',
    borderCollapse: 'collapse',
    color: colors.text
  };

  const thStyle = {
    padding: '10px',
    textAlign: 'left',
    borderBottom: `2px solid ${colors.yellow}`
  };

  const tdStyle = {
    padding: '10px',
    borderBottom: `1px solid rgba(255, 255, 255, 0.2)`
  };

  const sectionHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    marginBottom: '15px',
    marginTop: '20px'
  };

  const cardContainerStyle = {
    display: 'grid',
    gridTemplateColumns: 'repeat(auto-fill, minmax(280px, 1fr))',
    gap: '15px',
    marginTop: '20px'
  };

  const cardStyle = {
    backgroundColor: colors.darkGlass,
    borderRadius: '8px',
    padding: '15px',
    border: `1px solid ${colors.pink}`
  };

  const cardHeaderStyle = {
    color: colors.pink,
    borderBottom: `1px solid ${colors.pink}`,
    paddingBottom: '5px',
    marginBottom: '10px',
    fontSize: '16px'
  };

  const listStyle = {
    paddingLeft: '20px',
    margin: '10px 0'
  };

  const listItemStyle = {
    margin: '5px 0'
  };

  const complexityCellStyle = (complexity) => {
    let color;
    if (complexity.includes('O(1)')) {
      color = colors.green;
    } else if (complexity.includes('O(log n)')) {
      color = colors.cyan;
    } else if (complexity.includes('O(n)')) {
      color = colors.yellow;
    } else {
      color = colors.pink;
    }
    
    return {
      ...tdStyle,
      color: color,
      fontWeight: 'bold'
    };
  };

  const footerStyle = {
    textAlign: 'center',
    marginTop: '30px',
    color: colors.yellow,
    fontStyle: 'italic'
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Data Structure Complexity Comparison</h1>
      
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Time Complexity Comparison</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Data Structure</th>
              <th style={thStyle}>Access</th>
              <th style={thStyle}>Search</th>
              <th style={thStyle}>Insertion</th>
              <th style={thStyle}>Deletion</th>
              <th style={thStyle}>Notes</th>
            </tr>
          </thead>
          <tbody>
            {/* Array */}
            <tr>
              <td style={tdStyle}>Array</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>Random access, insertion requires shifting</td>
            </tr>
            
            {/* Vector */}
            <tr>
              <td style={tdStyle}>Vector</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n) / O(1)')}>O(n) / O(1)*</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>*O(1) amortized for push_back</td>
            </tr>
            
            {/* Singly Linked List */}
            <tr>
              <td style={tdStyle}>Singly Linked List</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)**</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)**</td>
              <td style={tdStyle}>**O(1) at head, O(n) elsewhere</td>
            </tr>
            
            {/* Doubly Linked List */}
            <tr>
              <td style={tdStyle}>Doubly Linked List</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)***</td>
              <td style={complexityCellStyle('O(1)')}>O(1)***</td>
              <td style={tdStyle}>***With pointer to position</td>
            </tr>
            
            {/* Stack */}
            <tr>
              <td style={tdStyle}>Stack</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={tdStyle}>Push/pop at one end only</td>
            </tr>
            
            {/* Queue */}
            <tr>
              <td style={tdStyle}>Queue</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={tdStyle}>Operations at both ends</td>
            </tr>
            
            {/* Deque */}
            <tr>
              <td style={tdStyle}>Deque</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={complexityCellStyle('O(1)')}>O(1)</td>
              <td style={tdStyle}>Fast operations at both ends</td>
            </tr>
            
            {/* Hash Table */}
            <tr>
              <td style={tdStyle}>Hash Table</td>
              <td style={complexityCellStyle('N/A')}>N/A</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={tdStyle}>****O(1) average, O(n) worst-case</td>
            </tr>
            
            {/* Set/Map (BST-based) */}
            <tr>
              <td style={tdStyle}>Set/Map (BST)</td>
              <td style={complexityCellStyle('N/A')}>N/A</td>
              <td style={complexityCellStyle('O(log n)')}>O(log n)</td>
              <td style={complexityCellStyle('O(log n)')}>O(log n)</td>
              <td style={complexityCellStyle('O(log n)')}>O(log n)</td>
              <td style={tdStyle}>Ordered operations available</td>
            </tr>
            
            {/* Unordered Set/Map */}
            <tr>
              <td style={tdStyle}>Unordered Set/Map</td>
              <td style={complexityCellStyle('N/A')}>N/A</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={complexityCellStyle('O(1) / O(n)')}>O(1) / O(n)****</td>
              <td style={tdStyle}>Hash-based implementation</td>
            </tr>
            
            {/* Priority Queue */}
            <tr>
              <td style={tdStyle}>Priority Queue</td>
              <td style={complexityCellStyle('O(1)')}>O(1)*****</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={complexityCellStyle('O(log n)')}>O(log n)</td>
              <td style={complexityCellStyle('O(log n)')}>O(log n)</td>
              <td style={tdStyle}>*****Top element only</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Space Complexity Comparison</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Data Structure</th>
              <th style={thStyle}>Space Complexity</th>
              <th style={thStyle}>Overhead Notes</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Array</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>Contiguous memory, no overhead per element</td>
            </tr>
            <tr>
              <td style={tdStyle}>Vector</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>May have unused capacity (extra space)</td>
            </tr>
            <tr>
              <td style={tdStyle}>Linked List</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>Extra pointer(s) per element</td>
            </tr>
            <tr>
              <td style={tdStyle}>Hash Table</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>Potentially wasted space in buckets</td>
            </tr>
            <tr>
              <td style={tdStyle}>Set/Map (BST)</td>
              <td style={complexityCellStyle('O(n)')}>O(n)</td>
              <td style={tdStyle}>Pointers for tree structure</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <h2 style={sectionHeaderStyle}>When to Use Each Data Structure</h2>
      
      <div style={cardContainerStyle}>
        {/* Array/Vector */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Array/Vector</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>Fast random access by index</li>
            <li style={listItemStyle}>Simple, contiguous memory</li>
            <li style={listItemStyle}>Frequent traversals</li>
            <li style={listItemStyle}>Cache-friendly operations</li>
          </ul>
          <p>Examples: Game board, image pixels</p>
        </div>
        
        {/* Linked List */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Linked List</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>Frequent insertions/deletions</li>
            <li style={listItemStyle}>No shifting of elements</li>
            <li style={listItemStyle}>Dynamic memory allocation</li>
            <li style={listItemStyle}>Implementing other data structures</li>
          </ul>
          <p>Examples: LRU caches, undo functionality</p>
        </div>
        
        {/* Stack */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Stack</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>LIFO (Last In, First Out) processing</li>
            <li style={listItemStyle}>Function call tracking</li>
            <li style={listItemStyle}>Expression evaluation/parsing</li>
            <li style={listItemStyle}>Backtracking algorithms</li>
          </ul>
          <p>Examples: Syntax parsing, depth-first search</p>
        </div>
        
        {/* Queue */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Queue</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>FIFO (First In, First Out) processing</li>
            <li style={listItemStyle}>Order preservation</li>
            <li style={listItemStyle}>Asynchronous data transfer</li>
            <li style={listItemStyle}>Buffering</li>
          </ul>
          <p>Examples: Print queues, breadth-first search</p>
        </div>
        
        {/* Hash Table */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Hash Tables</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>Fast lookups by key</li>
            <li style={listItemStyle}>Fast insertions and deletions</li>
            <li style={listItemStyle}>No sorting requirements</li>
            <li style={listItemStyle}>Implementing caches</li>
          </ul>
          <p>Examples: Dictionaries, symbol tables, counters</p>
        </div>
        
        {/* Set/Map */}
        <div style={cardStyle}>
          <h3 style={cardHeaderStyle}>Set/Map (BST-based)</h3>
          <p>Best when you need:</p>
          <ul style={listStyle}>
            <li style={listItemStyle}>Ordered iteration</li>
            <li style={listItemStyle}>Range queries</li>
            <li style={listItemStyle}>Reasonably fast operations</li>
            <li style={listItemStyle}>Predecessor/successor operations</li>
          </ul>
          <p>Examples: Ordered dictionaries, range queries</p>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 14: Weekly Challenge & Recap
      </div>
    </div>
  );
};

export default ComplexityComparison;
