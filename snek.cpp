#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int SCREEN_FPS = 10;
const int SCREEN_TICKS_PER_FRAME = 1000/SCREEN_FPS;


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
class Snek;
class Food;
Snek *snek;
Food *food;

class LTimer{
public:
    LTimer():mStartTicks(0), mPausedTicks(0),mPaused(false),mStarted(false){};

    void start(){
        mStarted = true;
        mPaused = false;
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    };
    void stop(){
        mStarted = false;
        mPaused = false;
        mStartTicks = 0;
        mPausedTicks = 0;
    };
    void pause(){
        if (mStarted && !mPaused) {
            mPaused = true;
            mPausedTicks = SDL_GetTicks() - mStartTicks;
            mStartTicks = 0;
        }
    };
    void unpause(){
        if(mStarted && mPaused){
            mPaused = false;
            mStartTicks = SDL_GetTicks() - mPausedTicks;

            mPausedTicks = 0;
        }
    };

    Uint32 getTicks(){
        Uint32 time = 0;

        if(mStarted) {
            if(mPaused){
                time = mPausedTicks;
            }else{
                time = SDL_GetTicks() - mStartTicks;
            }
        }
        return time;
    };
    bool isStarted(){return mStarted;};
    bool isPaused(){return mPaused && mStarted;};
private:
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;

};
struct segment{
    int x;
    int y;


    int scl = 16;
    int w = scl;
    int h = scl;
};


class Snek{

public:
    Snek(){
        this->scl = 16;
        this->x = SCREEN_WIDTH/2;
        this->y = SCREEN_HEIGHT/2;
        this->dx = 1*scl;
        this->dy = 0*scl; 
        this->width = 1*scl;
        this->height = 1*scl;
    }
    ~Snek();
    void draw(){
        // head
        SDL_Rect fillRect = {this->getX(),this->getY(),this->getW(),this->getH()};
        SDL_SetRenderDrawColor(gRenderer,0x8a,0xe2,0x34,0xff);
        SDL_RenderFillRect( gRenderer, &fillRect );
        for(int i = 0;i<tail.size();i++){
             SDL_Rect fillRect = {tail[i].x,tail[i].y,tail[i].w,tail[i].w};
             SDL_SetRenderDrawColor(gRenderer,0x8a,0xe2,0x34,0xff);
             SDL_RenderFillRect( gRenderer, &fillRect );
                 
        }

    }

    int getX(){return this->x;}

    int getY(){return this->y;}

    void setX(int x){this->x = x;}

    void setY(int y){this->y = y;}

    void eat(){
        // std::out<<"snek x: "<<this->x<<" snek y: "<<this->y<<std::endl;
        if(this->tail.empty()){
            struct segment tmp;
            tmp.x = this->x;
            tmp.y = this->y;
            tmp.scl = this->scl;
            tail.push_back(tmp);
        }else{
            struct segment tmp;
            tmp.x = (this->tail.back()).x;
            tmp.y = (this->tail.back()).y;
            tmp.scl = (this->tail.back()).scl;
            this->tail.push_back(tmp);
        }
        //std::cout<<tail.size()<<std::endl;
    };

    void update(){
        if(!this->tail.empty()){
            //tail[0].x = this->x;
            //tail[0].y = this->y;
            for(int i=tail.size()-1;i>0;--i){
                tail[i].x = tail[i-1].x;
                tail[i].y = tail[i-1].y;
            }
            tail[0].x = this->x;
            tail[0].y = this->y;
        }
        x+=dx;
        y+=dy;
        if((this->x<0 || this->x>SCREEN_WIDTH-this->width) ||
           (this->y<0 || this->y >SCREEN_HEIGHT-this->height) ){
            //dx = 0;
            tail.clear();
            this->x = SCREEN_WIDTH/2;
            this->y = SCREEN_HEIGHT/2;
            this->dx = scl;
            this->dy = 0;
        }
        //if(this->y<=0 || this->y >=SCREEN_HEIGHT-this->height ){
        //    dy = 0;
        //    tail.clear();
        //}
        // check if hit self
        for(int i=0;i<tail.size();++i){
            if(tail[i].x== this->x && tail[i].y == this->y){
                tail.clear();
                this->x = SCREEN_WIDTH/2;
                this->y = SCREEN_HEIGHT/2;
                this->dx = scl;
                this->dy = 0;
                break;

            }
        }
        //std::cout<<"snek x: "<<this->x<<" snek y: "<<this->y<<std::endl;
        // check for collision and call eat
        // scan through update position
        // might be able to remove dydx from segment struct
        //if(!this->tail.empty()){
        //    tail[0].x = this->x;
        //    tail[0].y = this->y;
        //}
        
    }

