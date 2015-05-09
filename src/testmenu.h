typedef bool (*is_menu_cb_t)(char *name);
typedef uint16_t (*num_items_cb_t)(char *name);
typedef char * (*item_text_cb_t)(char *name);
typedef void (*select_cb_t)(char *result);

void show_testmenu(select_cb_t select_cb, 
                   num_items_cb_t num_items_cb, 
                   item_text_cb_t item_text_cb, 
                   is_menu_cb_t is_menu_cb,
                   char *selection);
 
void hide_testmenu(void);
