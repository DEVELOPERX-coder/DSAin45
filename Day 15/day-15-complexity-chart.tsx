import React from 'react';

const RecursionComplexityChart = () => {
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
    marginBottom: '30px'
  };

  const tableContainerStyle = {
    backgroundColor: colors.darkGlass,
    borderRadius: '8px',
    padding: '15px',
    marginBottom: '30px',
    overflowX: 'auto'
  };

  const tableStyle = {
    width: '100%',
    borderCollapse: 'collapse',
    color: colors.text
  };

  const thStyle = {
    padding: '12px',
    textAlign: 'left',
    borderBottom: `2px solid ${colors.yellow}`
  };

  const tdStyle = {
    padding: '10px',
    borderBottom: `1px solid rgba(255, 255, 255, 0.2)`
  };

  const highlightTdStyle = {
    ...tdStyle,
    fontWeight: 'bold'
  };

  const sectionHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    marginBottom: '15px',
    marginTop: '20px'
  };

  const warningBoxStyle = {
    backgroundColor: 'rgba(255, 255, 0, 0.1)',
    border: `1px solid ${colors.yellow}`,
    borderRadius: '6px',
    padding: '15px',
    margin: '20px 0'
  };

  const infoBoxStyle = {
    backgroundColor: 'rgba(0, 255, 255, 0.1)',
    border: `1px solid ${colors.cyan}`,
    borderRadius: '6px',
    padding: '15px',
    margin: '20px 0'
  };

  const codeBlockStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    padding: '12px',
    borderRadius: '4px',
    fontFamily: 'monospace',
    fontSize: '14px',
    overflowX: 'auto',
    border: `1px solid ${colors.green}`,
    marginTop: '10px'
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Recursion Time & Space Complexity Analysis</h1>
      
      {/* Common Recursion Examples */}
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Common Recursive Algorithms</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Algorithm</th>
              <th style={thStyle}>Time Complexity</th>
              <th style={thStyle}>Space Complexity</th>
              <th style={thStyle}>Notes</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Factorial</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Linear recursion depth, n calls</td>
            </tr>
            <tr>
              <td style={tdStyle}>Fibonacci (naive)</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Exponential due to redundant calculations</td>
            </tr>
            <tr>
              <td style={tdStyle}>Fibonacci (memoized)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>Dynamic programming eliminates redundancy</td>
            </tr>
            <tr>
              <td style={tdStyle}>Binary Search</td>
              <td style={{...highlightTdStyle, color: colors.cyan}}>O(log n)</td>
              <td style={{...highlightTdStyle, color: colors.cyan}}>O(log n)</td>
              <td style={tdStyle}>Divide and conquer reduces problem size by half</td>
            </tr>
            <tr>
              <td style={tdStyle}>Merge Sort</td>
              <td style={{...highlightTdStyle, color: colors.cyan}}>O(n log n)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>Divide and conquer with linear combining step</td>
            </tr>
            <tr>
              <td style={tdStyle}>Quick Sort</td>
              <td style={{...highlightTdStyle, color: colors.cyan}}>O(n log n)</td>
              <td style={{...highlightTdStyle, color: colors.cyan}}>O(log n)</td>
              <td style={tdStyle}>Average case. Worst case is O(n²) time</td>
            </tr>
            <tr>
              <td style={tdStyle}>Tree Traversal</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(h)</td>
              <td style={tdStyle}>h = height of tree, worst case O(n) for skewed tree</td>
            </tr>
            <tr>
              <td style={tdStyle}>Tower of Hanoi</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>Exponential time, linear recursion depth</td>
            </tr>
            <tr>
              <td style={tdStyle}>Permutations</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(n!)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>Factorial time due to generating all permutations</td>
            </tr>
            <tr>
              <td style={tdStyle}>Subsets/Powerset</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>Exponential time - generates 2ⁿ subsets</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      {/* Tail Recursion Analysis */}
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Recursion Optimization: Normal vs. Tail Recursion</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Algorithm Version</th>
              <th style={thStyle}>Time Complexity</th>
              <th style={thStyle}>Space Complexity</th>
              <th style={thStyle}>Compiler Optimization</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Normal Factorial</td>
              <td style={tdStyle}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Not optimizable</td>
            </tr>
            <tr>
              <td style={tdStyle}>Tail Factorial</td>
              <td style={tdStyle}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)*</td>
              <td style={tdStyle}>*With tail call optimization</td>
            </tr>
            <tr>
              <td style={tdStyle}>Normal Tree Traversal</td>
              <td style={tdStyle}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(h)</td>
              <td style={tdStyle}>Not optimizable</td>
            </tr>
            <tr>
              <td style={tdStyle}>Tail Tree Traversal</td>
              <td style={tdStyle}>O(n)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)*</td>
              <td style={tdStyle}>*With tail call optimization</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div style={warningBoxStyle}>
        <h3 style={{color: colors.yellow, marginTop: 0}}>⚠️ Important Note on Tail Recursion in C++</h3>
        <p>Unlike some functional languages, C++ does not guarantee tail call optimization (TCO). TCO depends on:</p>
        <ul>
          <li>Compiler implementation</li>
          <li>Optimization flags (usually -O2 or higher)</li>
          <li>Function call conventions (inlining decisions)</li>
        </ul>
        <p>Always test on your target compiler if relying on tail call optimization!</p>
      </div>
      
      {/* Memoization Impact */}
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Impact of Memoization on Recursive Algorithms</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Algorithm</th>
              <th style={thStyle}>Without Memoization</th>
              <th style={thStyle}>With Memoization</th>
              <th style={thStyle}>Space Trade-off</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Fibonacci</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n)</td>
              <td style={tdStyle}>O(n) extra space needed</td>
            </tr>
            <tr>
              <td style={tdStyle}>Binomial Coefficient</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(n²)</td>
              <td style={tdStyle}>O(n²) extra space needed</td>
            </tr>
            <tr>
              <td style={tdStyle}>LCS (Longest Common Subsequence)</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(2ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(m*n)</td>
              <td style={tdStyle}>O(m*n) extra space needed</td>
            </tr>
            <tr>
              <td style={tdStyle}>Edit Distance</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(3ⁿ)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(m*n)</td>
              <td style={tdStyle}>O(m*n) extra space needed</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div style={infoBoxStyle}>
        <h3 style={{color: colors.cyan, marginTop: 0}}>Memoization Implementation Pattern</h3>
        <p>Use a hash map or array to store already computed results:</p>
        <div style={codeBlockStyle}>
          <pre style={{margin: 0}}>
            <code style={{color: colors.text}}>
{`// With plain recursion
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);  // Same subproblems solved repeatedly
}

// With memoization
int fibMemo(int n, unordered_map<int, int>& memo) {
    // Check if already computed
    if (memo.find(n) != memo.end()) return memo[n];
    
    if (n <= 1) return n;
    
    // Compute and store result
    memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
    return memo[n];
}`}
            </code>
          </pre>
        </div>
      </div>
      
      {/* Recursion vs Iteration */}
      <div style={tableContainerStyle}>
        <h2 style={sectionHeaderStyle}>Recursion vs. Iteration: Performance Comparison</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Metric</th>
              <th style={thStyle}>Recursion</th>
              <th style={thStyle}>Iteration</th>
              <th style={thStyle}>Winner</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Memory Usage</td>
              <td style={tdStyle}>Higher (call stack overhead)</td>
              <td style={tdStyle}>Lower (typically constant)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>Iteration</td>
            </tr>
            <tr>
              <td style={tdStyle}>Execution Speed</td>
              <td style={tdStyle}>Slower (function call overhead)</td>
              <td style={tdStyle}>Faster (loop operations)</td>
              <td style={{...highlightTdStyle, color: colors.green}}>Iteration</td>
            </tr>
            <tr>
              <td style={tdStyle}>Code Clarity</td>
              <td style={tdStyle}>Cleaner for naturally recursive problems</td>
              <td style={tdStyle}>Can become complex with nested loops</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>Recursion</td>
            </tr>
            <tr>
              <td style={tdStyle}>Implementation Difficulty</td>
              <td style={tdStyle}>Simpler for problems like tree traversal</td>
              <td style={tdStyle}>Simpler for linear processes</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>Depends</td>
            </tr>
            <tr>
              <td style={tdStyle}>Stack Overflow Risk</td>
              <td style={tdStyle}>High with deep recursion</td>
              <td style={tdStyle}>Very low</td>
              <td style={{...highlightTdStyle, color: colors.green}}>Iteration</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div style={warningBoxStyle}>
        <h3 style={{color: colors.yellow, marginTop: 0}}>The Recursion Rule of Thumb</h3>
        <p>Choose the approach that makes the most sense for the problem at hand:</p>
        <ul style={{marginBottom: 0}}>
          <li><strong>Use recursion when:</strong> The problem is naturally recursive (trees, graphs, divide & conquer), code clarity is paramount, or stack depth is limited.</li>
          <li><strong>Use iteration when:</strong> Performance is critical, memory is constrained, or the problem is naturally iterative (linear processes, loops).</li>
          <li><strong>Consider optimization:</strong> When using recursion in performance-critical code, consider tail recursion or memoization where appropriate.</li>
        </ul>
      </div>
      
      <div style={{textAlign: 'center', marginTop: '30px', color: colors.yellow, fontStyle: 'italic'}}>
        #DSAin45 Day 15: Recursion Fundamentals
      </div>
    </div>
  );
};

export default RecursionComplexityChart;
