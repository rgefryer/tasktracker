
// Max characters (including terminating \0) in a menu item
#define MAX_MENU_ITEM_LEN 30

// Max depth of menu structure.  1 means only the root menu
#define MAX_MENU_DEPTH 10

// Max number of items in a single menu
// Must be kept in line with MAX_MENU_DIGIT_PARTS
#define MAX_SUBMENU_LEN 250

// Max digits used to represent a menu item within a menu.
// Must be kept in line with MAX_SUBMENU_LEN
#define MAX_MENU_PART_DIGITS 3

// Max characters (including terminating \0) in a menu id
// eg "/250" is 4, "/250/250/250" is 13
#define MAX_ITEM_ID_LEN (1 + MAX_MENU_DEPTH * (MAX_MENU_PART_DIGITS + 1))

// Callback definitions for the menu to get
// information about the contents
typedef bool (*is_menu_cb_t)(char *name);
typedef uint16_t (*num_items_cb_t)(char *name);
typedef char * (*item_text_cb_t)(char *name);
typedef void (*select_cb_t)(char *result);

// Populate an array of parts from a decoded menu ID
// and return the number of parts.
// Pass in an "uint8_t parts[MAX_MENU_DEPTH]" array
uint8_t menu_parts(char *id, uint8_t *results);

// Show the menu
void show_testmenu(select_cb_t select_cb,
                   num_items_cb_t num_items_cb,
                   item_text_cb_t item_text_cb,
                   is_menu_cb_t is_menu_cb,
                   char *selection);

// Hide the menu
void hide_testmenu(void);


