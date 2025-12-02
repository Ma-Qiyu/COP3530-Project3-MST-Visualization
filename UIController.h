#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "MSTBuilder.h"

using namespace std;
using namespace sf;

enum class AppState {
    INITIAL,
    GRAPH_GENERATED,
    BUILDING_MST,
    FINISHED
};


class UIController {
public:
    UIController();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    RenderWindow window;
    Font font;

    AppState currentState;
    Graph graph;
    MSTBuilder builder;
    MSTResult mstResult;

    // Initialization section
    RectangleShape uiPanel;
    Text titleText;
    Text settingsTitle;
    RectangleShape generateButton;
    Text generateButtonText;

    // Run section
    Text runTitle;
    RectangleShape kruskalButton;
    Text kruskalButtonText;
    RectangleShape primButton;
    Text primButtonText;

    // Statistics section
    Text statsTitle;
    Text algorithmText;
    Text weightText;
    Text timeText;
    Text edgeCountText;
};
