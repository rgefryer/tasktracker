#include <pebble.h>
#include "mainface.h"
#include "mainmenu.h"
#include "data.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_roboto_condensed_21;
static TextLayer *clockface;
static TextLayer *this_task_text;
static TextLayer *this_task_current;
static TextLayer *label_text;
static TextLayer *this_task_total;
static TextLayer *label_remaining;
static TextLayer *date;
static InverterLayer *s_inverterlayer_3;
static InverterLayer *s_inverterlayer_2;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // clockface
  clockface = text_layer_create(GRect(1, 121, 140, 49));
  text_layer_set_background_color(clockface, GColorBlack);
  text_layer_set_text_color(clockface, GColorWhite);
  text_layer_set_text(clockface, "00:00");
  text_layer_set_text_alignment(clockface, GTextAlignmentCenter);
  text_layer_set_font(clockface, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)clockface);
  
  // this_task_text
  this_task_text = text_layer_create(GRect(2, 0, 140, 25));
  text_layer_set_background_color(this_task_text, GColorBlack);
  text_layer_set_text_color(this_task_text, GColorWhite);
  text_layer_set_text(this_task_text, "Paused");
  text_layer_set_font(this_task_text, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_text);
  
  // this_task_current
  this_task_current = text_layer_create(GRect(54, 24, 87, 25));
  text_layer_set_background_color(this_task_current, GColorBlack);
  text_layer_set_text_color(this_task_current, GColorWhite);
  text_layer_set_text(this_task_current, "00:00:00");
  text_layer_set_text_alignment(this_task_current, GTextAlignmentRight);
  text_layer_set_font(this_task_current, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_current);
  
  // label_text
  label_text = text_layer_create(GRect(2, 53, 140, 25));
  text_layer_set_background_color(label_text, GColorBlack);
  text_layer_set_text_color(label_text, GColorWhite);
  text_layer_set_text(label_text, "Label");
  text_layer_set_font(label_text, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)label_text);
  
  // this_task_total
  this_task_total = text_layer_create(GRect(1, 24, 51, 25));
  text_layer_set_background_color(this_task_total, GColorBlack);
  text_layer_set_text_color(this_task_total, GColorWhite);
  text_layer_set_text(this_task_total, "00:00");
  text_layer_set_font(this_task_total, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_total);
  
  // label_remaining
  label_remaining = text_layer_create(GRect(66, 77, 74, 25));
  text_layer_set_background_color(label_remaining, GColorBlack);
  text_layer_set_text_color(label_remaining, GColorWhite);
  text_layer_set_text(label_remaining, "0:00:00");
  text_layer_set_text_alignment(label_remaining, GTextAlignmentRight);
  text_layer_set_font(label_remaining, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)label_remaining);
  
  // date
  date = text_layer_create(GRect(3, 104, 139, 25));
  text_layer_set_background_color(date, GColorBlack);
  text_layer_set_text_color(date, GColorWhite);
  text_layer_set_text(date, "May 27");
  text_layer_set_font(date, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date);
  
  // s_inverterlayer_3
  s_inverterlayer_3 = inverter_layer_create(GRect(2, 103, 140, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_3);
  
  // s_inverterlayer_2
  s_inverterlayer_2 = inverter_layer_create(GRect(1, 50, 140, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_2);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(clockface);
  text_layer_destroy(this_task_text);
  text_layer_destroy(this_task_current);
  text_layer_destroy(label_text);
  text_layer_destroy(this_task_total);
  text_layer_destroy(label_remaining);
  text_layer_destroy(date);
  inverter_layer_destroy(s_inverterlayer_3);
  inverter_layer_destroy(s_inverterlayer_2);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

// Callback when a task is selected from the main menu.
// Start tracking the task.
void new_task_cb(uint8_t task_id)
{
  // Update the display of the current task
  char *text = task_name(task_id);
  text_layer_set_text(this_task_text, text);
  
  // Tell the data layer that we are starting a new task
  start_new_task(task_id);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler");
  //text_layer_set_text(text_layer, "Select");
  show_task_menu(new_task_cb);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "up_click_handler");
  //text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "down_click_handler");
  //text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "click_config_provider");
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Format a duration for display
void format_duration(time_t duration, char* buffer, bool secs) {
  int seconds = (int)duration % 60;
  int minutes = (int)duration / 60 % 60;
  int hours = (int)duration / 3600;

  if (secs)
    snprintf(buffer, 9, "%02d:%02d:%02d", hours, minutes, seconds);
  else
    snprintf(buffer, 9, "%02d:%02d", hours, minutes);    
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  update_tracked_time(temp);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char buffer2[] = "Dec 25";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  strftime(buffer2, sizeof("Dec 25"), "%b %e", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(clockface, buffer);
  text_layer_set_text(date, buffer2);
  
  // Update the time spent on the current task today.
  static char buffer3[] = "00:00:00";
  uint8_t task_id = current_task_id();
  uint32_t task_time = time_in_task_today(task_id);
  format_duration(task_time, buffer3, false);
  text_layer_set_text(this_task_total, buffer3);
  
  // Update the time spent on the current task today.
  static char buffer4[] = "00:00:00";
  task_time = time_in_current_task();
  format_duration(task_time, buffer4, true);
  text_layer_set_text(this_task_current, buffer4);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void show_main(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider); 
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);  
  
  // Make sure the time is displayed from the start
  update_time();  
}

void hide_main(void) {
  window_stack_remove(s_window, true);
}

static void init(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init");
  data_load();
  show_main();
}

static void deinit(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit");
  data_save();
  hide_main();
}

int main(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main");
  init();
  app_event_loop();
  deinit();
}
