#include <pebble.h>
#include <data.h>
#include <data_int.h>

char *g_task_names[NUM_TASKS+1];
#define SID_TASK_NAME_BASE 1000

char *g_label_names[NUM_LABELS+1];
#define SID_LABEL_NAME_BASE 2000

time_t g_task_times[NUM_TASKS+1][TASK_ARCHIVE_WEEKS + TASK_ARCHIVE_WEEK_BASE];
#define SID_TIMES_BASE 3000

// Bit field of tasks that each label applies to.  Both label
// and task values are 1-indexed.
char g_label_task_links[NUM_LABELS+1][LINK_BITFIELD_LEN];
#define SID_LINKS_BASE 4000

struct Goal g_goals[NUM_GOALS+1];
#define SID_GOALS_BASE 5000

// Circular buffer of recent tasks
uint8_t next_task_record = 0;
uint8_t num_task_records = 0;

struct TaskRecord g_task_records[NUM_TASK_RECORDS];
// 6000 is an array of the s_tasks; subsequent IDs have start/spent for
// each task.  First written is the oldest task.
#define SID_HISTORY_TASK_IDS 6000

// Time after which an archive step is required.  This will
// be midnight after the last one.
time_t g_last_archive;
#define SID_LAST_ARCHIVE 7000

// Details of the current task
time_t g_current_task_start;
uint8_t g_current_task_id;
#define SID_CURRENT_TASK_START 7001
#define SID_CURRENT_TASK_ID 7002

// Offset to add to the storage ID, 0 or 10,000.
// This allows us to commit an update as a transaction,
// and update the g_sid_offset at the end if all the
// writes were succesful.
int g_sid_offset = 0;
#define SID_OFFSET 10000
#define SID_OFFSET_ID 1

char *s_tasks[] = {
  "GR testing",  // 1
  "GR kit",      // 2
  "GR status",   // 3
  "Status",      // 4
  "Tracking",    // 5
  "Hackathon"    // 6
};

char *s_labels[] = {
  "Work",        // 1
  "EAS GR",      // 2
  "Fixed costs", // 3
  "Overheads"    // 4
};

// Pairs of task, label
char s_link_data[] = {
  1, 1,
  1, 2,
  2, 1,
  2, 2,
  3, 1,
  3, 2,
  4, 1,
  4, 3,
  5, 1,
  5, 3,
  6, 1,
  6, 3
};


void bitfield_set(char *bitfield, int bit) {
  int byte = bit / 8;
  bit = bit % 8;
  char mask = 0x01 << bit;

  bitfield[byte] |= mask;
}

bool bitfield_get(char *bitfield, int bit) {
  int byte = bit / 8;
  bit = bit % 8;
  char mask = 0x01 << bit;

  if (bitfield[byte] | mask) {
    return true;
  }
  else {
    return false;
  }
}

// Return the number of labels defined
int num_labels(void)
{
  int label_count = 0;
  for (ix = 1; ix <= NUM_LABELS; ix++)
    if (g_label_names[ix] != NULL)
      label_count++;

  return label_count;
}

// Return the number of tasks defined
int num_tasks(void)
{
  int task_count = 0;
  for (ix = 1; ix <= NUM_TASKS; ix++)
    if (g_task_names[ix] != NULL)
      task_count++;

  return task_count;
}

// Return the task IDs in a label (ordered by most recently used)
// If label is 0, don't filter by label.
uint8_t *ordered_tasks(uint8_t label) {
  static uint8_t tasks[NUM_TASKS + 1];

  int task_count = 0;
  for (ix = 1; ix <= NUM_TASKS; ix++) {
    if (g_task_names[ix] != NULL) {
      if (label == 0) {
        tasks[task_count++] = ix;
      }
      else {
        if (bitfield_get(g_label_task_links[label], ix)) {
          tasks[task_count++] = ix;
        }
      }
    }
  }

  tasks[task_count] = 0;

  return tasks;
}

// Return the labels (ordered by most recently used)
uint8_t *ordered_labels() {
  static uint8_t labels[NUM_LABELS + 1];

  int label_count = 0;
  for (ix = 1; ix <= NUM_LABELS; ix++)
    if (g_label_names[ix] != NULL)
      labels[label_count++] = ix;

  labels[label_count] = 0;

  return labels;
}

