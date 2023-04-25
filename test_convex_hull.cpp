#include "convex_hull.hpp"

#include <chrono>
#include <iostream>
#include <random>

int main()
{
    using namespace geom;

    int num_pts = 99'999;
    int coords_range = 1000;
    bool print_results = true;
    auto orientation = Orientation::CLOCKWISE;

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

    // Construct convex hull using GRAHAM SCAN
    {
        std::cout << std::endl << "Constructing convex hull using GRAHAM SCAN " << std::endl;

        auto t1 = std::chrono::high_resolution_clock::now();

        auto hull_indices1 = constructConvexHull(points, ConvexHullAlgorithm::GRAHAM_SCAN, orientation);

        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << "Elapsed time (s): " << (t2 - t1).count() / 1e9 << std::endl;
        std::cout << "Number of hull points: " << hull_indices1.size() << std::endl;

        // Check if points are counterclockwise
        auto p1 = points[hull_indices1[0]];
        auto p2 = points[hull_indices1[1]];
        auto p3 = points[hull_indices1[2]];

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
            for (int index : hull_indices1)
            {
                std::cout << index << " ";
            }
            std::cout << std::endl;
        }
    }

    // Construct convex hull using ANDREW MONOTONE CHAIN
    {
        std::cout << std::endl << "Constructing convex hull using ANDREW MONOTONE CHAIN " << std::endl;

        auto t3 = std::chrono::high_resolution_clock::now();

        auto hull_indices2 = constructConvexHull(points, ConvexHullAlgorithm::ANDREW_MONOTONE_CHAIN, orientation);

        auto t4 = std::chrono::high_resolution_clock::now();

        std::cout << "Elapsed time (s): " << (t4 - t3).count() / 1e9 << std::endl;
        std::cout << "Number of hull points: " << hull_indices2.size() << std::endl;

        // Check if points are counterclockwise
        auto p1 = points[hull_indices2[0]];
        auto p2 = points[hull_indices2[1]];
        auto p3 = points[hull_indices2[2]];

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
            for (int index : hull_indices2)
            {
                std::cout << index << " ";
            }
            std::cout << std::endl;
        }
    }

    // Construct convex hull using JARVIS MARCH
    {
        std::cout << std::endl << "Constructing convex hull using JARVIS MARCH " << std::endl;

        auto t5 = std::chrono::high_resolution_clock::now();

        auto hull_indices3 = constructConvexHull(points, ConvexHullAlgorithm::JARVIS_MARCH, orientation);

        auto t6 = std::chrono::high_resolution_clock::now();

        std::cout << "Elapsed time (s): " << (t6 - t5).count() / 1e9 << std::endl;
        std::cout << "Number of hull points: " << hull_indices3.size() << std::endl;

        // Check if points are counterclockwise
        auto p1 = points[hull_indices3[0]];
        auto p2 = points[hull_indices3[1]];
        auto p3 = points[hull_indices3[2]];

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
            for (int index : hull_indices3)
            {
                std::cout << index << " ";
            }
            std::cout << std::endl;
        }
    }

    // Construct convex hull using CHAN's algorithm
    {
        std::cout << std::endl << "Constructing convex hull using CHAN " << std::endl;

        auto t7 = std::chrono::high_resolution_clock::now();

        auto hull_indices4 = constructConvexHull(points, ConvexHullAlgorithm::CHAN, orientation);

        auto t8 = std::chrono::high_resolution_clock::now();

        std::cout << "Elapsed time (s): " << (t8 - t7).count() / 1e9 << std::endl;
        std::cout << "Number of hull points: " << hull_indices4.size() << std::endl;

        // Check if points are counterclockwise
        auto p1 = points[hull_indices4[0]];
        auto p2 = points[hull_indices4[1]];
        auto p3 = points[hull_indices4[2]];

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
            for (int index : hull_indices4)
            {
                std::cout << index << " ";
            }
            std::cout << std::endl;
        }
    }

    // // Print hull indices
    // if (print_results)
    // {
    //     std::cout << "Convex Hull Indices:" << std::endl;
    //     for (int index : hull_indices3)
    //     {
    //         std::cout << index << std::endl;
    //     }
    // }

    return 0;
}