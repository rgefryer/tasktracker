#pragma once

#define MAX_TASK_NAME_LEN 25
#define NUM_TASKS 50
  
#define MAX_LABEL_NAME_LEN 25
#define NUM_LABELS 10

#define TASK_ARCHIVE_WEEKS 1
#define TASK_COUNT_TODAY 0
#define TASK_COUNT_WEEK 1
#define TASK_ARCHIVE_WEEK_BASE 2

#define LINK_BITFIELD_LEN ((NUM_TASKS+8)/8)
  
#define NUM_GOALS 10
#define NUM_ARCHIVED_GOALS 7

#define NUM_TASK_RECORDS 40
  
  
bool data_save(void);
void data_load(void);

