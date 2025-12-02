#include "MSTBuilder.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <queue>
using namespace std;

MSTBuilder::UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.resize(n, 0);
    for (int i = 0; i < n; i++) {
        parent[i] = i; // Each node is its own parent initially
    }
}

// Find the root of the set containing i
int MSTBuilder::UnionFind::find(int i) {
    if (parent[i] == i) {
        return i;
    }

    // Set the parent of i directly to the root
    return parent[i] = find(parent[i]);
}

// Merge the sets containing i and j
void MSTBuilder::UnionFind::unite(int i, int j) {
    int rootI = find(i);
    int rootJ = find(j);

    if (rootI != rootJ) {
        // Only merge them when they are not in the same set
        // Union by rank: attach smaller tree to the root of the larger tree
        if (rank[rootI] < rank[rootJ]) {
            parent[rootI] = rootJ;
        } else if (rank[rootI] > rank[rootJ]) {
            parent[rootJ] = rootI;
        } else {
            parent[rootJ] = rootI;
            rank[rootI]++;
        }
    }
}

// Kruskal's Algorithm
MSTResult MSTBuilder::runKruskal(const Graph& graph) {
    cout << "Running Kruskal's Algorithm..." << endl;

    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

    int numNodes = graph.getNumNodes();
    MSTResult result;
    result.algorithmName = "Kruskal";

    if (numNodes == 0) return result;

    // Get all unique edges from the graph ---
    vector<Edge> allEdges;
    // Use a set to temporarily avoid duplicate edges while gathering
    set<pair<int, int> > seenEdges;
    for (const auto& edgeList: graph.getAdjList()) {
        for (const auto& edge: edgeList) {
            int u = edge.u, v = edge.v;
            if (u > v) swap(u, v);
            if (seenEdges.find({u, v}) == seenEdges.end()) {
                allEdges.push_back(edge);
                seenEdges.insert({u, v});
            }
        }
    }

    // Sort all edges by weight in ascending order
    sort(allEdges.begin(), allEdges.end());

    // Initialize Union-Find
    UnionFind uf(numNodes);
    // Iterate through sorted edges and build the MST
    for (const auto& edge: allEdges) {
        // Check if including this edge creates a cycle
        if (uf.find(edge.u) != uf.find(edge.v)) {
            // No cycle, this is a good edge. Add it to our MST.
            uf.unite(edge.u, edge.v);
            result.mstEdges.push_back(edge);
            result.totalWeight += edge.weight;
        }

        // Check for termination condition
        if (result.mstEdges.size() == numNodes - 1) {
            break; // We have found all the edges for the MST
        }
    }

    // Stop timing
    auto endTime = chrono::high_resolution_clock::now();
    result.exeTimeMs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Kruskal's completed in " << result.exeTimeMs << " us." << endl;
    return result;
}

// Overloaded runKruskal for animation
MSTResult MSTBuilder::runKruskal(const Graph& graph, vector<AnimationStep>& steps) {
    cout << "Running Kruskal's Algorithm..." << endl;

    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

    steps.clear();

    int numNodes = graph.getNumNodes();
    MSTResult result;
    result.algorithmName = "Kruskal";

    if (numNodes == 0) return result;

    // Get all unique edges from the graph ---
    vector<Edge> allEdges;
    // Use a set to temporarily avoid duplicate edges while gathering
    set<pair<int, int> > seenEdges;
    for (const auto& edgeList: graph.getAdjList()) {
        for (const auto& edge: edgeList) {
            int u = edge.u, v = edge.v;
            if (u > v) swap(u, v);
            if (seenEdges.find({u, v}) == seenEdges.end()) {
                allEdges.push_back(edge);
                seenEdges.insert({u, v});
            }
        }
    }

    // Sort all edges by weight in ascending order
    sort(allEdges.begin(), allEdges.end());

    // Initialize Union-Find
    UnionFind uf(numNodes);
    // Iterate through sorted edges and build the MST
    for (const auto& edge: allEdges) {
        // Record "considering"
        steps.push_back({edge, StepAction::CONSIDERING});

        // Check if including this edge creates a cycle
        if (uf.find(edge.u) != uf.find(edge.v)) {
            // Record "accepted"
            steps.push_back({edge, StepAction::ACCEPTED});

            // No cycle, this is a good edge. Add it to our MST.
            uf.unite(edge.u, edge.v);
            result.mstEdges.push_back(edge);
            result.totalWeight += edge.weight;
        }

        // Check for termination condition
        if (result.mstEdges.size() == numNodes - 1) {
            break; // We have found all the edges for the MST
        }
    }

    // Stop timing
    auto endTime = chrono::high_resolution_clock::now();
    result.exeTimeMs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Kruskal's completed in " << result.exeTimeMs << " us." << endl;
    return result;
}

