import React from 'react';
import { Search, Zap, Clock, Hash } from 'lucide-react';

const TrieMeme = () => {
  // Colors using neon theme
  const colors = {
    bgColor: "#121212",
    cyanColor: "#00FFFF",
    pinkColor: "#FF00FF",
    yellowColor: "#FFFF00",
    greenColor: "#00FF00"
  };
  
  return (
    <div style={{ 
      backgroundColor: colors.bgColor, 
      width: '100%', 
      height: '100%',
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      justifyContent: 'space-between',
      padding: '20px',
      boxSizing: 'border-box',
      fontFamily: 'Arial, sans-serif',
      color: '#fff',
      textAlign: 'center',
      borderRadius: '8px',
      overflow: 'hidden',
      aspectRatio: '1/1'
    }}>
      {/* Title */}
      <div style={{ marginBottom: '15px' }}>
        <h2 style={{ 
          color: colors.yellowColor, 
          margin: '0 0 5px 0',
          fontSize: '24px'
        }}>
          Autocomplete Systems
        </h2>
        <div style={{ fontSize: '16px', color: '#aaa' }}>
          Behind the scenes...
        </div>
      </div>
      
      {/* Regular Lookup vs. Trie Lookup */}
      <div style={{
        display: 'flex',
        width: '100%',
        justifyContent: 'space-between',
        marginBottom: '15px'
      }}>
        {/* Hash Table Approach */}
        <div style={{
          width: '48%',
          backgroundColor: '#222',
          borderRadius: '8px',
          padding: '15px',
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center'
        }}>
          <div style={{
            backgroundColor: '#333',
            borderRadius: '50%',
            width: '60px',
            height: '60px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            marginBottom: '10px'
          }}>
            <Hash size={30} color={colors.pinkColor} />
          </div>
          
          <div style={{ 
            fontWeight: 'bold', 
            marginBottom: '5px',
            color: colors.pinkColor
          }}>
            Hash Table
          </div>
          
          <div style={{ 
            fontSize: '12px', 
            color: '#ccc',
            marginBottom: '10px',
            height: '40px',
            display: 'flex',
            alignItems: 'center'
          }}>
            "Let me check every single word in my dictionary..."
          </div>
          
          <div style={{
            display: 'flex',
            flexDirection: 'column',
            width: '100%',
            gap: '5px'
          }}>
            {["program", "process", "protein", "promise", "..."].map((word, i) => (
              <div key={i} style={{
                backgroundColor: '#333',
                padding: '6px',
                borderRadius: '4px',
                fontSize: '12px'
              }}>
                {word}
              </div>
            ))}
          </div>
          
          <div style={{
            marginTop: '10px',
            fontSize: '14px',
            fontWeight: 'bold',
            color: colors.pinkColor
          }}>
            O(n) time complexity
          </div>
        </div>
        
        {/* Trie Approach */}
        <div style={{
          width: '48%',
          backgroundColor: '#222',
          borderRadius: '8px',
          padding: '15px',
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center'
        }}>
          <div style={{
            backgroundColor: '#333',
            borderRadius: '50%',
            width: '60px',
            height: '60px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            marginBottom: '10px'
          }}>
            <Zap size={30} color={colors.cyanColor} />
          </div>
          
          <div style={{ 
            fontWeight: 'bold', 
            marginBottom: '5px',
            color: colors.cyanColor
          }}>
            Trie
          </div>
          
          <div style={{ 
            fontSize: '12px', 
            color: '#ccc',
            marginBottom: '10px',
            height: '40px',
            display: 'flex',
            alignItems: 'center'
          }}>
            "I'll just follow the exact path for 'pro'..."
          </div>
          
          {/* Simple Trie Visualization */}
          <svg width="100%" height="120" style={{ marginBottom: '10px' }}>
            {/* Root */}
            <circle cx="80" cy="20" r="15" fill="#333" stroke={colors.cyanColor} strokeWidth="2" />
            <text x="80" y="24" textAnchor="middle" fill="#fff" fontSize="12">*</text>
            
            {/* Level 1 - 'p' */}
            <line x1="80" y1="35" x2="80" y2="50" stroke={colors.cyanColor} strokeWidth="2" />
            <circle cx="80" cy="65" r="15" fill="#333" stroke={colors.cyanColor} strokeWidth="3" />
            <text x="80" y="69" textAnchor="middle" fill="#fff" fontSize="12">p</text>
            
            {/* Level 2 - 'r' */}
            <line x1="80" y1="80" x2="80" y2="95" stroke={colors.cyanColor} strokeWidth="2" />
            <circle cx="80" cy="110" r="15" fill="#333" stroke={colors.cyanColor} strokeWidth="3" />
            <text x="80" y="114" textAnchor="middle" fill="#fff" fontSize="12">r</text>
            
            {/* Level 3 - 'o' */}
            <line x1="80" y1="125" x2="80" y2="140" stroke={colors.cyanColor} strokeWidth="2" />
            <circle cx="80" cy="155" r="15" fill="#333" stroke={colors.cyanColor} strokeWidth="3" />
            <text x="80" y="159" textAnchor="middle" fill="#fff" fontSize="12">o</text>
            
            {/* Paths from 'o' */}
            <line x1="80" y1="170" x2="50" y2="190" stroke="#555" strokeWidth="1" />
            <line x1="80" y1="170" x2="80" y2="190" stroke="#555" strokeWidth="1" />
            <line x1="80" y1="170" x2="110" y2="190" stroke="#555" strokeWidth="1" />
            
            <text x="50" y="205" textAnchor="middle" fill="#aaa" fontSize="10">gram</text>
            <text x="80" y="205" textAnchor="middle" fill="#aaa" fontSize="10">cess</text>
            <text x="110" y="205" textAnchor="middle" fill="#aaa" fontSize="10">mise</text>
          </svg>
          
          <div style={{
            fontSize: '14px',
            fontWeight: 'bold',
            color: colors.cyanColor
          }}>
            O(m + k) time complexity
          </div>
        </div>
      </div>
      
      {/* Explanation Box */}
      <div style={{
        backgroundColor: 'rgba(0, 255, 255, 0.1)',
        padding: '15px',
        borderRadius: '8px',
        width: '100%',
        marginBottom: '15px'
      }}>
        <div style={{ 
          fontSize: '16px', 
          marginBottom: '10px',
          color: colors.greenColor,
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          gap: '5px'
        }}>
          <Search size={16} />
          <span>Why Tries Win at Prefix Matching</span>
        </div>
        
        <div style={{ 
          fontSize: '14px',
          color: '#eee',
          textAlign: 'left'
        }}>
          Hash tables must check <span style={{ color: colors.pinkColor }}>every word</span> to find matches, while tries can directly <span style={{ color: colors.cyanColor }}>follow the path</span> for the prefix and only collect words below that node.
        </div>
      </div>
      
      {/* Bottom Time Comparison */}
      <div style={{
        display: 'flex',
        width: '100%',
        gap: '10px',
        alignItems: 'center',
        justifyContent: 'center',
        marginBottom: '5px'
      }}>
        <Clock size={20} color={colors.yellowColor} />
        <span style={{ fontSize: '14px' }}>
          Hash Table: <span style={{ color: colors.pinkColor }}>O(n)</span> vs. 
          Trie: <span style={{ color: colors.cyanColor }}>O(m + k)</span>
        </span>
      </div>
      
      {/* Where n = total words, m = prefix length, k = matching words */}
      <div style={{ 
        fontSize: '12px', 
        color: '#aaa',
        fontStyle: 'italic'
      }}>
        Where n = total words, m = prefix length, k = matching words
      </div>
      
      {/* Footer */}
      <div style={{
        fontSize: '12px',
        color: '#666',
        marginTop: '5px'
      }}>
        Day 27 of #DSAin45 - Trie Data Structure
      </div>
    </div>
  );
};

export default TrieMeme;