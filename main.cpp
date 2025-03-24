#include"Game.h"

// March 2025 - SFML test - Aiming Game

int main()
{
    // init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    // init game engine
    Game game;

    // game loop
    while (game.running() && !game.getEndGame()) 
    {
        // update
        game.update();

        // render
        game.render();
    }

    // end of application
    return 0;
}