import React from 'react';
import { ArrowLeft, ArrowRight, MoveRight, MoveLeft, Maximize2, MinusCircle, PlusCircle } from 'lucide-react';

// Main component for Two-Pointer Technique Visualizations
const TwoPointerVisualizations = () => {
  return (
    <div className="flex flex-col items-center justify-center w-full space-y-8 bg-gray-900 p-6 rounded-lg">
      <header className="w-full text-center mb-4">
        <h1 className="text-3xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-cyan-400 via-pink-500 to-yellow-300">
          Two-Pointer Technique Visualizations
        </h1>
        <p className="text-gray-300 mt-2">Day 20 of #DSAin45</p>
      </header>

      {/* Main visualization section */}
      <div className="grid grid-cols-1 gap-8 w-full">
        {/* Converging Pattern Visualization */}
        <ConvergingPointersVisualization />
        
        {/* Fast & Slow Pattern Visualization */}
        <FastSlowPointersVisualization />
        
        {/* Sliding Window Pattern Visualization */}
        <SlidingWindowVisualization />
        
        {/* Complexity Chart */}
        <ComplexityChart />
        
        {/* Two-Pointer Meme */}
        <TwoPointerMeme />
      </div>
    </div>
  );
};

// Visualization for the Converging Pointers Pattern
const ConvergingPointersVisualization = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Converging Pointers Pattern</h2>
      
      <div className="flex flex-col items-center">
        <div className="flex items-center justify-center w-full my-6 relative">
          {/* Array visualization */}
          <div className="flex items-center justify-center relative">
            {[2, 7, 11, 15, 19, 21].map((value, index) => (
              <div 
                key={index} 
                className={`w-12 h-12 m-1 flex items-center justify-center font-bold rounded
                  ${index === 0 ? 'bg-pink-500 text-white' : 
                    index === 5 ? 'bg-yellow-300 text-gray-900' : 
                    'bg-gray-700 text-gray-300'}`}
              >
                {value}
              </div>
            ))}
            
            {/* Left pointer */}
            <div className="absolute -bottom-10 left-2 text-pink-500 flex flex-col items-center">
              <ArrowUp />
              <span className="text-sm">left</span>
            </div>
            
            {/* Right pointer */}
            <div className="absolute -bottom-10 right-2 text-yellow-300 flex flex-col items-center">
              <ArrowUp />
              <span className="text-sm">right</span>
            </div>
          </div>
        </div>
        
        <div className="text-gray-300 text-center mt-4 max-w-md">
          <p>The converging pattern places pointers at opposite ends. They move toward each other until they meet or a condition is satisfied. Ideal for problems on sorted arrays.</p>
          <div className="mt-2 text-sm bg-gray-700 p-2 rounded">
            <p className="text-cyan-400">Time Complexity: O(n)</p>
            <p className="text-pink-400">Space Complexity: O(1)</p>
          </div>
        </div>
      </div>
    </div>
  );
};

// Arrow component for visualizations
const ArrowUp = () => (
  <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
    <line x1="12" y1="19" x2="12" y2="5" />
    <polyline points="5 12 12 5 19 12" />
  </svg>
);

