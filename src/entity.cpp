#include "entity.h"
blockElement::blockElement(int16_t x, int16_t y):x(x), y(y){}
int16_t blockElement::get_x() const {return x;}
int16_t blockElement::get_y() const {return y;}

Snake::Snake():snakeBody{} {}
Snake::Snake(int16_t length):snakeBody{} {
    // 生成蛇头，放入链表
    int16_t rand_x = rand()%WIDTH_BLOCK_COUNT;
    int16_t rand_y = rand()%HEIGHT_BLOCK_COUNT;
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
    snakeBody = std::move(other.snakeBody);
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


Food::Food() {
    int16_t rand_x = rand() % WIDTH_BLOCK_COUNT;
    int16_t rand_y = rand() % HEIGHT_BLOCK_COUNT;
    foodBlock = new  blockElement(rand_x, rand_y);
}
Food& Food::operator=(Food &&other) {
    foodBlock = other.foodBlock;
    other.foodBlock = nullptr;
    return *this;
}

Food::~Food() {
    delete foodBlock;
}
const blockElement* Food::get_foodBlock() const {
    return foodBlock;
}