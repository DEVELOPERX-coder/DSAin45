import React from 'react';

const DSAJourneyOverview = () => {
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

  const weeksContainerStyle = {
    display: 'flex',
    flexDirection: 'column',
    gap: '20px',
    marginBottom: '30px'
  };

  const weekStyle = {
    display: 'flex',
    flexDirection: 'column',
    gap: '10px',
  };

  const weekHeaderStyle = {
    color: colors.cyan,
    borderBottom: `1px solid ${colors.cyan}`,
    paddingBottom: '5px',
    fontSize: '20px'
  };

  const dayContainerStyle = {
    display: 'grid',
    gridTemplateColumns: 'repeat(7, 1fr)',
    gap: '10px'
  };

  const dayItemStyle = (color) => ({
    display: 'flex',
    flexDirection: 'column',
    justifyContent: 'space-between',
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
    border: `1px solid ${color}`,
    borderRadius: '5px',
    padding: '8px',
    minHeight: '100px'
  });

  const dayHeaderStyle = (color) => ({
    color: color,
    fontWeight: 'bold',
    marginBottom: '5px',
    fontSize: '14px'
  });

  const dayContentStyle = {
    fontSize: '12px',
    color: colors.text
  };

  const dayNumberStyle = {
    position: 'relative',
    textAlign: 'right',
    color: 'rgba(255, 255, 255, 0.3)',
    fontSize: '11px',
    marginTop: '5px'
  };

  const challengeSectionStyle = {
    backgroundColor: 'rgba(255, 255, 255, 0.05)',
    borderRadius: '8px',
    padding: '15px',
    marginTop: '20px',
    border: `1px solid ${colors.pink}`
  };

  const challengeHeaderStyle = {
    color: colors.pink,
    borderBottom: `1px solid ${colors.pink}`,
    paddingBottom: '5px',
    marginBottom: '10px',
    fontSize: '20px'
  };

  const challengeListStyle = {
    listStyle: 'none',
    padding: '0',
    margin: '0'
  };

  const challengeItemStyle = {
    margin: '10px 0',
    padding: '10px',
    backgroundColor: 'rgba(0, 0, 0, 0.3)',
    borderLeft: `3px solid ${colors.green}`,
    borderRadius: '3px'
  };

  const journeyProgressStyle = {
    display: 'flex',
    flexDirection: 'column',
    gap: '10px',
    marginTop: '20px'
  };

  const progressBarContainerStyle = {
    width: '100%',
    backgroundColor: 'rgba(255, 255, 255, 0.1)',
    borderRadius: '10px',
    height: '20px',
    overflow: 'hidden'
  };

  const progressBarStyle = {
    width: '31%', // 14/45 = ~31%
    backgroundColor: colors.green,
    height: '100%',
    borderRadius: '10px',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    color: '#000',
    fontWeight: 'bold',
    fontSize: '12px'
  };

  const footerStyle = {
    textAlign: 'center',
    marginTop: '30px',
    color: colors.yellow,
    fontStyle: 'italic'
  };

  // Define the journey data
  const journeyData = [
    {
      week: 'Week 1: C++ Fundamentals & Basic Data Structures',
      days: [
        { day: 1, title: 'Introduction & Setup', content: 'DSA fundamentals, C++ environment setup', color: colors.cyan },
        { day: 2, title: 'Time & Space Complexity', content: 'Big O, Big Omega, Big Theta analysis', color: colors.cyan },
        { day: 3, title: 'Arrays in C++', content: 'Static vs dynamic arrays, operations', color: colors.green },
        { day: 4, title: 'Strings in C++', content: 'String implementations, operations', color: colors.green },
        { day: 5, title: 'Vectors & STL Containers', content: 'Vector basics, STL overview', color: colors.green },
        { day: 6, title: 'Linked Lists - Part 1', content: 'Singly linked lists, implementation', color: colors.pink },
        { day: 7, title: 'Linked Lists - Part 2', content: 'Doubly & circular linked lists', color: colors.pink }
      ]
    },
    {
      week: 'Week 2: Basic Data Structures',
      days: [
        { day: 8, title: 'Stacks', content: 'Stack implementations and applications', color: colors.pink },
        { day: 9, title: 'Queues', content: 'Queue implementation, circular queue', color: colors.pink },
        { day: 10, title: 'Deques & Priority Queues', content: 'Double-ended queues, heap-based priority queues', color: colors.pink },
        { day: 11, title: 'Hash Tables - Part 1', content: 'Hashing concepts, collision resolution', color: colors.yellow },
        { day: 12, title: 'Hash Tables - Part 2', content: 'Advanced implementations, applications', color: colors.yellow },
        { day: 13, title: 'Sets & Maps in C++ STL', content: 'Ordered vs unordered containers', color: colors.yellow },
        { day: 14, title: 'Weekly Challenge & Recap', content: 'Integrated problems, optimization tips', color: colors.cyan }
      ]
    },
    {
      week: 'Next Week: Trees',
      days: [
        { day: 15, title: 'Recursion Fundamentals', content: 'Coming next!', color: 'gray' },
        { day: 16, title: 'Searching Algorithms', content: 'Coming next!', color: 'gray' },
        { day: 17, title: 'Basic Sorting Algorithms', content: 'Coming next!', color: 'gray' },
        { day: 18, title: 'Efficient Sorting Algorithms', content: 'Coming next!', color: 'gray' },
        { day: 19, title: 'Divide and Conquer', content: 'Coming next!', color: 'gray' },
        { day: 20, title: 'Two-Pointer Technique', content: 'Coming next!', color: 'gray' },
        { day: 21, title: 'Weekly Challenge & Recap', content: 'Coming next!', color: 'gray' }
      ]
    }
  ];

  // Challenge data
  const weeklyChallenge = [
    {
      title: "Challenge #1: Document Word Frequency Analyzer",
      description: "Implement a system to process text documents and perform various frequency analysis operations including finding top N words, frequency queries, and word distance calculations."
    },
    {
      title: "Challenge #2: Balanced Parentheses Extended",
      description: "Extend the classic balanced brackets problem to handle multiple types of brackets, identify problematic positions, and provide auto-completion functionality like modern code editors."
    },
    {
      title: "Challenge #3: LRU Cache with Time-Based Expiry",
      description: "Build an enhanced LRU Cache with time-to-live (TTL) functionality, batch operations, and comprehensive statistics tracking."
    }
  ];

  return (
    <div style={containerStyle}>
      <h1 style={headerStyle}>#DSAin45 - Three Week Journey Overview</h1>
      
      <div style={weeksContainerStyle}>
        {journeyData.map((week, weekIndex) => (
          <div key={weekIndex} style={weekStyle}>
            <h2 style={weekHeaderStyle}>{week.week}</h2>
            <div style={dayContainerStyle}>
              {week.days.map((day) => (
                <div key={day.day} style={dayItemStyle(day.color)}>
                  <div>
                    <div style={dayHeaderStyle(day.color)}>{day.title}</div>
                    <div style={dayContentStyle}>{day.content}</div>
                  </div>
                  <div style={dayNumberStyle}>Day {day.day}</div>
                </div>
              ))}
            </div>
          </div>
        ))}
      </div>
      
      <div style={challengeSectionStyle}>
        <h2 style={challengeHeaderStyle}>Week 3 Challenge Problems</h2>
        <ul style={challengeListStyle}>
          {weeklyChallenge.map((challenge, index) => (
            <li key={index} style={challengeItemStyle}>
              <div style={{fontWeight: 'bold', color: colors.green}}>{challenge.title}</div>
              <div style={{fontSize: '14px', marginTop: '5px'}}>{challenge.description}</div>
            </li>
          ))}
        </ul>
      </div>
      
      <div style={journeyProgressStyle}>
        <h3 style={{color: colors.cyan}}>Journey Progress</h3>
        <div style={progressBarContainerStyle}>
          <div style={progressBarStyle}>31% Complete (14/45 Days)</div>
        </div>
        <div style={{fontSize: '14px', textAlign: 'center'}}>
          Moving to Week 4: Binary Trees, BSTs, AVL Trees, Heaps, and Tries
        </div>
      </div>
      
      <div style={footerStyle}>
        #DSAin45 Day 14: Weekly Challenge & Recap
      </div>
    </div>
  );
};

export default DSAJourneyOverview;
