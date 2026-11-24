#include "GlobalState.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "config.h"
#include "graphics.h"

GlobalState* GlobalState::instance = nullptr;

GlobalState* GlobalState::getInstance() {
    if (!instance) instance = new GlobalState();
    return instance;
}

void GlobalState::init() {
    // 1. Clean Memory
    if (!nodes.empty()) {
        for (auto n : nodes) delete n;
        nodes.clear();
    }
    // 2. Create Grid
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            nodes.push_back(new Node(c, r, CELL_SIZE));
        }
    }
    // 3. Set Start/End
    if (nodes.size() > 0) {
        startNode = nodes[0];
        endNode = nodes[nodes.size() - 1];
    }
}

void GlobalState::update(float dt) {
    graphics::MouseState ms;
    graphics::getMouseState(ms);

    float mx = graphics::windowToCanvasX(ms.cur_pos_x);
    float my = graphics::windowToCanvasY(ms.cur_pos_y);

    int c = (int)(mx / CELL_SIZE);
    int r = (int)(my / CELL_SIZE);

    // --- PAUSE (S) ---
    bool s_pressed = graphics::getKeyState(graphics::SCANCODE_S);
    if (s_pressed && !wasSPressed) {
        if (isAnimating) {
            isPaused = !isPaused;

            if (isPaused) {
                // Αν παγώσαμε -> ΣΤΑΜΑΤΑ τον ήχο
                graphics::stopMusic();
                std::cout << "PAUSED" << std::endl;
            }
            else {
                // Αν συνεχίζουμε -> ΞΕΚΙΝΑ τον ήχο (looping)
                graphics::playMusic(std::string(ASSET_PATH) + SOUND_STEP, 0.4f, true);
                std::cout << "RESUMED" << std::endl;
            }
        }
    }
    wasSPressed = s_pressed;

    // --- MOUSE LOGIC ---
    if (c >= 0 && c < GRID_COLS && r >= 0 && r < GRID_ROWS) {
        int idx = r * GRID_COLS + c;
        if (idx >= 0 && idx < nodes.size()) {

            bool isLocked = isAnimating || !path.empty() || !openSet.empty();
            bool key1 = graphics::getKeyState(graphics::SCANCODE_1);
            bool key2 = graphics::getKeyState(graphics::SCANCODE_2);

            // 1. Move START
            if (key1 && ms.button_left_down) {
                if (!isLocked && nodes[idx] != endNode) {
                    startNode = nodes[idx];
                    startNode->is_wall = false;
                    reset();
                }
            }
            // 2. Move END
            else if (key2 && ms.button_left_down) {
                if (!isLocked && nodes[idx] != startNode) {
                    endNode = nodes[idx];
                    endNode->is_wall = false;
                    reset();
                }
            }
            // 3. Edit WALLS
            else if (!key1 && !key2 && nodes[idx] != startNode && nodes[idx] != endNode) {
                if (ms.button_left_down) {
                    bool inOpenSet = std::find(openSet.begin(), openSet.end(), nodes[idx]) != openSet.end();
                    if (!nodes[idx]->is_wall && !nodes[idx]->visited && !inOpenSet) {
                        setNodeToWall(idx, c, r);
                    }
                }
                else if (ms.button_right_down) {
                    if (nodes[idx]->is_wall) {
                        bool hasVisitedNeighbor = false;
                        std::vector<Node*> neighbors = getNeighbors(nodes[idx]);
                        for (Node* n : neighbors) {
                            if (n->visited) { hasVisitedNeighbor = true; break; }
                        }
                        if (!hasVisitedNeighbor) setNodeToEmpty(idx, c, r);
                    }
                }
            }
        }
    }

    // --- KEYBOARD ---
    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        // Space -> Instant -> ΟΧΙ μουσική
        graphics::stopMusic();
        isAnimating = false;
        isPaused = false;
        runAStar();
    }
    if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
        if (!isAnimating) startAStarAnimation();
    }
    if (graphics::getKeyState(graphics::SCANCODE_R)) {
        graphics::stopMusic(); // Ασφάλεια
        reset();
    }

    // --- ANIMATION LOOP ---
    if (isAnimating && !isPaused) {
        animationTimer += dt;
        if (animationTimer > 50.0f) { // Ταχύτητα Animation
            stepAStar();
            animationTimer = 0.0f;
        }
    }
}

// --- HELPERS ---
void GlobalState::setNodeToWall(int idx, int c, int r) {
    nodes[idx]->is_wall = true;
    nodes[idx]->visited = false;
    graphics::playSound(std::string(ASSET_PATH) + SOUND_WALL, 0.5f, false);
}

void GlobalState::setNodeToEmpty(int idx, int c, int r) {
    nodes[idx]->is_wall = false;
    graphics::playSound(std::string(ASSET_PATH) + SOUND_ERASE, 0.5f, false);
}