    void setD(int dx, int dy){
        this->dx = dx*scl; 
        this->dy = dy*scl;
    }

    int getDx(){return this->dx;};
 
    int getDy(){return this->dy;};

    int getW(){
        return this->width;
    }

    int getH(){
        return this->height;
    }

private:
    int x;
    int y;
    int dy;
    int dx;
    int scl;
    int width;
    int height;
    std::vector<segment> tail;

};



class Food{
public:
    Food(){
        srand(time(NULL));

        this->scl = 16;
        this->width = 1*scl;
        this->height = 1*scl;
        int col = SCREEN_WIDTH/scl;
        int row = SCREEN_HEIGHT/scl;

        this->x = scl*(rand()%col);
        this->y = scl*(rand()%row);
        //std::cout<<"food x: "<<this->x<<" food y: "<<this->y<<std::endl;


    }

    void draw(){
        SDL_Rect fillRect = {this->getX(),this->getY(),this->getW(),this->getH()};
       
        SDL_RenderFillRect( gRenderer, &fillRect );
        SDL_SetRenderDrawColor(gRenderer,0xff,0x40,0x81,0xff);
        SDL_RenderFillRect(gRenderer, &fillRect);
    
    }

    void update(){
        //if(this->x == snek->getX() && this->y == snek->getY()){
        srand(time(NULL));
        int col = SCREEN_WIDTH/scl;
        int row = SCREEN_HEIGHT/scl;

        this->x = scl*(rand()%col);
        this->y = scl*(rand()%row);
        // std::cout<<"food x: "<<this->x<<" food y: "<<this->y<<std::endl;

        //}

    };

    int getX(){return this->x;}

    int getY(){return this->y;}

    void setX(int x){this->x = x;}

    void setY(int y){this->y = y;}

    int getW(){
        return this->width;
    }

    int getH(){
        return this->height;
    }

private:
    int x;
    int y;
    int scl;
    int width;
    int height;
    float timeElapsed;

};



bool init(){
    // init flag
    bool success = true;

    snek = new Snek();
    food = new Food();
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDL could not initialize SDL_Error: %s \n", SDL_GetError());
        success = false;
    }else{

        // create window
        gWindow = SDL_CreateWindow("snek", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        if(gWindow ==NULL){
            printf("window could not be init SDL_Error %s\n",SDL_GetError());

        }else{

            // create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1,SDL_RENDERER_ACCELERATED);
            if(gRenderer ==NULL){
                printf("Renderer could not be created SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {

                //Initalize colour renderer
                SDL_SetRenderDrawColor(gRenderer, 0xff,0xff,0xff,0xff);
                // init png loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initalize. SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }



    }
    return success;


}
void close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gWindow = NULL;
    gRenderer = NULL;
    IMG_Quit();
    SDL_Quit();
    free(food);
    free(snek);
}
void update() {
    snek->update();
    // collision
    if((snek->getX() == food->getX()) && (snek->getY() == food->getY())){
        food->update();
        snek->eat();
    }
    
}

void draw(){

    snek->draw();
    food->draw();

    SDL_RenderPresent(gRenderer);

}

void gameLoop(){
    if(!init()){
        printf("Fail to init.");
    }else{
        bool quit = false;
        SDL_Event e;

        LTimer fpsTimer;
        LTimer capTimer;
        int countedFrames = 0;
        fpsTimer.start();
        // build ltimer class
        while(!quit){
            capTimer.start();
            while(SDL_PollEvent(&e)!= 0){
                if(e.type ==SDL_QUIT){
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if(snek->getDy()<=0){
                                snek->setD(0, -1);
                            }
                            break;
                        case SDLK_DOWN:
                            if(snek->getDy()>=0){
                                snek->setD(0, 1);
                            }
                            break;
                        case SDLK_LEFT:
                            if(snek->getDx()<=0){
                                snek->setD(-1, 0);
                            }
                            break;
                        case SDLK_RIGHT:
                            if(snek->getDx()>=0){
                                snek->setD(1, 0);
                            }
                            break;
                        case /*SDLK_SPACE*/SDLK_KP_1:
                            snek->eat();
                            break;
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                }
            }



            float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
            if(avgFPS > 2000000){
                avgFPS = 0;
            }
            update();

            SDL_SetRenderDrawColor(gRenderer,0x27,0x28,0x22,0xff);
            SDL_RenderClear(gRenderer);

            draw();
            ++countedFrames;

            int frameTicks = capTimer.getTicks();
            if(frameTicks < SCREEN_TICKS_PER_FRAME){
                SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
            }
        }
        close();
    }

}
int main(int argc, char* args) {
    gameLoop();
    return 0;
}
