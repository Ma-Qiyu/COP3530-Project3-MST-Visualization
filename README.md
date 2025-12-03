# MST Visualization

COP3530 Project 3

A program implements and compares two distinct algorithms, Prim’s Algorithm and Kruskal’s
Algorithm, to construct the Minimum Spanning Tree (MST) and visualize their process using
animation.

## How to Run Code

This project requires a **C++17 compiler**, and the **SFML** (Simple and Fast Multimedia Library) development
libraries (Version 2.5+).

### Dependencies

* **MacOS (Homebrew):** `brew install sfml`
* **Linux (Debian/Ubuntu):** `sudo apt-get install libsfml-dev build-essential`
* **Windows:** Download the SFML SDK and configure the path in CMake.

### Clone the Repository

```angular2html
git clone https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
cd YOUR_REPO_NAME
```

### Compilation and Execution

#### Linux / macOS

```angular2html
# Compile
g++ -std=c++17 main.cpp Graph.cpp MSTBuilder.cpp TextInputBox.cpp UIController.cpp \
-o mst_visualizer \
-I./lib \
-lsfml-graphics -lsfml-window -lsfml-system

# Run
./mst_visualizer
```
