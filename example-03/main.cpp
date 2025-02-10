#include <SDL3/SDL.h>
#include "qrcodegen.hpp"
#include "qrcodegen.hpp"

#include <vector>
#include <iostream>

const int PIXEL_SIZE = 10;
const int PADDING = 4 * PIXEL_SIZE;

using namespace qrcodegen;

void renderQRCode(QrCode &qr) {
  int size = qr.getSize();
  int windowSize = size * PIXEL_SIZE + 2 * PADDING;

  if(SDL_Init(SDL_INIT_VIDEO) == false) {
    std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << "\n";
    return;
  }

  SDL_Window* window = SDL_CreateWindow("QR code generator",
    windowSize,
    windowSize,
    0
  );

  if(!window) {
    std::cerr << "Window could not be created!" << SDL_GetError() << "\n";
    SDL_Quit();
    return;
  }

  auto renderer = SDL_CreateRenderer(window, NULL);
  
  if(!renderer) {
    std::cerr << "Renderer could not be created!" << SDL_GetError() << "\n";
    SDL_Quit();
    return;
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      if (qr.getModule(x, y)) { 
        SDL_FRect rect = {static_cast<float>(PADDING + x * PIXEL_SIZE),
                          static_cast<float>(PADDING + y * PIXEL_SIZE),
                          PIXEL_SIZE, PIXEL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
 
  SDL_RenderPresent(renderer);

  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <text-to-encode>\n";
  }

  std::string text = argv[1];

  QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);

  renderQRCode(qr);

  return 0;
}