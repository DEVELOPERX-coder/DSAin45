// Day 18: Efficient Sorting Algorithms #DSAin45
// Comprehensive implementations and comparisons of efficient sorting algorithms

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
#include <thread>
#include <cmath>
#include <fstream>
#include <sstream>

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
std::vector<int> generateNearlySortedVector(int size, int swaps = 5, int min = 0, int max = 1000)
{
    // Create a sorted vector
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
    {
        vec[i] = min + i \* ((max - min) / size);
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
        vec[i] = max - i \* ((max - min) / size);
    }
    return vec;
}

// Generate a vector with many duplicates
std::vector<int> generateVectorWithDuplicates(int size, int numUniqueValues = 10, int min = 0, int max = 1000)
{
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numUniqueValues - 1);

    // Generate unique values
    std::vector<int> uniqueValues(numUniqueValues);
    std::uniform_int_distribution<> valueDis(min, max);
    for (int i = 0; i < numUniqueValues; i++)
    {
        uniqueValues[i] = valueDis(gen);
    }

    // Fill the vector with randomly selected unique values
    for (int i = 0; i < size; i++)
    {
        vec[i] = uniqueValues[dis(gen)];
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

// Track the number of comparisons and swaps
struct SortStatistics
{
    size_t comparisons = 0;
    size_t swaps = 0;
};

// For baseline comparison - insertion sort
void insertionSort(std::vector<int> &arr)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

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

// ===== MERGE SORT IMPLEMENTATIONS =====

// Basic merge operation
void merge(std::vector<int> &arr, int left, int mid, int right)
{
    // Sizes of the temporary arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    std::vector<int> L(n1), R(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr
    int i = 0;    // Initial index of first subarray
    int j = 0;    // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Basic Merge Sort implementation
void mergeSort(std::vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        // Find the middle point
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Wrapper function for Merge Sort
void mergeSort(std::vector<int> &arr)
{
    mergeSort(arr, 0, arr.size() - 1);
}

// Merge operation with statistics
void mergeWithStats(std::vector<int> &arr, int left, int mid, int right, SortStatistics &stats)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        stats.comparisons++;
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        stats.swaps++; // Count array writes
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        stats.swaps++;
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        stats.swaps++;
        j++;
        k++;
    }
}

// Merge Sort with statistics
void mergeSortWithStats(std::vector<int> &arr, int left, int right, SortStatistics &stats)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSortWithStats(arr, left, mid, stats);
        mergeSortWithStats(arr, mid + 1, right, stats);

        mergeWithStats(arr, left, mid, right, stats);
    }
}

// Wrapper function for Merge Sort with statistics
void mergeSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    mergeSortWithStats(arr, 0, arr.size() - 1, stats);
}

// Merge Sort with Insertion Sort for small arrays
void hybridMergeSort(std::vector<int> &arr, int left, int right, int threshold = 10)
{
    if (right - left <= threshold)
    {
        // Use insertion sort for small arrays
        for (int i = left + 1; i <= right; i++)
        {
            int key = arr[i];
            int j = i - 1;

            while (j >= left && arr[j] > key)
            {
                arr[j + 1] = arr[j];
                j--;
            }

            arr[j + 1] = key;
        }
    }
    else
    {
        // Use merge sort for larger arrays
        int mid = left + (right - left) / 2;

        hybridMergeSort(arr, left, mid, threshold);
        hybridMergeSort(arr, mid + 1, right, threshold);

        merge(arr, left, mid, right);
    }
}

// Wrapper function for Hybrid Merge Sort
void hybridMergeSort(std::vector<int> &arr, int threshold = 10)
{
    hybridMergeSort(arr, 0, arr.size() - 1, threshold);
}

// Bottom-up (non-recursive) Merge Sort
void bottomUpMergeSort(std::vector<int> &arr)
{
    int n = arr.size();

    // Allocate a temporary array to avoid repeated memory allocations
    std::vector<int> temp(n);

    // Merge subarrays in bottom-up manner
    for (int width = 1; width < n; width = 2 * width)
    {
        // Merge subarrays arr[left:left+width-1] and arr[left+width:left+2*width-1]
        for (int left = 0; left < n; left += 2 * width)
        {
            int mid = std::min(left + width - 1, n - 1);
            int right = std::min(left + 2 * width - 1, n - 1);

            // Merge the two subarrays
            int i = left, j = mid + 1, k = 0;

            while (i <= mid && j <= right)
            {
                if (arr[i] <= arr[j])
                {
                    temp[k++] = arr[i++];
                }
                else
                {
                    temp[k++] = arr[j++];
                }
            }

            while (i <= mid)
            {
                temp[k++] = arr[i++];
            }

            while (j <= right)
            {
                temp[k++] = arr[j++];
            }

            // Copy back to the original array
            for (i = 0; i < k; i++)
            {
                arr[left + i] = temp[i];
            }
        }
    }
}

// Parallel Merge Sort using std::thread
void parallelMergeSort(std::vector<int> &arr, int numThreads = 4)
{
    int n = arr.size();
    if (n <= 1)
        return;

    // If array is small or too few threads, use sequential merge sort
    if (n < 1000 || numThreads <= 1)
    {
        mergeSort(arr);
        return;
    }

    std::vector<std::thread> threads;
    int segmentSize = n / numThreads;

    // Launch threads to sort segments in parallel
    for (int i = 0; i < numThreads; i++)
    {
        int left = i * segmentSize;
        int right = (i == numThreads - 1) ? n - 1 : (i + 1) * segmentSize - 1;

        threads.push_back(std::thread([&arr, left, right]()
                                      { mergeSort(arr, left, right); }));
    }

    // Wait for all threads to complete
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Merge the sorted segments
    for (int size = segmentSize; size < n; size = 2 * size)
    {
        for (int left = 0; left < n; left += 2 * size)
        {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);

            if (mid < right)
            {
                merge(arr, left, mid, right);
            }
        }
    }
}

// ===== QUICK SORT IMPLEMENTATIONS =====

// Basic partition operation for Quick Sort
int partition(std::vector<int> &arr, int low, int high)
{
    // Select the rightmost element as pivot
    int pivot = arr[high];

    // Index of smaller element
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot)
        {
            i++; // Increment index of smaller element
            std::swap(arr[i], arr[j]);
        }
    }

    // Place pivot in its correct position
    std::swap(arr[i + 1], arr[high]);
    return i + 1; // Return the partitioning index
}

// Basic Quick Sort implementation
void quickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        // Partition the array and get pivot position
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Wrapper function for Quick Sort
void quickSort(std::vector<int> &arr)
{
    quickSort(arr, 0, arr.size() - 1);
}

// Partition with statistics
int partitionWithStats(std::vector<int> &arr, int low, int high, SortStatistics &stats)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        stats.comparisons++;
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
            stats.swaps++;
        }
    }

    std::swap(arr[i + 1], arr[high]);
    stats.swaps++;

    return i + 1;
}

// Quick Sort with statistics
void quickSortWithStats(std::vector<int> &arr, int low, int high, SortStatistics &stats)
{
    if (low < high)
    {
        int pi = partitionWithStats(arr, low, high, stats);

        quickSortWithStats(arr, low, pi - 1, stats);
        quickSortWithStats(arr, pi + 1, high, stats);
    }
}

