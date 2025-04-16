// Day 17: Basic Sorting Algorithms #DSAin45
// Comprehensive implementations and comparisons of basic sorting algorithms

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <iomanip>
#include <cassert>
#include <limits>

// ===== UTILITY FUNCTIONS =====

// Helper function to measure execution time
template <typename Func, typename... Args>
auto measureExecutionTime(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return std::make_pair(result, duration.count());
}

// Overload for void functions
template <typename Func, typename... Args>
auto measureExecutionTimeVoid(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return duration.count();
}

// Generate a random vector of the specified size
std::vector<int> generateRandomVector(int size, int min = 0, int max = 1000)
{
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (int i = 0; i < size; i++)
    {
        vec[i] = dis(gen);
    }

    return vec;
}

// Generate an almost sorted vector (sorted with few elements out of place)
std::vector<int> generateAlmostSortedVector(int size, int swaps = 5, int min = 0, int max = 1000)
{
    // Create a sorted vector
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
    {
        vec[i] = min + i * ((max - min) / size);
    }

    // Perform a few random swaps
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size - 1);

    for (int i = 0; i < swaps; i++)
    {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        std::swap(vec[idx1], vec[idx2]);
    }

    return vec;
}

// Generate a reverse sorted vector
std::vector<int> generateReverseSortedVector(int size, int min = 0, int max = 1000)
{
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
    {
        vec[i] = max - i * ((max - min) / size);
    }
    return vec;
}

// Print the vector
void printVector(const std::vector<int> &vec, std::string name = "Vector", int maxDisplay = 20)
{
    std::cout << name << " (size=" << vec.size() << "): [";
    int showElements = std::min(maxDisplay, static_cast<int>(vec.size()));

    for (int i = 0; i < showElements; i++)
    {
        std::cout << vec[i];
        if (i < showElements - 1)
            std::cout << ", ";
    }

    if (vec.size() > maxDisplay)
    {
        std::cout << ", ... (" << vec.size() - maxDisplay << " more)";
    }

    std::cout << "]" << std::endl;
}

// Check if a vector is sorted
bool isSorted(const std::vector<int> &vec)
{
    for (size_t i = 1; i < vec.size(); i++)
    {
        if (vec[i - 1] > vec[i])
        {
            return false;
        }
    }
    return true;
}

// Clone a vector
std::vector<int> cloneVector(const std::vector<int> &vec)
{
    return std::vector<int>(vec);
}

// Count inversions in a vector (measure of how sorted it is)
int countInversions(const std::vector<int> &vec)
{
    int inversions = 0;
    for (size_t i = 0; i < vec.size(); i++)
    {
        for (size_t j = i + 1; j < vec.size(); j++)
        {
            if (vec[i] > vec[j])
            {
                inversions++;
            }
        }
    }
    return inversions;
}

// Track the number of comparisons and swaps
struct SortStatistics
{
    size_t comparisons = 0;
    size_t swaps = 0;
};

// ===== BUBBLE SORT IMPLEMENTATIONS =====

// Basic Bubble Sort
void bubbleSortBasic(std::vector<int> &arr)
{
    int n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Optimized Bubble Sort (with early termination if sorted)
void bubbleSort(std::vector<int> &arr)
{
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;

        // Last i elements are already sorted, so we don't need to check them
        for (int j = 0; j < n - i - 1; j++)
        {
            // If the current element is greater than the next element, swap them
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no swapping occurred in this pass, array is sorted
        if (!swapped)
        {
            break;
        }
    }
}

// Bubble sort with statistics
void bubbleSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++)
        {
            stats.comparisons++;
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

// Recursive bubble sort
void recursiveBubbleSort(std::vector<int> &arr, int n)
{
    // Base case
    if (n <= 1)
        return;

    bool swapped = false;

    // One pass of bubble sort
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            std::swap(arr[i], arr[i + 1]);
            swapped = true;
        }
    }

    // If no swapping occurred, array is sorted
    if (!swapped)
        return;

    // Largest element is now at the end, recur for remaining array
    recursiveBubbleSort(arr, n - 1);
}

// Wrapper for recursive bubble sort
void recursiveBubbleSort(std::vector<int> &arr)
{
    recursiveBubbleSort(arr, arr.size());
}

