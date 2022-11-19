class Game{
public:
    int screenwidth = -1;
    int screenheight = -1;
    sf::RenderWindow app;

    enum game_states {MENU, GAME, END_GAME};
    int state = MENU;
    std::vector<int> vhiscores;
    int lives = -1, score = -1;

    //keyboard handling
    // The keyboard's state in the current- and the previous frame
    bool CurrentKeyState[sf::Keyboard::KeyCount];
    bool PreviousKeyState[sf::Keyboard::KeyCount];

    //entities structure
    std::list<Entity*> entities;

    //the player
    Tank* player;
    bool playercanshoot = true;
    Animation abullet = Animation(getTexture("cannonball"),0,0,2,2,1,0.0);
    Animation awall = Animation(getTexture("wall"),0,0,32,32,1,0.0);
    Animation aexplosion = Animation(getTexture("explosion"), 0,0,64,64, 48, 0.5);

    //the map
    char mapa[10][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,0,0,0,0,0,0,0,1,1,0},
        {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
        {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,1,1,0,0,0,0,0,0,0,0,0,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    void init(int pscrw, int pscrh, int pposx, int pposy, std::string pname);
    void input();
    void update(sf::Time delta);
    void draw();

    //keyboard functions
    bool KeyPressed(sf::Keyboard::Key Key)
        { return (CurrentKeyState[Key] && !PreviousKeyState[Key]); }

    bool KeyReleased(sf::Keyboard::Key Key)
        { return (!CurrentKeyState[Key] && PreviousKeyState[Key]); }

    bool KeyHeld(sf::Keyboard::Key Key)
        { return CurrentKeyState[Key]; }
} game;

void Game::init(int pscrw, int pscrh, int pposx, int pposy, std::string pname)
{
    screenwidth = pscrw;
    screenheight = pscrh;
    app.create(sf::VideoMode(pscrw,pscrh),pname);
    //app.setPosition(sf::Vector2i(pposx,pposy));
    app.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2 - pscrw/2, sf::VideoMode::getDesktopMode().height/2 - pscrh/2));

    //keyboard buffers initialization
    memset(CurrentKeyState,     false, sizeof(CurrentKeyState));
    memset(PreviousKeyState,    false, sizeof(PreviousKeyState));

    //objects creation
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<15;j++)
        {
            if( mapa[i][j] == 1 )
            {
                wall* w = new wall();
                w->settings(awall,j*32+16,i*32+16,0.0,16);
                w->x = j*32+16;
                w->y = i*32+16;
                entities.push_back(w);
            }
        }
    }

    //player creation
    player = new Tank("player",16,16);
    entities.push_back(player);

    //enemies creation
    for(int i=1;i<2;i++) {
            Tank* enemytank = new Tank( "enemytank", 14*32 + 16, 9*32 + 16);
            entities.push_back(enemytank);
    }
}

void Game::input()
{
    switch(state)
        {
        case MENU:
            {
            sf::Event event;
            while (app.pollEvent(event))
            {
                if ((event.type == sf::Event::Closed) ||
                    ((event.type == sf::Event::KeyPressed)
                     && (event.key.code == sf::Keyboard::Escape)))
                    app.close();

                // S key pressed: change state to GAME
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
                {
                    state=GAME;
                    lives = 3;
                    score = 0;
                }
            }
            break;
            }
        case GAME:
            {
            sf::Event event;
            while (app.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    app.close();
            }

            // Save the state of each keyboard key (must be done before any Key* function is executed)
            for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
            {
                // Save the keyboard's state from the previous frame
                PreviousKeyState[i] = CurrentKeyState[i];

                // And save the keyboard's state in the current frame
                CurrentKeyState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
            }

            if(KeyPressed(sf::Keyboard::Escape))
                app.close();

            // Space is the fire key
            if(KeyPressed(sf::Keyboard::Space) && playercanshoot)
            {
                bullet *b = new bullet();
                b->name = "playerbullet";
                b->settings(abullet,player->x,player->y,player->turret->anim.sprite.getRotation());
                entities.push_back(b);
                playSound("cannon+2");
                playercanshoot = false;
            }

            player->update( KeyHeld(sf::Keyboard::Up), KeyHeld(sf::Keyboard::Down),
                        KeyHeld(sf::Keyboard::Right), KeyHeld(sf::Keyboard::Left), KeyPressed(sf::Keyboard::Space) );
            break;
            }
        case END_GAME:
            {
            //gamemusic.stop();
            sf::Event event;
            while (app.pollEvent(event))
            {
                if ((event.type == sf::Event::Closed) ||
                    ((event.type == sf::Event::KeyPressed)
                     && (event.key.code == sf::Keyboard::Escape)))
                    app.close();

                // Any key pressed: change state to MENU
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::M))
                {
                    state=MENU;
                }
            }
            break;
            }
        default:
            break;
        }
}

