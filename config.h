#pragma once

// Διαδρομή για τα assets (ήχοι, εικόνες) 
#define ASSET_PATH "assets\\"

// Ρυθμίσεις Παραθύρου και Καμβά SGG 
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define CANVAS_WIDTH  (GRID_COLS * CELL_SIZE)  // 1000
#define CANVAS_HEIGHT (GRID_ROWS * CELL_SIZE)  // 600

// Ρυθμίσεις Πλέγματος (Grid)
#define GRID_ROWS 15
#define GRID_COLS 25
#define CELL_SIZE 40.0f

// Χρώματα (RGB)
#define COLOR_WALL 0.1f, 0.1f, 0.1f
#define COLOR_PATH 1.0f, 0.8f, 0.0f
#define COLOR_START 0.2f, 0.8f, 0.2f
#define COLOR_END 0.8f, 0.2f, 0.2f

// Ήχοι
#define SOUND_WALL  "click.mp3"
#define SOUND_ERASE "erase.mp3"
#define SOUND_STEP  "step.mp3"
#define SOUND_WIN   "win.mp3"