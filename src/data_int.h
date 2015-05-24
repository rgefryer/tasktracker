#include <pebble.h>
#include <data.h>

extern char *g_task_names[NUM_TASKS+1];
extern char *g_label_names[NUM_LABELS+1];
extern time_t g_task_times[NUM_TASKS+1][TASK_ARCHIVE_WEEKS + TASK_ARCHIVE_WEEK_BASE];
extern char g_label_task_links[NUM_LABELS+1][LINK_BITFIELD_LEN];

struct Goal {
  uint32_t target;  // Target time to spend on this, in seconds
  uint32_t archive[NUM_ARCHIVED_GOALS];  //  Achievements in previous periods
  uint8_t init;  // Flag that this goal has been initialized
  uint8_t task;  // Task this goal applies to
  uint8_t label;   // Label this goal applies to
  uint8_t weekly; // Flag that the goal is weekly (otherwise it's daily)
};
extern struct Goal g_goals[NUM_GOALS+1];

struct TaskRecord {
  time_t start_time;
  uint32_t spent;

  uint8_t task; // First field that _is not_ written to storage.
};
extern uint8_t next_task_record;
extern uint8_t num_task_records;
extern struct TaskRecord g_task_records[NUM_TASK_RECORDS];

extern time_t g_last_archive;

extern time_t g_current_task_start;
extern uint8_t g_current_task_id;


