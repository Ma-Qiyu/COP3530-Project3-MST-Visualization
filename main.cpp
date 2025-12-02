#include <iostream>
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "MSTBuilder.h"

using namespace std;
using namespace sf;

int main() {
    const int WINDOW_WIDTH = 1500;
    const int WINDOW_HEIGHT = 1100;

    // --- CHOOSE YOUR TEST CASE ---
    const int NUM_NODES = 1000;
    const GraphBuildStrategy STRATEGY = GraphBuildStrategy::DELAUNAY; // Try changing to K_NEAREST
    const int K_VALUE = 3; // Only used for K_NEAREST strategy

    // --- 1. Graph Generation ---
    Graph graph;
    graph.generateGraph(NUM_NODES, WINDOW_WIDTH, WINDOW_HEIGHT, STRATEGY, K_VALUE);

    // --- 2. SFML Window Setup ---
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MST Visualization");
    window.setFramerateLimit(60);

    MSTBuilder builder;
    MSTResult kruskalResult = builder.runPrim(graph);

    // 在控制台打印统计结果
    cout << "\n--- MST BATCH RESULTS ---" << endl;
    cout << "Algorithm: " << kruskalResult.algorithmName << endl;
    cout << "Total Weight: " << kruskalResult.totalWeight << endl;
    cout << "Execution Time: " << kruskalResult.exeTimeMs << " us" << endl;
    cout << "Edge Count: " << kruskalResult.mstEdges.size() << endl;
    cout << "-------------------------" << endl;


    // --- 3. Main Loop ---
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        window.clear(Color(30, 30, 30));

        // Draw edges
        for (const auto& node_edges: graph.getAdjList()) {
            for (const auto& edge: node_edges) {
                if (edge.u < edge.v) {
                    const Node& node_u = graph.getNodes()[edge.u];
                    const Node& node_v = graph.getNodes()[edge.v];
                    Vertex line[] = {
                        Vertex(Vector2f(node_u.x, node_u.y), Color(100, 100, 100, 50)), // 灰色，半透明
                        Vertex(Vector2f(node_v.x, node_v.y), Color(100, 100, 100, 50))
                    };
                    window.draw(line, 2, sf::Lines);
                }
            }
        }

        // b. [新增] 再绘制 MST 的边 (用亮色，覆盖在上面)
        for (const auto& mst_edge: kruskalResult.mstEdges) {
            const Node& node_u = graph.getNodes()[mst_edge.u];
            const Node& node_v = graph.getNodes()[mst_edge.v];
            Vertex line[] = {
                Vertex(Vector2f(node_u.x, node_u.y), Color::Cyan), // 亮眼的青色
                Vertex(Vector2f(node_v.x, node_v.y), Color::Cyan)
            };
            window.draw(line, 2, sf::Lines);
        }

        // c. 最后绘制所有节点 (覆盖在所有线之上)
        CircleShape node_shape(4.0f);
        node_shape.setFillColor(Color::White);
        node_shape.setOrigin(4.0f, 4.0f);
        for (const auto& node: graph.getNodes()) {
            node_shape.setPosition(node.x, node.y);
            window.draw(node_shape);
        }

        window.display();
    }

    return 0;
}