// Wrapper function for Quick Sort with statistics
void quickSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    quickSortWithStats(arr, 0, arr.size() - 1, stats);
}

// Random pivot selection
int randomizedPartition(std::vector<int> &arr, int low, int high)
{
    // Generate a random index between low and high
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int random = dis(gen);

    // Swap the randomly selected element with the high element
    std::swap(arr[random], arr[high]);

    // Call the standard partition method
    return partition(arr, low, high);
}

// Quick Sort with random pivot
void randomizedQuickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int pi = randomizedPartition(arr, low, high);

        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

// Wrapper function for Randomized Quick Sort
void randomizedQuickSort(std::vector<int> &arr)
{
    randomizedQuickSort(arr, 0, arr.size() - 1);
}

// Median-of-three pivot selection
int medianOfThree(std::vector<int> &arr, int low, int high)
{
    int mid = low + (high - low) / 2;

    // Sort low, mid, high values
    if (arr[mid] < arr[low])
        std::swap(arr[mid], arr[low]);

    if (arr[high] < arr[low])
        std::swap(arr[high], arr[low]);

    if (arr[high] < arr[mid])
        std::swap(arr[high], arr[mid]);

    // Place pivot at high-1
    std::swap(arr[mid], arr[high - 1]);

    return high - 1;
}

// Partition using median-of-three
int medianOfThreePartition(std::vector<int> &arr, int low, int high)
{
    if (high - low > 2)
    {
        int pivotIndex = medianOfThree(arr, low, high);
        int pivot = arr[pivotIndex];

        // Standard partition but using pivot at high-1
        int i = low;
        for (int j = low; j < high - 1; j++)
        {
            if (arr[j] <= pivot)
            {
                std::swap(arr[i], arr[j]);
                i++;
            }
        }

        std::swap(arr[i], arr[high - 1]);
        return i;
    }
    else
    {
        // For small arrays, use standard partition
        return partition(arr, low, high);
    }
}

// Quick Sort with median-of-three pivot selection
void medianOfThreeQuickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int pi = medianOfThreePartition(arr, low, high);

        medianOfThreeQuickSort(arr, low, pi - 1);
        medianOfThreeQuickSort(arr, pi + 1, high);
    }
}

// Wrapper function for Median-of-Three Quick Sort
void medianOfThreeQuickSort(std::vector<int> &arr)
{
    medianOfThreeQuickSort(arr, 0, arr.size() - 1);
}

// Three-way partitioning for handling duplicates efficiently
void threeWayPartition(std::vector<int> &arr, int low, int high, int &lt, int &gt)
{
    int pivot = arr[low];
    int i = low + 1;
    lt = low;
    gt = high;

    // Partition array into three parts:
    // [low...lt-1] < pivot
    // [lt...gt-1] = pivot
    // [gt...high] > pivot
    while (i <= gt)
    {
        if (arr[i] < pivot)
        {
            std::swap(arr[lt], arr[i]);
            lt++;
            i++;
        }
        else if (arr[i] > pivot)
        {
            std::swap(arr[i], arr[gt]);
            gt--;
        }
        else
        { // arr[i] == pivot
            i++;
        }
    }
}

// Quick Sort with three-way partitioning (Dutch National Flag algorithm)
void threeWayQuickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        // Randomly select pivot to avoid worst case
        std::swap(arr[low], arr[low + rand() % (high - low + 1)]);

        int lt, gt;
        threeWayPartition(arr, low, high, lt, gt);

        // Recursively sort the less than and greater than parts
        threeWayQuickSort(arr, low, lt - 1);
        threeWayQuickSort(arr, gt + 1, high);
    }
}

// Wrapper function for Three-Way Quick Sort
void threeWayQuickSort(std::vector<int> &arr)
{
    threeWayQuickSort(arr, 0, arr.size() - 1);
}

// Hybrid Quick Sort (using Insertion Sort for small partitions)
void hybridQuickSort(std::vector<int> &arr, int low, int high, int threshold = 10)
{
    while (low < high)
    {
        // Switch to insertion sort for small arrays
        if (high - low < threshold)
        {
            for (int i = low + 1; i <= high; i++)
            {
                int key = arr[i];
                int j = i - 1;

                while (j >= low && arr[j] > key)
                {
                    arr[j + 1] = arr[j];
                    j--;
                }

                arr[j + 1] = key;
            }
            break;
        }
        else
        {
            int pi = randomizedPartition(arr, low, high);

            // Recursively sort smaller partition
            // Tail call optimization: iterate on larger partition
            if (pi - low < high - pi)
            {
                hybridQuickSort(arr, low, pi - 1, threshold);
                low = pi + 1;
            }
            else
            {
                hybridQuickSort(arr, pi + 1, high, threshold);
                high = pi - 1;
            }
        }
    }
}

// Wrapper function for Hybrid Quick Sort
void hybridQuickSort(std::vector<int> &arr, int threshold = 10)
{
    hybridQuickSort(arr, 0, arr.size() - 1, threshold);
}

// Quick Select algorithm (to find kth smallest element)
int quickSelect(std::vector<int> &arr, int low, int high, int k)
{
    // If k is smaller than number of elements in array
    if (k > 0 && k <= high - low + 1)
    {
        // Partition the array around a random pivot
        int pivotIndex = randomizedPartition(arr, low, high);

        // If pivot is the kth smallest element
        if (pivotIndex - low == k - 1)
        {
            return arr[pivotIndex];
        }

        // If pivot position is greater than k, search in left subarray
        if (pivotIndex - low > k - 1)
        {
            return quickSelect(arr, low, pivotIndex - 1, k);
        }

        // Else search in right subarray for (k - pivotIndex + low - 1)th smallest
        return quickSelect(arr, pivotIndex + 1, high, k - (pivotIndex - low + 1));
    }

    // If k is out of bounds
    return -1;
}

// Wrapper function for Quick Select (kth smallest)
int quickSelectKthSmallest(std::vector<int> &arr, int k)
{
    return quickSelect(arr, 0, arr.size() - 1, k);
}

// Find kth largest element using Quick Select
int quickSelectKthLargest(std::vector<int> &arr, int k)
{
    // Kth largest is (n-k+1)th smallest
    return quickSelect(arr, 0, arr.size() - 1, arr.size() - k + 1);
}

// ===== HEAP SORT IMPLEMENTATIONS =====

// Heapify a subtree rooted with node 'i'
void heapify(std::vector<int> &arr, int n, int i)
{
    // Initialize largest as root
    int largest = i;
    int left = 2 _ i + 1;  // Left child
    int right = 2 _ i + 2; // Right child

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// Basic Heap Sort implementation
void heapSort(std::vector<int> &arr)
{
    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// Heapify with statistics
void heapifyWithStats(std::vector<int> &arr, int n, int i, SortStatistics &stats)
{
    int largest = i;
    int left = 2 _ i + 1;
    int right = 2 _ i + 2;

    if (left < n)
    {
        stats.comparisons++;
        if (arr[left] > arr[largest])
            largest = left;
    }

    if (right < n)
    {
        stats.comparisons++;
        if (arr[right] > arr[largest])
            largest = right;
    }

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        stats.swaps++;

        heapifyWithStats(arr, n, largest, stats);
    }
}

// Heap Sort with statistics
void heapSortWithStats(std::vector<int> &arr, SortStatistics &stats)
{
    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyWithStats(arr, n, i, stats);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(arr[0], arr[i]);
        stats.swaps++;

        heapifyWithStats(arr, i, 0, stats);
    }
}

