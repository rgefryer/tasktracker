#include <pebble.h>
#include "main.h"
#include "button_click.h"
#include "data.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_roboto_condensed_21;
static TextLayer *clockface;
static TextLayer *this_task_name;
static TextLayer *this_task_current;
static TextLayer *last_task_name;
static TextLayer *last_task_current;
static InverterLayer *s_inverterlayer_2;
static TextLayer *goal_name;
static TextLayer *goal_target;
static TextLayer *this_task_total;
static TextLayer *last_task_total;
static TextLayer *goal_remaining;
static TextLayer *date;
static InverterLayer *s_inverterlayer_3;

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
  
  // this_task_name
  this_task_name = text_layer_create(GRect(2, 2, 140, 15));
  text_layer_set_background_color(this_task_name, GColorBlack);
  text_layer_set_text_color(this_task_name, GColorWhite);
  text_layer_set_text(this_task_name, "Current task");
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_name);
  
  // this_task_current
  this_task_current = text_layer_create(GRect(82, 18, 60, 15));
  text_layer_set_background_color(this_task_current, GColorBlack);
  text_layer_set_text_color(this_task_current, GColorWhite);
  text_layer_set_text(this_task_current, "1:15:32");
  text_layer_set_text_alignment(this_task_current, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_current);
  
  // last_task_name
  last_task_name = text_layer_create(GRect(2, 34, 140, 15));
  text_layer_set_background_color(last_task_name, GColorBlack);
  text_layer_set_text_color(last_task_name, GColorWhite);
  text_layer_set_text(last_task_name, "Previous task");
  layer_add_child(window_get_root_layer(s_window), (Layer *)last_task_name);
  
  // last_task_current
  last_task_current = text_layer_create(GRect(81, 50, 60, 15));
  text_layer_set_background_color(last_task_current, GColorBlack);
  text_layer_set_text_color(last_task_current, GColorWhite);
  text_layer_set_text(last_task_current, "0:15:32");
  text_layer_set_text_alignment(last_task_current, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)last_task_current);
  
  // s_inverterlayer_2
  s_inverterlayer_2 = inverter_layer_create(GRect(1, 68, 140, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_2);
  
  // goal_name
  goal_name = text_layer_create(GRect(2, 69, 140, 15));
  text_layer_set_background_color(goal_name, GColorBlack);
  text_layer_set_text_color(goal_name, GColorWhite);
  text_layer_set_text(goal_name, "Goal 1");
  layer_add_child(window_get_root_layer(s_window), (Layer *)goal_name);
  
  // goal_target
  goal_target = text_layer_create(GRect(2, 84, 83, 15));
  text_layer_set_background_color(goal_target, GColorBlack);
  text_layer_set_text_color(goal_target, GColorWhite);
  text_layer_set_text(goal_target, "Daily 7:30:00");
  layer_add_child(window_get_root_layer(s_window), (Layer *)goal_target);
  
  // this_task_total
  this_task_total = text_layer_create(GRect(20, 18, 66, 15));
  text_layer_set_background_color(this_task_total, GColorBlack);
  text_layer_set_text_color(this_task_total, GColorWhite);
  text_layer_set_text(this_task_total, "2:15:10");
  layer_add_child(window_get_root_layer(s_window), (Layer *)this_task_total);
  
  // last_task_total
  last_task_total = text_layer_create(GRect(20, 50, 66, 15));
  text_layer_set_background_color(last_task_total, GColorBlack);
  text_layer_set_text_color(last_task_total, GColorWhite);
  text_layer_set_text(last_task_total, "0:15:32");
  layer_add_child(window_get_root_layer(s_window), (Layer *)last_task_total);
  
  // goal_remaining
  goal_remaining = text_layer_create(GRect(81, 84, 59, 15));
  text_layer_set_background_color(goal_remaining, GColorBlack);
  text_layer_set_text_color(goal_remaining, GColorWhite);
  text_layer_set_text(goal_remaining, "3:23:15");
  text_layer_set_text_alignment(goal_remaining, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)goal_remaining);
  
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
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(clockface);
  text_layer_destroy(this_task_name);
  text_layer_destroy(this_task_current);
  text_layer_destroy(last_task_name);
  text_layer_destroy(last_task_current);
  inverter_layer_destroy(s_inverterlayer_2);
  text_layer_destroy(goal_name);
  text_layer_destroy(goal_target);
  text_layer_destroy(this_task_total);
  text_layer_destroy(last_task_total);
  text_layer_destroy(goal_remaining);
  text_layer_destroy(date);
  inverter_layer_destroy(s_inverterlayer_3);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler");
  //text_layer_set_text(text_layer, "Select");
  show_task_menu();
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

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
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
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
  
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