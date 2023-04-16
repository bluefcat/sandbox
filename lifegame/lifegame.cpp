#include "../graphic/graphic.h"
#include <unistd.h>
#include <signal.h>

#define MAXROW 40
#define MAXCOL 30

//field's shape is taurus
#define ROWP(X) ((X+1)%(MAXROW))
#define COLP(X) ((X+1)%(MAXCOL))
#define ROWM(X) ((X-1) < 0? (MAXROW-1): (X-1))
#define COLM(X) ((X-1) < 0? (MAXCOL-1): (X-1))

Display* display;

//signal
void sigint_handler(int n){
    endHandler(display);
    delete display;
    moveCursor(0, 0);
    printf("Good bye\n");
	signal(SIGINT, SIG_DFL);
    return;
}

int rowp(int x){ return ROWP(x); }
int colp(int x){ return COLP(x); }
int rowm(int x){ return ROWM(x); }
int colm(int x){ return COLM(x); }

class LifeGame: public Object{
    public:
        LifeGame();

        virtual void draw() override;
        virtual void redraw(int, int) override;

        virtual void event() override ;

        void erase();

        bool isAlive(int x, int y);

    private:
        int map[MAXROW*MAXCOL] = { 0, };
        int buffer[MAXROW*MAXCOL] = { 0, };
};

LifeGame::LifeGame()
:Object{0, 0, MAXROW, MAXCOL}{
	for(int i = 0; i < MAXCOL; i ++){
		for(int j = 0; j < MAXROW; j ++){
			scanf("%d", map + (j*MAXCOL) + i);
		}
	}
}

void LifeGame::draw(){
    erase();
    moveCursor(0, 0);
	char character[] = {' ', '@', '%'};
	for(int i = 0; i < MAXCOL; i ++){
		for(int j = 0; j < MAXROW; j ++){
			char target = 0;
			int data = map[j*MAXCOL+i];
			switch(data){
				case 0:
				case 1:
					target = character[data];
					break;
				default:
					target = character[2];
			}			
			
			moveCursor(2*j, i);
			printf("%c", target);
		}
	}
	return;
}

void LifeGame::redraw(int x, int y){
    return;
}

void LifeGame::event(){
    for(int i = 0; i < MAXCOL; i ++){
		for(int j = 0; j < MAXROW; j ++){
			buffer[j*MAXCOL+i] = isAlive(j, i);
		}
	}

	//copy cache to map
	for(int i = 0; i < MAXROW*MAXCOL; i ++){
		map[i] = buffer[i];
	}

	return;
}

void LifeGame::erase(){

	moveCursor(0, 0);
	for(int i = 0; i < MAXCOL; i ++){
		for(int j = 0; j < MAXROW; j ++){
			if(map[j*MAXCOL+i]){
				moveCursor(2*j, i);
				printf(" ");
			}
		}
	}
	return;
}

bool LifeGame::isAlive(int x, int y){
/*
	if is alive in rule, returns true) or died return false)
*/
	int count = 0;

	//8 direction
	int (*xp[8])(int) = { rowm, NULL, rowp, NULL, rowm, rowm, rowp, rowp };
	int (*yp[8])(int) = { NULL, colp, NULL, colm, colm, colp, colm, colp };

	for(int i = 0; i < 8; i ++){
		int nx = xp[i] != NULL? xp[i](x): x;
		int ny = yp[i] != NULL? yp[i](y): y;
		if(map[nx * MAXCOL + ny] != 0) count ++;		
	}
	
	//rules
	if(map[x * MAXCOL + y] != 0){
		if(2 <= count && count <= 3) return true;
		return false;
	}
	else{
		if(count == 3) return true;
		return false;
	}

}

int main(int argc, char* argv[]){
	signal(SIGINT, sigint_handler);
    display = new Display{3.l};
    
    Canvas canvas{0, 0, MAXROW+1, MAXCOL+1};

    LifeGame lifegame{};

    Canvas sub_canvas{2 * MAXROW+10, 0, 24, 28};

    Label debug_label{2, 1, "DEBUG INFO"};
    Label fpsChecker{2, 3, "FPS: 00.00"};
    Label fpsRChecker{2, 4, "FPSR: 0.00000"};
    Label watingChecker{2, 5, "WATING: 0.00000"};
    Label drawingChecker{2, 6, "DRAWING: 0.00000"};

    std::function<void(Object*)> show_fps = [&](Object* object){
        long double fps = display->getRealFPS();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "FPS: %.2Lf", fps);
        lbl->setContent({content});
    };

    std::function<void(Object*)> show_fpsr = [&](Object* object){
        long double fps = display->getRealFPS();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "FPSR: %.5Lf", 1.l/fps);
        lbl->setContent({content});
    };


    std::function<void(Object*)> show_wating = [&](Object* object){
        long double wating = display->getWating();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "WATING: %.5Lf", wating);
        lbl->setContent({content});
    };

    std::function<void(Object*)> show_drawing = [&](Object* object){
        long double fps = display->getRealFPS();
        long double wating = display->getWating();
        Label* lbl = dynamic_cast<Label*>(object);
        char content[20] = { 0, };
        snprintf(content, sizeof(content), "DRAWING: %.5Lf", std::max(0.l, 1.l/fps - wating));
        lbl->setContent({content});
    };

    fpsChecker.setEvent(&show_fps);
    fpsRChecker.setEvent(&show_fpsr);
    watingChecker.setEvent(&show_wating);
    drawingChecker.setEvent(&show_drawing);

    sub_canvas.append(&debug_label);
    sub_canvas.append(&fpsChecker);
    sub_canvas.append(&fpsRChecker);
    sub_canvas.append(&watingChecker);
    sub_canvas.append(&drawingChecker);

    display->append(&lifegame);
    display->append(&sub_canvas);
    std::thread draw([&](){display->draw();});

    draw.join();
	return 0;
}