#pragma once

#define MAX_TASK_NAME_LEN 25
#define NUM_TASKS 50

#define MAX_LABEL_NAME_LEN 25
#define NUM_LABELS 10

#define TASK_COUNT_TODAY 0
#define TASK_COUNT_YESTERDAY 1
#define TASK_COUNT_THIS_WEEK 2
#define TASK_COUNT_LAST_WEEK 3
#define TASK_COUNTS 4

#define LINK_BITFIELD_LEN ((NUM_TASKS+8)/8)

#define NUM_GOALS 10
#define NUM_ARCHIVED_GOALS 7

#define NUM_TASK_RECORDS 40

#define MAX_RECENT_TASKS 10

bool data_save(void);
void data_load(void);

/*
  Return the top level menu as an array of string
  pointers.  A null pointer marks the end of the menu.
*/
char **top_level_task_menu();

/*
  Return a sub-menu of tasks as an array of string
  pointers.  A null pointer marks the end of the menu.
*/
char **second_level_task_menu();

// Number of labels defined.
int num_labels(void);

// Number of tasks defined.
int num_tasks(void);

// Return the task IDs in a label (ordered by most recently used)
// If label is 0, don't filter by label.
uint8_t *ordered_tasks(uint8_t label);

// Return the label IDs (ordered by most recently used)
uint8_t *ordered_labels();

// Return the number of items in a 0-terminated
// array of IDs.
uint16_t num_ids(uint8_t *ids);

// Return the task/label name corresponding to an ID
char *label_name(uint8_t id);
char *task_name(uint8_t id);

// Trigger the startup of a new task
void start_new_task(uint8_t id, time_t start_time);

// Return the number of seconds spent on a task, today
uint32_t time_in_task_today(uint8_t id);

// Return the number of seconds spent on a label, today
uint32_t time_in_label_today(uint8_t id);

// Return the number of seconds spent on the current task
uint32_t time_in_current_task();

// Return the ID of the current task
uint8_t current_task_id();

// Pass the latest time to the tracking data
void update_tracked_time(time_t time_now);
