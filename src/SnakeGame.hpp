// Ensure this header is only included once per compilation unit
#pragma once


#include <SFML/Graphics.hpp>    // SFNL graphics module for windowing, drawing, shapes, fonts, etc.
#include <optional>             // For std::optional used in scoreText
#include <random>               // Random number generation for fruit spawning
#include <vector>               // Vector container used for grid and snake segments
#include <array>                // Fixed-size array used for keyboard input state tracking

// Main class encapsulating all logic for the Snake game 
class SnakeGame {
public:
    SnakeGame();                // Constructor
    void startGame();           // Starts the game loop and handles everything from initialisation to rendering

private:
    struct GridCell {               // Represents each cell in the grid: visual (shape) and logical (value)
        sf::RectangleShape shape;   // Visual representation (a rectangle on the screen)
        int value = 0;              // 0 = empty, 1 = snake, 2 = fruit
    };

    struct SnakeSegment {           // Represents a single segment of the snake's body with grid coordinates
        int x, y;
    };

    struct Fruit {                  // Represents a fruit in the game by its location in the grid
        int x, y;
    };

    struct KeyDir {                 // Represents a direction mapped to a keyboard key and a index
        sf::Keyboard::Key key;      // Which key triggers this direction (e.g., Arrows keys)
        sf::Vector2i delta;         // Direction vector (e.g., {0, 1} = right)
        int index;                  // Index used for input debouce tracking
    };

    // Dimensions of the game grid
    const int GRID_COLS = 24;           // Number of columns in the grid
    const int GRID_ROWS = 12;           // Number of rows in the grid

    const float moveInterval = 0.2f;    // How often the snake moves (in seconds)
    int windowWidth, windowHeight;      // Current window dimenstions in pixels
    float boxWidth, boxHeight;          // Size of one cell (box) in the grid, computed from window dimensions
    float timer = 0.f;                  // Track elapsed time between movement updates

    bool fullScreen = true;             // Whether the game is in fullscreen mode or windowed mode
    bool f11PreviouslyPressed = false;  // Used to prevent repeated toggling of fullscreen with one keypress
    int score = 0;                      // Playe's current score


    sf::RenderWindow window;                // The main game window
    sf::VideoMode desktop;                  // Hold screen resolution for fullscreen
    sf::Font font;                          // Font used for displaying text
    sf::Clock clock;                        // Clock used to time movement and redering
    std::optional<sf::Text> scoreText;      // Optional score text element, only intialised if font loads
    sf::Vector2i direction = {0, 1};        // Current direction the snake is moving. Starting direction is right
    sf::Vector2i nextDirection = {0, 1};    // Next direction, updated via input

    std::vector<std::vector<GridCell>> grid;    // 2D grid of cells representing the game field
    std::vector<SnakeSegment> snake;            // Vector of segments that make up the snake body
    std::vector<Fruit> fruits;                  // Vector of currently placed fruits (usually one)
    std::array<bool, 5> keysPressed{};          // Keeps track of whether input keys are currently pressed (debounce logic)

    std::random_device rd;                      // Hardware-based random device (used for seeding)
    std::mt19937 gen{rd()};                     // Mersenne Twister RNG engine

    std::vector<KeyDir> directions = {          // List of possible directions and their assciated keys
        {sf::Keyboard::Key::Up, {-1, 0}, 1},    // Move up
        {sf::Keyboard::Key::Down, {1, 0}, 2},   // Move down
        {sf::Keyboard::Key::Right, {0, 1}, 3},  // Move right
        {sf::Keyboard::Key::Left, {0, -1}, 4}   // Move left
    };

    void setupCell(GridCell& cell, int row, int col);               // Initialises size and position of a cell based on its grid location
    void setGridCell(int x, int y, int value, sf::Color color);     // Set a call's value and visual colour
    void moveSnake();                                               // Handles movement logic including collisions and fruit detection
    void updateGrid();                                              // Updates the visual grid based on game state
    void handleKeyInputs();                                         // Handles keyboard input (directional control, fullscreen toggle, etc.)
    void resizeWindow(int w, int h);                                // Called when window is resized to update layout
    void createGrid();                                              // Builds the initial grid at game start
    void createFruit();                                             // Randomly places a fruit on an empty cell
};