// Iterative heapify (non-recursive)
void iterativeHeapify(std::vector<int> &arr, int n, int i)
{
    int largest = i;
    bool changed = true;

    while (changed)
    {
        changed = false;
        int left = 2 * largest + 1;
        int right = 2 * largest + 2;
        int new_largest = largest;

        if (left < n && arr[left] > arr[new_largest])
            new_largest = left;

        if (right < n && arr[right] > arr[new_largest])
            new_largest = right;

        if (new_largest != largest)
        {
            std::swap(arr[largest], arr[new_largest]);
            largest = new_largest;
            changed = true;
        }
    }
}

// Heap Sort with iterative heapify
void iterativeHeapSort(std::vector<int> &arr)
{
    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        iterativeHeapify(arr, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(arr[0], arr[i]);
        iterativeHeapify(arr, i, 0);
    }
}

// Bottom-up heap construction (more efficient)
void buildHeapBottomUp(std::vector<int> &arr)
{
    int n = arr.size();

    // Start from the last non-leaf node and heapify all nodes in reverse level order
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}

// Heap Sort with bottom-up heap construction
void bottomUpHeapSort(std::vector<int> &arr)
{
    int n = arr.size();

    // Build max heap using more efficient bottom-up approach
    buildHeapBottomUp(arr);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ===== TIM SORT IMPLEMENTATION =====

// Insertion sort for small arrays or small parts of the array
void insertionSort(std::vector<int> &arr, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

// Merge operation for Tim Sort
void merge(std::vector<int> &arr, int l, int m, int r)
{
    // Original array is broken into two parts, left and right array
    int len1 = m - l + 1, len2 = r - m;
    std::vector<int> left(len1), right(len2);

    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;

    // After comparing, we merge the two arrays
    while (i < len1 && j < len2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }

    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
    }
}

// Simplified Tim Sort implementation
void timSort(std::vector<int> &arr)
{
    int n = arr.size();
    const int RUN = 32; // Size of sub-arrays to be sorted

    // Sort individual sub-arrays of size RUN
    for (int i = 0; i < n; i += RUN)
    {
        insertionSort(arr, i, std::min((i + RUN - 1), (n - 1)));
    }

    // Start merging from size RUN (or 32)
    for (int size = RUN; size < n; size = 2 * size)
    {
        for (int left = 0; left < n; left += 2 * size)
        {
            int mid = left + size - 1;
            int right = std::min((left + 2 * size - 1), (n - 1));

            if (mid < right)
            {
                merge(arr, left, mid, right);
            }
        }
    }
}

// ===== INTRO SORT IMPLEMENTATION =====

// Calculate log base 2 of a number (used for IntroSort)
int log2(int n)
{
    return (n > 1) ? 1 + log2(n / 2) : 0;
}

// IntroSort combines Quick Sort, Heap Sort, and Insertion Sort
void introSort(std::vector<int> &arr, int begin, int end, int depthLimit)
{
    // Size of the array
    int size = end - begin;

    // If the array is small, use insertion sort
    if (size < 16)
    {
        insertionSort(arr, begin, end - 1);
        return;
    }

    // If the recursion depth exceeds the limit, switch to heap sort
    if (depthLimit == 0)
    {
        // Create a subvector for heap sort
        std::vector<int> subArr(arr.begin() + begin, arr.begin() + end);
        heapSort(subArr);

        // Copy back to the original array
        for (int i = 0; i < size; i++)
        {
            arr[begin + i] = subArr[i];
        }

        return;
    }

    // Use QuickSort with median-of-three pivot
    int pivot = medianOfThree(arr, begin, end - 1);
    std::swap(arr[pivot], arr[end - 1]);

    int pi = partition(arr, begin, end - 1);

    // Recursively sort the subarrays with reduced depth limit
    introSort(arr, begin, pi, depthLimit - 1);
    introSort(arr, pi + 1, end, depthLimit - 1);
}

// Wrapper function for IntroSort
void introSort(std::vector<int> &arr)
{
    if (arr.empty())
        return;

    // Set the depth limit as 2*log2(n)
    int depthLimit = 2 * log2(arr.size());
    introSort(arr, 0, arr.size(), depthLimit);
}

// ===== EXTERNAL SORTING IMPLEMENTATION =====

// External sort simulates sorting data that doesn't fit in memory
void externalSort(const std::string &inputFile, const std::string &outputFile, int chunkSize = 100)
{
    // 1. Divide the file into chunks, sort each chunk and write to temp files
    std::ifstream inFile(inputFile);
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return;
    }

    std::vector<std::string> tempFiles;
    std::vector<int> chunk;
    int num;
    int chunkNumber = 0;

    while (inFile >> num)
    {
        chunk.push_back(num);

        if (chunk.size() == chunkSize)
        {
            // Sort the chunk
            std::sort(chunk.begin(), chunk.end());

            // Write the sorted chunk to a temp file
            std::string tempFilename = "temp_" + std::to_string(chunkNumber++) + ".txt";
            std::ofstream tempFile(tempFilename);

            for (int n : chunk)
            {
                tempFile << n << " ";
            }

            tempFile.close();
            tempFiles.push_back(tempFilename);
            chunk.clear();
        }
    }

    // Handle the last chunk if it's not empty
    if (!chunk.empty())
    {
        std::sort(chunk.begin(), chunk.end());

        std::string tempFilename = "temp_" + std::to_string(chunkNumber++) + ".txt";
        std::ofstream tempFile(tempFilename);

        for (int n : chunk)
        {
            tempFile << n << " ";
        }

        tempFile.close();
        tempFiles.push_back(tempFilename);
    }

    inFile.close();

    // 2. Merge the sorted chunks using a priority queue (simplified here)
    std::vector<std::ifstream> fileStreams(tempFiles.size());
    std::vector<int> currentValues(tempFiles.size(), -1);
    std::ofstream outFile(outputFile);

    // Open all temp files
    for (size_t i = 0; i < tempFiles.size(); i++)
    {
        fileStreams[i].open(tempFiles[i]);
        if (fileStreams[i] >> currentValues[i])
        {
            // File opened and first value read successfully
        }
        else
        {
            currentValues[i] = std::numeric_limits<int>::max(); // Mark as empty
        }
    }

    // Merge until all streams are exhausted
    while (true)
    {
        // Find the smallest value among current values
        int minIdx = -1;
        int minVal = std::numeric_limits<int>::max();

        for (size_t i = 0; i < currentValues.size(); i++)
        {
            if (currentValues[i] < minVal)
            {
                minVal = currentValues[i];
                minIdx = i;
            }
        }

        // If all streams are exhausted, break
        if (minIdx == -1)
            break;

        // Write the smallest value to the output file
        outFile << minVal << " ";

        // Read the next value from the corresponding stream
        if (fileStreams[minIdx] >> currentValues[minIdx])
        {
            // Successfully read next value
        }
        else
        {
            currentValues[minIdx] = std::numeric_limits<int>::max(); // Mark as exhausted
        }
    }

    outFile.close();

    // Close all file streams
    for (auto &stream : fileStreams)
    {
        stream.close();
    }

    // Clean up temp files
    for (const auto &tempFile : tempFiles)
    {
        std::remove(tempFile.c_str());
    }
}

