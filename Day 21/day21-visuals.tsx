import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer, BarChart, Bar } from 'recharts';
import { ArrowRight, GitMerge, Divide, Search, FileCode, CornerDownRight, Star, PlusCircle, MinusCircle, Settings, Award } from 'lucide-react';

// Main component for Day 21 Week 3 Recap Visualizations
const Week3RecapVisualizations = () => {
  return (
    <div className="flex flex-col items-center justify-center w-full space-y-8 bg-gray-900 p-6 rounded-lg">
      <header className="w-full text-center mb-4">
        <h1 className="text-3xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-cyan-400 via-pink-500 to-yellow-300">
          Week 3 Challenge & Recap
        </h1>
        <p className="text-gray-300 mt-2">Day 21 of #DSAin45 - Mastering Basic Algorithms</p>
      </header>

      {/* Main visualization section */}
      <div className="grid grid-cols-1 gap-8 w-full">
        {/* Week 3 Overview Section */}
        <AlgorithmOverview />
        
        {/* Time Complexity Comparison */}
        <ComplexityComparison />
        
        {/* Algorithm Selection Chart */}
        <AlgorithmSelectionChart />
        
        {/* Performance Optimization Case Studies */}
        <OptimizationGraph />
        
        {/* Algorithm Meme */}
        <AlgorithmMeme />
      </div>
    </div>
  );
};

