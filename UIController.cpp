#include "UIController.h"
#include <iostream>
#include <string>
using namespace std;

UIController::UIController()
    : window(VideoMode(1600, 900), "MST Visualization"), currentState(AppState::INITIAL) {
    window.setFramerateLimit(60);

    // Load font file from lib
    if (!font.loadFromFile("font.ttf")) {
        cerr << "Error: Could not load font file!" << endl;
    }

    // Initialize UI
    const int UI_WIDTH = 350;

    uiPanel.setSize(Vector2f(UI_WIDTH, window.getSize().y));
    uiPanel.setFillColor(Color(50, 50, 50));

    titleText.setFont(font);
    titleText.setString("MST VISUALIZATION");
    titleText.setCharacterSize(28);
    titleText.setPosition(20, 20);

    // --- Settings Section ---
    settingsTitle.setFont(font);
    settingsTitle.setString("Settings");
    settingsTitle.setCharacterSize(22);
    settingsTitle.setPosition(20, 80);

    generateButton.setSize(Vector2f(UI_WIDTH - 40, 50));
    generateButton.setPosition(20, 120);
    generateButton.setFillColor(Color(80, 80, 80));
    generateButtonText.setFont(font);
    generateButtonText.setString("Generate Graph");
    generateButtonText.setCharacterSize(20);
    generateButtonText.setPosition(generateButton.getPosition() + Vector2f(85, 12));

    // --- Run Algorithms Section ---
    runTitle.setFont(font);
    runTitle.setString("Run Algorithms");
    runTitle.setCharacterSize(22);
    runTitle.setPosition(20, 200);

    kruskalButton.setSize(Vector2f(UI_WIDTH - 40, 50));
    kruskalButton.setPosition(20, 240);
    kruskalButton.setFillColor(Color(80, 80, 80));
    kruskalButtonText.setFont(font);
    kruskalButtonText.setString("Run Kruskal");
    kruskalButtonText.setCharacterSize(20);
    kruskalButtonText.setPosition(kruskalButton.getPosition() + Vector2f(100, 12));

    primButton.setSize(Vector2f(UI_WIDTH - 40, 50));
    primButton.setPosition(20, 310);
    primButton.setFillColor(Color(80, 80, 80));
    primButtonText.setFont(font);
    primButtonText.setString("Run Prim");
    primButtonText.setCharacterSize(20);
    primButtonText.setPosition(primButton.getPosition() + Vector2f(115, 12));

    // --- Statistics Section ---
    statsTitle.setFont(font);
    statsTitle.setString("Statistics");
    statsTitle.setCharacterSize(22);
    statsTitle.setPosition(20, 400);

    algorithmText.setFont(font);
    algorithmText.setPosition(20, 440);
    weightText.setFont(font);
    weightText.setPosition(20, 470);
    timeText.setFont(font);
    timeText.setPosition(20, 500);
    edgeCountText.setFont(font);
    edgeCountText.setPosition(20, 530);

    // Set initial stats text
    algorithmText.setString("Algorithm: --");
    weightText.setString("Total Weight: --");
    timeText.setString("Execution Time: --");
    edgeCountText.setString("Edge Count: --");
}

void UIController::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// Handle events (mouse click)
void UIController::handleEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }

        // Mouse click
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                // Check if click "Generate Graph" button
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    cout << "Generate Graph button clicked!" << endl;
                    // Switch status
                    // Whatever the current status is, generate a new graph
                    graph.generateGraph(200, 1600 - 350, 900, GraphBuildStrategy::DELAUNAY, 5);
                    currentState = AppState::GRAPH_GENERATED;
                    // Clear last MST
                    mstResult = MSTResult();
                }

                // Check if click "Run Kruskal" Button
                if (currentState == AppState::GRAPH_GENERATED || currentState == AppState::FINISHED) {
                    if (kruskalButton.getGlobalBounds().contains(mousePos)) {
                        cout << "Run Kruskal button clicked!" << endl;
                        // Run Kruskal algorithm
                        mstResult = builder.runKruskal(graph);
                        currentState = AppState::FINISHED;
                    }

                    // Check if click "Run Prim" 按钮
                    if (primButton.getGlobalBounds().contains(mousePos)) {
                        cout << "Run Prim button clicked!" << endl;
                        // Run Prim algorithm
                        mstResult = builder.runPrim(graph);
                        currentState = AppState::FINISHED;
                    }
                }
            }
        }
    }
}

