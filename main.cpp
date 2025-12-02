#include <iostream>
#include <SFML/Graphics.hpp>
#include "Graph.h"

using namespace std;
using namespace sf;

int main() {
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;

    // --- CHOOSE YOUR TEST CASE ---
    const int NUM_NODES = 600;
    const GraphBuildStrategy STRATEGY = GraphBuildStrategy::K_NEAREST; // Try changing to K_NEAREST
    const int K_VALUE = 4; // Only used for K_NEAREST strategy

    // --- 1. Graph Generation ---
    Graph graph;
    graph.generateGraph(NUM_NODES, WINDOW_WIDTH, WINDOW_HEIGHT, STRATEGY, K_VALUE);

    // --- 2. SFML Window Setup ---
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MST Visualization");
    window.setFramerateLimit(60);

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
                        Vertex(Vector2f(node_u.x, node_u.y), Color(100, 100, 100)),
                        Vertex(Vector2f(node_v.x, node_v.y), Color(100, 100, 100))
                    };
                    window.draw(line, 2, sf::Lines);
                }
            }
        }

        // Draw nodes
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
