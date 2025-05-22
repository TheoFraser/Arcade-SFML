// Ensure this header is only included once per compilation unit
#pragma once

// SFNL graphics module for windowing, drawing, shapes, fonts, etc.
#include <SFML/Graphics.hpp>

// For std::optional used in scoreText
#include <optional>

// Random number generation for fruit spawning
#include <random>

// Vector container used for grid and snake segments
#include <vector>

// Fixed-size array used for keyboard input state tracking
#include <array>

// Main class encapsulating all logic for the Snake game 
class SnakeGame {
public:
    // Constructor
    SnakeGame();

    // Starts the game loop and handles everything from initialisation to rendering
    void startGame();

private:
    // Represents each cell in the grid: visual (shape) and logical (value)
    struct GridCell {
        sf::RectangleShape shape;   // Visual representation (a rectangle on the screen)
        int value = 0;              // 0 = empty, 1 = snake, 2 = fruit
    };

    // Represents a single segment of the snake's body with grid coordinates
    struct SnakeSegment {
        int x, y;
    };

    // Represents a fruit in the game by its location in the grid
    struct Fruit {
        int x, y;
    };

    // Represents a direction mapped to a keyboard key and a index
    struct KeyDir {
        sf::Keyboard::Key key;      // Which key triggers this direction (e.g., Arrows keys)
        sf::Vector2i delta;         // Direction vector (e.g., {0, 1} = right)
        int index;                  // Index used for input debouce tracking
    };

    // Dimensions of the game grid
    const int GRID_COLS = 24;       // Number of columns in the grid
    const int GRID_ROWS = 12;       // Number of rows in the grid

    // How often the snake moves (in seconds)
    const float moveInterval = 0.2f;

    // Current window dimenstions in pixels
    int windowWidth, windowHeight;
    
    // Size of one cell (box) in the grid, computed from window dimensions
    float boxWidth, boxHeight;

    // Track elapsed time between movement updates
    float timer = 0.f;

    // Whether the game is in fullscreen mode or windowed mode
    bool fullScreen = true;

    // Used to prevent repeated toggling of fullscreen with one keypress
    bool f11PreviouslyPressed = false;

    // Playe's current score
    int score = 0;

    // The main game window
    sf::RenderWindow window;
    
    // Hold screen resolution for fullscreen
    sf::VideoMode desktop;

    // Font used for displaying text
    sf::Font font;

    // Clock used to time movement and redering
    sf::Clock clock;

    // Optional score text element, only intialised if font loads
    std::optional<sf::Text> scoreText;

    // Current Direction the snake is moving
    sf::Vector2i direction = {0, 1};        // Starts moving right

    // Next direction, updated via input
    sf::Vector2i nextDirection = {0, 1};

    // 2D grid of cells representing the game field
    std::vector<std::vector<GridCell>> grid;

    // Vector of segments that make up the snake body
    std::vector<SnakeSegment> snake;

    // Vector of currently placed fruits (usually one)
    std::vector<Fruit> fruits;

    // Keeps track of whether input keys are currently pressed (debounce logic)
    std::array<bool, 5> keysPressed{};

    // Hardware-based random device (used for seeding)
    std::random_device rd;

    // Mersenne Twister RNG engine
    std::mt19937 gen{rd()};

    // List of possible directions and their assciated keys
    std::vector<KeyDir> directions = {
        {sf::Keyboard::Key::Up, {-1, 0}, 1},    // Move up
        {sf::Keyboard::Key::Down, {1, 0}, 2},   // Move down
        {sf::Keyboard::Key::Right, {0, 1}, 3},  // Move right
        {sf::Keyboard::Key::Left, {0, -1}, 4}   // Move left
    };

    // Initialises size and position of a cell based on its grid location
    void setupCell(GridCell& cell, int row, int col);

    // Set a call's value and visual colour
    void setGridCell(int x, int y, int value, sf::Color color);

    // Handles movement logic including collisions and fruit detection
    void moveSnake();

    // Updates the visual grid based on game state
    void updateGrid();

    // Handles keyboard input (directional control, fullscreen toggle, etc.)
    void handleKeyInputs();
    
    // Called when window is resized to update layout
    void resizeWindow(int w, int h);

    // Builds the initial grid at game start
    void createGrid();

    // Randomly places a fruit on an empty cell
    void createFruit();
};
