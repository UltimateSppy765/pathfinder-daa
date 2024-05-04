export module utilities;

import std.core;

using namespace std;

export namespace utils {
    typedef pair<size_t, size_t> Pair;

    enum class point_state {
        CANNOT_VISIT,
        CAN_VISIT,
        VISITED
    };

    using enum point_state;

    template <typename T>
    class Matrix {
    protected:
        T* matrix;

    public:
        const Pair dimensions;

        Matrix(Pair dimensions) : dimensions(dimensions),
            matrix(new T[dimensions.first * dimensions.second])
        {
        }

        // Custom copy constructor to prevent shallow copies
        Matrix(Matrix<T>& t) : dimensions(t.dimensions),
            matrix(new T[t.dimensions.first * t.dimensions.second])
        {
            for (size_t i(0); i < dimensions.first * dimensions.second; ++i)
                matrix[i] = t.matrix[i];
        }

        ~Matrix() {
            delete[] matrix;
        }

        T& operator[](Pair index) {
            return matrix[index.first * dimensions.second + index.second];
        }
    };

    bool track(Matrix<point_state>&, const Pair&, const Pair&, vector<Pair>&);
    void bound(Matrix<point_state>&, const Pair&, const Pair&, vector<Pair>&, vector<Pair>&);
}

using namespace utils;

template <typename T>
bool valid(const Matrix<T>& m, const Pair& p) {
    return p.first < m.dimensions.first && p.second < m.dimensions.second;
}