/*
 * Moving median.
 */
multiset<int> s;
auto it = s.end();

void add(int x) {
    bool split = s.size() % 2 == 0;
    s.insert(x);
    if (s.size() == 1)
        it = s.begin();
    else if (split  && x >= *it) ++it;
    else if (!split && x <  *it) --it;
}

void remove(int x) {
    bool split = s.size() % 2 == 0;
    if (x == *it) {
        split ? s.erase(it++) : s.erase(it--);
    } else {
        s.erase(s.find(x));
        if (split  && x < *it) ++it;
        if (!split && x > *it) --it;
    }
}

int doubleMedian() {
    return s.size() % 2 ? 2*(*it) : *it + *next(it);
}
