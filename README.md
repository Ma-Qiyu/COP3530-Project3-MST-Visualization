# MST Visualization

COP3530 Project 3

A program implements and compares two distinct algorithms, Prim’s Algorithm and Kruskal’s
Algorithm, to construct the Minimum Spanning Tree (MST) and visualize their process using
animation.

## How to Run Code

This project requires a **C++17 compiler**（GCC, Clang, or MSVC）, and the **SFML** (Simple and Fast Multimedia Library)
development
libraries (Version 2.5+).

### Dependencies

* **MacOS (Homebrew):** `brew install sfml`
* **Linux (Debian/Ubuntu):** `sudo apt-get install libsfml-dev build-essential`
* **Windows:** Download precompiled binaries from [SFML Downloads](https://www.sfml-dev.org/download.php)

### Clone the Repository

```angular2html
git clone https://github.com/Ma-Qiyu/COP3530-Project3-MST-Visualization.git
cd COP3530-Project3-MST-Visualization
```

### Compilation and Execution

#### Linux / macOS

```angular2html
# Compile
g++ -std=c++17 -Wall -I./lib -I/opt/homebrew/include -L/opt/homebrew/lib main.cpp Graph.cpp MSTBuilder.cpp TextInputBox.cpp UIController.cpp -o COP3530_Final_Project_MST -lsfml-graphics -lsfml-window -lsfml-system

# Copy font file
cp lib/Roboto-Regular.ttf font.ttf

# Run
./COP3530_Final_Project_MST
```

#### Windows (MinGW)

```angular2html
# Compile
g++ -std=c++17 -Wall -I./lib main.cpp Graph.cpp MSTBuilder.cpp TextInputBox.cpp UIController.cpp -o COP3530_Final_Project_MST.exe -lsfml-graphics -lsfml-window -lsfml-system

# Copy font file
copy lib\Roboto-Regular.ttf font.ttf

# Run
.\COP3530_Final_Project_MST.exe
```

#### Using Makefile

```angular2html
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./lib
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = main.cpp Graph.cpp MSTBuilder.cpp TextInputBox.cpp UIController.cpp
TARGET = COP3530_Final_Project_MST.exe

all: $(TARGET)

$(TARGET): $(SOURCES)
$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
del $(TARGET)

run: $(TARGET)
.\$(TARGET)

```