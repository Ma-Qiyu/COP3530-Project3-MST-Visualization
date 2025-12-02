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
    titleText.setStyle(Text::Bold);
    titleText.setFillColor(Color::White);

    // --- Settings Section ---
    settingsTitle.setFont(font);
    settingsTitle.setString("Settings");
    settingsTitle.setCharacterSize(22);
    settingsTitle.setPosition(20, 80);
    settingsTitle.setFillColor(Color(135, 206, 250)); // Light Sky Blue

    initTextInputBox(nodeCountInput, font, {20, 120}, {140, 40});
    nodeCountInput.text.setPosition(30, 122);

    generateButton.setSize(Vector2f(UI_WIDTH - 40 - 150, 40));
    generateButton.setPosition(170, 120);
    generateButton.setFillColor(Color(80, 80, 80));
    generateButtonText.setFont(font);
    generateButtonText.setString("Generate Graph");
    generateButtonText.setCharacterSize(18);
    FloatRect textBounds = generateButtonText.getLocalBounds();
    generateButtonText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    generateButtonText.setPosition(generateButton.getPosition() + (generateButton.getSize() / 2.f));

    // Run Algorithms Section
    runTitle.setFont(font);
    runTitle.setString("Run Algorithms");
    runTitle.setCharacterSize(22);
    runTitle.setPosition(20, 190);
    runTitle.setFillColor(Color(144, 238, 144)); // Light Green

    // Kruskal Batch Buttons
    kruskalBatchButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    kruskalBatchButton.setPosition(20, 230);
    kruskalBatchButton.setFillColor(Color(80, 80, 80));
    kruskalBatchButtonText.setFont(font);
    kruskalBatchButtonText.setString("Kruskal (Instant)");
    kruskalBatchButtonText.setCharacterSize(18);
    kruskalBatchButtonText.setPosition(kruskalBatchButton.getPosition() + Vector2f(80, 8));

    // Kruskal Animate Button
    kruskalAnimateButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    kruskalAnimateButton.setPosition(20, 280);
    kruskalAnimateButton.setFillColor(Color(80, 80, 80));
    kruskalAnimateButtonText.setFont(font);
    kruskalAnimateButtonText.setString("Kruskal (Animate)");
    kruskalAnimateButtonText.setCharacterSize(18);
    kruskalAnimateButtonText.setPosition(kruskalAnimateButton.getPosition() + Vector2f(75, 8));

    // Prim Buttons
    primBatchButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    primBatchButton.setPosition(20, 330);
    primBatchButtonText.setFont(font);
    primBatchButtonText.setString("Prim (Instant)");
    primBatchButtonText.setCharacterSize(18);
    primBatchButtonText.setPosition(primBatchButton.getPosition() + Vector2f(95, 8));

    primAnimateButton.setSize(Vector2f(UI_WIDTH - 40, 40));
    primAnimateButton.setPosition(20, 380);
    primAnimateButtonText.setFont(font);
    primAnimateButtonText.setString("Prim (Animate)");
    primAnimateButtonText.setCharacterSize(18);
    primAnimateButtonText.setPosition(primAnimateButton.getPosition() + Vector2f(90, 8));

    // Statistics Section
    statsTitle.setFont(font);
    statsTitle.setString("Statistics");
    statsTitle.setCharacterSize(22);
    statsTitle.setPosition(20, 460);
    statsTitle.setFillColor(Color(255, 255, 153));
    int statTextSize = 19;
    int lineSpacing = 25;
    int startY = 500;

    algorithmText.setFont(font);
    algorithmText.setCharacterSize(statTextSize);
    algorithmText.setFillColor(Color(220, 220, 220));
    algorithmText.setPosition(20, 500);

    weightText.setFont(font);
    weightText.setCharacterSize(statTextSize);
    weightText.setFillColor(Color(220, 220, 220));
    weightText.setPosition(20, 530);


    timeText.setFont(font);
    timeText.setCharacterSize(statTextSize);
    timeText.setFillColor(Color(220, 220, 220));
    timeText.setPosition(20, 560);

    edgeCountText.setFont(font);
    edgeCountText.setCharacterSize(statTextSize);
    edgeCountText.setFillColor(Color(220, 220, 220));
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
    animControlTitle.setFillColor(Color(221, 160, 221));
    animControlTitle.setPosition(20, 640);

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

    // Data Mode Text
    currentMode = DisplayMode::VISUALIZATION;
    dataModeMessage.setFont(font);
    dataModeMessage.setCharacterSize(24);
    dataModeMessage.setFillColor(Color::White);
    dataModeMessage.setPosition(400, 50);

    edgeListText.setFont(font);
    edgeListText.setCharacterSize(16);
    edgeListText.setFillColor(Color::Cyan);
    edgeListText.setPosition(400, 150);

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

        handleTextInput(event, nodeCountInput);

        // Mouse click event
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                // Check if click the input box
                if (nodeCountInput.box.getGlobalBounds().contains(mousePos)) {
                    nodeCountInput.isActive = true;
                } else {
                    nodeCountInput.isActive = false;
                }

                // "Generate Graph" button
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    // Get input
                    int numNodes = stoi(nodeCountInput.input);

                    // Check range
                    if (numNodes >= 2 && numNodes <= 100000) {
                        cout << "Generating graph with " << numNodes << " nodes..." << endl;

                        // Branch into different mode
                        GraphBuildStrategy strategy;

                        if (numNodes <= 1000) {
                            // Visualization mode
                            currentMode = DisplayMode::VISUALIZATION;
                            strategy = GraphBuildStrategy::DELAUNAY;
                            cout << "Mode: VISUALIZATION" << endl;
                        } else {
                            // Data mode
                            currentMode = DisplayMode::DATA_ONLY;
                            strategy = GraphBuildStrategy::K_NEAREST;
                            cout << "Mode: DATA_ONLY" << endl;
                        }

                        graph.generateGraph(numNodes, 1600 - 350, 900, strategy, 5); // K=5 for large scale
                        currentState = AppState::GRAPH_GENERATED;
                        mstResult = MSTResult();
                        isPlaying = false;
                        animationSteps.clear();
                        currentStepIndex = 0;
                    } else {
                        cout << "Invalid input! Please enter 2-100000." << endl;
                        nodeCountInput.input = "200"; // Default
                    }
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
                // Only move if dragging
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

    updateTextInputBox(nodeCountInput, window);

    // Only enable these buttons after the graph is generated
    bool algoButtonsEnabled = (currentState == AppState::GRAPH_GENERATED || currentState == AppState::FINISHED);

    // Only enable animation buttons when algo buttons enable
    bool animateEnabled = algoButtonsEnabled && (currentMode == DisplayMode::VISUALIZATION);

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

    if (animateEnabled) {
        // Kruskal Animate
        if (kruskalAnimateButton.getGlobalBounds().contains(mousePos)) {
            kruskalAnimateButton.setFillColor(Color(100, 100, 100));
        } else {
            kruskalAnimateButton.setFillColor(Color(80, 80, 80));
        }
        // Prim Animate
        if (primAnimateButton.getGlobalBounds().contains(mousePos)) {
            primAnimateButton.setFillColor(Color(100, 100, 100));
        } else {
            primAnimateButton.setFillColor(Color(80, 80, 80));
        }
    } else {
        // Disable
        kruskalAnimateButton.setFillColor(Color(60, 60, 60));
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
    if (currentMode == DisplayMode::VISUALIZATION) {
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
            // Instant button or animation finished.
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
    } else {
        // Large scale mode
        float rightPanelX = 350;
        float rightPanelWidth = 1600 - 350;
        float centerX = rightPanelX + rightPanelWidth / 2.0f;

        // Draw title in the middle
        string statusMsg = "Large Scale Mode (" + to_string(graph.getNumNodes()) + " nodes)\n";
        if (currentState == AppState::GRAPH_GENERATED) {
            statusMsg += "Graph generated. Ready to run algorithms.\nVisualization disabled for performance.";
        } else if (currentState == AppState::FINISHED) {
            statusMsg += "Algorithm Completed: " + mstResult.algorithmName;
        }

        dataModeMessage.setString(statusMsg);
        dataModeMessage.setCharacterSize(20);

        FloatRect titleBounds = dataModeMessage.getLocalBounds();
        dataModeMessage.setOrigin(titleBounds.left + titleBounds.width / 2.0f, 0);
        dataModeMessage.setPosition(centerX, 30);

        window.draw(dataModeMessage);

        // Show a sample of 100 edges
        if (currentState == AppState::FINISHED) {
            string edgeStr = "MST Edge List Sample (First 100 edges):\n";
            edgeStr += string(110, '-') + "\n";
            edgeStr += "Idx  (u - v)   W   |  Idx  (u - v)   W   |  Idx  (u - v)   W   |  Idx  (u - v)   W\n";
            edgeStr += string(110, '-') + "\n";

            int displayLimit = 100;
            int totalEdges = mstResult.mstEdges.size();
            int count = min(totalEdges, displayLimit);

            int rows = (count + 3) / 4;

            for (int i = 0; i < rows; ++i) {
                string line = "";

                for (int col = 0; col < 4; ++col) {
                    int currentIndex = i + (col * rows);

                    if (currentIndex < count) {
                        const auto& edge = mstResult.mstEdges[currentIndex];

                        string item = to_string(currentIndex + 1) + ". (" + to_string(edge.u) + "-" + to_string(edge.v)
                                      + ") " + to_string(edge.weight);

                        if (col < 3) {
                            while (item.length() < 26) {
                                item += " ";
                            }
                            item += "| ";
                        }
                        line += item;
                    }
                }
                edgeStr += line + "\n";
            }

            if (totalEdges > displayLimit) {
                edgeStr += "\n... and " + to_string(totalEdges - displayLimit) + " more edges not shown.";
            }

            edgeListText.setString(edgeStr);
            edgeListText.setCharacterSize(14);
            edgeListText.setLineSpacing(1.3f);
            FloatRect textBounds = edgeListText.getLocalBounds();

            edgeListText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top);

            edgeListText.setPosition(centerX, 130);
            window.draw(edgeListText);
        }
    }

    // Draw left part: controller
    window.draw(uiPanel);
    window.draw(titleText);
    drawTextInputBox(window, nodeCountInput);

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
