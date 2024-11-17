#include <iostream>
#include <vector>
#include <string>

// Aliases for convenience
// ELEM: Alias for a vector of elements
template <class T>
using ELEM = std::vector<T>;

template <class T>
using VEC = std::vector<ELEM<T>>;

template <class T>
using action_t = T (*) (int);

template <class T>
using pred_t = bool (*) (T);

template <class T>
using map_t = T (*) (T, T);

template <class T>
void printElem(ELEM<T>& v) {
    if (v.size() == 1) { // Special case for single elements
        std::cout << v[0] << std::endl;
        return;
    }
    std::cout << "["; 
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << ", "; // Print commas between elements
    }
    std::cout << "]" << std::endl;
}

// Utility function to initialize a vector of vectors (VEC<T>) with a new element (ELEM<T>)
template <class T>
void initVec(VEC<T>& v, ELEM<T>&& cons) {
    v.push_back(std::move(cons));
}

// Function to print the contents of a vector of vectors (VEC<T>)
template <class T>
void printVec(VEC<T> &v) {
    for (const auto &elem : v) {
        std::cout << "[";
        for (size_t j = 0; j < elem.size(); j++) {
            std::cout << elem[j];
            if (j < elem.size() - 1) std::cout << " , "; // Print commas between elements
        }
        std::cout << "]" << std::endl;
    }
}

// Function to print a zipped vector with optional double zipping
template <class T>
void printZippedVec(VEC<T>& v, bool doubleZip = false) {
    if (v.empty()) return; // Handle empty vector case

    const auto& elem = v[0];
    std::cout << "[";

    if (doubleZip) {
        // Process in groups of four
        for (size_t i = 0; i + 3 < elem.size(); i += 4) {
            if (i > 0) std::cout << " , ";
            std::cout << "(" << elem[i] << " " << elem[i + 2] << " " << elem[i + 1] << " " << elem[i + 3] << ")";
        }
    } else {
        // Process in pairs
        for (size_t i = 0; i + 1 < elem.size(); i += 2) {
            if (i > 0) std::cout << " , ";
            std::cout << "(" << elem[i] << " " << elem[i + 1] << ")";
        }
    }

    std::cout << "]" << std::endl;
}

// Function to zip two vectors (v and w) into one VEC<T> containing pairs of elements from both vectors
template<class T>
VEC<T> zip(VEC<T> &v, VEC<T> &w) {
    VEC<T> result;
    ELEM<T> zipped;
    for (size_t i = 0; i < v[0].size() && i < w[0].size(); i++) {
        zipped.push_back(v[0][i]);
        zipped.push_back(w[0][i]);
    }
    result.push_back(zipped);
    return result;
}

// Generate a vector of size N by applying function f
template <class T>
VEC<T> generate(int N, action_t<T> f) {
    VEC<T> v;
    ELEM<T> elem;
    for (int i = 0; i < N; ++i) {
        elem.push_back(f(i)); // Apply f to each index and store the result
    }
    v.push_back(elem);
    return v;
}

// Filter a vector of vectors (VEC<T>) based on predicate function f
template <class T>
VEC<T> filter(VEC<T>& v, pred_t<T> f) {
    VEC<T> result;
    for (auto& elem : v) {
        ELEM<T> filteredElem;
        for (const auto& e : elem) {
            if (f(e)) { // If element satisfies predicate, add to filtered list
                filteredElem.push_back(e);
            }
        }
        result.push_back(filteredElem);
    }
    return result;
}

// Apply a function f to each element of each sub-vector in VEC<T> and return the resulting VEC<T>
template <class T>
VEC<T> map(VEC<T>& v, action_t<T> f) {
    VEC<T> result;
    for (auto& elem : v) {
        ELEM<T> mappedElem;
        for (const auto& e : elem) {
            mappedElem.push_back(f(e)); // Apply f to each element of the sub-vector
        }
        result.push_back(mappedElem);
    }
    return result;
}

// Reduce
template <class T>
ELEM<T> reduce(VEC<T>& v, map_t<T> f, ELEM<T> ident) {
    ELEM<T> result = ident;
    for (const auto& elem : v) {
        for (const auto& e : elem) {
            result[0] = f(result[0], e); 
        }
    }
    return result;
}

// Supporting functions used for generating, filtering, and reducing
int f(int x) { return x * x; } // Squaring function
bool g(int x) { return x > 0; }
int h(int x) { return x > 0 ? 1 : 0; }
template <typename T>
T k(T a, T b) { return a + b; }

// Main function
int main() {
    VEC<int> v;
    initVec(v, ELEM<int>{1, 2, 3, 4});
    VEC<int> w;
    initVec(w, ELEM<int>{-1, 3, -3, 4});
    
    printVec(v);
    std::cout << std::string(10, '*') << std::endl;
    printVec(w);
    std::cout << std::string(10, '*') << std::endl;

    VEC<int> z = zip(v, w);
    printZippedVec(z);
    std::cout << std::string(10, '*') << std::endl;

    VEC<int> x = zip(z, z); 
    printZippedVec(x, true);
    std::cout << std::string(10, '*') << std::endl;

    VEC<int> a = generate(10, f);
    printVec(a);
    
    VEC<int> y = filter(w, g);
    printVec(y);
    
    VEC<int> u = map(w, h);
    printVec(u);
    
    ELEM<int> e = reduce(u, k, ELEM<int>{0});
    printElem(e);

    std::cout << std::string(10, '$') << std::endl;

    VEC<std::string> ws;
    initVec(ws, ELEM<std::string>{"hello", "there", "franco", "carlacci"});
    printVec(ws);

    ELEM<std::string> es = reduce(ws, k<std::string>, ELEM<std::string>{""});
    printElem(es);

    VEC<char> wc;
    initVec(wc, ELEM<char>{'a', 'b', 'c', 'd'});
    std::cout << std::string(10, '$') << std::endl;
    printVec(wc);

    ELEM<char> ec = reduce(wc, k<char>, ELEM<char>{' '});
    std::cout << std::endl << std::string(10, '$') << std::endl;
    printElem(ec);

    return 0; 
}

