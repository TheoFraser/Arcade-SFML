#include "SnakeGame.hpp" // Include the header for the SnakeGame Class
#include "arial_font.hpp" // Include the embedded font data

// Constructor
SnakeGame::SnakeGame() {}

// Function to configure the size and position of a single grid cell on the screen
void SnakeGame::setupCell(GridCell& cell, int row, int col) {
    cell.shape.setSize(sf::Vector2f(boxWidth - 1, boxHeight - 1));          // Set the size of each cell with a border gap of 1 pixel
    cell.shape.setPosition(sf::Vector2f(col * boxWidth, row * boxHeight));  // Set the position of the cell based on its row and column
}

// Set the value of a specific cell in the grid
void SnakeGame::setGridCell(int x, int y, int value, sf::Color color) {
    grid[x][y].value = value;               // value (0 = empty, 1 = snake, 2 = fruit)
    grid[x][y].shape.setFillColor(color);   // Change the colour of the cell for visual representation
}

// Handles the logic for moving the snake one grid cell
void SnakeGame::moveSnake() {
    direction = nextDirection;              // Update the direction to the most recent direction

    // Calculate new head coordinates by adding direction to current head position
    int headX = snake.front().x + direction.x;
    int headY = snake.front().y + direction.y;

    // Collision detection: if the snake hits the wall or itself
    if (headX < 0 || headX >= GRID_ROWS || headY < 0 || headY >= GRID_COLS || grid[headX][headY].value == 1) {
        window.close();                     // End the game by closing the window
        return;
    }

    // Add new head position to the beginning of the snake body vector
    snake.insert(snake.begin(), {headX, headY});

    bool ateFruit = false;                  // Flag to determine whether the snake eats a fruit

    // Loop through all fruits to see if one is at the new head location
    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
        if (it->x == headX && it->y == headY) {
            ateFruit = true;                // Mark that a fruit was eaten
            fruits.erase(it);               // Remove fruit from list
            createFruit();                  // Generate a new fruit
            score++;                        // Increase score

            // Update the score displa string if it's initialised
            if (scoreText) scoreText->setString("Score " + std::to_string(score));
            break;                          // Exit the loop since only one fruit can be eaten at a time
        }
    }

    // IF no fruit was eaten, remove the last segment of the snake (tail) to keep the length the same
    if (!ateFruit) snake.pop_back();

    // Update grid display to reflect new position of the snake and fruit
    updateGrid();
}

// Redraw the entire grid based on the current game state
void SnakeGame::updateGrid() {
    // Iterate through every cell in the grid
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            // Only reset cells that aren't fruit (fruit should stay until eaten)
            if (grid[row][col].value != 2) { 
                grid[row][col].value = 0;   // Mark as empty
                grid[row][col].shape.setFillColor(sf::Color::Black);    // Colour empty cells black
            }
        }
    }

    // Draw the snake on the grid by marking its segments
    for (const auto& seg : snake)
        setGridCell(seg.x, seg.y, 1, sf::Color::Red);   // Snake segments are red
}

// Detects and processed keyboard inputs (direction, pause, fullscrenn)
void SnakeGame::handleKeyInputs() {
    // Toggle fullscreen when F11 is pressed on the keyboard
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11)) {
        if (!f11PreviouslyPressed) {        // Prevents toggling multiple times per keypress
            fullScreen = !fullScreen;       // Toggle fullscreen state

            // Recreate the window in either fullscreen or windowed mode
            window.create(
                fullScreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({1280, 720}),
                "Snake", fullScreen ? sf::State::Fullscreen : sf::State::Windowed
            );
            resizeWindow(window.getSize().x, window.getSize().y);   // Resize grid based on new window size
            f11PreviouslyPressed = true;
        }
    } else {
        f11PreviouslyPressed = false;       // Reset flag when key is released
    }

    // Close game if Escape is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

    // Handle directional input (arrow keys mapped in 'directions')
    for (const auto& dir : directions) {
        // Only alow turning in perpendicular directions
        bool valid = (dir.delta.x != 0 && direction.x == 0) || (dir.delta.y != 0 && direction.y == 0);
        if (sf::Keyboard::isKeyPressed(dir.key)) {
            if (!keysPressed[dir.index] && valid) {
                nextDirection = dir.delta;      // Set the next direction for movement
                keysPressed[dir.index] = true;  // Mark key as held to prevent repeated input
            }
        } else {
            keysPressed[dir.index] = false;     // Reset key held state when released 
        }
    }
}