// Function to demonstrate external sort with in-memory simulation
void simulateExternalSort(std::vector<int> &arr, int chunkSize = 1000)
{
    int n = arr.size();
    std::vector<std::vector<int>> chunks;

    // 1. Divide the array into chunks
    for (int i = 0; i < n; i += chunkSize)
    {
        int chunkEnd = std::min(i + chunkSize, n);
        std::vector<int> chunk(arr.begin() + i, arr.begin() + chunkEnd);

        // Sort the chunk
        std::sort(chunk.begin(), chunk.end());
        chunks.push_back(chunk);
    }

    // 2. Merge the sorted chunks
    std::vector<int> result;
    std::vector<int> indices(chunks.size(), 0);

    while (true)
    {
        int minVal = std::numeric_limits<int>::max();
        int minChunk = -1;

        // Find the smallest value among current positions
        for (size_t i = 0; i < chunks.size(); i++)
        {
            if (indices[i] < chunks[i].size() && chunks[i][indices[i]] < minVal)
            {
                minVal = chunks[i][indices[i]];
                minChunk = i;
            }
        }

        // If all chunks are exhausted, break
        if (minChunk == -1)
            break;

        // Add the smallest value to the result
        result.push_back(minVal);

        // Move to the next value in the chosen chunk
        indices[minChunk]++;
    }

    // Copy the result back to the original array
    arr = result;
}

// ===== TEST AND VISUALIZATION FUNCTIONS =====

