import React, { useState, useEffect } from 'react';

const DSADashboard = () => {
  // State for tracking progress and stats
  const [activeTab, setActiveTab] = useState('overview');
  const [progressData, setProgressData] = useState(generateInitialProgress());
  const [practiceStats, setPracticeStats] = useState(calculateStats(generateInitialProgress()));
  const [selectedCategory, setSelectedCategory] = useState('all');
  const [scheduleView, setScheduleView] = useState('daily');
  
  // Update stats when progress data changes
  useEffect(() => {
    setPracticeStats(calculateStats(progressData));
  }, [progressData]);

  // Generate initial mock data for progress tracking
  function generateInitialProgress() {
    const categories = [
      'Arrays', 'Strings', 'Linked Lists', 'Stacks', 'Queues', 
      'Hash Tables', 'Trees', 'Graphs', 'Dynamic Programming', 
      'Greedy', 'Backtracking', 'Sorting', 'Searching', 'Bit Manipulation'
    ];
    
    const difficulties = ['Easy', 'Medium', 'Hard'];
    const problemNames = [
      'Two Sum', 'Reverse String', 'Merge Two Lists', 'Valid Parentheses',
      'Implement Queue', 'First Unique Character', 'Maximum Depth', 'Number of Islands',
      'Coin Change', 'Jump Game', 'N-Queens', 'Merge Sort', 'Binary Search', 'Counting Bits'
    ];
    
    const progress = [];
    
    // Generate one problem for each category
    categories.forEach((category, i) => {
      const difficulty = difficulties[i % 3];
      progress.push({
        id: i + 1,
        name: problemNames[i],
        category,
        difficulty,
        timeSpent: Math.floor(Math.random() * 40) + 10,
        solved: Math.random() > 0.3,
        date: getRandomDate(45),
        notes: `${category} practice problem`
      });
      
      // Add a few more random problems
      if (i < 10) {
        progress.push({
          id: i + 100,
          name: `${category} Challenge ${i+1}`,
          category,
          difficulty: difficulties[Math.floor(Math.random() * 3)],
          timeSpent: Math.floor(Math.random() * 50) + 5,
          solved: Math.random() > 0.3,
          date: getRandomDate(40),
          notes: 'Follow-up problem'
        });
      }
    });
    
    return progress;
  }
  
  // Get a random date in the last N days
  function getRandomDate(maxDaysAgo) {
    const today = new Date();
    const daysAgo = Math.floor(Math.random() * maxDaysAgo);
    const date = new Date(today);
    date.setDate(today.getDate() - daysAgo);
    return date.toISOString().split('T')[0];
  }
  
  // Calculate statistics from progress data
  function calculateStats(data) {
    const totalProblems = data.length;
    const solvedProblems = data.filter(p => p.solved).length;
    const solveRate = totalProblems > 0 ? (solvedProblems / totalProblems) * 100 : 0;
    
    // Category breakdown
    const categories = {};
    const difficulties = {
      'Easy': { attempted: 0, solved: 0, avgTime: 0, totalTime: 0 },
      'Medium': { attempted: 0, solved: 0, avgTime: 0, totalTime: 0 },
      'Hard': { attempted: 0, solved: 0, avgTime: 0, totalTime: 0 }
    };
    
    // Process each problem
    data.forEach(problem => {
      // Process categories
      if (!categories[problem.category]) {
        categories[problem.category] = {
          attempted: 0,
          solved: 0,
          solveRate: 0,
          avgTime: 0,
          totalTime: 0
        };
      }
      
      categories[problem.category].attempted++;
      difficulties[problem.difficulty].attempted++;
      
      if (problem.solved) {
        categories[problem.category].solved++;
        difficulties[problem.difficulty].solved++;
      }
      
      categories[problem.category].totalTime += problem.timeSpent;
      difficulties[problem.difficulty].totalTime += problem.timeSpent;
    });
    
    // Calculate rates and averages
    Object.keys(categories).forEach(cat => {
      const category = categories[cat];
      category.solveRate = category.attempted > 0 ? (category.solved / category.attempted) * 100 : 0;
      category.avgTime = category.solved > 0 ? category.totalTime / category.solved : 0;
    });
    
    Object.keys(difficulties).forEach(diff => {
      const difficulty = difficulties[diff];
      difficulty.solveRate = difficulty.attempted > 0 ? (difficulty.solved / difficulty.attempted) * 100 : 0;
      difficulty.avgTime = difficulty.solved > 0 ? difficulty.totalTime / difficulty.solved : 0;
    });
    
    // Get weakest and strongest categories
    const categoriesArray = Object.entries(categories).map(([name, stats]) => ({
      name,
      ...stats
    }));
    
    const weakestCategories = [...categoriesArray]
      .filter(c => c.attempted >= 2)
      .sort((a, b) => a.solveRate - b.solveRate)
      .slice(0, 3);
      
    const strongestCategories = [...categoriesArray]
      .filter(c => c.attempted >= 2)
      .sort((a, b) => b.solveRate - a.solveRate)
      .slice(0, 3);
    
    // Calculate day-by-day progress
    const dailyProgress = {};
    data.forEach(problem => {
      if (!dailyProgress[problem.date]) {
        dailyProgress[problem.date] = {
          attempted: 0,
          solved: 0
        };
      }
      
      dailyProgress[problem.date].attempted++;
      if (problem.solved) {
        dailyProgress[problem.date].solved++;
      }
    });
    
    // Create practice plan
    const practicePlan = generatePracticePlan(weakestCategories.map(c => c.name));
    
    return {
      totalProblems,
      solvedProblems,
      solveRate,
      categories,
      difficulties,
      weakestCategories,
      strongestCategories,
      dailyProgress,
      practicePlan
    };
  }
  
  // Generate a practice plan based on weak areas
  function generatePracticePlan(weakAreas) {
    // Default if no weak areas identified
    if (!weakAreas || weakAreas.length === 0) {
      weakAreas = ['Dynamic Programming', 'Graphs', 'Backtracking'];
    }
    
    const plan = {
      week1: [],
      month1: [],
      ongoing: []
    };
    
    // 5 problems/day for week 1 (showing first 5 days)
    for (let i = 0; i < 5; i++) {
      const day = [];
      // 70% weak areas
      for (let j = 0; j < 3; j++) {
        day.push({
          category: weakAreas[j % weakAreas.length],
          difficulty: j === 0 ? 'Easy' : (j === 1 ? 'Medium' : 'Medium'),
          focus: 'Weak Area'
        });
      }
      // 20% medium difficulty
      day.push({
        category: getRandomCategory(weakAreas),
        difficulty: 'Medium',
        focus: 'Maintain Strengths'
      });
      // 10% new concept
      day.push({
        category: getRandomCategory(weakAreas, true),
        difficulty: i % 5 === 0 ? 'Hard' : 'Medium',
        focus: 'New Concept'
      });
      
      plan.week1.push(day);
    }
    
    // 3 problems/day for month 1 (showing a sample week)
    for (let i = 0; i < 7; i++) {
      const day = [];
      // 70% weak areas (2 problems)
      for (let j = 0; j < 2; j++) {
        day.push({
          category: weakAreas[j % weakAreas.length],
          difficulty: j === 0 ? 'Easy' : 'Medium',
          focus: 'Weak Area'
        });
      }
      // 30% mixed (1 problem)
      day.push({
        category: i % 3 === 0 ? getRandomCategory(weakAreas, true) : getRandomCategory(weakAreas),
        difficulty: i % 7 === 0 ? 'Hard' : 'Medium',
        focus: i % 3 === 0 ? 'New Concept' : 'Maintain Strengths'
      });
      
      plan.month1.push(day);
    }
    
    // 5 problems/week ongoing (showing a sample)
    const ongoing = [];
    // 3 weak area problems (60%)
    for (let i = 0; i < 3; i++) {
      ongoing.push({
        category: weakAreas[i % weakAreas.length],
        difficulty: i === 0 ? 'Easy' : (i === 1 ? 'Medium' : 'Hard'),
        focus: 'Weak Area'
      });
    }
    // 1 maintenance problem (20%)
    ongoing.push({
      category: getRandomCategory(weakAreas),
      difficulty: 'Medium',
      focus: 'Maintain Strengths'
    });
    // 1 new concept problem (20%)
    ongoing.push({
      category: getRandomCategory(weakAreas, true),
      difficulty: 'Medium',
      focus: 'New Concept'
    });
    
    plan.ongoing = ongoing;
    
    return plan;
  }
  
  // Get a random category that's not in the weak areas
  function getRandomCategory(weakAreas, ensureDifferent = false) {
    const allCategories = [
      'Arrays', 'Strings', 'Linked Lists', 'Stacks', 'Queues', 
      'Hash Tables', 'Trees', 'Graphs', 'Dynamic Programming', 
      'Greedy', 'Backtracking', 'Sorting', 'Searching', 'Bit Manipulation'
    ];
    
    if (!ensureDifferent) {
      return allCategories[Math.floor(Math.random() * allCategories.length)];
    }
    
    // Filter out weak areas to ensure we're picking something different
    const otherCategories = allCategories.filter(c => !weakAreas.includes(c));
    if (otherCategories.length === 0) return allCategories[0];
    
    return otherCategories[Math.floor(Math.random() * otherCategories.length)];
  }
  
  // Tab content components
  const OverviewTab = () => (
    <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
      <div className="bg-gray-800 rounded-lg p-4">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Progress Summary</h3>
        <div className="grid grid-cols-2 gap-4">
          <div className="bg-gray-900 p-3 rounded-lg text-center">
            <div className="text-3xl text-green-400 font-bold">{practiceStats.solvedProblems}</div>
            <div className="text-sm text-gray-400">Problems Solved</div>
          </div>
          <div className="bg-gray-900 p-3 rounded-lg text-center">
            <div className="text-3xl text-yellow-400 font-bold">{practiceStats.totalProblems}</div>
            <div className="text-sm text-gray-400">Total Attempted</div>
          </div>
          <div className="bg-gray-900 p-3 rounded-lg text-center">
            <div className="text-3xl text-pink-400 font-bold">{practiceStats.solveRate.toFixed(1)}%</div>
            <div className="text-sm text-gray-400">Solve Rate</div>
          </div>
          <div className="bg-gray-900 p-3 rounded-lg text-center">
            <div className="text-3xl text-cyan-400 font-bold">{Object.keys(practiceStats.categories).length}</div>
            <div className="text-sm text-gray-400">Categories Practiced</div>
          </div>
        </div>
        
        <h4 className="text-pink-400 mt-6 mb-2 font-bold">45-Day Journey Complete!</h4>
        <div className="bg-gray-900 rounded-lg p-3">
          <div className="text-white mb-2">You've built a solid foundation in DSA!</div>
          <div className="text-sm text-gray-400">Your next steps are crucial for long-term retention. Follow the sustainable practice routine to maintain and build on your skills.</div>
        </div>
      </div>
      
      <div className="bg-gray-800 rounded-lg p-4">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Strengths & Weaknesses</h3>
        
        <h4 className="text-green-400 font-bold mt-2 mb-1">Strongest Areas</h4>
        <div className="space-y-2">
          {practiceStats.strongestCategories.map((category, index) => (
            <div key={`strong-${index}`} className="bg-gray-900 p-2 rounded-lg">
              <div className="flex justify-between items-center">
                <span className="text-white">{category.name}</span>
                <span className="text-green-400 font-bold">{category.solveRate.toFixed(0)}%</span>
              </div>
              <div className="w-full bg-gray-700 h-2 rounded-full mt-1">
                <div 
                  className="bg-green-500 h-2 rounded-full" 
                  style={{ width: `${category.solveRate}%` }}
                />
              </div>
            </div>
          ))}
        </div>
        
        <h4 className="text-pink-400 font-bold mt-4 mb-1">Areas for Improvement</h4>
        <div className="space-y-2">
          {practiceStats.weakestCategories.map((category, index) => (
            <div key={`weak-${index}`} className="bg-gray-900 p-2 rounded-lg">
              <div className="flex justify-between items-center">
                <span className="text-white">{category.name}</span>
                <span className="text-pink-400 font-bold">{category.solveRate.toFixed(0)}%</span>
              </div>
              <div className="w-full bg-gray-700 h-2 rounded-full mt-1">
                <div 
                  className="bg-pink-500 h-2 rounded-full" 
                  style={{ width: `${category.solveRate}%` }}
                />
              </div>
            </div>
          ))}
        </div>
        
        <h4 className="text-yellow-400 font-bold mt-4 mb-1">By Difficulty</h4>
        <div className="space-y-2">
          {Object.entries(practiceStats.difficulties).map(([diff, stats]) => (
            <div key={diff} className="bg-gray-900 p-2 rounded-lg">
              <div className="flex justify-between items-center">
                <span className="text-white">{diff}</span>
                <span className="text-yellow-400 font-bold">{stats.solveRate.toFixed(0)}% ({stats.solved}/{stats.attempted})</span>
              </div>
              <div className="w-full bg-gray-700 h-2 rounded-full mt-1">
                <div 
                  className="bg-yellow-500 h-2 rounded-full" 
                  style={{ width: `${stats.solveRate}%` }}
                />
              </div>
            </div>
          ))}
        </div>
      </div>
      
      <div className="bg-gray-800 rounded-lg p-4 md:col-span-2">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Your Sustainable Practice Plan</h3>
        <div className="bg-gray-900 p-3 rounded-lg mb-4">
          <p className="text-white text-sm mb-2">Based on the forgetting curve, you need a structured practice schedule to retain what you've learned:</p>
          <div className="grid grid-cols-1 md:grid-cols-3 gap-3 mt-4">
            <div className="bg-gray-800 p-2 rounded border border-pink-500">
              <div className="text-pink-400 font-bold">Week 1</div>
              <div className="text-white">5 problems/day</div>
              <div className="text-gray-400 text-xs">35 problems total</div>
            </div>
            <div className="bg-gray-800 p-2 rounded border border-yellow-500">
              <div className="text-yellow-400 font-bold">Weeks 2-4</div>
              <div className="text-white">3 problems/day</div>
              <div className="text-gray-400 text-xs">63 problems total</div>
            </div>
            <div className="bg-gray-800 p-2 rounded border border-green-500">
              <div className="text-green-400 font-bold">Ongoing</div>
              <div className="text-white">5 problems/week</div>
              <div className="text-gray-400 text-xs">Sustainable indefinitely</div>
            </div>
          </div>
        </div>
        
        <div className="flex space-x-3 mb-3">
          <button 
            className={`px-3 py-1 rounded text-sm ${scheduleView === 'daily' ? 'bg-cyan-600 text-white' : 'bg-gray-700 text-gray-300'}`}
            onClick={() => setScheduleView('daily')}
          >
            Daily View
          </button>
          <button 
            className={`px-3 py-1 rounded text-sm ${scheduleView === 'weekly' ? 'bg-cyan-600 text-white' : 'bg-gray-700 text-gray-300'}`}
            onClick={() => setScheduleView('weekly')}
          >
            Weekly View
          </button>
          <button 
            className={`px-3 py-1 rounded text-sm ${scheduleView === 'ongoing' ? 'bg-cyan-600 text-white' : 'bg-gray-700 text-gray-300'}`}
            onClick={() => setScheduleView('ongoing')}
          >
            Ongoing View
          </button>
        </div>
        
        {scheduleView === 'daily' && (
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="text-white font-bold mb-2">Week 1 Daily Schedule (Sample)</div>
            <div className="overflow-x-auto">
              <table className="w-full text-sm">
                <thead>
                  <tr className="text-gray-400 border-b border-gray-700">
                    <th className="text-left py-2">Day</th>
                    <th className="text-left py-2">Problem 1</th>
                    <th className="text-left py-2">Problem 2</th>
                    <th className="text-left py-2">Problem 3</th>
                    <th className="text-left py-2">Problem 4</th>
                    <th className="text-left py-2">Problem 5</th>
                  </tr>
                </thead>
                <tbody>
                  {practiceStats.practicePlan.week1.map((day, index) => (
                    <tr key={`day-${index}`} className="border-b border-gray-800">
                      <td className="py-2 font-bold text-cyan-400">Day {index + 1}</td>
                      {day.map((problem, pIndex) => (
                        <td key={`problem-${index}-${pIndex}`} className="py-2">
                          <div className={`text-${problem.focus === 'Weak Area' ? 'pink' : (problem.focus === 'New Concept' ? 'yellow' : 'green')}-400`}>
                            {problem.category}
                          </div>
                          <div className="text-gray-400 text-xs">{problem.difficulty} • {problem.focus}</div>
                        </td>
                      ))}
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        )}
        
        {scheduleView === 'weekly' && (
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="text-white font-bold mb-2">Month 1 Weekly Schedule (Sample Week)</div>
            <div className="overflow-x-auto">
              <table className="w-full text-sm">
                <thead>
                  <tr className="text-gray-400 border-b border-gray-700">
                    <th className="text-left py-2">Day</th>
                    <th className="text-left py-2">Problem 1</th>
                    <th className="text-left py-2">Problem 2</th>
                    <th className="text-left py-2">Problem 3</th>
                  </tr>
                </thead>
                <tbody>
                  {practiceStats.practicePlan.month1.map((day, index) => (
                    <tr key={`month-day-${index}`} className="border-b border-gray-800">
                      <td className="py-2 font-bold text-cyan-400">{['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'][index]}</td>
                      {day.map((problem, pIndex) => (
                        <td key={`month-problem-${index}-${pIndex}`} className="py-2">
                          <div className={`text-${problem.focus === 'Weak Area' ? 'pink' : (problem.focus === 'New Concept' ? 'yellow' : 'green')}-400`}>
                            {problem.category}
                          </div>
                          <div className="text-gray-400 text-xs">{problem.difficulty} • {problem.focus}</div>
                        </td>
                      ))}
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        )}
        
        {scheduleView === 'ongoing' && (
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="text-white font-bold mb-2">Ongoing Maintenance (Weekly 5 Problems)</div>
            <div className="overflow-x-auto">
              <table className="w-full text-sm">
                <thead>
                  <tr className="text-gray-400 border-b border-gray-700">
                    <th className="text-left py-2">#</th>
                    <th className="text-left py-2">Category</th>
                    <th className="text-left py-2">Difficulty</th>
                    <th className="text-left py-2">Focus Area</th>
                  </tr>
                </thead>
                <tbody>
                  {practiceStats.practicePlan.ongoing.map((problem, index) => (
                    <tr key={`ongoing-${index}`} className="border-b border-gray-800">
                      <td className="py-2 font-bold text-cyan-400">{index + 1}</td>
                      <td className="py-2">{problem.category}</td>
                      <td className="py-2">{problem.difficulty}</td>
                      <td className={`py-2 text-${problem.focus === 'Weak Area' ? 'pink' : (problem.focus === 'New Concept' ? 'yellow' : 'green')}-400`}>
                        {problem.focus}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
            <div className="mt-3 text-gray-400 text-xs">
              <div>• 70% focus on weak areas to improve where you struggle</div>
              <div>• 20% on maintaining strengths to prevent regression</div>
              <div>• 10% on new concepts to continuously expand knowledge</div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
  
  const StatsTab = () => (
    <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
      <div className="bg-gray-800 rounded-lg p-4">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Problem Solving Performance</h3>
        
        <div className="flex space-x-2 mb-4">
          <button 
            className={`px-3 py-1 rounded text-xs ${selectedCategory === 'all' ? 'bg-cyan-600 text-white' : 'bg-gray-700 text-gray-300'}`}
            onClick={() => setSelectedCategory('all')}
          >
            All Categories
          </button>
          {practiceStats.weakestCategories.map((cat, index) => (
            <button 
              key={`cat-btn-${index}`}
              className={`px-3 py-1 rounded text-xs ${selectedCategory === cat.name ? 'bg-pink-600 text-white' : 'bg-gray-700 text-gray-300'}`}
              onClick={() => setSelectedCategory(cat.name)}
            >
              {cat.name}
            </button>
          ))}
        </div>
        
        <div className="bg-gray-900 p-3 rounded-lg">
          <div className="text-white font-bold mb-2">
            {selectedCategory === 'all' ? 'Overall Performance' : `${selectedCategory} Performance`}
          </div>
          <div className="space-y-4">
            <div>
              <div className="flex justify-between text-sm mb-1">
                <span className="text-gray-400">Solve Rate</span>
                <span className="text-white font-bold">
                  {selectedCategory === 'all' 
                    ? `${practiceStats.solveRate.toFixed(1)}%`
                    : `${practiceStats.categories[selectedCategory]?.solveRate.toFixed(1) || 0}%`}
                </span>
              </div>
              <div className="w-full bg-gray-700 h-3 rounded-full">
                <div 
                  className="bg-gradient-to-r from-pink-500 to-cyan-500 h-3 rounded-full" 
                  style={{ 
                    width: `${selectedCategory === 'all' 
                      ? practiceStats.solveRate
                      : (practiceStats.categories[selectedCategory]?.solveRate || 0)}%` 
                  }}
                />
              </div>
            </div>
            
            <div>
              <div className="flex justify-between text-sm mb-1">
                <span className="text-gray-400">Average Solve Time</span>
                <span className="text-white font-bold">
                  {selectedCategory === 'all' 
                    ? `${(Object.values(practiceStats.categories).reduce((sum, cat) => sum + cat.avgTime, 0) / Object.keys(practiceStats.categories).length).toFixed(1)} min`
                    : `${practiceStats.categories[selectedCategory]?.avgTime.toFixed(1) || 0} min`}
                </span>
              </div>
              <div className="w-full bg-gray-700 h-3 rounded-full">
                <div 
                  className="bg-gradient-to-r from-yellow-500 to-green-500 h-3 rounded-full" 
                  style={{ 
                    width: `${selectedCategory === 'all' 
                      ? Math.min(100, (Object.values(practiceStats.categories).reduce((sum, cat) => sum + cat.avgTime, 0) / Object.keys(practiceStats.categories).length) * 2)
                      : Math.min(100, (practiceStats.categories[selectedCategory]?.avgTime || 0) * 2)}%` 
                  }}
                />
              </div>
            </div>
            
            <div>
              <div className="flex justify-between text-sm mb-1">
                <span className="text-gray-400">Problem Difficulty Distribution</span>
              </div>
              <div className="flex h-4 w-full rounded-full overflow-hidden">
                <div 
                  className="bg-green-500 h-full" 
                  style={{ width: '45%' }}
                />
                <div 
                  className="bg-yellow-500 h-full" 
                  style={{ width: '40%' }}
                />
                <div 
                  className="bg-pink-500 h-full" 
                  style={{ width: '15%' }}
                />
              </div>
              <div className="flex text-xs text-gray-400 mt-1 justify-between">
                <span>Easy (45%)</span>
                <span>Medium (40%)</span>
                <span>Hard (15%)</span>
              </div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-900 p-3 rounded-lg mt-4">
          <div className="text-white font-bold mb-2">Key Insights</div>
          <div className="space-y-2 text-sm">
            <div className="flex items-start">
              <div className="text-pink-400 mr-2">•</div>
              <div className="text-gray-300">Your strongest pattern recognition is in {practiceStats.strongestCategories[0]?.name || 'Arrays'} problems.</div>
            </div>
            <div className="flex items-start">
              <div className="text-pink-400 mr-2">•</div>
              <div className="text-gray-300">You solve {practiceStats.difficulties['Easy'].solveRate.toFixed(0)}% of Easy problems but only {practiceStats.difficulties['Hard'].solveRate.toFixed(0)}% of Hard problems.</div>
            </div>
            <div className="flex items-start">
              <div className="text-pink-400 mr-2">•</div>
              <div className="text-gray-300">Focus on {practiceStats.weakestCategories[0]?.name || 'Dynamic Programming'} to make the biggest improvement.</div>
            </div>
          </div>
        </div>
      </div>
      
      <div className="space-y-6">
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Category Breakdown</h3>
          <div className="max-h-80 overflow-y-auto pr-2">
            <table className="w-full text-sm">
              <thead className="sticky top-0 bg-gray-800">
                <tr className="text-gray-400 border-b border-gray-700">
                  <th className="text-left py-2">Category</th>
                  <th className="text-center py-2">Solved</th>
                  <th className="text-center py-2">Attempted</th>
                  <th className="text-center py-2">Rate</th>
                  <th className="text-center py-2">Avg Time</th>
                </tr>
              </thead>
              <tbody>
                {Object.entries(practiceStats.categories)
                  .sort((a, b) => b[1].solveRate - a[1].solveRate)
                  .map(([category, stats]) => (
                    <tr key={category} className="border-b border-gray-900 hover:bg-gray-700">
                      <td className="py-2">{category}</td>
                      <td className="py-2 text-center text-green-400">{stats.solved}</td>
                      <td className="py-2 text-center">{stats.attempted}</td>
                      <td className="py-2 text-center font-bold">{stats.solveRate.toFixed(0)}%</td>
                      <td className="py-2 text-center text-yellow-400">{stats.avgTime.toFixed(1)} min</td>
                    </tr>
                  ))}
              </tbody>
            </table>
          </div>
        </div>
        
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Problem Solving Trends</h3>
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="flex justify-between items-center mb-2">
              <span className="text-white font-bold">Daily Problems</span>
              <span className="text-gray-400 text-xs">Last 14 days</span>
            </div>
            <div className="h-40 flex items-end space-x-1">
              {Array(14).fill(0).map((_, index) => {
                const height = Math.floor(Math.random() * 80) + 10;
                const solvedHeight = Math.floor(height * (Math.random() * 0.4 + 0.6));
                
                return (
                  <div key={`day-${index}`} className="flex-1 flex flex-col-reverse">
                    <div className="w-full text-center text-gray-500 text-xs mt-1">
                      {index + 1}
                    </div>
                    <div className="w-full bg-gray-800 rounded-t-sm" style={{ height: `${height}%` }}>
                      <div className="w-full bg-cyan-500 rounded-t-sm" style={{ height: `${solvedHeight}%` }} />
                    </div>
                  </div>
                );
              })}
            </div>
            <div className="flex justify-between text-xs text-gray-400 mt-2">
              <div className="flex items-center">
                <div className="w-3 h-3 bg-cyan-500 mr-1 rounded-sm"></div>
                <span>Solved</span>
              </div>
              <div className="flex items-center">
                <div className="w-3 h-3 bg-gray-800 mr-1 rounded-sm"></div>
                <span>Attempted</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
  
  const ResourcesTab = () => (
    <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
      <div className="bg-gray-800 rounded-lg p-4">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Recommended Resources</h3>
        
        <div className="space-y-4">
          <div className="bg-gray-900 p-3 rounded-lg">
            <h4 className="text-pink-400 font-bold">Problem Practice Platforms</h4>
            <div className="space-y-2 mt-2">
              <div className="flex items-start">
                <div className="bg-gray-800 p-2 rounded-lg mr-3">
                  <div className="w-8 h-8 flex items-center justify-center text-lg text-cyan-400 font-bold">L</div>
                </div>
                <div>
                  <div className="text-white font-bold">LeetCode</div>
                  <div className="text-gray-400 text-sm">Best overall practice platform with comprehensive problem set</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 p-2 rounded-lg mr-3">
                  <div className="w-8 h-8 flex items-center justify-center text-lg text-green-400 font-bold">C</div>
                </div>
                <div>
                  <div className="text-white font-bold">Codeforces</div>
                  <div className="text-gray-400 text-sm">For competitive programming and advanced algorithms</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 p-2 rounded-lg mr-3">
                  <div className="w-8 h-8 flex items-center justify-center text-lg text-yellow-400 font-bold">A</div>
                </div>
                <div>
                  <div className="text-white font-bold">AlgoExpert</div>
                  <div className="text-gray-400 text-sm">Curated problems with video explanations</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 p-2 rounded-lg mr-3">
                  <div className="w-8 h-8 flex items-center justify-center text-lg text-pink-400 font-bold">H</div>
                </div>
                <div>
                  <div className="text-white font-bold">HackerRank</div>
                  <div className="text-gray-400 text-sm">Good for beginners and specific patterns</div>
                </div>
              </div>
            </div>
          </div>
          
          <div className="bg-gray-900 p-3 rounded-lg">
            <h4 className="text-yellow-400 font-bold">Books</h4>
            <div className="space-y-2 mt-2">
              <div>
                <div className="text-white font-bold">"Cracking the Coding Interview"</div>
                <div className="text-gray-400 text-sm">by Gayle Laakmann McDowell</div>
                <div className="text-cyan-400 text-xs mt-1">Industry standard preparation guide</div>
              </div>
              
              <div>
                <div className="text-white font-bold">"The Algorithm Design Manual"</div>
                <div className="text-gray-400 text-sm">by Steven Skiena</div>
                <div className="text-cyan-400 text-xs mt-1">Practical approach to algorithm design</div>
              </div>
              
              <div>
                <div className="text-white font-bold">"Competitive Programming"</div>
                <div className="text-gray-400 text-sm">by Steven and Felix Halim</div>
                <div className="text-cyan-400 text-xs mt-1">Advanced competitive programming techniques</div>
              </div>
              
              <div>
                <div className="text-white font-bold">"Elements of Programming Interviews in C++"</div>
                <div className="text-gray-400 text-sm">by Aziz, Lee, & Prakash</div>
                <div className="text-cyan-400 text-xs mt-1">In-depth C++ focus with detailed explanations</div>
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <div className="space-y-6">
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Advanced Courses</h3>
          
          <div className="space-y-3">
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">Stanford's CS 97SI</div>
              <div className="text-gray-400 text-sm">Competitive programming course</div>
              <div className="flex mt-2 space-x-2">
                <span className="bg-gray-800 text-pink-400 text-xs px-2 py-1 rounded">Advanced</span>
                <span className="bg-gray-800 text-yellow-400 text-xs px-2 py-1 rounded">Competitive</span>
              </div>
            </div>
            
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">Princeton's Algorithms Part I & II</div>
              <div className="text-gray-400 text-sm">Theoretical foundations of algorithms</div>
              <div className="flex mt-2 space-x-2">
                <span className="bg-gray-800 text-green-400 text-xs px-2 py-1 rounded">Fundamentals</span>
                <span className="bg-gray-800 text-yellow-400 text-xs px-2 py-1 rounded">Theory</span>
              </div>
            </div>
            
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">MIT 6.006 Introduction to Algorithms</div>
              <div className="text-gray-400 text-sm">Academic rigor with practical applications</div>
              <div className="flex mt-2 space-x-2">
                <span className="bg-gray-800 text-cyan-400 text-xs px-2 py-1 rounded">Academic</span>
                <span className="bg-gray-800 text-green-400 text-xs px-2 py-1 rounded">Comprehensive</span>
              </div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Communities</h3>
          
          <div className="space-y-3">
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">r/leetcode</div>
              <div className="text-gray-400 text-sm">Reddit community for discussion and support</div>
              <div className="text-pink-400 text-xs mt-1">95,000+ members</div>
            </div>
            
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">Blind</div>
              <div className="text-gray-400 text-sm">Tech interview discussions from industry professionals</div>
              <div className="text-pink-400 text-xs mt-1">Anonymous tech community</div>
            </div>
            
            <div className="bg-gray-900 p-3 rounded-lg">
              <div className="text-white font-bold">Discord DSA Communities</div>
              <div className="text-gray-400 text-sm">Real-time help and problem discussions</div>
              <div className="text-pink-400 text-xs mt-1">Interactive learning environment</div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Beyond DSA</h3>
          
          <div className="grid grid-cols-2 gap-2">
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">System Design</div>
              <div className="text-gray-400 text-xs">Architecture & scalability</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">C++ Mastery</div>
              <div className="text-gray-400 text-xs">Language idioms & STL</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">Clean Code</div>
              <div className="text-gray-400 text-xs">Readability & maintainability</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">Testing</div>
              <div className="text-gray-400 text-xs">Unit, integration & system</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">Development Tools</div>
              <div className="text-gray-400 text-xs">Git, debugging & profiling</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-white text-sm font-bold">Communication</div>
              <div className="text-gray-400 text-xs">Technical writing & presenting</div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
  
  const NextStepsTab = () => (
    <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
      <div className="md:col-span-2 bg-gray-800 rounded-lg p-4">
        <h3 className="text-cyan-400 text-lg font-bold mb-3">Building Your DSA Practice System</h3>
        
        <div className="space-y-4">
          <div className="bg-gray-900 p-3 rounded-lg">
            <h4 className="text-pink-400 font-bold">Daily Routine (30-60 minutes)</h4>
            <div className="mt-2 space-y-2">
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-cyan-400 mr-2 flex-shrink-0">1</div>
                <div>
                  <div className="text-white">Review a concept from your notes (5 minutes)</div>
                  <div className="text-gray-400 text-sm">Quick refresh of a key algorithm or data structure</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-cyan-400 mr-2 flex-shrink-0">2</div>
                <div>
                  <div className="text-white">Solve 1-2 problems (20-45 minutes)</div>
                  <div className="text-gray-400 text-sm">Focus on your weak areas 70% of the time</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-cyan-400 mr-2 flex-shrink-0">3</div>
                <div>
                  <div className="text-white">Compare your solution to model answers (5 minutes)</div>
                  <div className="text-gray-400 text-sm">Study optimal approaches after submitting your own</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-cyan-400 mr-2 flex-shrink-0">4</div>
                <div>
                  <div className="text-white">Log your performance (5 minutes)</div>
                  <div className="text-gray-400 text-sm">Track category, difficulty, time spent, and notes</div>
                </div>
              </div>
            </div>
          </div>
          
          <div className="bg-gray-900 p-3 rounded-lg">
            <h4 className="text-yellow-400 font-bold">Weekly Review (2 hours)</h4>
            <div className="mt-2 space-y-2">
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-yellow-400 mr-2 flex-shrink-0">1</div>
                <div>
                  <div className="text-white">Review your week's performance (30 minutes)</div>
                  <div className="text-gray-400 text-sm">Analyze patterns in your solving approach</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-yellow-400 mr-2 flex-shrink-0">2</div>
                <div>
                  <div className="text-white">Deep-dive into one weak area (30 minutes)</div>
                  <div className="text-gray-400 text-sm">Study the theoretical foundations of a challenging concept</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-yellow-400 mr-2 flex-shrink-0">3</div>
                <div>
                  <div className="text-white">Solve 2-3 harder problems applying your learning (60 minutes)</div>
                  <div className="text-gray-400 text-sm">Reinforce your deep-dive with practical application</div>
                </div>
              </div>
            </div>
          </div>
          
          <div className="bg-gray-900 p-3 rounded-lg">
            <h4 className="text-green-400 font-bold">Monthly Challenge (4 hours)</h4>
            <div className="mt-2 space-y-2">
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-green-400 mr-2 flex-shrink-0">1</div>
                <div>
                  <div className="text-white">Mock interview with time constraints (1 hour)</div>
                  <div className="text-gray-400 text-sm">Simulate real interview pressure</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-green-400 mr-2 flex-shrink-0">2</div>
                <div>
                  <div className="text-white">Attempt a "Hard" problem from an unfamiliar category (1 hour)</div>
                  <div className="text-gray-400 text-sm">Push your boundaries with new challenges</div>
                </div>
              </div>
              
              <div className="flex items-start">
                <div className="bg-gray-800 rounded-full w-6 h-6 flex items-center justify-center text-green-400 mr-2 flex-shrink-0">3</div>
                <div>
                  <div className="text-white">Revisit and optimize past solutions (2 hours)</div>
                  <div className="text-gray-400 text-sm">Refactor old code with new insights and techniques</div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <div className="space-y-6">
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">The Forgetting Curve</h3>
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="mb-4">
              <img 
                className="w-full h-auto rounded-lg" 
                src="/api/placeholder/300/200" 
                alt="Forgetting curve diagram"
              />
            </div>
            <div className="text-white text-sm">
              Without practice, you'll forget <span className="text-pink-400 font-bold">80%</span> of what you learned within <span className="text-pink-400 font-bold">30 days</span>.
            </div>
            <div className="text-gray-400 text-xs mt-2">
              The spaced repetition system counteracts this natural memory decay by strategically timing your review sessions.
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Common Pitfalls to Avoid</h3>
          <div className="space-y-2">
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-pink-400 font-bold">Solution Memorization</div>
              <div className="text-gray-400 text-xs">Understand patterns, don't memorize solutions</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-pink-400 font-bold">Skipping Fundamentals</div>
              <div className="text-gray-400 text-xs">Master the basics before advanced topics</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-pink-400 font-bold">Language Hopping</div>
              <div className="text-gray-400 text-xs">Stick with C++ for DSA mastery</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-pink-400 font-bold">Complexity Blindness</div>
              <div className="text-gray-400 text-xs">Always analyze time/space complexity</div>
            </div>
            
            <div className="bg-gray-900 p-2 rounded-lg">
              <div className="text-pink-400 font-bold">Tutorial Addiction</div>
              <div className="text-gray-400 text-xs">Watching vs. doing (do more, watch less)</div>
            </div>
          </div>
        </div>
        
        <div className="bg-gray-800 rounded-lg p-4">
          <h3 className="text-cyan-400 text-lg font-bold mb-3">Final Thoughts</h3>
          <div className="bg-gray-900 p-3 rounded-lg">
            <div className="text-white">
              The real difference between good and great engineers isn't what they know at any given moment, but their commitment to continuous improvement.
            </div>
            <div className="text-gray-400 text-sm mt-2">
              The habit of systematic learning you've developed during these 45 days is more valuable than any individual algorithm.
            </div>
            <div className="text-pink-400 text-sm mt-2">
              Keep practicing. Keep learning. Keep building.
            </div>
          </div>
        </div>
      </div>
    </div>
  );
  
  return (
    <div className="font-sans min-h-screen bg-gray-900 text-white p-4">
      <div className="max-w-7xl mx-auto">
        <div className="flex flex-col items-center mb-8">
          <h1 className="text-3xl font-bold text-transparent bg-clip-text bg-gradient-to-r from-cyan-400 via-pink-500 to-yellow-400">
            DSA Journey Complete
          </h1>
          <p className="text-gray-400 mt-2">Day 45: Journey Completion & Next Steps #DSAin45</p>
        </div>
        
        {/* Tabs navigation */}
        <div className="flex justify-center mb-8 overflow-x-auto">
          <div className="bg-gray-800 rounded-lg p-1 flex space-x-1">
            <button 
              className={`px-4 py-2 text-sm rounded-md ${activeTab === 'overview' ? 'bg-gray-700 text-white' : 'text-gray-400 hover:text-white hover:bg-gray-700'}`}
              onClick={() => setActiveTab('overview')}
            >
              Overview
            </button>
            <button 
              className={`px-4 py-2 text-sm rounded-md ${activeTab === 'stats' ? 'bg-gray-700 text-white' : 'text-gray-400 hover:text-white hover:bg-gray-700'}`}
              onClick={() => setActiveTab('stats')}
            >
              Performance Stats
            </button>
            <button 
              className={`px-4 py-2 text-sm rounded-md ${activeTab === 'resources' ? 'bg-gray-700 text-white' : 'text-gray-400 hover:text-white hover:bg-gray-700'}`}
              onClick={() => setActiveTab('resources')}
            >
              Resources
            </button>
            <button 
              className={`px-4 py-2 text-sm rounded-md ${activeTab === 'next' ? 'bg-gray-700 text-white' : 'text-gray-400 hover:text-white hover:bg-gray-700'}`}
              onClick={() => setActiveTab('next')}
            >
              Next Steps
            </button>
          </div>
        </div>
        
        {/* Tab content */}
        {activeTab === 'overview' && <OverviewTab />}
        {activeTab === 'stats' && <StatsTab />}
        {activeTab === 'resources' && <ResourcesTab />}
        {activeTab === 'next' && <NextStepsTab />}
        
        <div className="mt-8 text-center text-gray-500 text-xs">
          #DSAin45 Day 45: Journey Completion & Next Steps
        </div>
      </div>
    </div>
  );
};

export default DSADashboard;