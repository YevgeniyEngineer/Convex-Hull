#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>
#include <vector>

namespace geom
{
template <typename T> struct Point
{
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                  "Point can only be templated with integer or floating-point types");

    T x, y;

    Point(T x, T y) : x(x), y(y)
    {
    }

    bool operator<(const Point &other) const noexcept
    {
        if constexpr (std::is_integral<T>::value)
        {
            return (y < other.y) || ((y == other.y) && (x < other.x));
        }
        else
        {
            return (y < other.y) || (std::fabs(y - other.y) < std::numeric_limits<T>::epsilon() && (x < other.x));
        }
    }

    bool operator==(const Point &other) const noexcept
    {
        if constexpr (std::is_integral<T>::value)
        {
            return (x == other.x) && (y == other.y);
        }
        else
        {
            return (std::fabs(x - other.x) < std::numeric_limits<T>::epsilon()) &&
                   (std::fabs(y - other.y) < std::numeric_limits<T>::epsilon());
        }
    }
};

/// @brief Function to return the cross product of two vectors (p1, p2) and (p1, p3)
template <typename T> inline T crossProduct(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3) noexcept
{
    auto x1 = p2.x - p1.x;
    auto y1 = p2.y - p1.y;
    auto x2 = p3.x - p1.x;
    auto y2 = p3.y - p1.y;
    return x1 * y2 - x2 * y1;
}

/// @brief Helper function to compute the square of the Euclidean distance between two points
template <typename T> inline T squaredDistance(const Point<T> &p1, const Point<T> &p2) noexcept
{
    auto dx = p2.x - p1.x;
    auto dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}

/// @brief Comparator function to sort points by polar angle with respect to the reference point
template <typename T>
inline bool comparePolarAngle(const Point<T> &ref, const Point<T> &p1, const Point<T> &p2) noexcept
{
    int crossProd = crossProduct(ref, p1, p2);
    if (crossProd == 0)
    {
        return squaredDistance(ref, p1) < squaredDistance(ref, p2);
    }
    return (crossProd > 0);
}

/// @brief Class that stores orientation formed by three 2D points
enum class Orientation
{
    COUNTERCLOCKWISE,
    CLOCKWISE,
    COLINEAR
};

/// @brief Function to check if three points form a clockwise or counterclockwise order
template <typename T> Orientation getOrientation(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3) noexcept
{
    auto cross_product = crossProduct(p1, p2, p3);

    if (cross_product > 0)
    {
        return Orientation::COUNTERCLOCKWISE;
    }
    else if (cross_product < 0)
    {
        return Orientation::CLOCKWISE;
    }
    else
    {
        return Orientation::COLINEAR;
    }
}

/// @brief Class that stores available Convex Hull algorithms for selection
enum class ConvexHullAlgorithm
{
    GRAHAM_ANDREW,
    JARVIS_MARCH,
    CHAN
};

/// @brief Return convex hull indices calculated using Graham-Andrew algorithm
template <typename T>
std::vector<int> constructGrahamAndrewConvexHull(const std::vector<Point<T>> &points, Orientation orientation)
{
    int n = static_cast<int>(points.size());
    if (n < 3)
    {
        // Convex Hull is not possible for less than 3 points
        return {};
    }

    // Find point with the lowest y-coordinate (and lowest x coordinate if there is a tie)
    int min_index = 0;
    for (int i = 1; i < n; ++i)
    {
        if (points[i] < points[min_index])
        {
            min_index = i;
        }
    }

    // Swap the first point and the point with the lowest y-coordinate
    std::vector<Point<T>> sorted_points = points;
    std::swap(sorted_points[0], sorted_points[min_index]);

    // Sort the remaining points based on their polar angle with respect to the reference point
    const auto &ref_point = sorted_points[0];
    std::sort(
        sorted_points.begin() + 1, sorted_points.end(),
        [&](const Point<T> &p1, const Point<T> &p2) noexcept -> bool { return comparePolarAngle(ref_point, p1, p2); });

    // Initialize the convex hull with the first three sorted points
    std::vector<int> hull_indices = {0, 1, 2};
    int hull_size = 3;

    // Process remaining points
    for (int i = 3; i < n; ++i)
    {
        // Remove the last point from the hull while it makes a clockwise turn with the next point
        while (hull_size >= 2 &&
               getOrientation(sorted_points[hull_indices[hull_size - 2]], sorted_points[hull_indices[hull_size - 1]],
                              sorted_points[i]) != Orientation::COUNTERCLOCKWISE)
        {
            hull_indices.pop_back();
            --hull_size;
        }

        // Add the next point index to the hull
        hull_indices.push_back(i);
        ++hull_size;
    }

    // Convert the indices of the sorted_points vector back to the original points vector
    std::vector<int> original_indices(hull_size);
    for (int i = 0; i < hull_size; ++i)
    {
        const auto hull_index = hull_indices[i];
        for (int j = 0; j < n; ++j)
        {
            const auto &point = points[j];
            if (sorted_points[hull_index] == point)
            {
                original_indices[i] = j;
                break;
            }
        }
    }

    if (orientation == Orientation::CLOCKWISE)
    {
        std::reverse(original_indices.begin(), original_indices.end());
    }

    return original_indices;
}

template <typename T>
std::vector<int> constructConvexHull(const std::vector<Point<T>> &points,
                                     ConvexHullAlgorithm algorithm = ConvexHullAlgorithm::GRAHAM_ANDREW,
                                     Orientation orientation = Orientation::COUNTERCLOCKWISE)
{
    switch (algorithm)
    {
    case ConvexHullAlgorithm::GRAHAM_ANDREW: {
        return constructGrahamAndrewConvexHull(points, orientation);
    }
    case ConvexHullAlgorithm::JARVIS_MARCH: {
        return {};
    }
    case ConvexHullAlgorithm::CHAN: {
        return {};
    }
    default: {
        return {};
    }
    }
}

} // namespace geom

#endif // CONVEX_HULL_HPP