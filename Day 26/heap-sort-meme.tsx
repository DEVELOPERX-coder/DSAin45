import React from 'react';
import { Award, Cpu, Clock } from 'lucide-react';

const HeapSortMeme = () => {
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
      padding: '20px',
      display: 'flex',
      flexDirection: 'column',
      alignItems: 'center',
      justifyContent: 'center',
      fontFamily: 'Arial, sans-serif',
      color: '#fff',
      borderRadius: '8px',
      aspectRatio: '1/1',
      boxSizing: 'border-box'
    }}>
      <div style={{
        textAlign: 'center',
        marginBottom: '10px'
      }}>
        <h2 style={{ 
          color: colors.cyanColor,
          marginBottom: '5px',
          fontSize: '24px'
        }}>
          The Sorting Algorithms Podium
        </h2>
      </div>
      
      {/* Olympic Podium */}
      <div style={{
        display: 'flex',
        alignItems: 'flex-end',
        justifyContent: 'center',
        width: '100%',
        marginBottom: '20px',
        position: 'relative',
        height: '250px'
      }}>
        {/* Silver - Merge Sort */}
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          marginRight: '10px'
        }}>
          <div style={{
            backgroundColor: '#333',
            borderRadius: '50%',
            width: '70px',
            height: '70px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            marginBottom: '10px',
            border: '3px solid #C0C0C0'
          }}>
            <Cpu color={colors.cyanColor} size={40} />
          </div>
          
          <div style={{
            backgroundColor: '#333',
            padding: '8px 12px',
            borderRadius: '4px',
            fontSize: '14px',
            marginBottom: '10px'
          }}>
            Merge Sort
          </div>
          
          <div style={{
            backgroundColor: '#222',
            width: '100px',
            height: '120px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            borderTopLeftRadius: '8px',
            borderTopRightRadius: '8px'
          }}>
            2
          </div>
        </div>
        
        {/* Gold - Quick Sort */}
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          zIndex: 10
        }}>
          <div style={{
            backgroundColor: '#333',
            borderRadius: '50%',
            width: '80px',
            height: '80px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            marginBottom: '10px',
            border: '3px solid #FFD700'
          }}>
            <Clock color={colors.yellowColor} size={50} />
          </div>
          
          <div style={{
            backgroundColor: '#333',
            padding: '8px 12px',
            borderRadius: '4px',
            fontSize: '14px',
            marginBottom: '10px'
          }}>
            Quick Sort
          </div>
          
          <div style={{
            backgroundColor: '#222',
            width: '120px',
            height: '150px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            borderTopLeftRadius: '8px',
            borderTopRightRadius: '8px'
          }}>
            1
          </div>
        </div>
        
        {/* Bronze - Heap Sort */}
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          marginLeft: '10px'
        }}>
          <div style={{
            backgroundColor: '#333',
            borderRadius: '50%',
            width: '70px',
            height: '70px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            marginBottom: '10px',
            border: '3px solid #CD7F32'
          }}>
            <Award color={colors.pinkColor} size={40} />
          </div>
          
          <div style={{
            backgroundColor: '#333',
            padding: '8px 12px',
            borderRadius: '4px',
            fontSize: '14px',
            marginBottom: '10px'
          }}>
            Heap Sort
          </div>
          
          <div style={{
            backgroundColor: '#222',
            width: '100px',
            height: '90px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            borderTopLeftRadius: '8px',
            borderTopRightRadius: '8px'
          }}>
            3
          </div>
        </div>
      </div>
      
      {/* Algorithm Properties */}
      <div style={{
        display: 'flex',
        justifyContent: 'space-between',
        width: '100%',
        marginBottom: '20px'
      }}>
        <div style={{
          backgroundColor: '#222',
          padding: '10px',
          borderRadius: '4px',
          flex: '1',
          margin: '0 5px',
          fontSize: '12px',
          textAlign: 'center'
        }}>
          <div style={{ color: colors.cyanColor }}>Merge Sort</div>
          <div>O(n log n) Always</div>
          <div>But O(n) Space</div>
        </div>
        
        <div style={{
          backgroundColor: '#222',
          padding: '10px',
          borderRadius: '4px',
          flex: '1',
          margin: '0 5px',
          fontSize: '12px',
          textAlign: 'center'
        }}>
          <div style={{ color: colors.yellowColor }}>Quick Sort</div>
          <div>O(n log n) Average</div>
          <div>But O(n²) Worst</div>
        </div>
        
        <div style={{
          backgroundColor: '#222',
          padding: '10px',
          borderRadius: '4px',
          flex: '1',
          margin: '0 5px',
          fontSize: '12px',
          textAlign: 'center'
        }}>
          <div style={{ color: colors.pinkColor }}>Heap Sort</div>
          <div>O(n log n) Always</div>
          <div>And O(1) Space</div>
        </div>
      </div>
      
      {/* Crying Bubble Sort */}
      <div style={{
        display: 'flex',
        alignItems: 'center',
        backgroundColor: '#222',
        padding: '15px',
        borderRadius: '8px',
        width: '80%',
        marginBottom: '20px'
      }}>
        <div style={{ 
          fontSize: '30px',
          marginRight: '10px'
        }}>
          😭
        </div>
        <div>
          <div>Bubble Sort: O(n²)</div>
          <div style={{ fontSize: '12px', color: '#aaa' }}>Didn't even make the podium</div>
        </div>
      </div>
      
      {/* Caption */}
      <div style={{
        backgroundColor: 'rgba(255, 255, 255, 0.1)',
        padding: '10px 15px',
        borderRadius: '8px',
        fontSize: '14px',
        textAlign: 'center',
        color: colors.greenColor,
        width: '90%'
      }}>
        "Heap Sort: Never the fastest, but reliably efficient and memory conscious"
      </div>
      
      {/* Footer */}
      <div style={{ 
        marginTop: '20px',
        fontSize: '12px',
        color: '#666'
      }}>
        Day 26 of #DSAin45 - Heap Sort & Applications
      </div>
    </div>
  );
};

export default HeapSortMeme;