// Ensure this header is only included once per compilation unit
#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <random>
#include <vector>
#include <array>

class SnakeGame {
public:
    SnakeGame();
    void startGame();

private:
    struct GridCell {
        sf::RectangleShape shape;
        int value = 0; // 0 = empty, 1 = snake, 2 = fruit
    };

    struct SnakeSegment {
        int x, y;
    };

    struct Fruit {
        int x, y;
    };

    struct KeyDir {
        sf::Keyboard::Key key;
        sf::Vector2i delta;
        int index;
    };

    // Constants
    const int GRID_COLS = 24;
    const int GRID_ROWS = 12;
    const float moveInterval = 0.2f;

    // Window and game state
    int windowWidth, windowHeight;
    float boxWidth, boxHeight;
    float timer = 0.f;
    bool fullScreen = true;
    bool f11PreviouslyPressed = false;
    int score = 0;

    // SFML components
    sf::RenderWindow window;
    sf::VideoMode desktop;
    sf::Font font;
    sf::Clock clock;
    std::optional<sf::Text> scoreText;

    // Game data
    sf::Vector2i direction = {0, 1};
    sf::Vector2i nextDirection = {0, 1};
    std::vector<std::vector<GridCell>> grid;
    std::vector<SnakeSegment> snake;
    std::vector<Fruit> fruits;
    std::array<bool, 5> keysPressed{};

    // Random number generator
    std::random_device rd;
    std::mt19937 gen{rd()};

    // Input directions
    std::vector<KeyDir> directions = {
        {sf::Keyboard::Key::Up, {-1, 0}, 1},
        {sf::Keyboard::Key::Down, {1, 0}, 2},
        {sf::Keyboard::Key::Right, {0, 1}, 3},
        {sf::Keyboard::Key::Left, {0, -1}, 4}
    };

    // Internal methods
    void setupCell(GridCell& cell, int row, int col);
    void setGridCell(int x, int y, int value, sf::Color color);
    void moveSnake();
    void updateGrid();
    void handleKeyInputs();
    void resizeWindow(int w, int h);
    void createGrid();
    void createFruit();
};
