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


grv_window_t* grv_window_new(i32 width, i32 height, f32 scale, grv_str_t title) {
    grv_window_t* w = grv_alloc_zeros(sizeof(grv_window_t));
    w->width = width;
    w->height = height;
    w->scale = scale;
    w->title = title;
    w->should_close = false;
    w->borderless = false;
    w->use_int_scaling = true;
    grv_frame_buffer_init(&w->frame_buffer, FRAME_BUFFER_INDEXED, width, height);
    w->handle = grv_alloc_zeros(sizeof(grv_window_impl_t));
    return w;
}

bool grv_window_show(grv_window_t* w) {
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Init(SDL_INIT_VIDEO);
    }

    i32 window_width = (i32)(w->width * w->scale);
    i32 window_height = (i32)(w->height * w->scale);

    int flags = SDL_WINDOW_SHOWN;
    if (w->borderless) {
        flags |= SDL_WINDOW_BORDERLESS;
    } else {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    SDL_Window* window = SDL_CreateWindow(
        grv_str_cstr(w->title),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height, flags);

    SDL_SetWindowData(window, "grv_window", w);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w->width, w->height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x0ff000000);
    assert(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture);
    u32 format;
    SDL_QueryTexture(texture, &format, NULL, NULL, NULL);
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
    
    SDL_SetWindowMinimumSize(window, w->width, w->height);

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect src_rect = {0, 0, surface->w, surface->h};
    int window_width, window_height;
    SDL_GetWindowSize(impl->sdl_window, &window_width, &window_height);
    f32 scale_x = (f32)window_width / (f32)w->width; 
    f32 scale_y = (f32)window_height / (f32)w->height;
    f32 scalef = grv_min_f32(scale_x, scale_y);
    
    i32 dst_width, dst_height;
    if (!w->use_int_scaling || scalef < 1.0f) {
        dst_width = (i32)(w->width * scalef);
        dst_height = (i32)(w->height * scalef);
    } else {
        i32 scale = grv_floor_f32(scalef);
        dst_width = w->width * scale;
        dst_height = w->height * scale;
    }
    i32 dst_x = (window_width - dst_width) / 2;
    i32 dst_y = (window_height - dst_height) / 2;
    SDL_Rect dst_rect = {dst_x, dst_y, dst_width, dst_height};
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
    SDL_RenderPresent(renderer);
}

void grv_window_poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            for (i32 i = 0; i < grv_window_active_windows_arr.size; ++i) {
                grv_window_t* w = grv_window_active_windows_arr.arr[i];
                grv_window_impl_t* impl = w->handle;
                if(impl->sdl_window == SDL_GetWindowFromID(event.window.windowID)) {
                    w->should_close = true;
                }
            }
        } else if (event.type == SDL_WINDOWEVENT) {
            SDL_Window* sdl_window = SDL_GetWindowFromID(event.window.windowID);
            grv_window_t* grv_window = SDL_GetWindowData(sdl_window, "grv_window");
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                grv_window_present(grv_window);
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
