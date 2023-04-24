#ifndef CONVEX_HULL_3D_HPP
#define CONVEX_HULL_3D_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace polygonization
{
template <typename T> class hull
{
  public:
    hull() = delete;
    hull(hull &&) = default;
    hull &operator=(hull &&) = default;
    hull(const hull &) = delete;
    hull &operator=(const hull &) = delete;
    ~hull() = default;

    explicit hull(const std::vector<std::array<T, 3>> &points)
    {
        if (points.size() < 3)
        {
            throw std::runtime_error("Constructing convex hull from less than three points is ambiguous.");
        }

        points_.reserve(points.size());
        for (const auto &[x, y, z] : points)
        {
            points_.emplace_back(x, y, z);
        }
    }

    std::vector<std::array<std::ptrdiff_t, 3>> operator()()
    {
        int n = static_cast<int>(points_.size());
        std::vector<Point *> list(n);
        for (int i = 0; i < n; ++i)
        {
            list[i] = const_cast<Point *>(&points_[i]);
        }

        Point *head = sort(list.data(), n);

        std::vector<Point *> event_list(2 * n);
        std::vector<Point *> temp(2 * n);

        constructHull(head, n, event_list.data(), temp.data());

        std::vector<std::array<std::ptrdiff_t, 3>> faces;
        int i = 0;
        auto first_mem = list[0];
        for (; event_list[i] != NIL; event_list[i++]->act())
        {
            faces.push_back(
                {event_list[i]->prev - first_mem, event_list[i] - first_mem, event_list[i]->next - first_mem});
        }

        return faces;
    }

  private:
    struct Point final
    {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                      "Point can only be templated with integer or floating-point types");

        T x, y, z;
        Point *prev = nullptr;
        Point *next = nullptr;

        Point() : x(0), y(0), z(0), prev(nullptr), next(nullptr)
        {
        }
        explicit Point(T x, T y, T z) : x{x}, y{y}, z{z}, prev(nullptr), next(nullptr)
        {
        }
        explicit Point(T x, T y, T z, Point *prev, Point *next) : x{x}, y{y}, z{z}, prev{prev}, next{next}
        {
        }
        ~Point()
        {
            x = 0;
            y = 0;
            z = 0;
            prev = nullptr;
            next = nullptr;
        }

        Point(Point &&) = default;
        Point &operator=(Point &&) = default;
        Point(const Point &) = delete;
        Point &operator=(const Point &) = delete;

        void act()
        {
            if (prev->next != this)
            {
                // insert
                prev->next = next->prev = this;
            }
            else
            {
                // delete
                prev->next = next;
                next->prev = prev;
            }
        }
    };

    static constexpr auto INFINITY = std::numeric_limits<T>::infinity();
    Point nil{INFINITY, INFINITY, INFINITY, nullptr, nullptr};
    Point *NIL = &nil;
    std::vector<Point> points_;

    // <0 if CCW
    inline T turn(const Point *p, const Point *q, const Point *r) const noexcept
    {
        if (p == NIL || q == NIL || r == NIL)
        {
            return 1.0;
        }
        // turn (p, q, r)
        return ((q->x - p->x) * (r->z - p->z) - (r->x - p->x) * (q->z - p->z));
    }

    // when turn changes
    inline T time(const Point *p, const Point *q, const Point *r) const noexcept
    {
        if (p == NIL || q == NIL || r == NIL)
        {
            return INFINITY;
        }
        // turn (p, q, r)
        return ((q->x - p->x) * (r->z - p->z) - (r->x - p->x) * (q->z - p->z));
    }

    // Mergesort
    Point *sort(Point *p_ptr[], int n)
    {
        Point *a = nullptr;
        Point *b = nullptr;
        Point *c = nullptr;
        Point head;

        if (n == 1)
        {
            p_ptr[0]->next = NIL;
            return p_ptr[0];
        }

        a = sort(p_ptr, n / 2);
        b = sort(p_ptr + n / 2, n - n / 2);
        c = &head;

        do
        {
            if (a->x < b->x)
            {
                c = c->next = a;
                a = a->next;
            }
            else
            {
                c = c->next = b;
                b = b->next;
            }
        } while (c != NIL);

        return head.next;
    }

    // 3D Convex Hull
    void constructHull(Point *p_ptr, int n, Point **A, Point **B)
    {
        Point *u = nullptr;
        Point *v = nullptr;
        Point *mid_ptr = nullptr;
        T t[6], old_t, new_t;
        int i, j, k, l, min_l;

        if (n == 1)
        {
            A[0] = p_ptr->prev = p_ptr->next = NIL;
            return;
        }

        for (u = p_ptr, i = 0; i < n / 2 - 1; u = u->next, i++)
        {
            ;
        }

        mid_ptr = v = u->next;
        constructHull(p_ptr, n / 2, B, A); // recurse on left and right sides
        constructHull(mid_ptr, n - n / 2, B + n / 2 * 2, A + n / 2 * 2);

        // find initial bridge
        for (;;)
        {
            if (turn(u, v, v->next) < 0)
            {
                v = v->next;
            }
            else if (turn(u->prev, u, v) < 0)
            {
                u = u->prev;
            }
            else
            {
                break;
            }
        }

        // merge by tracking bridge uv over time
        for (i = k = 0, j = n / 2 * 2, old_t = -INFINITY;; old_t = new_t)
        {
            t[0] = time(B[i]->prev, B[i], B[i]->next);
            t[1] = time(B[j]->prev, B[j], B[j]->next);
            t[2] = time(u, u->next, v);
            t[3] = time(u->prev, u, v);
            t[4] = time(u, v->prev, v);
            t[5] = time(u, v, v->next);
            for (new_t = INFINITY, l = 0; l < 6; l++)
            {
                if (t[l] > old_t && t[l] < new_t)
                {
                    min_l = l;
                    new_t = t[l];
                }
            }
            if (new_t == INFINITY)
            {
                break;
            }
            switch (min_l)
            {
            case (0): {
                if (B[i]->x < u->x)
                {
                    A[k++] = B[i];
                }
                B[i++]->act();
                break;
            }
            case (1): {
                if (B[j]->x > v->x)
                {
                    A[k++] = B[j];
                }
                B[j++]->act();
                break;
            }
            case (2): {
                A[k++] = u = u->next;
                break;
            }
            case (3): {
                A[k++] = u;
                u = u->prev;
                break;
            }
            case (4): {
                A[k++] = v = v->prev;
                break;
            }
            case (5): {
                A[k++] = v;
                v = v->next;
                break;
            }
            }
        }

        A[k] = NIL;
        u->next = v;
        v->prev = u; // now go back in time to update pointers
        for (k--; k >= 0; k--)
        {
            if (A[k]->x <= u->x || A[k]->x >= v->x)
            {
                A[k]->act();
                if (A[k] == u)
                {
                    u = u->prev;
                }
                else if (A[k] == v)
                {
                    v = v->next;
                }
            }
            else
            {
                u->next = A[k];
                A[k]->prev = u;
                v->prev = A[k];
                A[k]->next = v;
                if (A[k]->x < mid_ptr->x)
                {
                    u = A[k];
                }
                else
                {
                    v = A[k];
                }
            }
        }
    }
};

} // namespace polygonization

#endif // CONVEX_HULL_3D_HPP