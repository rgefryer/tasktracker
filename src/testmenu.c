#include <pebble.h>
#include <testmenu.h>

static uint16_t menu_get_num_sections_callback(MenuLayer *me, void *data);
static uint16_t menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data);
static int16_t menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data);
static int16_t menu_get_cell_height_callback(MenuLayer *me, MenuIndex* cell_index, void *data);
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void menu_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
void start_submenu();

//#define NUM_ICONS 3
//#define ICON_THANKS 0
//#define ICON_TUBE 1
//#define ICON_BUS 2

//static GBitmap *icons[NUM_ICONS];

static is_menu_cb_t is_menu_callback;
static num_items_cb_t num_items_callback;
static item_text_cb_t item_text_callback;
static select_cb_t select_callback;
static char s_menu[20];  // Current menu ID, eg "/" or "/1/".
static bool s_selected;

#define MAX_MENU_DEPTH 10
static Window *s_window_array[MAX_MENU_DEPTH];
static MenuLayer *s_menulayer_1_array[MAX_MENU_DEPTH];
static TextLayer *s_textlayer_1_array[MAX_MENU_DEPTH];
static char s_menu_array[MAX_MENU_DEPTH][20];
static int s_menu_stack_pos = 0;  // Next slot to push


/*
  Deconstruct a menu ID into an array of integers.
*/
int *menu_parts(char *id) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_parts(%s)", id);
  static int parts[MAX_MENU_DEPTH];
  char part_str[5];

  int part = 0;
  while (*id != '\0') {
    if (*id != '/') {
      char *string_end = id + 1;
      int string_len = 1;
      while (*string_end != '\0' && *string_end != '/') {
        string_end++;
        string_len++;
      }
      strncpy(part_str, id, string_len);
      parts[part++] = atoi(part_str);
      id = string_end;
    }
    else
      id++;
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_parts: Returning %d part(s)", part);
  parts[part] = 0;
  return parts;
}

// Number of parts in a deconstructed menu ID
int menu_parts_count(int *parts)
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_parts_count");
  int count = 0;
  int part = 0;

  while (parts[part++] != 0) {
    count++;
  }

  return count;
}


/*
  Generate an item id by appending the number passed onto the
  string in s_prefix.

  Return a static buffer - this only remains valid until the
  next call to the function.
*/
char *create_item_id(int num)
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "create_item_id(%d)", num);
  static char buff[20] = {};

  num += 1;  // The menu numbering is 1-based

  APP_LOG(APP_LOG_LEVEL_DEBUG, "create_item_id: appending to %s", s_menu);
  snprintf(buff, 20, "%s%u", s_menu, num);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "create_item_id: returning %s", buff);
  return buff;
}

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer_1;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  // s_menulayer_1
  s_menulayer_1 = menu_layer_create(GRect(1, 21, 144, 146));
  menu_layer_set_click_config_onto_window(s_menulayer_1, s_window);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_menulayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 0, 143, 19));
  text_layer_set_background_color(s_textlayer_1, GColorBlack);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "Top");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_menulayer_1);
  text_layer_destroy(s_textlayer_1);
}
// END AUTO-GENERATED UI CODE

void push_menu() {
  if (s_menu_stack_pos == MAX_MENU_DEPTH) {
    return;
  }
  s_window_array[s_menu_stack_pos] = s_window;
  s_menulayer_1_array[s_menu_stack_pos] = s_menulayer_1;
  s_textlayer_1_array[s_menu_stack_pos] = s_textlayer_1;
  strcpy(s_menu_array[s_menu_stack_pos], s_menu);

  s_menu_stack_pos++;
}

void pop_menu() {
  if (s_menu_stack_pos == 0) {
    s_window = NULL;  // Flag that we've fully rewound
    return;
  }

  s_menu_stack_pos--;

  if (s_menu_stack_pos != 0) {
    s_window = s_window_array[s_menu_stack_pos-1];
    s_menulayer_1 = s_menulayer_1_array[s_menu_stack_pos-1];
    s_textlayer_1 = s_textlayer_1_array[s_menu_stack_pos-1];
    strcpy(s_menu, s_menu_array[s_menu_stack_pos-1]);
  }
  else {
    s_window = NULL;  // Flag that we've fully rewound
  }
}