// Test and compare merge sort variations
void testMergeSort()
{
    std::cout << "\n===== MERGE SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(10000, 1, 10000);
    std::vector<int> almostSorted = generateNearlySortedVector(10000, 100);
    std::vector<int> reverseSorted = generateReverseSortedVector(10000);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Random Small"
              << std::setw(15) << "Random Medium"
              << std::setw(15) << "Almost Sorted"
              << std::setw(15) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Basic Merge Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(mergeSort, copy1);
        auto time2 = measureExecutionTimeVoid(mergeSort, copy2);
        auto time3 = measureExecutionTimeVoid(mergeSort, copy3);
        auto time4 = measureExecutionTimeVoid(mergeSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Merge Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Hybrid Merge Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(hybridMergeSort, copy1);
        auto time2 = measureExecutionTimeVoid(hybridMergeSort, copy2);
        auto time3 = measureExecutionTimeVoid(hybridMergeSort, copy3);
        auto time4 = measureExecutionTimeVoid(hybridMergeSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Hybrid Merge Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Bottom-up Merge Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(bottomUpMergeSort, copy1);
        auto time2 = measureExecutionTimeVoid(bottomUpMergeSort, copy2);
        auto time3 = measureExecutionTimeVoid(bottomUpMergeSort, copy3);
        auto time4 = measureExecutionTimeVoid(bottomUpMergeSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Bottom-up Merge Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Parallel Merge Sort (only for larger arrays)
    {
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time2 = measureExecutionTimeVoid(parallelMergeSort, copy2);
        auto time3 = measureExecutionTimeVoid(parallelMergeSort, copy3);
        auto time4 = measureExecutionTimeVoid(parallelMergeSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Parallel Merge Sort"
                  << std::setw(15) << "N/A (too small)"
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Count operations for merge sort
    std::cout << "\nOperations count on random array (size=10000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(20) << "Comparisons"
              << std::setw(20) << "Swaps/Assignments"
              << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        mergeSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Merge Sort"
                  << std::setw(20) << stats.comparisons
                  << std::setw(20) << stats.swaps
                  << std::endl;
    }

    // Step-by-step visualization of merge sort
    std::cout << "\nStep-by-step merge sort visualization:" << std::endl;
    std::vector<int> demo = {38, 27, 43, 3, 9, 82, 10};
    std::cout << "Initial array: ";
    printVector(demo, "", demo.size());

    std::cout << "\n1. Divide: Split into [38, 27, 43, 3] and [9, 82, 10]" << std::endl;

    std::cout << "\n2. Recursively sort first half:" << std::endl;
    std::cout << "   2.1. Split [38, 27, 43, 3] into [38, 27] and [43, 3]" << std::endl;
    std::cout << "   2.2. Split [38, 27] into [38] and [27]" << std::endl;
    std::cout << "   2.3. Merge [38] and [27] into [27, 38]" << std::endl;
    std::cout << "   2.4. Split [43, 3] into [43] and [3]" << std::endl;
    std::cout << "   2.5. Merge [43] and [3] into [3, 43]" << std::endl;
    std::cout << "   2.6. Merge [27, 38] and [3, 43] into [3, 27, 38, 43]" << std::endl;

    std::cout << "\n3. Recursively sort second half:" << std::endl;
    std::cout << "   3.1. Split [9, 82, 10] into [9, 82] and [10]" << std::endl;
    std::cout << "   3.2. Split [9, 82] into [9] and [82]" << std::endl;
    std::cout << "   3.3. Merge [9] and [82] into [9, 82]" << std::endl;
    std::cout << "   3.4. Merge [9, 82] and [10] into [9, 10, 82]" << std::endl;

    std::cout << "\n4. Final merge: Combine [3, 27, 38, 43] and [9, 10, 82]" << std::endl;
    std::cout << "   Step 1: Compare 3 and 9    -> [3]" << std::endl;
    std::cout << "   Step 2: Compare 27 and 9   -> [3, 9]" << std::endl;
    std::cout << "   Step 3: Compare 27 and 10  -> [3, 9, 10]" << std::endl;
    std::cout << "   Step 4: Compare 27 and 82  -> [3, 9, 10, 27]" << std::endl;
    std::cout << "   Step 5: Compare 38 and 82  -> [3, 9, 10, 27, 38]" << std::endl;
    std::cout << "   Step 6: Compare 43 and 82  -> [3, 9, 10, 27, 38, 43]" << std::endl;
    std::cout << "   Step 7: Add remaining 82   -> [3, 9, 10, 27, 38, 43, 82]" << std::endl;

    // Actually perform the sort
    mergeSort(demo);
    std::cout << "\nFinal sorted array: ";
    printVector(demo, "", demo.size());
}

// Test and compare quick sort variations
void testQuickSort()
{
    std::cout << "\n===== QUICK SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(10000, 1, 10000);
    std::vector<int> almostSorted = generateNearlySortedVector(10000, 100);
    std::vector<int> reverseSorted = generateReverseSortedVector(10000);
    std::vector<int> manyDuplicates = generateVectorWithDuplicates(10000, 100);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Random Small"
              << std::setw(15) << "Random Medium"
              << std::setw(15) << "Almost Sorted"
              << std::setw(15) << "Reverse Sorted"
              << std::setw(15) << "Many Duplicates"
              << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    // Basic Quick Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);
        auto copy5 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(quickSort, copy1);
        auto time2 = measureExecutionTimeVoid(quickSort, copy2);
        auto time3 = measureExecutionTimeVoid(quickSort, copy3);
        auto time4 = measureExecutionTimeVoid(quickSort, copy4);
        auto time5 = measureExecutionTimeVoid(quickSort, copy5);

        std::cout << std::left
                  << std::setw(25) << "Quick Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::setw(15) << time5
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
        assert(isSorted(copy5));
    }

    // Randomized Quick Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);
        auto copy5 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(randomizedQuickSort, copy1);
        auto time2 = measureExecutionTimeVoid(randomizedQuickSort, copy2);
        auto time3 = measureExecutionTimeVoid(randomizedQuickSort, copy3);
        auto time4 = measureExecutionTimeVoid(randomizedQuickSort, copy4);
        auto time5 = measureExecutionTimeVoid(randomizedQuickSort, copy5);

        std::cout << std::left
                  << std::setw(25) << "Randomized Quick Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::setw(15) << time5
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
        assert(isSorted(copy5));
    }

    // Median-of-Three Quick Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);
        auto copy5 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(medianOfThreeQuickSort, copy1);
        auto time2 = measureExecutionTimeVoid(medianOfThreeQuickSort, copy2);
        auto time3 = measureExecutionTimeVoid(medianOfThreeQuickSort, copy3);
        auto time4 = measureExecutionTimeVoid(medianOfThreeQuickSort, copy4);
        auto time5 = measureExecutionTimeVoid(medianOfThreeQuickSort, copy5);

        std::cout << std::left
                  << std::setw(25) << "Median-of-Three Quick Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::setw(15) << time5
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
        assert(isSorted(copy5));
    }

    // Three-Way Quick Sort (Dutch National Flag)
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);
        auto copy5 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(threeWayQuickSort, copy1);
        auto time2 = measureExecutionTimeVoid(threeWayQuickSort, copy2);
        auto time3 = measureExecutionTimeVoid(threeWayQuickSort, copy3);
        auto time4 = measureExecutionTimeVoid(threeWayQuickSort, copy4);
        auto time5 = measureExecutionTimeVoid(threeWayQuickSort, copy5);

        std::cout << std::left
                  << std::setw(25) << "Three-Way Quick Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::setw(15) << time5
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
        assert(isSorted(copy5));
    }

    // Hybrid Quick Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);
        auto copy5 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(hybridQuickSort, copy1);
        auto time2 = measureExecutionTimeVoid(hybridQuickSort, copy2);
        auto time3 = measureExecutionTimeVoid(hybridQuickSort, copy3);
        auto time4 = measureExecutionTimeVoid(hybridQuickSort, copy4);
        auto time5 = measureExecutionTimeVoid(hybridQuickSort, copy5);

        std::cout << std::left
                  << std::setw(25) << "Hybrid Quick Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::setw(15) << time5
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
        assert(isSorted(copy5));
    }

    // Count operations for quick sort
    std::cout << "\nOperations count on random array (size=10000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Comparisons"
              << std::setw(15) << "Swaps"
              << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        quickSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Quick Sort"
                  << std::setw(15) << stats.comparisons
                  << std::setw(15) << stats.swaps
                  << std::endl;
    }

    // Test quick select
    std::cout << "\nQuick Select tests:" << std::endl;

    // Test on small array
    std::vector<int> selectDemo = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    printVector(selectDemo, "Original array", selectDemo.size());

    for (int k = 1; k <= selectDemo.size(); k++)
    {
        auto copy = cloneVector(selectDemo);
        int kthSmallest = quickSelectKthSmallest(copy, k);
        std::cout << k << "th smallest element: " << kthSmallest << std::endl;
    }

    for (int k = 1; k <= selectDemo.size(); k++)
    {
        auto copy = cloneVector(selectDemo);
        int kthLargest = quickSelectKthLargest(copy, k);
        std::cout << k << "th largest element: " << kthLargest << std::endl;
    }

    // Step-by-step visualization of quick sort
    std::cout << "\nStep-by-step quick sort visualization:" << std::endl;
    std::vector<int> qsDemo = {10, 7, 8, 9, 1, 5};
    std::cout << "Initial array: ";
    printVector(qsDemo, "", qsDemo.size());

    std::cout << "\n1. Choose pivot: Pivot = 5 (last element)" << std::endl;
    std::cout << "2. Partition:" << std::endl;
    std::cout << "   Compare 10 with pivot 5: 10 > 5, no swap" << std::endl;
    std::cout << "   Compare 7 with pivot 5: 7 > 5, no swap" << std::endl;
    std::cout << "   Compare 8 with pivot 5: 8 > 5, no swap" << std::endl;
    std::cout << "   Compare 9 with pivot 5: 9 > 5, no swap" << std::endl;
    std::cout << "   Compare 1 with pivot 5: 1 < 5, swap with first element > pivot (10)" << std::endl;
    std::cout << "   Array after swap: [1, 7, 8, 9, 10, 5]" << std::endl;
    std::cout << "   Final swap to put pivot in its correct position" << std::endl;
    std::cout << "   Array after partition: [1, 5, 8, 9, 10, 7]" << std::endl;
    std::cout << "   Pivot is at index 1" << std::endl;

    std::cout << "\n3. Recursively sort left partition [1]" << std::endl;
    std::cout << "   Only one element, already sorted" << std::endl;

    std::cout << "\n4. Recursively sort right partition [8, 9, 10, 7]" << std::endl;
    std::cout << "   Choose pivot 7" << std::endl;
    std::cout << "   Partition gives [7, 8, 9, 10]" << std::endl;
    std::cout << "   Recursively sort resulting partitions..." << std::endl;

    std::cout << "\n5. Final sorted array: [1, 5, 7, 8, 9, 10]" << std::endl;

    // Actually perform the sort
    quickSort(qsDemo);
    printVector(qsDemo, "Sorted array", qsDemo.size());
}

