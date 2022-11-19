
float DEGTORAD = 0.017453f; //pi/180
float PI = 3.14159265358979323846;  /* pi */
float RADTODEG = 180 / PI;

class Entity
{
    public:
        float x,y,dx,dy,R,angle = 0.0;
        bool life;
        bool rotleft = false, rotright = false;
        float rotspeed = 100;
        bool canseetarget = false;
        float targetx, targety;
        std::string name;
        Animation anim;
        int scrw=480,scrh=320;

    Entity()
    {
        life=1;
    }

    void settings(Animation &a,int X,int Y, float _angle, int radius=1)
    {
        anim = a;
        x=X; y=Y;
        angle = _angle;
        R = radius;
    }

    void setTarget(float tx, float ty)
    {
        canseetarget = true;
        targetx = tx; targety = ty;
    }

    void unsetTarget()
    {
        canseetarget = false;
    }

    int faceToTarget()
    {
        float anglewithtarget = std::atan2(y - targety, x - targetx) * RADTODEG;
        anglewithtarget = anglewithtarget + 180 % 360; //add 180 to convert to sfml drawing coordinates.
        float actualangle = anim.sprite.getRotation();

        float diffangle = anglewithtarget - actualangle;
        if( std::abs(diffangle) > 180 )
        {
            if( anglewithtarget > actualangle )
            {
                diffangle = -1 * ((360 - anglewithtarget) + actualangle);
            }
            else
            {
                diffangle = (360 - actualangle) + anglewithtarget;
            }
        }

        int rot = 1;
        if( diffangle > 0 )
            rot = 1;
        else
            rot = -1;

        return rot;
        //anim.sprite.rotate( rot * rotspeed );
    }

    virtual void update(sf::Time dt){};

    void draw(sf::RenderWindow &app)
    {
      anim.sprite.setPosition(x,y);
      app.draw(anim.sprite);
      drawmore(app);
    }

    virtual void drawmore(sf::RenderWindow &app){};

    virtual ~Entity(){};
};

class asteroid: public Entity
{
public:
    asteroid()
    {
        dx=-150;
        dy=0;
        name="asteroid";
    }

    void  update(sf::Time dt)
    {
        x+=dx * dt.asSeconds();

        if (x<0) life=0;
    }

};

class wall: public Entity
{
public:
    wall()
    {
        name = "wall";
    }
};

class bullet: public Entity
{
public:
    bullet()
    {
        name="bullet";
    }

    void  update(sf::Time dt)
    {
        dx = cos(angle*DEGTORAD)*600;
        dy = sin(angle*DEGTORAD)*600;

        x += dx * dt.asSeconds();
        y += dy * dt.asSeconds();

        if (x>scrw || x<0 || y>scrh || y<0) life=0;
    }

};

class tankman: public Entity
{
public:

   tankman()
   {
     name="tankman";
   }

   void update(sf::Time dt)
   {
        /*if( x+dx * dt.asSeconds() < scrw - 16 && x+dx*dt.asSeconds() > 16)
            x+=dx*dt.asSeconds();

        if( y+dy * dt.asSeconds() < scrh - 16 && y+dy*dt.asSeconds() > 16)
            y+=dy*dt.asSeconds();

        dx=dy=0.0;*/

        if( rotleft == true )
        {
            anim.sprite.rotate(rotspeed * dt.asSeconds());
            angle = anim.sprite.getRotation();
            rotleft = false;
        }
        else if( rotright == true )
        {
            anim.sprite.rotate(-rotspeed * dt.asSeconds());
            angle = anim.sprite.getRotation();
            rotright = false;
        }

        if(canseetarget)
        {
            int rot = faceToTarget();
            anim.sprite.rotate( rot * rotspeed * dt.asSeconds());
        }
   }
};

class tankturret: public Entity
{
public:

   tankturret()
   {
     name="tankturret";
   }

   void update(sf::Time dt)
   {
        /*if( x+dx * dt.asSeconds() < scrw - 16 && x+dx*dt.asSeconds() > 16)
            x+=dx*dt.asSeconds();

        if( y+dy * dt.asSeconds() < scrh - 16 && y+dy*dt.asSeconds() > 16)
            y+=dy*dt.asSeconds();

        dx=dy=0.0;*/

        if( rotleft == true )
        {
            anim.sprite.rotate(rotspeed * dt.asSeconds());
            angle = anim.sprite.getRotation();
            rotleft = false;
        }
        else if( rotright == true )
        {
            anim.sprite.rotate(-rotspeed * dt.asSeconds());
            angle = anim.sprite.getRotation();
            rotright = false;
        }

        if(canseetarget)
        {
            int rot = faceToTarget();
            anim.sprite.rotate( rot * rotspeed * dt.asSeconds());
        }
   }
};

