#include "SnakeGame.hpp"
#include "arial_font.hpp"

SnakeGame::SnakeGame() {}

void SnakeGame::setupCell(GridCell& cell, int row, int col) {
    cell.shape.setSize(sf::Vector2f(boxWidth - 1, boxHeight - 1));
    cell.shape.setPosition(sf::Vector2f(col * boxWidth, row * boxHeight));
}

void SnakeGame::setGridCell(int x, int y, int value, sf::Color color) {
    grid[x][y].value = value;
    grid[x][y].shape.setFillColor(color);
}

void SnakeGame::moveSnake() {
    direction = nextDirection;
    int headX = snake.front().x + direction.x;
    int headY = snake.front().y + direction.y;

    if (headX < 0 || headX >= GRID_ROWS || headY < 0 || headY >= GRID_COLS || grid[headX][headY].value == 1) {
        window.close();
        return;
    }

    snake.insert(snake.begin(), {headX, headY});

    bool ateFruit = false;
    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
        if (it->x == headX && it->y == headY) {
            ateFruit = true;
            fruits.erase(it);
            createFruit();
            score++;
            if (scoreText) scoreText->setString("Score " + std::to_string(score));
            break;
        }
    }

    if (!ateFruit) snake.pop_back();
    updateGrid();
}

void SnakeGame::updateGrid() {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            if (grid[row][col].value != 2) {
                grid[row][col].value = 0;
                grid[row][col].shape.setFillColor(sf::Color::Black);
            }
        }
    }

    for (const auto& seg : snake)
        setGridCell(seg.x, seg.y, 1, sf::Color::Red);
}

void SnakeGame::handleKeyInputs() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11)) {
        if (!f11PreviouslyPressed) {
            fullScreen = !fullScreen;
            window.create(
                fullScreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({1280, 720}),
                "Snake", fullScreen ? sf::State::Fullscreen : sf::State::Windowed
            );
            resizeWindow(window.getSize().x, window.getSize().y);
            f11PreviouslyPressed = true;
        }
    } else {
        f11PreviouslyPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

    for (const auto& dir : directions) {
        bool valid = (dir.delta.x != 0 && direction.x == 0) || (dir.delta.y != 0 && direction.y == 0);
        if (sf::Keyboard::isKeyPressed(dir.key)) {
            if (!keysPressed[dir.index] && valid) {
                nextDirection = dir.delta;
                keysPressed[dir.index] = true;
            }
        } else {
            keysPressed[dir.index] = false;
        }
    }
}

void SnakeGame::resizeWindow(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    boxWidth = w / GRID_COLS;
    boxHeight = h / GRID_ROWS;

    for (int row = 0; row < GRID_ROWS; ++row)
        for (int col = 0; col < GRID_COLS; ++col)
            setupCell(grid[row][col], row, col);

    if (scoreText) {
        scoreText->setPosition(sf::Vector2f(boxWidth, boxHeight - 1));
        scoreText->setCharacterSize(20);
    }
}

void SnakeGame::createGrid() {
    boxWidth = windowWidth / GRID_COLS;
    boxHeight = windowHeight / GRID_ROWS;
    grid.resize(GRID_ROWS, std::vector<GridCell>(GRID_COLS));

    for (int row = 0; row < GRID_ROWS; ++row)
        for (int col = 0; col < GRID_COLS; ++col)
            setupCell(grid[row][col], row, col);
}

void SnakeGame::createFruit() {
    std::uniform_int_distribution<> xDist(0, GRID_ROWS - 1);
    std::uniform_int_distribution<> yDist(0, GRID_COLS - 1);
    int x, y;
    do {
        x = xDist(gen);
        y = yDist(gen);
    } while (grid[x][y].value != 0);

    setGridCell(x, y, 2, sf::Color::Green);
    fruits.push_back({x, y});
}

void SnakeGame::startGame() {
    if (!font.openFromMemory(arial_ttf, sizeof(arial_ttf))) return;

    desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "Snake", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    windowWidth = window.getSize().x;
    windowHeight = window.getSize().y;

    createGrid();
    scoreText.emplace(font);
    scoreText->setCharacterSize(30);
    scoreText->setString("Score 0");
    scoreText->setPosition(sf::Vector2f(boxWidth, boxHeight));

    snake.push_back({4, 6});
    snake.push_back({4, 7});

    createFruit();

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        timer += dt.asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        handleKeyInputs();

        if (timer >= moveInterval) {
            timer = 0.f;
            moveSnake();
        }

        window.clear(sf::Color::Black);
        for (const auto& row : grid)
            for (const auto& cell : row)
                window.draw(cell.shape);

        if (scoreText) window.draw(*scoreText);
        window.display();
    }
}
