#include "entity.h"
RandomNumberGenerator::RandomNumberGenerator() : gen(rd()), dis(HEIGHT_BLOCK_COUNT*0.2, HEIGHT_BLOCK_COUNT*0.8) {}  
int16_t RandomNumberGenerator::generate() {  
    return dis(gen);
}
blockElement::blockElement(int16_t x, int16_t y):x(x), y(y){}
int16_t blockElement::get_x() const {return x;}
int16_t blockElement::get_y() const {return y;}

Snake::Snake():snakeBody{} {}
Snake::Snake(int16_t length, RandomNumberGenerator &rng):snakeBody{} {
    // 生成蛇头，放入链表
    int16_t rand_x = rng.generate();
    int16_t rand_y = rng.generate();
    blockElement *snake_head = new blockElement(rand_x, rand_y);
    snakeBody.push_back(snake_head);
    blockElement *snakePart = nullptr;
    blockElement *pre = snake_head;
    // 生成蛇身，放入链表，蛇默认水平朝右
    for (int16_t i=0;i< length-1; ++i) {
        snakeBody.emplace_back(new blockElement(pre->get_x()-1, pre->get_y()));
        pre = snakeBody.back();
    }
}
Snake& Snake::operator=(Snake &&other) {
    if (this != &other) {  
        for (auto it = snakeBody.begin(); it != snakeBody.end(); ++it) {
            delete *it;
        }
        snakeBody.clear();
        snakeBody = std::move(other.snakeBody);  // 然后获取其他对象的资源  
        // 确保其他对象不再拥有该资源  
    } 
    return *this;
}
void Snake::add_from_head(snake_dir dir){
    switch (dir)
    {
    case snake_dir::DIR_UP:
        snakeBody.emplace_front(new blockElement(snakeBody.front()->get_x(), snakeBody.front()->get_y()-1));
        /* code */
        break;
    case snake_dir::DIR_DOWN:
        snakeBody.emplace_front(new blockElement(snakeBody.front()->get_x(), snakeBody.front()->get_y()+1));
        /* code */
        break;
    case snake_dir::DIR_LEFT:
        snakeBody.emplace_front(new blockElement(snakeBody.front()->get_x()-1, snakeBody.front()->get_y()));
        /* code */
        break;
    case snake_dir::DIR_RIGHT:
        snakeBody.emplace_front(new blockElement(snakeBody.front()->get_x()+1, snakeBody.front()->get_y()));
        /* code */
        break;
    default:
        break;
    }
}

void Snake::delete_from_tail() {
    if (snakeBody.size()  == 0) {
        return;
    }
    auto ps = snakeBody.back();        
    snakeBody.pop_back();
    delete ps;
}

Snake::~Snake() {
    for (auto it = snakeBody.begin(); it != snakeBody.end(); ++it) {
        delete *it;
    }
}
const std::list<blockElement*>& Snake::get_snakeList() const {
    return snakeBody;
}

food_color::food_color(int16_t r, int16_t g, int16_t b):r(r),g(g),b(b){};

Food::Food() {}
Food::Food(RandomNumberGenerator &rng) {
    int16_t rand_x = rng.generate();
    int16_t rand_y = rng.generate();
    foodBlock = new blockElement(rand_x, rand_y);
    switch (rng.generate() % 3)
    {
    case 0:
        fc = HUAWEI;
        break;
    case 1:
        fc = OPPO;
        break;
    case 2:
        fc = VIVO;
        break;
    default:
        break;
    }
}
Food& Food::operator=(Food &&other) {
    if (this != &other) {  
        delete foodBlock;  // 首先删除当前对象的资源  
        foodBlock = other.foodBlock;  // 然后获取其他对象的资源  
        fc = other.fc;
        other.foodBlock = nullptr;  // 确保其他对象不再拥有该资源  
    } 
    return *this;
}

const food_color Food::get_food_color() const {
    return fc;
}

Food::~Food() {
    delete foodBlock;
}
const blockElement* Food::get_foodBlock() const {
    return foodBlock;
}