void Game::update(sf::Time delta)
{
    //Game_cycle();
    //update enemy tanks
    for(auto a:entities)
    {
        if( a->name=="enemytank" )
        {
            //float playerx = player->turret->x;
            //float playery = player->turret->y;

            //a->setTarget(playerx,playery); //set the canseetarget to true.
                                             //the turret rotate to follow the player
        }
    }

    if(state==GAME)
    {
     for(auto a:entities)
     {
        for(auto b:entities)
        {
          if (a->name=="wall" && ( b->name=="bullet" || b->name=="playerbullet") )
           if( a->anim.sprite.getGlobalBounds().intersects(b->anim.sprite.getGlobalBounds()) ) //if ( isCollide(a,b) )
            {
                b->life=false;

                //explosion
                Entity *e = new Entity();
                e->settings(aexplosion,b->x,b->y,0.0,1);
                e->name="explosion";
                entities.push_back(e);
                playSound("explosion");
            }

          if (a->name=="tankbase" && b->name=="asteroid")
           if ( isCollide(a,b) )
            {
                b->life=false;

                //ship explosion
                /*Entity *e = new Entity();
                e->settings(sExpl,a->x,a->y);
                e->name="explosion";
                entities.push_back(e);
                if(lives==1)
                    Explosion2.play();
                else
                    Explosion.play();
                lives--;
                if(lives<=0)
                {
                    UpdateHiScores(vhiscores, score);
                    state=END_GAME;
                }

                //relocate the ship
                p->settings(sPlayer,20,85,8);
                p->dx=0;*/
            }
        }
     }

        //remove explosions
        for(auto e:entities)
         if (e->name=="explosion")
          if (e->anim.isEnd()) e->life=0;

        /*if ( rnd.getRndInt(0,100) <= 1 )
         {
           asteroid *a = new asteroid();
           a->settings(sAster, 640 , rnd.getRndInt(0,screenheight), 8);
           entities.push_back(a);
         }*/

         float tempx = player->x;
         float tempy = player->y;
         float tempangle = player->base->anim.sprite.getRotation();

        for(auto i=entities.begin();i!=entities.end();)
        {
          Entity *e = *i;

          e->update(delta);
          e->anim.update();

          if (e->life==false) {
                if(e->name=="playerbullet") playercanshoot = true;
                i=entities.erase(i);
                delete e;
          }
          else i++;
        }

        //check for collisions with obstacles
        for(auto a : entities)
        {
            if( a->name=="wall" )
            {
                sf::FloatRect intersection; //overlapped rectangle in the intersection parameter.
                //if( player->base->anim.sprite.getGlobalBounds().intersects(a->anim.sprite.getGlobalBounds(), intersection))
                if( isCollide(player->base, a))
                {
                    std::cout << "collision ";
                    //if there is a collision with a obstacle then don't move the tank.
                    std::cout << "player xy = " << player->x << "," << player->y;
                    std::cout << "  wall xy = " << a->x << "," << a->y;
                    std::cout << "  intersection wh = " << intersection.width << "," << intersection.height << std::endl;
                    player->x = tempx;
                    player->y = tempy;
                    player->base->anim.sprite.setRotation(tempangle);
                    player->turret->anim.sprite.setRotation(tempangle);
                    player->man->anim.sprite.setRotation(tempangle);
                    /*float tempangle = player->base->anim.sprite.getRotation();
                    if(player->thrust) std::cout << "moviéndose hacia deltante " << tempangle << std::endl;
                    if(player->backthrust) std::cout << "moviéndose hacia atrás " << tempangle << std::endl;
                    if(!player->thrust && !player->backthrust) std::cout << "quieto " << tempangle << std::endl;

                    if(player->thrust && tempangle >= 0 && tempangle <= 90)
                    {
                        player->x -= intersection.width;
                        player->y -= intersection.height;
                    }*/
                    //player->x -= intersection.width;
                    //player->y -= intersection.height;
                }
            }
        }
    }
}

void Game::draw()
{
    app.clear();

    switch(state)
    {
    case MENU:
        {
        showTexture("menu",0,0,app);
        break;
        }
    case GAME:
        {
            //draw the map
            for(int i=0;i<15;i++)
            {
                for(int j=0;j<10;j++)
                {
                    showTexture("floor-grass", i*32, j*32, app);
                }
            }

            for(auto i:entities)
                i->draw(app);
        }
        break;
    case END_GAME:
        showTexture("end",0,0,app);
        break;
    default:
        break;
    }
    app.display();
}
