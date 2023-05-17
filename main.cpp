// Flappy Plane is written by Abthahi Ahmed Rifat in C++ with SDL2 library.
// Facebook : https://facebook.com/IAmAbthahi
// Website  : https://aarsoftx.com

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

float random(float low, float high){
	return low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
}

// Creating Window and Renderer
SDL_Window *win = SDL_CreateWindow("Flappy Plane | Abthahi & Programming", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);

// Event Handler Object
SDL_Event event;

// Texture Loading
SDL_Texture *ground = IMG_LoadTexture(ren, "texture/ground.png");
SDL_Texture *background = IMG_LoadTexture(ren, "texture/background.png");
SDL_Texture *plane  = IMG_LoadTexture(ren, "texture/plane.png");

void drawText(int x, int y, string text, SDL_Color color, TTF_Font *font){
    
    SDL_Texture *textTexture = NULL;
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    
    textTexture = SDL_CreateTextureFromSurface(ren, surface);
    
    
    SDL_Rect r = {x, y, surface->w, surface->h};
    
    SDL_RenderCopy(ren, textTexture, NULL, &r);
    
}
void drawCenteredText(int x, int y, string text, SDL_Color color, TTF_Font *font){
    
    SDL_Texture *textTexture = NULL;
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    
    textTexture = SDL_CreateTextureFromSurface(ren, surface);
    
    
    SDL_Rect r = {x - surface->w / 2, y, surface->w, surface->h};
    
    SDL_RenderCopy(ren, textTexture, NULL, &r);
    
}


void displayGround(){
    // Geometry for ground texture on screen;
    SDL_Rect r = {0, 445, 800, 155};
    // Display the ground on the screen;
    SDL_RenderCopy(ren, ground, NULL, &r);
}
void displayBackground(){
    // Geometry for ground texture on screen;
    SDL_Rect r = {0, 0, 800, 600};
    // Display the ground on the screen;
    SDL_RenderCopy(ren, background, NULL, &r);
}


class Particle{
    
private:
    int x, y;
    int a, sc, color;
public :
    
    Particle(float x, float y){
        
        this->x = x;
        
        int randY = random(-10, 10); 
        
        this->y = (y + 30) + randY;
        this->a = 255;
        
        this->sc = random(10, 16);
        this->color = random(150, 255);
    }
    
    void update(){
        this->x -= 3;
        this->a -= 7;
    }
    
    bool isHidden(){
        if (this->a < 0){
            return true;
        }
        return false;
    }
    
    void draw(){
       
        SDL_SetRenderDrawColor(ren, this->color, this->color, this->color, this->a);
        SDL_Rect r = {this->x, this->y, this->sc, this->sc};
        SDL_RenderFillRect(ren, &r);
    }
    
};


// Plane Class
class Plane{
    
private:
    float vy, g, l;
public:
    float x, y;
    int w, h, fireCount;
    vector<Particle> fire;
    Plane(float x, float y){
        // Constructor for initialization
        this->x  = x;
        this->y  = y;
        this->w  = 120;
        this->h  = 46;
        
        this->vy = 0;
        this->g  = 0.15;
        this->l = 5;
        
        this->fireCount = 0;
        
    }
    
    void reset(float x, float y){
        this->x  = x;
        this->y  = y;
        this->w  = 120;
        this->h  = 46;
        
        this->vy = 0;
        this->g  = 0.15;
        this->l = 5;
        
        this->fireCount = 0;
    }
    
    // Update the plane 
    void update(){
        this->y  += this->vy;
        this->vy += this->g;
        
        if (fireCount % 3 == 0){
            this->fire.push_back(Particle(this->x, this->y));
        }
        
        this->fireCount++;
        for (unsigned i = 0; i < this->fire.size(); i++){
            this->fire[i].update();
            if (this->fire[i].isHidden()){
                this->fire.erase(this->fire.begin() + i);
            }
        
        }
        
    }
    // Lifting
    void lift(){
        this->vy -= this->l;
    }
    // Check if plane collide with ground or not;
    void collideWithGround(int ground){
        if (this->y >= ground - this->h){
            this->vy = 0;
            this->y = ground - this->h;
        }
        
        if (this->y < 0){
            this->vy = 0;
            this->y = 0;
        }
    }
    
    // Render the plane on the screen;
    void draw(){
        
        for (auto f : this->fire){
            f.draw();
        }
        
        SDL_Rect r = {(int)this->x, (int)this->y, this->w, this->h};
        SDL_RenderCopy(ren, plane, NULL, &r);
    }
    
};



class Wall{
    
public:
    float x;
    int topHeight, bottomHeight, ground, width;
    
