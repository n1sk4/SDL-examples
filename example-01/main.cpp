#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

enum Direction {
  DOWN,
  LEFT,
  RIGHT,
  UP
};

#define SCREEN_SIZE 1000
#define SNAKE_SIZE 10.f
#define START_POS_X 500.f
#define START_POS_Y 500.f

int main() {
  SDL_Init(SDL_INIT_VIDEO 
          | SDL_INIT_EVENTS);
  auto window = SDL_CreateWindow("Snake",
    SCREEN_SIZE,
    SCREEN_SIZE,
    0
  );

  auto renderer = SDL_CreateRenderer(window, NULL);
  SDL_Event event;

  bool running = true;
  int dir = 0;
  
  SDL_FRect head = {START_POS_X, START_POS_Y, SNAKE_SIZE, SNAKE_SIZE};
  std::deque<SDL_FRect> rq;
  float size = 1.f;

  std::vector<SDL_FRect> apples;
  for(int i = 0; i < 100; i++) {
    apples.emplace_back(static_cast<float>(rand()%100*SNAKE_SIZE), 
                        static_cast<float>(rand()%100*SNAKE_SIZE), 
                        SNAKE_SIZE, SNAKE_SIZE);
  }

  while(running) {
    while (SDL_PollEvent(&event)) {
      if(event.type == SDL_EVENT_QUIT) {
        running = false;
      }
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if(event.key.key == SDLK_DOWN)  { dir = DOWN;  }
        if(event.key.key == SDLK_UP)    { dir = UP;    }
        if(event.key.key == SDLK_LEFT)  { dir = LEFT;  }
        if(event.key.key == SDLK_RIGHT) { dir = RIGHT; }
      }
    }

    // Move snake
    switch(dir) {
      case DOWN:  head.y += SNAKE_SIZE; break;
      case UP:    head.y -= SNAKE_SIZE; break;
      case LEFT:  head.x -= SNAKE_SIZE; break;
      case RIGHT: head.x += SNAKE_SIZE; break;
    }

    // Collision detection with walls
    if (head.x < 0 || head.x >= SCREEN_SIZE || head.y < 0 || head.y >= SCREEN_SIZE) {
      head = {START_POS_X, START_POS_Y, SNAKE_SIZE, SNAKE_SIZE};
      rq.clear();
      size = 1.f;
    }

    // Flag for body collision detection
    bool collidedWithBody = false;
    
    // Collision detection with body
    for(auto& snakeSegment : rq) {
      if(head.x == snakeSegment.x && head.y == snakeSegment.y) {
        collidedWithBody = true;
        break;
      }
    }

    if (collidedWithBody) {
      head = {START_POS_X, START_POS_Y, SNAKE_SIZE, SNAKE_SIZE};
      rq.clear();
      size = 1.f;
    }

    // Collision detection with apples
    std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
      if(head.x == apple.x && head.y == apple.y) {
        size    += SNAKE_SIZE;
        apple.x = -static_cast<float>(SNAKE_SIZE);
        apple.y = -static_cast<float>(SNAKE_SIZE);
      }
    });

    rq.push_front(head);
    while(rq.size() > size) {
      rq.pop_back();
    }

    // Clear window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw body
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::for_each(rq.begin(), rq.end(), [&](auto& snakeSegment) {
      SDL_RenderFillRect(renderer, &snakeSegment);
    });

    // Draw apples
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
      SDL_RenderFillRect(renderer, &apple);
    });

    SDL_RenderPresent(renderer);
    SDL_Delay(50);
  }
}