// Visualization for the Fast & Slow Pointers Pattern
const FastSlowPointersVisualization = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-pink-500">
      <h2 className="text-xl font-bold text-pink-400 mb-4">Fast & Slow Pointers Pattern</h2>
      
      <div className="flex flex-col items-center">
        <div className="w-full my-6 relative">
          {/* Linked list visualization */}
          <div className="flex items-center justify-center">
            {[3, 2, 0, 4].map((value, index) => (
              <React.Fragment key={index}>
                <div 
                  className={`w-12 h-12 flex items-center justify-center font-bold rounded
                    ${index === 0 ? 'bg-pink-500 text-white' : 
                      index === 2 ? 'bg-yellow-300 text-gray-900' : 
                      'bg-gray-700 text-gray-300'}`}
                >
                  {value}
                </div>
                
                {index < 3 && (
                  <div className="mx-1">
                    <MoveRight className="text-gray-500" />
                  </div>
                )}
                
                {/* Arrow back to position 1 to create a cycle */}
                {index === 3 && (
                  <div className="flex flex-col items-center">
                    <MoveRight className="text-gray-500" />
                    <div className="w-36 h-8 border-2 border-gray-600 border-b-0 rounded-t-full absolute -bottom-8 right-12" />
                    <div className="absolute -bottom-8 right-12 text-gray-600 transform -translate-y-1">
                      <MoveLeft size={20} />
                    </div>
                  </div>
                )}
              </React.Fragment>
            ))}
          </div>
          
          {/* Slow pointer */}
          <div className="absolute -top-10 left-0 text-pink-500 flex flex-col items-center">
            <span className="text-sm">slow</span>
            <ArrowDown />
          </div>
          
          {/* Fast pointer */}
          <div className="absolute -top-10 left-56 text-yellow-300 flex flex-col items-center">
            <span className="text-sm">fast</span>
            <ArrowDown />
          </div>
        </div>
        
        <div className="text-gray-300 text-center mt-4 max-w-md">
          <p>The fast & slow pattern uses two pointers moving at different speeds. Essential for cycle detection in linked lists, finding middle elements, or in-place operations.</p>
          <div className="mt-2 text-sm bg-gray-700 p-2 rounded">
            <p className="text-cyan-400">Time Complexity: O(n)</p>
            <p className="text-pink-400">Space Complexity: O(1)</p>
          </div>
        </div>
      </div>
    </div>
  );
};

// Arrow down component
const ArrowDown = () => (
  <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
    <line x1="12" y1="5" x2="12" y2="19" />
    <polyline points="19 12 12 19 5 12" />
  </svg>
);

// Visualization for the Sliding Window Pattern
const SlidingWindowVisualization = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-yellow-400">
      <h2 className="text-xl font-bold text-yellow-300 mb-4">Sliding Window Pattern</h2>
      
      <div className="flex flex-col items-center">
        <div className="w-full my-6 relative">
          {/* Array visualization */}
          <div className="flex items-center justify-center">
            {[1, 3, 2, 6, 4, 8, 5].map((value, index) => (
              <div 
                key={index} 
                className={`w-12 h-12 m-1 flex items-center justify-center font-bold rounded
                  ${index >= 2 && index <= 4 ? 'bg-green-400 text-gray-900' : 'bg-gray-700 text-gray-300'}`}
              >
                {value}
              </div>
            ))}
            
            {/* Window highlight */}
            <div className="absolute top-0 left-24 w-48 h-16 border-2 border-cyan-500 rounded-md pointer-events-none" />
            
            {/* Left window pointer */}
            <div className="absolute -bottom-10 left-24 text-green-400 flex flex-col items-center">
              <ArrowUp />
              <span className="text-sm">left</span>
            </div>
            
            {/* Right window pointer */}
            <div className="absolute -bottom-10 left-72 text-green-400 flex flex-col items-center">
              <ArrowUp />
              <span className="text-sm">right</span>
            </div>
          </div>
        </div>
        
        <div className="text-gray-300 text-center mt-4 max-w-md">
          <p>The sliding window pattern maintains a "window" between two pointers, expanding or contracting to satisfy certain conditions. Perfect for substring/subarray problems.</p>
          <div className="mt-2 text-sm bg-gray-700 p-2 rounded">
            <p className="text-cyan-400">Time Complexity: O(n)</p>
            <p className="text-pink-400">Space Complexity: Usually O(1) or O(k)</p>
          </div>
        </div>
      </div>
    </div>
  );
};

