#include "Game.h"

// private functions
void Game::initVariables()
{
	this->window = nullptr;

	// game logic
	this->endGame = false;
	this->points = 0;
	this->health = 150;
	this->enemySpawnTimerMax = 15.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 7;
	this->mouseHeld = false;

}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	//this->videoMode.getDesktopMode; // senare för fullscreen
	this->window = new sf::RenderWindow(this->videoMode, "ska du skita i din jaevel", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
	
}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/prstart.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << std::endl;
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(18);
	this->uiText.setFillColor(sf::Color::Yellow);
	this->uiText.setOutlineColor(sf::Color::Red);
	this->uiText.setOutlineThickness(1.0f),
	this->uiText.setString("NONE");

	this->uiText.setPosition(10.0f, 10.0f);
}

// constructors / destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
}

Game::~Game() 
{
	delete this->window;
}

// accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

// functions
void Game::spawnEnemy()
{
	/*
	* @return void
	* 
	* Spawnar enemies och sätter type, färg och pos random.
	* - sets random type
	* - sets random positions
	* - sets random color
	* - lägger till enemy till vector
	*/

	this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0.f);

	// randomize enemy
	int type = rand() % 5;

	switch (type) 
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(20.f, 20.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		this->enemy.setOutlineColor(sf::Color(200, 0, 125));
		this->enemy.setOutlineThickness(2.f);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Red);
		this->enemy.setOutlineColor(sf::Color(100, 0, 0));
		this->enemy.setOutlineThickness(3.f);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		this->enemy.setOutlineColor(sf::Color(225, 0, 0));
		this->enemy.setOutlineThickness(5.f);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Blue);
		this->enemy.setOutlineColor(sf::Color(0, 0, 100));
		this->enemy.setOutlineThickness(7.f);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(90.f, 90.f));
		this->enemy.setFillColor(sf::Color::Green);
		this->enemy.setOutlineColor(sf::Color(0, 100, 0));
		this->enemy.setOutlineThickness(10.f);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::White);
		this->enemy.setOutlineColor(sf::Color(255, 100, 255));
		this->enemy.setOutlineThickness(10.f);
		break;
	}

	// spawna enemy
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) // stäng ner
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/*
	* @ return void
	* uppdaterar positionen för musen:
		- relativt till fönstret (Vector2i)
		- vet inte exakt hur detta funkar, skitsamma
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << std::endl << "Health: " << this->health << std::endl;;

	this->uiText.setString(ss.str());
}



void Game::updateEnemies()

{
	/*
	* @return void
	*
	* uppdaterar enemy spawn timer och spawnar enemies
	* när totala mängden enemies är mindre än max.
	* Moves enemies neråt
	*/

	// updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//spawna enemy och återställ timern
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}

		else
		{
			this->enemySpawnTimer += 1.0f; // lite svårighetsgrad hur snabbt de spawnar (<0.9 - LÄTT) (1.0 - MEDIUM) (1.0< - SVÅR)
		}
	}

	// move och uppdatera enemies
	for (std::size_t i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 4.f); // hur snabbt ner de åker "y"

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i); // ta bort om passera kanten längst ner
			this->health -= 10; // hp tappat per missad enemy
			std::cout << "Health: " << this->health << std::endl;
		}
	
	}

	// check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false) 
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					// få poäng
					if (this->enemies[i].getFillColor() == sf::Color::Magenta) 
					{
						this->points += 10;
					}

					else if (this->enemies[i].getFillColor() == sf::Color::Red)
					{
						this->points += 7;
					}

					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
					{
						this->points += 5;
					}

					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
					{
						this->points += 3;
					}

					else if (this->enemies[i].getFillColor() == sf::Color::Green)
					{
						this->points += 1;
					}
					std::cout << "Points: " << this->points << std::endl;

					// ta bort enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else 
	{
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false) 
	{
		this->updateMousePositions();

		// uppdaterar positionen för musen relativt till spelfönstret, skrivs i konsolen
		/*std::cout << "Mouse pos: "
			<< sf::Mouse::getPosition(*this->window).x
			<< sf::Mouse::getPosition(*this->window).y << std::endl;*/

		this->updateText();

		this->updateEnemies();
	}

	// avslutar spelet condition
	if (this->health <= 0) 
	{
		this->endGame = true;
	}
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	for (std::size_t i = 0; i < this->enemies.size(); i++)
	{
		target.draw(this->enemies[i]);
	}
}

void Game::render()
{
	/*
	* @return void
		- clear old frame
		- render objects
		- display frame in window

		renders the game objects
	*/
	this->window->clear(sf::Color::Black);

	// draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}