// Return the number of items in a 0-terminated
// array of IDs.
uint16_t num_ids(uint8_t *ids) {
  pos = 0;
  while (ids[pos] != 0)
    pos++;

  return pos;
}

// Return the task/label name corresponding to an ID
char *label_name(uint8_t id) {
  return g_label_names[id];
}
char *task_name(uint8_t id) {
  return g_task_names[id];
}


#define ARRAY_SIZE(X) (sizeof (X) / sizeof (X)[0])
void data_load()
{
  g_sid_offset = 0;
  if (persist_exists(SID_OFFSET_ID)) {
    g_sid_offset = persist_read_int(SID_OFFSET_ID);
  }

  // For now, initialize task names from hard-coded data
  int ix;
  for (ix = 0; ix <= NUM_TASKS; ix++) {
    g_task_names[ix] = NULL;
  }

  int num_tasks = ARRAY_SIZE(s_tasks);
  for (ix = 0; ix < num_tasks; ix++) {
    g_task_names[ix+1] = s_tasks[ix];
  }

  // For now, initialize label names from hard-coded data
  for (ix = 0; ix <= NUM_LABELS; ix++) {
    g_label_names[ix] = NULL;
  }

  int num_labels = ARRAY_SIZE(s_labels);
  for (ix = 0; ix < num_labels; ix++) {
    g_label_names[ix+1] = s_labels[ix];
  }

  // For now, initialize g_label_task_links from hard-coded data
  int label;
  for (label = 0; label <= NUM_LABELS; label++) {
    for (ix = 0; ix < LINK_BITFIELD_LEN; ix++)
      g_label_task_links[label][ix] = 0;
  }

  int num_links = ARRAY_SIZE(s_link_data);
  for (ix = 0; ix < num_links; ix += 2) {
    int task = s_link_data[ix];
    int label = s_link_data[ix+1];
    bitfield_set(g_label_task_links[label], task);
  }

  // For now, initialize g_goals from hard-coded data
  for (ix = 0; ix < NUM_GOALS; ix++) {
    g_goals[ix].init = 0;
  }

  // Goal 0 is 7.5 hours working per day
  memset(&(g_goals[0]), 0, sizeof g_goals[0]);
  g_goals[0].init = 1;
  g_goals[0].target = (75 * 60 * 6);
  g_goals[0].label = 1;

  // Load in goal history from storage
  int bytes_read;
  for (ix = 0; ix < NUM_GOALS; ix++) {
    bytes_read = persist_read_data(g_sid_offset + SID_GOALS_BASE+ix,
                                   &(g_goals[ix]),
                                   sizeof g_goals[ix]);
  }

  // Load in task history from storage
  char task_array[NUM_TASK_RECORDS];
  bytes_read = persist_read_data(g_sid_offset + SID_HISTORY_TASK_IDS,
                                 task_array,
                                 NUM_TASK_RECORDS);
  for (ix = 0; ix < bytes_read; ix++) {
    if (task_array[ix] != 0) {
      g_task_records[ix].task =  task_array[ix];
      persist_read_data(g_sid_offset + SID_HISTORY_TASK_IDS+ix,
                        &(g_task_records[ix]),
                        offsetof(struct TaskRecord, task));
      next_task_record++;
      num_task_records++;
    }
  }
  if (next_task_record == NUM_TASK_RECORDS) {
    next_task_record = 0;
  }

  // Load in other global variables from storage
  g_last_archive = time(NULL);
  persist_read_data(g_sid_offset + SID_LAST_ARCHIVE,
                    &g_last_archive,
                    sizeof g_last_archive);

  g_current_task_start = 0;
  persist_read_data(g_sid_offset + SID_CURRENT_TASK_START,
                    &g_current_task_start,
                    sizeof g_current_task_start);

  g_current_task_id = 0;
  persist_read_data(g_sid_offset + SID_CURRENT_TASK_ID,
                    &g_current_task_id,
                    sizeof g_current_task_id);

}