static void handle_window_load(Window* me) {
  menu_layer_set_callbacks(s_menulayer_1, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .get_cell_height = menu_get_cell_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_click_callback
  });
  push_menu();

  //icons[ICON_THANKS] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_THANKS);
  //icons[ICON_TUBE] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_TUBE);
  //icons[ICON_BUS] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_BUS);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  //gbitmap_destroy(icons[ICON_THANKS]);
  //gbitmap_destroy(icons[ICON_BUS]);
  //gbitmap_destroy(icons[ICON_TUBE]);
  pop_menu();

  // If we've selected, made sure that all windows
  // are removed.
  if (s_selected && s_window) {
    hide_testmenu();
  }
}

uint16_t menu_get_num_sections_callback(MenuLayer *me, void *data) {
  return 1;
}

uint16_t menu_get_num_rows_callback(MenuLayer *me, uint16_t section_index, void *data) {
  return num_items_callback(s_menu);
}

int16_t menu_get_header_height_callback(MenuLayer *me, uint16_t section_index, void *data) {
  return 0;
}

int16_t menu_get_cell_height_callback(MenuLayer *me, MenuIndex* cell_index, void *data) {
  return 20;
}

void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Do nothing.
}

void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {

  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_draw_row_callback: row %d in menu %s", cell_index->row, s_menu);
  
  graphics_context_set_text_color(ctx, GColorWhite);
  //if (icon) {
    //graphics_draw_bitmap_in_rect(ctx, icon, GRect(4, 4, 24, 28));
  //}

  // Query the text to display, and prefix with ">" for a
  // submenu.
  char *item_id = create_item_id(cell_index->row);
  char *text = item_text_callback(item_id);
  char *prefix = (is_menu_callback(item_id)) ? "> ": "   ";
  char item[MAX_MENU_ITEM_LEN+4] = {};
  snprintf(item, MAX_MENU_ITEM_LEN+4, "%s%s", prefix, text);

  graphics_draw_text(ctx,
                     item,
                     fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
                     GRect(4, 2, 136, 18),
                     GTextOverflowModeFill,
                     GTextAlignmentLeft,
                     NULL);
}

void menu_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {

  char *item_id = create_item_id(cell_index->row);
  if (is_menu_callback(item_id)) {
    // User has selected a submenu.  Start up a new menu.
    char *new_name = create_item_id(cell_index->row);
    strcpy(s_menu, new_name);
    strcat(s_menu, "/");
    start_submenu();
  }
  else {
    // Just an item.  Tidy up the menu(s) and return it to the caller.
    s_selected = true;
    char *result = create_item_id(cell_index->row);
    hide_testmenu();
    select_callback(result);
  }

}

void start_submenu() {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = handle_window_load,
    .unload = handle_window_unload,
  });
  text_layer_set_text(s_textlayer_1, item_text_callback(s_menu));
  window_stack_push(s_window, true);
}


/*
  Return the next compononent in the menu spec, or zero if
  there isn't one.

  The pointer reference passed in is updated ready for the next call.
*/
int next_menu_part(char **spec) {
  if (*spec == NULL) {
    return 0;
  }

  if (**spec == '/') {
    (*spec) ++;
  }

  int val = 0;
  while ((**spec != '/') && (**spec != 0)) {
    val *= 10;
    val += **spec - '0';
    *spec += 1;
  }

  return val;
}

void show_testmenu(select_cb_t select_cb,
                   num_items_cb_t num_items_cb,
                   item_text_cb_t item_text_cb,
                   is_menu_cb_t is_menu_cb,
                   char *selection) {

  APP_LOG(APP_LOG_LEVEL_DEBUG, "show_testmenu");
  is_menu_callback = is_menu_cb;
  num_items_callback = num_items_cb;
  item_text_callback = item_text_cb;
  select_callback = select_cb;
  strcpy(s_menu, "/");
  s_selected = false;

  start_submenu();

  // While there is a selected item, we select and create menus
  char *buf_pos = selection;
  int index = next_menu_part(&buf_pos);
  while (index != 0) {
    MenuIndex mi = (MenuIndex){
      .section = 0,
      .row = index - 1
    };
    menu_layer_set_selected_index(s_menulayer_1, mi, MenuRowAlignCenter, false);
    int next_index = next_menu_part(&buf_pos);

    if (next_index != 0) {
      // Load submenu for this index
      char *item_id = create_item_id(index-1);
      if (is_menu_callback(item_id)) {
        char *new_name = create_item_id(index-1);
        strcpy(s_menu, new_name);
        strcpy(s_menu + strlen(new_name), "/");
        start_submenu();
      }
    }
    index = next_index;
  }
}

void hide_testmenu(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hide_testmenu");
  window_stack_remove(s_window, true);
}
