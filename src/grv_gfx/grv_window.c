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
    w->resizable = true;
    w->use_int_scaling = true;
    grv_framebuffer_init(&w->framebuffer, GRV_FRAMEBUFFER_INDEXED, width, height);
    w->handle = grv_alloc_zeros(sizeof(grv_window_impl_t));
    return w;
}

SDL_Rect _grv_window_align(grv_window_t* w) {
    SDL_Rect window_rect = {.w = (int)(w->width * w->scale), .h = (int)(w->height * w->scale) };
    SDL_Rect screen_rect;
    int result = SDL_GetDisplayBounds(0, &screen_rect);
    assert(result == 0);
    if (w->horizontal_align == GRV_WINDOW_HORIZONTAL_ALIGN_CENTER) {
        window_rect.x = SDL_WINDOWPOS_CENTERED;
    } else if (w->horizontal_align == GRV_WINDOW_HORIZONTAL_ALIGN_LEFT) {
        window_rect.x = screen_rect.x;
    } else if (w->horizontal_align == GRV_WINDOW_HORIZONTAL_ALIGN_RIGHT) {
        window_rect.x = screen_rect.x + screen_rect.w - window_rect.w;
    } else if (w->horizontal_align == GRV_WINDOW_HORIZONTAL_ALIGN_NONE) {
        window_rect.x = SDL_WINDOWPOS_UNDEFINED;
    }

    if (w->vertical_align == GRV_WINDOW_VERTICAL_ALIGN_CENTER) {
        window_rect.y = SDL_WINDOWPOS_CENTERED;
    } else if (w->vertical_align == GRV_WINDOW_VERTICAL_ALIGN_TOP) {
        window_rect.y = screen_rect.y;
    } else if (w->vertical_align == GRV_WINDOW_VERTICAL_ALIGN_BOTTOM) {
        window_rect.y = screen_rect.y + screen_rect.h - window_rect.h;
    } else if (w->vertical_align == GRV_WINDOW_VERTICAL_ALIGN_NONE) {
        window_rect.y = SDL_WINDOWPOS_UNDEFINED;
    }

    return window_rect;
}

void grv_window_resize(grv_window_t* w) {
    grv_window_impl_t* impl = w->handle;
    int window_width, window_height;
    SDL_GetWindowSize(impl->sdl_window, &window_width, &window_height);
    f32 scale_x = (f32)window_width / (f32)w->width; 
    f32 scale_y = (f32)window_height / (f32)w->height;
    f32 scalef = grv_min_f32(scale_x, scale_y);
    
    i32 dst_width, dst_height;
    if (!w->use_int_scaling || scalef < 1.0f) {
        dst_width = (i32)(w->width * scalef);
        dst_height = (i32)(w->height * scalef);
		w->view_scale = scalef;
    } else {
        i32 scale = grv_floor_f32(scalef);
        dst_width = w->width * scale;
        dst_height = w->height * scale;
		w->view_scale = scale;
    }
    i32 dst_x = (window_width - dst_width) / 2;
    i32 dst_y = (window_height - dst_height) / 2;

    w->view_rect = (rect_i32){dst_x, dst_y, dst_width, dst_height};
}

vec2_f32 grv_window_to_view_position(grv_window_t* w, vec2_i32 pos) {
	vec2_i32 view_pos = rect_i32_pos(w->view_rect);
	f32 vx = grv_clamp_f32((pos.x - view_pos.x) / w->view_scale, 0, w->framebuffer.width - 1);
	f32 vy = grv_clamp_f32((pos.y - view_pos.y) / w->view_scale, 0, w->framebuffer.height -1);
	return (vec2f){vx,vy};
}

void grv_window_update_mouse_pos(grv_window_t* w, i32 x, i32 y) {
	vec2_i32 pos = (vec2_i32){x,y};
	w->mouse_window_pos = pos;
	w->mouse_view_pos = grv_window_to_view_position(w, pos);
}

