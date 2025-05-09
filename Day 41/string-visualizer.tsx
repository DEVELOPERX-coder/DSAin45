import React, { useState, useEffect, useRef } from 'react';

const StringAlgorithmVisualizer = () => {
  const [text, setText] = useState('ABABDABACDABABCABAB');
  const [pattern, setPattern] = useState('ABABCABAB');
  const [algorithm, setAlgorithm] = useState('kmp');
  const [speed, setSpeed] = useState(500);
  const [isPlaying, setIsPlaying] = useState(false);
  const [step, setStep] = useState(0);
  const [maxSteps, setMaxSteps] = useState(0);
  const [currentState, setCurrentState] = useState({
    text: '',
    textHighlights: [],
    pattern: '',
    patternHighlights: [],
    patternPosition: 0,
    info: '',
    matches: [],
    helperArray: [],
    helperArrayHighlights: []
  });
  const [showCode, setShowCode] = useState(false);
  const animationRef = useRef(null);

  // Initialize visualization state
  useEffect(() => {
    resetVisualization();
  }, [text, pattern, algorithm]);

  // Control animation playing
  useEffect(() => {
    if (isPlaying) {
      animationRef.current = setInterval(() => {
        setStep(prevStep => {
          if (prevStep >= maxSteps) {
            setIsPlaying(false);
            return prevStep;
          }
          return prevStep + 1;
        });
      }, speed);
    } else {
      clearInterval(animationRef.current);
    }

    return () => clearInterval(animationRef.current);
  }, [isPlaying, speed, maxSteps]);

  // Update visualization based on current step
  useEffect(() => {
    if (step <= maxSteps) {
      updateVisualizationState(step);
    }
  }, [step]);

  const resetVisualization = () => {
    setStep(0);
    setIsPlaying(false);
    
    // Initialize algorithm-specific state
    let steps = 0;
    let initialState = {
      text,
      textHighlights: [],
      pattern,
      patternHighlights: [],
      patternPosition: 0,
      info: 'Starting visualization',
      matches: [],
      helperArray: [],
      helperArrayHighlights: []
    };
    
    switch (algorithm) {
      case 'kmp':
        const lps = computeLPSArray(pattern);
        initialState.helperArray = lps;
        initialState.info = 'Preprocessing: Computing LPS array';
        steps = calculateKMPSteps(text, pattern, lps);
        break;
      case 'rabinkarp':
        initialState.info = 'Computing hash values for pattern and first window';
        steps = calculateRabinKarpSteps(text, pattern);
        break;
      case 'naive':
        initialState.info = 'Starting naive string matching';
        steps = calculateNaiveSteps(text, pattern);
        break;
      case 'z':
        initialState.helperArray = computeZArray(pattern + '$' + text);
        initialState.info = 'Preprocessing: Computing Z array';
        steps = calculateZSteps(text, pattern);
        break;
    }
    
    setMaxSteps(steps);
    setCurrentState(initialState);
  };

  const updateVisualizationState = (currentStep) => {
    // Update visualization state based on current step and algorithm
    let newState = { ...currentState };
    
    switch (algorithm) {
      case 'kmp':
        newState = updateKMPState(currentStep);
        break;
      case 'rabinkarp':
        newState = updateRabinKarpState(currentStep);
        break;
      case 'naive':
        newState = updateNaiveState(currentStep);
        break;
      case 'z':
        newState = updateZState(currentStep);
        break;
    }
    
    setCurrentState(newState);
  };

  // KMP Algorithm Logic
  const computeLPSArray = (pattern) => {
    const m = pattern.length;
    const lps = new Array(m).fill(0);
    
    let len = 0;
    let i = 1;
    
    while (i < m) {
      if (pattern[i] === pattern[len]) {
        len++;
        lps[i] = len;
        i++;
      } else {
        if (len !== 0) {
          len = lps[len - 1];
        } else {
          lps[i] = 0;
          i++;
        }
      }
    }
    
    return lps;
  };

  const calculateKMPSteps = (text, pattern, lps) => {
    // Each comparison, shift, and match is a step
    let steps = 0;
    let i = 0;
    let j = 0;
    
    while (i < text.length) {
      steps++; // Step for comparison
      
      if (pattern[j] === text[i]) {
        i++;
        j++;
      }
      
      if (j === pattern.length) {
        steps++; // Step for match
        j = lps[j - 1];
      } else if (i < text.length && pattern[j] !== text[i]) {
        steps++; // Step for shift
        if (j !== 0) {
          j = lps[j - 1];
        } else {
          i++;
        }
      }
    }
    
    // Add some extra steps for preprocessing visualization
    return steps + 5;
  };

  const updateKMPState = (currentStep) => {
    // Simplified simulation of KMP algorithm for visualization
    const newState = { ...currentState };
    const { text, pattern } = currentState;
    const lps = computeLPSArray(pattern);
    
    // Show LPS array computation for the first few steps
    if (currentStep < 5) {
      newState.info = `LPS array computation: Step ${currentStep + 1}`;
      newState.helperArray = lps;
      newState.helperArrayHighlights = [Math.min(currentStep, lps.length - 1)];
      newState.patternHighlights = [Math.min(currentStep, pattern.length - 1)];
      return newState;
    }
    
    // Simulate KMP search with the remaining steps
    let simStep = currentStep - 5;
    let i = 0;
    let j = 0;
    let matchesFound = [];
    let currentPositions = { i: 0, j: 0 };
    let currentInfo = '';
    
    let stepCounter = 0;
    while (i < text.length && stepCounter < simStep) {
      if (pattern[j] === text[i]) {
        i++;
        j++;
        stepCounter++;
        currentPositions = { i, j };
        currentInfo = `Match: text[${i-1}] = pattern[${j-1}] = '${text[i-1]}'`;
      }
      
      if (j === pattern.length) {
        matchesFound.push(i - j);
        currentInfo = `Match found at position ${i - j}`;
        j = lps[j - 1];
        stepCounter++;
        currentPositions = { i, j };
      } else if (i < text.length && pattern[j] !== text[i]) {
        currentInfo = `Mismatch at text[${i}] = '${text[i]}' and pattern[${j}] = '${pattern[j]}'`;
        if (j !== 0) {
          j = lps[j - 1];
          currentInfo += `. Shift pattern by using LPS[${j}] = ${lps[j]}`;
        } else {
          i++;
          currentInfo += `. j = 0, advancing text position`;
        }
        stepCounter++;
        currentPositions = { i, j };
      }
    }
    
    // Update visualization state
    newState.patternPosition = Math.max(0, currentPositions.i - currentPositions.j);
    newState.textHighlights = [];
    newState.patternHighlights = [];
    
    // Highlight current comparison position
    if (currentPositions.i < text.length) {
      newState.textHighlights.push(currentPositions.i);
    }
    
    if (currentPositions.j < pattern.length) {
      newState.patternHighlights.push(currentPositions.j);
    }
    
    // Highlight already matched part
    for (let k = 0; k < currentPositions.j; k++) {
      newState.textHighlights.push(currentPositions.i - currentPositions.j + k);
      newState.patternHighlights.push(k);
    }
    
    newState.info = currentInfo;
    newState.matches = matchesFound;
    
    return newState;
  };

  // Rabin-Karp Algorithm Logic
  const calculateRabinKarpSteps = (text, pattern) => {
    // Each window check and shift is a step
    const windowCount = text.length - pattern.length + 1;
    // Add extra steps for hash calculations and potential character comparisons
    return windowCount * 2 + 5;
  };

  const updateRabinKarpState = (currentStep) => {
    const newState = { ...currentState };
    const { text, pattern } = currentState;
    
    // First steps for hash calculations
    if (currentStep < 5) {
      newState.info = `Computing hash values for pattern and first window: Step ${currentStep + 1}`;
      newState.textHighlights = Array.from({ length: Math.min(pattern.length, text.length) }, (_, i) => i);
      newState.patternHighlights = Array.from({ length: pattern.length }, (_, i) => i);
      newState.patternPosition = 0;
      return newState;
    }
    
    // Simulate Rabin-Karp search with the remaining steps
    const simStep = Math.floor((currentStep - 5) / 2);
    const isComparing = (currentStep - 5) % 2 === 1;
    
    let matchesFound = [];
    let windowStart = simStep;
    
    // Check if the current window contains a match (simplified for visualization)
    let isMatch = true;
    for (let i = 0; i < pattern.length; i++) {
      if (windowStart + i >= text.length || text[windowStart + i] !== pattern[i]) {
        isMatch = false;
        break;
      }
    }
    
    // Collect all matches up to the current step
    for (let i = 0; i < windowStart; i++) {
      let windowMatch = true;
      for (let j = 0; j < pattern.length; j++) {
        if (i + j >= text.length || text[i + j] !== pattern[j]) {
          windowMatch = false;
          break;
        }
      }
      if (windowMatch) {
        matchesFound.push(i);
      }
    }
    
    if (isMatch) {
      matchesFound.push(windowStart);
    }
    
    // Update visualization state
    newState.patternPosition = windowStart;
    newState.textHighlights = [];
    newState.patternHighlights = [];
    
    if (isComparing) {
      // Highlight characters being compared
      for (let i = 0; i < pattern.length; i++) {
        if (windowStart + i < text.length) {
          newState.textHighlights.push(windowStart + i);
          newState.patternHighlights.push(i);
        }
      }
      newState.info = isMatch 
        ? `Hash values match at position ${windowStart}. Verifying characters: Match found!` 
        : `Hash values match at position ${windowStart}. Verifying characters: Mismatch detected.`;
    } else {
      // Highlight current window for hash calculation
      if (windowStart < text.length - pattern.length + 1) {
        for (let i = 0; i < pattern.length; i++) {
          if (windowStart + i < text.length) {
            newState.textHighlights.push(windowStart + i);
          }
        }
        newState.info = `Computing hash for window at position ${windowStart}`;
      } else {
        newState.info = `Search complete. Found ${matchesFound.length} matches.`;
      }
    }
    
    newState.matches = matchesFound;
    
    return newState;
  };

  // Naive Algorithm Logic
  const calculateNaiveSteps = (text, pattern) => {
    // Each character comparison is a step
    return (text.length - pattern.length + 1) * pattern.length + 1;
  };

  const updateNaiveState = (stepNum) => {
    const newState = { ...currentState };
    const { text, pattern } = currentState;
    
    if (stepNum === 0) {
      newState.info = 'Starting naive string matching';
      return newState;
    }
    
    // Calculate current window and character position
    const totalWindows = text.length - pattern.length + 1;
    const totalComparisons = pattern.length;
    
    let window = 0;
    let charPos = 0;
    let stepCount = stepNum - 1;
    
    while (stepCount >= totalComparisons) {
      window++;
      stepCount -= totalComparisons;
    }
    
    charPos = stepCount;
    
    // Check for matches up to the current position
    let matchesFound = [];
    for (let w = 0; w < window; w++) {
      let isMatch = true;
      for (let i = 0; i < pattern.length; i++) {
        if (text[w + i] !== pattern[i]) {
          isMatch = false;
          break;
        }
      }
      if (isMatch) matchesFound.push(w);
    }
    
    // Check current window up to current character
    let currentMatch = true;
    for (let i = 0; i <= charPos; i++) {
      if (text[window + i] !== pattern[i]) {
        currentMatch = false;
        break;
      }
    }
    
    // Update visualization state
    newState.patternPosition = window;
    newState.textHighlights = [window + charPos];
    newState.patternHighlights = [charPos];
    
    // Update information text
    if (charPos === pattern.length - 1 && currentMatch) {
      newState.info = `Match found at position ${window}`;
      matchesFound.push(window);
    } else if (currentMatch) {
      newState.info = `Comparing position ${window + charPos}: text[${window + charPos}] = '${text[window + charPos]}' matches pattern[${charPos}] = '${pattern[charPos]}'`;
    } else {
      newState.info = `Comparing position ${window + charPos}: text[${window + charPos}] = '${text[window + charPos]}' does NOT match pattern[${charPos}] = '${pattern[charPos]}'`;
    }
    
    newState.matches = matchesFound;
    
    return newState;
  };

  // Z Algorithm Logic
  const computeZArray = (s) => {
    const n = s.length;
    const z = new Array(n).fill(0);
    
    let left = 0, right = 0;
    for (let i = 1; i < n; i++) {
      if (i > right) {
        left = right = i;
        while (right < n && s[right - left] === s[right]) {
          right++;
        }
        z[i] = right - left;
        right--;
      } else {
        const k = i - left;
        if (z[k] < right - i + 1) {
          z[i] = z[k];
        } else {
          left = i;
          while (right < n && s[right - left] === s[right]) {
            right++;
          }
          z[i] = right - left;
          right--;
        }
      }
    }
    
    return z;
  };

  const calculateZSteps = (text, pattern) => {
    // Each Z-value computation is a step, plus some steps for showing matches
    return pattern.length + text.length + 5;
  };

  const updateZState = (currentStep) => {
    const newState = { ...currentState };
    const { text, pattern } = currentState;
    
    const concatString = pattern + '$' + text;
    const z = computeZArray(concatString);
    
    // Show Z array computation for the first few steps
    if (currentStep < 5) {
      newState.info = `Z array computation: Step ${currentStep + 1}`;
      newState.helperArray = z;
      const highlightIndex = Math.min(currentStep + 1, z.length - 1);
      newState.helperArrayHighlights = [highlightIndex];
      
      // Highlight the corresponding character in the concatenated string
      if (highlightIndex < pattern.length) {
        newState.patternHighlights = [highlightIndex];
      } else if (highlightIndex > pattern.length) {
        const textIndex = highlightIndex - pattern.length - 1;
        if (textIndex >= 0 && textIndex < text.length) {
          newState.textHighlights = [textIndex];
        }
      }
      
      return newState;
    }
    
    // Simulate Z algorithm search with the remaining steps
    const patternLength = pattern.length;
    let matchesFound = [];
    
    // Check Z values in the text part (after pattern + '$')
    for (let i = 0; i < z.length; i++) {
      if (z[i] === patternLength && i > patternLength) {
        matchesFound.push(i - patternLength - 1);
      }
    }
    
    // Show search process step by step
    const simStep = currentStep - 5;
    if (simStep < text.length) {
      const checkIndex = pattern.length + 1 + simStep;
      const zValue = z[checkIndex];
      
      newState.textHighlights = [simStep];
      
      if (zValue > 0) {
        // Highlight the pattern match
        for (let i = 0; i < zValue; i++) {
          if (simStep + i < text.length) {
            newState.textHighlights.push(simStep + i);
          }
          newState.patternHighlights.push(i);
        }
        
        newState.info = zValue === patternLength 
          ? `Z[${checkIndex}] = ${zValue} equals pattern length. Match found at position ${simStep}!` 
          : `Z[${checkIndex}] = ${zValue}. Partial match found.`;
      } else {
        newState.info = `Checking position ${simStep}: Z[${checkIndex}] = 0, no match starts here.`;
      }
      
      newState.patternPosition = simStep;
    } else {
      newState.info = `Search complete. Found ${matchesFound.length} matches.`;
    }
    
    newState.matches = matchesFound;
    
    return newState;
  };

  const getCodeSnippet = () => {
    switch (algorithm) {
      case 'kmp':
        return `// KMP Search
vector<int> KMPSearch(const string& text, const string& pattern) {
  vector<int> matches;
  int n = text.length();
  int m = pattern.length();
  
  // Preprocess pattern to build LPS array
  vector<int> lps(m);
  computeLPSArray(pattern, lps);
  
  int i = 0;  // Index for text
  int j = 0;  // Index for pattern
  
  while (i < n) {
    if (pattern[j] == text[i]) {
      i++;
      j++;
    }
    
    if (j == m) {
      // Found a match
      matches.push_back(i - j);
      j = lps[j-1];
    } else if (i < n && pattern[j] != text[i]) {
      if (j != 0) {
        j = lps[j-1];
      } else {
        i++;
      }
    }
  }
  
  return matches;
}

// LPS array computation
void computeLPSArray(const string& pattern, vector<int>& lps) {
  int m = pattern.length();
  int len = 0;  // Length of previous longest prefix suffix
  
  lps[0] = 0;  // LPS of first char is always 0
  
  int i = 1;
  while (i < m) {
    if (pattern[i] == pattern[len]) {
      len++;
      lps[i] = len;
      i++;
    } else {
      if (len != 0) {
        len = lps[len-1];
      } else {
        lps[i] = 0;
        i++;
      }
    }
  }
}`;
      case 'rabinkarp':
        return `// Rabin-Karp Search
vector<int> rabinKarp(const string& text, const string& pattern) {
  vector<int> matches;
  int n = text.length();
  int m = pattern.length();
  
  const int d = 256;  // Number of characters in alphabet
  const int q = 101;  // A prime number
  
  // Calculate hash for pattern and first window of text
  int patternHash = 0;
  int textHash = 0;
  int h = 1;  // d^(m-1) % q
  
  // Calculate h = d^(m-1) % q
  for (int i = 0; i < m - 1; i++) {
    h = (h * d) % q;
  }
  
  // Calculate initial hash values
  for (int i = 0; i < m; i++) {
    patternHash = (d * patternHash + pattern[i]) % q;
    textHash = (d * textHash + text[i]) % q;
  }
  
  // Slide pattern over text
  for (int i = 0; i <= n - m; i++) {
    // Check if hash values match
    if (patternHash == textHash) {
      // Verify character by character
      bool match = true;
      for (int j = 0; j < m; j++) {
        if (text[i + j] != pattern[j]) {
          match = false;
          break;
        }
      }
      if (match) {
        matches.push_back(i);
      }
    }
    
    // Calculate hash for next window
    if (i < n - m) {
      textHash = (d * (textHash - text[i] * h) + text[i + m]) % q;
      if (textHash < 0) {
        textHash += q;
      }
    }
  }
  
  return matches;
}`;
      case 'naive':
        return `// Naive String Matching
vector<int> naiveSearch(const string& text, const string& pattern) {
  vector<int> matches;
  int n = text.length();
  int m = pattern.length();
  
  // Check each possible position in text
  for (int i = 0; i <= n - m; i++) {
    int j;
    // Check if pattern matches at position i
    for (j = 0; j < m; j++) {
      if (text[i + j] != pattern[j]) {
        break;
      }
    }
    
    // If we checked all characters, it's a match
    if (j == m) {
      matches.push_back(i);
    }
  }
  
  return matches;
}`;
      case 'z':
        return `// Z Algorithm Search
vector<int> zSearch(const string& text, const string& pattern) {
  vector<int> matches;
  
  // Concatenate pattern and text with separator
  string concat = pattern + "$" + text;
  
  // Compute Z array
  vector<int> z = computeZArray(concat);
  
  // Find matches
  for (int i = 0; i < concat.length(); i++) {
    if (z[i] == pattern.length()) {
      int pos = i - pattern.length() - 1;
      if (pos >= 0) {
        matches.push_back(pos);
      }
    }
  }
  
  return matches;
}

// Compute Z array
vector<int> computeZArray(const string& s) {
  int n = s.length();
  vector<int> z(n, 0);
  
  int L = 0, R = 0;
  for (int i = 1; i < n; i++) {
    if (i > R) {
      L = R = i;
      while (R < n && s[R-L] == s[R]) {
        R++;
      }
      z[i] = R - L;
      R--;
    } else {
      int k = i - L;
      if (z[k] < R - i + 1) {
        z[i] = z[k];
      } else {
        L = i;
        while (R < n && s[R-L] == s[R]) {
          R++;
        }
        z[i] = R - L;
        R--;
      }
    }
  }
  
  return z;
}`;
      default:
        return '// Select an algorithm to see the code';
    }
  };

  const renderCharBox = (char, highlight = false, match = false) => {
    let bgColor = 'bg-gray-800';
    if (highlight && match) {
      bgColor = 'bg-green-500';
    } else if (highlight) {
      bgColor = 'bg-pink-500';
    } else if (match) {
      bgColor = 'bg-cyan-600 bg-opacity-60';
    }
    
    return (
      <div className={`w-8 h-8 flex items-center justify-center border ${bgColor}`}>
        <span className="text-lg">{char}</span>
      </div>
    );
  };

  const renderText = () => {
    const { text, textHighlights, matches, patternPosition, pattern } = currentState;
    
    return (
      <div className="flex flex-wrap">
        {text.split('').map((char, index) => {
          const isHighlighted = textHighlights.includes(index);
          const isMatched = matches.some(matchPos => 
            index >= matchPos && index < matchPos + pattern.length
          );
          return (
            <div key={index} className="relative">
              {index === patternPosition && (
                <div className="absolute -top-6 left-0 w-8 flex justify-center">
                  <span className="text-cyan-400">↓</span>
                </div>
              )}
              {renderCharBox(char, isHighlighted, isMatched)}
              <div className="text-xs text-center">{index}</div>
            </div>
          );
        })}
      </div>
    );
  };

  const renderPattern = () => {
    const { pattern, patternHighlights } = currentState;
    
    return (
      <div className="flex">
        {pattern.split('').map((char, index) => (
          <div key={index} className="relative">
            {renderCharBox(char, patternHighlights.includes(index))}
            <div className="text-xs text-center">{index}</div>
          </div>
        ))}
      </div>
    );
  };

  const renderHelperArray = () => {
    const { helperArray, helperArrayHighlights } = currentState;
    
    if (helperArray.length === 0) return null;
    
    const arrayName = algorithm === 'kmp' ? 'LPS Array' : 
                     algorithm === 'z' ? 'Z Array' : 'Helper Array';
    
    return (
      <div className="mt-4">
        <h3 className="font-semibold text-green-400 mb-2">{arrayName}</h3>
        <div className="flex">
          {helperArray.map((value, index) => (
            <div key={index} className="flex flex-col items-center mr-1">
              <div className={`w-8 h-8 flex items-center justify-center border ${
                helperArrayHighlights.includes(index) ? 'bg-yellow-500' : 'bg-gray-800'
              }`}>
                {value}
              </div>
              <div className="text-xs">{index}</div>
            </div>
          ))}
        </div>
      </div>
    );
  };

  return (
    <div className="p-6 w-full bg-gray-900 text-white min-h-screen">
      <div className="max-w-4xl mx-auto">
        <h1 className="text-3xl font-bold text-center mb-6 text-cyan-400">String Algorithm Visualizer</h1>
        
        <div className="bg-gray-800 p-6 rounded-lg shadow-lg mb-6">
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            <div>
              <h2 className="text-xl font-semibold mb-4 text-yellow-300">Input</h2>
              
              <div className="mb-4">
                <label className="block mb-2">Text:</label>
                <input 
                  type="text" 
                  value={text} 
                  onChange={(e) => setText(e.target.value)} 
                  className="w-full p-2 bg-gray-700 text-white rounded"
                />
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Pattern:</label>
                <input 
                  type="text" 
                  value={pattern} 
                  onChange={(e) => setPattern(e.target.value)} 
                  className="w-full p-2 bg-gray-700 text-white rounded"
                />
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Algorithm:</label>
                <select 
                  value={algorithm} 
                  onChange={(e) => setAlgorithm(e.target.value)}
                  className="w-full p-2 bg-gray-700 text-white rounded"
                >
                  <option value="kmp">Knuth-Morris-Pratt (KMP)</option>
                  <option value="rabinkarp">Rabin-Karp</option>
                  <option value="naive">Naive Search</option>
                  <option value="z">Z Algorithm</option>
                </select>
              </div>
            </div>
            
            <div>
              <h2 className="text-xl font-semibold mb-4 text-green-400">Visualization</h2>
              
              <div className="mb-4">
                <label className="block mb-2">Text:</label>
                <div className="overflow-x-auto pb-2">
                  {renderText()}
                </div>
              </div>
              
              <div className="mb-4">
                <label className="block mb-2">Pattern:</label>
                <div className="overflow-x-auto pb-2">
                  {renderPattern()}
                </div>
              </div>
              
              {renderHelperArray()}
              
              <div className="mt-4">
                <label className="block mb-2">Information:</label>
                <div className="p-2 bg-gray-700 rounded min-h-12">{currentState.info}</div>
              </div>
              
              <div className="mt-4">
                <label className="block mb-2">Matches Found:</label>
                <div className="p-2 bg-gray-700 rounded">
                  {currentState.matches.length > 0 
                    ? currentState.matches.map(pos => pos).join(', ')
                    : 'No matches yet'}
                </div>
              </div>
            </div>
          </div>
          
          <div className="mt-6">
            <div className="flex justify-center space-x-4 mb-4">
              <button 
                onClick={() => {
                  setStep(Math.max(0, step - 1));
                }}
                disabled={step <= 0}
                className="px-4 py-2 bg-gray-700 rounded-md hover:bg-gray-600 disabled:opacity-50"
              >
                ⏮ Back
              </button>
              
              {isPlaying ? (
                <button 
                  onClick={() => setIsPlaying(false)}
                  className="px-4 py-2 bg-pink-600 rounded-md hover:bg-pink-500"
                >
                  ⏸ Pause
                </button>
              ) : (
                <button 
                  onClick={() => setIsPlaying(true)}
                  disabled={step >= maxSteps}
                  className="px-4 py-2 bg-cyan-600 rounded-md hover:bg-cyan-500 disabled:opacity-50"
                >
                  ▶️ Play
                </button>
              )}
              
              <button 
                onClick={() => {
                  setStep(Math.min(maxSteps, step + 1));
                }}
                disabled={step >= maxSteps}
                className="px-4 py-2 bg-gray-700 rounded-md hover:bg-gray-600 disabled:opacity-50"
              >
                ⏭ Forward
              </button>
              
              <button 
                onClick={resetVisualization}
                className="px-4 py-2 bg-yellow-600 rounded-md hover:bg-yellow-500"
              >
                🔄 Reset
              </button>
            </div>
            
            <div className="mb-4">
              <label className="block mb-2">Animation Speed:</label>
              <input 
                type="range" 
                min="100" 
                max="2000" 
                step="100" 
                value={speed} 
                onChange={(e) => setSpeed(parseInt(e.target.value))} 
                className="w-full"
              />
              <div className="flex justify-between text-xs">
                <span>Fast</span>
                <span>Slow</span>
              </div>
            </div>
            
            <div className="mb-4">
              <label className="block mb-2">Progress:</label>
              <div className="w-full bg-gray-700 rounded-full h-2.5">
                <div 
                  className="bg-cyan-400 h-2.5 rounded-full" 
                  style={{ width: `${(step / maxSteps) * 100}%` }}
                ></div>
              </div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 p-6 rounded-lg shadow-lg">
          <div className="flex justify-between items-center mb-4">
            <h2 className="text-xl font-semibold text-pink-400">Algorithm Implementation</h2>
            <button 
              onClick={() => setShowCode(!showCode)}
              className="px-3 py-1 bg-gray-700 text-sm rounded hover:bg-gray-600"
            >
              {showCode ? 'Hide Code' : 'Show Code'}
            </button>
          </div>
          
          {showCode && (
            <div className="bg-gray-900 p-4 rounded-md overflow-x-auto">
              <pre className="text-green-400">
                {getCodeSnippet()}
              </pre>
            </div>
          )}
          
          <div className="mt-6">
            <h3 className="font-semibold text-yellow-300 mb-2">Time & Space Complexity</h3>
            <table className="w-full border-collapse">
              <thead>
                <tr>
                  <th className="border border-gray-600 p-2 text-left">Algorithm</th>
                  <th className="border border-gray-600 p-2 text-left">Time Complexity</th>
                  <th className="border border-gray-600 p-2 text-left">Space Complexity</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td className="border border-gray-600 p-2">Naive</td>
                  <td className="border border-gray-600 p-2">O(n×m)</td>
                  <td className="border border-gray-600 p-2">O(1)</td>
                </tr>
                <tr>
                  <td className="border border-gray-600 p-2">KMP</td>
                  <td className="border border-gray-600 p-2">O(n+m)</td>
                  <td className="border border-gray-600 p-2">O(m)</td>
                </tr>
                <tr>
                  <td className="border border-gray-600 p-2">Rabin-Karp</td>
                  <td className="border border-gray-600 p-2">O(n×m) worst, O(n+m) average</td>
                  <td className="border border-gray-600 p-2">O(1)</td>
                </tr>
                <tr>
                  <td className="border border-gray-600 p-2">Z Algorithm</td>
                  <td className="border border-gray-600 p-2">O(n+m)</td>
                  <td className="border border-gray-600 p-2">O(n+m)</td>
                </tr>
              </tbody>
            </table>
          </div>
          
          <div className="mt-6">
            <h3 className="font-semibold text-yellow-300 mb-2">Key Insights</h3>
            <ul className="list-disc list-inside space-y-2">
              <li>KMP avoids redundant comparisons by using information from previous matches</li>
              <li>Rabin-Karp uses hashing to quickly filter non-matches</li>
              <li>Z Algorithm provides pattern searching with a cleaner implementation than KMP</li>
              <li>For small patterns or single searches, the overhead of preprocessing may not be worth it</li>
              <li>For multiple pattern searches in the same text, consider Aho-Corasick algorithm</li>
            </ul>
          </div>
          
          <div className="mt-6">
            <h3 className="font-semibold text-yellow-300 mb-2">Real-World Applications</h3>
            <ul className="list-disc list-inside space-y-2">
              <li>Text editors (search and replace functionality)</li>
              <li>Bioinformatics (DNA sequence analysis)</li>
              <li>Plagiarism detection systems</li>
              <li>Search engines and information retrieval</li>
              <li>Network intrusion detection systems</li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  );
};

export default StringAlgorithmVisualizer;
