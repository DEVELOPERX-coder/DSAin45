import React, { useState } from 'react';

const AVLTreeVisuals = () => {
  const [activeTab, setActiveTab] = useState('rotations');
  
  // Colors (neon on dark background)
  const bgColor = "#121212";
  const cyanColor = "#00FFFF";
  const pinkColor = "#FF00FF";
  const yellowColor = "#FFFF00";
  const greenColor = "#00FF00";
  
  const renderRotationsVisual = () => {
    return (
      <div style={{ padding: '20px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree Rotations
        </h3>
        
        <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '20px' }}>
          {/* Left Rotation (RR case) */}
          <div style={{ border: '1px solid #333', borderRadius: '8px', padding: '15px' }}>
            <h4 style={{ color: cyanColor, textAlign: 'center', marginBottom: '15px' }}>
              Left Rotation (RR Imbalance)
            </h4>
            <svg width="240" height="220" style={{ display: 'block', margin: '0 auto' }}>
              {/* Before rotation */}
              <g transform="translate(30, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">Before</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={cyanColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">x</text>
                
                <line x1="60" y1="65" x2="90" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={yellowColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="30" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="20" cy="100" r="20" fill={cyanColor} />
                <text x="20" y="105" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="85" y1="115" x2="70" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="70" cy="150" r="20" fill={cyanColor} />
                <text x="70" y="155" fill="#000" textAnchor="middle">T2</text>
                
                <line x1="115" y1="115" x2="130" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="130" cy="150" r="20" fill={cyanColor} />
                <text x="130" y="155" fill="#000" textAnchor="middle">T3</text>
              </g>
              
              {/* Arrow */}
              <g transform="translate(120, 80)">
                <line x1="0" y1="0" x2="40" y2="0" stroke={pinkColor} strokeWidth="2" />
                <polygon points="40,0 35,-5 35,5" fill={pinkColor} />
                <text x="20" y="-10" fill={pinkColor} textAnchor="middle">Left Rotate</text>
              </g>
              
              {/* After rotation */}
              <g transform="translate(190, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">After</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={yellowColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">y</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={cyanColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">x</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={cyanColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">T3</text>
                
                <line x1="-15" y1="115" x2="-30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="-30" cy="150" r="20" fill={cyanColor} />
                <text x="-30" y="155" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="15" y1="115" x2="30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="30" cy="150" r="20" fill={cyanColor} />
                <text x="30" y="155" fill="#000" textAnchor="middle">T2</text>
              </g>
            </svg>
          </div>
          
          {/* Right Rotation (LL case) */}
          <div style={{ border: '1px solid #333', borderRadius: '8px', padding: '15px' }}>
            <h4 style={{ color: cyanColor, textAlign: 'center', marginBottom: '15px' }}>
              Right Rotation (LL Imbalance)
            </h4>
            <svg width="240" height="220" style={{ display: 'block', margin: '0 auto' }}>
              {/* Before rotation */}
              <g transform="translate(30, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">Before</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={cyanColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">y</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={yellowColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">x</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={cyanColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">T3</text>
                
                <line x1="-15" y1="115" x2="-30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="-30" cy="150" r="20" fill={cyanColor} />
                <text x="-30" y="155" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="15" y1="115" x2="30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="30" cy="150" r="20" fill={cyanColor} />
                <text x="30" y="155" fill="#000" textAnchor="middle">T2</text>
              </g>
              
              {/* Arrow */}
              <g transform="translate(120, 80)">
                <line x1="0" y1="0" x2="40" y2="0" stroke={pinkColor} strokeWidth="2" />
                <polygon points="40,0 35,-5 35,5" fill={pinkColor} />
                <text x="20" y="-10" fill={pinkColor} textAnchor="middle">Right Rotate</text>
              </g>
              
              {/* After rotation */}
              <g transform="translate(190, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">After</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={yellowColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">x</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={cyanColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={cyanColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="85" y1="115" x2="70" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="70" cy="150" r="20" fill={cyanColor} />
                <text x="70" y="155" fill="#000" textAnchor="middle">T2</text>
                
                <line x1="115" y1="115" x2="130" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="130" cy="150" r="20" fill={cyanColor} />
                <text x="130" y="155" fill="#000" textAnchor="middle">T3</text>
              </g>
            </svg>
          </div>
          
          {/* Left-Right Rotation (LR case) */}
          <div style={{ border: '1px solid #333', borderRadius: '8px', padding: '15px' }}>
            <h4 style={{ color: cyanColor, textAlign: 'center', marginBottom: '15px' }}>
              Left-Right Rotation (LR Imbalance)
            </h4>
            <svg width="240" height="220" style={{ display: 'block', margin: '0 auto' }}>
              {/* Before rotation */}
              <g transform="translate(30, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">Before</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={cyanColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">z</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={yellowColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={cyanColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">T4</text>
                
                <line x1="-15" y1="115" x2="-30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="-30" cy="150" r="20" fill={cyanColor} />
                <text x="-30" y="155" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="15" y1="115" x2="30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="30" cy="150" r="20" fill={pinkColor} />
                <text x="30" y="155" fill="#000" textAnchor="middle">x</text>
                
                <line x1="15" y1="165" x2="0" y2="185" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="200" r="15" fill={cyanColor} />
                <text x="0" y="204" fill="#000" textAnchor="middle" fontSize="12">T2</text>
                
                <line x1="45" y1="165" x2="60" y2="185" stroke={greenColor} strokeWidth="2" />
                <circle cx="60" cy="200" r="15" fill={cyanColor} />
                <text x="60" y="204" fill="#000" textAnchor="middle" fontSize="12">T3</text>
              </g>
              
              {/* Arrow */}
              <g transform="translate(120, 80)">
                <line x1="0" y1="0" x2="40" y2="0" stroke={pinkColor} strokeWidth="2" />
                <polygon points="40,0 35,-5 35,5" fill={pinkColor} />
                <text x="20" y="-10" fill={pinkColor} textAnchor="middle">Left-Right</text>
              </g>
              
              {/* After rotation */}
              <g transform="translate(190, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">After</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={pinkColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">x</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={yellowColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={cyanColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">z</text>
                
                <line x1="-15" y1="115" x2="-30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="-30" cy="150" r="15" fill={cyanColor} />
                <text x="-30" y="154" fill="#000" textAnchor="middle" fontSize="12">T1</text>
                
                <line x1="15" y1="115" x2="0" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="150" r="15" fill={cyanColor} />
                <text x="0" y="154" fill="#000" textAnchor="middle" fontSize="12">T2</text>
                
                <line x1="85" y1="115" x2="70" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="70" cy="150" r="15" fill={cyanColor} />
                <text x="70" y="154" fill="#000" textAnchor="middle" fontSize="12">T3</text>
                
                <line x1="115" y1="115" x2="130" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="130" cy="150" r="15" fill={cyanColor} />
                <text x="130" y="154" fill="#000" textAnchor="middle" fontSize="12">T4</text>
              </g>
            </svg>
          </div>
          
          {/* Right-Left Rotation (RL case) */}
          <div style={{ border: '1px solid #333', borderRadius: '8px', padding: '15px' }}>
            <h4 style={{ color: cyanColor, textAlign: 'center', marginBottom: '15px' }}>
              Right-Left Rotation (RL Imbalance)
            </h4>
            <svg width="240" height="220" style={{ display: 'block', margin: '0 auto' }}>
              {/* Before rotation */}
              <g transform="translate(30, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">Before</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={cyanColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">z</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={cyanColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">T1</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={yellowColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="85" y1="115" x2="70" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="70" cy="150" r="20" fill={pinkColor} />
                <text x="70" y="155" fill="#000" textAnchor="middle">x</text>
                
                <line x1="115" y1="115" x2="130" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="130" cy="150" r="20" fill={cyanColor} />
                <text x="130" y="155" fill="#000" textAnchor="middle">T4</text>
                
                <line x1="55" y1="165" x2="40" y2="185" stroke={greenColor} strokeWidth="2" />
                <circle cx="40" cy="200" r="15" fill={cyanColor} />
                <text x="40" y="204" fill="#000" textAnchor="middle" fontSize="12">T2</text>
                
                <line x1="85" y1="165" x2="100" y2="185" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="200" r="15" fill={cyanColor} />
                <text x="100" y="204" fill="#000" textAnchor="middle" fontSize="12">T3</text>
              </g>
              
              {/* Arrow */}
              <g transform="translate(120, 80)">
                <line x1="0" y1="0" x2="40" y2="0" stroke={pinkColor} strokeWidth="2" />
                <polygon points="40,0 35,-5 35,5" fill={pinkColor} />
                <text x="20" y="-10" fill={pinkColor} textAnchor="middle">Right-Left</text>
              </g>
              
              {/* After rotation */}
              <g transform="translate(190, 0)">
                <text x="50" y="20" fill="#999" textAnchor="middle">After</text>
                
                {/* Nodes and connections */}
                <circle cx="50" cy="50" r="20" fill={pinkColor} />
                <text x="50" y="55" fill="#000" textAnchor="middle">x</text>
                
                <line x1="35" y1="65" x2="20" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="100" r="20" fill={cyanColor} />
                <text x="0" y="105" fill="#000" textAnchor="middle">z</text>
                
                <line x1="65" y1="65" x2="80" y2="85" stroke={greenColor} strokeWidth="2" />
                <circle cx="100" cy="100" r="20" fill={yellowColor} />
                <text x="100" y="105" fill="#000" textAnchor="middle">y</text>
                
                <line x1="-15" y1="115" x2="-30" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="-30" cy="150" r="15" fill={cyanColor} />
                <text x="-30" y="154" fill="#000" textAnchor="middle" fontSize="12">T1</text>
                
                <line x1="15" y1="115" x2="0" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="0" cy="150" r="15" fill={cyanColor} />
                <text x="0" y="154" fill="#000" textAnchor="middle" fontSize="12">T2</text>
                
                <line x1="85" y1="115" x2="70" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="70" cy="150" r="15" fill={cyanColor} />
                <text x="70" y="154" fill="#000" textAnchor="middle" fontSize="12">T3</text>
                
                <line x1="115" y1="115" x2="130" y2="135" stroke={greenColor} strokeWidth="2" />
                <circle cx="130" cy="150" r="15" fill={cyanColor} />
                <text x="130" y="154" fill="#000" textAnchor="middle" fontSize="12">T4</text>
              </g>
            </svg>
          </div>
        </div>
      </div>
    );
  };
  
  const renderComplexityChart = () => {
    return (
      <div style={{ padding: '20px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree Time Complexity
        </h3>
        
        <div style={{ 
          backgroundColor: '#1a1a1a', 
          border: '1px solid #333',
          borderRadius: '8px', 
          padding: '20px',
          maxWidth: '600px',
          margin: '0 auto'
        }}>
          <table style={{ width: '100%', borderCollapse: 'collapse' }}>
            <thead>
              <tr style={{ borderBottom: '1px solid #444' }}>
                <th style={{ padding: '10px', color: cyanColor, textAlign: 'left' }}>Operation</th>
                <th style={{ padding: '10px', color: cyanColor, textAlign: 'center' }}>Average Case</th>
                <th style={{ padding: '10px', color: cyanColor, textAlign: 'center' }}>Worst Case</th>
                <th style={{ padding: '10px', color: cyanColor, textAlign: 'center' }}>Regular BST (Worst)</th>
              </tr>
            </thead>
            <tbody>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '10px', color: '#fff' }}>Search</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: pinkColor, textAlign: 'center' }}>O(n)</td>
              </tr>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '10px', color: '#fff' }}>Insert</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: pinkColor, textAlign: 'center' }}>O(n)</td>
              </tr>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '10px', color: '#fff' }}>Delete</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: greenColor, textAlign: 'center' }}>O(log n)</td>
                <td style={{ padding: '10px', color: pinkColor, textAlign: 'center' }}>O(n)</td>
              </tr>
              <tr>
                <td style={{ padding: '10px', color: '#fff' }}>Space</td>
                <td style={{ padding: '10px', color: yellowColor, textAlign: 'center' }}>O(n)</td>
                <td style={{ padding: '10px', color: yellowColor, textAlign: 'center' }}>O(n)</td>
                <td style={{ padding: '10px', color: yellowColor, textAlign: 'center' }}>O(n)</td>
              </tr>
            </tbody>
          </table>
          
          <div style={{ marginTop: '20px', color: '#999', fontSize: '14px', lineHeight: '1.5' }}>
            <p>
              <strong style={{ color: yellowColor }}>Height Bound:</strong> The height of an AVL tree with n nodes is at most 1.44 × log₂(n+2) - 0.328
            </p>
            <p>
              <strong style={{ color: cyanColor }}>Balance Property:</strong> For any node, the heights of its left and right subtrees differ by at most 1
            </p>
          </div>
        </div>
      </div>
    );
  };
  
  const renderComparisonVisual = () => {
    return (
      <div style={{ padding: '20px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree vs Regular BST Comparison
        </h3>
        
        <div style={{ display: 'flex', justifyContent: 'center', marginBottom: '30px' }}>
          <div style={{ 
            backgroundColor: '#1a1a1a', 
            border: '1px solid #333',
            borderRadius: '8px', 
            padding: '20px',
            width: '45%',
            margin: '0 10px'
          }}>
            <h4 style={{ color: greenColor, textAlign: 'center', marginBottom: '15px' }}>
              AVL Tree (After Sequential Insertion)
            </h4>
            <svg width="240" height="180" style={{ display: 'block', margin: '0 auto' }}>
              <circle cx="120" cy="30" r="20" fill={cyanColor} />
              <text x="120" y="35" fill="#000" textAnchor="middle">4</text>
              
              <line x1="110" y1="45" x2="60" y2="75" stroke={greenColor} strokeWidth="2" />
              <circle cx="60" cy="90" r="20" fill={cyanColor} />
              <text x="60" y="95" fill="#000" textAnchor="middle">2</text>
              
              <line x1="130" y1="45" x2="180" y2="75" stroke={greenColor} strokeWidth="2" />
              <circle cx="180" cy="90" r="20" fill={cyanColor} />
              <text x="180" y="95" fill="#000" textAnchor="middle">6</text>
              
              <line x1="50" y1="105" x2="30" y2="135" stroke={greenColor} strokeWidth="2" />
              <circle cx="30" cy="150" r="20" fill={cyanColor} />
              <text x="30" y="155" fill="#000" textAnchor="middle">1</text>
              
              <line x1="70" y1="105" x2="90" y2="135" stroke={greenColor} strokeWidth="2" />
              <circle cx="90" cy="150" r="20" fill={cyanColor} />
              <text x="90" y="155" fill="#000" textAnchor="middle">3</text>
              
              <line x1="170" y1="105" x2="150" y2="135" stroke={greenColor} strokeWidth="2" />
              <circle cx="150" cy="150" r="20" fill={cyanColor} />
              <text x="150" y="155" fill="#000" textAnchor="middle">5</text>
              
              <line x1="190" y1="105" x2="210" y2="135" stroke={greenColor} strokeWidth="2" />
              <circle cx="210" cy="150" r="20" fill={cyanColor} />
              <text x="210" y="155" fill="#000" textAnchor="middle">7</text>
              
              <text x="120" y="190" fill="#999" textAnchor="middle">Height: 3 (Balanced)</text>
            </svg>
          </div>
          
          <div style={{ 
            backgroundColor: '#1a1a1a', 
            border: '1px solid #333',
            borderRadius: '8px', 
            padding: '20px',
            width: '45%',
            margin: '0 10px'
          }}>
            <h4 style={{ color: pinkColor, textAlign: 'center', marginBottom: '15px' }}>
              Regular BST (After Sequential Insertion)
            </h4>
            <svg width="240" height="180" style={{ display: 'block', margin: '0 auto' }}>
              <circle cx="30" cy="30" r="20" fill={cyanColor} />
              <text x="30" y="35" fill="#000" textAnchor="middle">1</text>
              
              <line x1="45" y1="40" x2="75" y2="55" stroke={greenColor} strokeWidth="2" />
              <circle cx="80" cy="60" r="20" fill={cyanColor} />
              <text x="80" y="65" fill="#000" textAnchor="middle">2</text>
              
              <line x1="95" y1="70" x2="125" y2="85" stroke={greenColor} strokeWidth="2" />
              <circle cx="130" cy="90" r="20" fill={cyanColor} />
              <text x="130" y="95" fill="#000" textAnchor="middle">3</text>
              
              <line x1="145" y1="100" x2="175" y2="115" stroke={greenColor} strokeWidth="2" />
              <circle cx="180" cy="120" r="20" fill={cyanColor} />
              <text x="180" y="125" fill="#000" textAnchor="middle">4</text>
              
              <line x1="195" y1="130" x2="205" y2="145" stroke={greenColor} strokeWidth="2" />
              <circle cx="210" cy="150" r="18" fill={cyanColor} />
              <text x="210" y="155" fill="#000" textAnchor="middle" fontSize="14">5...</text>
              
              <text x="120" y="190" fill="#999" textAnchor="middle">Height: 7 (Degenerated to Linked List)</text>
            </svg>
          </div>
        </div>
        
        <div style={{ 
          backgroundColor: '#1a1a1a', 
          border: '1px solid #333',
          borderRadius: '8px', 
          padding: '15px',
          maxWidth: '600px',
          margin: '0 auto',
          color: '#fff'
        }}>
          <h4 style={{ color: yellowColor, marginBottom: '10px' }}>Key Differences:</h4>
          <ul style={{ listStyleType: 'none', padding: '0 15px' }}>
            <li style={{ marginBottom: '8px', display: 'flex', alignItems: 'center' }}>
              <div style={{ width: '10px', height: '10px', backgroundColor: greenColor, marginRight: '10px' }}></div>
              <span>AVL Trees maintain height of O(log n) through self-balancing</span>
            </li>
            <li style={{ marginBottom: '8px', display: 'flex', alignItems: 'center' }}>
              <div style={{ width: '10px', height: '10px', backgroundColor: greenColor, marginRight: '10px' }}></div>
              <span>Operations remain efficient regardless of insertion order</span>
            </li>
            <li style={{ marginBottom: '8px', display: 'flex', alignItems: 'center' }}>
              <div style={{ width: '10px', height: '10px', backgroundColor: greenColor, marginRight: '10px' }}></div>
              <span>Tree is automatically rebalanced after insertions and deletions</span>
            </li>
            <li style={{ display: 'flex', alignItems: 'center' }}>
              <div style={{ width: '10px', height: '10px', backgroundColor: greenColor, marginRight: '10px' }}></div>
              <span>Slightly more memory overhead to store height information</span>
            </li>
          </ul>
        </div>
      </div>
    );
  };
  
  // AVL Tree Meme
  const renderMemeFun = () => {
    return (
      <div style={{ padding: '20px' }}>
        <h3 style={{ color: pinkColor, textAlign: 'center', marginBottom: '20px' }}>
          AVL Tree Meme
        </h3>
        
        <div style={{ 
          backgroundColor: '#1a1a1a', 
          border: '1px solid #333',
          borderRadius: '8px', 
          padding: '15px',
          maxWidth: '500px',
          margin: '0 auto',
          textAlign: 'center'
        }}>
          <svg width="460" height="400" style={{ backgroundColor: '#1a1a1a', margin: '0 auto' }}>
            {/* Title */}
            <text x="230" y="30" fill={yellowColor} textAnchor="middle" fontSize="20" fontWeight="bold">
              Regular BST vs AVL Tree
            </text>
            
            {/* Regular BST */}
            <text x="120" y="60" fill="#fff" textAnchor="middle" fontSize="16">
              Regular BST:
            </text>
            
            <text x="120" y="85" fill="#999" textAnchor="middle" fontSize="14">
              "I'll take your data in any order!"
            </text>
            
            {/* Initial insertions - both trees look the same */}
            <g transform="translate(70, 100)">
              <circle cx="50" cy="20" r="15" fill={cyanColor} />
              <text x="50" y="25" fill="#000" textAnchor="middle" fontSize="12">10</text>
              
              <line x1="40" y1="30" x2="25" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="20" cy="50" r="15" fill={cyanColor} />
              <text x="20" y="55" fill="#000" textAnchor="middle" fontSize="12">5</text>
              
              <line x1="60" y1="30" x2="75" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="80" cy="50" r="15" fill={cyanColor} />
              <text x="80" y="55" fill="#000" textAnchor="middle" fontSize="12">15</text>
            </g>
            
            {/* Regular BST after sequential insertions */}
            <g transform="translate(70, 200)">
              <circle cx="50" cy="20" r="15" fill={cyanColor} />
              <text x="50" y="25" fill="#000" textAnchor="middle" fontSize="12">10</text>
              
              <line x1="60" y1="30" x2="75" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="80" cy="50" r="15" fill={cyanColor} />
              <text x="80" y="55" fill="#000" textAnchor="middle" fontSize="12">15</text>
              
              <line x1="90" y1="60" x2="105" y2="75" stroke={greenColor} strokeWidth="2" />
              <circle cx="110" cy="80" r="15" fill={cyanColor} />
              <text x="110" y="85" fill="#000" textAnchor="middle" fontSize="12">20</text>
              
              <line x1="120" y1="90" x2="135" y2="105" stroke={greenColor} strokeWidth="2" />
              <circle cx="140" cy="110" r="15" fill={cyanColor} />
              <text x="140" y="115" fill="#000" textAnchor="middle" fontSize="12">25</text>
            </g>
            
            <text x="120" y="330" fill={pinkColor} textAnchor="middle" fontSize="14">
              "Wait... this is just a linked list now..."
            </text>
            
            <text x="120" y="355" fill="#ff6666" textAnchor="middle" fontSize="14">
              O(n) operations
            </text>
            
            <text x="120" y="380" fill="#ff6666" textAnchor="middle" fontSize="12">
              *sweats nervously*
            </text>
            
            {/* AVL Tree */}
            <text x="340" y="60" fill="#fff" textAnchor="middle" fontSize="16">
              AVL Tree:
            </text>
            
            <text x="340" y="85" fill="#999" textAnchor="middle" fontSize="14">
              "I'll take your data in any order!"
            </text>
            
            {/* Initial insertions - both trees look the same */}
            <g transform="translate(290, 100)">
              <circle cx="50" cy="20" r="15" fill={cyanColor} />
              <text x="50" y="25" fill="#000" textAnchor="middle" fontSize="12">10</text>
              
              <line x1="40" y1="30" x2="25" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="20" cy="50" r="15" fill={cyanColor} />
              <text x="20" y="55" fill="#000" textAnchor="middle" fontSize="12">5</text>
              
              <line x1="60" y1="30" x2="75" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="80" cy="50" r="15" fill={cyanColor} />
              <text x="80" y="55" fill="#000" textAnchor="middle" fontSize="12">15</text>
            </g>
            
            {/* AVL Tree after sequential insertions - still balanced */}
            <g transform="translate(290, 200)">
              <circle cx="50" cy="20" r="15" fill={cyanColor} />
              <text x="50" y="25" fill="#000" textAnchor="middle" fontSize="12">15</text>
              
              <line x1="40" y1="30" x2="25" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="20" cy="50" r="15" fill={cyanColor} />
              <text x="20" y="55" fill="#000" textAnchor="middle" fontSize="12">10</text>
              
              <line x1="60" y1="30" x2="75" y2="45" stroke={greenColor} strokeWidth="2" />
              <circle cx="80" cy="50" r="15" fill={cyanColor} />
              <text x="80" y="55" fill="#000" textAnchor="middle" fontSize="12">20</text>
              
              <line x1="10" y1="60" x2="0" y2="75" stroke={greenColor} strokeWidth="2" />
              <circle cx="-5" cy="80" r="15" fill={cyanColor} />
              <text x="-5" y="85" fill="#000" textAnchor="middle" fontSize="12">5</text>
              
              <line x1="90" y1="60" x2="105" y2="75" stroke={greenColor} strokeWidth="2" />
              <circle cx="110" cy="80" r="15" fill={cyanColor} />
              <text x="110" y="85" fill="#000" textAnchor="middle" fontSize="12">25</text>
            </g>
            
            <text x="340" y="330" fill={greenColor} textAnchor="middle" fontSize="14">
              "Perfectly balanced, as all things should be."
            </text>
            
            <text x="340" y="355" fill={greenColor} textAnchor="middle" fontSize="14">
              O(log n) operations
            </text>
            
            <text x="340" y="380" fill={greenColor} textAnchor="middle" fontSize="12">
              *rotates confidently*
            </text>
            
            {/* Dividing line */}
            <line x1="230" y1="50" x2="230" y2="390" stroke="#333" strokeWidth="2" />
          </svg>
          
          <div style={{ marginTop: '10px', fontSize: '14px', color: '#999' }}>
            #DSAin45 #Day24 #AVLTrees #ProgrammerHumor
          </div>
        </div>
      </div>
    );
  };
  
  return (
    <div style={{ 
      backgroundColor: bgColor, 
      width: '100%', 
      height: '100%',
      minHeight: '700px',
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      padding: '20px',
      overflow: 'auto'
    }}>
      {/* Title */}
      <div style={{ 
        fontSize: '28px', 
        fontWeight: 'bold', 
        color: pinkColor,
        marginBottom: '20px',
        textAlign: 'center'
      }}>
        AVL Trees - Day 24 #DSAin45
      </div>
      
      {/* Tabs */}
      <div style={{ 
        display: 'flex', 
        backgroundColor: '#1a1a1a',
        borderRadius: '8px',
        overflow: 'hidden',
        marginBottom: '20px'
      }}>
        <button 
          onClick={() => setActiveTab('rotations')} 
          style={{ 
            padding: '10px 15px',
            backgroundColor: activeTab === 'rotations' ? '#2a2a2a' : 'transparent',
            border: 'none',
            color: activeTab === 'rotations' ? '#fff' : '#999',
            cursor: 'pointer',
            fontWeight: activeTab === 'rotations' ? 'bold' : 'normal'
          }}
        >
          Rotation Types
        </button>
        <button 
          onClick={() => setActiveTab('complexity')}
          style={{ 
            padding: '10px 15px',
            backgroundColor: activeTab === 'complexity' ? '#2a2a2a' : 'transparent',
            border: 'none',
            color: activeTab === 'complexity' ? '#fff' : '#999',
            cursor: 'pointer',
            fontWeight: activeTab === 'complexity' ? 'bold' : 'normal'
          }}
        >
          Complexity Chart
        </button>
        <button 
          onClick={() => setActiveTab('comparison')}
          style={{ 
            padding: '10px 15px',
            backgroundColor: activeTab === 'comparison' ? '#2a2a2a' : 'transparent',
            border: 'none',
            color: activeTab === 'comparison' ? '#fff' : '#999',
            cursor: 'pointer',
            fontWeight: activeTab === 'comparison' ? 'bold' : 'normal'
          }}
        >
          AVL vs BST
        </button>
        <button 
          onClick={() => setActiveTab('meme')}
          style={{ 
            padding: '10px 15px',
            backgroundColor: activeTab === 'meme' ? '#2a2a2a' : 'transparent',
            border: 'none',
            color: activeTab === 'meme' ? '#fff' : '#999',
            cursor: 'pointer',
            fontWeight: activeTab === 'meme' ? 'bold' : 'normal'
          }}
        >
          Meme
        </button>
      </div>
      
      {/* Content */}
      <div style={{ 
        width: '100%', 
        maxWidth: '800px',
        backgroundColor: '#1a1a1a',
        borderRadius: '8px',
        overflow: 'hidden',
        boxShadow: '0 4px 6px rgba(0, 0, 0, 0.3)'
      }}>
        {activeTab === 'rotations' ? renderRotationsVisual() : 
         activeTab === 'complexity' ? renderComplexityChart() : 
         activeTab === 'comparison' ? renderComparisonVisual() :
         renderMemeFun()}
      </div>
      
      {/* Footer */}
      <div style={{ marginTop: '20px', color: '#999', textAlign: 'center', fontSize: '14px' }}>
        Self-balancing binary search trees that maintain O(log n) operations through height balancing
        <div style={{ marginTop: '5px', color: yellowColor, fontSize: '12px' }}>
          #DSAin45 #Day24 #AVLTrees #DataStructures #Algorithms
        </div>
      </div>
    </div>
  );
};

export default AVLTreeVisuals;