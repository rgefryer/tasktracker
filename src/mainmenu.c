#include <pebble.h>
#include <menunest.h>
#include <data.h>
#include <mainmenu.h>

// Callback for when a task is selected from the menu
static new_task_cb_t task_callback;

// Callback for when pause is selected from the menu
static pause_cb_t pause_callback;

// Callback for when nothing is selected from the menu
static nothing_selected_cb_t nothing_selected_callback;

// Flag that we are currently paused
bool g_menu_paused;

// Menu callbacks for task items.  Menu structure is:
//  /1    Recent
//  /2    Most recently used label
//  /3    Next most recently used label

// Call back for querying if a menu item is,
// in fact, a menu.
// Expect item names like /1 & /3/4
static bool task_cb_is_menu(char *name) {
  // All single item paths are menus.
  APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_is_menu");
  if (strlen(name) <= 3)
    return true;
  else
    return false;
}

// Call back for returning the number of
// items in a menu.
// Expect menu names like / & /3/
static uint16_t task_cb_num_items(char *name) {

  // Deconstruct the menu ID
  APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_num_items(%s)", name);
  uint8_t parts[MAX_MENU_DEPTH];
  int num_parts = menu_parts(name, parts);

  // Root menu contains the number of labels, plus
  // 1 (for the "most recent" menu).
  if (num_parts == 0)
  {
    return 1 + num_labels();
  }

  // Menu 1 is the most recent tasks.  This has a
  // fixed size of MAX_RECENT_TASKS, unless there
  // are not that many tasks defined.
  APP_LOG(APP_LOG_LEVEL_DEBUG, "1st menu part is %d", parts[0]);
  if (parts[0] == 1) {

    uint8_t *tasks = ordered_tasks(0);
    int task_count = num_ids(tasks);
    if (!g_menu_paused) {
      task_count++;  // Add 1 for "Pause"
    }
    if (task_count < MAX_RECENT_TASKS)
      return task_count;
    else
      return MAX_RECENT_TASKS;
  }

  // We're onto a menu for a label.  We need to find which
  // label, then find the number of tasks with that label.
  uint8_t *labels = ordered_labels();
  uint8_t label_id = labels[parts[0]-2];
  uint8_t *tasks = ordered_tasks(label_id);
  return num_ids(tasks);
}

// Call back for querying the text of a
// menu item, or menu.
// Expect item names like /1 & /3/4, and
// Menu names like / & /3/
static char *task_cb_item_text(char *name) {

  // Deconstruct the menu ID
  APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text(%s)", name);
  uint8_t parts[MAX_MENU_DEPTH];
  int num_parts = menu_parts(name, parts);
  char *rc = NULL;

  // It's a menu if the ID ends with a '/'
  bool is_menu = (name[strlen(name)-1] == '/');

  if (num_parts == 0) {
    rc = "Categories";
  }

  else if ((num_parts == 1) && (parts[0] == 1)) {
    rc = "Recent";
  }

  else {
    // Get the label ID
    uint8_t label_id = 0;
    if (parts[0] != 1) {
      uint8_t *labels = ordered_labels();
      label_id = labels[parts[0]-2];
      APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text: Translate menu %d to label ID %d", parts[0], label_id);

      // If we're looking for a label name, set it up
      if (num_parts == 1) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text: Get label name for label ID %d", label_id);
        rc = label_name(label_id);
      }
    }

    // Handle the "Pause" at the top of the Recents menu
    uint8_t task_ix = parts[1] - 1; // Because menu-IDs are 1-based
    if (!g_menu_paused && (rc == NULL)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text: Get task name");
      if (parts[0] == 1)
      {
        // Recents menu - ignore the pause option
        if (task_ix == 0) {
          rc = "Pause";
        }
        else {
          task_ix -= 1;  // To make space for the pause option
        }
      }
    }

    // Get the task name from the ordered tasks
    if (rc == NULL) {
      uint8_t *tasks = ordered_tasks(label_id);
      uint8_t task_id = tasks[task_ix];
      rc = task_name(task_id);
    }
  }

  if (rc == NULL) {
    rc = "Unknown";
  }

  if (strlen(rc) >= MAX_MENU_ITEM_LEN)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text: dumping %s: too long", rc);
    rc = "Too long";
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_item_text: return %s", rc);
  return rc;
}

// Callback to handle a menu selection
// Expect item names like /1 & /3/4
static void task_cb_select(char *result) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "task_cb_select");

  // If result is NULL, nothing was selected
  if (result == NULL) {
    nothing_selected_callback();
    return;
  }

  uint8_t task_id = 0;

  // Deconstruct the menu ID
  uint8_t parts[MAX_MENU_DEPTH];
  int num_parts = menu_parts(result, parts);

  // If there's two parts to the menu ID, we must have a task.
  if (num_parts == 2) {
    // Get the label ID
    uint8_t label_id = 0;
    if (parts[0] >= 2) {
      // Get the label ID, unless this is the "Recents" submenu.
      uint8_t *labels = ordered_labels();
      label_id = labels[parts[0]-2];
    }

    // Special case entry 1 in the recents menu, which is pause
    if (!g_menu_paused && (parts[0] == 1)) {
      if (parts[1] == 1) {
        pause_callback();
        return;
      }
      parts[1]--;
    }

    // Get the task ID.
    uint8_t *tasks = ordered_tasks(label_id);
    task_id = tasks[parts[1] - 1];
  }

  if (task_id != 0) {
    task_callback(task_id);
  }
}

void show_task_menu(new_task_cb_t task_cb, pause_cb_t pause_cb, nothing_selected_cb_t nothing_selected_cb, bool paused) {
  task_callback = task_cb;
  pause_callback = pause_cb;
  nothing_selected_callback = nothing_selected_cb;
  g_menu_paused = paused;

  show_menunest(task_cb_select,
                task_cb_num_items,
                task_cb_item_text,
                task_cb_is_menu,
                "/1/1");
}