// Adjust game elements when the window is resized
void SnakeGame::resizeWindow(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    boxWidth = w / GRID_COLS;   // Calculate width of each grid cell
    boxHeight = h / GRID_ROWS;  // Calculate height of each grid cell

    // Re-position all cells based on new sizes
    for (int row = 0; row < GRID_ROWS; ++row)
        for (int col = 0; col < GRID_COLS; ++col)
            setupCell(grid[row][col], row, col);

    // Resize and reposition score text.
    if (scoreText) {
        scoreText->setPosition(sf::Vector2f(boxWidth, boxHeight - 1));
        scoreText->setCharacterSize(20);
    }
}

// Creates and configures the game grid
void SnakeGame::createGrid() {
    boxWidth = windowWidth / GRID_COLS;     // Set cell width
    boxHeight = windowHeight / GRID_ROWS;   // Set cell height
    grid.resize(GRID_ROWS, std::vector<GridCell>(GRID_COLS));   // Create 2D grid of cells

    // Setup each grid cell
    for (int row = 0; row < GRID_ROWS; ++row)
        for (int col = 0; col < GRID_COLS; ++col)
            setupCell(grid[row][col], row, col);
}

// Adds new fruit to a random empty location in the gid
void SnakeGame::createFruit() {
    std::uniform_int_distribution<> xDist(0, GRID_ROWS - 1);    // Random row
    std::uniform_int_distribution<> yDist(0, GRID_COLS - 1);    // Random column
    int x, y;
    // Find a random position that is currently empty
    do {
        x = xDist(gen);
        y = yDist(gen);
    } while (grid[x][y].value != 0);    // Repeat if cell is not empty

    setGridCell(x, y, 2, sf::Color::Green); // Set cell as fruit
    fruits.push_back({x, y});               // Add fruit to list
}

void SnakeGame::startGame() {
    // Load font from memory: return if failed
    if (!font.openFromMemory(arial_ttf, sizeof(arial_ttf))) return;

    desktop = sf::VideoMode::getDesktopMode();  // Get desktop resolution
    window.create(desktop, "Snake", sf::State::Fullscreen); // Open fullscreen window
    window.setFramerateLimit(60);               // Limit frame rate to 60 FPS

    windowWidth = window.getSize().x;
    windowHeight = window.getSize().y;

    createGrid();                               // Initialise grid cells

    // Setup score text display
    scoreText.emplace(font);
    scoreText->setCharacterSize(30);
    scoreText->setString("Score 0");
    scoreText->setPosition(sf::Vector2f(boxWidth, boxHeight));

    // Start the snake with two segments
    snake.push_back({4, 6});
    snake.push_back({4, 7});

    createFruit();                              // Spawn the first fruit

    // Main game loop: runs while window is open
    while (window.isOpen()) {
        sf::Time dt = clock.restart();          // Get time since last frame
        timer += dt.asSeconds();                // Accumulate time

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        handleKeyInputs();                      // Process user inputs

        // Move snake based on the accumulated time interval
        if (timer >= moveInterval) {
            timer = 0.f;
            moveSnake();                        // Move the snake forward 
        }

        // Clear previous frame
        window.clear(sf::Color::Black);

        // Draw all grid cells
        for (const auto& row : grid)
            for (const auto& cell : row)
                window.draw(cell.shape);

        // Draw the score text
        if (scoreText) window.draw(*scoreText);

        window.display();                       // Show updated frame
    }
}
