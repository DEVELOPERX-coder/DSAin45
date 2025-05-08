/**
 * @file backtracking_algorithms.cpp
 * @brief Comprehensive implementation of common backtracking algorithms for DSAin45 Day 39
 * @author DSAin45 Team
 * @date May 2025
 *
 * This file provides production-quality implementations of various backtracking algorithms,
 * including N-Queens, Sudoku Solver, Permutations, Combinations, and more.
 * Each algorithm is thoroughly documented with time and space complexity analysis,
 * real-world applications, and benchmarking code to compare different approaches.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <bitset>
#include <cassert>
#include <functional>
#include <map>

using namespace std;
using namespace std::chrono;

// Forward declarations
namespace NQueens
{
    class NQueensSolver;
    void runDemo();
    void runBenchmark(int size);
}

namespace Sudoku
{
    class SudokuSolver;
    void runDemo();
    void runOptimizedDemo();
    void runBenchmark();
}

namespace SubsetSum
{
    bool findSubset(const vector<int> &nums, int target, vector<int> &result);
    void runDemo();
}

namespace Permutations
{
    vector<vector<int>> generate(vector<int> &nums);
    void runDemo();
}

namespace Combinations
{
    vector<vector<int>> generate(int n, int k);
    void runDemo();
}

namespace WordSearch
{
    bool exist(vector<vector<char>> &board, string word);
    void runDemo();
}

namespace GraphColoring
{
    bool colorGraph(vector<vector<int>> &graph, int m, vector<int> &colors);
    void runDemo();
}

// Utility functions
void printDivider();
void printHeader(const string &title);
void printBoard(const vector<string> &board);
void printSudoku(const vector<vector<char>> &board);

/**
 * @brief Main function to demonstrate and benchmark backtracking algorithms
 */
