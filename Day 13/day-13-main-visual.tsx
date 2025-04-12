import React from 'react';

const MapSetsVisual = () => {
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
    maxWidth: '800px',
    margin: '0 auto'
  };

  const headerStyle = {
    textAlign: 'center',
    color: colors.yellow,
    marginBottom: '30px'
  };

  const sectionStyle = {
    marginBottom: '40px'
  };

  const sectionHeaderStyle = {
    color: colors.pink,
    borderBottom: `2px solid ${colors.pink}`,
    paddingBottom: '5px',
    marginBottom: '15px'
  };

  const gridContainerStyle = {
    display: 'grid',
    gridTemplateColumns: 'repeat(2, 1fr)',
    gap: '20px',
    marginBottom: '20px'
  };

  const boxStyle = {
    backgroundColor: 'rgba(255, 255, 255, 0.1)',
    borderRadius: '6px',
    padding: '15px',
    position: 'relative',
    height: '100%'
  };

  const boxHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    marginBottom: '10px'
  };

  const listItemStyle = {
    margin: '8px 0',
    display: 'flex',
    alignItems: 'center'
  };

  const dotStyle = {
    height: '8px',
    width: '8px',
    borderRadius: '50%',
    marginRight: '10px',
    display: 'inline-block'
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

  const hashTableStyle = {
    display: 'grid',
    gridTemplateColumns: '1fr',
    gap: '2px',
    marginTop: '15px'
  };

  const bucketStyle = {
    padding: '8px',
    backgroundColor: 'rgba(0, 255, 255, 0.1)',
    borderRadius: '4px',
    marginBottom: '2px',
    display: 'flex',
    position: 'relative'
  };

  const treeNodeStyle = {
    width: '60px',
    height: '60px',
    borderRadius: '50%',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    margin: '0 auto',
    position: 'relative',
    backgroundColor: 'rgba(255, 0, 255, 0.2)',
    border: `2px solid ${colors.pink}`,
    color: colors.text,
    fontWeight: 'bold'
  };

  const treeEdgeStyle = {
    position: 'absolute',
    width: '50px',
    height: '30px',
    border: `2px solid ${colors.pink}`,
    borderTop: 'none',
    borderRight: 'none',
    borderLeft: 'none',
    zIndex: '-1'
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Maps & Sets in C++ STL</h1>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>The Two Container Families</h2>
        <div style={gridContainerStyle}>
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Ordered Containers</h3>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.pink}}></span>
              <span>Based on <strong>balanced BSTs</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.pink}}></span>
              <span>Items stored in <strong>sorted order</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.pink}}></span>
              <span><strong>O(log n)</strong> operations</span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.pink}}></span>
              <span>Support range queries</span>
            </div>
            <div style={codeBlockStyle}>
              <span style={{color: colors.pink}}>std::set</span><br/>
              <span style={{color: colors.pink}}>std::map</span><br/>
              <span style={{color: colors.pink}}>std::multiset</span><br/>
              <span style={{color: colors.pink}}>std::multimap</span>
            </div>
          </div>
          
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Unordered Containers</h3>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.cyan}}></span>
              <span>Based on <strong>hash tables</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.cyan}}></span>
              <span>No guaranteed ordering</span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.cyan}}></span>
              <span><strong>O(1)</strong> average case lookups</span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.cyan}}></span>
              <span>No range queries</span>
            </div>
            <div style={codeBlockStyle}>
              <span style={{color: colors.cyan}}>std::unordered_set</span><br/>
              <span style={{color: colors.cyan}}>std::unordered_map</span><br/>
              <span style={{color: colors.cyan}}>std::unordered_multiset</span><br/>
              <span style={{color: colors.cyan}}>std::unordered_multimap</span>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>Implementation Visualizations</h2>
        <div style={gridContainerStyle}>
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Tree-based (std::map)</h3>
            <div style={{position: 'relative', height: '200px', display: 'flex', justifyContent: 'center', alignItems: 'center'}}>
              {/* Node 30 (root) */}
              <div style={{position: 'absolute', top: '20px', left: '50%', transform: 'translateX(-50%)'}}>
                <div style={treeNodeStyle}>30</div>
              </div>
              
              {/* Level 2 */}
              <div style={{position: 'absolute', top: '100px', left: '25%', transform: 'translateX(-50%)'}}>
                <div style={treeNodeStyle}>10</div>
              </div>
              <div style={{position: 'absolute', top: '100px', left: '75%', transform: 'translateX(-50%)'}}>
                <div style={treeNodeStyle}>50</div>
              </div>
              
              {/* Connect edges */}
              <div style={{position: 'absolute', top: '50px', left: '40%', width: '60px', height: '50px', borderLeft: `2px solid ${colors.pink}`, borderBottom: `2px solid ${colors.pink}`, borderBottomLeftRadius: '10px'}}></div>
              <div style={{position: 'absolute', top: '50px', left: '52%', width: '60px', height: '50px', borderRight: `2px solid ${colors.pink}`, borderBottom: `2px solid ${colors.pink}`, borderBottomRightRadius: '10px'}}></div>

              {/* Sorted keys note */}
              <div style={{position: 'absolute', bottom: '0', textAlign: 'center', width: '100%', color: colors.yellow}}>
                Keys maintained in sorted order: 10, 30, 50
              </div>
            </div>
          </div>
          
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Hash-based (std::unordered_map)</h3>
            <div style={hashTableStyle}>
              <div style={{...bucketStyle, borderLeft: `3px solid ${colors.cyan}`}}>
                <span style={{fontWeight: 'bold', marginRight: '5px', color: colors.cyan}}>0:</span>
                <span>{"{ key: 50, value: 500 }"}</span>
              </div>
              <div style={{...bucketStyle, borderLeft: `3px solid ${colors.cyan}`}}>
                <span style={{fontWeight: 'bold', marginRight: '5px', color: colors.cyan}}>1:</span>
                <span>-empty-</span>
              </div>
              <div style={{...bucketStyle, borderLeft: `3px solid ${colors.cyan}`}}>
                <span style={{fontWeight: 'bold', marginRight: '5px', color: colors.cyan}}>2:</span>
                <span>{"{ key: 10, value: 100 }"}</span>
              </div>
              <div style={{...bucketStyle, borderLeft: `3px solid ${colors.cyan}`}}>
                <span style={{fontWeight: 'bold', marginRight: '5px', color: colors.cyan}}>3:</span>
                <span>-empty-</span>
              </div>
              <div style={{...bucketStyle, borderLeft: `3px solid ${colors.cyan}`}}>
                <span style={{fontWeight: 'bold', marginRight: '5px', color: colors.cyan}}>4:</span>
                <span>{"{ key: 30, value: 300 }"}</span>
              </div>
              <div style={{marginTop: '10px', textAlign: 'center', color: colors.yellow}}>
                Keys distributed by hash function (no ordering)
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <div style={sectionStyle}>
        <h2 style={sectionHeaderStyle}>When to Use Each Container Type</h2>
        <div style={gridContainerStyle}>
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Use Ordered (map/set) when:</h3>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.green}}></span>
              <span>Items need to be in <strong>sorted order</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.green}}></span>
              <span>Need <strong>range queries</strong> (lower/upper bound)</span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.green}}></span>
              <span>Using <strong>custom types</strong> where comparison is easier than hashing</span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.green}}></span>
              <span>Need <strong>predictable iteration</strong> order</span>
            </div>
          </div>
          
          <div style={boxStyle}>
            <h3 style={boxHeaderStyle}>Use Unordered (unordered_map/unordered_set) when:</h3>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.yellow}}></span>
              <span>Need <strong>fastest possible lookups</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.yellow}}></span>
              <span>Don't care about <strong>element order</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.yellow}}></span>
              <span>Keys have <strong>natural hash functions</strong></span>
            </div>
            <div style={listItemStyle}>
              <span style={{...dotStyle, backgroundColor: colors.yellow}}></span>
              <span>Working with <strong>large datasets</strong> where performance matters</span>
            </div>
          </div>
        </div>
      </div>
      
      <div style={{textAlign: 'center', marginTop: '40px', color: colors.yellow, fontStyle: 'italic'}}>
        #DSAin45 Day 13: Maps & Sets in C++ STL
      </div>
    </div>
  );
};

export default MapSetsVisual;
