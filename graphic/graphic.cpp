#include "graphic.h"

void moveCursor(int x, int y){
    printf("\033[%dd\033[%dG", y, x);
    return;
}

Display::Display(long double FPS)
:FPS_(FPS), timing_(1.l/FPS), drawing_(true){
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);

    console_width_ = (int)(w.ws_col);
    console_height_ = (int)(w.ws_row);

    printf("\e[?25l");      //hide cursor
    setbuf(stdout, NULL);   //buffer off
    system("clear");
};

void Display::append(Object* object){
    object->setDisplay(this);
    objects_.push_back(object);
}

void Display::draw(){
    while(drawing_){
        timer[0] = steady_clock::now();
        //Object draw start part

        for(auto& i: objects_){
            i->draw(); 
            i->event();
        }

        //Object draw end part
        timer[1] = steady_clock::now();

        //period what drawing and object event
        auto draw_rate = std::chrono::duration_cast<milliseconds>(timer[1] - timer[0]);

        //for keep up FPS
        wating_ = std::max((long double)(timing_ - (long double)draw_rate.count() / 1000.f), 0.l);
        std::this_thread::sleep_for(milliseconds((int)(wating_ * 1000.l)));

        timer[2] = steady_clock::now();
        auto diff = std::chrono::duration_cast<milliseconds>(timer[2] - timer[0]);

        //real FPS.
        real_FPS_ = 1000.l/diff.count();

        #ifdef DEBUG
            //Debuging print
            moveCursor(3, console_height_ - 2);
            printf("FPS: %.5Lf FPSR: %.5Lf", real_FPS_, 1.l/real_FPS_);
            moveCursor(3, console_height_ - 1);
            printf("DRAW: %.5Lf, WATING: %.5Lf D+W: %.5LF", (long double)draw_rate.count() / 1000.f, wating_, (long double)draw_rate.count() / 1000.f + wating_);
        #endif
    }
}

const long double Display::getFPS(){
    return FPS_;
}

const long double Display::getRealFPS(){
    return real_FPS_;
}

const long double Display::getWating(){
    return wating_;
}

Display::~Display(){
    printf("\e[?25h");  //show cursor
    system("clear");
}

Canvas::Canvas(int x, int y, uint width, uint height)
:Object(x+1, y+1, width, height){
}

void Canvas::draw(){

    //draw border
    if(!is_draw){
        for(int i = x_; i < x_ + width_; i ++){
            moveCursor(i, y_); printf("─");
        }
        
        for(int i = x_; i < x_ + width_; i ++){
            moveCursor(i, y_ + height_ - 1); printf("─");
        }
        

        for(int i = y_; i < y_ + height_; i ++){
            moveCursor(x_, i); printf("│");
        }

        for(int i = y_; i < y_ + height_; i ++){
            moveCursor(x_ + width_ - 1, i); printf("│");
        }
        moveCursor(x_, y_); printf("┌");
        moveCursor(x_+width_-1, y_); printf("┐");
        moveCursor(x_, y_+height_-1); printf("└");
        moveCursor(x_+width_-1, y_+height_-1); printf("┘");
        is_draw = true;
    }

    //content drawing
    for(auto& content: contents_){
        content->draw();
    }

}

void Canvas::redraw(int x, int y){
    for(auto& content: contents_){
        if(content->isIn(x, y)) content->redraw(x, y);
    }
}

void Canvas::event(){
    Object::event();
    for(auto& content: contents_){
        content->event();
    }
}

void Canvas::append(Object* object){
    object->setX(object->getX() + x_);
    object->setY(object->getY() + y_);
    object->setDisplay(display_);

    contents_.push_back(object);
}

Label::Label(int x, int y, const std::string& content, bool vertical)
:Object(x, y, 0, 0){
    content_ = content;
    vertical_ = vertical;

    width_ = content.size();
    height_ = 1;

    if(vertical_) std::swap(width_, height_);
}

void Label::draw(){
    if(is_draw) return;

    uint size = (vertical_)? height_: width_;

    for(int idx = 0; idx < size; idx ++){
        if(!vertical_) moveCursor(x_ + idx, y_);
        else moveCursor(x_, y_ + idx);

        printf("%c", content_[idx]);
    }

    is_draw = true;
}

void Label::redraw(int x, int y){
    moveCursor(x, y);
    if(!vertical_) printf("%c", content_[x - x_]);
    else printf("%c", content_[y- y_]);
}

void Label::setContent(const std::string& content){
    is_draw = false;
    content_ = content;
}

Marquee::Marquee(int x, int y, uint width, uint height, double velocity,
                 const std::string& content, bool vertical, bool clockwise)
    :Object(x, y, width, height), content_(content),
    velocity_(velocity), vertical_(vertical), clockwise_(clockwise){
        text_x_ = x;
        text_y_ = y;
        move_ = 0.l;

        value_ = (int)(content_.size() / width_) + 1;

        string_position_ = new int[content_.size()];
        for(int i = 0; i < content_.size(); i ++) string_position_[i] = i;
}

void Marquee::draw(){
    //make a empty space
    int target_size = (vertical_)? height_: width_;
    int target_point = (vertical_)? y_: x_;

    for(int idx = target_point; idx < target_point + target_size; idx ++){
        if(vertical_) moveCursor(x_, idx);
        else moveCursor(idx, y_);
        printf(" ");
    }

    //draw the text
    for(int i = 0; i < content_.size(); i ++){
        if(string_position_[i] >= target_size) continue;
        if(vertical_) moveCursor(x_, y_ + string_position_[i]);
        else moveCursor(x_ + string_position_[i], y_);
        printf("%c", content_[i]);
    }
}

void Marquee::redraw(int x, int y){
    return;
}

void Marquee::event(){
    Object::event();
    long double FPS = display_->getRealFPS();

    if(std::isnan(FPS)) FPS = display_->getFPS();   

    move_ += velocity_ * (1.l/ FPS);
    
    if(move_ >= 1.l){
        for(int i = 0; i < content_.size(); i ++){
            if(clockwise_) string_position_[i] += (int)move_;
            else{
                string_position_[i] += ((int)((value_ * width_)) - (int)move_);
            }
            
            
            if(vertical_) string_position_[i] %= (value_ * height_);
            else string_position_[i] %= (value_ * width_);
        }
        move_ = 0.l;
    }
}

void Marquee::setX(int x){
    x_ = x;
    text_x_ = x;
}

void Marquee::setY(int y){
    y_ = y;
    text_y_ = y;
}
