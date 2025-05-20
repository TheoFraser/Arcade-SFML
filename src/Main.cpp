#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "arial_font.hpp"
#include "SnakeGame.hpp"

using namespace std;

enum class GameState {
    Menu,
    Playing
};

int main()
{
    GameState gameState = GameState::Menu;
    bool wasKeyPressed = false;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Snake", sf::State::Fullscreen);
    window.setFramerateLimit(144);
    sf::Font font;
    if (!font.openFromMemory(arial_ttf, sizeof(arial_ttf))) {
        return -1;
    }
    sf::Text title(font);
    title.setString("MENU");
    title.setFillColor(sf::Color::Green);
    title.setCharacterSize(60);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(desktop.size.x / 2.f - title.getLocalBounds().position.x / 2, 100.f));

    sf::Text snakeOption(font);
    snakeOption.setString("Play Snake");
    snakeOption.setFillColor(sf::Color::Green);
    snakeOption.setCharacterSize(40);
    snakeOption.setPosition(sf::Vector2f(desktop.size.x / 2.f - snakeOption.getLocalBounds().position.x / 2, 250.f));

    sf::Text tetrisOption(font);
    tetrisOption.setString("Play Tetris");
    tetrisOption.setCharacterSize(40);
    tetrisOption.setFillColor(sf::Color(150, 150, 150));    
    tetrisOption.setPosition(sf::Vector2f(desktop.size.x / 2.f - tetrisOption.getLocalBounds().position.x / 2, 320.f));

    
    int selectedMenuItem = 0;
    std::vector<sf::Text*> menuItems = { &snakeOption, &tetrisOption };

    sf::RectangleShape selectorBox(sf::Vector2f(400, 50));
    selectorBox.setFillColor(sf::Color(255, 255, 255, 50));
    selectorBox.setPosition(sf::Vector2f(snakeOption.getPosition().x - 20, snakeOption.getPosition().y));
    while (window.isOpen()){
        while (auto event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (gameState == GameState::Menu) {
                if (!wasKeyPressed) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                            selectedMenuItem = (selectedMenuItem + menuItems.size() - 1) % menuItems.size();
                            wasKeyPressed = true;
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                            selectedMenuItem = (selectedMenuItem + 1) % menuItems.size();
                            wasKeyPressed = true;
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                            if (selectedMenuItem == 0) {
                                gameState = GameState::Playing;
                            }
                            wasKeyPressed = true;
                        }
                    }

                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) &&
                        !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) &&
                        !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                        wasKeyPressed = false;
                    }
                    for (int i = 0; i < menuItems.size(); ++i) {
                        if (i == selectedMenuItem) {
                            menuItems[i]->setFillColor(sf::Color::Green);
                        } else {
                            menuItems[i]->setFillColor(sf::Color(150, 150, 150));
                        }
                    }
                    selectorBox.setPosition(sf::Vector2f(menuItems[selectedMenuItem]->getPosition().x - 20, menuItems[selectedMenuItem]->getPosition().y));
                    window.clear();
                    window.draw(title);
                    window.draw(selectorBox);
                    window.draw(snakeOption);
                    window.draw(tetrisOption);
            }
            window.display();
        }
        if(gameState == GameState::Playing){
            window.close(); 
            SnakeGame snake;
            snake.startGame();
            gameState = GameState::Menu;
            window.create(desktop, "Menu", sf::State::Fullscreen);
            window.setFramerateLimit(144);
        }
    }
    return 0;
}