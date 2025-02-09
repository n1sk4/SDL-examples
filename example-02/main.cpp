#include <SDL3/SDL.h>
#include <random>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <array>

const int WIDTH = 640;
const int HEIGHT = 480;

class SDLHelper {
public:
  SDLHelper() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("Game of life", WIDTH, HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer);
    SDL_SetRenderScale(renderer, 4, 4);
  }

  void drawpixel(double xm, double ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) {
    points.emplace_back(static_cast<float>(xm), static_cast<float>(ym));
    colors.emplace_back(r, g, b, a);
  }
  
  void clearpixels() {
    points.clear();
  }

  void update() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (long unsigned int i = 0; i < points.size(); i++) {
      SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
      SDL_RenderPoint(renderer, points[i].x, points[i].y);
    }

    SDL_RenderPresent(renderer);
  }

  void input() {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        SDL_Quit();
        exit(0);
        break;

      default:
        break;
      }
    }
  }

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool done;
  std::vector<SDL_FPoint> points;
  std::vector<SDL_Color> colors;
  SDL_Event event;
};

bool isAlive (std::vector<std::vector<bool>> &game,
              const int x, 
              const int y) {
  int alive = 0;
  
  // test the left
  if(x > 0 && game[x - 1][y] == 1)      alive++;
  // test the right
  if(x < WIDTH && game[x + 1][y] == 1)  alive++;
  // test top
  if(y > 0 && game[x][y - 1] == 1)      alive++;
  // test bottom
  if(y < HEIGHT && game[x][y + 1])      alive++;

  // test top left
  if(y > 0 && x > 0 && game[x - 1][y - 1] == 1)          alive++;
  // test top right
  if(y > 0 && x < WIDTH && game[x + 1][y - 1] == 1)      alive++;
  // test bottom left
  if(y < HEIGHT && x < 0 && game[x - 1][y + 1] == 1)     alive++;
  // test bottom right
  if(y < HEIGHT && x < WIDTH && game[x + 1][y + 1] == 1) alive++;

  // Game rules:
  // alive and fewer than 2 -> DEAD
  if(game[x][y] == 1 && alive < 2)                return false;
  // alive and 2 or 3 -> ALIVE
  if(game[x][y] == 1 && alive == 2 || alive == 3) return true;
  // more than 3 -> DEAD
  if(alive > 3)                                   return false;
  // 3 alive and point is dead -> ALIVE
  if(game[x][y] == 0 && alive == 3)               return true;

  // any other case -> DEAD
  return false;
}

int main() {
  SDLHelper h;
  std::vector<std::vector<bool>> display(WIDTH, std::vector<bool>(HEIGHT, 0));
  std::vector<std::vector<bool>> swap(WIDTH, std::vector<bool>(HEIGHT, 0));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist (0, 1);

  for (int x = 0; x < WIDTH; ++x) {
    for (int y = 0; y < HEIGHT; ++y) {
        display[x][y] = dist(gen) == 1;
    }
  }
  
  while(true) {
    for(int i = 0; i < WIDTH - 1; ++i) {
      for (int j = 0; j < HEIGHT - 1; ++j) {
        swap[i][j] = isAlive(display, i, j) ? 1 : 0;
      }
    }

    for(int i = 0; i < WIDTH - 1; ++i) {
      for (int j = 0; j < HEIGHT - 1; ++j) {
        if(swap[i][j]) {
          h.drawpixel(i, j);
        }
      }
    }

    display = swap;

    h.update();
    SDL_Delay(50);
    h.input();
    h.clearpixels();
  }
}