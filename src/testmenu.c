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
static char s_menu[MAX_ITEM_ID_LEN];  // Current menu ID, eg "/" or "/1/".
static bool s_selected;


static Window *s_window_array[MAX_MENU_DEPTH];
static MenuLayer *s_menulayer_1_array[MAX_MENU_DEPTH];
static TextLayer *s_textlayer_1_array[MAX_MENU_DEPTH];
static char s_menu_array[MAX_MENU_DEPTH][MAX_ITEM_ID_LEN];
static int s_menu_stack_pos = 0;  // Next slot to push



/*
  Deconstruct a menu ID into an array of integers.
*/
uint8_t menu_parts(char *id, uint8_t *parts) {
  char part_str[MAX_MENU_PART_DIGITS+1];

  uint8_t part = 0;
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

  parts[part] = 0;
  return part;
}


/*
  Generate an item id by appending the number passed onto the
  string in s_prefix.
*/
char *create_item_id_from_row(int num, char *buff)
{
  num += 1;  // The menu numbering is 1-based

  snprintf(buff, MAX_ITEM_ID_LEN, "%s%u", s_menu, num);

  return buff;
}

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer_1;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
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
  uint16_t rows = num_items_callback(s_menu);
  return rows;
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

  graphics_context_set_text_color(ctx, GColorWhite);
  //if (icon) {
    //graphics_draw_bitmap_in_rect(ctx, icon, GRect(4, 4, 24, 28));
  //}

  // Query the text to display, and prefix with ">" for a
  // submenu.
  char item_id[MAX_ITEM_ID_LEN];
  create_item_id_from_row(cell_index->row, item_id);
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
  char item_id[MAX_ITEM_ID_LEN];
  create_item_id_from_row(cell_index->row, item_id);
  if (is_menu_callback(item_id)) {
    // User has selected a submenu.  Start up a new menu.
    snprintf(s_menu, MAX_ITEM_ID_LEN, "%s/", item_id);
    start_submenu();
  }
  else {
    // Just an item.  Tidy up the menu(s) and return it to the caller.
    s_selected = true;
    hide_testmenu();
    select_callback(item_id);
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

void show_testmenu(select_cb_t select_cb,
                   num_items_cb_t num_items_cb,
                   item_text_cb_t item_text_cb,
                   is_menu_cb_t is_menu_cb,
                   char *selection) {

  is_menu_callback = is_menu_cb;
  num_items_callback = num_items_cb;
  item_text_callback = item_text_cb;
  select_callback = select_cb;
  strcpy(s_menu, "/");
  s_selected = false;

  // Initialise this menu
  start_submenu();

  // While there is a selected item, we select and create menus
  char *buf_pos = selection;
  uint8_t parts[MAX_MENU_DEPTH];
  uint8_t num_parts = menu_parts(selection, parts);
  uint8_t this_part = 0;
  for (this_part = 0; this_part < num_parts; this_part++) {

    // Select the item in this menu
    uint8_t this_row = parts[this_part] - 1;
    MenuIndex mi = (MenuIndex){
      .section = 0,
      .row = this_row
    };
    menu_layer_set_selected_index(s_menulayer_1, mi, MenuRowAlignCenter, false);

    // If there's another selection coming along, load the submenu
    if (parts[this_part + 1] != 0) {
      // Load submenu for this index
      char item_id[MAX_ITEM_ID_LEN];
      create_item_id_from_row(this_row, item_id);
      if (is_menu_callback(item_id)) {
        snprintf(s_menu, MAX_ITEM_ID_LEN, "%s/", item_id);
        start_submenu();
      }
    }
  }
}

void hide_testmenu(void) {
  window_stack_remove(s_window, true);
}
