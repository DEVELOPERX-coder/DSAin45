import React from 'react';

const RecursionVisualization = () => {
  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    text: '#FFFFFF'
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

  const sectionStyle = {
    marginBottom: '30px',
  };

  const sectionHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    marginBottom: '15px'
  };

  const boxStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.3)',
    borderRadius: '8px',
    padding: '15px',
    marginBottom: '20px',
    border: `1px solid ${colors.pink}`
  };

  const codeBlockStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    padding: '15px',
    borderRadius: '4px',
    fontFamily: 'monospace',
    overflowX: 'auto',
    border: `1px solid ${colors.green}`,
    marginTop: '10px',
    marginBottom: '10px',
  };

  const callStackSectionStyle = {
    display: 'flex',
    flexDirection: 'column',
    gap: '10px',
    marginTop: '20px'
  };

  const stackFrameStyle = (index) => ({
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    border: `1px solid ${index === 0 ? colors.green : colors.pink}`,
    borderRadius: '6px',
    padding: '10px',
    marginBottom: '5px',
    position: 'relative',
  });

  const footerStyle = {
    textAlign: 'center',
    marginTop: '30px',
    color: colors.yellow,
    fontStyle: 'italic'
  };

  const typesGridStyle = {
    display: 'grid',
    gridTemplateColumns: 'repeat(auto-fill, minmax(280px, 1fr))',
    gap: '15px',
    marginTop: '20px'
  };

  const typeCardStyle = {
    backgroundColor: 'rgba(0, 0, 0, 0.3)',
    border: `1px solid ${colors.cyan}`,
    borderRadius: '6px',
    padding: '15px'
  };

  const typeHeaderStyle = {
    color: colors.yellow,
    marginBottom: '10px',
    borderBottom: `1px solid ${colors.yellow}`,
    paddingBottom: '5px'
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Recursion Fundamentals</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Understanding Recursion</h2>
        <div style={boxStyle}>
          <p>Recursion is a method where a function calls itself to solve smaller instances of the same problem until it reaches a simple enough case (base case) that can be solved directly.</p>
          
          <div style={{display: 'flex', justifyContent: 'space-between', flexWrap: 'wrap', gap: '15px'}}>
            <div style={{flex: '1', minWidth: '250px'}}>
              <h3 style={{color: colors.green}}>Essential Components:</h3>
              <ul style={{paddingLeft: '20px'}}>
                <li><strong style={{color: colors.green}}>Base Case:</strong> The terminating scenario that can be answered without recursion</li>
                <li><strong style={{color: colors.pink}}>Recursive Case:</strong> The scenario where the function calls itself with a simpler version of the problem</li>
              </ul>
            </div>
            
            <div style={{flex: '1', minWidth: '250px'}}>
              <h3 style={{color: colors.yellow}}>Recursion Philosophy:</h3>
              <p><em>"Trust that your function will correctly solve smaller subproblems without worrying about how it does it."</em></p>
            </div>
          </div>
          
          <div style={codeBlockStyle}>
            <code style={{color: colors.cyan}}>
              // Factorial example<br/>
              int factorial(int n) &#123;<br/>
              &nbsp;&nbsp;&nbsp;&nbsp;// Base case<br/>
              &nbsp;&nbsp;&nbsp;&nbsp;<span style={{color: colors.green}}>if (n &lt;= 1) return 1;</span><br/><br/>
              &nbsp;&nbsp;&nbsp;&nbsp;// Recursive case<br/>
              &nbsp;&nbsp;&nbsp;&nbsp;<span style={{color: colors.pink}}>return n * factorial(n - 1);</span><br/>
              &#125;
            </code>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>The Call Stack Visualization</h2>
        <p>When a function calls itself, each call creates a new stack frame:</p>
        
        <div style={callStackSectionStyle}>
          {/* Stack representation */}
          <div>
            <div style={{display: 'flex', justifyContent: 'space-between'}}>
              <span style={{color: colors.yellow}}>Call Stack for factorial(5)</span>
              <span style={{color: colors.yellow}}>Return Value</span>
            </div>
            
            <div style={stackFrameStyle(0)}>
              <div style={{display: 'flex', justifyContent: 'space-between'}}>
                <span>factorial(1) <strong style={{color: colors.green}}>[BASE CASE]</strong></span>
                <span style={{color: colors.green}}>returns 1</span>
              </div>
            </div>
            
            <div style={stackFrameStyle(1)}>
              <div style={{display: 'flex', justifyContent: 'space-between'}}>
                <span>factorial(2)</span>
                <span style={{color: colors.pink}}>returns 2*1 = 2</span>
              </div>
            </div>
            
            <div style={stackFrameStyle(2)}>
              <div style={{display: 'flex', justifyContent: 'space-between'}}>
                <span>factorial(3)</span>
                <span style={{color: colors.pink}}>returns 3*2 = 6</span>
              </div>
            </div>
            
            <div style={stackFrameStyle(3)}>
              <div style={{display: 'flex', justifyContent: 'space-between'}}>
                <span>factorial(4)</span>
                <span style={{color: colors.pink}}>returns 4*6 = 24</span>
              </div>
            </div>
            
            <div style={stackFrameStyle(4)}>
              <div style={{display: 'flex', justifyContent: 'space-between'}}>
                <span>factorial(5) <strong style={{color: colors.yellow}}>[INITIAL CALL]</strong></span>
                <span style={{color: colors.pink}}>returns 5*24 = 120</span>
              </div>
            </div>
            
            <div style={{textAlign: 'center', marginTop: '10px', fontSize: '12px'}}>
              Stack grows downward, then unwinds upward as calls complete
            </div>
          </div>
          
          <div style={{marginTop: '20px', textAlign: 'center'}}>
            <div style={{display: 'inline-block', textAlign: 'left', backgroundColor: 'rgba(0, 0, 0, 0.3)', padding: '15px', borderRadius: '8px'}}>
              <div><strong style={{color: colors.green}}>Each Stack Frame Contains:</strong></div>
              <ul style={{paddingLeft: '20px', marginTop: '5px'}}>
                <li>Parameters (e.g., n=5)</li>
                <li>Local variables</li>
                <li>Return address</li>
                <li>Return value (when computed)</li>
              </ul>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Types of Recursion</h2>
        
        <div style={typesGridStyle}>
          <div style={typeCardStyle}>
            <h3 style={typeHeaderStyle}>Linear Recursion</h3>
            <p>A function makes a single recursive call in each execution.</p>
            <div style={codeBlockStyle}>
              <code style={{color: colors.text}}>
                int factorial(int n) &#123;<br/>
                &nbsp;&nbsp;if (n &lt;= 1) return 1;<br/>
                &nbsp;&nbsp;return <span style={{color: colors.pink}}>n * factorial(n-1);</span><br/>
                &#125;
              </code>
            </div>
          </div>
          
          <div style={typeCardStyle}>
            <h3 style={typeHeaderStyle}>Binary Recursion</h3>
            <p>A function makes two recursive calls in each execution.</p>
            <div style={codeBlockStyle}>
              <code style={{color: colors.text}}>
                int fibonacci(int n) &#123;<br/>
                &nbsp;&nbsp;if (n &lt;= 1) return n;<br/>
                &nbsp;&nbsp;return <span style={{color: colors.pink}}>fibonacci(n-1) +<br/>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;fibonacci(n-2);</span><br/>
                &#125;
              </code>
            </div>
          </div>
          
          <div style={typeCardStyle}>
            <h3 style={typeHeaderStyle}>Tail Recursion</h3>
            <p>Recursive call is the last operation, can be optimized by compilers.</p>
            <div style={codeBlockStyle}>
              <code style={{color: colors.text}}>
                int factorial(int n, int acc=1) &#123;<br/>
                &nbsp;&nbsp;if (n &lt;= 1) return acc;<br/>
                &nbsp;&nbsp;return <span style={{color: colors.pink}}>factorial(n-1, n*acc);</span><br/>
                &#125;
              </code>
            </div>
          </div>
          
          <div style={typeCardStyle}>
            <h3 style={typeHeaderStyle}>Mutual Recursion</h3>
            <p>Two or more functions call each other in a cycle.</p>
            <div style={codeBlockStyle}>
              <code style={{color: colors.text}}>
                bool isEven(int n) &#123;<br/>
                &nbsp;&nbsp;if (n == 0) return true;<br/>
                &nbsp;&nbsp;return <span style={{color: colors.pink}}>isOdd(n-1);</span><br/>
                &#125;<br/><br/>
                bool isOdd(int n) &#123;<br/>
                &nbsp;&nbsp;if (n == 0) return false;<br/>
                &nbsp;&nbsp;return <span style={{color: colors.pink}}>isEven(n-1);</span><br/>
                &#125;
              </code>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Common Recursive Patterns</h2>
        
        <div style={{display: 'flex', flexDirection: 'column', gap: '20px'}}>
          <div style={boxStyle}>
            <h3 style={{color: colors.yellow}}>Divide and Conquer</h3>
            <p>Break a problem into smaller subproblems, solve them recursively, then combine results.</p>
            <div style={{display: 'flex', justifyContent: 'center', marginTop: '10px', padding: '10px', backgroundColor: 'rgba(255, 255, 255, 0.05)', borderRadius: '8px'}}>
              <div style={{textAlign: 'center'}}>
                <div style={{color: colors.cyan}}>Examples:</div>
                <ul style={{listStyle: 'none', padding: 0, textAlign: 'left'}}>
                  <li>• Merge Sort</li>
                  <li>• Quick Sort</li>
                  <li>• Binary Search</li>
                </ul>
              </div>
            </div>
          </div>
          
          <div style={boxStyle}>
            <h3 style={{color: colors.yellow}}>Backtracking</h3>
            <p>Build solutions incrementally, abandoning partial solutions that fail to meet constraints.</p>
            <div style={{display: 'flex', justifyContent: 'center', marginTop: '10px', padding: '10px', backgroundColor: 'rgba(255, 255, 255, 0.05)', borderRadius: '8px'}}>
              <div style={{textAlign: 'center'}}>
                <div style={{color: colors.cyan}}>Examples:</div>
                <ul style={{listStyle: 'none', padding: 0, textAlign: 'left'}}>
                  <li>• N-Queens Problem</li>
                  <li>• Sudoku Solver</li>
                  <li>• Permutations Generator</li>
                </ul>
              </div>
            </div>
          </div>
          
          <div style={boxStyle}>
            <h3 style={{color: colors.yellow}}>Dynamic Programming (Memoization)</h3>
            <p>Cache previously computed results to avoid redundant calculations.</p>
            <div style={codeBlockStyle}>
              <code style={{color: colors.text}}>
                int fib(int n, unordered_map&lt;int, int&gt;&amp; memo) &#123;<br/>
                &nbsp;&nbsp;<span style={{color: colors.green}}>if (memo.find(n) != memo.end())<br/>
                &nbsp;&nbsp;&nbsp;&nbsp;return memo[n];</span> // Use cached result<br/><br/>
                &nbsp;&nbsp;// Base cases<br/>
                &nbsp;&nbsp;if (n &lt;= 1) return n;<br/><br/>
                &nbsp;&nbsp;// Cache and return result<br/>
                &nbsp;&nbsp;<span style={{color: colors.pink}}>memo[n] = fib(n-1, memo) + fib(n-2, memo);</span><br/>
                &nbsp;&nbsp;return memo[n];<br/>
                &#125;
              </code>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Recursion vs. Iteration</h2>
        
        <div style={{display: 'flex', flexWrap: 'wrap', gap: '20px', justifyContent: 'space-between'}}>
          <div style={{flex: '1', minWidth: '300px', padding: '15px', backgroundColor: 'rgba(0, 255, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.cyan}`}}>
            <h3 style={{color: colors.cyan, marginTop: 0}}>Recursion Advantages</h3>
            <ul style={{paddingLeft: '20px'}}>
              <li>Cleaner, more elegant code for naturally recursive problems</li>
              <li>Directly mirrors the mathematical definition</li>
              <li>Excellent for tree/graph traversal and divide & conquer</li>
              <li>Can reduce complex problems to simple cases</li>
            </ul>
          </div>
          
          <div style={{flex: '1', minWidth: '300px', padding: '15px', backgroundColor: 'rgba(255, 0, 255, 0.1)', borderRadius: '8px', border: `1px solid ${colors.pink}`}}>
            <h3 style={{color: colors.pink, marginTop: 0}}>Recursion Disadvantages</h3>
            <ul style={{paddingLeft: '20px'}}>
              <li>Higher memory usage (call stack overhead)</li>
              <li>Risk of stack overflow for deep recursion</li>
              <li>Generally slower due to function call overhead</li>
              <li>Can lead to redundant calculations (without memoization)</li>
            </ul>
          </div>
        </div>
        
        <div style={{marginTop: '20px', padding: '15px', backgroundColor: 'rgba(0, 255, 0, 0.1)', borderRadius: '8px', border: `1px solid ${colors.green}`}}>
          <h3 style={{color: colors.green, marginTop: 0}}>When to Choose Recursion</h3>
          <div style={{display: 'flex', flexWrap: 'wrap', gap: '15px'}}>
            <div style={{flex: 1, minWidth: '200px'}}>
              <strong>Ideal for:</strong>
              <ul style={{paddingLeft: '20px'}}>
                <li>Tree traversals</li>
                <li>Graph algorithms</li>
                <li>Divide and conquer algorithms</li>
                <li>Problems with recursive mathematical definitions</li>
                <li>When code clarity is prioritized over performance</li>
              </ul>
            </div>
            <div style={{flex: 1, minWidth: '200px'}}>
              <strong>Consider iteration for:</strong>
              <ul style={{paddingLeft: '20px'}}>
                <li>Performance-critical code</li>
                <li>Simple linear processes</li>
                <li>Memory-constrained environments</li>
                <li>When recursion would be very deep</li>
                <li>Simple loops with straightforward logic</li>
              </ul>
            </div>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 15: Recursion Fundamentals
      </div>
    </div>
  );
};

export default RecursionVisualization;
