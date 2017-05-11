#ifndef __smallwin_h
#define __smallwin_h

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   HWND hwnd;
   HDC dc;
   HGLRC rc;
   int _backup_width;
   int _backup_height;
   int _backup_x;
   int _backup_y;
} window;

void (*smallwin_keydown_callback)(unsigned char);
void (*smallwin_keyup_callback)(unsigned char);

void smallwin_error_message(const char *message);
void smallwin_swap_buffers(window win);
void smallwin_close_window(window win);
int blocking_message_loop();
int non_blocking_message_loop();
void exit_smallwin();
int smallwin_win_width(window win);
int smallwin_win_height(window win);
void smallwin_go_fullscreen(window win);
void smallwin_go_windowed(window win);
void init_smallwin();
window smallwin_open_window(const char* title, int width, int height, int major, int minor);

#ifdef __cplusplus
}
#endif

#endif
