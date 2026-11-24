#include "Node.h"
#include "config.h"

Node::Node(int c, int r, float sz) : col(c), row(r), size(sz) {
    x = col * size + size / 2.0f;
    y = row * size + size / 2.0f;
}

void Node::draw() {
    graphics::Brush br;
    br.outline_opacity = 0.3f;

    // ΠΡΟΤΕΡΑΙΟΤΗΤΑ 1: Αν είναι ΤΟΙΧΟΣ -> Μαύρο
    if (is_wall) {
        br.fill_color[0] = 0.1f;
        br.fill_color[1] = 0.1f;
        br.fill_color[2] = 0.1f;
    }
    // ΠΡΟΤΕΡΑΙΟΤΗΤΑ 2: Αν είναι VISITED -> Γαλάζιο
    else if (visited) {
        br.fill_color[0] = 0.7f;
        br.fill_color[1] = 0.8f;
        br.fill_color[2] = 1.0f;
    }
    // ΠΡΟΤΕΡΑΙΟΤΗΤΑ 3: ΚΕΝΟ -> Λευκό/Γκρι
    else {
        br.fill_color[0] = 0.9f;
        br.fill_color[1] = 0.9f;
        br.fill_color[2] = 0.9f;
    }

    graphics::drawRect(x, y, size * 0.9f, size * 0.9f, br);
}

// Η WallNode::draw δεν χρειάζεται πρακτικά πλέον, αλλά την αφήνουμε για συμβατότητα
void WallNode::draw() {
    graphics::Brush br;
    br.fill_color[0] = 0.1f; br.fill_color[1] = 0.1f; br.fill_color[2] = 0.1f;
    graphics::drawRect(x, y, size, size, br);
}