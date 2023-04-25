#include "convex_hull.hpp"

#include <chrono>
#include <iostream>
#include <random>

int main()
{
    using namespace geom;

    int num_pts = 100000;
    int coords_range = 1000;
    bool print_results = false;

    // Seed the random number generator with the current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0, coords_range);

    // Generate random points
    std::vector<Point<double>> points;
    points.reserve(num_pts);
    for (int i = 0; i < num_pts; ++i)
    {
        points.push_back(Point<double>(distribution(generator), distribution(generator)));
    }

    // Construct convex hull
    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<int> hull_indices =
        constructConvexHull(points, ConvexHullAlgorithm::GRAHAM_ANDREW, Orientation::COUNTERCLOCKWISE);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Elapsed time (s): " << (t2 - t1).count() / 1e9 << std::endl;
    std::cout << "Number of hull points: " << hull_indices.size() << std::endl;

    // Check if points are counterclockwise
    auto p1 = points[hull_indices[0]];
    auto p2 = points[hull_indices[1]];
    auto p3 = points[hull_indices[2]];

    auto orientation = getOrientation(p1, p2, p3);

    if (orientation == Orientation::COUNTERCLOCKWISE)
    {
        std::cout << "Hull is oriented counterclockwise" << std::endl;
    }
    else if (orientation == Orientation::CLOCKWISE)
    {
        std::cout << "Hull is oriented clockwise" << std::endl;
    }

    // Print hull indices
    if (print_results)
    {
        std::cout << "Convex Hull Indices:" << std::endl;
        for (int index : hull_indices)
        {
            std::cout << index << std::endl;
        }
    }

    return 0;
}