#include <iostream>
#include <tuple>

template <typename T>
struct point {
    T x, y;

    explicit point<T>(T x = 0, T y = 0): x(x), y(y) {}

    point operator+(point q) const { return point(x + q.x, y + q.y); }
    point operator-(point q) const { return point(x - q.x, y - q.y); }
    point operator*(T t)     const { return point(x * t, y * t); }
    point operator/(T t)     const { return point(x / t, y / t); }

    // Dot product.
    T operator*(point q) const { return x * q.x + y * q.y; }

    // "Cross" product (signed area of the quadrilateral).
    T operator%(point q) const { return x * q.y - y * q.x; }

    bool operator==(point q) const
        { return std::tie(x, y) == std::tie(q.x, q.y); }
    bool operator!=(point q) const
        { return std::tie(x, y) != std::tie(q.x, q.y); }
    bool operator<(point q) const
        { return std::tie(x, y) < std::tie(q.x, q.y); }
};

int main() {
    return 0;
}
