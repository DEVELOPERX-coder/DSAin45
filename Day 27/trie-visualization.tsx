import React, { useState, useEffect } from 'react';
import { Search, BookOpen, Zap, Code, PlayCircle, RotateCw, CheckCircle2, Plus, Hash } from 'lucide-react';

const TrieVisualization = () => {
  // States for the application
  const [trie, setTrie] = useState(() => createEmptyTrie());
  const [wordInput, setWordInput] = useState('');
  const [searchInput, setSearchInput] = useState('');
  const [prefixInput, setPrefixInput] = useState('');
  const [message, setMessage] = useState('Welcome to Trie Visualization!');
  const [activeTab, setActiveTab] = useState('visualization');
  const [wordList, setWordList] = useState([]);
  const [suggestions, setSuggestions] = useState([]);
  const [animationState, setAnimationState] = useState({
    isAnimating: false,
    currentWord: '',
    currentPrefix: '',
    highlightedNodes: [],
    foundWord: false
  });
  const [autoCompleteWord, setAutoCompleteWord] = useState('');
  const [autoCompleteResults, setAutoCompleteResults] = useState([]);
  
  // Colors using neon theme
  const colors = {
    bgColor: "#121212",
    cyanColor: "#00FFFF",
    pinkColor: "#FF00FF",
    yellowColor: "#FFFF00",
    greenColor: "#00FF00",
    grayColor: "#555555"
  };
  
  // Create an empty trie
  function createEmptyTrie() {
    return {
      isEndOfWord: false,
      children: {},
      wordCount: 0,
      x: 0,
      y: 0,
      char: 'root'
    };
  }
  
  // Function to insert a word into the trie
  function insertWord(word) {
    if (!word || word.trim() === '') {
      setMessage('Please enter a valid word');
      return;
    }
    
    word = word.toLowerCase(); // Convert to lowercase for consistency
    
    // Animation setup
    setAnimationState({
      isAnimating: true,
      currentWord: word,
      currentPrefix: '',
      highlightedNodes: [],
      foundWord: false
    });
    
    // Create a copy of trie
    const newTrie = JSON.parse(JSON.stringify(trie));
    let current = newTrie;
    
    // Update word list
    setWordList(prev => {
      if (prev.includes(word)) {
        setMessage(`"${word}" is already in the trie`);
        return prev;
      }
      setMessage(`Inserting "${word}" into the trie`);
      return [...prev, word].sort();
    });
    
    // Animated insertion
    animateInsertion(word, newTrie);
  }
  
  // Function to animate word insertion
  async function animateInsertion(word, newTrie) {
    let current = newTrie;
    let currentHighlightedNodes = [];
    
    for (let i = 0; i < word.length; i++) {
      const char = word[i];
      
      // Check if the current character exists in children
      if (!current.children[char]) {
        current.children[char] = {
          isEndOfWord: false,
          children: {},
          char: char,
          wordCount: 0
        };
      }
      
      current = current.children[char];
      currentHighlightedNodes.push(char);
      
      // Update animation state
      setAnimationState(prev => ({
        ...prev,
        currentPrefix: word.substring(0, i + 1),
        highlightedNodes: [...currentHighlightedNodes]
      }));
      
      // Wait for animation
      await new Promise(resolve => setTimeout(resolve, 300));
    }
    
    // Mark the end of the word
    current.isEndOfWord = true;
    current.wordCount++;
    
    setAnimationState(prev => ({
      ...prev,
      isAnimating: false,
      foundWord: true
    }));
    
    // Update the trie state
    setTrie(newTrie);
    setWordInput('');
  }
  
  // Function to search for a word in the trie
  async function searchWord(word) {
    if (!word || word.trim() === '') {
      setMessage('Please enter a valid word to search');
      return;
    }
    
    word = word.toLowerCase();
    
    // Animation setup
    setAnimationState({
      isAnimating: true,
      currentWord: word,
      currentPrefix: '',
      highlightedNodes: [],
      foundWord: false
    });
    
    let current = trie;
    let currentHighlightedNodes = [];
    
    for (let i = 0; i < word.length; i++) {
      const char = word[i];
      
      // Update animation state
      setAnimationState(prev => ({
        ...prev,
        currentPrefix: word.substring(0, i),
        highlightedNodes: [...currentHighlightedNodes]
      }));
      
      // Wait for animation
      await new Promise(resolve => setTimeout(resolve, 300));
      
      // If character doesn't exist in the trie
      if (!current.children[char]) {
        setMessage(`Word "${word}" not found in the trie`);
        setAnimationState(prev => ({
          ...prev,
          isAnimating: false,
          foundWord: false
        }));
        setSearchInput('');
        return;
      }
      
      // Move to the next node
      current = current.children[char];
      currentHighlightedNodes.push(char);
    }
    
    // Check if it's the end of a word
    const isFound = current.isEndOfWord;
    
    setAnimationState(prev => ({
      ...prev,
      isAnimating: false,
      foundWord: isFound,
      highlightedNodes: [...currentHighlightedNodes]
    }));
    
    setMessage(isFound ? 
      `Word "${word}" found in the trie!` : 
      `"${word}" is not a complete word in the trie`);
      
    setSearchInput('');
  }
  
  // Function to search for words with a given prefix
  async function searchPrefix(prefix) {
    if (!prefix || prefix.trim() === '') {
      setMessage('Please enter a valid prefix to search');
      setSuggestions([]);
      return;
    }
    
    prefix = prefix.toLowerCase();
    
    // Animation setup
    setAnimationState({
      isAnimating: true,
      currentWord: prefix,
      currentPrefix: '',
      highlightedNodes: [],
      foundWord: false
    });
    
    let current = trie;
    let currentHighlightedNodes = [];
    
    // Traverse to the end of the prefix
    for (let i = 0; i < prefix.length; i++) {
      const char = prefix[i];
      
      // Update animation state
      setAnimationState(prev => ({
        ...prev,
        currentPrefix: prefix.substring(0, i),
        highlightedNodes: [...currentHighlightedNodes]
      }));
      
      // Wait for animation
      await new Promise(resolve => setTimeout(resolve, 300));
      
      // If character doesn't exist in the trie
      if (!current.children[char]) {
        setMessage(`No words found with prefix "${prefix}"`);
        setSuggestions([]);
        setAnimationState(prev => ({
          ...prev,
          isAnimating: false,
          foundWord: false
        }));
        setPrefixInput('');
        return;
      }
      
      // Move to the next node
      current = current.children[char];
      currentHighlightedNodes.push(char);
    }
    
    // Collect all words with the given prefix
    const words = [];
    collectWords(current, prefix, words);
    
    setAnimationState(prev => ({
      ...prev,
      isAnimating: false,
      highlightedNodes: [...currentHighlightedNodes]
    }));
    
    setMessage(words.length > 0 ? 
      `Found ${words.length} word(s) with prefix "${prefix}"` : 
      `No complete words found with prefix "${prefix}"`);
      
    setSuggestions(words);
    setPrefixInput('');
  }
  
  // Helper function to collect all words with a given prefix
  function collectWords(node, currentWord, words) {
    if (node.isEndOfWord) {
      words.push(currentWord);
    }
    
    for (const char in node.children) {
      collectWords(node.children[char], currentWord + char, words);
    }
  }
  
  // Function to demo autocomplete
  function handleAutoComplete(e) {
    const input = e.target.value.toLowerCase();
    setAutoCompleteWord(input);
    
    if (!input) {
      setAutoCompleteResults([]);
      return;
    }
    
    // Find suggestions
    const suggestions = findSuggestions(input);
    setAutoCompleteResults(suggestions);
  }
  
  // Helper function to find autocomplete suggestions
  function findSuggestions(prefix) {
    let current = trie;
    
    // Traverse to the end of the prefix
    for (let i = 0; i < prefix.length; i++) {
      const char = prefix[i];
      
      // If character doesn't exist in the trie
      if (!current.children[char]) {
        return [];
      }
      
      // Move to the next node
      current = current.children[char];
    }
    
    // Collect all words with the given prefix
    const words = [];
    collectWords(current, prefix, words);
    
    return words.slice(0, 5); // Return top 5 suggestions
  }
  
  // Load sample words
  function loadSampleWords() {
    const sampleWords = [
      "tree", "trie", "try", "triangle", "treasure",
      "programming", "prefix", "preorder", "practice",
      "data", "structure", "algorithm", "search", "sort"
    ];
    
    // Clear existing trie
    const newTrie = createEmptyTrie();
    setTrie(newTrie);
    setWordList([]);
    setSuggestions([]);
    setMessage('Loading sample words...');
    
    // Insert each word sequentially with a delay
    sampleWords.forEach((word, index) => {
      setTimeout(() => {
        // Create a copy of current trie
        const currentTrie = JSON.parse(JSON.stringify(trie));
        let current = currentTrie;
        
        // Insert the word
        for (let i = 0; i < word.length; i++) {
          const char = word[i];
          
          if (!current.children[char]) {
            current.children[char] = {
              isEndOfWord: false,
              children: {},
              char: char,
              wordCount: 0
            };
          }
          
          current = current.children[char];
        }
        
        // Mark the end of the word
        current.isEndOfWord = true;
        current.wordCount++;
        
        // Update the trie state
        setTrie(currentTrie);
        setWordList(prev => [...prev, word].sort());
        
        if (index === sampleWords.length - 1) {
          setMessage('Sample words loaded successfully!');
        }
      }, index * 100);
    });
  }
  
  // Reset the trie
  function resetTrie() {
    setTrie(createEmptyTrie());
    setWordList([]);
    setSuggestions([]);
    setAnimationState({
      isAnimating: false,
      currentWord: '',
      currentPrefix: '',
      highlightedNodes: [],
      foundWord: false
    });
    setMessage('Trie has been reset');
  }
  
  // Calculate node positions for visualization
  function calculateNodePositions(trieNode, x, y, level = 0, horizontalSpacing = 60, verticalSpacing = 80) {
    // Set the position for the current node
    trieNode.x = x;
    trieNode.y = y;
    
    // Calculate the number of children
    const children = Object.keys(trieNode.children);
    
    if (children.length === 0) return;
    
    // Calculate positions for children
    const totalWidth = horizontalSpacing * (Math.pow(26, level + 1) - 1);
    const startX = x - totalWidth / 2;
    
    let currentChildIndex = 0;
    for (const char in trieNode.children) {
      const childX = startX + currentChildIndex * (totalWidth / children.length) + totalWidth / (2 * children.length);
      const childY = y + verticalSpacing;
      
      calculateNodePositions(
        trieNode.children[char],
        childX,
        childY,
        level + 1,
        horizontalSpacing,
        verticalSpacing
      );
      
      currentChildIndex++;
    }
  }
  
  // Calculate positions for the visualization
  useEffect(() => {
    const trieCopy = JSON.parse(JSON.stringify(trie));
    calculateNodePositions(trieCopy, 300, 80);
    setTrie(trieCopy);
  }, [wordList.length]); // Recalculate when words are added
  
  // Render the trie visualization with nodes and edges
  function renderTrieVisualization() {
    // Function to render nodes recursively
    const renderNode = (node, prefix = '', parentX = null, parentY = null) => {
      const elements = [];
      const nodeSize = 30;
      
      // Render current node
      if (node.char !== 'root') {
        const isHighlighted = animationState.highlightedNodes.includes(node.char) && 
                              prefix.slice(-1) === node.char;
        const isEndOfWord = node.isEndOfWord;
        
        // Determine node color
        let nodeColor = colors.grayColor;
        if (isHighlighted && animationState.foundWord && prefix.length === animationState.currentWord.length) {
          nodeColor = colors.greenColor; // Found word
        } else if (isHighlighted) {
          nodeColor = colors.yellowColor; // Highlighted during search
        } else if (isEndOfWord) {
          nodeColor = colors.cyanColor; // End of word
        }
        
        // Node circle
        elements.push(
          <g key={`node-${prefix}`}>
            {/* Edge to parent */}
            {parentX !== null && parentY !== null && (
              <line
                x1={parentX}
                y1={parentY}
                x2={node.x}
                y2={node.y - nodeSize/2}
                stroke={isHighlighted ? colors.yellowColor : '#555'}
                strokeWidth={isHighlighted ? 2 : 1}
              />
            )}
            
            {/* Node circle */}
            <circle
              cx={node.x}
              cy={node.y}
              r={nodeSize/2}
              fill={nodeColor}
              stroke={isHighlighted ? colors.pinkColor : "#333"}
              strokeWidth={isHighlighted ? 3 : 1}
            />
            
            {/* Character label */}
            <text
              x={node.x}
              y={node.y + 5}
              textAnchor="middle"
              fill="#000"
              fontSize="14"
              fontWeight="bold"
            >
              {node.char}
            </text>
            
            {/* Word end indicator */}
            {isEndOfWord && (
              <circle
                cx={node.x}
                cy={node.y + nodeSize/2 + 5}
                r={4}
                fill={colors.pinkColor}
              />
            )}
          </g>
        );
      }
      
      // Recursively render children
      for (const char in node.children) {
        const childNode = node.children[char];
        const childPrefix = prefix + char;
        
        elements.push(
          ...renderNode(
            childNode,
            childPrefix,
            node.x,
            node.y + (node.char === 'root' ? 0 : nodeSize/2)
          )
        );
      }
      
      return elements;
    };
    
    // Root node indicator
    const renderRootNode = () => {
      return (
        <g key="root">
          <circle
            cx={trie.x}
            cy={trie.y}
            r={20}
            fill="#333"
            stroke="#555"
          />
          <text
            x={trie.x}
            y={trie.y + 5}
            textAnchor="middle"
            fill="#fff"
            fontSize="12"
          >
            Root
          </text>
        </g>
      );
    };
    
    // Calculate the height based on the number of words
    const svgHeight = Math.max(500, 100 + wordList.length * 30);
    
    return (
      <div style={{ textAlign: 'center', marginTop: '20px' }}>
        <svg width="600" height={svgHeight} style={{ backgroundColor: colors.bgColor }}>
          {renderRootNode()}
          {renderNode(trie)}
        </svg>
      </div>
    );
  }
  
  // Render the word list panel
  function renderWordList() {
    return (
      <div style={{
        backgroundColor: '#222',
        padding: '10px',
        borderRadius: '5px',
        marginTop: '20px',
        maxHeight: '200px',
        overflowY: 'auto'
      }}>
        <div style={{ marginBottom: '10px', color: colors.cyanColor }}>
          Words in Trie: {wordList.length}
        </div>
        <div style={{
          display: 'flex',
          flexWrap: 'wrap',
          gap: '8px'
        }}>
          {wordList.map((word, index) => (
            <div
              key={index}
              style={{
                padding: '5px 10px',
                backgroundColor: '#333',
                borderRadius: '4px',
                fontSize: '14px',
                cursor: 'pointer'
              }}
              onClick={() => searchWord(word)}
            >
              {word}
            </div>
          ))}
        </div>
      </div>
    );
  }
  
  // Render the suggestions panel
  function renderSuggestions() {
    if (suggestions.length === 0) return null;
    
    return (
      <div style={{
        backgroundColor: '#222',
        padding: '10px',
        borderRadius: '5px',
        marginTop: '20px',
        maxHeight: '200px',
        overflowY: 'auto'
      }}>
        <div style={{ marginBottom: '10px', color: colors.yellowColor }}>
          Suggestions:
        </div>
        <div style={{
          display: 'flex',
          flexWrap: 'wrap',
          gap: '8px'
        }}>
          {suggestions.map((word, index) => (
            <div
              key={index}
              style={{
                padding: '5px 10px',
                backgroundColor: '#333',
                borderRadius: '4px',
                fontSize: '14px',
                cursor: 'pointer'
              }}
              onClick={() => searchWord(word)}
            >
              {word}
            </div>
          ))}
        </div>
      </div>
    );
  }
  
  // Render the controls section
  function renderControls() {
    return (
      <div style={{
        display: 'flex',
        flexDirection: 'column',
        gap: '15px',
        marginTop: '20px'
      }}>
        {/* Word insertion */}
        <div style={{
          display: 'flex',
          gap: '10px',
          alignItems: 'center'
        }}>
          <div style={{
            backgroundColor: '#333',
            padding: '5px 10px',
            borderRadius: '4px',
            display: 'flex',
            alignItems: 'center',
            width: '120px'
          }}>
            <Plus size={16} color={colors.greenColor} style={{ marginRight: '5px' }} />
            <span>Insert:</span>
          </div>
          <input
            type="text"
            value={wordInput}
            onChange={e => setWordInput(e.target.value)}
            onKeyPress={e => e.key === 'Enter' && insertWord(wordInput)}
            placeholder="Enter a word"
            style={{
              flex: 1,
              padding: '8px 12px',
              backgroundColor: '#222',
              color: '#fff',
              border: '1px solid #444',
              borderRadius: '4px'
            }}
            disabled={animationState.isAnimating}
          />
          <button
            onClick={() => insertWord(wordInput)}
            disabled={animationState.isAnimating}
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: colors.greenColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              opacity: animationState.isAnimating ? 0.6 : 1
            }}
          >
            Insert
          </button>
        </div>
        
        {/* Word search */}
        <div style={{
          display: 'flex',
          gap: '10px',
          alignItems: 'center'
        }}>
          <div style={{
            backgroundColor: '#333',
            padding: '5px 10px',
            borderRadius: '4px',
            display: 'flex',
            alignItems: 'center',
            width: '120px'
          }}>
            <Search size={16} color={colors.cyanColor} style={{ marginRight: '5px' }} />
            <span>Search:</span>
          </div>
          <input
            type="text"
            value={searchInput}
            onChange={e => setSearchInput(e.target.value)}
            onKeyPress={e => e.key === 'Enter' && searchWord(searchInput)}
            placeholder="Search for a word"
            style={{
              flex: 1,
              padding: '8px 12px',
              backgroundColor: '#222',
              color: '#fff',
              border: '1px solid #444',
              borderRadius: '4px'
            }}
            disabled={animationState.isAnimating}
          />
          <button
            onClick={() => searchWord(searchInput)}
            disabled={animationState.isAnimating}
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: colors.cyanColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              opacity: animationState.isAnimating ? 0.6 : 1
            }}
          >
            Search
          </button>
        </div>
        
        {/* Prefix search */}
        <div style={{
          display: 'flex',
          gap: '10px',
          alignItems: 'center'
        }}>
          <div style={{
            backgroundColor: '#333',
            padding: '5px 10px',
            borderRadius: '4px',
            display: 'flex',
            alignItems: 'center',
            width: '120px'
          }}>
            <Hash size={16} color={colors.yellowColor} style={{ marginRight: '5px' }} />
            <span>Prefix:</span>
          </div>
          <input
            type="text"
            value={prefixInput}
            onChange={e => setPrefixInput(e.target.value)}
            onKeyPress={e => e.key === 'Enter' && searchPrefix(prefixInput)}
            placeholder="Search by prefix"
            style={{
              flex: 1,
              padding: '8px 12px',
              backgroundColor: '#222',
              color: '#fff',
              border: '1px solid #444',
              borderRadius: '4px'
            }}
            disabled={animationState.isAnimating}
          />
          <button
            onClick={() => searchPrefix(prefixInput)}
            disabled={animationState.isAnimating}
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: colors.yellowColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              opacity: animationState.isAnimating ? 0.6 : 1
            }}
          >
            Find
          </button>
        </div>
        
        {/* Control buttons */}
        <div style={{
          display: 'flex',
          gap: '10px',
          justifyContent: 'center',
          marginTop: '10px'
        }}>
          <button
            onClick={loadSampleWords}
            disabled={animationState.isAnimating}
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: colors.pinkColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              opacity: animationState.isAnimating ? 0.6 : 1,
              display: 'flex',
              alignItems: 'center',
              gap: '5px'
            }}
          >
            <BookOpen size={16} />
            Load Sample Words
          </button>
          
          <button
            onClick={resetTrie}
            disabled={animationState.isAnimating}
            style={{
              padding: '8px 12px',
              backgroundColor: '#333',
              color: colors.yellowColor,
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              opacity: animationState.isAnimating ? 0.6 : 1,
              display: 'flex',
              alignItems: 'center',
              gap: '5px'
            }}
          >
            <RotateCw size={16} />
            Reset Trie
          </button>
        </div>
      </div>
    );
  }
  
  // Render status message
  function renderMessage() {
    return (
      <div style={{
        backgroundColor: '#222',
        padding: '10px 15px',
        borderRadius: '5px',
        marginTop: '20px',
        color: animationState.foundWord ? colors.greenColor : '#fff'
      }}>
        {message}
      </div>
    );
  }
  
  // Render auto-complete demo
  function renderAutoComplete() {
    return (
      <div style={{
        marginTop: '30px',
        padding: '20px',
        backgroundColor: '#222',
        borderRadius: '8px'
      }}>
        <h3 style={{ 
          color: colors.cyanColor, 
          marginTop: 0,
          marginBottom: '15px',
          display: 'flex',
          alignItems: 'center',
          gap: '8px'
        }}>
          <Zap size={20} />
          Auto-Complete Demo
        </h3>
        
        <div style={{ marginBottom: '15px' }}>
          Try typing to see suggestions based on the words in the trie:
        </div>
        
        <div style={{
          position: 'relative',
          marginBottom: '10px'
        }}>
          <input
            type="text"
            value={autoCompleteWord}
            onChange={handleAutoComplete}
            placeholder="Type to see suggestions..."
            style={{
              width: '100%',
              padding: '10px 15px',
              backgroundColor: '#333',
              color: '#fff',
              border: '1px solid #444',
              borderRadius: '4px',
              fontSize: '16px'
            }}
          />
          
          {autoCompleteResults.length > 0 && (
            <div style={{
              position: 'absolute',
              top: '100%',
              left: 0,
              right: 0,
              backgroundColor: '#333',
              border: '1px solid #444',
              borderTop: 'none',
              borderRadius: '0 0 4px 4px',
              zIndex: 10
            }}>
              {autoCompleteResults.map((word, index) => (
                <div
                  key={index}
                  style={{
                    padding: '8px 15px',
                    borderBottom: index < autoCompleteResults.length - 1 ? '1px solid #444' : 'none',
                    cursor: 'pointer',
                    color: index === 0 ? colors.cyanColor : '#fff',
                    fontWeight: index === 0 ? 'bold' : 'normal'
                  }}
                  onClick={() => {
                    setAutoCompleteWord(word);
                    setAutoCompleteResults([]);
                  }}
                >
                  {word}
                </div>
              ))}
            </div>
          )}
        </div>
        
        <div style={{
          fontSize: '14px',
          color: '#999',
          fontStyle: 'italic'
        }}>
          Note: Suggestions are based on the words you've added to the trie.
        </div>
      </div>
    );
  }
  
  // Render the code implementation tab
  function renderCodeTab() {
    return (
      <div style={{
        padding: '20px',
        backgroundColor: '#1a1a1a',
        borderRadius: '8px'
      }}>
        <h3 style={{ 
          color: colors.yellowColor, 
          marginTop: 0,
          marginBottom: '15px',
          display: 'flex',
          alignItems: 'center',
          gap: '8px'
        }}>
          <Code size={20} />
          Trie Implementation in C++
        </h3>
        
        <div style={{
          backgroundColor: '#111',
          padding: '15px',
          borderRadius: '5px',
          overflowX: 'auto',
          fontSize: '14px',
          lineHeight: '1.5',
          color: '#ccc'
        }}>
          <pre>{`#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class TrieNode {
public:
    bool isEndOfWord;
    std::unordered_map<char, TrieNode*> children;
    
    TrieNode() : isEndOfWord(false) {}
    
    ~TrieNode() {
        for (auto& pair : children) {
            delete pair.second;
        }
    }
};

class Trie {
private:
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    ~Trie() {
        delete root;
    }
    
    // Insert a word into the trie
    void insert(const std::string& word) {
        TrieNode* current = root;
        
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        
        current->isEndOfWord = true;
    }
    
    // Search for a word in the trie
    bool search(const std::string& word) {
        TrieNode* node = findNode(word);
        return node != nullptr && node->isEndOfWord;
    }
    
    // Check if there is any word in the trie that starts with the given prefix
    bool startsWith(const std::string& prefix) {
        return findNode(prefix) != nullptr;
    }
    
    // Find all words with the given prefix
    std::vector<std::string> findWordsWithPrefix(const std::string& prefix) {
        std::vector<std::string> result;
        TrieNode* node = findNode(prefix);
        
        if (node != nullptr) {
            collectWords(node, prefix, result);
        }
        
        return result;
    }
    
private:
    // Helper function to find a node that matches the given prefix
    TrieNode* findNode(const std::string& prefix) {
        TrieNode* current = root;
        
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return nullptr;
            }
            current = current->children[c];
        }
        
        return current;
    }
    
    // Recursively collect all words with a given prefix
    void collectWords(TrieNode* node, const std::string& prefix, std::vector<std::string>& result) {
        if (node->isEndOfWord) {
            result.push_back(prefix);
        }
        
        for (const auto& pair : node->children) {
            char c = pair.first;
            TrieNode* child = pair.second;
            collectWords(child, prefix + c, result);
        }
    }
};

// Example usage
int main() {
    Trie trie;
    
    // Insert words
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("banana");
    trie.insert("ball");
    
    // Search for words
    std::cout << "Search 'apple': " << (trie.search("apple") ? "Found" : "Not found") << std::endl;
    std::cout << "Search 'app': " << (trie.search("app") ? "Found" : "Not found") << std::endl;
    std::cout << "Search 'apricot': " << (trie.search("apricot") ? "Found" : "Not found") << std::endl;
    
    // Check prefixes
    std::cout << "Starts with 'app': " << (trie.startsWith("app") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'ban': " << (trie.startsWith("ban") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'cat': " << (trie.startsWith("cat") ? "Yes" : "No") << std::endl;
    
    // Find words with prefix
    std::cout << "Words with prefix 'app':" << std::endl;
    std::vector<std::string> words = trie.findWordsWithPrefix("app");
    for (const std::string& word : words) {
        std::cout << "  " << word << std::endl;
    }
    
    return 0;
}`}</pre>
        </div>
        
        <div style={{
          marginTop: '20px',
          display: 'flex',
          flexDirection: 'column',
          gap: '15px'
        }}>
          <div>
            <h4 style={{ color: colors.pinkColor, marginBottom: '5px' }}>Key Operations:</h4>
            <ul style={{ paddingLeft: '20px', margin: 0 }}>
              <li>Insert: O(m) time, where m is the word length</li>
              <li>Search: O(m) time, where m is the word length</li>
              <li>Prefix Search: O(m) to find prefix node + O(k) to collect results</li>
            </ul>
          </div>
          
          <div>
            <h4 style={{ color: colors.greenColor, marginBottom: '5px' }}>Memory Optimization Tips:</h4>
            <ul style={{ paddingLeft: '20px', margin: 0 }}>
              <li>Use array-based implementation instead of map for languages with limited character set</li>
              <li>Implement compressed tries (radix trees) for better space efficiency</li>
              <li>Use shared_ptr for automatic memory management in large tries</li>
            </ul>
          </div>
        </div>
      </div>
    );
  }
  
  // Render the applications tab
  function renderApplicationsTab() {
    const applications = [
      {
        title: "Autocomplete & Type-ahead",
        icon: <Search size={24} color={colors.cyanColor} />,
        description: "Quickly suggest words as users type, used in search engines, word processors, and mobile keyboards.",
        code: `// Get autocomplete suggestions
vector<string> getSuggestions(string prefix) {
    return trie.findWordsWithPrefix(prefix);
}`
      },
      {
        title: "Spell Checking",
        icon: <CheckCircle2 size={24} color={colors.greenColor} />,
        description: "Verify if words exist in a dictionary, with the ability to suggest corrections for misspelled words.",
        code: `// Check if a word is spelled correctly
bool isSpelledCorrectly(string word) {
    return dictionary.search(word);
}`
      },
      {
        title: "IP Routing (CIDR)",
        icon: <Zap size={24} color={colors.yellowColor} />,
        description: "Store IP address prefixes for efficient routing table lookups in network routers.",
        code: `// Lookup longest matching prefix for IP routing
Route* findRoute(string ipAddress) {
    return routingTrie.findLongestPrefix(ipAddress);
}`
      },
      {
        title: "Word Games & Puzzles",
        icon: <Hash size={24} color={colors.pinkColor} />,
        description: "Quickly validate words in games like Scrabble, Boggle, or crossword puzzles.",
        code: `// Check if word is valid in a game
bool isValidWord(string word) {
    return wordTrie.search(word);
}`
      }
    ];
    
    return (
      <div style={{
        padding: '20px',
        backgroundColor: '#1a1a1a',
        borderRadius: '8px'
      }}>
        <h3 style={{ 
          color: colors.pinkColor, 
          marginTop: 0,
          marginBottom: '20px',
          textAlign: 'center'
        }}>
          Real-World Trie Applications
        </h3>
        
        <div style={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(280px, 1fr))',
          gap: '20px'
        }}>
          {applications.map((app, index) => (
            <div
              key={index}
              style={{
                backgroundColor: '#222',
                borderRadius: '8px',
                padding: '15px',
                display: 'flex',
                flexDirection: 'column',
                height: '100%'
              }}
            >
              <div style={{
                display: 'flex',
                alignItems: 'center',
                gap: '10px',
                marginBottom: '10px'
              }}>
                {app.icon}
                <h4 style={{ margin: 0, color: '#fff' }}>{app.title}</h4>
              </div>
              
              <p style={{ 
                margin: '0 0 15px 0',
                color: '#aaa',
                flex: 1
              }}>
                {app.description}
              </p>
              
              <div style={{
                backgroundColor: '#111',
                padding: '10px',
                borderRadius: '4px',
                fontSize: '12px',
                fontFamily: 'monospace',
                color: '#ccc'
              }}>
                <pre style={{ margin: 0 }}>{app.code}</pre>
              </div>
            </div>
          ))}
        </div>
        
        <div style={{
          marginTop: '30px',
          padding: '15px',
          backgroundColor: '#222',
          borderRadius: '8px',
          textAlign: 'center'
        }}>
          <h4 style={{ color: colors.yellowColor, marginTop: 0 }}>
            Performance Comparison
          </h4>
          
          <table style={{
            width: '100%',
            borderCollapse: 'collapse',
            marginTop: '10px',
            fontSize: '14px'
          }}>
            <thead>
              <tr style={{ borderBottom: '1px solid #444' }}>
                <th style={{ padding: '8px', textAlign: 'left', color: colors.cyanColor }}>Operation</th>
                <th style={{ padding: '8px', textAlign: 'center', color: colors.cyanColor }}>Trie</th>
                <th style={{ padding: '8px', textAlign: 'center', color: colors.cyanColor }}>Hash Table</th>
                <th style={{ padding: '8px', textAlign: 'center', color: colors.cyanColor }}>Binary Search Tree</th>
              </tr>
            </thead>
            <tbody>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '8px', textAlign: 'left' }}>Insert</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m log n)</td>
              </tr>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '8px', textAlign: 'left' }}>Search</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m log n)</td>
              </tr>
              <tr style={{ borderBottom: '1px solid #333' }}>
                <td style={{ padding: '8px', textAlign: 'left' }}>Prefix Search</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m + k)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.pinkColor }}>O(n*m)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.yellowColor }}>O(m log n + k)</td>
              </tr>
              <tr>
                <td style={{ padding: '8px', textAlign: 'left' }}>Space</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.yellowColor }}>O(ALPHABET_SIZE * m * n)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m * n)</td>
                <td style={{ padding: '8px', textAlign: 'center', color: colors.greenColor }}>O(m * n)</td>
              </tr>
            </tbody>
          </table>
          
          <div style={{ 
            marginTop: '15px', 
            fontSize: '12px',
            color: '#999',
            fontStyle: 'italic',
            textAlign: 'left'
          }}>
            Note: m = word length, n = number of words, k = number of matches
          </div>
        </div>
      </div>
    );
  }
  
  // Render visualization tab content
  function renderVisualizationTab() {
    return (
      <div>
        {renderTrieVisualization()}
        {renderMessage()}
        {renderControls()}
        {renderWordList()}
        {renderSuggestions()}
        {renderAutoComplete()}
      </div>
    );
  }
  
  return (
    <div style={{ 
      backgroundColor: colors.bgColor, 
      color: '#fff',
      fontFamily: 'Arial, sans-serif',
      padding: '20px',
      borderRadius: '8px',
      minHeight: '700px',
      maxWidth: '900px',
      margin: '0 auto'
    }}>
      <div style={{ 
        textAlign: 'center',
        marginBottom: '20px'
      }}>
        <h1 style={{ 
          color: colors.cyanColor,
          margin: '0 0 10px 0',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          gap: '10px'
        }}>
          <BookOpen size={28} />
          Trie Data Structure
        </h1>
        <div style={{ 
          color: '#aaa',
          fontSize: '16px'
        }}>
          An efficient tree data structure for string operations
        </div>
      </div>
      
      <div style={{ 
        display: 'flex',
        backgroundColor: '#222',
        borderRadius: '8px',
        overflow: 'hidden',
        marginBottom: '20px'
      }}>
        <button 
          onClick={() => setActiveTab('visualization')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'visualization' ? '#333' : 'transparent',
            color: activeTab === 'visualization' ? colors.cyanColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'visualization' ? 'bold' : 'normal'
          }}
        >
          <PlayCircle size={16} style={{ marginRight: '5px', verticalAlign: 'middle' }} />
          Interactive
        </button>
        
        <button 
          onClick={() => setActiveTab('code')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'code' ? '#333' : 'transparent',
            color: activeTab === 'code' ? colors.yellowColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'code' ? 'bold' : 'normal'
          }}
        >
          <Code size={16} style={{ marginRight: '5px', verticalAlign: 'middle' }} />
          Implementation
        </button>
        
        <button 
          onClick={() => setActiveTab('applications')}
          style={{
            flex: 1,
            padding: '12px',
            backgroundColor: activeTab === 'applications' ? '#333' : 'transparent',
            color: activeTab === 'applications' ? colors.pinkColor : '#999',
            border: 'none',
            cursor: 'pointer',
            fontWeight: activeTab === 'applications' ? 'bold' : 'normal'
          }}
        >
          <Zap size={16} style={{ marginRight: '5px', verticalAlign: 'middle' }} />
          Applications
        </button>
      </div>
      
      <div style={{ 
        backgroundColor: '#1a1a1a',
        borderRadius: '8px',
        padding: '20px'
      }}>
        {activeTab === 'visualization' && renderVisualizationTab()}
        {activeTab === 'code' && renderCodeTab()}
        {activeTab === 'applications' && renderApplicationsTab()}
      </div>
      
      <div style={{ 
        textAlign: 'center',
        marginTop: '20px',
        fontSize: '14px',
        color: '#888'
      }}>
        Day 27 of #DSAin45 - Trie Data Structure
      </div>
    </div>
  );
};

export default TrieVisualization;