#include <SDL2/SDL.h>  
#include <iostream>  

int main(int argc, char* argv[]) {  
    SDL_Init(SDL_INIT_VIDEO);  
    SDL_Window* window = SDL_CreateWindow("SDL2 Example",  
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  
                                          640, 480, SDL_WINDOW_SHOWN);  
    if (!window) {  
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;  
        return 1;  
    }  

    SDL_Delay(3000);  // 延迟3秒  

    SDL_DestroyWindow(window);  
    SDL_Quit();  
    return 0;  
}