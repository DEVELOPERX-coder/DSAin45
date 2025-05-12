import React, { useState } from 'react';

const InterviewSimulator = () => {
  // State for the current step in the interview process
  const [currentStep, setCurrentStep] = useState(0);
  const [speed, setSpeed] = useState(1);
  const [isPlaying, setIsPlaying] = useState(false);
  const [showCode, setShowCode] = useState(false);
  const [activePattern, setActivePattern] = useState('twosum');
  
  // Interview process steps
  const steps = [
    { 
      name: "Understand", 
      time: "1-2 minutes",
      description: "Clarify the problem statement completely",
      tasks: [
        "Restate the problem in your own words",
        "Identify input/output formats",
        "Ask for edge cases and constraints",
        "Verify examples"
      ],
      code: "",
      visual: (
        <div className="flex flex-col items-center">
          <div className="w-64 h-32 bg-gray-800 rounded flex items-center justify-center mb-4">
            <div className="text-cyan-400 font-mono text-sm">
              Input: nums = [2,7,11,15], target = 9
              <br />
              Output: [0,1]
            </div>
          </div>
          <div className="flex space-x-4">
            <div className="p-2 bg-gray-800 rounded border border-pink-500">
              <span className="text-pink-500">Edge Case</span>
              <br />
              <span className="text-white text-sm">Empty array</span>
            </div>
            <div className="p-2 bg-gray-800 rounded border border-yellow-500">
              <span className="text-yellow-500">Constraint</span>
              <br />
              <span className="text-white text-sm">2 ≤ nums.length ≤ 10<sup>4</sup></span>
            </div>
          </div>
        </div>
      )
    },
    { 
      name: "Strategize", 
      time: "2-3 minutes",
      description: "Develop and discuss solution approaches",
      tasks: [
        "Start with brute force approach",
        "Identify optimizations",
        "Analyze time & space complexity",
        "Decide on best approach"
      ],
      code: "// Brute Force O(n²)\nfor (int i = 0; i < nums.size(); i++) {\n  for (int j = i + 1; j < nums.size(); j++) {\n    if (nums[i] + nums[j] == target) {\n      return {i, j};\n    }\n  }\n}\n\n// Optimized O(n)\nusing hash map to store visited numbers",
      visual: (
        <div className="flex flex-col items-center">
          <div className="flex space-x-4 mb-4">
            <div className="p-3 bg-gray-800 rounded border border-pink-500">
              <span className="text-pink-500">Brute Force</span>
              <br />
              <span className="text-white text-xs">O(n²) time</span>
              <br />
              <span className="text-white text-xs">O(1) space</span>
            </div>
            <div className="p-3 bg-gray-800 rounded border border-green-500">
              <span className="text-green-500">Optimized</span>
              <br />
              <span className="text-white text-xs">O(n) time</span>
              <br />
              <span className="text-white text-xs">O(n) space</span>
            </div>
          </div>
          <div className="w-64 h-32 bg-gray-800 rounded flex items-center justify-center">
            <div className="text-yellow-400 font-mono text-sm">
              HashMap&lt;value, index&gt;
              <br />
              Lookup: target - nums[i]
            </div>
          </div>
        </div>
      )
    },
    { 
      name: "Code", 
      time: "8-10 minutes",
      description: "Implement your solution clearly and correctly",
      tasks: [
        "Use meaningful variable names",
        "Handle edge cases",
        "Write clean, modular code",
        "Comment key steps"
      ],
      code: "vector<int> twoSum(vector<int>& nums, int target) {\n  // Edge case\n  if (nums.size() < 2) return {};\n  \n  // Use hash map for O(1) lookups\n  unordered_map<int, int> numToIndex;\n  \n  for (int i = 0; i < nums.size(); i++) {\n    int complement = target - nums[i];\n    \n    // Check if we've seen the complement\n    if (numToIndex.count(complement)) {\n      return {numToIndex[complement], i};\n    }\n    \n    // Store current number and its index\n    numToIndex[nums[i]] = i;\n  }\n  \n  return {}; // No solution found\n}",
      visual: (
        <div className="flex flex-col items-center">
          <div className="w-64 h-48 bg-gray-800 rounded flex flex-col p-2 mb-4">
            <div className="text-white font-mono text-xs mb-2">nums = [2,7,11,15], target = 9</div>
            <div className="flex flex-col space-y-2">
              <div className="flex space-x-2">
                <div className="w-6 h-6 bg-gray-700 flex items-center justify-center text-cyan-400">2</div>
                <div className="w-6 h-6 bg-gray-700 flex items-center justify-center text-cyan-400">7</div>
                <div className="w-6 h-6 bg-gray-700 flex items-center justify-center text-cyan-400">11</div>
                <div className="w-6 h-6 bg-gray-700 flex items-center justify-center text-cyan-400">15</div>
              </div>
              <div className="mt-2 p-2 bg-gray-700 rounded text-pink-400 text-xs">
                HashMap: {'{'}2→0, 7→1{'}'}
              </div>
              <div className="text-yellow-400 text-xs">
                complement = 9 - 7 = 2
                <br />
                2 found in map at index 0
                <br />
                return [0, 1]
              </div>
            </div>
          </div>
        </div>
      )
    },
    { 
      name: "Test", 
      time: "3-4 minutes",
      description: "Verify your solution with examples and edge cases",
      tasks: [
        "Trace through with examples",
        "Test edge cases (empty, single element)",
        "Check for off-by-one errors",
        "Verify time/space complexity"
      ],
      code: "// Test case: nums = [2,7,11,15], target = 9\n// i=0: nums[0]=2, complement=7, map={}, add 2->0 to map\n// i=1: nums[1]=7, complement=2, found in map at index 0, return [0,1]\n\n// Edge case: empty array -> returns empty result\n// Edge case: no solution -> returns empty result",
      visual: (
        <div className="flex flex-col items-center">
          <div className="w-64 mb-4 bg-gray-800 rounded p-3">
            <div className="text-green-400 font-bold mb-2">Test Cases</div>
            <div className="space-y-2 text-xs">
              <div className="p-1 border border-green-500 rounded">
                <span className="text-white">nums=[2,7,11,15], target=9</span>
                <br />
                <span className="text-green-400">✓ Returns [0,1]</span>
              </div>
              <div className="p-1 border border-green-500 rounded">
                <span className="text-white">nums=[], target=1</span>
                <br />
                <span className="text-green-400">✓ Returns []</span>
              </div>
              <div className="p-1 border border-green-500 rounded">
                <span className="text-white">nums=[3,3], target=6</span>
                <br />
                <span className="text-green-400">✓ Returns [0,1]</span>
              </div>
            </div>
          </div>
          <div className="w-64 bg-gray-800 rounded p-3">
            <div className="text-yellow-400 font-bold mb-2">Complexity</div>
            <div className="text-white text-xs">
              Time: O(n) - single pass through array
              <br />
              Space: O(n) - hash map stores at most n elements
            </div>
          </div>
        </div>
      )
    },
    { 
      name: "Optimize", 
      time: "1-2 minutes",
      description: "Discuss potential improvements and alternatives",
      tasks: [
        "Suggest further optimizations",
        "Consider space-time tradeoffs",
        "Discuss alternative approaches",
        "Address interviewer questions"
      ],
      code: "// Optimization for sorted arrays:\n// Use two pointers approach\n// Time: O(n), Space: O(1)\n\n// If input array is very large:\n// Consider external sorting/processing\n\n// If called frequently:\n// Consider caching results",
      visual: (
        <div className="flex flex-col items-center">
          <div className="w-64 h-48 bg-gray-800 rounded p-3">
            <div className="text-pink-400 font-bold mb-2">Optimizations</div>
            <div className="space-y-3 text-xs">
              <div className="p-1 border border-pink-500 rounded">
                <span className="text-white">If input is sorted:</span>
                <br />
                <span className="text-cyan-400">Use two-pointer technique</span>
                <br />
                <span className="text-green-400">O(n) time, O(1) space</span>
              </div>
              <div className="p-1 border border-yellow-500 rounded">
                <span className="text-white">For repeated calls:</span>
                <br />
                <span className="text-cyan-400">Consider caching results</span>
              </div>
            </div>
          </div>
        </div>
      )
    }
  ];
  
  // Interview pattern data
  const patterns = {
    twosum: {
      name: "Two Sum Problem",
      description: "Find indices of two numbers that add up to target",
      complexity: "O(n) time, O(n) space with hash map"
    },
    slidingwindow: {
      name: "Sliding Window",
      description: "Find longest substring without repeating characters",
      complexity: "O(n) time, O(min(m,n)) space"
    },
    binarysearch: {
      name: "Binary Search",
      description: "Search in rotated sorted array",
      complexity: "O(log n) time, O(1) space"
    },
    dynamicprogramming: {
      name: "Dynamic Programming",
      description: "Coin change problem (min coins for amount)",
      complexity: "O(amount * coins) time, O(amount) space"
    }
  };
  
  // Handle step navigation
  const goToStep = (step) => {
    setCurrentStep(step);
  };
  
  const nextStep = () => {
    if (currentStep < steps.length - 1) {
      setCurrentStep(currentStep + 1);
    }
  };
  
  const prevStep = () => {
    if (currentStep > 0) {
      setCurrentStep(currentStep - 1);
    }
  };
  
  // Toggle play/pause automation
  const togglePlay = () => {
    setIsPlaying(!isPlaying);
    
    if (!isPlaying && currentStep < steps.length - 1) {
      // Start auto-advance timer
      const timer = setInterval(() => {
        setCurrentStep(prevStep => {
          const nextStep = prevStep + 1;
          if (nextStep >= steps.length) {
            clearInterval(timer);
            setIsPlaying(false);
            return prevStep;
          }
          return nextStep;
        });
      }, 3000 / speed);
      
      return () => clearInterval(timer);
    }
  };
  
  // Handle speed changes
  const changeSpeed = (newSpeed) => {
    setSpeed(newSpeed);
  };
  
  // Current step data
  const currentStepData = steps[currentStep];
  const currentPattern = patterns[activePattern];
  
  return (
    <div className="font-sans min-h-screen bg-gray-900 text-white p-4">
      <div className="max-w-5xl mx-auto">
        <div className="mb-6 text-center">
          <h1 className="text-2xl font-bold text-cyan-400">Technical Interview Process Simulator</h1>
          <p className="text-gray-400">Day 44: #DSAin45 - Interview Preparation Strategies</p>
        </div>
        
        {/* Pattern selection */}
        <div className="mb-6 bg-gray-800 p-4 rounded-lg">
          <h2 className="text-pink-400 font-bold mb-2">Common Interview Patterns</h2>
          <div className="flex flex-wrap gap-2">
            {Object.keys(patterns).map(key => (
              <button
                key={key}
                className={`px-3 py-2 rounded text-sm ${activePattern === key ? 'bg-cyan-500 text-white' : 'bg-gray-700 text-gray-300'}`}
                onClick={() => setActivePattern(key)}
              >
                {patterns[key].name}
              </button>
            ))}
          </div>
          <div className="mt-3 p-3 bg-gray-700 rounded">
            <div className="text-yellow-400 font-bold">{currentPattern.name}</div>
            <div className="text-sm text-gray-300">{currentPattern.description}</div>
            <div className="text-xs text-green-400 mt-1">{currentPattern.complexity}</div>
          </div>
        </div>
        
        {/* Progress bar */}
        <div className="mb-6 bg-gray-800 p-2 rounded-lg">
          <div className="flex justify-between mb-2">
            <div className="text-sm text-gray-400">Interview Progress</div>
            <div className="text-sm text-gray-400">{currentStep + 1} of {steps.length}</div>
          </div>
          <div className="h-4 w-full bg-gray-700 rounded-full overflow-hidden">
            <div 
              className="h-full bg-gradient-to-r from-cyan-500 to-pink-500" 
              style={{ width: `${((currentStep + 1) / steps.length) * 100}%` }}
            />
          </div>
          <div className="flex justify-between mt-2">
            {steps.map((step, index) => (
              <button
                key={index}
                onClick={() => goToStep(index)}
                className={`flex-1 text-xs py-1 mx-0.5 rounded ${currentStep === index ? 'bg-pink-500 text-white' : 'bg-gray-700 text-gray-400 hover:bg-gray-600'}`}
              >
                {step.name}
              </button>
            ))}
          </div>
        </div>
        
        {/* Main content area */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          {/* Left column - Visualization */}
          <div className="bg-gray-800 p-4 rounded-lg">
            <div className="flex justify-between items-center mb-4">
              <h2 className="text-lg font-bold text-cyan-400">{currentStepData.name} <span className="text-sm text-gray-400">({currentStepData.time})</span></h2>
              <div className="text-xs text-yellow-400 font-mono px-2 py-1 bg-gray-700 rounded">
                {currentStepData.time}
              </div>
            </div>
            
            <p className="text-sm text-gray-300 mb-4">{currentStepData.description}</p>
            
            <div className="flex flex-wrap gap-2 mb-4">
              {currentStepData.tasks.map((task, index) => (
                <div key={index} className="bg-gray-700 text-xs px-2 py-1 rounded text-white">
                  • {task}
                </div>
              ))}
            </div>
            
            <div className="mt-4 border border-gray-700 rounded-lg p-4 min-h-64 flex items-center justify-center">
              {currentStepData.visual}
            </div>
          </div>
          
          {/* Right column - Code view */}
          <div className="bg-gray-800 p-4 rounded-lg">
            <div className="flex justify-between items-center mb-4">
              <h2 className="text-lg font-bold text-pink-400">Code Implementation</h2>
              <div className="flex items-center">
                <button
                  onClick={() => setShowCode(!showCode)}
                  className="text-xs px-3 py-1 bg-gray-700 rounded text-cyan-400 hover:bg-gray-600"
                >
                  {showCode ? "Hide Code" : "Show Code"}
                </button>
              </div>
            </div>
            
            {showCode ? (
              <pre className="bg-gray-900 p-4 rounded text-green-400 text-xs font-mono overflow-x-auto h-64">
                {currentStepData.code}
              </pre>
            ) : (
              <div className="bg-gray-900 p-4 rounded h-64 flex flex-col items-center justify-center">
                <div className="text-gray-400 mb-4">Code view is hidden</div>
                <button
                  onClick={() => setShowCode(true)}
                  className="px-4 py-2 bg-gray-700 rounded text-cyan-400 hover:bg-gray-600"
                >
                  Show Code
                </button>
              </div>
            )}
            
            <div className="mt-4">
              <h3 className="text-sm font-bold text-yellow-400 mb-2">Interview Tips</h3>
              <div className="bg-gray-700 p-3 rounded text-xs text-white">
                <p className="mb-2">• <span className="text-pink-400">Communication is key:</span> Explain your thought process clearly.</p>
                <p className="mb-2">• <span className="text-cyan-400">Start simple:</span> Begin with a working solution, then optimize.</p>
                <p className="mb-2">• <span className="text-yellow-400">Test thoroughly:</span> Consider edge cases and verify correctness.</p>
                <p>• <span className="text-green-400">Time management:</span> Allocate time appropriately for each step.</p>
              </div>
            </div>
          </div>
        </div>
        
        {/* Playback controls */}
        <div className="mt-6 bg-gray-800 p-4 rounded-lg flex flex-wrap items-center justify-between">
          <div className="flex space-x-2">
            <button
              onClick={prevStep}
              disabled={currentStep === 0}
              className={`px-4 py-2 rounded ${currentStep === 0 ? 'bg-gray-700 text-gray-500' : 'bg-gray-700 text-white hover:bg-gray-600'}`}
            >
              ← Previous
            </button>
            <button
              onClick={togglePlay}
              className="px-4 py-2 bg-cyan-600 text-white rounded hover:bg-cyan-500"
            >
              {isPlaying ? "Pause" : "Play"}
            </button>
            <button
              onClick={nextStep}
              disabled={currentStep === steps.length - 1}
              className={`px-4 py-2 rounded ${currentStep === steps.length - 1 ? 'bg-gray-700 text-gray-500' : 'bg-gray-700 text-white hover:bg-gray-600'}`}
            >
              Next →
            </button>
          </div>
          
          <div className="flex items-center space-x-2 mt-2 sm:mt-0">
            <span className="text-sm text-gray-400">Speed:</span>
            <button
              onClick={() => changeSpeed(0.5)}
              className={`px-2 py-1 rounded text-xs ${speed === 0.5 ? 'bg-pink-500 text-white' : 'bg-gray-700 text-gray-300'}`}
            >
              0.5x
            </button>
            <button
              onClick={() => changeSpeed(1)}
              className={`px-2 py-1 rounded text-xs ${speed === 1 ? 'bg-pink-500 text-white' : 'bg-gray-700 text-gray-300'}`}
            >
              1x
            </button>
            <button
              onClick={() => changeSpeed(2)}
              className={`px-2 py-1 rounded text-xs ${speed === 2 ? 'bg-pink-500 text-white' : 'bg-gray-700 text-gray-300'}`}
            >
              2x
            </button>
          </div>
        </div>
        
        <div className="mt-6 text-center text-xs text-gray-500">
          #DSAin45 Day 44: Interview Preparation Strategies
        </div>
      </div>
    </div>
  );
};

export default InterviewSimulator;