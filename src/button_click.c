#include <pebble.h>
#include <testmenu.h>
#include <data.h>

static Window *window;
static TextLayer *text_layer;

// Menu callbacks for task items.  Menu structure is:
//  /1    Recent
//  /2    Most recently used label
//  /3    Next most recently used label

// Call back for querying if a menu item is,
// in fact, a menu.
// Expect item names like /1 & /3/4
static bool task_cb_is_menu(char *name) {
  // All single item paths are menus.
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
  int *parts = menu_parts(name);
  int num_parts = menu_parts_count(parts);


  // Root menu contains the number of labels, plus
  // 1 (for the "most recent" menu).
  if (num_parts == 0)
  {
    return 1 + num_labels();
  }


  // Menu 1 is the most recent tasks.  This has a
  // fixed size of MAX_RECENT_TASKS, unless there
  // are not that many tasks defined.
  if (parts[0] == 1) {

    int task_count = num_tasks();
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
  int *parts = menu_parts(name);
  int num_parts = menu_parts_count(parts);

  // It's a menu if the ID ends with a '/'
  bool is_menu = (name[strlen(name)-1] == '/');

  if (num_parts == 0)
    return "Categories";

  if ((num_parts == 1) && (parts[0] == 1)) {
    return "Recent";
  }

  // Get the label ID
  uint8_t *labels = ordered_labels();
  uint8_t label_id = labels[parts[0]-2];

  if (num_parts == 1) {
    return label_name(label_id);
  }

  uint8_t *tasks = ordered_tasks(label_id);
  uint8_t task_id = tasks(parts[1] - 1);
  return task_name(label_id);
}

// Callback to handle a menu selection
// Expect item names like /1 & /3/4
static void task_cb_select(char *result) {
  // Display the selected menu item in the text layer
  text_layer_set_text(text_layer, result);
}

// Call back for querying if a menu item is,
// in fact, a menu.
// Expect item names like /1 & /3/4
static bool cb_is_menu(char *name) {
  if (strcmp(name, "/1") == 0) {
    return true;
  }
  if (strcmp(name, "/2") == 0) {
    return true;
  }
  if (strcmp(name, "/3") == 0) {
    return true;
  }
  if (strcmp(name, "/3/2") == 0) {
    return true;
  }

  return false;
}

// Call back for returning the number of
// items in a menu.
// Expect menu names like / & /3/
static uint16_t cb_num_items(char *name) {
  if (strcmp(name, "/1/") == 0) {
    return 5;
  }
  return 3;
}

// Call back for querying the text of a
// menu item, or menu.
// Expect item names like /1 & /3/4, and
// Menu names like / & /3/
static char *cb_item_text(char *name) {
  if (strcmp(name, "/1/") == 0) {
    return "/Recent";
  }
  if (strcmp(name, "/2/") == 0) {
    return "/Work";
  }
  if (strcmp(name, "/3/") == 0) {
    return "/Personal";
  }
  if (strcmp(name, "/") == 0) {
    return "Categories";
  }
  if (strcmp(name, "/1") == 0) {
    return "Recent";
  }
  if (strcmp(name, "/2") == 0) {
    return "Work";
  }
  if (strcmp(name, "/3") == 0) {
    return "Personal";
  }
  if (strcmp(name, "/1/1") == 0) {
    return "Task Recent One";
  }
  if (strcmp(name, "/1/2") == 0) {
    return "Task Recent Two";
  }
  if (strcmp(name, "/1/3") == 0) {
    return "Task Recent Theee";
  }
  if (strcmp(name, "/1/4") == 0) {
    return "Task Recent Four";
  }
  if (strcmp(name, "/1/5") == 0) {
    return "Task Recent Five";
  }
  if (strcmp(name, "/2/1") == 0) {
    return "Task Work One";
  }
  if (strcmp(name, "/2/2") == 0) {
    return "Task Work Two";
  }
  if (strcmp(name, "/2/3") == 0) {
    return "Task Work Three";
  }
  if (strcmp(name, "/3/1") == 0) {
    return "Task Personal One";
  }
  if (strcmp(name, "/3/2") == 0) {
    return "Task Personal Two";
  }
  if (strcmp(name, "/3/3") == 0) {
    return "Task Personal Threee";
  }
  if (strcmp(name, "/3/2/1") == 0) {
    return "Task Personal /3/2/1";
  }
  if (strcmp(name, "/3/2/2") == 0) {
    return "Task Personal /3/2/2";
  }
  if (strcmp(name, "/3/2/3") == 0) {
    return "Task Personal /3/2/3";
  }
  return "Unknown";
}

// Callback to handle a menu selection
// Expect item names like /1 & /3/4
static void cb_select(char *result) {
  // Display the selected menu item in the text layer
  text_layer_set_text(text_layer, result);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  show_testmenu(task_cb_select,
                task_cb_num_items,
                task_cb_item_text,
                task_cb_is_menu,
                "/1/1");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  data_load();
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  data_save();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}