// Prim's Algorithm
MSTResult MSTBuilder::runPrim(const Graph& graph) {
    cout << "Running Prim's Algorithm..." << endl;
    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

    int numNodes = graph.getNumNodes();
    MSTResult result;
    result.algorithmName = "Prim";

    if (numNodes == 0) return result;

    // Initialization
    vector<bool> inMST(numNodes, false); // Check if node is already in MST
    // Min Heap to store potential edges
    priority_queue<Edge, vector<Edge>, greater<Edge> > pq;

    // Always build MST with start node 0
    int startNode = 0;
    inMST[startNode] = true;
    // Add all the outcoming edges into the min heap
    for (const auto& edge: graph.getAdjList()[startNode]) {
        pq.push(edge);
    }

    // Repeatedly build MST
    // MST needs n-1 edges
    while (!pq.empty() && result.mstEdges.size() < numNodes - 1) {
        // Get the shortest edge
        Edge shortestEdge = pq.top();
        pq.pop();

        int u = shortestEdge.u;
        int v = shortestEdge.v;

        // Check if it form a cycle
        if (inMST[v]) {
            continue;
        }

        // Accept this edge and add it into the MST
        inMST[v] = true;
        result.mstEdges.push_back(shortestEdge);
        result.totalWeight += shortestEdge.weight;

        // Mark all the outcoming edges of the new node as potential edges
        for (const auto& newEdge: graph.getAdjList()[v]) {
            if (!inMST[newEdge.v]) {
                pq.push(newEdge);
            }
        }
    }

    // Stop timing
    auto endTime = chrono::high_resolution_clock::now();
    result.exeTimeMs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Prim's completed in " << result.exeTimeMs << " us." << endl;
    return result;
}

MSTResult MSTBuilder::runPrim(const Graph& graph, vector<AnimationStep>& steps) {
    cout << "Running Prim's Algorithm..." << endl;
    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

    steps.clear();

    int numNodes = graph.getNumNodes();
    MSTResult result;
    result.algorithmName = "Prim";

    if (numNodes == 0) return result;

    // Initialization
    vector<bool> inMST(numNodes, false); // Check if node is already in MST
    // Min Heap to store potential edges
    priority_queue<Edge, vector<Edge>, greater<Edge> > pq;

    // Always build MST with start node 0
    int startNode = 0;
    inMST[startNode] = true;
    // Add all the outcoming edges into the min heap
    for (const auto& edge: graph.getAdjList()[startNode]) {
        pq.push(edge);
    }

    // Repeatedly build MST
    // MST needs n-1 edges
    while (!pq.empty() && result.mstEdges.size() < numNodes - 1) {
        // Get the shortest edge
        Edge shortestEdge = pq.top();
        pq.pop();

        int u = shortestEdge.u;
        int v = shortestEdge.v;

        // Check if it form a cycle
        if (inMST[v]) {
            continue;
        }

        // Animation
        steps.push_back({shortestEdge, StepAction::CONSIDERING});
        steps.push_back({shortestEdge, StepAction::ACCEPTED});

        // Accept this edge and add it into the MST
        inMST[v] = true;
        result.mstEdges.push_back(shortestEdge);
        result.totalWeight += shortestEdge.weight;

        // Mark all the outcoming edges of the new node as potential edges
        for (const auto& newEdge: graph.getAdjList()[v]) {
            if (!inMST[newEdge.v]) {
                pq.push(newEdge);
            }
        }
    }

    // Stop timing
    auto endTime = chrono::high_resolution_clock::now();
    result.exeTimeMs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Prim's completed in " << result.exeTimeMs << " us." << endl;
    return result;
}
