#pragma once
#include "Graph.h"
#include <vector>
#include <string>
using namespace std;

// A struct to hold the results of an MST algorithm run
struct MSTResult {
    string algorithmName;
    double totalWeight = 0.0;
    long long exeTimeMs = 0;
    vector<Edge> mstEdges;
};

// Status for animation
enum class StepAction {
    CONSIDERING, // Yellow, edges being considered
    ACCEPTED, // Green, edges being accepted
};

// For animation, store each step
struct AnimationStep {
    Edge edge; // edge in this step
    StepAction action;
};

class MSTBuilder {
    // Union-Find (Disjoint Set) for Kruskal
    struct UnionFind {
        vector<int> parent;
        vector<int> rank;

        // Constructor
        UnionFind(int n);

        int find(int i);
        void unite(int i, int j);
    };

public:
    MSTResult runPrim(const Graph& graph);
    MSTResult runKruskal(const Graph& graph);

    // Function overloading, for animation
    MSTResult runPrim(const Graph& graph, vector<AnimationStep>& steps);
    MSTResult runKruskal(const Graph& graph, vector<AnimationStep>& steps);
};
