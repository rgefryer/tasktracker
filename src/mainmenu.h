#include <pebble.h>

// Callback definitions for the menu to get
// information about the contents
typedef void (*new_task_cb_t)(uint8_t task);
typedef void (*pause_cb_t)(void);
typedef void (*nothing_selected_cb_t)(void);

// Show the main menu.
// If the selection is a task, pass its ID to task_cb
void show_task_menu(new_task_cb_t task_cb, pause_cb_t pause_cb, nothing_selected_cb_t nothing_selected_cb, bool paused);