// Week 3 Overview with icons for each day/topic
const AlgorithmOverview = () => {
  const weekTopics = [
    { day: 15, topic: "Recursion Fundamentals", icon: <CornerDownRight className="text-green-400" />, color: "green" },
    { day: 16, topic: "Searching Algorithms", icon: <Search className="text-cyan-400" />, color: "cyan" },
    { day: 17, topic: "Basic Sorting Algorithms", icon: <Settings className="text-pink-400" />, color: "pink" },
    { day: 18, topic: "Efficient Sorting Algorithms", icon: <Award className="text-purple-400" />, color: "purple" },
    { day: 19, topic: "Divide and Conquer Strategy", icon: <Divide className="text-yellow-300" />, color: "yellow" },
    { day: 20, topic: "Two-Pointer Technique", icon: <ArrowRight className="text-blue-400" />, color: "blue" },
    { day: 21, topic: "Week 3 Challenge & Recap", icon: <Star className="text-fuchsia-400" />, color: "fuchsia" }
  ];

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-cyan-400">
      <h2 className="text-xl font-bold text-cyan-400 mb-4">Week 3 Topics Overview</h2>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
        {weekTopics.map((item, index) => (
          <div 
            key={index} 
            className={`p-4 rounded-lg flex items-center border-l-4 ${
              getBorderColor(item.color)
            } bg-gray-700 hover:bg-gray-600 transition-all duration-200`}
          >
            <div className="mr-4">{item.icon}</div>
            <div>
              <div className="font-bold text-gray-200">Day {item.day}</div>
              <div className="text-gray-300">{item.topic}</div>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};

// Helper function to get border color based on name
const getBorderColor = (color) => {
  const colorMap = {
    green: "border-green-400",
    cyan: "border-cyan-400",
    pink: "border-pink-400",
    purple: "border-purple-400", 
    yellow: "border-yellow-300",
    blue: "border-blue-400",
    fuchsia: "border-fuchsia-400"
  };
  return colorMap[color] || "border-gray-400";
};

// Time Complexity Comparison Chart
const ComplexityComparison = () => {
  // Data for the complexity chart (n vs operations)
  const generateComplexityData = () => {
    const data = [];
    for (let n = 1; n <= 10; n++) {
      data.push({
        n: n,
        'O(1)': 1,
        'O(log n)': Math.log2(n) || 0.1, // Avoid log(0)
        'O(n)': n,
        'O(n log n)': n * (Math.log2(n) || 0.1),
        'O(n²)': n * n,
      });
    }
    return data;
  };

  const complexityData = generateComplexityData();

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-pink-400">
      <h2 className="text-xl font-bold text-pink-400 mb-4">Algorithm Time Complexity Comparison</h2>
      
      <div className="mt-4 w-full h-64">
        <ResponsiveContainer width="100%" height="100%">
          <LineChart
            data={complexityData}
            margin={{ top: 10, right: 30, left: 20, bottom: 10 }}
          >
            <CartesianGrid strokeDasharray="3 3" stroke="#444" />
            <XAxis dataKey="n" stroke="#aaa" />
            <YAxis stroke="#aaa" />
            <Tooltip 
              contentStyle={{ 
                backgroundColor: '#222',
                border: '1px solid #444',
                color: '#eee'
              }}
            />
            <Legend wrapperStyle={{ color: '#eee' }} />
            <Line type="monotone" dataKey="O(1)" stroke="#00FFFF" strokeWidth={2} dot={false} />
            <Line type="monotone" dataKey="O(log n)" stroke="#00FF00" strokeWidth={2} dot={false} />
            <Line type="monotone" dataKey="O(n)" stroke="#FFFF00" strokeWidth={2} dot={false} />
            <Line type="monotone" dataKey="O(n log n)" stroke="#FF00FF" strokeWidth={2} dot={false} />
            <Line type="monotone" dataKey="O(n²)" stroke="#FF3333" strokeWidth={2} dot={false} />
          </LineChart>
        </ResponsiveContainer>
      </div>
      
      <div className="mt-4 grid grid-cols-1 md:grid-cols-2 lg:grid-cols-5 gap-2">
        <ComplexityBadge color="cyan" name="O(1)" examples="Array access, Hash table insertion" />
        <ComplexityBadge color="green" name="O(log n)" examples="Binary search, Balanced BST operations" />
        <ComplexityBadge color="yellow" name="O(n)" examples="Linear search, Two-pointer techniques" />
        <ComplexityBadge color="pink" name="O(n log n)" examples="Merge sort, Quick sort (average case)" />
        <ComplexityBadge color="red" name="O(n²)" examples="Bubble sort, Selection sort" />
      </div>
    </div>
  );
};

// Complexity Badge Component
const ComplexityBadge = ({ color, name, examples }) => {
  const colorMap = {
    cyan: "bg-cyan-900 border-cyan-400 text-cyan-400",
    green: "bg-green-900 border-green-400 text-green-400",
    yellow: "bg-yellow-900 border-yellow-300 text-yellow-300",
    pink: "bg-pink-900 border-pink-400 text-pink-400",
    red: "bg-red-900 border-red-400 text-red-400"
  };
  
  const colorClass = colorMap[color] || "bg-gray-700 border-gray-400 text-gray-400";
  
  return (
    <div className={`border rounded-lg p-2 ${colorClass}`}>
      <div className="font-bold">{name}</div>
      <div className="text-xs text-gray-300">{examples}</div>
    </div>
  );
};

// Algorithm Selection Decision Chart
const AlgorithmSelectionChart = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-yellow-300">
      <h2 className="text-xl font-bold text-yellow-300 mb-4">Algorithm Selection Framework</h2>
      
      <div className="flex flex-col space-y-4">
        {/* Task/Problem Type Row */}
        <div className="grid grid-cols-1 md:grid-cols-4 gap-4">
          <DecisionBox color="cyan" title="Searching" icon={<Search size={24} />}>
            Find an element in a collection
          </DecisionBox>
          
          <DecisionBox color="pink" title="Sorting" icon={<Settings size={24} />}>
            Arrange elements in a specific order
          </DecisionBox>
          
          <DecisionBox color="yellow" title="Divide & Conquer" icon={<Divide size={24} />}>
            Break down into smaller subproblems
          </DecisionBox>
          
          <DecisionBox color="green" title="Two-Pointer" icon={<ArrowRight size={24} />}>
            Process arrays/strings efficiently
          </DecisionBox>
        </div>
        
        {/* Decision Factors Row */}
        <div className="bg-gray-700 p-4 rounded-lg">
          <h3 className="text-gray-200 font-bold mb-2">Decision Factors</h3>
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4">
            <Factor name="Data Structure" options={["Array", "Linked List", "Tree", "Graph"]} />
            <Factor name="Data Order" options={["Sorted", "Unsorted", "Partially Sorted"]} />
            <Factor name="Size" options={["Small", "Medium", "Large"]} />
            <Factor name="Space Constraints" options={["Tight", "Flexible"]} />
          </div>
        </div>
        
        {/* Algorithm Recommendations */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <RecommendationBox 
            condition="Searching in sorted data" 
            algorithm="Binary Search" 
            complexity="O(log n)"
            icon={<Search size={20} className="text-cyan-400" />}
          />
          
          <RecommendationBox 
            condition="Searching in unsorted data" 
            algorithm="Linear Search or Hash Table" 
            complexity="O(n) or O(1)"
            icon={<Search size={20} className="text-yellow-300" />}
          />
          
          <RecommendationBox 
            condition="Sorting with space constraints" 
            algorithm="Quick Sort" 
            complexity="O(n log n) avg, O(1) space"
            icon={<Settings size={20} className="text-pink-400" />}
          />
          
          <RecommendationBox 
            condition="Sorting with stability needed" 
            algorithm="Merge Sort" 
            complexity="O(n log n), O(n) space"
            icon={<Settings size={20} className="text-green-400" />}
          />
          
          <RecommendationBox 
            condition="Finding pairs in sorted array" 
            algorithm="Two-Pointer Technique" 
            complexity="O(n) time, O(1) space"
            icon={<ArrowRight size={20} className="text-blue-400" />}
          />
          
          <RecommendationBox 
            condition="Complex problems with optimal substructure" 
            algorithm="Divide and Conquer" 
            complexity="Problem dependent"
            icon={<Divide size={20} className="text-purple-400" />}
          />
        </div>
      </div>
    </div>
  );
};

// Decision Box component
const DecisionBox = ({ color, title, icon, children }) => {
  const colorMap = {
    cyan: "border-cyan-400 text-cyan-400",
    pink: "border-pink-400 text-pink-400",
    yellow: "border-yellow-300 text-yellow-300",
    green: "border-green-400 text-green-400"
  };
  
  const colorClass = colorMap[color] || "border-gray-400 text-gray-400";
  
  return (
    <div className={`border ${colorClass} rounded-lg p-3 bg-gray-700`}>
      <div className="flex items-center mb-2">
        <div className="mr-2">{icon}</div>
        <div className="font-bold">{title}</div>
      </div>
      <div className="text-sm text-gray-300">{children}</div>
    </div>
  );
};

// Factor component
const Factor = ({ name, options }) => {
  return (
    <div className="bg-gray-800 p-2 rounded">
      <div className="text-gray-300 font-bold text-sm">{name}:</div>
      <div className="flex flex-wrap gap-1 mt-1">
        {options.map((option, index) => (
          <span key={index} className="bg-gray-700 text-gray-300 px-2 py-1 rounded text-xs">
            {option}
          </span>
        ))}
      </div>
    </div>
  );
};

// Recommendation Box component
const RecommendationBox = ({ condition, algorithm, complexity, icon }) => {
  return (
    <div className="bg-gray-700 border border-gray-600 rounded-lg p-3 hover:bg-gray-600 transition-all duration-200">
      <div className="text-gray-400 text-sm">When you need:</div>
      <div className="text-white font-bold mb-2">{condition}</div>
      <div className="flex items-center">
        {icon}
        <div className="ml-2 text-gray-200">{algorithm}</div>
      </div>
      <div className="text-gray-400 text-xs mt-1">Complexity: {complexity}</div>
    </div>
  );
};

// Performance Optimization Graph
const OptimizationGraph = () => {
  // Sample data for performance comparison
  const performanceData = [
    {
      name: 'Searching',
      'Linear Search': 100,
      'Binary Search': 7,
      gain: 'O(n) → O(log n)'
    },
    {
      name: 'Sorting',
      'Bubble Sort': 100,
      'Quick Sort': 15,
      gain: 'O(n²) → O(n log n)'
    },
    {
      name: 'Two Sum',
      'Brute Force': 100,
      'Two-Pointer': 4,
      gain: 'O(n²) → O(n)'
    },
    {
      name: 'LIS',
      'DP Approach': 100,
      'Binary Search': 8,
      gain: 'O(n²) → O(n log n)'
    },
  ];

  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-green-400">
      <h2 className="text-xl font-bold text-green-400 mb-4">Performance Optimization Case Studies</h2>
      
      <div className="mt-4 w-full h-64">
        <ResponsiveContainer width="100%" height="100%">
          <BarChart
            data={performanceData}
            layout="vertical"
            margin={{ top: 10, right: 30, left: 100, bottom: 10 }}
          >
            <CartesianGrid strokeDasharray="3 3" stroke="#444" />
            <XAxis type="number" stroke="#aaa" />
            <YAxis dataKey="name" type="category" stroke="#aaa" />
            <Tooltip
              contentStyle={{ 
                backgroundColor: '#222',
                border: '1px solid #444',
                color: '#eee'
              }}
            />
            <Legend wrapperStyle={{ color: '#eee' }} />
            <Bar dataKey="Linear Search" stackId="a" fill="#FF5555" />
            <Bar dataKey="Bubble Sort" stackId="a" fill="#FF5555" />
            <Bar dataKey="Brute Force" stackId="a" fill="#FF5555" />
            <Bar dataKey="DP Approach" stackId="a" fill="#FF5555" />
            <Bar dataKey="Binary Search" stackId="b" fill="#00FFFF" />
            <Bar dataKey="Quick Sort" stackId="b" fill="#FF00FF" />
            <Bar dataKey="Two-Pointer" stackId="b" fill="#FFFF00" />
            <Bar dataKey="Binary Search" stackId="b" fill="#00FF00" />
          </BarChart>
        </ResponsiveContainer>
      </div>
      
      <div className="mt-6 grid grid-cols-1 md:grid-cols-2 gap-4">
        <CaseStudyBox 
          title="Case Study 1: Searching Optimizations" 
          original="Linear Search (O(n))"
          optimized="Binary Search (O(log n))"
          speedup="~14x faster for 1M elements"
          condition="When data is sorted"
          color="cyan"
        />
        
        <CaseStudyBox 
          title="Case Study 2: Two Sum Problem" 
          original="Nested Loops (O(n²))"
          optimized="Two-Pointer Technique (O(n))"
          speedup="~25x faster for large arrays"
          condition="When data is sorted"
          color="yellow"
        />
        
        <CaseStudyBox 
          title="Case Study 3: Sorting Comparison" 
          original="Bubble Sort (O(n²))"
          optimized="Quick Sort (O(n log n))"
          speedup="~6-7x faster for 10K elements"
          condition="When in-place sorting needed"
          color="pink"
        />
        
        <CaseStudyBox 
          title="Case Study 4: Longest Increasing Subsequence" 
          original="DP Approach (O(n²))"
          optimized="Binary Search Approach (O(n log n))"
          speedup="~12x faster for large arrays"
          condition="When optimal time complexity needed"
          color="green"
        />
      </div>
    </div>
  );
};

