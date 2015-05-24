
#define MAX_MENU_ITEM_LEN 30

// Callback definitions for the menu to get
// information about the contents
typedef bool (*is_menu_cb_t)(char *name);
typedef uint16_t (*num_items_cb_t)(char *name);
typedef char * (*item_text_cb_t)(char *name);
typedef void (*select_cb_t)(char *result);

// Return an array of parts from a decoded menu ID
int *menu_parts(char *id);
int menu_parts_count(int *parts);

// Show the menu
void show_testmenu(select_cb_t select_cb,
                   num_items_cb_t num_items_cb,
                   item_text_cb_t item_text_cb,
                   is_menu_cb_t is_menu_cb,
                   char *selection);

// Hide the menu
void hide_testmenu(void);

