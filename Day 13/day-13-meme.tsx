import React from 'react';

const ContainerMeme = () => {
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
    display: 'grid',
    gridTemplateRows: 'auto auto',
    gap: '5px',
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

  const panelsContainerStyle = {
    display: 'grid',
    gridTemplateColumns: '1fr 1fr',
    gap: '10px'
  };

  const panelStyle = {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    padding: '15px',
    backgroundColor: 'rgba(255, 255, 255, 0.05)',
    borderRadius: '5px'
  };

  const panelTextStyle = {
    color: colors.text,
    fontWeight: 'bold',
    marginTop: '10px',
    textAlign: 'center',
    fontSize: '14px'
  };

  const drakeImgStyle = {
    width: '100%',
    height: '120px',
    backgroundColor: '#333',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    color: colors.yellow,
    fontWeight: 'bold',
    borderRadius: '4px',
    border: `1px solid ${colors.yellow}`
  };

  const codeBlockStyle = {
    fontFamily: 'monospace',
    textAlign: 'left',
    padding: '8px',
    borderRadius: '4px',
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    marginTop: '10px',
    width: '90%',
    fontSize: '14px'
  };

  const footerStyle = {
    fontSize: '14px',
    color: colors.yellow,
    marginTop: '20px',
    fontStyle: 'italic'
  };

  return (
    <div style={containerStyle}>
      <h2 style={titleStyle}>Map & Set Container Meme</h2>
      
      <div style={memeContainerStyle}>
        <div style={memeStyle}>
          <div style={headerTextStyle}>Choosing the Right C++ Container</div>
          
          <div style={panelsContainerStyle}>
            {/* Left panel - Drake disapproves */}
            <div style={panelStyle}>
              <div style={drakeImgStyle}>✋😒</div>
              <div style={panelTextStyle}>Using std::map when you only need fast lookups</div>
              <div style={{...codeBlockStyle, color: colors.pink}}>
                std::map&lt;string, int&gt; myMap;<br/>
                // O(log n) lookups
              </div>
            </div>
            
            {/* Right panel - Drake approves */}
            <div style={panelStyle}>
              <div style={drakeImgStyle}>👍😏</div>
              <div style={panelTextStyle}>Using std::unordered_map for blazing fast access</div>
              <div style={{...codeBlockStyle, color: colors.cyan}}>
                std::unordered_map&lt;string, int&gt; myMap;<br/>
                // O(1) lookups
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <div style={memeContainerStyle}>
        <div style={memeStyle}>
          <div style={headerTextStyle}>But When You Need Sorting...</div>
          
          <div style={panelsContainerStyle}>
            {/* Left panel - Drake disapproves */}
            <div style={panelStyle}>
              <div style={drakeImgStyle}>✋😒</div>
              <div style={panelTextStyle}>Manually sorting unordered_map elements</div>
              <div style={{...codeBlockStyle, color: colors.pink}}>
                vector&lt;pair&lt;K,V&gt;&gt; vec(map.begin(), map.end());<br/>
                sort(vec.begin(), vec.end());<br/>
                // Extra steps & memory
              </div>
            </div>
            
            {/* Right panel - Drake approves */}
            <div style={panelStyle}>
              <div style={drakeImgStyle}>👍😏</div>
              <div style={panelTextStyle}>Just using std::map for built-in sorting</div>
              <div style={{...codeBlockStyle, color: colors.green}}>
                std::map&lt;string, int&gt; myMap;<br/>
                // Already sorted by key!
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 13: Maps & Sets in C++ STL
      </div>
    </div>
  );
};

export default ContainerMeme;
