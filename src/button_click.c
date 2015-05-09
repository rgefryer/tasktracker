#include <pebble.h>
#include <testmenu.h>
#include <data.h>

static Window *window;
static TextLayer *text_layer;

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
  text_layer_set_text(text_layer, result);    
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  show_testmenu(cb_select, cb_num_items, cb_item_text, cb_is_menu, "/1/3");
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