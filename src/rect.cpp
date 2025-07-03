#include "rect.h"
#include <iostream>


//Az autó kirajzolását végző tagfüggvény.
void rect::draw(SDL_Renderer *renderer, SDL_Window *window, double angle, int r, int g, int b)
{
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_Rect rect = {x - w / 2, y - h / 2, w, h};
    SDL_Point center = {w / 2, h / 2};
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, &center, SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}