// Time and Space Complexity Chart
const ComplexityChart = () => {
  const rows = [
    { pattern: "Brute Force Approach", time: "O(n²)", space: "O(1)", example: "Checking all pairs of elements" },
    { pattern: "Converging Pointers", time: "O(n)", space: "O(1)", example: "Two Sum (sorted array)" },
    { pattern: "Fast & Slow Pointers", time: "O(n)", space: "O(1)", example: "Linked List Cycle Detection" },
    { pattern: "Sliding Window", time: "O(n)", space: "O(1) or O(k)", example: "Longest Substring Without Repeating Characters" },
    { pattern: "Hash-Based Approach", time: "O(n)", space: "O(n)", example: "Two Sum (unsorted array)" }
  ];

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-green-400">
      <h2 className="text-xl font-bold text-green-400 mb-4">Time & Space Complexity Comparison</h2>
      
      <div className="overflow-x-auto">
        <table className="w-full text-left">
          <thead>
            <tr className="border-b border-gray-700">
              <th className="py-2 px-4 text-cyan-400">Pattern</th>
              <th className="py-2 px-4 text-pink-400">Time</th>
              <th className="py-2 px-4 text-yellow-300">Space</th>
              <th className="py-2 px-4 text-green-400">Example Problem</th>
            </tr>
          </thead>
          <tbody>
            {rows.map((row, index) => (
              <tr key={index} className={index % 2 === 0 ? 'bg-gray-800' : 'bg-gray-700'}>
                <td className="py-2 px-4 text-cyan-400">{row.pattern}</td>
                <td className="py-2 px-4 text-pink-400">{row.time}</td>
                <td className="py-2 px-4 text-yellow-300">{row.space}</td>
                <td className="py-2 px-4 text-gray-300">{row.example}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      
      <div className="mt-4 text-gray-300 text-sm">
        <div className="flex items-center">
          <Maximize2 className="text-green-400 mr-2" size={16} />
          <p>Two-pointer techniques generally offer optimal time complexity while maintaining minimal space usage</p>
        </div>
      </div>
    </div>
  );
};

// Meme component
const TwoPointerMeme = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-500">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Two-Pointer Meme</h2>
      
      <div className="flex flex-col items-center">
        <div className="bg-gray-900 p-4 rounded-lg w-full max-w-lg">
          <div className="text-white text-center font-bold text-xl mb-4">
            The Evolution of Problem Solving
          </div>
          
          <div className="space-y-4">
            <div className="flex items-center p-2 bg-gray-800 rounded">
              <div className="w-12 h-12 rounded-full bg-gray-700 flex items-center justify-center">
                <PlusCircle className="text-red-500" />
              </div>
              <div className="ml-4">
                <div className="text-red-400">Junior Dev</div>
                <div className="text-gray-400 text-sm">"Let me write two nested loops for this..."</div>
              </div>
            </div>
            
            <div className="flex items-center p-2 bg-gray-800 rounded">
              <div className="w-12 h-12 rounded-full bg-gray-700 flex items-center justify-center">
                <MinusCircle className="text-yellow-300" />
              </div>
              <div className="ml-4">
                <div className="text-yellow-300">Mid-level Dev</div>
                <div className="text-gray-400 text-sm">"I think we can use a hash map here..."</div>
              </div>
            </div>
            
            <div className="flex items-center p-2 bg-gray-800 rounded">
              <div className="w-12 h-12 rounded-full bg-gray-700 flex items-center justify-center">
                <div className="flex space-x-1">
                  <ArrowLeft className="text-cyan-400" size={16} />
                  <ArrowRight className="text-pink-500" size={16} />
                </div>
              </div>
              <div className="ml-4">
                <div className="text-green-400">Senior Dev</div>
                <div className="text-gray-400 text-sm">"Two pointers. O(n) time, O(1) space."</div>
              </div>
            </div>
          </div>
          
          <div className="mt-4 text-gray-500 text-sm text-center">
            #TwoPointerTechnique #DSAin45 #BigOOptimization
          </div>
        </div>
      </div>
    </div>
  );
};

export default TwoPointerVisualizations;