// Cocktail Shaker Sort (Bidirectional Bubble Sort)
void cocktailShakerSort(std::vector<int> &arr)
{
    bool swapped = true;
    int start = 0;
    int end = arr.size() - 1;

    while (swapped)
    {
        // Reset swapped flag for forward pass
        swapped = false;

        // Forward pass (like bubble sort)
        for (int i = start; i < end; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        // If nothing moved, array is sorted
        if (!swapped)
            break;

        // Reset swapped flag for backward pass
        swapped = false;

        // Decrement end as largest element is now at end
        end--;

        // Backward pass (bubble smallest element to the front)
        for (int i = end - 1; i >= start; i--)
        {
            if (arr[i] > arr[i + 1])
            {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        // Increment start as smallest element is now at start
        start++;
    }
}

// ===== SELECTION SORT IMPLEMENTATIONS =====

// Basic Selection Sort
void selectionSort(std::vector<int> &arr)
{
    int n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {
        // Find the minimum element in the unsorted part
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first element of the unsorted part
        if (min_idx != i)
        {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

// Selection sort with statistics
void selectionSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    int n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
        {
            stats.comparisons++;
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }

        if (min_idx != i)
        {
            std::swap(arr[i], arr[min_idx]);
            stats.swaps++;
        }
    }
}

// Recursive selection sort
void recursiveSelectionSort(std::vector<int> &arr, int start_idx, int n)
{
    // Base case
    if (start_idx >= n - 1)
        return;

    // Find minimum element in the unsorted part
    int min_idx = start_idx;
    for (int j = start_idx + 1; j < n; j++)
    {
        if (arr[j] < arr[min_idx])
        {
            min_idx = j;
        }
    }

    // Swap if needed
    if (min_idx != start_idx)
    {
        std::swap(arr[start_idx], arr[min_idx]);
    }

    // Recur for the next element
    recursiveSelectionSort(arr, start_idx + 1, n);
}

// Wrapper for recursive selection sort
void recursiveSelectionSort(std::vector<int> &arr)
{
    recursiveSelectionSort(arr, 0, arr.size());
}

// Bidirectional selection sort
void bidirectionalSelectionSort(std::vector<int> &arr)
{
    int left = 0;
    int right = arr.size() - 1;

    while (left < right)
    {
        // Find minimum in the range [left, right]
        int min_idx = left;
        for (int i = left + 1; i <= right; i++)
        {
            if (arr[i] < arr[min_idx])
            {
                min_idx = i;
            }
        }

        // Swap minimum with left element if needed
        if (min_idx != left)
        {
            std::swap(arr[left], arr[min_idx]);
        }

        // Find maximum in the range [left+1, right]
        int max_idx = right;
        for (int i = right - 1; i >= left + 1; i--)
        {
            if (arr[i] > arr[max_idx])
            {
                max_idx = i;
            }
        }

        // Swap maximum with right element if needed
        if (max_idx != right)
        {
            std::swap(arr[max_idx], arr[right]);
        }

        left++;
        right--;
    }
}

// ===== INSERTION SORT IMPLEMENTATIONS =====

// Basic Insertion Sort
void insertionSort(std::vector<int> &arr)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int key = arr[i]; // Current element to be inserted
        int j = i - 1;    // Index of the last element in the sorted part

        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        // Insert the key at its correct position
        arr[j + 1] = key;
    }
}

// Insertion sort with statistics
void insertionSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            stats.comparisons++;
            if (arr[j] > key)
            {
                arr[j + 1] = arr[j];
                stats.swaps++;
                j--;
            }
            else
            {
                break;
            }
        }

        if (j + 1 != i)
        {
            arr[j + 1] = key;
            stats.swaps++; // Count the insertion as a swap
        }
    }
}

// Recursive insertion sort
void recursiveInsertionSort(std::vector<int> &arr, int n)
{
    // Base case
    if (n <= 1)
        return;

    // Recursively sort first n-1 elements
    recursiveInsertionSort(arr, n - 1);

    // Insert the last element into its correct position
    int key = arr[n - 1];
    int j = n - 2;

    while (j >= 0 && arr[j] > key)
    {
        arr[j + 1] = arr[j];
        j--;
    }

    arr[j + 1] = key;
}

// Wrapper for recursive insertion sort
void recursiveInsertionSort(std::vector<int> &arr)
{
    recursiveInsertionSort(arr, arr.size());
}

// Binary Insertion Sort
void binaryInsertionSort(std::vector<int> &arr)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int left = 0;
        int right = i - 1;

        // Binary search to find insertion point
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (arr[mid] > key)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        // Shift elements to make room for key
        for (int j = i - 1; j >= left; j--)
        {
            arr[j + 1] = arr[j];
        }

        // Insert the key at the correct position
        arr[left] = key;
    }
}

// Binary insertion sort with statistics
void binaryInsertionSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int left = 0;
        int right = i - 1;

        // Binary search to find insertion point
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            stats.comparisons++;
            if (arr[mid] > key)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        // Shift elements to make room for key
        for (int j = i - 1; j >= left; j--)
        {
            arr[j + 1] = arr[j];
            stats.swaps++;
        }

        // Insert the key at the correct position
        if (left != i)
        {
            arr[left] = key;
            stats.swaps++;
        }
    }
}

