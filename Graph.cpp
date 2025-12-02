#include "Graph.h"
// For Delaunay triangulation
#include "lib/delaunator-cpp/delaunator.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
using namespace std;

void Graph::clear() {
    nodes.clear();
    adjList.clear();
}

void Graph::generateGraph(int numNodes, int canvasWidth, int canvasHeight, GraphBuildStrategy strategy, int k) {
    clear();
    if (numNodes <= 1) return;

    // Generate nodes using Grid Jitter first.
    generateNodesGridJitter(numNodes, canvasWidth, canvasHeight);

    // Choose an edge connection strategy!
    switch (strategy) {
        case GraphBuildStrategy::K_NEAREST:
            cout << "Connecting edges using K-Nearest heuristic..." << endl;
            connectEdgesKNearest(k, canvasWidth, canvasHeight);
            // Ensure the graph is connected
            ensureConnectivity();
            break;
        case GraphBuildStrategy::DELAUNAY:
            cout << "Connecting edges using Delaunay Triangulation..." << endl;
            connectEdgesDelaunay();
            break;
    }

    // Ensure the graph is connected
}

void Graph::generateNodesGridJitter(int numNodes, int canvasWidth, int canvasHeight) {
    nodes.clear();
    if (numNodes <= 1) return;

    // Determine grid size
    double d_numNodes = static_cast<double>(numNodes);
    // Compute number of columns and rows
    int gridCols = static_cast<int>(ceil(sqrt(d_numNodes)));
    int gridRows = (numNodes + gridCols - 1) / gridCols;

    // If cells number not enough, add rows
    while (gridRows * gridCols < numNodes) {
        gridRows++;
    }
    // Compute each cell's width and height
    double cellWidth = static_cast<double>(canvasWidth) / gridCols;
    double cellHeight = static_cast<double>(canvasHeight) / gridRows;

    // Traverse all the cells and put points
    random_device rd;
    mt19937 gen(rd());
    // We let the range of jitter to cover 80% of the area centered on the cell
    uniform_real_distribution<> distrib(0.15, 0.85);

    nodes.reserve(numNodes);
    int nodeId = 0;
    for (int r = 0; r < gridRows && nodeId < numNodes; r++) {
        for (int c = 0; c < gridCols && nodeId < numNodes; c++) {
            // Upleft side coordinate
            double startX = c * cellWidth;
            double startY = r * cellHeight;
            int X = static_cast<int>(startX + distrib(gen) * cellWidth);
            int Y = static_cast<int>(startY + distrib(gen) * cellHeight);
            nodes.push_back({nodeId, X, Y});
            nodeId++;
        }
    }
    cout << "Generated " << nodes.size() << " nodes using Grid Jitter." << endl;
}

void Graph::connectEdgesKNearest(int k, int canvasWidth, int canvasHeight) {
    int numNodes = nodes.size();
    if (numNodes <= 1 || k <= 0) return;

    // Determine a reasonable cell size.
    double max_dim = max(canvasWidth, canvasHeight); // Assuming canvas size from main.cpp
    int cellSize = static_cast<int>(max_dim / sqrt(numNodes)) + 1;
    if (cellSize == 0) cellSize = 1;

    // Construct a Hash Table
    unordered_map<long long, vector<int> > grid;
    // A large prime number for creating a unique key from (gx, gy)
    const long long C = 50000; // Should be larger than max grid y-coord

    for (const auto& node: nodes) {
        int gx = node.x / cellSize;
        int gy = node.y / cellSize;
        long long key = (long long) gx * C + gy;
        grid[key].push_back(node.id);
    }
    cout << "Spatial hash grid created with cell size: " << cellSize << endl;

    // Search for neighbors
    set<pair<int, int> > uniqueEdges;

    for (const auto& node: nodes) {
        vector<pair<double, int> > neighbors; // <distance, nodeId>
        int gx = node.x / cellSize;
        int gy = node.y / cellSize;

        // Iterate through the 3x3 neighborhood
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                long long key = (long long) (gx + i) * C + (gy + j);
                if (grid.count(key)) {
                    // For each node in the neighboring cell
                    for (int neighborId: grid[key]) {
                        if (neighborId == node.id) continue; // Don't connect to self
                        double dist = sqrt(pow(nodes[node.id].x - nodes[neighborId].x, 2) +
                                           pow(nodes[node.id].y - nodes[neighborId].y, 2));
                        neighbors.push_back({dist, neighborId});
                    }
                }
            }
        }

        // Sort the found neighbors by distance
        sort(neighbors.begin(), neighbors.end());

        // Add the top k edges to our unique set
        for (int i = 0; i < k && i < neighbors.size(); ++i) {
            int u = node.id;
            int v = neighbors[i].second;
            if (u > v) swap(u, v);
            uniqueEdges.insert({u, v});
        }
    }

    // Construct the AdjList
    adjList.resize(numNodes);
    for (const auto& edgePair: uniqueEdges) {
        int u = edgePair.first;
        int v = edgePair.second;
        double dist = sqrt(pow(nodes[u].x - nodes[v].x, 2) + pow(nodes[u].y - nodes[v].y, 2));
        adjList[u].push_back({u, v, dist});
        adjList[v].push_back({v, u, dist});
    }

    cout << "Connected " << uniqueEdges.size() << " unique edges using Spatial Hash K-Nearest." << endl;
}

