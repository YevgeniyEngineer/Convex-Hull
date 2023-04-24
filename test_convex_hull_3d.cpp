#include "convex_hull_3d.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main()
{
    using namespace polygonization;

    int n = 100'000;
    bool print_faces = false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1000.0, 1000.0);

    std::vector<std::array<double, 3>> points(n);

    for (int i = 0; i < n; ++i)
    {
        auto &point = points[i];
        point[0] = dist(gen);
        point[1] = dist(gen);
        point[2] = dist(gen);
    }

    auto t1 = std::chrono::steady_clock::now();

    auto faces = hull<double>(points)();

    auto t2 = std::chrono::steady_clock::now();

    if (print_faces)
    {
        for (auto &face : faces)
        {
            std::cout << face[0] << " " << face[1] << " " << face[2] << std::endl;
        }
    }

    auto elapsed = (t2 - t1).count() / 1e9;
    std::cout << "Number of faces: " << faces.size() << std::endl;
    std::cout << "Time per loop (s): " << elapsed << std::endl;

    return 0;
}