// Case Study Box Component
const CaseStudyBox = ({ title, original, optimized, speedup, condition, color }) => {
  const colorMap = {
    cyan: "border-cyan-400 text-cyan-400",
    pink: "border-pink-400 text-pink-400",
    yellow: "border-yellow-300 text-yellow-300",
    green: "border-green-400 text-green-400"
  };
  
  const colorClass = colorMap[color] || "border-gray-400 text-gray-400";
  
  return (
    <div className={`border ${colorClass} rounded-lg p-4 bg-gray-700`}>
      <h3 className={`font-bold ${colorClass.split(' ')[1]}`}>{title}</h3>
      <div className="grid grid-cols-3 gap-2 mt-2">
        <div className="col-span-1 text-gray-400 text-sm">Original:</div>
        <div className="col-span-2 text-red-400">{original}</div>
        
        <div className="col-span-1 text-gray-400 text-sm">Optimized:</div>
        <div className="col-span-2 text-green-400">{optimized}</div>
        
        <div className="col-span-1 text-gray-400 text-sm">Speedup:</div>
        <div className="col-span-2 text-yellow-300">{speedup}</div>
        
        <div className="col-span-1 text-gray-400 text-sm">When:</div>
        <div className="col-span-2 text-gray-300">{condition}</div>
      </div>
    </div>
  );
};

