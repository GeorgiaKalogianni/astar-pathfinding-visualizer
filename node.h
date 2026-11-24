#pragma once
#include "graphics.h"

// Βασική κλάση (Base Class) 
class Node {
public:
    int col, row;
    float x, y, size;
    float g_cost = 1e9, h_cost = 1e9;
    Node* parent = nullptr;
    bool is_wall = false;
    bool visited = false;

    Node(int c, int r, float sz);
    virtual ~Node() {}
    virtual void draw(); // Πολυμορφική μέθοδος 
    float f_cost() const { return g_cost + h_cost; }
};

// Παράγωγη κλάση για εμπόδια (Inheritance)
class WallNode : public Node {
public:
    WallNode(int c, int r, float sz) : Node(c, r, sz) { is_wall = true; }
    void draw() override;
};