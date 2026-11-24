#pragma once
#include <vector>
#include "Node.h"

class GlobalState {
private:
    static GlobalState* instance;
    std::vector<Node*> nodes; // Χρήση STL 
    std::vector<Node*> path;
    std::vector<Node*> openSet; // Λίστα για τον αλγόριθμο
    Node* startNode = nullptr, * endNode = nullptr;
    int cols = 25, rows = 15;
    float cell_size = 40.0f;

    bool isAnimating = false;
    bool isPaused = false;
    bool wasSPressed = false;
    float animationTimer = 0.0f;

    GlobalState() {}

public:
    static GlobalState* getInstance();
    void init();
    void update(float dt);
    void draw();

    void runAStar(); // Αλγόριθμος A-star  (space)
    void startAStarAnimation(); // Προετοιμασία για αργή εκτέλεση (Enter)
    void stepAStar(); // Εκτελεί ΕΝΑ βήμα του αλγορίθμου

    std::vector<Node*> getNeighbors(Node* n);
    void reset();
    // Helpers για αλλαγή τύπου κόμβου
    void setNodeToWall(int idx, int c, int r);
    void setNodeToEmpty(int idx, int c, int r);

    ~GlobalState();
};