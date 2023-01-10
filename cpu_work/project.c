#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>


#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

#define MAX_JOBS 100

typedef struct {
  int burst_time;
  int arrival_time;
  int priority;
} Job;

int preemptive = -1;
int wait_time = 0;
int turnaround_time = 0;

//read jobs
int read_jobs(Job jobs[], char *file_name) {
  FILE *input_file = fopen("input.txt", "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Could not open input file.\n");
    return 0;
  }

  int num_jobs = 0;
  while (fscanf(input_file, "%d:%d:%d", &jobs[num_jobs].burst_time, &jobs[num_jobs].arrival_time, &jobs[num_jobs].priority) == 3) {
    num_jobs++;
  }

  fclose(input_file);
  return num_jobs;
}

int find_next_job(Job *jobs, int num_jobs, int current_time) {
    int min_priority = INT_MAX;
    int min_priority_index = -1;
    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].arrival_time <= current_time) {
            if (jobs[i].priority < min_priority) {
                min_priority = jobs[i].priority;
                min_priority_index = i;
            }
        }
    }
    return min_priority_index;
}


void write_jobs(Job *jobs, int num_jobs, FILE *output_file) {
  output_file = fopen("output.txt", "w");
  for (int i = 0; i < num_jobs; i++) {
    fprintf(output_file, "%d:%d:%d\n", jobs[i].arrival_time, jobs[i].burst_time,
            jobs[i].priority);
  }
}


//first come first serve function

void fcfs(Job *jobs, int num_jobs, FILE *output_file) {
  printf("\nFirst come first-served (FCFS):\n");
  fprintf(output_file, "First come first served (FCFS):\n");

  int current_time = 0;
  int waiting_time[num_jobs];
  int total_waiting_time = 0;
  for (int i = 0; i < num_jobs; i++) {
    waiting_time[i] = current_time - jobs[i].arrival_time;
    current_time += jobs[i].burst_time;
  }

  for (int i = 0; i < num_jobs; i++) {
    total_waiting_time += waiting_time[i];
    printf("p %d: Waiting time = %d\n", i + 1, waiting_time[i]);
    fprintf(output_file, "p %d: Waiting time = %d\n", i + 1, waiting_time[i]);
  }
   float average_waiting_time = (float) total_waiting_time / (float)num_jobs;
   printf("Average waiting time %.2f \n", average_waiting_time);
}


//shortest job first function
void sjf(Job *jobs, int num_jobs, int preemptive, FILE *output_file) {
  printf("\nShortest job first (SJF):\n");
  fprintf(output_file, "Shortest job first (SJF):\n");

  int current_time = 0;
  int waiting_time[num_jobs];
  int remaining_time[num_jobs];
  for (int i = 0; i < num_jobs; i++) {
    waiting_time[i] = 0;
    remaining_time[i] = jobs[i].burst_time;
  }

  while (TRUE) {
    int shortest_job = -1;
    int shortest_time = INT_MAX;
    for (int i = 0; i < num_jobs; i++) {
      if (remaining_time[i] > 0 && remaining_time[i] < shortest_time && jobs[i].arrival_time <= current_time) {
        shortest_time = remaining_time[i];
        shortest_job = i;
      }
    }

    if (shortest_job == -1) {
      break;
    }

    remaining_time[shortest_job]--;
    current_time++;

    for (int i = 0; i < num_jobs; i++) {
      if (i != shortest_job && remaining_time[i] > 0 && jobs[i].arrival_time <= current_time) {
        waiting_time[i]++;
      }
    }

    if (preemptive == 1) {
      int shortest_job = -1;
      int shortest_time = INT_MAX;
      for (int i = 0; i < num_jobs; i++) {
        if (remaining_time[i] > 0 && remaining_time[i] < shortest_time && jobs[i].arrival_time <= current_time) {
          shortest_time = remaining_time[i];
          shortest_job = i;
        }
      }

      if (shortest_time == 0) {
        remaining_time[shortest_job] = -1;
        continue;
      }
    }
  }

  int total_waiting_time = 0;
  for (int i = 0; i < num_jobs; i++) {
    total_waiting_time += waiting_time[i];
    printf("p %d: Waiting time = %d\n", i + 1, waiting_time[i]);
    fprintf(output_file, "p %d: Waiting time = %d ms\n", i + 1, waiting_time[i]);
  }
  float Average_waiting_time = (float)total_waiting_time/ (float)num_jobs;
  printf("Average waiting time %.2f \n", Average_waiting_time);
}

 