// Test and compare heap sort variations
void testHeapSort()
{
    std::cout << "\n===== HEAP SORT TESTS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(10000, 1, 10000);
    std::vector<int> almostSorted = generateNearlySortedVector(10000, 100);
    std::vector<int> reverseSorted = generateReverseSortedVector(10000);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Random Small"
              << std::setw(15) << "Random Medium"
              << std::setw(15) << "Almost Sorted"
              << std::setw(15) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Basic Heap Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(heapSort, copy1);
        auto time2 = measureExecutionTimeVoid(heapSort, copy2);
        auto time3 = measureExecutionTimeVoid(heapSort, copy3);
        auto time4 = measureExecutionTimeVoid(heapSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Heap Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Iterative Heap Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(iterativeHeapSort, copy1);
        auto time2 = measureExecutionTimeVoid(iterativeHeapSort, copy2);
        auto time3 = measureExecutionTimeVoid(iterativeHeapSort, copy3);
        auto time4 = measureExecutionTimeVoid(iterativeHeapSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Iterative Heap Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Bottom-up Heap Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(bottomUpHeapSort, copy1);
        auto time2 = measureExecutionTimeVoid(bottomUpHeapSort, copy2);
        auto time3 = measureExecutionTimeVoid(bottomUpHeapSort, copy3);
        auto time4 = measureExecutionTimeVoid(bottomUpHeapSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Bottom-up Heap Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Count operations for heap sort
    std::cout << "\nOperations count on random array (size=10000):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Comparisons"
              << std::setw(15) << "Swaps"
              << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    {
        auto copy = cloneVector(randomMedium);
        SortStatistics stats;
        heapSortWithStats(copy, stats);

        std::cout << std::left
                  << std::setw(25) << "Heap Sort"
                  << std::setw(15) << stats.comparisons
                  << std::setw(15) << stats.swaps
                  << std::endl;
    }

    // Step-by-step visualization of heap sort
    std::cout << "\nStep-by-step heap sort visualization:" << std::endl;
    std::vector<int> heapDemo = {4, 10, 3, 5, 1};
    std::cout << "Initial array: ";
    printVector(heapDemo, "", heapDemo.size());

    std::cout << "\n1. Build a max heap:" << std::endl;
    std::cout << "   Start with the last non-leaf node (index = n/2-1 = 1): value 10" << std::endl;
    std::cout << "   Compare with children: 10 > 5 and 10 > 1, no swap needed" << std::endl;
    std::cout << "   Move to the next node leftward (index = 0): value 4" << std::endl;
    std::cout << "   Compare with children: 4 < 10, swap 4 and 10" << std::endl;
    std::cout << "   After heapify at root: [10, 4, 3, 5, 1]" << std::endl;
    std::cout << "   Recheck the subtree: 4 < 5, swap 4 and 5" << std::endl;
    std::cout << "   After complete heapification: [10, 5, 3, 4, 1]" << std::endl;

    std::cout << "\n2. Extract the maximum element (10):" << std::endl;
    std::cout << "   Swap 10 and 1: [1, 5, 3, 4, 10]" << std::endl;
    std::cout << "   Heapify the first 4 elements starting from root:" << std::endl;
    std::cout << "   Compare 1 with children: 1 < 5 and 1 < 3, swap with larger child (5)" << std::endl;
    std::cout << "   After swap: [5, 1, 3, 4, 10]" << std::endl;
    std::cout << "   Recheck the subtree: 1 < 4, swap 1 and 4" << std::endl;
    std::cout << "   After heapify: [5, 4, 3, 1, 10]" << std::endl;

    std::cout << "\n3. Extract the maximum element (5):" << std::endl;
    std::cout << "   Swap 5 and 1: [1, 4, 3, 5, 10]" << std::endl;
    std::cout << "   Heapify the first 3 elements..." << std::endl;
    std::cout << "   After heapify: [4, 1, 3, 5, 10]" << std::endl;

    std::cout << "\n4. Extract the maximum element (4):" << std::endl;
    std::cout << "   Swap 4 and 3: [3, 1, 4, 5, 10]" << std::endl;
    std::cout << "   Heapify the first 2 elements..." << std::endl;
    std::cout << "   After heapify: [3, 1, 4, 5, 10]" << std::endl;

    std::cout << "\n5. Extract the maximum element (3):" << std::endl;
    std::cout << "   Swap 3 and 1: [1, 3, 4, 5, 10]" << std::endl;

    std::cout << "\n6. Final sorted array: [1, 3, 4, 5, 10]" << std::endl;

    // Actually perform the sort
    heapSort(heapDemo);
    printVector(heapDemo, "Sorted array", heapDemo.size());
}

// Test Tim Sort and IntroSort
void testAdvancedSorts()
{
    std::cout << "\n===== ADVANCED SORTS (TIM SORT & INTROSORT) =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(100, 1, 1000);
    std::vector<int> randomMedium = generateRandomVector(10000, 1, 10000);
    std::vector<int> almostSorted = generateNearlySortedVector(10000, 100);
    std::vector<int> reverseSorted = generateReverseSortedVector(10000);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Random Small"
              << std::setw(15) << "Random Medium"
              << std::setw(15) << "Almost Sorted"
              << std::setw(15) << "Reverse Sorted"
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Tim Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(timSort, copy1);
        auto time2 = measureExecutionTimeVoid(timSort, copy2);
        auto time3 = measureExecutionTimeVoid(timSort, copy3);
        auto time4 = measureExecutionTimeVoid(timSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Tim Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // IntroSort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(introSort, copy1);
        auto time2 = measureExecutionTimeVoid(introSort, copy2);
        auto time3 = measureExecutionTimeVoid(introSort, copy3);
        auto time4 = measureExecutionTimeVoid(introSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "IntroSort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // std::sort for comparison
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomMedium);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(reverseSorted);

        auto time1 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy1.begin(), copy1.end());
        auto time2 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy2.begin(), copy2.end());
        auto time3 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy3.begin(), copy3.end());
        auto time4 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy4.begin(), copy4.end());

        std::cout << std::left
                  << std::setw(25) << "std::sort (for comparison)"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }
}

// Test external sorting
void testExternalSort()
{
    std::cout << "\n===== EXTERNAL SORT SIMULATION =====" << std::endl;

    // Generate a large array
    int size = 100000;
    std::vector<int> bigArray = generateRandomVector(size, 1, size * 10);
    std::vector<int> bigArrayCopy = cloneVector(bigArray);

    std::cout << "Sorting a large array of " << size << " elements using external sort simulation..." << std::endl;

    auto duration = measureExecutionTimeVoid(simulateExternalSort, bigArray, 1000);

    std::cout << "External sort completed in " << duration << " microseconds" << std::endl;
    assert(isSorted(bigArray));

    // Compare with standard sort
    auto stdDuration = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, bigArrayCopy.begin(), bigArrayCopy.end());
    std::cout << "Standard sort completed in " << stdDuration << " microseconds" << std::endl;
    assert(isSorted(bigArrayCopy));

    std::cout << "External/Standard sort time ratio: " << static_cast<double>(duration) / stdDuration << std::endl;
}

