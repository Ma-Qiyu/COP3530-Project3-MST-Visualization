#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "MSTBuilder.h"

using namespace std;
using namespace sf;

enum class AppState {
    INITIAL,
    GRAPH_GENERATED,
    ANIMATING,
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

    // Animation related
    vector<AnimationStep> animationSteps;
    int currentStepIndex;
    bool isPlaying;
    float animationDelaySeconds; // Delay between steps
    Clock stepTimer;
    bool isDraggingSlider; // For slider

    // Initialization section
    RectangleShape uiPanel;
    Text titleText;
    Text settingsTitle;
    RectangleShape generateButton;
    Text generateButtonText;

    // Run algorithm section
    Text runTitle;
    Text runModeTitle;
    RectangleShape kruskalBatchButton;
    Text kruskalBatchButtonText;
    RectangleShape kruskalAnimateButton;
    Text kruskalAnimateButtonText;
    RectangleShape primBatchButton;
    Text primBatchButtonText;
    RectangleShape primAnimateButton;
    Text primAnimateButtonText;

    // Statistics section
    Text statsTitle;
    Text algorithmText;
    Text weightText;
    Text timeText;
    Text edgeCountText;

    // Animation control section
    Text animControlTitle;
    RectangleShape playPauseButton;
    Text playPauseButtonText;
    Text speedSliderText; // Speed slider
    RectangleShape speedSliderBack;
    RectangleShape speedSliderHandle;
};
