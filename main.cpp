#include "graphic/graphic.h"

int main(){
    Display display{60.l};

    Canvas canvas{0, 0, 100, 30};
    Label label{12, 1, "012345678901234567890123456789012345678901234567890123456789"};
    Label label1{2, 2, "[SYSTEM]:"};
    Marquee marq1{12, 2, 60, 1, 1.f, "This is Test Message1 "};

    Label label2{2, 3, "[SYSTEM]:"};
    Marquee marq2{12, 3, 60, 1, 15.f, "This is Test Message22"};

    Label label3{2, 4, "[SYSTEM]:"};
    Marquee marq3{12, 4, 60, 1, 30.f, "This is Test Message333"};

    Label label4{2, 5, "[SYSTEM]:"};
    Marquee marq4{12, 5, 60, 1, 60.f, "This is Test Message4444"};

    Label label5{2, 6, "[SYSTEM]:"};
    Marquee marq5{12, 6, 60, 1, 120.f, "This is Test Message55555"};

    Label label6{2, 7, "[SYSTEM]:"};
    Marquee marq6{12, 7, 60, 1, 240.f, "This is Test Message666666"};

    Label label7{2, 8, "[SYSTEM]:"};
    Marquee marq7{12, 8, 60, 1, 480.f, "This is Test Message7777777"};

    Canvas sub_canvas{74, 0, 24, 28};

    Label debug_label{2, 1, "DEBUG INFO"};
    Label fpsChecker{2, 3, "FPS: 00.00"};
    Label fpsRChecker{2, 4, "FPSR: 0.00000"};
    Label watingChecker{2, 5, "WATING: 0.00000"};
    Label drawingChecker{2, 6, "DRAWING: 0.00000"};

    std::function<void(Object*)> show_fps = [&](Object* object){
        long double fps = display.getRealFPS();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "FPS: %.2Lf", fps);
        lbl->setContent({content});
    };

    std::function<void(Object*)> show_fpsr = [&](Object* object){
        long double fps = display.getRealFPS();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "FPSR: %.5Lf", 1.l/fps);
        lbl->setContent({content});
    };


    std::function<void(Object*)> show_wating = [&](Object* object){
        long double wating = display.getWating();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "WATING: %.5Lf", wating);
        lbl->setContent({content});
    };

    std::function<void(Object*)> show_drawing = [&](Object* object){
        long double fps = display.getRealFPS();
        long double wating = display.getWating();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "DRAWING: %.5Lf", std::max(0.l, 1.l/fps - wating));
        lbl->setContent({content});
    };

    fpsChecker.setEvent(&show_fps);
    fpsRChecker.setEvent(&show_fpsr);
    watingChecker.setEvent(&show_wating);
    drawingChecker.setEvent(&show_drawing);

    /*
    ProgressBar progress_bar{3, console_height-4, range, "#"};
    for(auto& i: progress_bar(can range_base)){
        some task;
    }
    */

    display.append(&canvas);
    
    canvas.append(&label);
    canvas.append(&label1);
    canvas.append(&marq1);

    canvas.append(&label2);
    canvas.append(&marq2);

    canvas.append(&label3);
    canvas.append(&marq3);

    canvas.append(&label4);
    canvas.append(&marq4);

    canvas.append(&label5);
    canvas.append(&marq5);

    canvas.append(&label6);
    canvas.append(&marq6);

    canvas.append(&label7);
    canvas.append(&marq7);

    //canvas.append(&progress_bar);

    canvas.append(&sub_canvas);

    sub_canvas.append(&debug_label);
    sub_canvas.append(&fpsChecker);
    sub_canvas.append(&fpsRChecker);
    sub_canvas.append(&watingChecker);
    sub_canvas.append(&drawingChecker);

    display.draw();
    
    return 0;
}