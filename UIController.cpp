#include "UIController.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;

UIController::UIController()
    : window(VideoMode(1600, 900), "MST Visualization"), currentState(AppState::INITIAL), isDraggingSlider(false) {
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

    // Kruskal Batch Buttons
    kruskalBatchButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    kruskalBatchButton.setPosition(20, 240);
    kruskalBatchButton.setFillColor(Color(80, 80, 80));
    kruskalBatchButtonText.setFont(font);
    kruskalBatchButtonText.setString("Kruskal (Instant)");
    kruskalBatchButtonText.setCharacterSize(18);
    kruskalBatchButtonText.setPosition(kruskalBatchButton.getPosition() + Vector2f(80, 8));

    // Kruskal Animate Button
    kruskalAnimateButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    kruskalAnimateButton.setPosition(20, 290);
    kruskalAnimateButton.setFillColor(Color(80, 80, 80));
    kruskalAnimateButtonText.setFont(font);
    kruskalAnimateButtonText.setString("Kruskal (Animate)");
    kruskalAnimateButtonText.setCharacterSize(18);
    kruskalAnimateButtonText.setPosition(kruskalAnimateButton.getPosition() + Vector2f(75, 8));

    // Prim Buttons
    primBatchButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    primBatchButton.setPosition(20, 340);
    primBatchButtonText.setFont(font);
    primBatchButtonText.setString("Prim (Instant)");
    primBatchButtonText.setCharacterSize(18);
    primBatchButtonText.setPosition(primBatchButton.getPosition() + Vector2f(95, 8));

    primAnimateButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    primAnimateButton.setPosition(20, 390);
    primAnimateButtonText.setFont(font);
    primAnimateButtonText.setString("Prim (Animate)");
    primAnimateButtonText.setCharacterSize(18);
    primAnimateButtonText.setPosition(primAnimateButton.getPosition() + Vector2f(90, 8));

    // Statistics Section
    statsTitle.setFont(font);
    statsTitle.setString("Statistics");
    statsTitle.setCharacterSize(22);
    statsTitle.setPosition(20, 460);

    algorithmText.setFont(font);
    algorithmText.setPosition(20, 500);
    weightText.setFont(font);
    weightText.setPosition(20, 530);
    timeText.setFont(font);
    timeText.setPosition(20, 560);
    edgeCountText.setFont(font);
    edgeCountText.setPosition(20, 590);

    // Set initial stats text
    algorithmText.setString("Algorithm: --");
    weightText.setString("Total Weight: --");
    timeText.setString("Execution Time: --");
    edgeCountText.setString("Edge Count: --");

    // Animation control section initialization
    animControlTitle.setFont(font);
    animControlTitle.setString("Animation Control");
    animControlTitle.setCharacterSize(22);
    animControlTitle.setPosition(20, 650);

    playPauseButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    playPauseButton.setPosition(20, 690);
    playPauseButton.setFillColor(Color(80, 80, 80));
    playPauseButtonText.setFont(font);
    playPauseButtonText.setString("Play"); // Initialized as "Play"
    playPauseButtonText.setCharacterSize(18);
    playPauseButtonText.setPosition(playPauseButton.getPosition() + Vector2f(130, 8));

    speedSliderText.setFont(font);
    speedSliderText.setString("Speed");
    speedSliderText.setCharacterSize(18);
    speedSliderText.setPosition(20, 740);

    speedSliderBack.setSize(Vector2f(UI_WIDTH - 120, 10));
    speedSliderBack.setPosition(100, 745);
    speedSliderBack.setFillColor(Color(30, 30, 30));

    speedSliderHandle.setSize(Vector2f(10, 24));
    speedSliderHandle.setOrigin(5, 12); // Center point
    speedSliderHandle.setFillColor(Color::White);
    // Speed slider initialization
    float initialSliderX = speedSliderBack.getPosition().x + speedSliderBack.getSize().x / 2.f;
    speedSliderHandle.setPosition(initialSliderX, speedSliderBack.getPosition().y + 5);

    isPlaying = false;
    currentStepIndex = 0;
    animationDelaySeconds = 0.05f;
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

        // Mouse click event
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                // "Generate Graph" button
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    cout << "Generate Graph button clicked!" << endl;
                    graph.generateGraph(200, 1600 - 350, 900, GraphBuildStrategy::DELAUNAY, 5);
                    currentState = AppState::GRAPH_GENERATED;
                    mstResult = MSTResult();
                    // When generate new graph, stop and clear old animation
                    isPlaying = false;
                    animationSteps.clear();
                    currentStepIndex = 0;
                }

                // Algorithm and animation controller button
                if (currentState != AppState::INITIAL) {
                    // Check if click "Kruskal" Button
                    if (kruskalBatchButton.getGlobalBounds().contains(mousePos)) {
                        isPlaying = false;
                        mstResult = builder.runKruskal(graph);
                        currentState = AppState::FINISHED;
                    }

                    // Click Kruskal (Animate) button
                    if (kruskalAnimateButton.getGlobalBounds().contains(mousePos)) {
                        mstResult = builder.runKruskal(graph, animationSteps);
                        currentStepIndex = 0;
                        isPlaying = true;
                        stepTimer.restart();
                        currentState = AppState::ANIMATING;
                    }

                    // Prim Button
                    if (primBatchButton.getGlobalBounds().contains(mousePos)) {
                        isPlaying = false;
                        mstResult = builder.runPrim(graph);
                        currentState = AppState::FINISHED;
                    }
                    if (primAnimateButton.getGlobalBounds().contains(mousePos)) {
                        mstResult = builder.runPrim(graph, animationSteps);
                        currentStepIndex = 0;
                        isPlaying = true;
                        stepTimer.restart();
                        currentState = AppState::ANIMATING;
                    }

                    // Click Play/Pause
                    if (currentState == AppState::ANIMATING && playPauseButton.getGlobalBounds().contains(mousePos)) {
                        isPlaying = !isPlaying;
                        if (isPlaying) {
                            stepTimer.restart();
                        }
                    }
                    if (speedSliderHandle.getGlobalBounds().contains(mousePos)) {
                        isDraggingSlider = true;
                    }
                }
            }
        }
        // --- Mouse Button Released ---
        if (event.type == Event::MouseButtonReleased) {
            if (event.mouseButton.button == Mouse::Left) {
                isDraggingSlider = false; // Stop dragging
            }
        }

        // --- Mouse Moved ---
        if (event.type == Event::MouseMoved) {
            if (isDraggingSlider) {
                // [优化] Only move if dragging
                Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                float newX = max(speedSliderBack.getPosition().x,
                                 min(mousePos.x, speedSliderBack.getPosition().x + speedSliderBack.getSize().x));
                speedSliderHandle.setPosition(newX, speedSliderHandle.getPosition().y);
                float percent = (newX - speedSliderBack.getPosition().x) / speedSliderBack.getSize().x;
                animationDelaySeconds = 0.5f - (percent * 0.499f);
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
        generateButton.setFillColor(Color(100, 100, 100)); // Hover color
    } else {
        generateButton.setFillColor(Color(80, 80, 80));
    }

    // Only enable these buttons after the graph is generated
    bool algoButtonsEnabled = (currentState == AppState::GRAPH_GENERATED);

    // Kruskal and Prim buttons
    if (algoButtonsEnabled) {
        // Kruskal Batch Button
        if (kruskalBatchButton.getGlobalBounds().contains(mousePos)) {
            kruskalBatchButton.setFillColor(Color(100, 100, 100));
        } else {
            kruskalBatchButton.setFillColor(Color(80, 80, 80));
        }
        // Kruskal Animate Button
        if (kruskalAnimateButton.getGlobalBounds().contains(mousePos)) {
            kruskalAnimateButton.setFillColor(Color(100, 100, 100));
        } else {
            kruskalAnimateButton.setFillColor(Color(80, 80, 80));
        }
        // Prim Batch Button
        if (primBatchButton.getGlobalBounds().contains(mousePos)) {
            primBatchButton.setFillColor(Color(100, 100, 100));
        } else {
            primBatchButton.setFillColor(Color(80, 80, 80));
        }
        // Prim Animate Button
        if (primAnimateButton.getGlobalBounds().contains(mousePos)) {
            primAnimateButton.setFillColor(Color(100, 100, 100));
        } else {
            primAnimateButton.setFillColor(Color(80, 80, 80));
        }
    } else {
        // If buttons not enabled
        kruskalBatchButton.setFillColor(Color(60, 60, 60));
        kruskalAnimateButton.setFillColor(Color(60, 60, 60));
        primBatchButton.setFillColor(Color(60, 60, 60));
        primAnimateButton.setFillColor(Color(60, 60, 60));
    }

    // Animation logics
    if (currentState == AppState::ANIMATING) {
        if (playPauseButton.getGlobalBounds().contains(mousePos)) {
            playPauseButton.setFillColor(Color(100, 100, 100));
        } else {
            playPauseButton.setFillColor(Color(80, 80, 80));
        }
    } else {
        playPauseButton.setFillColor(Color(60, 60, 60));
    }

    if (currentState == AppState::ANIMATING && isPlaying) {
        if (stepTimer.getElapsedTime().asSeconds() >= animationDelaySeconds) {
            if (currentStepIndex < animationSteps.size()) {
                currentStepIndex++;
            } else {
                // Animation done
                isPlaying = false;
                currentState = AppState::FINISHED;
            }
            stepTimer.restart();
        }
    }

    // Update the text of Play/Pause
    if (isPlaying) {
        playPauseButtonText.setString("Pause");
    } else {
        playPauseButtonText.setString("Play");
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
        // If regenerate the graph, clear the statistics.
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
    map<pair<int, int>, Color> edgeColors;

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

    if (currentState == AppState::ANIMATING) {
        // Set accepted edges cyan
        for (size_t i = 0; i < currentStepIndex; ++i) {
            const auto& step = animationSteps[i];
            if (step.action == StepAction::ACCEPTED) {
                const Node& u_node = graph.getNodes()[step.edge.u];
                const Node& v_node = graph.getNodes()[step.edge.v];
                Vertex line[] = {
                    Vertex(Vector2f(u_node.x + UI_WIDTH, u_node.y), Color::Cyan),
                    Vertex(Vector2f(v_node.x + UI_WIDTH, v_node.y), Color::Cyan)
                };
                window.draw(line, 2, Lines);
            }
        }
        // Set considering edges yellow
        if (currentStepIndex > 0 && currentStepIndex <= animationSteps.size()) {
            const auto& lastStep = animationSteps[currentStepIndex - 1];
            if (lastStep.action == StepAction::CONSIDERING) {
                const Node& u_node = graph.getNodes()[lastStep.edge.u];
                const Node& v_node = graph.getNodes()[lastStep.edge.v];
                Vertex line[] = {
                    Vertex(Vector2f(u_node.x + UI_WIDTH, u_node.y), Color::Yellow),
                    Vertex(Vector2f(v_node.x + UI_WIDTH, v_node.y), Color::Yellow)
                };
                window.draw(line, 2, Lines);
            }
        }
    } else if (currentState == AppState::FINISHED) {
        // Instant button or animation finished
        for (const auto& edge: mstResult.mstEdges) {
            const Node& u_node = graph.getNodes()[edge.u];
            const Node& v_node = graph.getNodes()[edge.v];
            Vertex line[] = {
                Vertex(Vector2f(u_node.x + UI_WIDTH, u_node.y), Color::Cyan),
                Vertex(Vector2f(v_node.x + UI_WIDTH, v_node.y), Color::Cyan)
            };
            window.draw(line, 2, Lines);
        }
    }

    for (const auto& edgeList: graph.getAdjList()) {
        for (const auto& edge: edgeList) {
            if (edge.u < edge.v) {
                Color color = Color(100, 100, 100, 50); // 默认灰色
                if (edgeColors.count({edge.u, edge.v})) {
                    color = edgeColors.at({edge.u, edge.v});
                }
                const Node& u_node = graph.getNodes()[edge.u];
                const Node& v_node = graph.getNodes()[edge.v];
                Vertex line[] = {
                    Vertex(Vector2f(u_node.x + UI_WIDTH, u_node.y), color),
                    Vertex(Vector2f(v_node.x + UI_WIDTH, v_node.y), color)
                };
                window.draw(line, 2, Lines);
            }
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

    // Draw settings section
    window.draw(settingsTitle);
    window.draw(generateButton);
    window.draw(generateButtonText);

    // Draw run algorithm section
    window.draw(runTitle);
    window.draw(kruskalBatchButton);
    window.draw(kruskalBatchButtonText);
    window.draw(kruskalAnimateButton);
    window.draw(kruskalAnimateButtonText);
    window.draw(primBatchButton);
    window.draw(primBatchButtonText);
    window.draw(primAnimateButton);
    window.draw(primAnimateButtonText);

    // Draw statistics section
    window.draw(statsTitle);
    window.draw(algorithmText);
    window.draw(weightText);
    window.draw(timeText);
    window.draw(edgeCountText);

    // Draw animation controller
    window.draw(animControlTitle);
    window.draw(playPauseButton);
    window.draw(playPauseButtonText);
    window.draw(speedSliderText);
    window.draw(speedSliderBack);
    window.draw(speedSliderHandle);

    window.display();
}
