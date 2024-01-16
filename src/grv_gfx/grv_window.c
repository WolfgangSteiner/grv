#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv/grv_arr.h"
#include <SDL2/SDL.h>

grv_window_active_windows_arr_t grv_window_active_windows_arr = {0};

typedef struct {
    grv_window_t* window;
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* sdl_texture;
    SDL_Surface* sdl_surface;
} grv_window_sdl_t;


grv_window_t* grv_window_new(s32 width, s32 height, grv_str_t title) {
    grv_window_t* w = grv_alloc_zeros(sizeof(grv_window_t));
    w->width = width;
    w->height = height;
    w->title = title;
    w->should_close = false;
    grv_frame_buffer_init(&w->frame_buffer, FRAME_BUFFER_INDEXED, width, height);
    w->handle = grv_alloc_zeros(sizeof(grv_window_sdl_t));
    return w;
}

bool grv_window_show(grv_window_t* w) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Init(SDL_INIT_VIDEO);
    }

    SDL_Window* window = SDL_CreateWindow(
        grv_str_cstr(w->title),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        w->width, w->height, SDL_WINDOW_SHOWN);    
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w->width, w->height, 32, 0, 0, 0, 0); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    grv_window_sdl_t sdl_window = {
        .window = w,
        .sdl_window = window,
        .sdl_renderer = renderer,
        .sdl_texture = texture,
        .sdl_surface = surface,
    };

    w->handle = grv_alloc_zeros(sizeof(grv_window_sdl_t));
    memcpy(w->handle, &sdl_window, sizeof(grv_window_sdl_t));
    grv_arr_push(&grv_window_active_windows_arr, w);
    grv_window_present(w);
    return true;
}

void grv_window_present(grv_window_t* w) {
    grv_window_sdl_t* sdl_window = w->handle;
    SDL_Renderer* renderer = sdl_window->sdl_renderer;
    SDL_Texture* texture = sdl_window->sdl_texture;
    SDL_Surface* surface = sdl_window->sdl_surface;
    grv_frame_buffer_render(&w->frame_buffer, surface->pixels, surface->pitch);
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void grv_window_poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            for (s32 i = 0; i < grv_window_active_windows_arr.size; ++i) {
                grv_window_t* w = grv_window_active_windows_arr.arr[i];
                grv_window_sdl_t* sdl_window = w->handle;
                if(sdl_window->sdl_window == SDL_GetWindowFromID(event.window.windowID)) {
                    w->should_close = true;
                }
            }
        }
    }
}