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
} grv_window_impl_t;


grv_window_t* grv_window_new(s32 width, s32 height, f32 scale, grv_str_t title) {
    grv_window_t* w = grv_alloc_zeros(sizeof(grv_window_t));
    w->width = width;
    w->height = height;
    w->scale = scale;
    w->title = title;
    w->should_close = false;
    w->borderless = false;
    grv_frame_buffer_init(&w->frame_buffer, FRAME_BUFFER_INDEXED, width, height);
    w->handle = grv_alloc_zeros(sizeof(grv_window_impl_t));
    return w;
}

bool grv_window_show(grv_window_t* w) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Init(SDL_INIT_VIDEO);
    }

    s32 window_width = (s32)(w->width * w->scale);
    s32 window_height = (s32)(w->height * w->scale);

    int flags = SDL_WINDOW_SHOWN;
    if (w->borderless) {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    SDL_Window* window = SDL_CreateWindow(
        grv_str_cstr(w->title),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height, flags);    
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w->width, w->height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x0ff000000);
    assert(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture);
    u32 format;
    SDL_QueryTexture(texture, &format, NULL, NULL, NULL);
    printf("format: %s\n", SDL_GetPixelFormatName(format));
    grv_window_impl_t impl = {
        .window = w,
        .sdl_window = window,   
        .sdl_renderer = renderer,
        .sdl_texture = texture,
        .sdl_surface = surface,
    };

    w->handle = grv_alloc_zeros(sizeof(grv_window_impl_t));
    memcpy(w->handle, &impl, sizeof(grv_window_impl_t));
    grv_arr_push(&grv_window_active_windows_arr, w);
    grv_window_present(w);
    return true;
}

void grv_window_present(grv_window_t* w) {
    grv_window_impl_t* impl = w->handle;
    SDL_Renderer* renderer = impl->sdl_renderer;
    SDL_Texture* texture = impl->sdl_texture;
    SDL_Surface* surface = impl->sdl_surface;
    grv_frame_buffer_render_argb(&w->frame_buffer, surface->pixels, surface->pitch);
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_Rect src_rect = {0, 0, w->width, w->height};
    SDL_Rect dst_rect = {0, 0, w->width * w->scale, w->height * w->scale};
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
    SDL_RenderPresent(renderer);
}

void grv_window_poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            for (s32 i = 0; i < grv_window_active_windows_arr.size; ++i) {
                grv_window_t* w = grv_window_active_windows_arr.arr[i];
                grv_window_impl_t* impl = w->handle;
                if(impl->sdl_window == SDL_GetWindowFromID(event.window.windowID)) {
                    w->should_close = true;
                }
            }
        }
    }
}

void grv_sleep(f32 seconds) {
    SDL_Delay((u32)(seconds * 1000));
}

void grv_window_set_opacity(grv_window_t* w, f32 opacity) {
    grv_window_impl_t* impl = w->handle;
    SDL_SetWindowOpacity(impl->sdl_window, opacity);
}