import React from 'react';

const JourneyMeme = () => {
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
    padding: '10px 5px',
    textAlign: 'center'
  };

  const memeImageContainerStyle = {
    backgroundColor: '#333',
    padding: '15px',
    position: 'relative',
    height: '320px',
    display: 'flex',
    flexDirection: 'column',
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: '4px',
    overflow: 'hidden'
  };

  const characterStyle = {
    width: '90px',
    height: '90px',
    borderRadius: '50%',
    position: 'absolute',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    fontSize: '40px',
    fontWeight: 'bold',
    color: '#000'
  };

  const progressBarStyle = {
    position: 'absolute',
    bottom: '40px',
    width: '90%',
    height: '20px',
    backgroundColor: 'rgba(255, 255, 255, 0.1)',
    borderRadius: '10px',
    overflow: 'hidden'
  };

  const progressFillStyle = {
    height: '100%',
    width: '31%', // 14/45 days = ~31%
    backgroundColor: colors.green,
    borderRadius: '10px'
  };

  const footerStyle = {
    fontSize: '14px',
    color: colors.yellow,
    marginTop: '20px',
    fontStyle: 'italic'
  };

  const treeSection = {
    position: 'absolute',
    right: '20px',
    top: '70px',
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center'
  };

  const treeNode = {
    width: '50px',
    height: '50px',
    backgroundColor: '#555',
    borderRadius: '50%',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    color: '#aaa',
    fontSize: '12px',
    border: '2px dashed #777',
    marginBottom: '5px'
  };

  const dataStructures = [
    { pos: [40, 90], color: colors.green, label: 'Arrays' },
    { pos: [110, 60], color: colors.green, label: 'Vectors' },
    { pos: [190, 50], color: colors.pink, label: 'LinkedLists' },
    { pos: [270, 70], color: colors.pink, label: 'Stacks' },
    { pos: [350, 110], color: colors.pink, label: 'Queues' },
    { pos: [420, 160], color: colors.yellow, label: 'HashTables' },
    { pos: [470, 220], color: colors.yellow, label: 'Sets & Maps' }
  ];

  return (
    <div style={containerStyle}>
      <h2 style={titleStyle}>The DSA Journey So Far</h2>
      
      <div style={memeContainerStyle}>
        <div style={memeStyle}>
          <div style={headerTextStyle}>Three Weeks In, And Here We Are...</div>
          
          <div style={memeImageContainerStyle}>
            {/* Journey Path - curved line */}
            <svg width="550" height="300" style={{position: 'absolute', top: 0, left: 0}}>
              <path 
                d="M30,100 Q150,20 300,100 Q450,180 500,250" 
                stroke={colors.cyan} 
                strokeWidth="4" 
                fill="none" 
                strokeDasharray="8,8"
              />
              
              {/* You are here marker */}
              <circle cx="500" cy="250" r="8" fill={colors.green} />
            </svg>
            
            {/* Character */}
            <div style={{...characterStyle, bottom: '20px', right: '20px', backgroundColor: colors.green}}>
              👨‍💻
            </div>
            
            {/* Data structure markers along the path */}
            {dataStructures.map((ds, index) => (
              <div 
                key={index} 
                style={{
                  ...characterStyle, 
                  left: ds.pos[0], 
                  top: ds.pos[1], 
                  width: '70px', 
                  height: '70px',
                  backgroundColor: ds.color,
                  fontSize: '10px'
                }}
              >
                {ds.label}
              </div>
            ))}
            
            {/* Progress bar */}
            <div style={progressBarStyle}>
              <div style={progressFillStyle}></div>
            </div>
            <div style={{position: 'absolute', bottom: '15px', fontSize: '14px'}}>
              31% Complete (14/45 Days)
            </div>
            
            {/* Trees section (coming next) */}
            <div style={treeSection}>
              <div style={{color: colors.cyan, marginBottom: '10px', fontSize: '16px'}}>Coming Next</div>
              <div style={treeNode}>Trees</div>
              <svg width="100" height="30">
                <line x1="25" y1="0" x2="0" y2="30" stroke="#777" strokeWidth="2" strokeDasharray="3,3" />
                <line x1="25" y1="0" x2="50" y2="30" stroke="#777" strokeWidth="2" strokeDasharray="3,3" />
              </svg>
              <div style={{display: 'flex', gap: '10px'}}>
                <div style={{...treeNode, width: '40px', height: '40px'}}>BST</div>
                <div style={{...treeNode, width: '40px', height: '40px'}}>Heap</div>
              </div>
            </div>
          </div>
          
          <div style={{padding: '10px', fontSize: '14px', textAlign: 'center', fontStyle: 'italic'}}>
            When you realize there's still so much ahead, but you've already learned so much!
          </div>
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 14: Weekly Challenge & Recap
      </div>
    </div>
  );
};

export default JourneyMeme;