class tankbase: public Entity
{
public:

   tankbase()
   {
     name="tankbase";
   }

   void update(sf::Time dt)
   {
        /*if( x+dx * dt.asSeconds() < scrw - 16 && x+dx*dt.asSeconds() > 16)
            x+=dx*dt.asSeconds();

        if( y+dy * dt.asSeconds() < scrh - 16 && y+dy*dt.asSeconds() > 16)
            y+=dy*dt.asSeconds();

        dx=dy=0.0;*/

        /*if( rotleft == true )
        {
            anim.sprite.rotate(100 * dt.asSeconds());
            rotleft = false;
        }
        else if( rotright == true )
        {
            anim.sprite.rotate(-100 * dt.asSeconds());
            rotright = false;
        }*/
   }
};

//a tank
class Tank : public Entity{
    public:
    bool thrust = false;
    bool backthrust = false;

    tankbase *base;
    tankturret *turret;
    tankman *man;

    float speed = 150;
    float turnspeed = 300;

    Tank(std::string _name,int _x, int _y){
        name = _name;
        x = _x;
        y = _y;
        base = new tankbase();
        turret = new tankturret();
        man = new tankman();

        Animation atankbase = Animation(getTexture("tankbase"),0,0,26,26,1,0.0);
        base->settings( atankbase ,x,y,0,14);
        Animation atankturret = Animation(getTexture("tankturret"),0,0,32,32,1,0.0);
        turret->settings( atankturret ,x,y,0,16);
        Animation atankman = Animation(getTexture("tankman"),0,0,32,32,1,0.0);
        man->settings( atankman ,x,y,0,16);
    }

    ~Tank(){
        delete base;
        delete turret;
        delete man;
    }

    void update(bool _fordw, bool _backw, bool _rotright, bool _rotleft, bool _fire){
        thrust = _fordw;
        backthrust = _backw;
        rotright = _rotright;
        rotleft = _rotleft;
    }

    void update(sf::Time delta)
    {
        //enemy things
        if(canseetarget)
        {
            turret->setTarget(targetx,targety);
            man->setTarget(targetx,targety);
        }

        //movement things
        if( rotright )
        {
            base->anim.sprite.rotate(turnspeed * delta.asSeconds());
            turret->anim.sprite.rotate(turnspeed * delta.asSeconds());
            man->anim.sprite.rotate(turnspeed * delta.asSeconds());
        }
        else if( rotleft )
        {
            base->anim.sprite.rotate(-turnspeed * delta.asSeconds());
            turret->anim.sprite.rotate(-turnspeed * delta.asSeconds());
            man->anim.sprite.rotate(-turnspeed * delta.asSeconds());
        }

        angle = base->anim.sprite.getRotation();
        dx = 0.0; dy = 0.0;

         if (thrust)
          {
            dx+=cos(angle*DEGTORAD)*speed;
            dy+=sin(angle*DEGTORAD)*speed;
          }
        else if(backthrust)
          {
            if(angle>180)
                angle = angle - 180;
            else
                angle = angle + 180;

            dx+=cos(angle*DEGTORAD)*speed;
            dy+=sin(angle*DEGTORAD)*speed;
          }
        else
          { dx = 0.0;
            dy = 0.0; }

        float actualspeed = sqrt(dx*dx+dy*dy);
        if (actualspeed>speed)
         { dx *= speed/actualspeed;
           dy *= speed/actualspeed; }

        if( x+dx * delta.asSeconds() < scrw - 16 && x+dx*delta.asSeconds() > 16)
            x+=dx*delta.asSeconds();

        if( y+dy * delta.asSeconds() < scrh - 16 && y+dy*delta.asSeconds() > 16)
            y+=dy*delta.asSeconds();

        dx=dy=0.0;
        base->x = turret->x = man->x = x;
        base->y = turret->y = man->y = y;

        base->update(delta);
        turret->update(delta);
        man->update(delta);
    }

    void drawmore(sf::RenderWindow& app)
    {
        base->anim.sprite.setPosition(base->x,base->y);
        app.draw(base->anim.sprite);
        turret->anim.sprite.setPosition(turret->x,turret->y);
        app.draw(turret->anim.sprite);
        man->anim.sprite.setPosition(man->x,man->y);
        app.draw(man->anim.sprite);
    }

};


bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}