// Update status
void UIController::update() {
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

    // Hover Effect for buttons
    // Generate Button
    if (generateButton.getGlobalBounds().contains(mousePos)) {
        generateButton.setFillColor(Color(100, 100, 100));
    } else {
        generateButton.setFillColor(Color(80, 80, 80));
    }

    // Kruskal and Prim buttons
    if (currentState == AppState::GRAPH_GENERATED || currentState == AppState::FINISHED) {
        if (kruskalButton.getGlobalBounds().contains(mousePos)) {
            kruskalButton.setFillColor(Color(100, 100, 100));
        } else {
            kruskalButton.setFillColor(Color(80, 80, 80));
        }
        if (primButton.getGlobalBounds().contains(mousePos)) {
            primButton.setFillColor(Color(100, 100, 100));
        } else {
            primButton.setFillColor(Color(80, 80, 80));
        }
    } else {
        // If graph isn't generated, the button is gray
        kruskalButton.setFillColor(Color(60, 60, 60));
        primButton.setFillColor(Color(60, 60, 60));
    }

    // Update the statistics
    // Only update after MST is built
    if (currentState == AppState::FINISHED) {
        algorithmText.setString("Algorithm: " + mstResult.algorithmName);
        string weightStr = to_string(mstResult.totalWeight);
        weightText.setString("Total Weight: " + weightStr.substr(0, weightStr.find('.') + 3));
        timeText.setString("Execution Time: " + to_string(mstResult.exeTimeMs) + " us");
        edgeCountText.setString("Edge Count: " + to_string(mstResult.mstEdges.size()));
    } else {
        // If regenerate the graph, clear the statistics
        algorithmText.setString("Algorithm: --");
        weightText.setString("Total Weight: --");
        timeText.setString("Execution Time: --");
        edgeCountText.setString("Edge Count: --");
    }
}

// Render
void UIController::render() {
    window.clear(Color(20, 20, 20));

    // Draw right part: MST building Area
    const int UI_WIDTH = 350;

    for (const auto& edgeList: graph.getAdjList()) {
        for (const auto& edge: edgeList) {
            if (edge.u < edge.v) {
                const Node& u = graph.getNodes()[edge.u];
                const Node& v = graph.getNodes()[edge.v];
                Vertex line[] = {
                    Vertex(Vector2f(u.x + UI_WIDTH, u.y), Color(100, 100, 100, 50)),
                    Vertex(Vector2f(v.x + UI_WIDTH, v.y), Color(100, 100, 100, 50))
                };
                window.draw(line, 2, Lines);
            }
        }
    }

    // Draw the edges of MST
    if (currentState == AppState::FINISHED) {
        for (const auto& edge: mstResult.mstEdges) {
            const Node& u = graph.getNodes()[edge.u];
            const Node& v = graph.getNodes()[edge.v];
            Vertex line[] = {
                Vertex(Vector2f(u.x + UI_WIDTH, u.y), Color::Cyan),
                Vertex(Vector2f(v.x + UI_WIDTH, v.y), Color::Cyan)
            };
            window.draw(line, 2, Lines);
        }
    }

    // Draw the nodes
    CircleShape nodeShape(4.0f);
    nodeShape.setOrigin(4.f, 4.f);
    nodeShape.setFillColor(Color::White);
    for (const auto& node: graph.getNodes()) {
        nodeShape.setPosition(node.x + UI_WIDTH, node.y);
        window.draw(nodeShape);
    }

    // Draw left part: controller
    window.draw(uiPanel);
    window.draw(titleText);
    window.draw(settingsTitle);
    window.draw(generateButton);
    window.draw(generateButtonText);
    window.draw(runTitle);
    window.draw(kruskalButton);
    window.draw(kruskalButtonText);
    window.draw(primButton);
    window.draw(primButtonText);
    window.draw(statsTitle);
    window.draw(algorithmText);
    window.draw(weightText);
    window.draw(timeText);
    window.draw(edgeCountText);

    window.display();
}
