import React from 'react';

const ComplexityChart = () => {
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
    maxWidth: '800px',
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

  const warningBoxStyle = {
    backgroundColor: 'rgba(255, 255, 0, 0.1)',
    border: `1px solid ${colors.yellow}`,
    borderRadius: '6px',
    padding: '15px',
    margin: '20px 0'
  };

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>Time & Space Complexity Comparison</h1>
      
      {/* Ordered Containers (Set/Map) */}
      <div style={tableContainerStyle}>
        <h2 style={{color: colors.pink, marginBottom: '15px'}}>Ordered Containers (std::set, std::map)</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Operation</th>
              <th style={thStyle}>Average Case</th>
              <th style={thStyle}>Worst Case</th>
              <th style={thStyle}>Notes</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Insert</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(log n)</td>
              <td style={tdStyle}>O(log n)</td>
              <td style={tdStyle}>Tree rebalancing may occur</td>
            </tr>
            <tr>
              <td style={tdStyle}>Find/Lookup</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(log n)</td>
              <td style={tdStyle}>O(log n)</td>
              <td style={tdStyle}>Binary search in tree</td>
            </tr>
            <tr>
              <td style={tdStyle}>Erase</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(log n)</td>
              <td style={tdStyle}>O(log n)</td>
              <td style={tdStyle}>Tree rebalancing may occur</td>
            </tr>
            <tr>
              <td style={tdStyle}>lower_bound/upper_bound</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(log n)</td>
              <td style={tdStyle}>O(log n)</td>
              <td style={tdStyle}>Not available in unordered containers</td>
            </tr>
            <tr>
              <td style={tdStyle}>Iteration</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(n)</td>
              <td style={tdStyle}>O(n)</td>
              <td style={tdStyle}>Always in sorted order</td>
            </tr>
            <tr>
              <td style={tdStyle}>Size/Empty</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={tdStyle}>O(1)</td>
              <td style={tdStyle}>Typically tracked internally</td>
            </tr>
            <tr>
              <td style={tdStyle}>Space Complexity</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(n)</td>
              <td style={tdStyle}>O(n)</td>
              <td style={tdStyle}>Tree nodes require pointers</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      {/* Unordered Containers (Unordered_set/Unordered_map) */}
      <div style={tableContainerStyle}>
        <h2 style={{color: colors.cyan, marginBottom: '15px'}}>Unordered Containers (std::unordered_set, std::unordered_map)</h2>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Operation</th>
              <th style={thStyle}>Average Case</th>
              <th style={thStyle}>Worst Case</th>
              <th style={thStyle}>Notes</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>Insert</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Worst case: rehashing or many collisions</td>
            </tr>
            <tr>
              <td style={tdStyle}>Find/Lookup</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Worst case: all keys in same bucket</td>
            </tr>
            <tr>
              <td style={tdStyle}>Erase</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>O(n)</td>
              <td style={tdStyle}>Worst case: all keys in same bucket</td>
            </tr>
            <tr>
              <td style={tdStyle}>lower_bound/upper_bound</td>
              <td style={{...highlightTdStyle, color: colors.yellow}}>N/A</td>
              <td style={tdStyle}>N/A</td>
              <td style={tdStyle}>Not available - no ordering</td>
            </tr>
            <tr>
              <td style={tdStyle}>Iteration</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(n)</td>
              <td style={tdStyle}>O(n)</td>
              <td style={tdStyle}>No guaranteed order</td>
            </tr>
            <tr>
              <td style={tdStyle}>Size/Empty</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={tdStyle}>O(1)</td>
              <td style={tdStyle}>Typically tracked internally</td>
            </tr>
            <tr>
              <td style={tdStyle}>load_factor/bucket_count</td>
              <td style={{...highlightTdStyle, color: colors.green}}>O(1)</td>
              <td style={tdStyle}>O(1)</td>
              <td style={tdStyle}>Hash table specific operations</td>
            </tr>
            <tr>
              <td style={tdStyle}>Space Complexity</td>
              <td style={{...highlightTdStyle, color: colors.pink}}>O(n)</td>
              <td style={tdStyle}>O(n)</td>
              <td style={tdStyle}>Extra overhead for buckets</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      {/* Multi-variants */}
      <div style={tableContainerStyle}>
        <h2 style={{color: colors.yellow, marginBottom: '15px'}}>Multi-variants (multiset, multimap, unordered_multiset, unordered_multimap)</h2>
        <p style={{marginBottom: '15px'}}>
          Multi-variants have the same time complexities as their respective base containers, but
          with some differences in behavior:
        </p>
        <table style={tableStyle}>
          <thead>
            <tr>
              <th style={thStyle}>Container</th>
              <th style={thStyle}>Key Behavior</th>
              <th style={thStyle}>Performance Note</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td style={tdStyle}>multiset/multimap</td>
              <td style={tdStyle}>Allow duplicate keys</td>
              <td style={tdStyle}>Still O(log n) operations</td>
            </tr>
            <tr>
              <td style={tdStyle}>unordered_multiset/unordered_multimap</td>
              <td style={tdStyle}>Allow duplicate keys</td>
              <td style={tdStyle}>Still O(1) average case operations</td>
            </tr>
            <tr>
              <td style={tdStyle}>equal_range()</td>
              <td style={tdStyle}>Returns range of matching elements</td>
              <td style={tdStyle}>O(log n) for ordered, O(1) average for unordered</td>
            </tr>
            <tr>
              <td style={tdStyle}>count()</td>
              <td style={tdStyle}>Returns number of matching elements</td>
              <td style={tdStyle}>O(log n + count) for ordered, O(1 + count) average for unordered</td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div style={warningBoxStyle}>
        <h3 style={{color: colors.yellow, marginTop: 0}}>Performance Considerations</h3>
        <ul style={{paddingLeft: '20px'}}>
          <li style={{marginBottom: '8px'}}>
            <strong style={{color: colors.green}}>Unordered containers</strong> are typically 
            <strong> 3-5x faster</strong> for lookups than ordered ones with reasonable data sizes
          </li>
          <li style={{marginBottom: '8px'}}>
            <strong style={{color: colors.yellow}}>Worst-case scenarios</strong> for hash tables
            happen rarely with good hash functions but can be catastrophic
          </li>
          <li style={{marginBottom: '8px'}}>
            <strong style={{color: colors.pink}}>Ordered containers</strong> provide more
            <strong> predictable</strong> performance regardless of data distribution
          </li>
          <li style={{marginBottom: '8px'}}>
            <strong style={{color: colors.cyan}}>Memory usage</strong> is generally higher
            for unordered containers due to hash table buckets
          </li>
        </ul>
      </div>
      
      <div style={{textAlign: 'center', marginTop: '40px', color: colors.yellow, fontStyle: 'italic'}}>
        #DSAin45 Day 13: Maps & Sets in C++ STL
      </div>
    </div>
  );
};

export default ComplexityChart;
