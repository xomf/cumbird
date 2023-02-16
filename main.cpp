#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 30;
const int PIPE_GAP = 8;

class Bird {
public:
  int x, y;
  int vy;

  Bird(int initX, int initY) {
    x = initX;
    y = initY;
    vy = 0;
  }

  void update() {
    vy += 1;
    y += vy;
  }

  void flap() { vy = -4; }

  void draw() { mvprintw(y, x, "8==>"); }
};

class Pipe {
public:
  int x, y_top, y_bottom;

  Pipe(int initX, int gapHeight) {
    x = initX;
    y_top = 0;
    y_bottom = gapHeight + PIPE_GAP;
  }

  void move() { x -= 1; }

  void draw() {
    for (int i = 0; i < y_top; i++) {
      mvprintw(i, x, "|");
    }
    for (int i = y_bottom; i < SCREEN_HEIGHT; i++) {
      mvprintw(i, x, "|");
    }
  }

  bool check_collision(int bird_x, int bird_y) {
    if (bird_x >= x && bird_x <= x + 1) {
      if (bird_y <= y_top || bird_y >= y_bottom) {
        return true;
      }
    }
    return false;
  }
};

int main() {
  srand(time(NULL));

  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);

  Bird bird(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
  Pipe pipe(SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - PIPE_GAP));

  int score = 0;

  while (true) {
    clear();

    if (pipe.x < -1) {
      pipe = Pipe(SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - PIPE_GAP));
      score++;
    }

    pipe.move();
    pipe.draw();

    bird.update();
    bird.draw();

    mvprintw(SCREEN_HEIGHT + 1, 0, "Score: %d", score);

    if (pipe.check_collision(bird.x, bird.y) || bird.y >= SCREEN_HEIGHT) {
      mvprintw(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, "Game Over");
      break;
    }

    refresh();
    usleep(100000);

    int ch = getch();
    if (ch == ' ') {
      bird.flap();
    }
    
  }

  getch();
  endwin();

  std::cout << "Oopsie doopsie! you made a wittle fucky wucky!! a wittle fucko "
               "boingo!"
            << std::endl;
  std::cout << "Final score: " << score << std::endl;

  if (score < 10) {

    std::cout << "Live Tucker Reaction: Cringe" << std::endl;

  } else {

    std::cout << "Live Tucker Reaction: Based" << std::endl;
  }

  return 0;
}
