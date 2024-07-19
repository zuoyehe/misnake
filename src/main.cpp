#include <SDL.h>  
#include <iostream>
  
constexpr int WINDOW_WIDTH = 1000;  
constexpr int WINDOW_HEIGHT = 800;  

void clear(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景  
    SDL_RenderClear(renderer); // 清除屏幕（如果设置了背景色）  
}

void draw_snake(SDL_Renderer* renderer, const SDL_Rect &orangeRect) {
    // 设置绘制颜色为橙色  
    SDL_SetRenderDrawColor(renderer, 255, 126, 0, 255); // RGBA  
  
    // 绘制一个橙色的小方块  
    SDL_RenderFillRect(renderer, &orangeRect); 
}
  
int main(int argc, char* argv[]) {  
    // 初始化SDL  
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;  
        return 1;  
    }  
  
    // 创建窗口  
    SDL_Window* window = SDL_CreateWindow("Snake",  
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  
                                          WINDOW_WIDTH, WINDOW_HEIGHT,  
                                          SDL_WINDOW_SHOWN);  
    if (window == nullptr) {  
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;  
        SDL_Quit();  
        return 1;  
    }  
  
    // 创建渲染器  
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);  
    if (renderer == nullptr) {  
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;  
        SDL_DestroyWindow(window);  
        SDL_Quit();  
        return 1;  
    }  

    int orangeRectX = WINDOW_WIDTH / 2 - 10;  
    int orangeRectY = WINDOW_HEIGHT / 2 - 10; 
    SDL_Rect orangeRect = {orangeRectX, orangeRectY, 20, 20};  

    clear(renderer);
    draw_snake(renderer, orangeRect);
  
    // 更新屏幕以显示绘制的内容  
    SDL_RenderPresent(renderer);  
  
    // 等待用户按键  
    SDL_Event e;  
    bool quit = false;  
    while (!quit) {  
        // 处理事件  
        while (SDL_PollEvent(&e) != 0) {  
            if (e.type == SDL_QUIT) {  
                quit = true;  
            } else if (e.type == SDL_KEYDOWN) {  
                switch (e.key.keysym.sym) {  
                    case SDLK_w:  
                        orangeRect.y -= 8; // 向上移动  
                        break;  
                    case SDLK_s:  
                        orangeRect.y += 8; // 向下移动  
                        break;  
                    case SDLK_a:  
                        orangeRect.x -= 8; // 向左移动  
                        break;  
                    case SDLK_d:  
                        orangeRect.x += 8; // 向右移动  
                        break;  
                    // 可以添加更多按键  
                }  
            }  
        }  
        clear(renderer);
        draw_snake(renderer, orangeRect);
    
        // 更新屏幕以显示绘制的内容  
        SDL_RenderPresent(renderer); 
    }
  
    // 销毁渲染器和窗口  
    SDL_DestroyRenderer(renderer);  
    SDL_DestroyWindow(window);  
  
    // 退出SDL子系统  
    SDL_Quit();  
  
    return 0;  
}