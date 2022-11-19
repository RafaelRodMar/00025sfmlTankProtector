#include<iostream>
#include<random>
#include<chrono>
#include<vector>
#include<list>
#include<map>
#include<fstream>
#include<sstream>
#include<cmath>
#include<cstring>

#include<sfml/graphics.hpp>
#include<sfml/audio.hpp>

//Textures
std::map<std::string, sf::Texture> mTextures;
//fonts
std::map<std::string, sf::Font> mFonts;
//Sound Buffers
std::map<std::string, sf::SoundBuffer> mSoundBuffers;
//Sounds
std::map<std::string, sf::Sound> mSounds;
//Music
std::map<std::string, sf::Music*> mMusic;

#include "various.h"
#include "assets.h"
#include "Animation.h"
#include "entity.h"
#include "game.h"

int main()
{
    //create the main window
    game.init(480,320,200,200,"Tank Protector");

    ReadHiScores(game.vhiscores);

    loadAssets();

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.f/60.f); //60fps
    sf::Time elapsed = sf::Time::Zero;

    while(game.app.isOpen())
    {
        elapsed += clock.restart();

        game.input();

        while( elapsed >= timePerFrame )
        {
            game.update(elapsed);
            elapsed -= timePerFrame;
        }

        game.draw();
    }

    CleanupAll();

    WriteHiScores(game.vhiscores);

    return EXIT_SUCCESS;
}

