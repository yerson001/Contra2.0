#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <iostream>
using namespace std;
using namespace sf;

const int W = 800;
const int H = 500;

float DEGTORAD = 0.017453f;

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
    std::vector<IntRect> frames;

	Animation(){}

    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
	    Frame = 0;
        speed = Speed;

		for (int i=0;i<count;i++)
         frames.push_back( IntRect(x+i*w, y, w, h)  );

		sprite.setTexture(t);
		sprite.setOrigin(w/2,h/2);
    sprite.setTextureRect(frames[0]);
	}


	void update()
	{
    Frame += speed;
		int n = frames.size();
    cout<<"THIS IS FRAMES SIZE:: "<<frames.size()<<endl;
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect( frames[int(Frame)] );
	}

	bool isEnd()
	{
	  return Frame+speed>=frames.size();
	}

};


class Entity
{
public:
float x,y,dx,dy,R,angle;
bool life;
std::string name;
Animation anim;

Entity()
{
  life=1;
}

void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
{
  anim = a;
  x=X; y=Y;
  angle = Angle;
  R = radius;
}

virtual void update(){};

void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);
  anim.sprite.setRotation(angle+90);
  app.draw(anim.sprite);

  CircleShape circle(R);
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(x,y);
  circle.setOrigin(R,R);
  //app.draw(circle);
}

virtual ~Entity(){};
};


class asteroid: public Entity
{
public:
  asteroid()
  {
    dx=rand()%8-4;
    dy=rand()%8-4;
    name="asteroid";
  }

void  update()
  {
   x+=dx;
   y+=dy;

   if (x>W) x=0;  if (x<0) x=W;
   if (y>H) y=0;  if (y<0) y=H;
  }

};



class bullet: public Entity
{
public:
  bullet()
  {
    name="bullet";
  }

void  update()
  {
   dx=cos(angle*DEGTORAD)*6;
   dy=sin(angle*DEGTORAD)*6;
   x+=dx;
   y+=dy;
   if (x>W || x<0 || y>H || y<0) life=0;
  }

};


class player: public Entity
{
public:
   int m;
   int thrust;

   player()
   {
     name="player";
   }

   void update()
   {
    if (x>W) x=0; if (x<0) x=W;
    if (y>H) y=0; if (y<0) y=H;
   }
   void moveW(){y-=2;}
   void moveS(){y+=2;}
   void moveD(){x+=2;}
   void moveA(){x-=2;}

};


bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Contra 2.0");
    app.setFramerateLimit(60);

    Texture t0,t01,t02,t03,t04,t05,t1,t2,t3,t4,t5,t6,t7;
    t0.loadFromFile("images/soldier.png");
    t01.loadFromFile("images/soldier.png");
    t02.loadFromFile("images/soldier.png");
    t03.loadFromFile("images/soldier.png");
    t04.loadFromFile("images/soldier.png");
    t05.loadFromFile("images/soldier.png");

    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_red.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);
  
    Sprite background(t2);
    background.setScale(0.5,0.4);

    Animation sSoldier0(t0,160,0,38.7,40,1,0.8);
    /*
    Animation sSoldier1(t01,80,0,38.7,40,1,1);
    Animation sSoldier2(t02,120,0,38.7,40,1,1);
    Animation sSoldier3(t03,160,0,38.7,40,1,1);
    Animation sSoldier4(t04,220,0,38.7,40,1,1);
    Animation sSoldier5(t05,260,0,38.7,40,1,1);
    
    40,0,38.7,40

    */
    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);



    std::list<Entity*> entities;
/*
    for(int i=0;i<5;i++)
    {
      asteroid *a = new asteroid();
      a->settings(sRock, rand()%W, rand()%H, rand()%360, 25);
      entities.push_back(a);
    }
*/
  
    player *p = new player();
    p->settings(sSoldier0,50,50,-90,20);
    entities.push_back(p);
/*
    player *q1 = new player();
    q1->settings(sSoldier1,50,100,0,20);
    entities.push_back(q1);

    player *q2 = new player();
    q2->settings(sSoldier2,50,150,0,20);
    entities.push_back(q2);
    
    player *q3 = new player();
    q3->settings(sSoldier3,50,200,0,20);
    entities.push_back(q3);

    player *q4 = new player();
    q4->settings(sSoldier4,50,250,0,20);
    entities.push_back(q4);

    player *q5 = new player();
    q5->settings(sSoldier5,50,300,0,20);
    entities.push_back(q5);

*/





/*
    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);
*/
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space)
              {
                bullet *b = new bullet();
                b->settings(sBullet,p->x,p->y,p->angle+90,10);
                entities.push_back(b);
              }
        }
    p->thrust=0;
    if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle+=3;
    if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle-=3;
    if (Keyboard::isKeyPressed(Keyboard::W)) p->moveW();
    if (Keyboard::isKeyPressed(Keyboard::S)) p->moveS();
    if (Keyboard::isKeyPressed(Keyboard::D)) p->moveD();
    if (Keyboard::isKeyPressed(Keyboard::A)) p->moveA();

    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();
      e->anim.update();

      if (e->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }



   app.draw(background);
   for(auto i:entities)
     i->draw(app);
   app.display();
    }
    return 0;
}