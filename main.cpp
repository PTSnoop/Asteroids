#include <irrlicht/irrlicht.h>
using namespace irr;

#include <list>
#include <vector>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "animage.h"

class TehReceiver : public IEventReceiver {
public:
    virtual bool OnEvent(const SEvent& event) {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        return false;
    }
    virtual bool IsKeyDown(EKEY_CODE keyCode) {
        return KeyIsDown[keyCode];
    }

    TehReceiver() {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }
private:
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int res_x;
int res_y;

class Bullet {
public:
    bool alive;
    float theta;
    float x;
    float y;
    float vx;
    float vy;
    Bullet();
    void update(float);
};

class Roid {
public:
    bool alive;
    float theta;
    float x;
    float y;
    float vx;
    float vy;
    Roid();
    void update(float);
};

class Player {
public:
    float theta;
    float x;
    float y;
    float vx;
    float vy;
    bool alive;
    int score;

    float flyspeed;
    float rotspeed;

    vector<Bullet> bulletstorage;
    float bullettime;
    
    float roidrate;
    vector<Roid> roidstorage;

    void restart();
    Player();
};

Player player;


bool collis(float ax, float ay, float bx, float by, float val) {
    return ( pow(ax-bx,2) + pow(ay-by,2) < pow(val,2) );
};

void Bullet::update(float dt) {
    x += vx*dt;
    y += vy*dt;
    if (x < -30)
        alive = false;
    if (x > res_x+30)
        alive = false;
    if (y < -30)
        alive = false;
    if (y > res_y+30)
        alive = false;
};

void Roid::update(float dt) {
    x += vx*dt;
    y += vy*dt;
    if (x < -30)
        x = res_x + 10;
    if (x > res_x+30)
        x = -30;
    if (y < -30)
        y = res_y + 10;
    if (y > res_y+30)
        y = -30;
    
};

Roid::Roid() {
    alive = true;
    theta = 0;
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
    
    bool looping = true;
    while (looping) {
        x = ( rand() % (res_x + 30) ) - 15;
        y = ( rand() % (res_y + 30) ) - 15;
        if (     x > -10 and x < res_x
             and y > -10 and y < res_y ) {
            looping = true;
        } else {
            looping = false;
        }
    }
    theta = (rand() % 1024) / 1024.0;
    theta *= 2*M_PI;
    vx = cos(theta) * 20.0 * (1+player.roidrate*1000);
    vy = sin(theta) * 20.0 * (1+player.roidrate*1000);
};

Bullet::Bullet() {
    alive = true;
    theta = 0;
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
};

Player::Player() {
    srand(time(NULL));
    restart();
};

void Player::restart() {
    theta = 0;
    vx = 0;
    vy = 0;
    x = 300;
    y = 300;
    alive = true;
    score = 0;

    flyspeed = 100;
    rotspeed = 2;

    bulletstorage.clear();
    bullettime = 0;

    roidrate = 0.0001;
    roidstorage.clear();
    for (int i=0; i<=49; i++) {
        roidstorage.push_back(Roid ());
    }
//    for (vector<Roid>::iterator i = roidstorage.begin(); i != roidstorage.end(); i++) {
//        cout << i->x << "\t" << i->y << endl;
//    }
};

int main() {

    IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

    core::dimension2d<u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();

    nulldevice -> drop();


    TehReceiver receiver;

    res_x = deskres.Width;
    res_y = deskres.Height;

    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,core::dimension2d<u32>(res_x,res_y),16,true,true,true,&receiver);
    device->setWindowCaption(L"Asteroids!");
    video::IVideoDriver* driver = device->getVideoDriver();

    // love.load()

    gui::IGUIFont* font = device->getGUIEnvironment()->getFont("lucida.xml");
    //gui::IGUIFont* font = device->getGUIEnvironment()->getFont("fonthaettenschweiler.bmp");
    //gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
    video::ITexture* shiptex = driver->getTexture("spacechevron.png");
    video::ITexture* aroidtex = driver->getTexture("asteroid.png");
    video::ITexture* shardtex = driver->getTexture("shard.png");
    video::ITexture* blobtex = driver->getTexture("blob.png");
    video::ITexture* gameovertex = driver->getTexture("gameover.png");
    video::ITexture* rect = driver->getTexture("whitesquare.png");
    u32 then = device->getTimer()->getTime();

