// Day 19: Divide and Conquer Strategy #DSAin45
// Comprehensive implementations of divide and conquer algorithms

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
#include <complex>
#include <cmath>
#include <unordered_map>

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

// Generate a sorted vector of the specified size
std::vector<int> generateSortedVector(int size, int min = 0, int max = 1000)
{
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (int i = 0; i < size; i++)
    {
        vec[i] = dis(gen);
    }

    std::sort(vec.begin(), vec.end());
    return vec;
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

// Print a 2D matrix
void printMatrix(const std::vector<std::vector<int>> &matrix, std::string name = "Matrix")
{
    std::cout << name << " (" << matrix.size() << "x" << (matrix.empty() ? 0 : matrix[0].size()) << "):" << std::endl;

    for (const auto &row : matrix)
    {
        std::cout << "  [";
        for (size_t i = 0; i < row.size(); i++)
        {
            std::cout << row[i];
            if (i < row.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// ===== BINARY SEARCH IMPLEMENTATIONS =====

// Recursive binary search (classic divide and conquer approach)
int binarySearchRecursive(const std::vector<int> &arr, int target, int left, int right)
{
    // Base case: element not found
    if (left > right)
        return -1;

    // Find the middle index
    int mid = left + (right - left) / 2;

    // Check if the middle element is the target
    if (arr[mid] == target)
        return mid;

    // If the target is less than the middle element, search the left half
    if (arr[mid] > target)
        return binarySearchRecursive(arr, target, left, mid - 1);

    // If the target is greater than the middle element, search the right half
    return binarySearchRecursive(arr, target, mid + 1, right);
}

// Wrapper function for binary search
int binarySearch(const std::vector<int> &arr, int target)
{
    return binarySearchRecursive(arr, target, 0, arr.size() - 1);
}

// Iterative binary search
int binarySearchIterative(const std::vector<int> &arr, int target)
{
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return mid;
        else if (arr[mid] > target)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return -1;
}

// ===== MAXIMUM SUBARRAY IMPLEMENTATIONS =====

// Brute force approach (for comparison)
int maxSubArrayBruteForce(const std::vector<int> &nums)
{
    int n = nums.size();
    int maxSum = INT_MIN;

    for (int i = 0; i < n; i++)
    {
        int currentSum = 0;
        for (int j = i; j < n; j++)
        {
            currentSum += nums[j];
            maxSum = std::max(maxSum, currentSum);
        }
    }

    return maxSum;
}

// Kadane's algorithm (for comparison)
int maxSubArrayKadane(const std::vector<int> &nums)
{
    int maxSoFar = nums[0];
    int maxEndingHere = nums[0];

    for (int i = 1; i < nums.size(); i++)
    {
        maxEndingHere = std::max(nums[i], maxEndingHere + nums[i]);
        maxSoFar = std::max(maxSoFar, maxEndingHere);
    }

    return maxSoFar;
}

// Helper function for divide and conquer approach to maximum subarray
int maxCrossingSum(const std::vector<int> &nums, int left, int mid, int right)
{
    // Find maximum sum starting from mid and going left
    int leftSum = 0;
    int maxLeftSum = INT_MIN;
    for (int i = mid; i >= left; i--)
    {
        leftSum += nums[i];
        maxLeftSum = std::max(maxLeftSum, leftSum);
    }

    // Find maximum sum starting from mid+1 and going right
    int rightSum = 0;
    int maxRightSum = INT_MIN;
    for (int i = mid + 1; i <= right; i++)
    {
        rightSum += nums[i];
        maxRightSum = std::max(maxRightSum, rightSum);
    }

    // Return sum of the two halves
    return maxLeftSum + maxRightSum;
}

// Divide and conquer approach to maximum subarray
int maxSubArrayDivideConquer(const std::vector<int> &nums, int left, int right)
{
    // Base case: single element
    if (left == right)
        return nums[left];

    int mid = left + (right - left) / 2;

    // Find maximum subarray sum in left half
    int leftMax = maxSubArrayDivideConquer(nums, left, mid);

    // Find maximum subarray sum in right half
    int rightMax = maxSubArrayDivideConquer(nums, mid + 1, right);

    // Find maximum subarray sum that crosses the midpoint
    int crossMax = maxCrossingSum(nums, left, mid, right);

    // Return the maximum of the three
    return std::max({leftMax, rightMax, crossMax});
}

// Wrapper function for maximum subarray
int maxSubArray(const std::vector<int> &nums)
{
    return maxSubArrayDivideConquer(nums, 0, nums.size() - 1);
}

// ===== MERGE SORT (RECAP) =====

// Merge function for merge sort
void merge(std::vector<int> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    std::vector<int> leftArray(n1);
    std::vector<int> rightArray(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArray[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArray[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (leftArray[i] <= rightArray[j])
            arr[k++] = leftArray[i++];
        else
            arr[k++] = rightArray[j++];
    }

    // Copy the remaining elements of leftArray, if any
    while (i < n1)
        arr[k++] = leftArray[i++];

    // Copy the remaining elements of rightArray, if any
    while (j < n2)
        arr[k++] = rightArray[j++];
}

// Merge sort function
void mergeSort(std::vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Wrapper function for merge sort
void mergeSort(std::vector<int> &arr)
{
    if (arr.size() <= 1)
        return;
    mergeSort(arr, 0, arr.size() - 1);
}

// ===== QUICK SORT (RECAP) =====

// Lomuto partition scheme for quick sort
int partition(std::vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick sort function
void quickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

// Wrapper function for quick sort
void quickSort(std::vector<int> &arr)
{
    if (arr.size() <= 1)
        return;
    quickSort(arr, 0, arr.size() - 1);
}

// ===== QUICK SELECT IMPLEMENTATION =====

// Quick select to find the kth smallest element
int quickSelect(std::vector<int> &arr, int left, int right, int k)
{
    if (left == right)
        return arr[left];

    // Choose a random pivot to avoid worst-case scenario
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(left, right);
    int pivotIndex = dis(gen);

    // Swap pivot with right
    std::swap(arr[pivotIndex], arr[right]);
    pivotIndex = right;

    // Partition the array
    int pivot = arr[pivotIndex];
    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[right]);
    pivotIndex = i + 1;

    // Check if we found the kth element
    if (k == pivotIndex)
        return arr[k];
    else if (k < pivotIndex)
        return quickSelect(arr, left, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, right, k);
}

// Wrapper function for quick select
int findKthSmallest(std::vector<int> &arr, int k)
{
    if (k < 1 || k > arr.size())
        throw std::invalid_argument("k is out of bounds");

    // Convert to 0-indexed
    return quickSelect(arr, 0, arr.size() - 1, k - 1);
}

// Find kth largest element using quick select
int findKthLargest(std::vector<int> &arr, int k)
{
    if (k < 1 || k > arr.size())
        throw std::invalid_argument("k is out of bounds");

    // Convert to 0-indexed and find the (n-k)th smallest
    return quickSelect(arr, 0, arr.size() - 1, arr.size() - k);
}

// ===== COUNTING INVERSIONS IMPLEMENTATION =====

// Helper function to count inversions during merge
long long mergAndCountInversions(std::vector<int> &arr, std::vector<int> &temp, int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = left;
    long long inversions = 0;

    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            // Inversion found: all elements from i to mid are greater than arr[j]
            inversions += (mid - i + 1);
            temp[k++] = arr[j++];
        }
    }

    // Copy the remaining elements
    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    // Copy back to original array
    for (i = left; i <= right; i++)
        arr[i] = temp[i];

    return inversions;
}

// Count inversions using divide and conquer
long long countInversionsDivideConquer(std::vector<int> &arr, std::vector<int> &temp, int left, int right)
{
    long long inversions = 0;

    if (left < right)
    {
        int mid = left + (right - left) / 2;

        // Count inversions in left half
        inversions += countInversionsDivideConquer(arr, temp, left, mid);

        // Count inversions in right half
        inversions += countInversionsDivideConquer(arr, temp, mid + 1, right);

        // Count inversions across the two halves
        inversions += mergAndCountInversions(arr, temp, left, mid, right);
    }

    return inversions;
}

// Wrapper function to count inversions
long long countInversions(std::vector<int> arr)
{
    std::vector<int> temp(arr.size());
    return countInversionsDivideConquer(arr, temp, 0, arr.size() - 1);
}

// ===== MAJORITY ELEMENT IMPLEMENTATION =====

// Majority element using divide and conquer
int majorityElementDivideConquer(const std::vector<int> &nums, int left, int right)
{
    // Base case: single element
    if (left == right)
        return nums[left];

    // Divide the array into two halves
    int mid = left + (right - left) / 2;
    int leftMajority = majorityElementDivideConquer(nums, left, mid);
    int rightMajority = majorityElementDivideConquer(nums, mid + 1, right);

    // If both halves have the same majority element
    if (leftMajority == rightMajority)
        return leftMajority;

    // Count occurrences of the two candidates
    int leftCount = std::count(nums.begin() + left, nums.begin() + right + 1, leftMajority);
    int rightCount = std::count(nums.begin() + left, nums.begin() + right + 1, rightMajority);

    // Return the element with more occurrences
    return leftCount > rightCount ? leftMajority : rightMajority;
}

// Wrapper function for majority element
int majorityElement(const std::vector<int> &nums)
{
    return majorityElementDivideConquer(nums, 0, nums.size() - 1);
}

// ===== MATRIX MULTIPLICATION IMPLEMENTATIONS =====

// Standard matrix multiplication (for comparison)
std::vector<std::vector<int>> standardMatrixMultiply(
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B)
{

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Helper functions for Strassen's algorithm
std::vector<std::vector<int>> matrixAdd(
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B)
{

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}

std::vector<std::vector<int>> matrixSubtract(
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B)
{

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }

    return C;
}

// Strassen's matrix multiplication algorithm
std::vector<std::vector<int>> strassenMultiply(
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B)
{

    int n = A.size();

    // Base case
    if (n <= 64)
    {
        // For small matrices, use standard multiplication
        return standardMatrixMultiply(A, B);
    }

    int newSize = n / 2;
    std::vector<std::vector<int>> A11(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A12(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A21(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A22(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B11(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B12(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B21(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B22(newSize, std::vector<int>(newSize));

    // Divide matrices into quarters
    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Compute the seven products using divide and conquer
    auto M1 = strassenMultiply(matrixAdd(A11, A22), matrixAdd(B11, B22));
    auto M2 = strassenMultiply(matrixAdd(A21, A22), B11);
    auto M3 = strassenMultiply(A11, matrixSubtract(B12, B22));
    auto M4 = strassenMultiply(A22, matrixSubtract(B21, B11));
    auto M5 = strassenMultiply(matrixAdd(A11, A12), B22);
    auto M6 = strassenMultiply(matrixSubtract(A21, A11), matrixAdd(B11, B12));
    auto M7 = strassenMultiply(matrixSubtract(A12, A22), matrixAdd(B21, B22));

    // Combine results to get the four quarters of the result matrix
    auto C11 = matrixAdd(matrixSubtract(matrixAdd(M1, M4), M5), M7);
    auto C12 = matrixAdd(M3, M5);
    auto C21 = matrixAdd(M2, M4);
    auto C22 = matrixAdd(matrixSubtract(matrixAdd(M1, M3), M2), M6);

    // Combine the four quarters into a single result matrix
    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    return C;
}

// Generate a square matrix of size n with random values
std::vector<std::vector<int>> generateRandomMatrix(int n, int min = 0, int max = 10)
{
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// ===== CLOSEST PAIR OF POINTS IMPLEMENTATION =====

// Structure to represent a point
struct Point
{
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    double distanceSquared(const Point &p) const
    {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }

    double distance(const Point &p) const
    {
        return std::sqrt(distanceSquared(p));
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

// Comparator for sorting points by x-coordinate
bool compareX(const Point &a, const Point &b)
{
    return a.x < b.x;
}

// Comparator for sorting points by y-coordinate
bool compareY(const Point &a, const Point &b)
{
    return a.y < b.y;
}

// Find the closest pair of points using the brute force method (for small sets)
std::pair<Point, Point> bruteForceClosestPair(const std::vector<Point> &points)
{
    int n = points.size();
    double minDist = DBL_MAX;
    std::pair<Point, Point> closestPair;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double dist = points[i].distanceSquared(points[j]);
            if (dist < minDist)
            {
                minDist = dist;
                closestPair = {points[i], points[j]};
            }
        }
    }

    return closestPair;
}

// Find closest pair of points using divide and conquer
std::pair<Point, Point> closestPairDivideConquer(
    std::vector<Point> &pointsX,
    std::vector<Point> &pointsY)
{

    int n = pointsX.size();

    // Base case: use brute force for small sets
    if (n <= 3)
    {
        return bruteForceClosestPair(pointsX);
    }

    // Divide the points into left and right halves
    int mid = n / 2;
    Point midPoint = pointsX[mid];

    std::vector<Point> leftX(pointsX.begin(), pointsX.begin() + mid);
    std::vector<Point> rightX(pointsX.begin() + mid, pointsX.end());

    std::vector<Point> leftY, rightY;
    for (const Point &p : pointsY)
    {
        if (p.x <= midPoint.x)
        {
            leftY.push_back(p);
        }
        else
        {
            rightY.push_back(p);
        }
    }

    // Recursively find the closest pairs in each half
    std::pair<Point, Point> leftPair = closestPairDivideConquer(leftX, leftY);
    std::pair<Point, Point> rightPair = closestPairDivideConquer(rightX, rightY);

    // Find the minimum distance from each half
    double leftDist = leftPair.first.distanceSquared(leftPair.second);
    double rightDist = rightPair.first.distanceSquared(rightPair.second);
    double minDist = std::min(leftDist, rightDist);
    std::pair<Point, Point> closestPair = (leftDist <= rightDist) ? leftPair : rightPair;

    // Find points in the strip around the middle line
    std::vector<Point> strip;
    for (const Point &p : pointsY)
    {
        if (std::abs(p.x - midPoint.x) < std::sqrt(minDist))
        {
            strip.push_back(p);
        }
    }

    // Check for closer points in the strip
    for (int i = 0; i < strip.size(); i++)
    {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < std::sqrt(minDist); j++)
        {
            double dist = strip[i].distanceSquared(strip[j]);
            if (dist < minDist)
            {
                minDist = dist;
                closestPair = {strip[i], strip[j]};
            }
        }
    }

    return closestPair;
}

// Wrapper function to find the closest pair of points
std::pair<Point, Point> findClosestPair(std::vector<Point> points)
{
    if (points.size() <= 1)
    {
        throw std::invalid_argument("At least 2 points are required");
    }

    // Sort points by x and y coordinates
    std::vector<Point> pointsX = points;
    std::sort(pointsX.begin(), pointsX.end(), compareX);

    std::vector<Point> pointsY = points;
    std::sort(pointsY.begin(), pointsY.end(), compareY);

    return closestPairDivideConquer(pointsX, pointsY);
}

// Generate a vector of random points
std::vector<Point> generateRandomPoints(int n, double minCoord = 0.0, double maxCoord = 100.0)
{
    std::vector<Point> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(minCoord, maxCoord);

    for (int i = 0; i < n; i++)
    {
        points.push_back(Point(dis(gen), dis(gen)));
    }

    return points;
}

// ===== KARATSUBA INTEGER MULTIPLICATION IMPLEMENTATION =====

// Standard integer multiplication (for comparison)
long long standardMultiply(long long x, long long y)
{
    return x * y;
}

// Karatsuba algorithm for integer multiplication
long long karatsuba(long long x, long long y)
{
    // Base case
    if (x < 10 || y < 10)
    {
        return x * y;
    }

    // Calculate the number of digits
    long long n = std::max(
        (long long)std::to_string(x).length(),
        (long long)std::to_string(y).length());
    long long half = n / 2;

    // Split the digits
    long long power = (long long)std::pow(10, half);
    long long a = x / power;
    long long b = x % power;
    long long c = y / power;
    long long d = y % power;

    // Recursive steps (only 3 multiplications instead of 4)
    long long ac = karatsuba(a, c);
    long long bd = karatsuba(b, d);
    long long abcd = karatsuba(a + b, c + d) - ac - bd;

    // Combine results
    return ac * (long long)std::pow(10, 2 * half) + abcd * power + bd;
}

// ===== FAST POWER ALGORITHM IMPLEMENTATION =====

// Compute x^n using divide and conquer
long long fastPower(long long x, long long n)
{
    if (n == 0)
        return 1;

    // Handle negative exponents
    if (n < 0)
    {
        x = 1 / x;
        n = -n;
    }

    // Recursive case: x^n = (x^(n/2))^2 if n is even, or x * (x^(n/2))^2 if n is odd
    long long half = fastPower(x, n / 2);

    if (n % 2 == 0)
    {
        return half * half;
    }
    else
    {
        return x * half * half;
    }
}

// ===== TEST AND VISUALIZATION FUNCTIONS =====

// Test binary search implementations
void testBinarySearch()
{
    std::cout << "\n===== BINARY SEARCH TESTS =====" << std::endl;

    // Generate a sorted vector
    std::vector<int> sortedArr = generateSortedVector(1000);
    printVector(sortedArr, "Sorted Array", 10);

    // Choose some elements to search for
    int existingElement = sortedArr[sortedArr.size() / 2];
    int nonExistingElement = -1;

    std::cout << "\nSearching for " << existingElement << " (should exist) and "
              << nonExistingElement << " (should not exist):" << std::endl;

    // Test recursive binary search
    {
        auto [resultRecur1, timeRecur1] = measureExecutionTime(binarySearch, sortedArr, existingElement);
        auto [resultRecur2, timeRecur2] = measureExecutionTime(binarySearch, sortedArr, nonExistingElement);

        std::cout << "Recursive Binary Search:" << std::endl;
        std::cout << "  Found " << existingElement << " at index " << resultRecur1
                  << " in " << timeRecur1 << " microseconds" << std::endl;
        std::cout << "  Found " << nonExistingElement << " at index " << resultRecur2
                  << " in " << timeRecur2 << " microseconds" << std::endl;
    }

    // Test iterative binary search
    {
        auto [resultIter1, timeIter1] = measureExecutionTime(binarySearchIterative, sortedArr, existingElement);
        auto [resultIter2, timeIter2] = measureExecutionTime(binarySearchIterative, sortedArr, nonExistingElement);

        std::cout << "Iterative Binary Search:" << std::endl;
        std::cout << "  Found " << existingElement << " at index " << resultIter1
                  << " in " << timeIter1 << " microseconds" << std::endl;
        std::cout << "  Found " << nonExistingElement << " at index " << resultIter2
                  << " in " << timeIter2 << " microseconds" << std::endl;
    }

    // Performance comparison for different array sizes
    std::cout << "\nPerformance comparison for different array sizes (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Array Size"
              << std::setw(18) << "Recursive Search"
              << std::setw(18) << "Iterative Search" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> sizes = {100, 1000, 10000, 100000, 1000000};
    for (int size : sizes)
    {
        std::vector<int> arr = generateSortedVector(size);
        int target = arr[size / 2]; // Middle element

        auto [_, recursiveTime] = measureExecutionTime(binarySearch, arr, target);
        auto [__, iterativeTime] = measureExecutionTime(binarySearchIterative, arr, target);

        std::cout << std::left << std::setw(12) << size
                  << std::setw(18) << recursiveTime
                  << std::setw(18) << iterativeTime << std::endl;
    }
}

// Test maximum subarray implementations
void testMaximumSubarray()
{
    std::cout << "\n===== MAXIMUM SUBARRAY TESTS =====" << std::endl;

    // Test cases
    std::vector<std::vector<int>> testCases = {
        {-2, 1, -3, 4, -1, 2, 1, -5, 4},
        {5, 4, -1, 7, 8},
        {-1},
        {-2, -1, -3, -4, -1, -2, -1, -5, -4},
        {1, 2, 3, 4, 5}};

    for (int i = 0; i < testCases.size(); i++)
    {
        std::cout << "\nTest Case " << (i + 1) << ": ";
        printVector(testCases[i], "", testCases[i].size());

        // Get results from different algorithms
        auto [resultBrute, timeBrute] = measureExecutionTime(maxSubArrayBruteForce, testCases[i]);
        auto [resultKadane, timeKadane] = measureExecutionTime(maxSubArrayKadane, testCases[i]);
        auto [resultDivide, timeDivide] = measureExecutionTime(maxSubArray, testCases[i]);

        std::cout << "Results:" << std::endl;
        std::cout << "  Brute Force: " << resultBrute << " (in " << timeBrute << " microseconds)" << std::endl;
        std::cout << "  Kadane's Algorithm: " << resultKadane << " (in " << timeKadane << " microseconds)" << std::endl;
        std::cout << "  Divide and Conquer: " << resultDivide << " (in " << timeDivide << " microseconds)" << std::endl;

        // Verify results match
        assert(resultBrute == resultKadane && resultKadane == resultDivide);
    }

    // Performance comparison for larger arrays
    std::cout << "\nPerformance comparison for larger arrays (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Array Size"
              << std::setw(18) << "Brute Force"
              << std::setw(18) << "Kadane's"
              << std::setw(18) << "Divide & Conquer" << std::endl;
    std::cout << std::string(66, '-') << std::endl;

    std::vector<int> sizes = {100, 1000, 5000, 10000};
    for (int size : sizes)
    {
        std::vector<int> arr = generateRandomVector(size, -100, 100);

        auto [_, bruteTime] = measureExecutionTime(maxSubArrayBruteForce, arr);
        auto [__, kadaneTime] = measureExecutionTime(maxSubArrayKadane, arr);
        auto [___, divideTime] = measureExecutionTime(maxSubArray, arr);

        std::cout << std::left << std::setw(12) << size
                  << std::setw(18) << bruteTime
                  << std::setw(18) << kadaneTime
                  << std::setw(18) << divideTime << std::endl;
    }
}

// Test quick select implementation
void testQuickSelect()
{
    std::cout << "\n===== QUICK SELECT TESTS =====" << std::endl;

    // Generate a random array
    std::vector<int> arr = generateRandomVector(20, 1, 100);
    printVector(arr, "Original Array", arr.size());

    // Make a sorted copy for verification
    std::vector<int> sortedArr = arr;
    std::sort(sortedArr.begin(), sortedArr.end());
    printVector(sortedArr, "Sorted Array", sortedArr.size());

    // Find different order statistics
    std::cout << "\nFinding order statistics:" << std::endl;

    for (int k = 1; k <= arr.size(); k += 4)
    {
        std::vector<int> arrCopy = arr;
        int result = findKthSmallest(arrCopy, k);
        std::cout << "  " << k << "-th smallest element: " << result
                  << " (should be " << sortedArr[k - 1] << ")" << std::endl;
        assert(result == sortedArr[k - 1]);
    }

    // Find kth largest elements
    std::cout << "\nFinding kth largest elements:" << std::endl;

    for (int k = 1; k <= arr.size(); k += 4)
    {
        std::vector<int> arrCopy = arr;
        int result = findKthLargest(arrCopy, k);
        std::cout << "  " << k << "-th largest element: " << result
                  << " (should be " << sortedArr[arr.size() - k] << ")" << std::endl;
        assert(result == sortedArr[arr.size() - k]);
    }

    // Performance comparison against sorting and selecting
    std::cout << "\nPerformance comparison (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Array Size"
              << std::setw(18) << "Quick Select"
              << std::setw(18) << "Sort & Select" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> sizes = {100, 1000, 10000, 100000};
    for (int size : sizes)
    {
        std::vector<int> largeArr = generateRandomVector(size);
        int k = size / 2; // Middle element

        // Quick select approach
        {
            std::vector<int> copy = largeArr;
            auto [_, quickTime] = measureExecutionTime(findKthSmallest, copy, k);
            std::cout << std::left << std::setw(12) << size
                      << std::setw(18) << quickTime;
        }

        // Sort and select approach
        {
            std::vector<int> copy = largeArr;
            auto sortAndSelect = [&copy, k]()
            {
                std::sort(copy.begin(), copy.end());
                return copy[k - 1];
            };
            auto [_, sortTime] = measureExecutionTime(sortAndSelect);
            std::cout << std::setw(18) << sortTime << std::endl;
        }
    }
}

// Test count inversions implementation
void testCountInversions()
{
    std::cout << "\n===== COUNT INVERSIONS TESTS =====" << std::endl;

    // Test cases
    std::vector<std::vector<int>> testCases = {
        {1, 2, 3, 4, 5},         // Sorted (0 inversions)
        {5, 4, 3, 2, 1},         // Reverse sorted (10 inversions)
        {2, 4, 1, 3, 5},         // Some inversions
        {5, 1, 8, 2, 3, 4, 7, 6} // Random order
    };

    for (int i = 0; i < testCases.size(); i++)
    {
        std::cout << "\nTest Case " << (i + 1) << ": ";
        printVector(testCases[i], "", testCases[i].size());

        auto [inversions, time] = measureExecutionTime(countInversions, testCases[i]);

        std::cout << "  Number of inversions: " << inversions << " (in " << time << " microseconds)" << std::endl;
    }

    // Performance comparison for different array sizes
    std::cout << "\nPerformance comparison for different array sizes:" << std::endl;
    std::cout << std::left << std::setw(20) << "Array Type"
              << std::setw(15) << "Size"
              << std::setw(25) << "Inversions"
              << std::setw(20) << "Time (microseconds)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    std::vector<int> sizes = {100, 1000, 10000};

    for (int size : sizes)
    {
        // Sorted array (0 inversions)
        {
            std::vector<int> arr(size);
            for (int i = 0; i < size; i++)
                arr[i] = i;

            auto [inversions, time] = measureExecutionTime(countInversions, arr);

            std::cout << std::left << std::setw(20) << "Sorted"
                      << std::setw(15) << size
                      << std::setw(25) << inversions
                      << std::setw(20) << time << std::endl;
        }

        // Reverse sorted array (n*(n-1)/2 inversions)
        {
            std::vector<int> arr(size);
            for (int i = 0; i < size; i++)
                arr[i] = size - i - 1;

            auto [inversions, time] = measureExecutionTime(countInversions, arr);

            std::cout << std::left << std::setw(20) << "Reverse Sorted"
                      << std::setw(15) << size
                      << std::setw(25) << inversions
                      << std::setw(20) << time << std::endl;
        }

        // Random array (expected ~n^2/4 inversions)
        {
            std::vector<int> arr = generateRandomVector(size);

            auto [inversions, time] = measureExecutionTime(countInversions, arr);

            std::cout << std::left << std::setw(20) << "Random"
                      << std::setw(15) << size
                      << std::setw(25) << inversions
                      << std::setw(20) << time << std::endl;
        }
    }
}

// Test matrix multiplication implementations
void testMatrixMultiplication()
{
    std::cout << "\n===== MATRIX MULTIPLICATION TESTS =====" << std::endl;

    // Test with small matrices
    {
        int n = 4;
        auto A = generateRandomMatrix(n);
        auto B = generateRandomMatrix(n);

        std::cout << "\nTesting with " << n << "x" << n << " matrices:" << std::endl;

        printMatrix(A, "Matrix A");
        printMatrix(B, "Matrix B");

        // Standard multiplication
        auto [resultStandard, timeStandard] = measureExecutionTime(standardMatrixMultiply, A, B);

        // Strassen's algorithm
        auto [resultStrassen, timeStrassen] = measureExecutionTime(strassenMultiply, A, B);

        printMatrix(resultStandard, "Result (Standard)");
        // printMatrix(resultStrassen, "Result (Strassen)");

        std::cout << "Time comparison:" << std::endl;
        std::cout << "  Standard multiplication: " << timeStandard << " microseconds" << std::endl;
        std::cout << "  Strassen's algorithm: " << timeStrassen << " microseconds" << std::endl;

        // Verify results match
        bool match = true;
        for (int i = 0; i < n && match; i++)
        {
            for (int j = 0; j < n && match; j++)
            {
                if (resultStandard[i][j] != resultStrassen[i][j])
                {
                    match = false;
                }
            }
        }
        std::cout << "Results match: " << (match ? "Yes" : "No") << std::endl;
    }

    // Performance comparison for different matrix sizes
    std::cout << "\nPerformance comparison for different matrix sizes (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Matrix Size"
              << std::setw(18) << "Standard"
              << std::setw(18) << "Strassen" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> sizes = {16, 32, 64, 128, 256};
    for (int size : sizes)
    {
        auto A = generateRandomMatrix(size);
        auto B = generateRandomMatrix(size);

        auto [_, standardTime] = measureExecutionTime(standardMatrixMultiply, A, B);
        auto [__, strassenTime] = measureExecutionTime(strassenMultiply, A, B);

        std::cout << std::left << std::setw(12) << size
                  << std::setw(18) << standardTime
                  << std::setw(18) << strassenTime << std::endl;
    }
}

// Test closest pair of points implementation
void testClosestPair()
{
    std::cout << "\n===== CLOSEST PAIR OF POINTS TESTS =====" << std::endl;

    // Generate a small set of random points
    int n = 10;
    std::vector<Point> points = generateRandomPoints(n);

    std::cout << "\nTesting with " << n << " points:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << "  Point " << (i + 1) << ": " << points[i] << std::endl;
    }

    // Find closest pair using brute force
    auto [p1Brute, p2Brute] = bruteForceClosestPair(points);
    double distBrute = p1Brute.distance(p2Brute);

    std::cout << "\nBrute Force Result:" << std::endl;
    std::cout << "  Closest pair: " << p1Brute << " and " << p2Brute << std::endl;
    std::cout << "  Distance: " << distBrute << std::endl;

    // Find closest pair using divide and conquer
    auto [p1DivConq, p2DivConq] = findClosestPair(points);
    double distDivConq = p1DivConq.distance(p2DivConq);

    std::cout << "\nDivide and Conquer Result:" << std::endl;
    std::cout << "  Closest pair: " << p1DivConq << " and " << p2DivConq << std::endl;
    std::cout << "  Distance: " << distDivConq << std::endl;

    // Verify results match (within a small epsilon for floating point comparison)
    const double epsilon = 1e-9;
    bool distMatch = std::abs(distBrute - distDivConq) < epsilon;
    std::cout << "\nResults match: " << (distMatch ? "Yes" : "No") << std::endl;

    // Performance comparison for different point set sizes
    std::cout << "\nPerformance comparison for different point set sizes (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Number of Points"
              << std::setw(18) << "Brute Force"
              << std::setw(18) << "Divide & Conquer" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> sizes = {10, 100, 1000, 10000};
    for (int size : sizes)
    {
        auto points = generateRandomPoints(size);

        // Skip brute force for large sets due to O(n²) complexity
        if (size <= 1000)
        {
            auto [_, bruteTime] = measureExecutionTime(bruteForceClosestPair, points);
            std::cout << std::left << std::setw(12) << size
                      << std::setw(18) << bruteTime;
        }
        else
        {
            std::cout << std::left << std::setw(12) << size
                      << std::setw(18) << "N/A (too slow)";
        }

        auto [__, divConqTime] = measureExecutionTime(findClosestPair, points);
        std::cout << std::setw(18) << divConqTime << std::endl;
    }
}

// Test Karatsuba multiplication implementation
void testKaratsubaMultiplication()
{
    std::cout << "\n===== KARATSUBA MULTIPLICATION TESTS =====" << std::endl;

    // Test cases
    std::vector<std::pair<long long, long long>> testCases = {
        {12345, 6789},
        {1234567890, 9876543210},
        {9999, 9999},
        {123, 456}};

    for (const auto &[a, b] : testCases)
    {
        std::cout << "\nMultiplying " << a << " * " << b << ":" << std::endl;

        // Standard multiplication
        auto [resultStandard, timeStandard] = measureExecutionTime(standardMultiply, a, b);

        // Karatsuba algorithm
        auto [resultKaratsuba, timeKaratsuba] = measureExecutionTime(karatsuba, a, b);

        std::cout << "  Standard: " << resultStandard << " (in " << timeStandard << " microseconds)" << std::endl;
        std::cout << "  Karatsuba: " << resultKaratsuba << " (in " << timeKaratsuba << " microseconds)" << std::endl;
        std::cout << "  Results match: " << (resultStandard == resultKaratsuba ? "Yes" : "No") << std::endl;
    }

    // Performance comparison for larger numbers
    std::cout << "\nPerformance comparison for larger numbers (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Digits"
              << std::setw(18) << "Standard"
              << std::setw(18) << "Karatsuba" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> digits = {4, 8, 12, 16};
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int d : digits)
    {
        // Generate random numbers with d digits
        std::uniform_int_distribution<long long> dis(
            (long long)std::pow(10, d - 1),
            (long long)std::pow(10, d) - 1);
        long long a = dis(gen);
        long long b = dis(gen);

        auto [_, standardTime] = measureExecutionTime(standardMultiply, a, b);
        auto [__, karatsubaTime] = measureExecutionTime(karatsuba, a, b);

        std::cout << std::left << std::setw(12) << d
                  << std::setw(18) << standardTime
                  << std::setw(18) << karatsubaTime << std::endl;
    }
}

// Test fast power implementation
void testFastPower()
{
    std::cout << "\n===== FAST POWER TESTS =====" << std::endl;

    // Test cases
    std::vector<std::pair<long long, long long>> testCases = {
        {2, 10},  // 2^10 = 1024
        {3, 5},   // 3^5 = 243
        {5, 3},   // 5^3 = 125
        {10, 0},  // 10^0 = 1
        {2, 20},  // 2^20 = 1048576
        {1, 1000} // 1^1000 = 1
    };

    for (const auto &[base, exponent] : testCases)
    {
        std::cout << "\nCalculating " << base << "^" << exponent << ":" << std::endl;

        // Standard method
        auto standardPower = [](long long x, long long n)
        {
            long long result = 1;
            for (long long i = 0; i < n; i++)
            {
                result *= x;
            }
            return result;
        };

        // Skip standard method for large exponents
        if (exponent <= 30)
        {
            auto [resultStandard, timeStandard] = measureExecutionTime(standardPower, base, exponent);
            std::cout << "  Standard: " << resultStandard << " (in " << timeStandard << " microseconds)" << std::endl;

            auto [resultFast, timeFast] = measureExecutionTime(fastPower, base, exponent);
            std::cout << "  Fast Power: " << resultFast << " (in " << timeFast << " microseconds)" << std::endl;
            std::cout << "  Results match: " << (resultStandard == resultFast ? "Yes" : "No") << std::endl;
        }
        else
        {
            auto [resultFast, timeFast] = measureExecutionTime(fastPower, base, exponent);
            std::cout << "  Fast Power: " << resultFast << " (in " << timeFast << " microseconds)" << std::endl;
            std::cout << "  Standard method skipped (exponent too large)" << std::endl;
        }
    }

    // Performance comparison for different exponents
    std::cout << "\nPerformance comparison for different exponents (time in microseconds):" << std::endl;
    std::cout << std::left << std::setw(12) << "Exponent"
              << std::setw(18) << "Standard"
              << std::setw(18) << "Fast Power" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    std::vector<int> exponents = {10, 100, 1000, 10000, 100000};

    for (int e : exponents)
    {
        // Use base 2 for simplicity
        long long base = 2;

        // Standard method (only for small exponents)
        if (e <= 20)
        {
            auto standardPower = [](long long x, long long n)
            {
                long long result = 1;
                for (long long i = 0; i < n; i++)
                {
                    result *= x;
                }
                return result;
            };

            auto [_, standardTime] = measureExecutionTime(standardPower, base, e);
            std::cout << std::left << std::setw(12) << e
                      << std::setw(18) << standardTime;
        }
        else
        {
            std::cout << std::left << std::setw(12) << e
                      << std::setw(18) << "N/A (too slow)";
        }

        auto [__, fastTime] = measureExecutionTime(fastPower, base, e);
        std::cout << std::setw(18) << fastTime << std::endl;
    }
}

// ===== LEETCODE-STYLE PROBLEMS =====

// Power of Two (LeetCode #231)
bool isPowerOfTwo(int n)
{
    if (n <= 0)
        return false;

    // A power of two has only one bit set in its binary representation
    return (n & (n - 1)) == 0;
}

// Majority Element (LeetCode #169)
int majorityElementBoyer(std::vector<int> &nums)
{
    int count = 0;
    int candidate = 0;

    for (int num : nums)
    {
        if (count == 0)
        {
            candidate = num;
        }

        count += (num == candidate) ? 1 : -1;
    }

    return candidate;
}

// Search a 2D Matrix II (LeetCode #240)
bool searchMatrix(std::vector<std::vector<int>> &matrix, int target)
{
    if (matrix.empty() || matrix[0].empty())
        return false;

    // Start from the top-right corner
    int row = 0;
    int col = matrix[0].size() - 1;

    while (row < matrix.size() && col >= 0)
    {
        if (matrix[row][col] == target)
        {
            return true;
        }
        else if (matrix[row][col] > target)
        {
            // Current column is too large, move left
            col--;
        }
        else
        {
            // Current row is too small, move down
            row++;
        }
    }

    return false;
}

// Different Ways to Add Parentheses (LeetCode #241)
std::vector<int> diffWaysToCompute(std::string expression)
{
    // Memoization
    static std::unordered_map<std::string, std::vector<int>> memo;

    // Check if result is already computed
    if (memo.find(expression) != memo.end())
    {
        return memo[expression];
    }

    std::vector<int> results;
    bool isNumber = true;

    // Check if the input is just a number
    for (char c : expression)
    {
        if (!isdigit(c))
        {
            isNumber = false;
            break;
        }
    }

    // Base case: just a number
    if (isNumber)
    {
        results.push_back(std::stoi(expression));
        memo[expression] = results;
        return results;
    }

    // Recursive case
    for (int i = 0; i < expression.size(); i++)
    {
        char c = expression[i];
        if (c == '+' || c == '-' || c == '*')
        {
            // Split the expression at this operator
            std::vector<int> leftResults = diffWaysToCompute(expression.substr(0, i));
            std::vector<int> rightResults = diffWaysToCompute(expression.substr(i + 1));

            // Combine the results from left and right
            for (int left : leftResults)
            {
                for (int right : rightResults)
                {
                    if (c == '+')
                        results.push_back(left + right);
                    else if (c == '-')
                        results.push_back(left - right);
                    else if (c == '*')
                        results.push_back(left * right);
                }
            }
        }
    }

    memo[expression] = results;
    return results;
}

// Test LeetCode-style problems
void testLeetcodeProblems()
{
    std::cout << "\n===== LEETCODE-STYLE PROBLEMS =====" << std::endl;

    // Test Power of Two
    std::cout << "\nProblem 1: Power of Two (LeetCode #231)" << std::endl;
    std::vector<int> powerCases = {1, 2, 3, 4, 8, 16, 17, 32, 64, 1024, 1025};

    for (int n : powerCases)
    {
        bool result = isPowerOfTwo(n);
        std::cout << "  " << n << " is " << (result ? "" : "NOT ") << "a power of two" << std::endl;
    }

    // Test Majority Element
    std::cout << "\nProblem 2: Majority Element (LeetCode #169)" << std::endl;
    std::vector<std::vector<int>> majorityTestCases = {
        {3, 2, 3},
        {2, 2, 1, 1, 1, 2, 2},
        {1},
        {1, 2, 3, 3, 3, 3, 4, 5}};

    for (auto &nums : majorityTestCases)
    {
        printVector(nums, "Input", nums.size());

        // Test both implementations
        int resultBoyer = majorityElementBoyer(nums);
        int resultDivConq = majorityElement(nums);

        std::cout << "  Boyer-Moore: " << resultBoyer << std::endl;
        std::cout << "  Divide & Conquer: " << resultDivConq << std::endl;
        std::cout << "  Results match: " << (resultBoyer == resultDivConq ? "Yes" : "No") << std::endl;
    }

    // Test Search a 2D Matrix II
    std::cout << "\nProblem 3: Search a 2D Matrix II (LeetCode #240)" << std::endl;
    std::vector<std::vector<int>> matrix = {
        {1, 4, 7, 11, 15},
        {2, 5, 8, 12, 19},
        {3, 6, 9, 16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}};

    std::cout << "Matrix:" << std::endl;
    printMatrix(matrix);

    std::vector<int> targets = {5, 20};
    for (int target : targets)
    {
        bool result = searchMatrix(matrix, target);
        std::cout << "  Target " << target << " is " << (result ? "" : "NOT ") << "in the matrix" << std::endl;
    }

    // Test Different Ways to Add Parentheses
    std::cout << "\nProblem 4: Different Ways to Add Parentheses (LeetCode #241)" << std::endl;
    std::vector<std::string> expressionCases = {
        "2-1-1",
        "2*3-4*5"};

    for (const auto &expr : expressionCases)
    {
        std::cout << "  Expression: " << expr << std::endl;

        auto results = diffWaysToCompute(expr);

        std::cout << "  Possible results: ";
        for (int i = 0; i < results.size(); i++)
        {
            std::cout << results[i];
            if (i < results.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "====================================================" << std::endl;
    std::cout << "DAY 19: DIVIDE AND CONQUER STRATEGY #DSAin45" << std::endl;
    std::cout << "====================================================" << std::endl;

    // Test binary search
    testBinarySearch();

    // Test maximum subarray problem
    testMaximumSubarray();

    // Test quick select
    testQuickSelect();

    // Test count inversions
    testCountInversions();

    // Test matrix multiplication
    testMatrixMultiplication();

    // Test closest pair of points
    testClosestPair();

    // Test Karatsuba multiplication
    testKaratsubaMultiplication();

    // Test fast power
    testFastPower();

    // Test LeetCode-style problems
    testLeetcodeProblems();

    std::cout << "\n===== KEY TAKEAWAYS FROM DIVIDE AND CONQUER =====" << std::endl;
    std::cout << "1. Divide and conquer breaks problems into smaller, similar subproblems" << std::endl;
    std::cout << "2. The three main steps are: Divide, Conquer, and Combine" << std::endl;
    std::cout << "3. This paradigm often improves time complexity, e.g., O(n²) → O(n log n)" << std::endl;
    std::cout << "4. Divide and conquer uses recursion but can sometimes be implemented iteratively" << std::endl;
    std::cout << "5. Recurrence relations and the Master Theorem help analyze these algorithms" << std::endl;
    std::cout << "6. Many fundamental algorithms use divide and conquer:" << std::endl;
    std::cout << "   - Binary Search: O(log n)" << std::endl;
    std::cout << "   - Merge Sort: O(n log n)" << std::endl;
    std::cout << "   - Quick Sort: O(n log n) average case" << std::endl;
    std::cout << "   - Strassen's Algorithm: O(n^2.81) for matrix multiplication" << std::endl;
    std::cout << "   - Closest Pair of Points: O(n log n)" << std::endl;
    std::cout << "   - Karatsuba's Algorithm: O(n^1.58) for integer multiplication" << std::endl;
    std::cout << "7. Divide and conquer can be combined with other techniques like memoization" << std::endl;

    std::cout << "\n====================================================" << std::endl;
    std::cout << "End of Day 19: Divide and Conquer Strategy" << std::endl;
    std::cout << "Next: Day 20 - Dynamic Programming" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;
}