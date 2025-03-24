#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	class that acts as the game engine.
	wrapper class.
*/

class Game
{
private:
	// variables
	// window
	sf::RenderWindow *window;
	sf::Event ev;
	sf::VideoMode videoMode;

	// mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// resources
	sf::Font font;

	// text
	sf::Text uiText;

	// game logic
	bool endGame;
	unsigned points; //positiva tal
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	// game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
public:
	// constructors / destructors
	Game();
	virtual ~Game();

	// accessors
	const bool running() const;
	const bool getEndGame() const;

	// functions
	void spawnEnemy();
	void pollEvents();

	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};