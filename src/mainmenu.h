#include <pebble.h>

// Callback definitions for the menu to get
// information about the contents
typedef void (*new_task_cb_t)(uint8_t task);

// Show the main menu.
// If the selection is a task, pass its ID to task_cb
void show_task_menu(new_task_cb_t task_cb);