// Meme component for Week 3 recap
const AlgorithmMeme = () => {
  return (
    <div className="bg-gray-800 p-6 rounded-lg border border-fuchsia-400">
      <h2 className="text-xl font-bold text-fuchsia-400 mb-4">The Algorithm Evolution Meme</h2>
      
      <div className="bg-gray-900 rounded-lg p-6 max-w-lg mx-auto">
        <div className="text-center font-bold text-xl text-gray-100 mb-6">
          The Evolution of a Programmer
        </div>
        
        <div className="space-y-6">
          <EvolutionStage 
            level="Beginner"
            code="for (i=0; i<n; i++) {
  for (j=0; j<n; j++) {
    // Check every pair
  }
}"
            comment="I'll just use nested loops for everything!"
            complexity="O(n²)"
            icon={<MinusCircle size={24} className="text-red-500" />}
          />
          
          <EvolutionStage 
            level="Intermediate"
            code="std::unordered_map<int, int> map;
for (i=0; i<n; i++) {
  // Use a hash map
}"
            comment="I can optimize using the right data structures!"
            complexity="O(n) time, O(n) space"
            icon={<Settings size={24} className="text-yellow-300" />}
          />
          
          <EvolutionStage 
            level="Advanced"
            code="int left = 0, right = n-1;
while (left < right) {
  // Two-pointer technique
}"
            comment="O(n) time AND O(1) space? Perfect!"
            complexity="O(n) time, O(1) space"
            icon={<PlusCircle size={24} className="text-green-400" />}
          />
          
          <EvolutionStage 
            level="Expert"
            code="return std::lower_bound(
  begin(nums), end(nums), target
);"
            comment="Why reinvent the wheel? STL has optimal implementations..."
            complexity="O(log n) with zero bugs"
            icon={<Award size={24} className="text-cyan-400" />}
          />
        </div>
        
        <div className="mt-6 text-center text-gray-500 text-sm">
          #DSAin45 #AlgorithmEvolution
        </div>
      </div>
    </div>
  );
};

// Evolution Stage Component
const EvolutionStage = ({ level, code, comment, complexity, icon }) => {
  return (
    <div className="bg-gray-800 rounded-lg p-4 border border-gray-700">
      <div className="flex items-center mb-2">
        {icon}
        <span className="ml-2 font-bold text-gray-200">{level} Programmer</span>
        <span className="ml-auto text-sm text-gray-400">{complexity}</span>
      </div>
      
      <div className="bg-gray-900 p-3 rounded font-mono text-sm text-gray-300 whitespace-pre">
        {code}
      </div>
      
      <div className="mt-2 text-sm text-gray-400 italic">
        "{comment}"
      </div>
    </div>
  );
};

export default Week3RecapVisualizations;
