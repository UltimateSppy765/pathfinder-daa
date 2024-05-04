module utilities;

using namespace utils;

template <typename T>
bool valid(const Matrix<T>& m, const Pair& p) {
    return p.first < m.dimensions.first && p.second < m.dimensions.second;
}

// Backtracking
bool utils::track(Matrix<point_state>& m, const Pair& source, const Pair& destination, vector<Pair>& path) {
    m[source] = VISITED;
    if (source == destination) {
        m[source] = CAN_VISIT;
        return true;
    }
    Pair thing(source);
    thing.first++; // Down
    if (valid(m, thing) && m[thing] == CAN_VISIT) {
        if (track(m, thing, destination, path)) {
            path.emplace_back(thing);
            m[source] = CAN_VISIT;
            return true;
        }
    }
    thing.first--; // Reset
    thing.second++; // Right
    if (valid(m, thing) && m[thing] == CAN_VISIT) {
        if (track(m, thing, destination, path)) {
            path.emplace_back(thing);
            m[source] = CAN_VISIT;
            return true;
        }
    }
    thing.second--; // Reset
    thing.first--; // Up
    if (valid(m, thing) && m[thing] == CAN_VISIT) {
        if (track(m, thing, destination, path)) {
            path.emplace_back(thing);
            m[source] = CAN_VISIT;
            return true;
        }
    }
    thing.first++; // Reset
    thing.second--; // Left
    if (valid(m, thing) && m[thing] == CAN_VISIT) {
        if (track(m, thing, destination, path)) {
            path.emplace_back(thing);
            m[source] = CAN_VISIT;
            return true;
        }
    }
    m[source] = CAN_VISIT;
    return false;
}

// Branch and Bound
void utils::bound(Matrix<point_state>& m, const Pair& source, const Pair& destination, vector<Pair>& result, vector<Pair>& current) {
    m[source] = VISITED;
    current.emplace_back(source);
    if (source == destination) {
        m[source] = CAN_VISIT;
        result.clear();
        result.insert(result.begin(), current.rbegin(), current.rend());
        current.pop_back();
        return;
    }
    Pair thing(source);
    thing.first++; // Down
    // vector<T>.size() reutrns size_t
    // If it is 0, subtracting 1 gives us size_t max value
    if (valid(m, thing) && m[thing] == CAN_VISIT && current.size() < result.size() - 1) {
        bound(m, thing, destination, result, current);
    }
    thing.first--; // Reset
    thing.second++; // Right
    if (valid(m, thing) && m[thing] == CAN_VISIT && current.size() < result.size() - 1) {
        bound(m, thing, destination, result, current);
    }
    thing.second--; // Reset
    thing.first--; // Up
    if (valid(m, thing) && m[thing] == CAN_VISIT && current.size() < result.size() - 1) {
        bound(m, thing, destination, result, current);
    }
    thing.first++; // Reset
    thing.second--; // Left
    if (valid(m, thing) && m[thing] == CAN_VISIT && current.size() < result.size() - 1) {
        bound(m, thing, destination, result, current);
    }
    m[source] = CAN_VISIT;
    current.pop_back();
}