#include <SDL.h>  
#include <iostream>
#include <list>

constexpr int32_t WIDTH_BLOCK_COUNT = 50;
constexpr int32_t HEIGHT_BLOCK_COUNT = 40;
constexpr int32_t  BLOCK_SIZE = 20;  
constexpr int32_t WINDOW_WIDTH = WIDTH_BLOCK_COUNT * BLOCK_SIZE;  
constexpr int32_t WINDOW_HEIGHT = HEIGHT_BLOCK_COUNT * BLOCK_SIZE;  

class blockElement{
public:
    blockElement(int32_t x, int32_t y):x(x),y(y){}
    int32_t get_x() const {return x;}
    int32_t get_y() const {return y;}
private:
    int32_t x,y;
};

class Snake{
public:
    Snake(int32_t length):snakeBody{} {
        // 生成蛇头，放入链表
        int32_t rand_x = rand()%WIDTH_BLOCK_COUNT;
        int32_t rand_y = rand()%HEIGHT_BLOCK_COUNT;
        blockElement *snake_head = new blockElement(rand_x, rand_y);
        snakeBody.push_back(snake_head);
        blockElement *snakePart = nullptr;
        blockElement *pre = snake_head;
        // 生成蛇身，放入链表，蛇默认水平朝右
        for (int32_t i=0;i< length-1; ++i) {
            snakeBody.emplace_back(new blockElement(pre->get_x()-1, pre->get_y()));
            pre = snakeBody.back();
        }
    }
    ~Snake() {
        for (auto it = snakeBody.begin(); it != snakeBody.end(); ++it) {
            delete *it;
        }
    }
    auto get_snakeList() const {
        return snakeBody;
    }

private:
    std::list<blockElement*> snakeBody;
};

class Food
{
public:
    Food() {
        int32_t rand_x = rand() % WIDTH_BLOCK_COUNT;
        int32_t rand_y = rand() % HEIGHT_BLOCK_COUNT;
        foodBlock = new  blockElement(rand_x, rand_y);
    }
    ~Food() {
        delete foodBlock;
    }
    auto get_foodBlock() const {
        return foodBlock;
    }

private:
    blockElement* foodBlock;
};


// 初始化场景
void initialize(SDL_Renderer*);

// 绘制场景
void draw(SDL_Renderer*, const Snake&, const Food&);

// 蛇移动
void snake_move();

// 蛇吃食物
void snake_eat();

int32_t score;
  
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

    // 使用当前时间作为随机数生成的种子  
    srand(static_cast<unsigned int>(time(0)));

    initialize(renderer);

    // 等待用户按键  
    // SDL_Event e;  
    // bool quit = false;  
    // while (!quit) {  
    //     // 处理事件  
    //     while (SDL_PollEvent(&e) != 0) {  
    //         if (e.type == SDL_QUIT) {  
    //             quit = true;  
    //         } else if (e.type == SDL_KEYDOWN) {  
    //             switch (e.key.keysym.sym) {  
    //                 case SDLK_w:  
    //                     orangeRect.y -= 8; // 向上移动  
    //                     break;  
    //                 case SDLK_s:  
    //                     orangeRect.y += 8; // 向下移动  
    //                     break;  
    //                 case SDLK_a:  
    //                     orangeRect.x -= 8; // 向左移动  
    //                     break;  
    //                 case SDLK_d:  
    //                     orangeRect.x += 8; // 向右移动  
    //                     break;  
    //                 // 可以添加更多按键  
    //             }  
    //         }  
    //     }  
    //     clear(renderer);
    //     draw_snake(renderer, orangeRect);
    
    //     // 更新屏幕以显示绘制的内容  
    //     SDL_RenderPresent(renderer); 
    // }
    SDL_Delay(1000);
  
    // 销毁渲染器和窗口  
    SDL_DestroyRenderer(renderer);  
    SDL_DestroyWindow(window);  
  
    // 退出SDL子系统  
    SDL_Quit();  
  
    return 0;  
}

void draw(SDL_Renderer* renderer, const Snake &s, const Food &f) {
    // 背景颜色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // 清屏
    SDL_RenderClear(renderer);
    // 渲染蛇
    for (auto& be : s.get_snakeList())
    {
        SDL_Rect rect = {be->get_x() * BLOCK_SIZE, be->get_y() * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 126, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
    // 渲染食物
    SDL_Rect rect = {f.get_foodBlock()->get_x() * BLOCK_SIZE, f.get_foodBlock()->get_y() * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 126, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    // 显示
    SDL_RenderPresent(renderer);
}

void initialize(SDL_Renderer* renderer) {
    score = 0;
    draw(renderer, Snake(4), Food());
}