    player.restart();
    
    while (device->run() && driver) {
        if (device->isWindowActive()) {
            // Work out a frame delta time.
            u32 now = device->getTimer()->getTime();
            f32 dt = (f32)(now - then) / 1000.f; // Time in seconds
            then = now;

            if (receiver.IsKeyDown(irr::KEY_KEY_Q) or receiver.IsKeyDown(irr::KEY_ESCAPE)) {
                device->closeDevice();
            }

            // love.update(dt)
            
            player.roidrate += 0.0001*dt;
            if ( receiver.IsKeyDown(irr::KEY_KEY_A) or receiver.IsKeyDown(irr::KEY_LEFT)  ) {
                player.theta -= player.rotspeed*dt;
            }

            if ( receiver.IsKeyDown(irr::KEY_KEY_D) or receiver.IsKeyDown(irr::KEY_RIGHT)  ) {
                player.theta += player.rotspeed*dt;
            }

            if ( receiver.IsKeyDown(irr::KEY_KEY_W) or receiver.IsKeyDown(irr::KEY_UP)  ) {
                player.vx += cos(player.theta) * player.flyspeed * dt;
                player.vy += sin(player.theta) * player.flyspeed * dt;
            }

            if (not player.alive and receiver.IsKeyDown(irr::KEY_RETURN))
                player.restart();
            
            if ( receiver.IsKeyDown(irr::KEY_SPACE) and player.alive ) {
                if (player.bullettime > 0.5) {
                    player.bullettime = 0;
                    Bullet a;
                    a.x = player.x;
                    a.y = player.y;
                    a.vx = player.vx;
                    a.vy = player.vy;
                    a.theta = player.theta;
                    a.vx += cos(a.theta)*400;
                    a.vy += sin(a.theta)*400;
                    player.bulletstorage.push_back(a);
                }
            }
            player.bullettime += dt;

            for (vector<Roid>::iterator i = player.roidstorage.begin(); i != player.roidstorage.end(); i++) {
                i->update(dt);
            }
            for (vector<Bullet>::iterator i = player.bulletstorage.begin(); i != player.bulletstorage.end();) {
                i->update(dt);
                if (not i->alive) {
                    i = player.bulletstorage.erase(i);
                }
                else ++i;
            }
            
            if (player.alive) {
                for (vector<Roid>::iterator i = player.roidstorage.begin(); i != player.roidstorage.end();) {
                    for (vector<Bullet>::iterator j = player.bulletstorage.begin(); j != player.bulletstorage.end();) {
                        if (collis(i->x+64,i->y+64,j->x+64,j->y+64,35)) {
                            i->alive = false;
                            j = player.bulletstorage.erase(j);
                            player.score += 10;
                        }
                        else ++j;
                    }
                    if (not i->alive)
                        i = player.roidstorage.erase(i);
                    else 
                        ++i;
                }
            }

            float toppt_real_x = player.x + 23*sin(-player.theta+(M_PI/2.0));
            float toppt_real_y = player.y + 23*cos(-player.theta+(M_PI/2.0));

            float sidept1_real_x = player.x + 30*sin(-player.theta+(3.0*M_PI/2.0)+(M_PI/6.0));
            float sidept1_real_y = player.y + 30*cos(-player.theta+(3.0*M_PI/2.0)+(M_PI/6.0));

            float sidept2_real_x = player.x + 30*sin(-player.theta+(3.0*M_PI/2.0)-(M_PI/6.0));
            float sidept2_real_y = player.y + 30*cos(-player.theta+(3.0*M_PI/2.0)-(M_PI/6.0));

            float ctrpt_real_x = player.x + 10*sin(-player.theta+(M_PI/2.0));
            float ctrpt_real_y = player.y + 10*cos(-player.theta+(M_PI/2.0));

            bool colliding = false;
            for (vector<Roid>::iterator i = player.roidstorage.begin(); i != player.roidstorage.end(); i++) {
                if ( collis(i->x, i->y, toppt_real_x, toppt_real_y, 35) )     colliding = true;
                if ( collis(i->x, i->y, sidept1_real_x, sidept1_real_y, 35) ) colliding = true;
                if ( collis(i->x, i->y, sidept2_real_x, sidept2_real_y, 35) ) colliding = true;
                if ( collis(i->x, i->y, ctrpt_real_x, ctrpt_real_y, 45) )     colliding = true;
            }

            if (colliding and player.alive) {
                player.alive = false;
            }

            player.x += player.vx * dt;
            player.y += player.vy * dt;

            if (player.x < -30)      player.x = res_x+30;
            if (player.x > res_x+30) player.x = -30;
            if (player.y < -30)      player.y = res_y+30;
            if (player.y > res_y+30) player.y = -30;

            if (player.vx > 1000) player.vx = 1000;
            if (player.vy > 1000) player.vy = 1000;
            if (player.vx < -1000) player.vx = -1000;
            if (player.vy < -1000) player.vy = -1000;

            if ( (float)(rand()%10240)/10240.0 < player.roidrate ) {
//                Roid tmproid ();
                player.roidstorage.push_back(Roid ());
            }

            // love.draw(dt)

            driver->beginScene(true,true,video::SColor(0,0,0,0));

//            driver->draw2DPolygon(core::position2d<s32>(toppt_real_x,toppt_real_y),40);
//            driver->draw2DPolygon(core::position2d<s32>(sidept1_real_x,sidept1_real_y),40);
//           driver->draw2DPolygon(core::position2d<s32>(sidept2_real_x,sidept2_real_y),40);
//            driver->draw2DPolygon(core::position2d<s32>(ctrpt_real_x,ctrpt_real_y),40);

//            core::position2d<s32> m = device->getCursorControl()->getPosition();

            if (player.alive) {
                anImage(driver,shiptex, core::position2d<s32>(player.x-32,player.y-32),player.theta+(M_PI/2.0),core::vector2df(0.5,0.5),core::position2d<s32>(32,32));
            }

            for (vector<Bullet>::iterator i = player.bulletstorage.begin(); i != player.bulletstorage.end(); i++) {
                anImage(driver,rect,core::position2d<s32>(i->x,i->y));
            }

            for (vector<Roid>::iterator i = player.roidstorage.begin(); i != player.roidstorage.end(); i++) {
                if (i->alive)
                    anImage(driver, aroidtex, core::position2d<s32>(i->x-32,i->y-32),i->theta+(M_PI/2.0),core::vector2df(0.5,0.5),core::position2d<s32>(32,32));
                
            }

            if (not player.alive) {
                anImage(driver, gameovertex, core::position2d<s32>(400,300));
            }


            /*
            driver->draw2DImage(red, core::position2d<s32>(m.X-16,m.Y-16),
                    core::rect<s32>(0,0,32,32),0,
                    video::SColor(255,255,255,255),true);
            */

            driver->draw2DRectangle(video::SColor(255,0,0,0),
                         core::rect<s32>(res_x-50, 8, res_x-10, 28));
            
            core::stringw scoretext; 
            scoretext += player.score;
            font->draw(scoretext.c_str(),
                        core::rect<s32>(res_x-45,10,res_x-10,24),
                        video::SColor(255,255,255,255));
            
            

            driver->endScene();

            int fps = driver->getFPS();
            core::stringw framerate ("Asteroids! - ");
            framerate += fps;

            device->setWindowCaption(framerate.c_str());

/*
            if ( receiver.IsKeyDown(irr::KEY_SPACE) ) {
                squaretile tmp;
                tmp.pos = core::position2d<s32>(m.X-16,m.Y-16);
                tmp.colournum = rand() % 4;
                spaces.push_back(tmp);
            }
*/
        }
    }
    device->drop();
    return 0;
}