void Graph::connectEdgesDelaunay() {
    int numNodes = nodes.size();
    if (numNodes == 2) {
        cout << "Connecting 2 nodes directly." << endl;
        adjList.resize(2);
        double dist = sqrt(pow(nodes[0].x - nodes[1].x, 2) + pow(nodes[0].y - nodes[1].y, 2));
        adjList[0].push_back({0, 1, dist});
        adjList[1].push_back({1, 0, dist});
        return;
    }

    // Prepare the data structure the library needs
    vector<double> coords;
    coords.reserve(numNodes * 2);
    for (const auto& node: nodes) {
        coords.push_back(static_cast<double>(node.x));
        coords.push_back(static_cast<double>(node.y));
    }

    // Use Delaunay library
    delaunator::Delaunator d(coords);

    // Triangulation
    set<pair<int, int> > uniqueEdges;
    for (size_t i = 0; i < d.triangles.size(); i += 3) {
        int p1 = d.triangles[i];
        int p2 = d.triangles[i + 1];
        int p3 = d.triangles[i + 2];

        // (u, v) with u < v
        if (p1 > p2) swap(p1, p2);
        if (p1 > p3) swap(p1, p3);
        if (p2 > p3) swap(p2, p3);

        uniqueEdges.insert({p1, p2});
        uniqueEdges.insert({p1, p3});
        uniqueEdges.insert({p2, p3});
    }

    // Construct the AdjList
    adjList.resize(numNodes);
    for (const auto& edgePair: uniqueEdges) {
        int u = edgePair.first;
        int v = edgePair.second;
        double dist = sqrt(pow(nodes[u].x - nodes[v].x, 2) + pow(nodes[u].y - nodes[v].y, 2));
        adjList[u].push_back({u, v, dist});
        adjList[v].push_back({v, u, dist});
    }
    cout << "Connected " << uniqueEdges.size() << " unique edges using Delaunay." << endl;
}

void Graph::bfs(int startNode, vector<bool>& visited, vector<int>& component) {
    queue<int> q;

    // Start the BFS
    q.push(startNode);
    visited[startNode] = true;
    component.push_back(startNode);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const auto& edge: adjList[u]) {
            int v = edge.v;
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
                component.push_back(v);
            }
        }
    }
}

// Ensure the whole graph is connected
void Graph::ensureConnectivity() {
    int numNodes = nodes.size();
    if (numNodes == 0) return;

    vector<bool> visited(numNodes, false);
    vector<vector<int> > components; // A list of all connected components

    // Find all connected components using BFS
    for (int i = 0; i < numNodes; ++i) {
        if (!visited[i]) {
            vector<int> currentComponent;
            bfs(i, visited, currentComponent);
            components.push_back(currentComponent);
        }
    }

    // Check if bridges need to be built
    if (components.size() <= 1) {
        cout << "Graph is already connected. Components: " << components.size() << endl;
        return; // The graph is connected, nothing to do.
    }
    cout << "Graph is not connected. Found " << components.size() << " components. Building bridges..." << endl;

    // Building bridges
    // Connect component i with component i+1
    for (size_t i = 0; i < components.size() - 1; ++i) {
        // Pick a random node from each component to connect
        int u = components[i][0];
        int v = components[i + 1][0];

        // Add a new edge
        double dist = sqrt(pow(nodes[u].x - nodes[v].x, 2) + pow(nodes[u].y - nodes[v].y, 2));
        adjList[u].push_back({u, v, dist});
        adjList[v].push_back({v, u, dist});

        cout << "Added bridge between node " << u << " (component " << i << ") and node " << v << " (component " << i +
                1 << ")." << endl;
    }
}


// Accessors
int Graph::getNumNodes() const { return nodes.size(); }
const vector<Node>& Graph::getNodes() const { return nodes; }
const vector<vector<Edge> >& Graph::getAdjList() const { return adjList; }