    Wall(float x){
        this->x = x;
        this->topHeight = (int)random(80, 150);
        this->bottomHeight = (int)random(80, 150);
        this->ground = 445;
        this->width = 50;
    }
    
    void update(){
        
        this->x -= 2;
        
    }
    void draw(){
        
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        
        SDL_Rect r1 = {(int)this->x, 0, this->width, this->topHeight};
        SDL_Rect r2 = {(int)this->x, this->ground - this->bottomHeight, this->width, this->bottomHeight};
        

        SDL_RenderFillRect(ren, &r1);
        SDL_RenderFillRect(ren, &r2);
 

        SDL_SetRenderDrawColor(ren, 200, 100, 100, 255);
        SDL_Rect rs1 = {(int)this->x, this->topHeight - 30, this->width, 30};
        SDL_Rect rs2 = {(int)this->x, this->ground - this->bottomHeight, this->width, 30};
       
        SDL_RenderFillRect(ren, &rs1);
        SDL_RenderFillRect(ren, &rs2);        
    }
    
    bool gone(){
        return this->x < - this->width;
    }
    
    
    bool hit(Plane p){
        if (p.y < this->topHeight || p.y + p.h > this->ground - bottomHeight){
            if (p.x + p.w > this->x && p.x < this->x + this->width){
                return true;
            }
        }
        return false;
    }
    
};




int main(int argc, char **argv)
{
    
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    
    TTF_Init();
 
    // Font Loading
    TTF_Font *font16 = TTF_OpenFont("font/poppins.ttf", 16);
    TTF_Font *font24 = TTF_OpenFont("font/poppins.ttf", 24);
    TTF_Font *font40 = TTF_OpenFont("font/poppins.ttf", 40);
   
    if (!font16){
        cout<<"Font not loaded"<<endl;
        exit(1);
    }
    
    int score = 0;
    
    
    
    // Set Running to true;
    bool isRunning = true;
    
    // Plane Object
    Plane plane = Plane(140, 100);
    
    // Walls 
    vector<Wall> walls;
    
    // wallCount;
    int wallCount = 0;
    
    // Main Loop
    
    bool isPlaying = false;
    bool isGameOver = false;
    
    while (isRunning){
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
     
        
        // Processes
        if (isPlaying){
            if (wallCount % 120 == 0){
                walls.push_back(Wall(800));
            }
            
            for (unsigned i = 0; i < walls.size(); i++){
                if (walls[i].gone()){
                    walls.erase(walls.begin() + i);
                }
                walls[i].update();
                if (walls[i].hit(plane)){
                    isPlaying = false;
                    isGameOver = true;
                }
            }
            
            plane.update();
            plane.collideWithGround(445);
            wallCount++;
            score++;
        }
        
                
         // Display background
        displayBackground();  
        
        for (auto w : walls){
            w.draw();
        }
        
        // Draw plane
        plane.draw();

        // Display ground
        displayGround();        
        
        SDL_Color textColor = {255,255,255};
        drawText(10, 570, "Score " + to_string(score), textColor, font16);
        
        
        if (isGameOver){
            
            SDL_Rect overlay = {250, 150, 300, 200};
            SDL_SetRenderDrawColor(ren, 40, 40, 40, 150);
            SDL_RenderFillRect(ren, &overlay);
            
            drawCenteredText(400, 160, "Game Over!", textColor, font40);
            drawCenteredText(400, 230, "Your Score : " + to_string(score), textColor, font24);
            drawCenteredText(400, 300, "Press \"Enter\" to try again!", textColor, font16);
            SDL_SetRenderDrawColor(ren, 255, 255, 70, 255);
            SDL_RenderDrawRect(ren, &overlay);
        }
        
        if (!isGameOver && !isPlaying){
            
            SDL_Rect overlay = {250, 210, 300, 70};
            SDL_SetRenderDrawColor(ren, 40, 40, 40, 150);
            SDL_RenderFillRect(ren, &overlay);
            
            drawCenteredText(400, 230, "Press \"Enter\" to start!", textColor, font24);

            SDL_SetRenderDrawColor(ren, 255, 255, 70, 255);
            SDL_RenderDrawRect(ren, &overlay);
            
        }
        
        
        
        // Render the graphics
        SDL_RenderPresent(ren);
    
        
        // Event Handling
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_SPACE: 
                    plane.lift();
                break;
                case SDLK_RETURN:
                    if (isGameOver){
                        walls.clear();
                        plane.reset(140, 100);
                        score = 0;
                        wallCount = 0;
                        
                        isGameOver = false;
                    }
                    isPlaying = true;
                break;
            }
        }
        // FPS Controll
        SDL_Delay(1000 / 80);

    }
    
    
    
	return 0;
}