//priority function
void priority(Job jobs[], int num_jobs, FILE *output_file) 
{
   int current_time = 0;
    int completed_jobs = 0;
    int total_waiting_time = 0;
    while (completed_jobs < num_jobs) {
        int next_job = find_next_job(jobs, num_jobs, current_time);
        if (next_job == -1) {
            current_time++;
            
        }
        int waiting_time = current_time - jobs[next_job].arrival_time;
        current_time += jobs[next_job].burst_time;
        completed_jobs++;
        total_waiting_time += waiting_time;
        printf( "p %d waiting time %d\n", next_job +1, waiting_time);
        fprintf(output_file, "p %d waiting time %d\n", next_job + 1, waiting_time);
    }
      float average_waiting_time = (float) total_waiting_time / (float) num_jobs;
       printf("Average waiting time: %.2f\n", average_waiting_time);
       fprintf(output_file, "Average waiting time: %.2f\n", average_waiting_time);
}


//round robin function
void round_robin(Job *jobs, int num_jobs, int time_slice, FILE *output_file) {
    int current_time = 0;
    int completed_jobs = 0;
    int total_waiting_time = 0;
    int current_job = 0;

    while (completed_jobs < num_jobs) {
        // check if the current job has arrived
        if (jobs[current_job].arrival_time <= current_time) {
            if (jobs[current_job].burst_time > time_slice) {
                current_time += time_slice;
                jobs[current_job].burst_time -= time_slice;
            } else {
                current_time += jobs[current_job].burst_time;
                int waiting_time = current_time - jobs[current_job].arrival_time - jobs[current_job].burst_time;
                total_waiting_time += waiting_time;
                printf("p %d  waiting time %d\n", current_job+1, waiting_time);
                fprintf(output_file, "p %d  waiting time %d\n", current_job+1, waiting_time);
                completed_jobs++;
            }
        } else {
            current_time++;
        }
        current_job = (current_job + 1) % num_jobs;
    }
    float average_waiting_time = (float) total_waiting_time / (float) num_jobs;
    printf("Average waiting time: %.2f\n", average_waiting_time);
    fprintf(output_file, "Average waiting time: %.2f\n", average_waiting_time);
}




int main(int argc, char *argv[]) {
  int opt;
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  int preemptive = -1;
  int schedule_algorithm = -1;
  while ((opt = getopt(argc, argv, "f:o:s:p")) != -1) {
    switch (opt) {
      case 'f':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 's':
        schedule_algorithm = atoi(optarg);
        break;  
      case 'p':
        preemptive = atoi(optarg);
        break;
      default:
        fprintf(stderr, "Usage: %s [-f input_file] [-o output_file] [-p]\n", argv[0]);
        exit(1);
    }
  }

  if (input_file_name == NULL) {
    fprintf(stderr, "Error: Input file not specified.\n");
    exit(1);
  }

  if (output_file_name == NULL) {
    fprintf(stderr, "Error: Output file not specified.\n");
    exit(1);
  }

  Job jobs[MAX_JOBS];
  int num_jobs = read_jobs(jobs, input_file_name);
  
  printf("\t Scheduling algorithms:\n");
  printf("1. First-come-first-served (FCFS)\n");
  printf("2. Shortest job first (SJF)\n");
  printf("3. Priority scheduling\n");
  printf("4. Round Robin\n");
  printf("Enter the number of the scheduling algorithm to use: ");
  
  scanf("%d", &schedule_algorithm);

  printf("Select Preemptive mode:\n");
  printf("0 - Non-Preemptive mode\n");
  printf("1 - Peemptive mode\n");
  scanf("%d", &preemptive);


  FILE *output_file = fopen("output.txt", "w");
  if (output_file == NULL) {
    fprintf(stderr, "Error: Could not open output file.\n");
    exit(1);
  }

  switch (schedule_algorithm) {
    case 1:
      fcfs(jobs, num_jobs, output_file);
      break;
    case 2:
      sjf(jobs, num_jobs, preemptive, output_file);
      break;
    case 3:
      priority(jobs, num_jobs, output_file);
      break;
    case 4:
      printf("Enter time quantum: ");
      int time_slice;
      scanf("%d", &time_slice);
      round_robin(jobs, num_jobs, time_slice, output_file);
      break;
    default:
      fprintf(stderr, "Error: Invalid scheduling algorithm.\n");
      exit(1);
  }
     write_jobs(jobs, num_jobs, output_file);
  fclose(output_file);
  return 0;
}