// Compare all efficient sorting algorithms together
void compareAllSortingAlgorithms()
{
    std::cout << "\n===== COMPARING ALL EFFICIENT SORTING ALGORITHMS =====" << std::endl;

    // Generate test data sets of different sizes
    std::vector<int> randomSmall = generateRandomVector(1000, 1, 1000);
    std::vector<int> randomLarge = generateRandomVector(100000, 1, 100000);
    std::vector<int> almostSorted = generateNearlySortedVector(10000, 100);
    std::vector<int> manyDuplicates = generateVectorWithDuplicates(10000, 10);

    std::cout << "\nPerformance comparison (times in microseconds):" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Random (1K)"
              << std::setw(15) << "Random (100K)"
              << std::setw(15) << "Almost Sorted"
              << std::setw(15) << "Many Duplicates"
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // =============== O(n^2) ALGORITHMS FOR COMPARISON ===============

    // Insertion Sort (only for smaller arrays)
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(insertionSort, copy1);
        auto time3 = measureExecutionTimeVoid(insertionSort, copy3);
        auto time4 = measureExecutionTimeVoid(insertionSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Insertion Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << "Too slow"
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness for smaller arrays
        assert(isSorted(copy1));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // =============== O(n log n) ALGORITHMS ===============

    // Merge Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(mergeSort, copy1);
        auto time2 = measureExecutionTimeVoid(mergeSort, copy2);
        auto time3 = measureExecutionTimeVoid(mergeSort, copy3);
        auto time4 = measureExecutionTimeVoid(mergeSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Merge Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Quick Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(randomizedQuickSort, copy1);
        auto time2 = measureExecutionTimeVoid(randomizedQuickSort, copy2);
        auto time3 = measureExecutionTimeVoid(randomizedQuickSort, copy3);
        auto time4 = measureExecutionTimeVoid(randomizedQuickSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Quick Sort (Randomized)"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Heap Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(heapSort, copy1);
        auto time2 = measureExecutionTimeVoid(heapSort, copy2);
        auto time3 = measureExecutionTimeVoid(heapSort, copy3);
        auto time4 = measureExecutionTimeVoid(heapSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Heap Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Tim Sort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(timSort, copy1);
        auto time2 = measureExecutionTimeVoid(timSort, copy2);
        auto time3 = measureExecutionTimeVoid(timSort, copy3);
        auto time4 = measureExecutionTimeVoid(timSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "Tim Sort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // IntroSort
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(introSort, copy1);
        auto time2 = measureExecutionTimeVoid(introSort, copy2);
        auto time3 = measureExecutionTimeVoid(introSort, copy3);
        auto time4 = measureExecutionTimeVoid(introSort, copy4);

        std::cout << std::left
                  << std::setw(25) << "IntroSort"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // std::sort (for comparison)
    {
        auto copy1 = cloneVector(randomSmall);
        auto copy2 = cloneVector(randomLarge);
        auto copy3 = cloneVector(almostSorted);
        auto copy4 = cloneVector(manyDuplicates);

        auto time1 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy1.begin(), copy1.end());
        auto time2 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy2.begin(), copy2.end());
        auto time3 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy3.begin(), copy3.end());
        auto time4 = measureExecutionTimeVoid(std::sort<std::vector<int>::iterator>, copy4.begin(), copy4.end());

        std::cout << std::left
                  << std::setw(25) << "std::sort (C++ STL)"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4
                  << std::endl;

        // Verify sorting correctness
        assert(isSorted(copy1));
        assert(isSorted(copy2));
        assert(isSorted(copy3));
        assert(isSorted(copy4));
    }

    // Summary
    std::cout << "\nAlgorithm Characteristics:" << std::endl;
    std::cout << std::left
              << std::setw(25) << "Algorithm"
              << std::setw(15) << "Time (Best)"
              << std::setw(15) << "Time (Avg)"
              << std::setw(15) << "Time (Worst)"
              << std::setw(15) << "Space"
              << std::setw(15) << "Stable"
              << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::left
              << std::setw(25) << "Merge Sort"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n)"
              << std::setw(15) << "Yes"
              << std::endl;

    std::cout << std::left
              << std::setw(25) << "Quick Sort"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n²)"
              << std::setw(15) << "O(log n)"
              << std::setw(15) << "No"
              << std::endl;

    std::cout << std::left
              << std::setw(25) << "Heap Sort"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(1)"
              << std::setw(15) << "No"
              << std::endl;

    std::cout << std::left
              << std::setw(25) << "Tim Sort"
              << std::setw(15) << "O(n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n)"
              << std::setw(15) << "Yes"
              << std::endl;

    std::cout << std::left
              << std::setw(25) << "IntroSort"
              << std::setw(15) << "O(n log n)"
              << std::
              << std::setw(25) << "IntroSort"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(n log n)"
              << std::setw(15) << "O(1)"
              << std::setw(15) << "No"
              << std::endl;
}

// ===== REAL-WORLD EXAMPLES =====

// Sorting a dataset of student records
void studentRecordsSorting()
{
    std::cout << "\n===== REAL-WORLD EXAMPLE: STUDENT RECORDS SORTING =====" << std::endl;

    struct Student
    {
        std::string name;
        int id;
        float gpa;

        // For printing
        std::string toString() const
        {
            std::ostringstream oss;
            oss << "Name: " << name << ", ID: " << id << ", GPA: " << gpa;
            return oss.str();
        }
    };

    // Create a dataset of students
    std::vector<Student> students = {
        {"Alice", 1001, 3.8},
        {"Bob", 1002, 3.6},
        {"Charlie", 1003, 3.9},
        {"David", 1004, 3.5},
        {"Eve", 1005, 4.0},
        {"Frank", 1006, 3.7},
        {"Grace", 1007, 3.4},
        {"Heidi", 1008, 3.2},
        {"Ivan", 1009, 3.8},
        {"Judy", 1010, 3.9}};

    std::cout << "Original student records:" << std::endl;
    for (const auto &student : students)
    {
        std::cout << student.toString() << std::endl;
    }

    // Sort by ID using stable sort (equivalent to Merge Sort)
    std::cout << "\nSorting by ID (using stable_sort):" << std::endl;
    std::stable_sort(students.begin(), students.end(),
                     [](const Student &a, const Student &b)
                     {
                         return a.id < b.id;
                     });

    for (const auto &student : students)
    {
        std::cout << student.toString() << std::endl;
    }

    // Sort by GPA in descending order using Quick Sort-like algorithm
    std::cout << "\nSorting by GPA in descending order (using std::sort):" << std::endl;
    std::sort(students.begin(), students.end(),
              [](const Student &a, const Student &b)
              {
                  return a.gpa > b.gpa;
              });

    for (const auto &student : students)
    {
        std::cout << student.toString() << std::endl;
    }

    // Sort by name while preserving GPA order when names are the same
    std::cout << "\nSorting by name (keeping GPA order when names are same):" << std::endl;

    // Create a different dataset with some duplicate names
    std::vector<Student> studentsWithDuplicates = {
        {"Alice", 1001, 3.8},
        {"Bob", 1002, 3.6},
        {"Charlie", 1003, 3.9},
        {"Alice", 1004, 3.5}, // Same name as first student
        {"Eve", 1005, 4.0},
        {"Bob", 1006, 3.7}, // Same name as second student
        {"Grace", 1007, 3.4},
        {"Charlie", 1008, 3.2}, // Same name as third student
        {"Ivan", 1009, 3.8},
        {"Judy", 1010, 3.9}};

    // Sort by GPA first (unstable sort is fine)
    std::sort(studentsWithDuplicates.begin(), studentsWithDuplicates.end(),
              [](const Student &a, const Student &b)
              {
                  return a.gpa > b.gpa;
              });

    // Then sort by name using stable sort
    std::stable_sort(studentsWithDuplicates.begin(), studentsWithDuplicates.end(),
                     [](const Student &a, const Student &b)
                     {
                         return a.name < b.name;
                     });

    for (const auto &student : studentsWithDuplicates)
    {
        std::cout << student.toString() << std::endl;
    }
}