// Save off the current state.  Return true on success,
// false on failure.
bool data_save()
{
  // Toggle the data version to write
  int old_sid_offset = g_sid_offset;
  if (g_sid_offset == SID_OFFSET) {
    g_sid_offset = 0;
  }
  else {
    g_sid_offset = SID_OFFSET;
  }

  // Save task names to storage
  int ix;
  int bytes_written = 0;
  int bytes_to_write = 0;
  for (ix = 1; ix <= NUM_TASKS; ix++) {
    if (g_task_names[ix] != NULL) {
        bytes_written += persist_write_string(g_sid_offset + SID_TASK_NAME_BASE+ix,
                                              g_task_names[ix]);
        bytes_to_write += 1 + strlen(g_task_names[ix]);
    }
    // Never delete s_tasks, as it might lose information.  Just
    // allow them to be renamed.
    // else {
    //   persist_delete(g_sid_offset + SID_TASK_NAME_BASE+ix);
    // }
  }

  // Save label names to storage
  for (ix = 1; ix <= NUM_LABELS; ix++) {
    if (g_label_names[ix] != NULL) {
        bytes_written += persist_write_string(g_sid_offset + SID_LABEL_NAME_BASE+ix,
                                             g_label_names[ix]);
        bytes_to_write += 1 + strlen(g_label_names[ix]);
    }
    else {
      persist_delete(g_sid_offset + SID_LABEL_NAME_BASE+ix);
    }
  }

  // Write configured g_label_task_links to storage
  for (ix = 1; ix <= NUM_LABELS; ix++) {
    if (g_label_names[ix] != NULL) {
        bytes_written += persist_write_data(g_sid_offset + SID_LINKS_BASE+ix,
                                           g_label_task_links[ix],
                                           LINK_BITFIELD_LEN);
        bytes_to_write += LINK_BITFIELD_LEN;
    }
    else {
      persist_delete(g_sid_offset + SID_LINKS_BASE+ix);
    }
  }

  // Write configured g_goals to storage
  for (ix = 0; ix < NUM_GOALS; ix++) {
    if (g_goals[ix].init) {
      bytes_written += persist_write_data(g_sid_offset + SID_GOALS_BASE+ix,
                                         &(g_goals[ix]),
                                         sizeof g_goals[ix]);
      bytes_to_write += sizeof g_goals[ix];

    }
    else {
      persist_delete(g_sid_offset + SID_GOALS_BASE+ix);
    }
  }

  // Save task history to storage.  First, build an array of the s_tasks IDs.
  char task_array[NUM_TASK_RECORDS];
  memset(task_array, 0, NUM_TASK_RECORDS);
  if (num_task_records < NUM_TASK_RECORDS) {
    ix = 0;
  }
  else {
    ix = next_task_record;
    if (ix == NUM_TASK_RECORDS) {
      ix = 0;
    }
  }
  int count = num_task_records;
  int pos = 0;
  while (count != 0) {

    bytes_written += persist_write_data(g_sid_offset + SID_HISTORY_TASK_IDS+pos,
                                       &(g_task_records[ix]),
                                       offsetof(struct TaskRecord, task));
    bytes_to_write += offsetof(struct TaskRecord, task);
    // or: OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field)) if not supported
    task_array[pos++] = g_task_records[ix].task;

    if (++ix == NUM_TASK_RECORDS) {
      ix = 0;
    }
    count--;
  }
  bytes_written += persist_write_data(g_sid_offset + SID_HISTORY_TASK_IDS,
                                     task_array,
                                     NUM_TASK_RECORDS);
  bytes_to_write += NUM_TASK_RECORDS;

  // Write other globals to storage.
  bytes_written += persist_write_data(g_sid_offset + SID_LAST_ARCHIVE,
                     &g_last_archive,
                     sizeof g_last_archive);

  bytes_written += persist_write_data(g_sid_offset + SID_CURRENT_TASK_START,
                     &g_current_task_start,
                     sizeof g_current_task_start);

  bytes_written += persist_write_data(g_sid_offset + SID_CURRENT_TASK_ID,
                     &g_current_task_id,
                     sizeof g_current_task_id);

  // Check that we wrote everything we expected.
  bytes_to_write += sizeof g_last_archive + sizeof g_current_task_start + sizeof g_current_task_id;
  if (bytes_written != bytes_to_write) {
    g_sid_offset = old_sid_offset;
    return false;
  }

  // "Commit" the "Transaction".
  if (persist_write_int(SID_OFFSET_ID, g_sid_offset) != S_SUCCESS) {
    g_sid_offset = old_sid_offset;
    return false;
  }

  return true;
}

