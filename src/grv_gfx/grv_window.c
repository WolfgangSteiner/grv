#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv/grv_arr.h"
#include <GLFW/glfw3.h>


grv_window_active_windows_arr_t grv_window_active_windows_arr = {0};

static void _grv_window_error_callback(int error, const char* description) {
    grv_log_error("GLFW error %d: %s", error, description);
}

static void _grv_window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

grv_window_t* grv_window_new(s32 width, s32 height, grv_str_t title) {
    grv_window_t* w = grv_alloc_zeros(sizeof(grv_window_t));
    w->width = width;
    w->height = height;
    w->title = title;
    w->should_close = false;
    return w;
}

bool grv_window_show(grv_window_t* w) {
    glfwSetErrorCallback(_grv_window_error_callback);
    if (!glfwInit()) {
        grv_log_error("Failed to initialize GLFW");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(w->width, w->height, grv_str_cstr(w->title), NULL, NULL);
    if (!window) {
        grv_log_error("Failed to create GLFW window");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    
    // clear the window to black:
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwSetKeyCallback(window, _grv_window_key_callback);
    grv_arr_push(&grv_window_active_windows_arr, w);
    w->handle = window;
    return true;
}

void grv_window_poll_events() {
    glfwPollEvents();
    for (s32 i = 0; i < grv_window_active_windows_arr.size; i++) {
        grv_window_t* w = grv_window_active_windows_arr.arr[i];
        if (glfwWindowShouldClose(w->handle)) {
            w->should_close = true;
        }
    }
}