// --- ALGORITHMS ---
void GlobalState::runAStar() {
    startAStarAnimation();
    isAnimating = false;

    // Αφού τρέχουμε Instant, σταματάμε αμέσως τη μουσική που ξεκίνησε η startAStarAnimation
    graphics::stopMusic();

    while (!openSet.empty() && path.empty()) {
        stepAStar();
    }

    // Αν βρήκαμε λύση στο Instant, παίξε Win
    if (!path.empty()) {
        graphics::playSound(std::string(ASSET_PATH) + SOUND_WIN, 1.0f, false);
    }
}

void GlobalState::startAStarAnimation() {
    for (auto n : nodes) {
        n->g_cost = 1e9;
        n->visited = false;
        n->parent = nullptr;
    }
    path.clear();
    openSet.clear();

    if (startNode) {
        startNode->g_cost = 0;
        startNode->h_cost = (float)std::hypot(startNode->col - endNode->col, startNode->row - endNode->row);
        openSet.push_back(startNode);
    }
    isAnimating = true;
    isPaused = false;
    animationTimer = 0.0f;

    // ΕΝΑΡΞΗ ΜΟΥΣΙΚΗΣ LOOP
    // Το 'true' στο τέλος σημαίνει ότι θα παίζει συνέχεια
    graphics::playMusic(std::string(ASSET_PATH) + SOUND_STEP, 0.4f, true);
}

void GlobalState::stepAStar() {
    if (openSet.empty()) {
        isAnimating = false;
        graphics::stopMusic(); // Τέλος αναζήτησης -> Στοπ ήχος
        return;
    }

    auto it = std::min_element(openSet.begin(), openSet.end(),
        [](Node* a, Node* b) { return a->f_cost() < b->f_cost(); });

    Node* current = *it;

    // --- WIN CONDITION ---
    if (current == endNode) {
        Node* temp = current;
        while (temp != nullptr) {
            path.push_back(temp);
            temp = temp->parent;
        }
        isAnimating = false;

        // Βρέθηκε λύση!
        graphics::stopMusic(); // Σταματάμε το "Step"
        graphics::playSound(std::string(ASSET_PATH) + SOUND_WIN, 1.0f, false); // Παίζουμε το Win
        return;
    }

    openSet.erase(it);
    current->visited = true;

    for (Node* neighbor : getNeighbors(current)) {
        if (neighbor->is_wall || neighbor->visited) continue;

        float tentative_g = current->g_cost + 1;
        if (tentative_g < neighbor->g_cost) {
            neighbor->parent = current;
            neighbor->g_cost = tentative_g;
            neighbor->h_cost = (float)std::hypot(neighbor->col - endNode->col, neighbor->row - endNode->row);

            if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                openSet.push_back(neighbor);
            }
        }
    }
}

void GlobalState::draw() {
    for (auto n : nodes) n->draw();
    graphics::Brush br;

    // OpenSet
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 0.5f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.5f;
    br.fill_opacity = 0.5f;
    for (auto n : openSet) {
        graphics::drawRect(n->x, n->y, CELL_SIZE * 0.5f, CELL_SIZE * 0.5f, br);
    }

    // Path
    if (!path.empty()) {
        br.outline_opacity = 1.0f;
        br.fill_opacity = 1.0f;
        br.fill_color[0] = 1.0f; br.fill_color[1] = 0.8f; br.fill_color[2] = 0.0f;
        for (size_t i = 0; i < path.size() - 1; i++) {
            graphics::drawLine(path[i]->x, path[i]->y, path[i + 1]->x, path[i + 1]->y, br);
        }
    }

    // Start / End
    if (startNode && endNode) {
        br.outline_opacity = 1.0f;
        br.fill_color[0] = 0.2f; br.fill_color[1] = 0.8f; br.fill_color[2] = 0.2f;
        graphics::drawRect(startNode->x, startNode->y, CELL_SIZE * 0.7f, CELL_SIZE * 0.7f, br);
        br.fill_color[0] = 0.8f; br.fill_color[1] = 0.2f; br.fill_color[2] = 0.2f;
        graphics::drawRect(endNode->x, endNode->y, CELL_SIZE * 0.7f, CELL_SIZE * 0.7f, br);
    }
}

std::vector<Node*> GlobalState::getNeighbors(Node* n) {
    std::vector<Node*> res;
    int d[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
    for (auto i : d) {
        int nc = n->col + i[0];
        int nr = n->row + i[1];
        if (nc >= 0 && nc < GRID_COLS && nr >= 0 && nr < GRID_ROWS) {
            res.push_back(nodes[nr * GRID_COLS + nc]);
        }
    }
    return res;
}

void GlobalState::reset() {
    for (auto n : nodes) {
        n->visited = false;
        n->g_cost = 1e9;
        n->parent = nullptr;
    }
    path.clear();
    openSet.clear();
    isAnimating = false;
    isPaused = false;
}

GlobalState::~GlobalState() {
    for (auto n : nodes) delete n;
}