// Example of finding median with Quick Select
void medianFinding()
{
    std::cout << "\n===== REAL-WORLD EXAMPLE: MEDIAN FINDING WITH QUICK SELECT =====" << std::endl;

    // Generate a random dataset
    std::vector<int> data = generateRandomVector(15, 1, 100);
    printVector(data, "Original dataset", data.size());

    // Make a copy for verification
    auto dataCopy = cloneVector(data);
    std::sort(dataCopy.begin(), dataCopy.end());
    printVector(dataCopy, "Sorted dataset (for reference)", dataCopy.size());

    // Find median using Quick Select
    int n = data.size();
    int medianIndex = (n + 1) / 2; // For 1-indexed median

    std::cout << "Finding the " << medianIndex << "th smallest element (median) using Quick Select..." << std::endl;

    int median = quickSelectKthSmallest(data, medianIndex);
    std::cout << "Median: " << median << std::endl;

    // Verify median is correct
    int expectedMedian = dataCopy[medianIndex - 1]; // Adjust for 0-indexing
    std::cout << "Expected median: " << expectedMedian << std::endl;
    assert(median == expectedMedian);

    // Show performance comparison with sorting
    auto copy1 = cloneVector(dataCopy);
    auto copy2 = cloneVector(dataCopy);

    // Time to find median using sorting
    auto sortTime = measureExecutionTimeVoid([&copy1]()
                                             {
        std::sort(copy1.begin(), copy1.end());
        int medianIndex = (copy1.size() + 1) / 2 - 1; // Adjust for 0-indexing
        int median = copy1[medianIndex]; });

    // Time to find median using Quick Select
    auto quickSelectTime = measureExecutionTimeVoid([&copy2, medianIndex]()
                                                    { int median = quickSelectKthSmallest(copy2, medianIndex); });

    std::cout << "Time to find median by sorting: " << sortTime << " microseconds" << std::endl;
    std::cout << "Time to find median by Quick Select: " << quickSelectTime << " microseconds" << std::endl;
    std::cout << "Quick Select is " << static_cast<double>(sortTime) / quickSelectTime << "x faster" << std::endl;
}

// Example of external sorting for log files
void logFileSorting()
{
    std::cout << "\n===== REAL-WORLD EXAMPLE: LOG FILE SORTING SIMULATION =====" << std::endl;

    // Create a simulated log file with timestamps and messages
    struct LogEntry
    {
        std::string timestamp;
        std::string message;

        std::string toString() const
        {
            return timestamp + " - " + message;
        }

        bool operator<(const LogEntry &other) const
        {
            return timestamp < other.timestamp;
        }
    };

    // Generate some sample log entries
    std::vector<LogEntry> logEntries = {
        {"2023-06-15 14:25:36", "User login failed"},
        {"2023-06-15 09:12:45", "System startup"},
        {"2023-06-15 16:42:12", "Database backup completed"},
        {"2023-06-15 10:30:18", "New user registered"},
        {"2023-06-15 15:05:22", "File download error"},
        {"2023-06-15 12:17:33", "Memory usage alert"},
        {"2023-06-15 08:45:50", "Daily maintenance started"},
        {"2023-06-15 11:22:15", "Email service restarted"},
        {"2023-06-15 13:40:27", "Configuration update"},
        {"2023-06-15 17:30:05", "System shutdown"}};

    // Shuffle the log entries to simulate unsorted logs
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(logEntries.begin(), logEntries.end(), g);

    std::cout << "Unsorted log entries:" << std::endl;
    for (const auto &entry : logEntries)
    {
        std::cout << entry.toString() << std::endl;
    }

    // External sort simulation (simplified for this example)
    int chunkSize = 3; // Small chunk size for demonstration
    std::vector<std::vector<LogEntry>> chunks;

    // Divide into chunks and sort each chunk
    for (int i = 0; i < logEntries.size(); i += chunkSize)
    {
        int end = std::min(i + chunkSize, static_cast<int>(logEntries.size()));
        std::vector<LogEntry> chunk(logEntries.begin() + i, logEntries.begin() + end);

        // Sort the chunk
        std::sort(chunk.begin(), chunk.end());
        chunks.push_back(chunk);

        std::cout << "\nSorted chunk " << (i / chunkSize + 1) << ":" << std::endl;
        for (const auto &entry : chunk)
        {
            std::cout << entry.toString() << std::endl;
        }
    }

    // Merge the sorted chunks
    std::vector<LogEntry> sortedLogs;
    std::vector<int> indices(chunks.size(), 0);

    while (true)
    {
        int minChunk = -1;
        LogEntry minEntry = {"9999-99-99 99:99:99", ""}; // Initialize with a value larger than any possible timestamp

        // Find the smallest timestamp among current positions in all chunks
        for (size_t i = 0; i < chunks.size(); i++)
        {
            if (indices[i] < chunks[i].size() && chunks[i][indices[i]] < minEntry)
            {
                minEntry = chunks[i][indices[i]];
                minChunk = i;
            }
        }

        // If all chunks are exhausted, break
        if (minChunk == -1)
            break;

        // Add the smallest entry to the result
        sortedLogs.push_back(minEntry);

        // Move to the next entry in the chosen chunk
        indices[minChunk]++;
    }

    std::cout << "\nFinal sorted log entries:" << std::endl;
    for (const auto &entry : sortedLogs)
    {
        std::cout << entry.toString() << std::endl;
    }
}

// ===== LEETCODE-STYLE PROBLEMS =====

// Merge Sorted Array (LeetCode #88)
void mergeSortedArray(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
{
    int i = m - 1;     // Last element in nums1
    int j = n - 1;     // Last element in nums2
    int k = m + n - 1; // Last position in merged array

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

    // If there are remaining elements in nums2
    while (j >= 0)
    {
        nums1[k--] = nums2[j--];
    }
}

// Sort Colors (LeetCode #75)
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

// Kth Largest Element in an Array (LeetCode #215)
int findKthLargest(std::vector<int> &nums, int k)
{
    return quickSelectKthLargest(nums, k);
}

// Test LeetCode-style problems
void testLeetcodeProblems()
{
    std::cout << "\n===== LEETCODE-STYLE PROBLEMS =====" << std::endl;

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

        mergeSortedArray(nums1, m, nums2, n);

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

    // Test Kth Largest Element
    {
        std::vector<int> nums = {3, 2, 1, 5, 6, 4};
        int k = 2;

        std::cout << "\nKth Largest Element in an Array:" << std::endl;
        std::cout << "Array: ";
        printVector(nums, "", nums.size());
        std::cout << "k = " << k << std::endl;

        int result = findKthLargest(nums, k);

        std::cout << "The " << k << "th largest element is: " << result << std::endl;

        // Verify by sorting
        auto numsCopy = nums;
        std::sort(numsCopy.begin(), numsCopy.end(), std::greater<int>()); // Sort in descending order
        std::cout << "Sorted array (descending): ";
        printVector(numsCopy, "", numsCopy.size());
        std::cout << "Expected " << k << "th largest: " << numsCopy[k - 1] << std::endl;
        std::cout << "Valid result: " << (result == numsCopy[k - 1] ? "Yes" : "No") << std::endl;
    }
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "====================================================" << std::endl;
    std::cout << "DAY 18: EFFICIENT SORTING ALGORITHMS #DSAin45" << std::endl;
    std::cout << "====================================================" << std::endl;

    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Test each efficient sorting algorithm independently
    testMergeSort();
    testQuickSort();
    testHeapSort();
    testAdvancedSorts();

    // Test external sorting
    testExternalSort();

    // Compare all sorting algorithms
    compareAllSortingAlgorithms();

    // Real-world examples
    studentRecordsSorting();
    medianFinding();
    logFileSorting();

    // LeetCode-style problems
    testLeetcodeProblems();

    std::cout << "\n====================================================" << std::endl;
    std::cout << "End of Day 18: Efficient Sorting Algorithms" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;
}