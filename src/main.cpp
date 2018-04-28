#include <iostream>
#include <SDL.h>
#include <ctime>
#include "rasterization.h"
#include "color.h"
#include "vertex.h"
#include "triangle.h"

bool init();
void kill();
bool loop();
void drawTriangle(float, float, float, float, float, float);

SDL_Window* window;
SDL_Renderer* renderer;

const int WIDTH=640, HEIGHT=WIDTH*3/4;
clock_t current_ticks = clock(), delta_ticks;
clock_t fps = 0;
int fpsRate = 100000;
int oldClock = clock();

int main(int argc, char** args) {
        if ( !init() ) return 1;

        while ( loop() ) {
		delta_ticks = clock() - current_ticks;
		if(delta_ticks > 0){
			fps = CLOCKS_PER_SEC / delta_ticks;
		}
		if(clock() - oldClock > fpsRate){
			//std::cout << "FPS: "<< fps << std::endl;
			oldClock = clock();
		}

		current_ticks = clock();
                // wait before processing the next frame
                SDL_Delay(10);
        } kill();
        return 0;
}
float t1, t2, t3, t4, t5, t6, x=0.0f;
float scaleSize = 0.3;
bool loop() {

        static const unsigned char* keys = SDL_GetKeyboardState( NULL );

        SDL_Event e;
        SDL_Rect r;
        // For mouse rectangle (static to presist between function calls)
        static int mouseX, mouseY;

        // Clear the window to white
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear( renderer );

	Rasterization rast;
	rast.setRenderer(renderer);

        // Event loop
        while ( SDL_PollEvent( &e ) != 0 ) {
                switch ( e.type ) {
                        case SDL_QUIT:
                                return false;
                        case SDL_MOUSEBUTTONDOWN:
                                mouseX = e.button.x;
                                mouseY = e.button.y;
				//std::cout << "X: " << mouseX << "\tY: " << mouseY << std::endl;
				break;
                        case SDL_MOUSEMOTION:
                                mouseX = e.motion.x;
                                mouseY = e.motion.y;
                                break;
                        case SDL_MOUSEBUTTONUP:
                                mouseX = e.button.x;
				mouseY = e.button.y;
                                break;
                }
        }
        // Set drawing color to black
	SDL_SetRenderDrawColor(renderer, 0,0,0, 255);

        // Test key states - this could also be done with events
        if ( keys[SDL_SCANCODE_EQUALS] ) {
		scaleSize *= 1.01f;
        }
        if ( keys[SDL_SCANCODE_MINUS] ) {
		scaleSize *= 0.99f;
        }
        if ( keys[SDL_SCANCODE_3] ) {
                r.x = 20;
                r.y = 20;
                r.w = 100;
                r.h = 100;
                SDL_RenderFillRect( renderer, &r );
        }

        // colors for each point of the triangle
        Color color1(1.0f, 0.0f, 0.0f);
        Color color2(0.0f, 1.0f, 0.0f);
        Color color3(0.0f, 0.0f, 1.0f);
	x+=0.01f;
	float scale = WIDTH*scaleSize;
	t1 = WIDTH/2 + sinf(x)*scale;
	t2 = WIDTH/2 + cosf(x)*scale;
	t3 = WIDTH/2 + sinf(x+(2*M_PI/3))*scale;
	t4 = WIDTH/2 + cosf(x+(2*M_PI/3))*scale;
	t5 = WIDTH/2 + sinf(x+(4*M_PI/3))*scale;
	t6 = WIDTH/2 + cosf(x+(4*M_PI/3))*scale;

	//rast.DrawTriangle(color1, t1, t2, color2, t3, t4, color3, t5, t6);
	Triangle tri[] = {Triangle(Vertex(0,0,0), Vertex(100,0,0), Vertex(0,0,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(100,0,0), Vertex(0,0,100), Vertex(100,0,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(0,0,0), Vertex(0,-100,0), Vertex(0,0,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(0,-100,0), Vertex(0,0,100), Vertex(0,-100,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(0,0,0), Vertex(100,0,0), Vertex(0,-100,0), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(-100,0,0), Vertex(-100,-100,20), Vertex(-100,0,50), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(100,0,0), Vertex(100,0,100), Vertex(200,0,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(20,20,0), Vertex(20,-40,100), Vertex(20,-40,20), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(0,-100,0), Vertex(100,-100,0), Vertex(100,-100,100), Vertex(400,50,0), x, Vertex(400,50,0)),
			  Triangle(Vertex(0,-100,0), Vertex(100,-100,100), Vertex(0,-100,100), Vertex(400,50,0), x, Vertex(400,50,0))};
	for(int i=0;i<8;i++){
		//std::cout<<"Triangle " << i << ": " << std::endl;
		tri[i].get2dCoords();
		rast.fillTriangle(color1, tri[i].xCoords[0], tri[i].yCoords[0], color2, tri[i].xCoords[1], tri[i].yCoords[1], color3, tri[i].xCoords[2], tri[i].yCoords[2]);
		rast.drawTriangle(Color(0.0f,0.0f,0.0f), tri[i].xCoords[0], tri[i].yCoords[0], tri[i].xCoords[1], tri[i].yCoords[1], tri[i].xCoords[2], tri[i].yCoords[2]);
	}
	SDL_RenderPresent( renderer );
	
        return true;
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3){
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
        SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

bool init() {
        // See last example for comments
        if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
                std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
                system("pause");
                return false;
        }

        window = SDL_CreateWindow( "Isometry", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
        if ( !window ) {
                std::cout << "Error creating window: " << SDL_GetError()  << std::endl;
                system("pause");
                return false;
        }

        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        if ( !renderer ) {
                std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
                return false;
        }

        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear( renderer );
        return true;
}

void kill() {
        // Quit
        SDL_DestroyRenderer( renderer );
        SDL_DestroyWindow( window );
        SDL_Quit();
}