bool grv_window_show(grv_window_t* w) { 
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Init(SDL_INIT_VIDEO);
    }
    if (!SDL_WasInit(SDL_INIT_TIMER)) {
        SDL_Init(SDL_INIT_TIMER);
    }

    int flags = SDL_WINDOW_SHOWN;
    if (w->borderless) {
        flags |= SDL_WINDOW_BORDERLESS;
    }
    if (w->resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    SDL_Rect window_rect = _grv_window_align(w);

    SDL_Window* window = SDL_CreateWindow(
        grv_str_cstr(w->title),
        window_rect.x, window_rect.y,
        window_rect.w, window_rect.h, flags);

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

	grv_window_resize(w);
    grv_window_present(w);
    return true;
}

void grv_window_present(grv_window_t* w) {
    grv_window_impl_t* impl = w->handle;
    SDL_Renderer* renderer = impl->sdl_renderer;
    SDL_Texture* texture = impl->sdl_texture;
    SDL_Surface* surface = impl->sdl_surface;
    grv_framebuffer_render_argb(&w->framebuffer, surface->pixels, surface->pitch);
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect src_rect = {0, 0, surface->w, surface->h};
	SDL_Rect dst_rect = {w->view_rect.x, w->view_rect.y, w->view_rect.w, w->view_rect.h};
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
    SDL_RenderPresent(renderer);

	// update mouse button state
	for (i32 i=0; i < GRV_WINDOW_MAX_MOUSE_BUTTON_ID; i++) {
		grv_mouse_button_info_t* button_info = &w->mouse_button_info[i];
		button_info->was_down = button_info->is_down;
	}
}

void grv_window_handle_mouse_motion(SDL_Event* event) {
	SDL_Window* sdl_window = SDL_GetWindowFromID(event->motion.windowID);
	grv_window_t* grv_window = SDL_GetWindowData(sdl_window, "grv_window");
	grv_window_update_mouse_pos(grv_window, event->motion.x, event->motion.y);
	f32 min_drag_distance = 1.0f;
	grv_mouse_button_info_t* button_info = &grv_window->mouse_button_info[1];
	
	if (!grv_window->is_in_drag 
		&& button_info->is_down
		&& button_info->click_count == 0) {
		f32 dist = vec2f_dist(
			grv_window->mouse_view_pos,
			grv_window->mouse_button_info[1].initial_view_pos);
		if (dist > min_drag_distance && !grv_window->is_in_drag) {
			grv_window->is_in_drag = true;
			grv_window->mouse_drag_initial_window_pos = button_info->initial_window_pos;
			grv_window->mouse_drag_initial_view_pos = button_info->initial_view_pos;
		}
	}
}

void grv_window_handle_mouse_button(SDL_Event* event) {
	SDL_Window* sdl_window = SDL_GetWindowFromID(event->motion.windowID);
	grv_window_t* grv_window = SDL_GetWindowData(sdl_window, "grv_window");
	i32 button_id = event->button.button;
	u32 type = event->button.type;
	vec2_i32 window_pos = {event->button.x, event->button.y};

	if (button_id >= GRV_WINDOW_MAX_MOUSE_BUTTON_ID) return;
	grv_mouse_button_info_t* button_info = &grv_window->mouse_button_info[button_id];
	button_info->is_down = (type == SDL_MOUSEBUTTONDOWN);
	if (button_info->is_down && button_info->click_count == 0) {
		button_info->was_down = false;
		button_info->initial_window_pos = window_pos;
		button_info->initial_view_pos = grv_window_to_view_position(grv_window, window_pos);
	} else if (grv_window->is_in_drag && type == SDL_MOUSEBUTTONUP) {
		grv_window->is_in_drag = false;
	}
}

void grv_window_poll_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            for (size_t i = 0; i < grv_window_active_windows_arr.size; ++i) {
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
				grv_window_resize(grv_window);
                grv_window_present(grv_window);
            } 
        } else if (event.type == SDL_MOUSEMOTION) {
			grv_window_handle_mouse_motion(&event);
		} else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
			grv_window_handle_mouse_button(&event);
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
