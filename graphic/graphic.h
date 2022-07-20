#pragma once
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <thread>
#include <future>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

//use for ns, 
using namespace std::literals; 

using std::chrono::milliseconds;
using std::chrono::steady_clock;

using uint = unsigned int;
class Display;

void moveCursor(int x, int y);  
void endHandler(Display* display);

class Object{
    public:
        Object(int x, int y, uint width, uint height)
        :x_(x), y_(y), width_(width), height_(height), event_(nullptr){
            is_draw = false;
        };

        virtual void draw() = 0;
        virtual void redraw(int, int) = 0;
        virtual bool isIn(int x, int y){
            return x_ <= x && x < x_ + width_ && 
                   y_ <= y && y < y_ + height_;
        }

        virtual void setEvent(std::function<void(Object*)>* event){ event_ = event; }
        virtual void event(){ if(event_) (*event_)(this); }

        void setDisplay(Display* display){ display_ = display; }

        virtual void setX(int x){ x_ = x; };
        int getX(){ return x_; };

        virtual void setY(int y){ y_ = y; };
        int getY(){ return y_; };

        virtual void setWidth(int w){width_ = w;};
        int getWidth(){ return width_; };

        virtual void setHeight(int h){height_ = h; };
        int getHeight(){ return height_; };


        bool is_draw;
    protected:
        Display* display_;

        int x_, y_;
        uint width_, height_;

        std::function<void(Object*)>* event_;
};

class Display{
    public:
        Display(long double FPS);
        Display(Display&) = delete;
        Display(Display&&) = delete;

        void append(Object*);

        void draw();
        void drawEnd();
        
        const long double getFPS();
        const long double getRealFPS();
        const long double getWating();

        ~Display();

    private:
        long double FPS_;           //defined FPS
        long double real_FPS_;      //real FPS (it is mutable FPS)
        long double timing_;        //wating time for keep up FPS.
        long double wating_;        //real wating time

        int console_width_;         //terminal width
        int console_height_;        //terminal height

        /*
        * 0: drawing start time
        * 1: drawing end time
        * 2: idle waited time 
        */
        decltype(steady_clock::now()) timer[3]; 

        //drawing signal
        bool drawing_;
        bool drawend_;

        //objects for drawing
        std::vector<Object*> objects_;
};

class Canvas: public Object{
    public:
        Canvas(int, int, uint, uint);

        virtual void draw() override;
        virtual void redraw(int, int) override;

        virtual void event() override;

        void append(Object* object);

    private:
        std::vector<Object*> contents_;
};

class Label: public Object{
    public:
        Label(int, int, const std::string&, bool = false);

        virtual void draw() override;
        virtual void redraw(int, int) override;

        void setContent(const std::string&);

        std::string getContent(){ return content_; };

    private:
        std::string content_;
        bool vertical_;


};

class Marquee: public Object{
    public:
        Marquee(int, int, uint, uint, double, 
                const std::string&, bool = false, bool = false);

        virtual void draw() override;

        virtual void redraw(int ,int) override;

        virtual void event() override;

        virtual void setX(int) override;
        virtual void setY(int) override;

    private:
        int text_x_, text_y_;

        double velocity_;
        long double move_;

        std::string content_;
        bool vertical_;
        bool clockwise_;

        int* string_position_;

        int value_;
};