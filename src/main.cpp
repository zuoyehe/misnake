#include "entity.h"
#include <SDL_ttf.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() < 0) {
        std::cout << "TTF could not initialize! SDL_Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("Snake",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

    // 分数
    int16_t score = 0;
    // 蛇
    Snake s {};
    // 食物
    Food f {};
    // 随机数生成器
    RandomNumberGenerator rng;
    // 计时器
    Uint32 timer = 0;

    initialize(renderer, s, f, score, rng);
    /* Main Loop */
    bool quit = false;
    SDL_Event event;
    bool dirchanged = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
#define KEY event.key.keysym.sym
            case SDL_KEYDOWN:
                if (KEY == SDLK_w && s.dir != snake_dir::DIR_DOWN && !dirchanged) {
                    s.dir = snake_dir::DIR_UP;
                    dirchanged = true;
                } else if (KEY == SDLK_s && s.dir != snake_dir::DIR_UP && !dirchanged) {
                    s.dir = snake_dir::DIR_DOWN;
                    dirchanged = true;
                } else if (KEY == SDLK_a && s.dir != snake_dir::DIR_RIGHT && !dirchanged) {
                    s.dir = snake_dir::DIR_LEFT;
                    dirchanged = true;
                } else if (KEY == SDLK_d && s.dir != snake_dir::DIR_LEFT && !dirchanged) {
                    s.dir = snake_dir::DIR_RIGHT;
                    dirchanged = true;
                }
#undef KEY
                break;
            /* 退出 */
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }
        /* 判断现在距离上次过了多久 */
        if (SDL_GetTicks() - timer > FIXED_TIME) /* 如果超过了 */
        {
            draw(renderer, s, f);
            s.snake_move();
            dirchanged = false;
            if (!s.collision_detection(renderer)) {
                s.snake_eat(f, score, rng);
                /* 重新计时 */
                timer = SDL_GetTicks();
            } else {
                quit = true;
            }
        }
    }

    // 销毁渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // 退出SDL子系统
    SDL_Quit();

    return 0;
}

void draw(SDL_Renderer* renderer, const Snake& s, const Food& f)
{
    // 背景颜色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // 清屏
    SDL_RenderClear(renderer);
    // 渲染蛇
    for (const auto be : s.get_snakeList()) {
        SDL_Rect rect = { be->get_x() * BLOCK_SIZE, be->get_y() * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
        SDL_SetRenderDrawColor(renderer, 255, 105, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
    // 渲染食物
    SDL_Rect rect = { f.get_foodBlock()->get_x() * BLOCK_SIZE, f.get_foodBlock()->get_y() * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
    SDL_SetRenderDrawColor(renderer, f.get_food_color().r, f.get_food_color().g, f.get_food_color().b, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    // 显示
    SDL_RenderPresent(renderer);
}

void initialize(SDL_Renderer* renderer, Snake& snake, Food& food, int16_t& score, RandomNumberGenerator& rng)
{
    score = 0;
    snake = std::move(Snake(4, rng));
    food = std::move(Food(snake, rng));
    draw(renderer, snake, food);
}

void game_over(SDL_Renderer* renderer)
{
    // 背景颜色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // 清屏
    SDL_RenderClear(renderer);
    // 打开字体资源
    TTF_Font* font = TTF_OpenFont("/home/gaoyuan28/misnake/res/ttf/UbuntuMono-R.ttf", 52);
    if (!font) {
        std::cout << "TTF could not open font! SDL_Error: " << TTF_GetError() << std::endl;
        return;
    }
    // 字体颜色
    SDL_Color textColor = { 225, 0, 10, 255 };
    // 使用指定的字体和颜色渲染文本到Surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, "GAME OVER", textColor);
    // 如果渲染失败，输出错误信息并返回
    if (!textSurface) {
        std::cout << "Could not render text to surface!" << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }
    // 从Surface创建Texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (!textTexture) {
        std::cout << "Could not create texture from surface!" << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        SDL_FreeSurface(textSurface); // 清理Surface资源
        return;
    }

    // 设置Texture的渲染位置（例如，屏幕中心）
    SDL_Rect textRect = { (WINDOW_WIDTH - textSurface->w) / 2, (WINDOW_HEIGHT - textSurface->h) / 2, textSurface->w, textSurface->h };

    // 渲染Texture到屏幕
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // 更新屏幕以显示Texture
    SDL_RenderPresent(renderer);
    SDL_Delay(500);

    // 清理资源
    TTF_CloseFont(font); //释放字体资源
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}