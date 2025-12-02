#pragma once
#include <vector>
#include <cmath>
using namespace std;

// Choose the edge connection strategy
enum class GraphBuildStrategy {
    K_NEAREST,
    DELAUNAY
};

// Basic struct for a vertex/node in the graph
struct Node {
    int id;
    int x;
    int y;
};

// A weighted, undirected edge
struct Edge {
    int u; // node ID
    int v;
    double weight; // Eclidean distance

    // Comparison for sorting and priority queues
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Graph
class Graph {
    vector<Node> nodes;
    vector<vector<Edge> > adjList;

    // Helper functions for graph generation
    // Point Generation
    void generateNodesGridJitter(int numNodes, int canvasWidth, int canvasHeight);
    // Edge Generation
    void connectEdgesKNearest(int k, int canvasWidth, int canvasHeight);
    void connectEdgesDelaunay();
    void bfs(int startNode, vector<bool>& visited, vector<int>& component);
    void ensureConnectivity();

public:
    // Clear all graph data
    void clear();

    // Generate a random graph with certain number of nodes
    // Each node is connected to its k nearest neighbors
    void generateGraph(int numNodes, int canvasWidth, int canvasHeight, GraphBuildStrategy, int k = 5);

    // Accessors
    int getNumNodes() const;
    const vector<Node>& getNodes() const;
    const vector<vector<Edge> >& getAdjList() const;
};
