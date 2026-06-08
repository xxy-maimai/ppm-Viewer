#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <string>

using namespace std;

//lazy habit
void succf(int flag){
	if(flag < 0){
		cout << "SDL Error: " << SDL_GetError();
	}
}

//some variables & constants
SDL_Window *window = NULL;
const int wWidth = 1000, wHeight = 750;
const char* wTitle = "ppm Viewer";
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
int pWidth, pHeight;

void ReadImageFile(char* path){
	ifstream fin(path, ios::binary);
	string head;
	string color_max;
	fin >> head >> pWidth >> pHeight >> color_max;
	if(head != "P6"){
		cout << "Only support P6 format" << '\n';
	}
	
	surface = SDL_CreateSurface(pWidth, pHeight, SDL_PIXELFORMAT_RGBA8888);
	if(surface == NULL){
		cout << "Failed to create surface: " << SDL_GetError();
	}
	SDL_FillSurfaceRect(surface, NULL, 0);
	char ch;
	int index = 0, count = 0;
	int r, g, b;
	while(fin.get(ch)){
		switch(index){
			case 0:
				b = (int) ch;
				break;
			
			case 1:
				r = (int) ch;
				break;
				
			case 2:
				g = (int) ch;
				break;
		}
		index++;
		if(index == 3){
			SDL_WriteSurfacePixel(surface, count % pWidth, count / pWidth, r, g, b, 0xff);
			index = 0;
			count++;
		}
	}
	
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(texture == NULL){
		cout << "Failed to create texture: " << SDL_GetError();
	}
}

int main(int argc, char* argv[]){
	succf(SDL_Init(SDL_INIT_VIDEO));
	
	window = SDL_CreateWindow(wTitle, wWidth, wHeight, 0);
	if(window == NULL){
		cout << "Failed to create window: " << SDL_GetError();
	}
	
	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL){
		cout << "Failed to create renderer: " << SDL_GetError();
	}
	
	ReadImageFile(argv[1]);
	
	SDL_Event event;
	bool running = true;
	while(running){
		//event handling
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_EVENT_QUIT:
				running = false;
		}
		
		//render
		SDL_RenderClear(renderer);
		SDL_FRect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = pWidth;
		rect.h = pHeight;
		SDL_RenderTexture(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroySurface(surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