// Shell Sort (an extension of insertion sort)
void shellSort(std::vector<int> &arr)
{
    int n = arr.size();

    // Start with a large gap, then reduce it
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        // Perform insertion sort for each gap size
        for (int i = gap; i < n; i++)
        {
            int temp = arr[i];
            int j;

            // Shift elements that are gap positions apart
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
            {
                arr[j] = arr[j - gap];
            }

            // Put temp in its correct location
            arr[j] = temp;
        }
    }
}

// ===== TEST AND VISUALIZATION FUNCTIONS =====

// Test and compare bubble sort variations
void testBubbleSort()
{
    std::cout << "\n===== BUBBLE SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(1000, 1, 10000);
    std::vector<int> almostSorted = generateAlmostSortedVector(1000, 10);
    std::vector<int> reverseSorted = generateReverseSortedVector(1000);

    // Clone arrays for different algorithms
    auto randomSmall1 = cloneVector(randomSmall);
    auto randomSmall2 = cloneVector(randomSmall);
    auto randomMedium1 = cloneVector(randomMedium);
    auto randomMedium2 = cloneVector(randomMedium);
    auto almostSorted1 = cloneVector(almostSorted);
    auto almostSorted2 = cloneVector(almostSorted);
    auto reverseSorted1 = cloneVector(reverseSorted);
    auto reverseSorted2 = cloneVector(reverseSorted);

    // Calculate inversions for each array
    std::cout << "Inversions count (measure of disorder):" << std::endl;
    std::cout << "  Random Small: " << countInversions(randomSmall) << std::endl;
    std::cout << "  Random Medium: " << countInversions(randomMedium) << std::endl;
    std::cout << "  Almost Sorted: " << countInversions(almostSorted) << std::endl;
    std::cout << "  Reverse Sorted: " << countInversions(reverseSorted) << std::endl;

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(20) << "Random Small"
              << std::setw(20) << "Random Medium"
              << std::setw(20) << "Almost Sorted"
              << std::setw(20) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(105, '-') << std::endl;

    // Basic Bubble Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(bubbleSortBasic, copy1);
        auto time2 = measureExecutionTimeVoid(bubbleSortBasic, copy2);
        auto time3 = measureExecutionTimeVoid(bubbleSortBasic, copy3);
        auto time4 = measureExecutionTimeVoid(bubbleSortBasic, copy4);

        std::cout << std::left
                  << std::setw(25) << "Basic Bubble Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Optimized Bubble Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(bubbleSort, copy1);
        auto time2 = measureExecutionTimeVoid(bubbleSort, copy2);
        auto time3 = measureExecutionTimeVoid(bubbleSort, copy3);
        auto time4 = measureExecutionTimeVoid(bubbleSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Optimized Bubble Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Recursive Bubble Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(recursiveBubbleSort, copy1);
        auto time2 = measureExecutionTimeVoid(recursiveBubbleSort, copy2);
        auto time3 = measureExecutionTimeVoid(recursiveBubbleSort, copy3);
        auto time4 = measureExecutionTimeVoid(recursiveBubbleSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Recursive Bubble Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Cocktail Shaker Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(cocktailShakerSort, copy1);
        auto time2 = measureExecutionTimeVoid(cocktailShakerSort, copy2);
        auto time3 = measureExecutionTimeVoid(cocktailShakerSort, copy3);
        auto time4 = measureExecutionTimeVoid(cocktailShakerSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Cocktail Shaker Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Count operations for bubble sort variants
    std::cout << "\nOperations count on random array (size=1000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(20) << "Comparisons"
              << std::setw(20) << "Swaps"
              << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        bubbleSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Bubble Sort"
                  << std::setw(20) << stats.comparisons
                  << std::setw(20) << stats.swaps
                  << std::endl;
    }

    // Step-by-step visualization of bubble sort
    std::cout << "\nStep-by-step bubble sort visualization:" << std::endl;
    std::vector<int> demo = {5, 3, 8, 4, 2};
    std::cout << "Initial array: ";
    printVector(demo, "", demo.size());

    for (int i = 0; i < demo.size() - 1; i++)
    {
        bool swapped = false;
        std::cout << "\nPass " << (i + 1) << ":" << std::endl;

        for (int j = 0; j < demo.size() - i - 1; j++)
        {
            std::cout << "  Compare " << demo[j] << " and " << demo[j + 1];

            if (demo[j] > demo[j + 1])
            {
                std::swap(demo[j], demo[j + 1]);
                swapped = true;
                std::cout << " -> Swap -> ";
            }
            else
            {
                std::cout << " -> No swap -> ";
            }

            for (int k = 0; k < demo.size(); k++)
            {
                if (k == j + 1)
                {
                    std::cout << "[" << demo[k] << "] ";
                }
                else
                {
                    std::cout << demo[k] << " ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << "  End of pass " << (i + 1) << ": ";
        printVector(demo, "", demo.size());

        if (!swapped)
        {
            std::cout << "  No swaps in this pass, array is sorted!" << std::endl;
            break;
        }
    }
}

// Test and compare selection sort variations
void testSelectionSort()
{
    std::cout << "\n===== SELECTION SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(1000, 1, 10000);
    std::vector<int> almostSorted = generateAlmostSortedVector(1000, 10);
    std::vector<int> reverseSorted = generateReverseSortedVector(1000);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(30) << "Algorithm"
              << std::setw(20) << "Random Small"
              << std::setw(20) << "Random Medium"
              << std::setw(20) << "Almost Sorted"
              << std::setw(20) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(110, '-') << std::endl;

    // Basic Selection Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(selectionSort, copy1);
        auto time2 = measureExecutionTimeVoid(selectionSort, copy2);
        auto time3 = measureExecutionTimeVoid(selectionSort, copy3);
        auto time4 = measureExecutionTimeVoid(selectionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Selection Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Recursive Selection Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(recursiveSelectionSort, copy1);
        auto time2 = measureExecutionTimeVoid(recursiveSelectionSort, copy2);
        auto time3 = measureExecutionTimeVoid(recursiveSelectionSort, copy3);
        auto time4 = measureExecutionTimeVoid(recursiveSelectionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Recursive Selection Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Bidirectional Selection Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(bidirectionalSelectionSort, copy1);
        auto time2 = measureExecutionTimeVoid(bidirectionalSelectionSort, copy2);
        auto time3 = measureExecutionTimeVoid(bidirectionalSelectionSort, copy3);
        auto time4 = measureExecutionTimeVoid(bidirectionalSelectionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Bidirectional Selection Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Count operations for selection sort
    std::cout << "\nOperations count on random array (size=1000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(20) << "Comparisons"
              << std::setw(20) << "Swaps"
              << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        selectionSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Selection Sort"
                  << std::setw(20) << stats.comparisons
                  << std::setw(20) << stats.swaps
                  << std::endl;
    }

    // Step-by-step visualization of selection sort
    std::cout << "\nStep-by-step selection sort visualization:" << std::endl;
    std::vector<int> demo = {5, 3, 8, 4, 2};
    std::cout << "Initial array: ";
    printVector(demo, "", demo.size());

    for (int i = 0; i < demo.size() - 1; i++)
    {
        std::cout << "\nPass " << (i + 1) << ":" << std::endl;

        // Find minimum element
        int min_idx = i;
        std::cout << "  Finding minimum in positions " << i << " to " << (demo.size() - 1) << ":" << std::endl;

        for (int j = i + 1; j < demo.size(); j++)
        {
            std::cout << "  Compare " << demo[min_idx] << " and " << demo[j];

            if (demo[j] < demo[min_idx])
            {
                min_idx = j;
                std::cout << " -> New minimum at position " << j << ": " << demo[j];
            }

            std::cout << std::endl;
        }

        // Swap if needed
        if (min_idx != i)
        {
            std::cout << "  Swap positions " << i << " and " << min_idx << ": " << demo[i] << " <-> " << demo[min_idx] << std::endl;
            std::swap(demo[i], demo[min_idx]);
        }
        else
        {
            std::cout << "  No swap needed, minimum is already at position " << i << std::endl;
        }

        std::cout << "  End of pass " << (i + 1) << ": ";
        for (int k = 0; k < demo.size(); k++)
        {
            if (k <= i)
            {
                std::cout << "[" << demo[k] << "] ";
            }
            else
            {
                std::cout << demo[k] << " ";
            }
        }
        std::cout << std::endl;
        std::cout << "  Sorted portion: [";
        for (int k = 0; k <= i; k++)
        {
            std::cout << demo[k];
            if (k < i)
                std::cout << ", ";
        }
        std::cout << "], Unsorted portion: [";
        for (int k = i + 1; k < demo.size(); k++)
        {
            std::cout << demo[k];
            if (k < demo.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// Test and compare insertion sort variations
void testInsertionSort()
{
    std::cout << "\n===== INSERTION SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(1000, 1, 10000);
    std::vector<int> almostSorted = generateAlmostSortedVector(1000, 10);
    std::vector<int> reverseSorted = generateReverseSortedVector(1000);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(30) << "Algorithm"
              << std::setw(20) << "Random Small"
              << std::setw(20) << "Random Medium"
              << std::setw(20) << "Almost Sorted"
              << std::setw(20) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(110, '-') << std::endl;

    // Basic Insertion Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(insertionSort, copy1);
        auto time2 = measureExecutionTimeVoid(insertionSort, copy2);
        auto time3 = measureExecutionTimeVoid(insertionSort, copy3);
        auto time4 = measureExecutionTimeVoid(insertionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Insertion Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Recursive Insertion Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(recursiveInsertionSort, copy1);
        auto time2 = measureExecutionTimeVoid(recursiveInsertionSort, copy2);
        auto time3 = measureExecutionTimeVoid(recursiveInsertionSort, copy3);
        auto time4 = measureExecutionTimeVoid(recursiveInsertionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Recursive Insertion Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Binary Insertion Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(binaryInsertionSort, copy1);
        auto time2 = measureExecutionTimeVoid(binaryInsertionSort, copy2);
        auto time3 = measureExecutionTimeVoid(binaryInsertionSort, copy3);
        auto time4 = measureExecutionTimeVoid(binaryInsertionSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Binary Insertion Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Shell Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(shellSort, copy1);
        auto time2 = measureExecutionTimeVoid(shellSort, copy2);
        auto time3 = measureExecutionTimeVoid(shellSort, copy3);
        auto time4 = measureExecutionTimeVoid(shellSort, copy4);

        std::cout << std::left
                  << std::setw(30) << "Shell Sort"
                  << std::setw(20) << time1
                  << std::setw(20) << time2
                  << std::setw(20) << time3
                  << std::setw(20) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Count operations for insertion sort variants
    std::cout << "\nOperations count on random array (size=1000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(20) << "Comparisons"
              << std::setw(20) << "Swaps/Shifts"
              << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        insertionSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Insertion Sort"
                  << std::setw(20) << stats.comparisons
                  << std::setw(20) << stats.swaps
                  << std::endl;
    }

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        binaryInsertionSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Binary Insertion Sort"
                  << std::setw(20) << stats.comparisons
                  << std::setw(20) << stats.swaps
                  << std::endl;
    }

    // Step-by-step visualization of insertion sort
    std::cout << "\nStep-by-step insertion sort visualization:" << std::endl;
    std::vector<int> demo = {5, 3, 8, 4, 2};
    std::cout << "Initial array: ";
    printVector(demo, "", demo.size());

    std::cout << "Consider first element as sorted: [5]" << std::endl;

    for (int i = 1; i < demo.size(); i++)
    {
        std::cout << "\nStep " << i << ": Insert " << demo[i] << " into sorted portion:" << std::endl;

        int key = demo[i];
        int j = i - 1;

        std::cout << "  Current sorted portion: [";
        for (int k = 0; k <= j; k++)
        {
            std::cout << demo[k];
            if (k < j)
                std::cout << ", ";
        }
        std::cout << "], Key = " << key << std::endl;

        // Move elements greater than key one position ahead
        while (j >= 0 && demo[j] > key)
        {
            std::cout << "  " << demo[j] << " > " << key << ", shift " << demo[j] << " to position " << (j + 1) << std::endl;
            demo[j + 1] = demo[j];
            j--;
        }

        // Insert the key at its correct position
        demo[j + 1] = key;
        std::cout << "  Insert " << key << " at position " << (j + 1) << std::endl;

        std::cout << "  Result after inserting " << key << ": [";
        for (int k = 0; k <= i; k++)
        {
            std::cout << demo[k];
            if (k < i)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        std::cout << "  Array state: ";
        for (int k = 0; k < demo.size(); k++)
        {
            if (k <= i)
            {
                std::cout << "[" << demo[k] << "] ";
            }
            else
            {
                std::cout << demo[k] << " ";
            }
        }
        std::cout << std::endl;

        std::cout << "  Sorted portion: [";
        for (int k = 0; k <= i; k++)
        {
            std::cout << demo[k];
            if (k < i)
                std::cout << ", ";
        }
        std::cout << "], Unsorted portion: [";
        for (int k = i + 1; k < demo.size(); k++)
        {
            std::cout << demo[k];
            if (k < demo.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// Compare all sorting algorithms together
void compareAllSortingAlgorithms()
{
    std::cout << "\n===== COMPARING ALL BASIC SORTING ALGORITHMS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> small = generateRandomVector(100, 1, 1000);
    std::vector<int> medium = generateRandomVector(1000, 1, 10000);
    std::vector<int> large = generateRandomVector(5000, 1, 10000);
    std::vector<int> almostSorted = generateAlmostSortedVector(1000, 10);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(30) << "Algorithm"
              << std::setw(15) << "Small (100)"
              << std::setw(15) << "Medium (1K)"
              << std::setw(15) << "Large (5K)"
              << std::setw(20) << "Almost Sorted (1K)"
              << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    // Bubble Sort
    {
        auto time1 = measureExecutionTimeVoid(bubbleSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(bubbleSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(bubbleSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(bubbleSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Bubble Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Cocktail Shaker Sort
    {
        auto time1 = measureExecutionTimeVoid(cocktailShakerSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(cocktailShakerSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(cocktailShakerSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(cocktailShakerSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Cocktail Shaker Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Selection Sort
    {
        auto time1 = measureExecutionTimeVoid(selectionSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(selectionSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(selectionSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(selectionSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Selection Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Insertion Sort
    {
        auto time1 = measureExecutionTimeVoid(insertionSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(insertionSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(insertionSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(insertionSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Insertion Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Binary Insertion Sort
    {
        auto time1 = measureExecutionTimeVoid(binaryInsertionSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(binaryInsertionSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(binaryInsertionSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(binaryInsertionSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Binary Insertion Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Shell Sort
    {
        auto time1 = measureExecutionTimeVoid(shellSort, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(shellSort, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(shellSort, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(shellSort, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "Shell Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // STL's std::sort (for comparison)
    {
        auto sortFunc = [](std::vector<int> &arr)
        { std::sort(arr.begin(), arr.end()); };
        auto time1 = measureExecutionTimeVoid(sortFunc, cloneVector(small));
        auto time2 = measureExecutionTimeVoid(sortFunc, cloneVector(medium));
        auto time3 = measureExecutionTimeVoid(sortFunc, cloneVector(large));
        auto time4 = measureExecutionTimeVoid(sortFunc, cloneVector(almostSorted));

        std::cout << std::left
                  << std::setw(30) << "std::sort (for comparison)"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(20) << time4
                  << std::endl;
    }

    // Stability test
    std::cout << "\nStability test (checking if equal elements maintain their relative order):" << std::endl;

    // Create a test case for stability
    struct Item
    {
        int value;
        int originalIndex;

        bool operator<(const Item &other) const
        {
            return value < other.value;
        }

        bool operator>(const Item &other) const
        {
            return value > other.value;
        }
    };

    // Initialize test data with duplicate values
    std::vector<Item> testStability;
    testStability.push_back({5, 0});
    testStability.push_back({3, 1});
    testStability.push_back({5, 2});
    testStability.push_back({8, 3});
    testStability.push_back({3, 4});
    testStability.push_back({2, 5});

    std::cout << "Test array with duplicate values:" << std::endl;
    for (const auto &item : testStability)
    {
        std::cout << "(" << item.value << ", " << item.originalIndex << ") ";
    }
    std::cout << std::endl;

    // Custom bubble sort for Items
    auto bubbleSortItems = [](std::vector<Item> &arr)
    {
        int n = arr.size();
        bool swapped;

        for (int i = 0; i < n - 1; i++)
        {
            swapped = false;

            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j].value > arr[j + 1].value)
                {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }

            if (!swapped)
            {
                break;
            }
        }
    };

    // Custom selection sort for Items
    auto selectionSortItems = [](std::vector<Item> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            int min_idx = i;
            for (int j = i + 1; j < n; j++)
            {
                if (arr[j].value < arr[min_idx].value)
                {
                    min_idx = j;
                }
            }

            if (min_idx != i)
            {
                std::swap(arr[i], arr[min_idx]);
            }
        }
    };

    // Custom insertion sort for Items
    auto insertionSortItems = [](std::vector<Item> &arr)
    {
        int n = arr.size();

        for (int i = 1; i < n; i++)
        {
            Item key = arr[i];
            int j = i - 1;

            while (j >= 0 && arr[j].value > key.value)
            {
                arr[j + 1] = arr[j];
                j--;
            }

            arr[j + 1] = key;
        }
    };

    // Test bubble sort stability
    {
        auto arrCopy = testStability;
        bubbleSortItems(arrCopy);

        std::cout << "Bubble sort result: ";
        for (const auto &item : arrCopy)
        {
            std::cout << "(" << item.value << ", " << item.originalIndex << ") ";
        }
        std::cout << " - " << (arrCopy[0].value == arrCopy[1].value && arrCopy[0].originalIndex < arrCopy[1].originalIndex && arrCopy[2].value == arrCopy[3].value && arrCopy[2].originalIndex < arrCopy[3].originalIndex ? "Stable" : "Not stable") << std::endl;
    }

    // Test selection sort stability
    {
        auto arrCopy = testStability;
        selectionSortItems(arrCopy);

        std::cout << "Selection sort result: ";
        for (const auto &item : arrCopy)
        {
            std::cout << "(" << item.value << ", " << item.originalIndex << ") ";
        }
        std::cout << " - " << (arrCopy[0].value == arrCopy[1].value && arrCopy[0].originalIndex < arrCopy[1].originalIndex && arrCopy[2].value == arrCopy[3].value && arrCopy[2].originalIndex < arrCopy[3].originalIndex ? "Stable" : "Not stable") << std::endl;
    }

    // Test insertion sort stability
    {
        auto arrCopy = testStability;
        insertionSortItems(arrCopy);

        std::cout << "Insertion sort result: ";
        for (const auto &item : arrCopy)
        {
            std::cout << "(" << item.value << ", " << item.originalIndex << ") ";
        }
        std::cout << " - " << (arrCopy[0].value == arrCopy[1].value && arrCopy[0].originalIndex < arrCopy[1].originalIndex && arrCopy[2].value == arrCopy[3].value && arrCopy[2].originalIndex < arrCopy[3].originalIndex ? "Stable" : "Not stable") << std::endl;
    }
}

// ===== PRACTICAL APPLICATION EXAMPLES =====

// Sort playing cards by rank (ignoring suit)
void cardSorting()
{
    std::cout << "\n===== PRACTICAL APPLICATION: CARD SORTING =====" << std::endl;

    struct Card
    {
        std::string rank;
        std::string suit;
        int rankValue; // Numerical value for sorting

        Card(std::string r, std::string s) : rank(r), suit(s)
        {
            // Assign numerical values
            if (r == "A")
                rankValue = 1;
            else if (r == "J")
                rankValue = 11;
            else if (r == "Q")
                rankValue = 12;
            else if (r == "K")
                rankValue = 13;
            else
                rankValue = std::stoi(r);
        }

        std::string toString() const
        {
            return rank + "-" + suit;
        }
    };

    // Create a deck of cards (simplified)
    std::vector<Card> hand;
    hand.push_back(Card("5", "H"));
    hand.push_back(Card("J", "C"));
    hand.push_back(Card("2", "S"));
    hand.push_back(Card("A", "D"));
    hand.push_back(Card("8", "H"));
    hand.push_back(Card("K", "S"));
    hand.push_back(Card("7", "D"));

    std::cout << "Initial hand: ";
    for (const auto &card : hand)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;

    // Sort using insertion sort (the way most people sort cards by hand)
    for (int i = 1; i < hand.size(); i++)
    {
        Card key = hand[i];
        int j = i - 1;

        // Move elements greater than key one position ahead
        while (j >= 0 && hand[j].rankValue > key.rankValue)
        {
            hand[j + 1] = hand[j];
            j--;
        }

        // Insert the key at its correct position
        hand[j + 1] = key;

        // Show the state after each card is inserted
        std::cout << "After inserting " << key.toString() << ": ";
        for (const auto &card : hand)
        {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nFinal sorted hand: ";
    for (const auto &card : hand)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;

    std::cout << "\nNotice how this matches the way most people sort cards in their hand!" << std::endl;
}

// Online sorting example (sorting elements as they arrive)
void onlineSorting()
{
    std::cout << "\n===== PRACTICAL APPLICATION: ONLINE SORTING =====" << std::endl;

    std::vector<int> stream;
    std::cout << "Simulating an online sorting scenario where elements arrive one by one:" << std::endl;

    // Simulate elements arriving one by one
    int newElements[] = {42, 17, 30, 5, 22, 10, 50, 3};

    for (int element : newElements)
    {
        std::cout << "\nNew element arrived: " << element << std::endl;

        // Insert the new element into the sorted position using insertion sort approach
        auto it = std::lower_bound(stream.begin(), stream.end(), element);
        stream.insert(it, element);

        std::cout << "Current sorted state: ";
        for (int num : stream)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nFinal sorted stream: ";
    for (int num : stream)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "\nThis demonstrates how insertion sort can be used for online sorting scenarios" << std::endl;
    std::cout << "where data arrives incrementally and needs to be kept sorted at all times." << std::endl;
}

// ===== LEETCODE-STYLE PROBLEMS =====

// Sort Array By Parity (LeetCode #905)
std::vector<int> sortArrayByParity(std::vector<int> &nums)
{
    int i = 0;
    int j = nums.size() - 1;

    while (i < j)
    {
        // Find an odd number from the left
        while (i < j && nums[i] % 2 == 0)
        {
            i++;
        }

        // Find an even number from the right
        while (i < j && nums[j] % 2 == 1)
        {
            j--;
        }

        // Swap them
        if (i < j)
        {
            std::swap(nums[i], nums[j]);
            i++;
            j--;
        }
    }

    return nums;
}

// Sort Colors (LeetCode #75) - Dutch National Flag problem
void sortColors(std::vector<int> &nums)
{
    int low = 0;                // For 0's
    int mid = 0;                // For 1's
    int high = nums.size() - 1; // For 2's

    while (mid <= high)
    {
        if (nums[mid] == 0)
        {
            std::swap(nums[low], nums[mid]);
            low++;
            mid++;
        }
        else if (nums[mid] == 1)
        {
            mid++;
        }
        else
        { // nums[mid] == 2
            std::swap(nums[mid], nums[high]);
            high--;
        }
    }
}

// Merge Sorted Array (LeetCode #88)
void merge(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
{
    int i = m - 1;     // Index for nums1
    int j = n - 1;     // Index for nums2
    int k = m + n - 1; // Index for merged array

    // Start from the end and work backwards
    while (i >= 0 && j >= 0)
    {
        if (nums1[i] > nums2[j])
        {
            nums1[k--] = nums1[i--];
        }
        else
        {
            nums1[k--] = nums2[j--];
        }
    }

    // Copy remaining elements from nums2 (if any)
    while (j >= 0)
    {
        nums1[k--] = nums2[j--];
    }
}

// Test LeetCode-style problems
void testLeetcodeProblems()
{
    std::cout << "\n===== LEETCODE-STYLE PROBLEMS =====" << std::endl;

    // Test Sort Array By Parity
    {
        std::vector<int> nums = {3, 1, 2, 4, 5, 6, 7, 8};
        std::cout << "Sort Array By Parity:" << std::endl;
        std::cout << "Original array: ";
        printVector(nums, "", nums.size());

        sortArrayByParity(nums);

        std::cout << "Result: ";
        printVector(nums, "", nums.size());

        // Verify: all even numbers should come before all odd numbers
        bool valid = true;
        bool foundOdd = false;
        for (int num : nums)
        {
            if (num % 2 == 1)
            { // Odd
                foundOdd = true;
            }
            else if (foundOdd)
            { // Even after odd
                valid = false;
                break;
            }
        }
        std::cout << "Valid result: " << (valid ? "Yes" : "No") << std::endl;
    }

    // Test Sort Colors
    {
        std::vector<int> nums = {2, 0, 2, 1, 1, 0, 0, 2, 1};
        std::cout << "\nSort Colors (Dutch National Flag Problem):" << std::endl;
        std::cout << "Original array: ";
        printVector(nums, "", nums.size());

        sortColors(nums);

        std::cout << "Result: ";
        printVector(nums, "", nums.size());

        // Verify: all 0s, then all 1s, then all 2s
        bool valid = true;
        int state = 0; // Expect 0s, then 1s, then 2s
        for (int num : nums)
        {
            if (num < state)
            {
                valid = false;
                break;
            }
            if (num > state)
            {
                state = num;
            }
        }
        std::cout << "Valid result: " << (valid ? "Yes" : "No") << std::endl;
    }

    // Test Merge Sorted Array
    {
        std::vector<int> nums1 = {1, 3, 5, 7, 0, 0, 0, 0};
        std::vector<int> nums2 = {2, 4, 6, 8};
        int m = 4; // Number of elements in nums1
        int n = 4; // Number of elements in nums2

        std::cout << "\nMerge Sorted Array:" << std::endl;
        std::cout << "nums1: ";
        printVector(std::vector<int>(nums1.begin(), nums1.begin() + m), "", m);
        std::cout << "nums2: ";
        printVector(nums2, "", n);

        merge(nums1, m, nums2, n);

        std::cout << "Merged result: ";
        printVector(nums1, "", nums1.size());

        // Verify: result should be sorted
        bool valid = true;
        for (int i = 1; i < nums1.size(); i++)
        {
            if (nums1[i] < nums1[i - 1])
            {
                valid = false;
                break;
            }
        }
        std::cout << "Valid result: " << (valid ? "Yes" : "No") << std::endl;
    }
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "====================================================" << std::endl;
    std::cout << "DAY 17: BASIC SORTING ALGORITHMS #DSAin45" << std::endl;
    std::cout << "====================================================" << std::endl;

    // Test each sorting algorithm independently
    testBubbleSort();
    testSelectionSort();
    testInsertionSort();

    // Compare all algorithms together
    compareAllSortingAlgorithms();

    // Practical applications
    cardSorting();
    onlineSorting();

    // LeetCode-style problems
    testLeetcodeProblems();

    std::cout << "\n====================================================" << std::endl;
    std::cout << "End of Day 17: Basic Sorting Algorithms" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;
}