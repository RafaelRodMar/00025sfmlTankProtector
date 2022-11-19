//---------------------------------------------------------------------
// Textures
//---------------------------------------------------------------------
bool loadTexture(const std::string &name, const std::string &filename)
{
    sf::Texture t;
    if( !t.loadFromFile(filename)) return false;

    mTextures[name] = t;
    return true;
}

sf::Texture &getTexture(const std::string &name) { return mTextures[name]; };

void showTexture(const std::string &name, float x, float y, sf::RenderWindow &window)
{
    sf::Sprite sp;
    sp.setTexture(mTextures[name]);
    sp.setPosition(x,y);
    window.draw(sp);
}

void CleanupTextures() { mTextures.clear(); }

//-----------------------------------------------------------------
// Sounds
//-----------------------------------------------------------------
bool loadSoundBuffer(const std::string &name, const std::string &filename)
{
    //soundbuffer
    sf::SoundBuffer sb;
    if( !sb.loadFromFile(filename) ) return false;

    mSoundBuffers[name] = sb;

    //sound
    sf::Sound s;
    s.setBuffer(mSoundBuffers[name]);
    mSounds[name] = s;
    return true;
}

void playSound(const std::string &name)
{
    mSounds[name].play();
}

void CleanupSounds()
{
    mSoundBuffers.clear();
    mSounds.clear();
}

//------------------------------
//Music
//------------------------------
bool openMusic(const std::string &name, const std::string &filename)
{
    sf::Music* tempmusic = new sf::Music();
    mMusic[name] = tempmusic;
    if( !mMusic[name]->openFromFile(filename) ) return false;

    return true;
}

void playMusic(const std::string &name, bool loop)
{
    mMusic[name]->setLoop(loop);
    mMusic[name]->play();
}

void pauseMusic(const std::string &name)
{
    mMusic[name]->pause();
}

void continueMusic(const std::string &name)
{
    mMusic[name]->play();
}

void setVolume(const std::string &name, float vol)
{
    mMusic[name]->setVolume(vol);
}

void stopMusic(const std::string &name)
{
    mMusic[name]->stop();
}

void CleanupMusic()
{
    for( std::map<std::string, sf::Music*>::iterator it = mMusic.begin(); it != mMusic.end(); ++it )
    {
      delete (it->second);
    }
    mMusic.clear();
}

//-----------------------------
//fonts
//-----------------------------
bool loadFont(const std::string &name, const std::string &filename)
{
    sf::Font f;
    if( !f.loadFromFile(filename) ) return false;
    mFonts[name] = f;
    return true;
}

void Text(const std::string &pstr, float px, float py, sf::Color pcolor, int psize, const std::string &fontname, sf::RenderWindow &window)
{
    sf::Text str;
    str.setString(pstr);
    str.setFont(mFonts[fontname]);
    str.setCharacterSize(psize);
    str.setPosition(px, py);
    str.setFillColor(pcolor);
    window.draw(str);
}

void CleanupFonts()
{
    mFonts.clear();
}

//-----------------------------
//assets
//-----------------------------
void loadAssets()
{
    std::ifstream in("assets/assets.txt");
    if(in.good())
    {
        std::string str;
        while( std::getline(in,str) )
        {
            std::stringstream ss(str);
            std::string datatype, name, filename;
            ss>>datatype;
            ss>>name;
            ss>>filename;

            if( datatype == "img" ) loadTexture(name, "assets/img/" + filename);
            if( datatype == "snd" ) loadSoundBuffer(name, "assets/snd/" + filename);
            if( datatype == "mus" ) openMusic(name, "assets/mus/" + filename);
            if( datatype == "fnt" ) loadFont(name, "assets/fnt/" + filename);
        }
        in.close();
    }
    else
    {
        std::cout << "Error loading assets" << std::endl;
    }
}

void CleanupAll()
{
    CleanupTextures();
    CleanupSounds();
    CleanupMusic();
    CleanupFonts();
}
