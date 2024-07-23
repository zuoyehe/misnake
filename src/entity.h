#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <cstdint>
#include <stdlib.h>
#include <random> 
constexpr int16_t WIDTH_BLOCK_COUNT = 50;
constexpr int16_t HEIGHT_BLOCK_COUNT = 40;
constexpr int16_t  BLOCK_SIZE = 20;  
constexpr int16_t WINDOW_WIDTH = WIDTH_BLOCK_COUNT * BLOCK_SIZE;  
constexpr int16_t WINDOW_HEIGHT = HEIGHT_BLOCK_COUNT * BLOCK_SIZE;  
constexpr int16_t FIXED_TIME = 80;

enum class snake_dir
{
    DIR_DOWN=0,  /* 下 */
    DIR_LEFT,  /* 左 */
    DIR_RIGHT, /* 右 */
    DIR_UP     /* 上 */
};

class RandomNumberGenerator {  
private:  
    std::random_device rd; // 用于获取一个种子 
    std::mt19937 gen;  
    std::uniform_int_distribution<> dis;  
public:  
    RandomNumberGenerator();
    int16_t generate();
};  

class blockElement{
public:
    blockElement(int16_t x, int16_t y);
    int16_t get_x() const;
    int16_t get_y() const;
private:
    int16_t x,y;
};

class Snake{
public:
    Snake();
    Snake(int16_t length, RandomNumberGenerator &rng);
    Snake& operator=(Snake &&other);
    void add_from_head(snake_dir dir);
    void delete_from_tail();
    ~Snake();
    const std::list<blockElement*>& get_snakeList() const;
private:
    std::list<blockElement*> snakeBody;
};

class food_color{
public:
    food_color() {};
    food_color(int16_t, int16_t, int16_t);
    int16_t r=0, g=0, b=0;
};

const food_color HUAWEI = {225, 0, 10}; 
const food_color OPPO = {0, 108, 49}; 
const food_color VIVO = {66, 95, 252}; 

class Food {
public:
    Food();
    Food(RandomNumberGenerator&);
    Food& operator=(Food &&other);
    ~Food();
    const blockElement* get_foodBlock() const;
    const food_color get_food_color() const;

private:
    blockElement* foodBlock;
    food_color fc;
};
#endif
