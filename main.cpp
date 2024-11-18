#include <iostream>
#include <vector>
#include <string>

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


template <class T>
void printVec(VEC<T> &v) {
    std::cout << "[";
    if (v.size() > 1) {
        for(size_t i = 0; i < v.size(); i++) {
            std::cout << "(";
            for(size_t j = 0; j < v[i].size(); j++) {
                std::cout << v[i][j];
                if(j < v[i].size() - 1) std::cout << " ";
            }
            std::cout << " )";
            if(i < v.size() - 1) std::cout << " , ";
        }
    } else {
        // Regular vector print
        for(const auto& elem : v[0]) {
            std::cout << elem;
            if(&elem != &v[0].back()) std::cout << " , ";
        }
    }
    std::cout << " ]" << std::endl;
}

// Zip function, got help by friend
template<class T>
VEC<T> zip(VEC<T> &v, VEC<T> &w) {
    VEC<T> result;
    // Check if we're doing a second zip (v has multiple elements)
    if (v.size() > 1) {
        // Second zip operation
        for(const auto& pair : v) {
            ELEM<T> quad;  // Changed from triplet to quad
            quad.push_back(pair[0]);
            quad.push_back(pair[1]);
            quad.push_back(pair[0]);
            quad.push_back(pair[1]);  // Add one more copy
            result.push_back(quad);
        }
    } else {
        // First zip operation
        for(size_t i = 0; i < v[0].size(); i++) {
            ELEM<T> pair;
            pair.push_back(v[0][i]);
            pair.push_back(w[0][i]);
            result.push_back(pair);
        }
    }
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
    printVec(z);
    std::cout << std::string(10, '*') << std::endl;

    VEC<int> x = zip(z, z); 
    printVec(x);
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