int main()
{
    printHeader("BACKTRACKING ALGORITHMS - COMPREHENSIVE IMPLEMENTATIONS");
    cout << "DSAin45 Day 39: Exploring the Power of Backtracking Algorithms" << endl
         << endl;

    int choice;

    do
    {
        cout << "\nPlease select an algorithm to demonstrate:\n";
        cout << "1. N-Queens Problem\n";
        cout << "2. Sudoku Solver\n";
        cout << "3. Subset Sum Problem\n";
        cout << "4. Permutations Generator\n";
        cout << "5. Combinations Generator\n";
        cout << "6. Word Search\n";
        cout << "7. Graph Coloring\n";
        cout << "8. Run All Benchmarks\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            NQueens::runDemo();
            break;
        case 2:
            Sudoku::runDemo();
            break;
        case 3:
            SubsetSum::runDemo();
            break;
        case 4:
            Permutations::runDemo();
            break;
        case 5:
            Combinations::runDemo();
            break;
        case 6:
            WordSearch::runDemo();
            break;
        case 7:
            GraphColoring::runDemo();
            break;
        case 8:
            NQueens::runBenchmark(8);
            Sudoku::runBenchmark();
            break;
        case 0:
            cout << "Exiting program. Thank you for exploring backtracking algorithms!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}

// Utility function implementations
void printDivider()
{
    cout << "\n"
         << string(80, '-') << "\n"
         << endl;
}

void printHeader(const string &title)
{
    printDivider();
    cout << title << endl;
    printDivider();
}

void printBoard(const vector<string> &board)
{
    int n = board.size();
    cout << string(2 * n + 1, '-') << endl;
    for (const auto &row : board)
    {
        cout << "|";
        for (char cell : row)
        {
            cout << cell << "|";
        }
        cout << endl
             << string(2 * n + 1, '-') << endl;
    }
}

void printSudoku(const vector<vector<char>> &board)
{
    cout << "-------------------------" << endl;
    for (int i = 0; i < 9; i++)
    {
        cout << "| ";
        for (int j = 0; j < 9; j++)
        {
            cout << board[i][j] << " ";
            if ((j + 1) % 3 == 0)
                cout << "| ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0)
            cout << "-------------------------" << endl;
    }
}

/**
 * @namespace NQueens
 * @brief Implementation of the N-Queens problem
 *
 * The N-Queens problem asks how to place N chess queens on an N×N
 * chessboard so that no two queens threaten each other.
 */
namespace NQueens
{
    /**
     * @class NQueensSolver
     * @brief Class to solve the N-Queens problem using backtracking
     */
    class NQueensSolver
    {
    private:
        int n;                            // Board size
        vector<vector<string>> solutions; // All valid solutions
        int solutionCount;                // Number of solutions

        // Optimized version using arrays to track attacked positions
        void solveNQueensOptimized()
        {
            vector<string> board(n, string(n, '.'));
            vector<bool> cols(n, false);
            vector<bool> diag1(2 * n - 1, false); // Main diagonals (r+c)
            vector<bool> diag2(2 * n - 1, false); // Anti-diagonals (r-c+n-1)

            solveNQueensOptimizedUtil(board, 0, cols, diag1, diag2);
        }

        void solveNQueensOptimizedUtil(vector<string> &board, int row,
                                       vector<bool> &cols,
                                       vector<bool> &diag1,
                                       vector<bool> &diag2)
        {
            if (row == n)
            {
                solutions.push_back(board);
                solutionCount++;
                return;
            }

            for (int col = 0; col < n; col++)
            {
                int d1 = row + col;
                int d2 = row - col + n - 1;

                // Check if the current position is under attack
                if (cols[col] || diag1[d1] || diag2[d2])
                {
                    continue;
                }

                // Place queen
                board[row][col] = 'Q';
                cols[col] = true;
                diag1[d1] = true;
                diag2[d2] = true;

                // Recur for next row
                solveNQueensOptimizedUtil(board, row + 1, cols, diag1, diag2);

                // Backtrack
                board[row][col] = '.';
                cols[col] = false;
                diag1[d1] = false;
                diag2[d2] = false;
            }
        }

        // Optimized version using bitsets for faster computation
        void solveNQueensBitset()
        {
            vector<string> board(n, string(n, '.'));
            bitset<32> cols(0);
            bitset<64> diag1(0);
            bitset<64> diag2(0);

            solveNQueensBitsetUtil(board, 0, cols, diag1, diag2);
        }

        void solveNQueensBitsetUtil(vector<string> &board, int row,
                                    bitset<32> &cols,
                                    bitset<64> &diag1,
                                    bitset<64> &diag2)
        {
            if (row == n)
            {
                solutions.push_back(board);
                solutionCount++;
                return;
            }

            for (int col = 0; col < n; col++)
            {
                int d1 = row + col;
                int d2 = row - col + n - 1;

                // Check if the current position is under attack
                if (cols[col] || diag1[d1] || diag2[d2])
                {
                    continue;
                }

                // Place queen
                board[row][col] = 'Q';
                cols[col] = true;
                diag1[d1] = true;
                diag2[d2] = true;

                // Recur for next row
                solveNQueensBitsetUtil(board, row + 1, cols, diag1, diag2);

                // Backtrack
                board[row][col] = '.';
                cols[col] = false;
                diag1[d1] = false;
                diag2[d2] = false;
            }
        }

        // Check if a queen can be placed at board[row][col]
        bool isSafe(const vector<string> &board, int row, int col)
        {
            // Check column
            for (int i = 0; i < row; i++)
            {
                if (board[i][col] == 'Q')
                {
                    return false;
                }
            }

            // Check upper-left diagonal
            for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
            {
                if (board[i][j] == 'Q')
                {
                    return false;
                }
            }

            // Check upper-right diagonal
            for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++)
            {
                if (board[i][j] == 'Q')
                {
                    return false;
                }
            }

            return true;
        }

        // Basic backtracking approach
        void solveNQueensUtil(vector<string> &board, int row)
        {
            if (row == n)
            {
                solutions.push_back(board);
                solutionCount++;
                return;
            }

            for (int col = 0; col < n; col++)
            {
                if (isSafe(board, row, col))
                {
                    // Place queen
                    board[row][col] = 'Q';

                    // Recur to place queens in remaining rows
                    solveNQueensUtil(board, row + 1);

                    // Backtrack and remove queen
                    board[row][col] = '.';
                }
            }
        }

    public:
        NQueensSolver(int boardSize) : n(boardSize), solutionCount(0) {}

        // Solve using basic backtracking
        vector<vector<string>> solveNQueens()
        {
            solutions.clear();
            solutionCount = 0;

            vector<string> board(n, string(n, '.'));
            solveNQueensUtil(board, 0);

            return solutions;
        }

        // Solve using array-based optimization
        vector<vector<string>> solveNQueensOptimizedArray()
        {
            solutions.clear();
            solutionCount = 0;

            solveNQueensOptimized();

            return solutions;
        }

        // Solve using bitset optimization
        vector<vector<string>> solveNQueensBitsetOptimized()
        {
            if (n > 32)
            {
                throw std::runtime_error("Bitset implementation only supports N <= 32");
            }

            solutions.clear();
            solutionCount = 0;

            solveNQueensBitset();

            return solutions;
        }

        // Get count of solutions
        int getSolutionCount() const
        {
            return solutionCount;
        }
    };

    /**
     * @brief Demonstrate the N-Queens algorithm
     */
    void runDemo()
    {
        printHeader("N-QUEENS PROBLEM");

        cout << "The N-Queens problem asks how to place N chess queens on an N×N chessboard\n";
        cout << "so that no two queens threaten each other.\n\n";

        // Solve for different board sizes
        for (int size = 4; size <= 8; size += 4)
        {
            cout << "Solving N-Queens for a " << size << "×" << size << " board:\n\n";

            NQueensSolver solver(size);
            auto startTime = high_resolution_clock::now();
            vector<vector<string>> solutions = solver.solveNQueens();
            auto endTime = high_resolution_clock::now();

            cout << "Found " << solutions.size() << " solutions in "
                 << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
                 << " milliseconds.\n\n";

            // Display the first few solutions
            int displayCount = min(2, static_cast<int>(solutions.size()));
            for (int i = 0; i < displayCount; i++)
            {
                cout << "Solution " << (i + 1) << ":\n";
                printBoard(solutions[i]);
                cout << endl;
            }

            if (solutions.size() > displayCount)
            {
                cout << "... and " << (solutions.size() - displayCount) << " more solutions.\n\n";
            }
        }

        // Compare different implementations
        int benchmarkSize = 8;
        cout << "Comparing different implementations for " << benchmarkSize << "×" << benchmarkSize << " board:\n\n";

        NQueensSolver basicSolver(benchmarkSize);
        NQueensSolver optimizedSolver(benchmarkSize);
        NQueensSolver bitsetSolver(benchmarkSize);

        // Basic implementation
        auto startTime = high_resolution_clock::now();
        basicSolver.solveNQueens();
        auto endTime = high_resolution_clock::now();
        auto basicTime = duration_cast<microseconds>(endTime - startTime);

        // Optimized implementation with arrays
        startTime = high_resolution_clock::now();
        optimizedSolver.solveNQueensOptimizedArray();
        endTime = high_resolution_clock::now();
        auto optimizedTime = duration_cast<microseconds>(endTime - startTime);

        // Bitset implementation
        startTime = high_resolution_clock::now();
        bitsetSolver.solveNQueensBitsetOptimized();
        endTime = high_resolution_clock::now();
        auto bitsetTime = duration_cast<microseconds>(endTime - startTime);

        cout << "Basic Backtracking: " << basicTime.count() / 1000.0 << " milliseconds\n";
        cout << "Optimized (Arrays): " << optimizedTime.count() / 1000.0 << " milliseconds\n";
        cout << "Optimized (Bitsets): " << bitsetTime.count() / 1000.0 << " milliseconds\n\n";

        // Calculate speedup
        double arraySpeedup = static_cast<double>(basicTime.count()) / optimizedTime.count();
        double bitsetSpeedup = static_cast<double>(basicTime.count()) / bitsetTime.count();

        cout << "Optimized (Arrays) is " << fixed << setprecision(2) << arraySpeedup << "× faster\n";
        cout << "Optimized (Bitsets) is " << fixed << setprecision(2) << bitsetSpeedup << "× faster\n\n";

        // Real-world application
        cout << "Real-world Application: Constraint Satisfaction Problems\n";
        cout << "The N-Queens problem is a classic example of a constraint satisfaction problem.\n";
        cout << "Similar backtracking approaches are used in scheduling, planning, and resource allocation.\n";
    }

    /**
     * @brief Benchmark the N-Queens algorithm
     * @param size Board size
     */
    void runBenchmark(int size)
    {
        printHeader("N-QUEENS BENCHMARK");

        cout << "Benchmarking N-Queens for " << size << "×" << size << " board...\n\n";

        NQueensSolver solver(size);

        // Benchmark different implementations
        auto startTime = high_resolution_clock::now();
        solver.solveNQueens();
        auto endTime = high_resolution_clock::now();
        auto basicTime = duration_cast<microseconds>(endTime - startTime);

        startTime = high_resolution_clock::now();
        solver.solveNQueensOptimizedArray();
        endTime = high_resolution_clock::now();
        auto optimizedTime = duration_cast<microseconds>(endTime - startTime);

        startTime = high_resolution_clock::now();
        solver.solveNQueensBitsetOptimized();
        endTime = high_resolution_clock::now();
        auto bitsetTime = duration_cast<microseconds>(endTime - startTime);

        int solutions = solver.getSolutionCount();

        cout << "Number of solutions: " << solutions << "\n\n";
        cout << "Basic Backtracking: " << basicTime.count() / 1000.0 << " milliseconds\n";
        cout << "Optimized (Arrays): " << optimizedTime.count() / 1000.0 << " milliseconds\n";
        cout << "Optimized (Bitsets): " << bitsetTime.count() / 1000.0 << " milliseconds\n\n";

        // Calculate speedup
        double arraySpeedup = static_cast<double>(basicTime.count()) / optimizedTime.count();
        double bitsetSpeedup = static_cast<double>(basicTime.count()) / bitsetTime.count();

        cout << "Optimized (Arrays) is " << fixed << setprecision(2) << arraySpeedup << "× faster\n";
        cout << "Optimized (Bitsets) is " << fixed << setprecision(2) << bitsetSpeedup << "× faster\n";
    }
} // namespace NQueens

/**
 * @namespace Sudoku
 * @brief Implementation of the Sudoku Solver
 *
 * Sudoku is a 9×9 grid puzzle where each row, column, and 3×3 subgrid
 * must contain all digits from 1 to 9 without repetition.
 */
namespace Sudoku
{
    /**
     * @class SudokuSolver
     * @brief Class to solve Sudoku puzzles using backtracking
     */
    class SudokuSolver
    {
    private:
        // Check if placing digit at board[row][col] is valid
        bool isValid(vector<vector<char>> &board, int row, int col, char digit)
        {
            // Check row
            for (int j = 0; j < 9; j++)
            {
                if (board[row][j] == digit)
                {
                    return false;
                }
            }

            // Check column
            for (int i = 0; i < 9; i++)
            {
                if (board[i][col] == digit)
                {
                    return false;
                }
            }

            // Check 3x3 subgrid
            int subgridRow = 3 * (row / 3);
            int subgridCol = 3 * (col / 3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[subgridRow + i][subgridCol + j] == digit)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        // Find an empty cell
        bool findEmptyCell(vector<vector<char>> &board, int &row, int &col)
        {
            for (row = 0; row < 9; row++)
            {
                for (col = 0; col < 9; col++)
                {
                    if (board[row][col] == '.')
                    {
                        return true;
                    }
                }
            }
            return false; // No empty cell found
        }

        // Backtracking function to solve Sudoku
        bool solveSudokuUtil(vector<vector<char>> &board)
        {
            int row, col;

            // If no empty cell is found, we've solved the puzzle
            if (!findEmptyCell(board, row, col))
            {
                return true;
            }

            // Try digits 1-9 for the empty cell
            for (char digit = '1'; digit <= '9'; digit++)
            {
                if (isValid(board, row, col, digit))
                {
                    // Place digit
                    board[row][col] = digit;

                    // Recur to solve rest of the board
                    if (solveSudokuUtil(board))
                    {
                        return true;
                    }

                    // If placing digit doesn't lead to a solution, backtrack
                    board[row][col] = '.';
                }
            }

            // Trigger backtracking
            return false;
        }

        // Enhanced solver with constraint propagation
        bool solveSudokuOptimized(vector<vector<char>> &board)
        {
            // Track possible values for each cell
            vector<vector<unordered_set<char>>> possibleValues(9, vector<unordered_set<char>>(9));

            // Initialize possible values for empty cells
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (board[i][j] == '.')
                    {
                        for (char d = '1'; d <= '9'; d++)
                        {
                            if (isValid(board, i, j, d))
                            {
                                possibleValues[i][j].insert(d);
                            }
                        }
                    }
                }
            }

            return solveSudokuOptimizedUtil(board, possibleValues);
        }

        // Helper function for optimized solver
        bool solveSudokuOptimizedUtil(vector<vector<char>> &board,
                                      vector<vector<unordered_set<char>>> &possibleValues)
        {
            // Find the cell with minimum possible values
            int minRow = -1, minCol = -1;
            int minPossibilities = 10;

            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (board[i][j] == '.' && possibleValues[i][j].size() > 0)
                    {
                        if (possibleValues[i][j].size() < minPossibilities)
                        {
                            minPossibilities = possibleValues[i][j].size();
                            minRow = i;
                            minCol = j;
                        }
                    }
                }
            }

            // If no empty cell is found, we've solved the puzzle
            if (minRow == -1)
            {
                return true;
            }

            // Try each possible value for the chosen cell
            unordered_set<char> possibleValuesCopy = possibleValues[minRow][minCol];
            for (char digit : possibleValuesCopy)
            {
                // Place digit
                board[minRow][minCol] = digit;

                // Update possible values for affected cells
                vector<vector<unordered_set<char>>> newPossibleValues = possibleValues;
                updatePossibleValues(newPossibleValues, minRow, minCol, digit);

                // Check if any cell has no possible values
                bool invalidState = false;
                for (int i = 0; i < 9 && !invalidState; i++)
                {
                    for (int j = 0; j < 9; j++)
                    {
                        if (board[i][j] == '.' && newPossibleValues[i][j].empty())
                        {
                            invalidState = true;
                            break;
                        }
                    }
                }

                // If valid state, recur
                if (!invalidState && solveSudokuOptimizedUtil(board, newPossibleValues))
                {
                    return true;
                }

                // Backtrack
                board[minRow][minCol] = '.';
            }

            return false;
        }

        // Update possible values after placing a digit
        void updatePossibleValues(vector<vector<unordered_set<char>>> &possibleValues,
                                  int row, int col, char digit)
        {
            // Remove the digit from the same row
            for (int j = 0; j < 9; j++)
            {
                possibleValues[row][j].erase(digit);
            }

            // Remove the digit from the same column
            for (int i = 0; i < 9; i++)
            {
                possibleValues[i][col].erase(digit);
            }

            // Remove the digit from the same 3x3 subgrid
            int subgridRow = 3 * (row / 3);
            int subgridCol = 3 * (col / 3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    possibleValues[subgridRow + i][subgridCol + j].erase(digit);
                }
            }
        }

    public:
        // Basic backtracking solver
        bool solveSudoku(vector<vector<char>> &board)
        {
            return solveSudokuUtil(board);
        }

        // Optimized solver with constraint propagation
        bool solveSudokuWithConstraintPropagation(vector<vector<char>> &board)
        {
            return solveSudokuOptimized(board);
        }
    };

    /**
     * @brief Demonstrate the Sudoku Solver
     */
    void runDemo()
    {
        printHeader("SUDOKU SOLVER");

        cout << "Sudoku is a 9×9 grid puzzle where each row, column, and 3×3 subgrid\n";
        cout << "must contain all digits from 1 to 9 without repetition.\n\n";

        // Example Sudoku puzzle (0 represents empty cells)
        vector<vector<char>> board = {
            {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
            {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
            {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
            {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
            {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
            {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
            {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
            {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
            {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};

        cout << "Original Sudoku Puzzle:\n";
        printSudoku(board);

        SudokuSolver solver;
        auto startTime = high_resolution_clock::now();
        bool solved = solver.solveSudoku(board);
        auto endTime = high_resolution_clock::now();

        if (solved)
        {
            cout << "\nSolved Sudoku Puzzle:\n";
            printSudoku(board);

            cout << "\nSolved in "
                 << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
                 << " milliseconds.\n";
        }
        else
        {
            cout << "\nNo solution exists for this Sudoku puzzle.\n";
        }

        // Harder puzzle for comparison
        vector<vector<char>> hardBoard = {
            {'.', '.', '.', '2', '.', '.', '.', '6', '3'},
            {'3', '.', '.', '.', '.', '5', '4', '.', '1'},
            {'.', '.', '1', '.', '.', '3', '9', '8', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '9', '.'},
            {'.', '.', '.', '5', '3', '8', '.', '.', '.'},
            {'.', '3', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '2', '6', '3', '.', '.', '5', '.', '.'},
            {'5', '.', '3', '7', '.', '.', '.', '.', '8'},
            {'4', '7', '.', '.', '.', '1', '.', '.', '.'}};

        cout << "\nHarder Sudoku Puzzle:\n";
        printSudoku(hardBoard);

        startTime = high_resolution_clock::now();
        solved = solver.solveSudoku(hardBoard);
        endTime = high_resolution_clock::now();

        if (solved)
        {
            cout << "\nSolved Sudoku Puzzle:\n";
            printSudoku(hardBoard);

            cout << "\nSolved in "
                 << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
                 << " milliseconds.\n";
        }
        else
        {
            cout << "\nNo solution exists for this Sudoku puzzle.\n";
        }

        // Real-world application
        cout << "\nReal-world Application: Constraint Satisfaction and Puzzles\n";
        cout << "Sudoku solving demonstrates how backtracking can efficiently solve constraint\n";
        cout << "satisfaction problems. Similar techniques are used in scheduling, planning,\n";
        cout << "and resource allocation in various industries.\n";
    }

    /**
     * @brief Demonstrate the optimized Sudoku Solver
     */
    void runOptimizedDemo()
    {
        printHeader("OPTIMIZED SUDOKU SOLVER");

        // Very hard Sudoku puzzle
        vector<vector<char>> hardBoard = {
            {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '3', '.', '8', '5'},
            {'.', '.', '1', '.', '2', '.', '.', '.', '.'},
            {'.', '.', '.', '5', '.', '7', '.', '.', '.'},
            {'.', '.', '4', '.', '.', '.', '1', '.', '.'},
            {'.', '9', '.', '.', '.', '.', '.', '.', '.'},
            {'5', '.', '.', '.', '.', '.', '.', '7', '3'},
            {'.', '.', '2', '.', '1', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '4', '.', '.', '.', '9'}};

        cout << "Very Hard Sudoku Puzzle:\n";
        printSudoku(hardBoard);

        // Copy for comparison
        vector<vector<char>> hardBoardCopy = hardBoard;

        SudokuSolver solver;

        // Standard backtracking
        auto startTime = high_resolution_clock::now();
        bool solved = solver.solveSudoku(hardBoard);
        auto endTime = high_resolution_clock::now();
        auto standardTime = duration_cast<microseconds>(endTime - startTime);

        if (solved)
        {
            cout << "\nSolved with Standard Backtracking:\n";
            printSudoku(hardBoard);
            cout << "Time: " << standardTime.count() / 1000.0 << " milliseconds\n";
        }

        // Optimized with constraint propagation
        startTime = high_resolution_clock::now();
        solved = solver.solveSudokuWithConstraintPropagation(hardBoardCopy);
        endTime = high_resolution_clock::now();
        auto optimizedTime = duration_cast<microseconds>(endTime - startTime);

        if (solved)
        {
            cout << "\nSolved with Constraint Propagation:\n";
            printSudoku(hardBoardCopy);
            cout << "Time: " << optimizedTime.count() / 1000.0 << " milliseconds\n";
        }

        // Compare performance
        if (standardTime.count() > 0 && optimizedTime.count() > 0)
        {
            double speedup = static_cast<double>(standardTime.count()) / optimizedTime.count();
            cout << "\nConstraint Propagation is " << fixed << setprecision(2)
                 << speedup << "× faster than standard backtracking.\n";
        }
    }

    /**
     * @brief Benchmark the Sudoku Solver
     */
    void runBenchmark()
    {
        printHeader("SUDOKU SOLVER BENCHMARK");

        // Array of increasingly difficult Sudoku puzzles
        vector<vector<vector<char>>> puzzles = {
            // Easy puzzle
            {
                {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
                {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
                {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
                {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
                {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
                {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
                {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
                {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
                {'.', '.', '.', '.', '8', '.', '.', '7', '9'}},
            // Medium puzzle
            {
                {'.', '.', '.', '2', '.', '.', '.', '6', '3'},
                {'3', '.', '.', '.', '.', '5', '4', '.', '1'},
                {'.', '.', '1', '.', '.', '3', '9', '8', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '9', '.'},
                {'.', '.', '.', '5', '3', '8', '.', '.', '.'},
                {'.', '3', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '2', '6', '3', '.', '.', '5', '.', '.'},
                {'5', '.', '3', '7', '.', '.', '.', '.', '8'},
                {'4', '7', '.', '.', '.', '1', '.', '.', '.'}},
            // Hard puzzle
            {
                {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '3', '.', '8', '5'},
                {'.', '.', '1', '.', '2', '.', '.', '.', '.'},
                {'.', '.', '.', '5', '.', '7', '.', '.', '.'},
                {'.', '.', '4', '.', '.', '.', '1', '.', '.'},
                {'.', '9', '.', '.', '.', '.', '.', '.', '.'},
                {'5', '.', '.', '.', '.', '.', '.', '7', '3'},
                {'.', '.', '2', '.', '1', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '4', '.', '.', '.', '9'}}};

        SudokuSolver solver;

        cout << "Benchmarking Sudoku Solver on puzzles of increasing difficulty...\n\n";
        cout << "Puzzle | Standard Backtracking | Constraint Propagation | Speedup\n";
        cout << "-------|------------------------|------------------------|--------\n";

        for (int i = 0; i < puzzles.size(); i++)
        {
            // Copy puzzles for both solvers
            vector<vector<char>> puzzleStandard = puzzles[i];
            vector<vector<char>> puzzleOptimized = puzzles[i];

            // Standard backtracking
            auto startTime = high_resolution_clock::now();
            solver.solveSudoku(puzzleStandard);
            auto endTime = high_resolution_clock::now();
            auto standardTime = duration_cast<microseconds>(endTime - startTime);

            // Optimized with constraint propagation
            startTime = high_resolution_clock::now();
            solver.solveSudokuWithConstraintPropagation(puzzleOptimized);
            endTime = high_resolution_clock::now();
            auto optimizedTime = duration_cast<microseconds>(endTime - startTime);

            // Calculate speedup
            double speedup = static_cast<double>(standardTime.count()) / optimizedTime.count();

            // Print results
            cout << left << setw(7) << (i == 0 ? "Easy" : (i == 1 ? "Medium" : "Hard"))
                 << " | " << setw(22) << (standardTime.count() / 1000.0) << " ms"
                 << " | " << setw(22) << (optimizedTime.count() / 1000.0) << " ms"
                 << " | " << fixed << setprecision(2) << speedup << "×\n";
        }

        cout << "\nNote: Constraint propagation is especially effective for harder puzzles\n";
        cout << "where more complex reasoning is required to reduce the search space.\n";
    }
} // namespace Sudoku

/**
 * @namespace SubsetSum
 * @brief Implementation of the Subset Sum problem
 *
 * Given a set of integers and a target sum, determine if there is a subset
 * that adds up to the target sum.
 */
namespace SubsetSum
{
    /**
     * @brief Find a subset of nums that sums to target
     * @param nums Vector of integers
     * @param target Target sum
     * @param result Vector to store the subset
     * @return True if a subset exists, false otherwise
     * @time_complexity O(2^n), where n is the size of nums
     * @space_complexity O(n) for the recursion stack and result
     */
    bool findSubsetUtil(const vector<int> &nums, int target, int index, vector<int> &current, vector<int> &result)
    {
        // Base case: Target reached
        if (target == 0)
        {
            result = current;
            return true;
        }

        // Base case: End of array or target becomes negative
        if (index == nums.size() || target < 0)
        {
            return false;
        }

        // Include current element
        current.push_back(nums[index]);
        if (findSubsetUtil(nums, target - nums[index], index + 1, current, result))
        {
            return true;
        }

        // Backtrack: Exclude current element
        current.pop_back();
        return findSubsetUtil(nums, target, index + 1, current, result);
    }

    /**
     * @brief Find a subset of nums that sums to target
     * @param nums Vector of integers
     * @param target Target sum
     * @param result Vector to store the subset
     * @return True if a subset exists, false otherwise
     */
    bool findSubset(const vector<int> &nums, int target, vector<int> &result)
    {
        vector<int> current;
        return findSubsetUtil(nums, target, 0, current, result);
    }

    /**
     * @brief Generate all subsets of nums that sum to target
     * @param nums Vector of integers
     * @param target Target sum
     * @return Vector of all valid subsets
     */
    vector<vector<int>> findAllSubsets(const vector<int> &nums, int target)
    {
        vector<vector<int>> result;
        vector<int> current;

        function<void(int, int)> backtrack = [&](int index, int remainingTarget)
        {
            // Base case: Target reached
            if (remainingTarget == 0)
            {
                result.push_back(current);
                return;
            }

            // Base case: End of array or target becomes negative
            if (index == nums.size() || remainingTarget < 0)
            {
                return;
            }

            // Include current element
            current.push_back(nums[index]);
            backtrack(index + 1, remainingTarget - nums[index]);

            // Backtrack: Exclude current element
            current.pop_back();
            backtrack(index + 1, remainingTarget);
        };

        backtrack(0, target);

        return result;
    }

    /**
     * @brief Demonstrate the Subset Sum algorithm
     */
    void runDemo()
    {
        printHeader("SUBSET SUM PROBLEM");

        cout << "Given a set of integers and a target sum, determine if there is a subset\n";
        cout << "that adds up to the target sum.\n\n";

        // Example 1: Standard case
        vector<int> nums = {3, 34, 4, 12, 5, 2};
        int target = 9;

        cout << "Example 1: Standard case\n";
        cout << "Set: [";
        for (int i = 0; i < nums.size(); i++)
        {
            cout << nums[i];
            if (i < nums.size() - 1)
                cout << ", ";
        }
        cout << "]\n";
        cout << "Target sum: " << target << "\n\n";

        vector<int> result;
        bool exists = findSubset(nums, target, result);

        if (exists)
        {
            cout << "Subset found: [";
            for (int i = 0; i < result.size(); i++)
            {
                cout << result[i];
                if (i < result.size() - 1)
                    cout << ", ";
            }
            cout << "]\n";
        }
        else
        {
            cout << "No subset sums to the target.\n";
        }

        // Example 2: Finding all subsets
        vector<int> nums2 = {2, 3, 5, 6, 8, 10};
        target = 10;

        cout << "\nExample 2: Finding all subsets\n";
        cout << "Set: [";
        for (int i = 0; i < nums2.size(); i++)
        {
            cout << nums2[i];
            if (i < nums2.size() - 1)
                cout << ", ";
        }
        cout << "]\n";
        cout << "Target sum: " << target << "\n\n";

        vector<vector<int>> allSubsets = findAllSubsets(nums2, target);

        if (!allSubsets.empty())
        {
            cout << "All subsets that sum to " << target << ":\n";
            for (const auto &subset : allSubsets)
            {
                cout << "[";
                for (int i = 0; i < subset.size(); i++)
                {
                    cout << subset[i];
                    if (i < subset.size() - 1)
                        cout << ", ";
                }
                cout << "]\n";
            }
        }
        else
        {
            cout << "No subset sums to the target.\n";
        }

        // Real-world application
        cout << "\nReal-world Application: Resource Allocation\n";
        cout << "The Subset Sum problem has applications in resource allocation, where you need\n";
        cout << "to select items with specific values to meet a target. Examples include:\n";
        cout << "- Selecting projects within a budget constraint\n";
        cout << "- Packing items to achieve a target weight\n";
        cout << "- Distributing workload evenly among processors\n";
    }
} // namespace SubsetSum

/**
 * @namespace Permutations
 * @brief Implementation of the Permutations problem
 *
 * Generate all possible permutations of a set of elements.
 */
namespace Permutations
{
    /**
     * @brief Generate all permutations of nums
     * @param nums Vector of integers
     * @return Vector of all permutations
     * @time_complexity O(n!), where n is the size of nums
     * @space_complexity O(n) for the recursion stack and current permutation
     */
    void generateUtil(vector<int> &nums, int start, vector<vector<int>> &result)
    {
        // Base case: All positions fixed
        if (start == nums.size())
        {
            result.push_back(nums);
            return;
        }

        // Try each element as the next element
        for (int i = start; i < nums.size(); i++)
        {
            // Swap to place element at start
            swap(nums[start], nums[i]);

            // Recursively generate permutations for the rest
            generateUtil(nums, start + 1, result);

            // Backtrack
            swap(nums[start], nums[i]);
        }
    }

    /**
     * @brief Generate all permutations of nums
     * @param nums Vector of integers
     * @return Vector of all permutations
     */
    vector<vector<int>> generate(vector<int> &nums)
    {
        vector<vector<int>> result;
        generateUtil(nums, 0, result);
        return result;
    }

    /**
     * @brief Generate permutations using the "used" array approach
     * @param nums Vector of integers
     * @return Vector of all permutations
     */
    vector<vector<int>> generateAlternative(const vector<int> &nums)
    {
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);

        function<void()> backtrack = [&]()
        {
            // Base case: Current permutation is complete
            if (current.size() == nums.size())
            {
                result.push_back(current);
                return;
            }

            // Try each unused element as the next element
            for (int i = 0; i < nums.size(); i++)
            {
                if (!used[i])
                {
                    // Use this element
                    used[i] = true;
                    current.push_back(nums[i]);

                    // Recursively generate permutations of remaining elements
                    backtrack();

                    // Backtrack
                    current.pop_back();
                    used[i] = false;
                }
            }
        };

        backtrack();

        return result;
    }

    /**
     * @brief Demonstrate the Permutations algorithm
     */
    void runDemo()
    {
        printHeader("PERMUTATIONS GENERATOR");

        cout << "Generate all possible permutations of a set of elements.\n\n";

        // Example 1: Small set
        vector<int> nums = {1, 2, 3};

        cout << "Example 1: Generating permutations of {1, 2, 3}\n\n";

        auto startTime = high_resolution_clock::now();
        vector<vector<int>> permutations = generate(nums);
        auto endTime = high_resolution_clock::now();

        cout << "All permutations:\n";
        for (const auto &perm : permutations)
        {
            cout << "[";
            for (int i = 0; i < perm.size(); i++)
            {
                cout << perm[i];
                if (i < perm.size() - 1)
                    cout << ", ";
            }
            cout << "]\n";
        }

        cout << "\nTotal permutations: " << permutations.size() << "\n";
        cout << "Generated in "
             << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
             << " milliseconds.\n";

        // Example 2: Comparing approaches
        vector<int> nums2 = {1, 2, 3, 4};

        cout << "\nExample 2: Comparing two permutation generation approaches for {1, 2, 3, 4}\n\n";

        // Swap-based approach
        startTime = high_resolution_clock::now();
        vector<vector<int>> perms1 = generate(nums2);
        endTime = high_resolution_clock::now();
        auto time1 = duration_cast<microseconds>(endTime - startTime);

        // "Used" array approach
        startTime = high_resolution_clock::now();
        vector<vector<int>> perms2 = generateAlternative(nums2);
        endTime = high_resolution_clock::now();
        auto time2 = duration_cast<microseconds>(endTime - startTime);

        assert(perms1.size() == perms2.size()); // Verify both give same number of results

        cout << "Swap-based approach: " << time1.count() / 1000.0 << " milliseconds\n";
        cout << "\"Used\" array approach: " << time2.count() / 1000.0 << " milliseconds\n";

        if (time1.count() < time2.count())
        {
            cout << "Swap-based approach is " << fixed << setprecision(2)
                 << static_cast<double>(time2.count()) / time1.count() << "× faster\n";
        }
        else
        {
            cout << "\"Used\" array approach is " << fixed << setprecision(2)
                 << static_cast<double>(time1.count()) / time2.count() << "× faster\n";
        }

        // Real-world application
        cout << "\nReal-world Application: Combinatorial Optimization\n";
        cout << "Permutation generation is essential in many optimization problems:\n";
        cout << "- Traveling Salesman Problem (finding the optimal route)\n";
        cout << "- Job Scheduling (determining optimal task order)\n";
        cout << "- DNA Sequence Analysis (exploring possible arrangements)\n";
        cout << "- Cryptography (permutation-based encryption algorithms)\n";
    }
} // namespace Permutations

/**
 * @namespace Combinations
 * @brief Implementation of the Combinations problem
 *
 * Generate all possible k-sized combinations of n elements.
 */
namespace Combinations
{
    /**
     * @brief Generate all k-sized combinations of integers from 1 to n
     * @param n The upper bound of integers
     * @param k The size of each combination
     * @return Vector of all combinations
     * @time_complexity O(n choose k), which is O(n! / (k! * (n-k)!))
     * @space_complexity O(k) for the recursion stack and current combination
     */
    void generateUtil(int n, int k, int start, vector<int> &current, vector<vector<int>> &result)
    {
        // Base case: Combination is complete
        if (current.size() == k)
        {
            result.push_back(current);
            return;
        }

        // Try remaining elements
        for (int i = start; i <= n; i++)
        {
            // Include current element
            current.push_back(i);

            // Recur for next elements
            generateUtil(n, k, i + 1, current, result);

            // Backtrack
            current.pop_back();
        }
    }

    /**
     * @brief Generate all k-sized combinations of integers from 1 to n
     * @param n The upper bound of integers
     * @param k The size of each combination
     * @return Vector of all combinations
     */
    vector<vector<int>> generate(int n, int k)
    {
        vector<vector<int>> result;
        vector<int> current;

        generateUtil(n, k, 1, current, result);

        return result;
    }

    /**
     * @brief Optimized combination generation with pruning
     * @param n The upper bound of integers
     * @param k The size of each combination
     * @return Vector of all combinations
     */
    vector<vector<int>> generateOptimized(int n, int k)
    {
        vector<vector<int>> result;
        vector<int> current;

        function<void(int)> backtrack = [&](int start)
        {
            // Base case: Combination is complete
            if (current.size() == k)
            {
                result.push_back(current);
                return;
            }

            // Pruning: If we can't form a combination of size k with remaining elements, backtrack
            if (current.size() + (n - start + 1) < k)
            {
                return;
            }

            // Try remaining elements
            for (int i = start; i <= n; i++)
            {
                // Include current element
                current.push_back(i);

                // Recur for next elements
                backtrack(i + 1);

                // Backtrack
                current.pop_back();
            }
        };

        backtrack(1);

        return result;
    }

    /**
     * @brief Demonstrate the Combinations algorithm
     */
    void runDemo()
    {
        printHeader("COMBINATIONS GENERATOR");

        cout << "Generate all possible k-sized combinations of n elements.\n\n";

        // Example 1: Small example
        int n = 4, k = 2;

        cout << "Example 1: Generating " << k << "-sized combinations of integers from 1 to " << n << "\n\n";

        auto startTime = high_resolution_clock::now();
        vector<vector<int>> combinations = generate(n, k);
        auto endTime = high_resolution_clock::now();

        cout << "All combinations:\n";
        for (const auto &combo : combinations)
        {
            cout << "[";
            for (int i = 0; i < combo.size(); i++)
            {
                cout << combo[i];
                if (i < combo.size() - 1)
                    cout << ", ";
            }
            cout << "]\n";
        }

        cout << "\nTotal combinations: " << combinations.size() << "\n";
        cout << "Generated in "
             << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
             << " milliseconds.\n";

        // Example 2: Comparing approaches
        n = 15;
        k = 4;

        cout << "\nExample 2: Comparing two combination generation approaches for n=" << n << ", k=" << k << "\n\n";

        // Basic approach
        startTime = high_resolution_clock::now();
        vector<vector<int>> combos1 = generate(n, k);
        endTime = high_resolution_clock::now();
        auto time1 = duration_cast<microseconds>(endTime - startTime);

        // Optimized approach with pruning
        startTime = high_resolution_clock::now();
        vector<vector<int>> combos2 = generateOptimized(n, k);
        endTime = high_resolution_clock::now();
        auto time2 = duration_cast<microseconds>(endTime - startTime);

        assert(combos1.size() == combos2.size()); // Verify both give same number of results

        cout << "Basic approach: " << time1.count() / 1000.0 << " milliseconds\n";
        cout << "Optimized approach: " << time2.count() / 1000.0 << " milliseconds\n";

        if (time1.count() < time2.count())
        {
            cout << "Basic approach is " << fixed << setprecision(2)
                 << static_cast<double>(time2.count()) / time1.count() << "× faster\n";
        }
        else
        {
            cout << "Optimized approach is " << fixed << setprecision(2)
                 << static_cast<double>(time1.count()) / time2.count() << "× faster\n";
        }

        // Real-world application
        cout << "\nReal-world Application: Selection Problems\n";
        cout << "Combination generation is used in many selection problems:\n";
        cout << "- Team formation (selecting k people from a group of n)\n";
        cout << "- Portfolio optimization (selecting k assets from n possibilities)\n";
        cout << "- Feature selection in machine learning (choosing k features from n)\n";
        cout << "- Test case design (selecting k test configurations from n parameters)\n";
    }
} // namespace Combinations

/**
 * @namespace WordSearch
 * @brief Implementation of the Word Search problem
 *
 * Given a 2D board and a word, find if the word exists in the grid.
 */
namespace WordSearch
{
    /**
     * @brief Check if word exists in the board
     * @param board 2D grid of characters
     * @param word Word to search for
     * @return True if word exists, false otherwise
     * @time_complexity O(m*n*4^L), where m and n are dimensions of the board and L is word length
     * @space_complexity O(L) for the recursion stack
     */
    bool existUtil(vector<vector<char>> &board, const string &word, int index, int i, int j, vector<vector<bool>> &visited)
    {
        // Base case: All characters found
        if (index == word.length())
        {
            return true;
        }

        // Check boundaries and if current cell matches current character
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() ||
            visited[i][j] || board[i][j] != word[index])
        {
            return false;
        }

        // Mark cell as visited
        visited[i][j] = true;

        // Explore all four directions
        bool found = existUtil(board, word, index + 1, i + 1, j, visited) || // Down
                     existUtil(board, word, index + 1, i - 1, j, visited) || // Up
                     existUtil(board, word, index + 1, i, j + 1, visited) || // Right
                     existUtil(board, word, index + 1, i, j - 1, visited);   // Left

        // Backtrack: mark cell as unvisited
        visited[i][j] = false;

        return found;
    }

    /**
     * @brief Check if word exists in the board
     * @param board 2D grid of characters
     * @param word Word to search for
     * @return True if word exists, false otherwise
     */
    bool exist(vector<vector<char>> &board, string word)
    {
        if (board.empty() || board[0].empty())
        {
            return false;
        }

        int m = board.size();
        int n = board[0].size();

        // Try starting from each cell
        vector<vector<bool>> visited(m, vector<bool>(n, false));

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (existUtil(board, word, 0, i, j, visited))
                {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * @brief Find and highlight the path of the word in the board
     * @param board 2D grid of characters
     * @param word Word to search for
     * @return Pair of bool (whether word exists) and 2D vector of int (path indices)
     */
    pair<bool, vector<pair<int, int>>> findWordPath(vector<vector<char>> &board, string word)
    {
        if (board.empty() || board[0].empty())
        {
            return {false, {}};
        }

        int m = board.size();
        int n = board[0].size();
        vector<pair<int, int>> path;

        function<bool(int, int, int)> backtrack = [&](int i, int j, int index)
        {
            // Base case: All characters found
            if (index == word.length())
            {
                return true;
            }

            // Check boundaries and if current cell matches current character
            if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] != word[index])
            {
                return false;
            }

            // Save original character and mark as visited
            char original = board[i][j];
            board[i][j] = '#';

            // Add to path
            path.push_back({i, j});

            // Explore all four directions
            bool found = backtrack(i + 1, j, index + 1) || // Down
                         backtrack(i - 1, j, index + 1) || // Up
                         backtrack(i, j + 1, index + 1) || // Right
                         backtrack(i, j - 1, index + 1);   // Left

            if (!found)
            {
                // Backtrack: remove from path
                path.pop_back();
            }

            // Restore original character
            board[i][j] = original;

            return found;
        };

        // Try starting from each cell
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (backtrack(i, j, 0))
                {
                    return {true, path};
                }
            }
        }

        return {false, {}};
    }

    /**
     * @brief Demonstrate the Word Search algorithm
     */
    void runDemo()
    {
        printHeader("WORD SEARCH");

        cout << "Given a 2D board and a word, find if the word exists in the grid.\n";
        cout << "The word can be constructed from letters of sequentially adjacent cells,\n";
        cout << "where \"adjacent\" cells are those horizontally or vertically neighboring.\n\n";

        // Example board
        vector<vector<char>> board = {
            {'A', 'B', 'C', 'E'},
            {'S', 'F', 'C', 'S'},
            {'A', 'D', 'E', 'E'}};

        cout << "Board:\n";
        for (const auto &row : board)
        {
            for (char c : row)
            {
                cout << c << " ";
            }
            cout << endl;
        }

        // Example words
        vector<string> words = {"ABCCED", "SEE", "ABCB"};

        cout << "\nSearching for words:\n";
        for (const auto &word : words)
        {
            auto startTime = high_resolution_clock::now();
            auto [found, path] = findWordPath(board, word);
            auto endTime = high_resolution_clock::now();

            cout << "Word \"" << word << "\": ";
            if (found)
            {
                cout << "Found! Path: ";
                for (int i = 0; i < path.size(); i++)
                {
                    cout << "(" << path[i].first << "," << path[i].second << ")";
                    if (i < path.size() - 1)
                        cout << " -> ";
                }
                cout << endl;
            }
            else
            {
                cout << "Not found." << endl;
            }

            cout << "Search time: "
                 << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
                 << " milliseconds.\n\n";
        }

        // Real-world application
        cout << "Real-world Application: Pattern Recognition\n";
        cout << "Word search is similar to pattern recognition problems in:\n";
        cout << "- Image processing (finding specific patterns in images)\n";
        cout << "- DNA sequence analysis (finding gene patterns)\n";
        cout << "- Network topology analysis (finding specific connectivity patterns)\n";
        cout << "- Game AI (pathfinding and pattern recognition)\n";
    }
} // namespace WordSearch

/**
 * @namespace GraphColoring
 * @brief Implementation of the Graph Coloring problem
 *
 * Assign colors to vertices of a graph such that no two adjacent
 * vertices have the same color.
 */
namespace GraphColoring
{
    /**
     * @brief Check if it's safe to color vertex v with color c
     * @param graph Adjacency matrix of the graph
     * @param v Vertex to color
     * @param c Color to assign
     * @param colors Current coloring of vertices
     * @return True if safe, false otherwise
     */
    bool isSafe(const vector<vector<int>> &graph, int v, int c, const vector<int> &colors)
    {
        for (int i = 0; i < graph.size(); i++)
        {
            if (graph[v][i] && colors[i] == c)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Color vertices of a graph using backtracking
     * @param graph Adjacency matrix of the graph
     * @param m Maximum number of colors
     * @param v Current vertex
     * @param colors Array to store colors (1 to m) for each vertex
     * @return True if graph can be colored with at most m colors
     */
    bool graphColoringUtil(const vector<vector<int>> &graph, int m, int v, vector<int> &colors)
    {
        // Base case: All vertices colored
        if (v == graph.size())
        {
            return true;
        }

        // Try different colors for vertex v
        for (int c = 1; c <= m; c++)
        {
            // Check if color c is safe for vertex v
            if (isSafe(graph, v, c, colors))
            {
                // Assign color c to vertex v
                colors[v] = c;

                // Recur to color rest of the graph
                if (graphColoringUtil(graph, m, v + 1, colors))
                {
                    return true;
                }

                // Backtrack: remove color if coloring not possible
                colors[v] = 0;
            }
        }

        // No color can be assigned to vertex v
        return false;
    }

    /**
     * @brief Color vertices of a graph using backtracking
     * @param graph Adjacency matrix of the graph
     * @param m Maximum number of colors
     * @param colors Array to store colors (1 to m) for each vertex
     * @return True if graph can be colored with at most m colors
     */
    bool colorGraph(vector<vector<int>> &graph, int m, vector<int> &colors)
    {
        // Initialize all vertices as uncolored
        colors.assign(graph.size(), 0);

        // Start coloring from vertex 0
        return graphColoringUtil(graph, m, 0, colors);
    }

    /**
     * @brief Demonstrate the Graph Coloring algorithm
     */
    void runDemo()
    {
        printHeader("GRAPH COLORING");

        cout << "Assign colors to vertices of a graph such that no two adjacent\n";
        cout << "vertices have the same color, using at most m colors.\n\n";

        // Example graph (represented as adjacency matrix)
        vector<vector<int>> graph = {
            {0, 1, 1, 1},
            {1, 0, 1, 0},
            {1, 1, 0, 1},
            {1, 0, 1, 0}};

        int vertices = graph.size();

        cout << "Graph with " << vertices << " vertices:\n";
        cout << "Adjacency matrix:\n";
        for (const auto &row : graph)
        {
            for (int val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }

        // Try coloring with different numbers of colors
        for (int m = 2; m <= 4; m++)
        {
            vector<int> colors;

            auto startTime = high_resolution_clock::now();
            bool possible = colorGraph(graph, m, colors);
            auto endTime = high_resolution_clock::now();

            cout << "\nColoring with " << m << " colors: ";
            if (possible)
            {
                cout << "Possible!\n";
                cout << "Vertex colors: ";
                for (int i = 0; i < vertices; i++)
                {
                    cout << "Vertex " << i << ": Color " << colors[i] << "  ";
                }
                cout << endl;

                // Verify coloring is valid
                bool valid = true;
                for (int i = 0; i < vertices; i++)
                {
                    for (int j = i + 1; j < vertices; j++)
                    {
                        if (graph[i][j] && colors[i] == colors[j])
                        {
                            valid = false;
                            break;
                        }
                    }
                }

                cout << "Coloring is " << (valid ? "valid" : "invalid") << ".\n";
            }
            else
            {
                cout << "Not possible.\n";
            }

            cout << "Execution time: "
                 << duration_cast<microseconds>(endTime - startTime).count() / 1000.0
                 << " milliseconds.\n";
        }

        // Real-world application
        cout << "\nReal-world Applications of Graph Coloring:\n";
        cout << "1. Map Coloring: Assigning colors to regions on a map such that no adjacent regions have the same color.\n";
        cout << "2. Scheduling: Assigning time slots for exams such that no two conflicting exams are scheduled at the same time.\n";
        cout << "3. Register Allocation: Assigning registers to variables in a compiler such that variables in use at the same time are assigned different registers.\n";
        cout << "4. Frequency Assignment: Assigning frequencies to radio stations such that stations with overlapping coverage areas use different frequencies.\n";
        cout << "5. Sudoku: The classic puzzle can be modeled as a graph coloring problem where cells are vertices and cells that can't have the same value are connected by edges.\n";
    }
} // namespace GraphColoring