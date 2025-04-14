import React from 'react';

const RecursionMeme = () => {
  // Neon colors
  const colors = {
    background: '#121212',
    cyan: '#00FFFF',
    pink: '#FF00FF',
    yellow: '#FFFF00',
    green: '#00FF00',
    text: '#FFFFFF'
  };

  const containerStyle = {
    backgroundColor: colors.background,
    color: colors.text,
    fontFamily: 'sans-serif',
    padding: '20px',
    borderRadius: '8px',
    width: '100%',
    maxWidth: '600px',
    margin: '0 auto',
    textAlign: 'center'
  };

  const titleStyle = {
    color: colors.yellow,
    marginBottom: '20px'
  };

  const memeContainerStyle = {
    backgroundColor: 'rgba(255, 255, 255, 0.1)',
    borderRadius: '8px',
    padding: '20px',
    marginBottom: '20px'
  };

  const memeStyle = {
    display: 'flex',
    flexDirection: 'column',
    backgroundColor: '#000',
    padding: '10px',
    borderRadius: '6px',
    border: `2px solid ${colors.pink}`
  };

  const headerTextStyle = {
    color: colors.text,
    fontWeight: 'bold',
    fontSize: '18px',
    padding: '5px'
  };

  const recursionPanelsStyle = {
    display: 'flex',
    flexDirection: 'column',
    gap: '10px',
    padding: '10px',
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    borderRadius: '4px'
  };

  const recursionLevelStyle = (depth) => ({
    display: 'flex',
    flexDirection: 'column',
    padding: '10px',
    borderRadius: '5px',
    backgroundColor: 'rgba(0, 0, 0, 0.8)',
    border: `1px solid ${depth === 4 ? colors.green : colors.pink}`,
    marginLeft: `${depth * 15}px`,
  });

  const codeLineStyle = {
    fontFamily: 'monospace',
    textAlign: 'left',
    color: colors.text,
    margin: '3px 0'
  };

  const footerStyle = {
    fontSize: '14px',
    color: colors.yellow,
    marginTop: '20px',
    fontStyle: 'italic'
  };

  return (
    <div style={containerStyle}>
      <h2 style={titleStyle}>Understanding Recursion</h2>
      
      <div style={memeContainerStyle}>
        <div style={memeStyle}>
          <div style={headerTextStyle}>To understand recursion...</div>
          
          <div style={recursionPanelsStyle}>
            {/* First recursion level */}
            <div style={recursionLevelStyle(0)}>
              <div style={codeLineStyle}>function understandRecursion() {</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;// To understand recursion</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;understandRecursion();</div>
              <div style={codeLineStyle}>}</div>
            </div>
            
            {/* Second recursion level */}
            <div style={recursionLevelStyle(1)}>
              <div style={codeLineStyle}>function understandRecursion() {</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;// To understand recursion</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;understandRecursion();</div>
              <div style={codeLineStyle}>}</div>
            </div>
            
            {/* Third recursion level */}
            <div style={recursionLevelStyle(2)}>
              <div style={codeLineStyle}>function understandRecursion() {</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;// To understand recursion</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;understandRecursion();</div>
              <div style={codeLineStyle}>}</div>
            </div>
            
            {/* Fourth recursion level - with base case */}
            <div style={recursionLevelStyle(4)}>
              <div style={codeLineStyle}>function understandRecursion() {</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;// Base case</div>
              <div style={{...codeLineStyle, color: colors.green}}>
                &nbsp;&nbsp;if (youUnderstandRecursion()) {
              </div>
              <div style={{...codeLineStyle, color: colors.green}}>
                &nbsp;&nbsp;&nbsp;&nbsp;return true;
              </div>
              <div style={{...codeLineStyle, color: colors.green}}>
                &nbsp;&nbsp;}
              </div>
              <div style={codeLineStyle}>&nbsp;&nbsp;// Otherwise, try again</div>
              <div style={codeLineStyle}>&nbsp;&nbsp;understandRecursion();</div>
              <div style={codeLineStyle}>}</div>
            </div>
            
            {/* Stack overflow error */}
            <div style={{
              marginTop: '10px', 
              padding: '10px', 
              backgroundColor: 'rgba(255, 0, 0, 0.3)', 
              border: '1px solid red',
              borderRadius: '5px',
              fontFamily: 'monospace'
            }}>
              Error: Maximum call stack size exceeded
            </div>
          </div>
          
          <div style={{marginTop: '15px', fontStyle: 'italic'}}>
            Don't forget your base case, or you'll never understand recursion...
          </div>
        </div>
      </div>
      
      <div style={memeContainerStyle}>
        <div style={memeStyle}>
          <div style={headerTextStyle}>The Two Rules of Recursion:</div>
          
          <div style={{
            display: 'flex',
            flexDirection: 'column',
            gap: '15px',
            padding: '15px',
            backgroundColor: 'rgba(0, 0, 0, 0.5)',
          }}>
            <div style={{
              padding: '10px',
              backgroundColor: 'rgba(0, 255, 255, 0.1)',
              border: `1px solid ${colors.cyan}`,
              borderRadius: '5px'
            }}>
              <div style={{fontWeight: 'bold', color: colors.cyan}}>Rule 1:</div>
              <div>Every recursive function must have a base case.</div>
            </div>
            
            <div style={{
              padding: '10px',
              backgroundColor: 'rgba(255, 0, 255, 0.1)',
              border: `1px solid ${colors.pink}`,
              borderRadius: '5px'
            }}>
              <div style={{fontWeight: 'bold', color: colors.pink}}>Rule 2:</div>
              <div>Every recursive function must move toward the base case.</div>
            </div>
            
            <div style={{
              padding: '10px',
              backgroundColor: 'rgba(255, 255, 0, 0.1)',
              border: `1px solid ${colors.yellow}`,
              borderRadius: '5px'
            }}>
              <div style={{fontWeight: 'bold', color: colors.yellow}}>Secret Rule 3:</div>
              <div>To understand the Secret Rule 3 of recursion, first understand the Secret Rule 3 of recursion.</div>
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

export default RecursionMeme;
