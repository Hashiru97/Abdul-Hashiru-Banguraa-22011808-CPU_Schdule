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

int preemptive = 0;
int wait_time = 0;
int turnaround_time = 0;


int read_jobs(Job jobs[], char *file_name) {
  FILE *input_file = fopen("input.txt", "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Could not open input file.\n");
    return 0;
  }

  int num_jobs = 0;
  while (fscanf(input_file, "%d %d %d", &jobs[num_jobs].arrival_time, &jobs[num_jobs].burst_time, &jobs[num_jobs].priority) == 3) {
    num_jobs++;
  }

  fclose(input_file);
  return num_jobs;
}

//first come first serve function

void fcfs(Job *jobs, int num_jobs, FILE *output_file) {
  printf("\nFirst-come-first-served (FCFS):\n");
  fprintf(output_file, "First-come-first-served (FCFS):\n");

  int current_time = 0;
  int waiting_time[num_jobs];
  for (int i = 0; i < num_jobs; i++) {
    waiting_time[i] = current_time - jobs[i].arrival_time;
    current_time += jobs[i].burst_time;
  }

  for (int i = 0; i < num_jobs; i++) {
    printf("Job %d: Waiting time = %d\n", i + 1, waiting_time[i]);
    fprintf(output_file, "Job %d: Waiting time = %d\n", i + 1, waiting_time[i]);
  }
}


//shortest job first function
void sjf(Job *jobs, int num_jobs, int preemptive, FILE *output_file) {
  printf("\nShortest job first (SJF):\n");
  fprintf(output_file, "Shortest job first (SJF):\n");

  int current_time = 0;
  int waiting_time[num_jobs];
  int remaining_time[num_jobs];
  for (int i = 0; i < num_jobs; i++) {
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
    waiting_time[shortest_job] = current_time - jobs[shortest_job].arrival_time - jobs[shortest_job].burst_time;
  }

  for (int i = 0; i < num_jobs; i++) {
    printf("Job %d: Waiting time = %d\n", i + 1, waiting_time[i]);
    fprintf(output_file, "Job %d: Waiting time = %d\n", i + 1, waiting_time[i]);
  }
}


//priority function
void priority(Job jobs[], int num_jobs) {
  int current_time = 0;
  while (num_jobs > 0) {
    int highest_priority = -1;
    for (int j = 0; j < num_jobs; j++) {
      if (jobs[j].arrival_time <= current_time) {
        if (highest_priority == -1 || jobs[j].priority > jobs[highest_priority].priority) {
          highest_priority = j;
        }
      }
    }
    if (highest_priority == -1) {
      current_time++;
    } else {
      printf("Processing job %d\n", highest_priority + 1);
      wait_time += current_time - jobs[highest_priority].arrival_time;
      turnaround_time += current_time - jobs[highest_priority].arrival_time + jobs[highest_priority].burst_time;
      current_time += jobs[highest_priority].burst_time;
      num_jobs--;
      jobs[highest_priority] = jobs[num_jobs];
    }
  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);
}

//round robin function
void round_robin(Job jobs[], int num_jobs, int time_slice) {
  int current_time = 0;
  int current_job = 0;
  while (num_jobs > 0) {
    printf("P %d\n", current_job + 1);
    int run_time = MIN(time_slice, jobs[current_job].burst_time);
    wait_time += current_time - jobs[current_job].arrival_time;
    turnaround_time += current_time - jobs[current_job].arrival_time + run_time;
    current_time += run_time;
    jobs[current_job].burst_time -= run_time;
    if (jobs[current_job].burst_time == 0) {
      num_jobs--;
      jobs[current_job] = jobs[num_jobs];
    }
    current_job = (current_job + 1) % num_jobs;
  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
}


int main(int argc, char *argv[]) {
  int opt;
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  while ((opt = getopt(argc, argv, "f:o:")) != -1) {
    switch (opt) {
      case 'f':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      default:
        fprintf(stderr, "Usage: %s [-f input_file] [-o output_file]\n", argv[0]);
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

  printf("Scheduling algorithms:\n");
  printf("1. First-come-first-served (FCFS)\n");
  printf("2. Shortest job first (SJF)\n");
  printf("3. Priority scheduling\n");
  printf("Enter the number of the scheduling algorithm to use: ");
  int algorithm;
  scanf("%d", &algorithm);

  FILE *output_file = fopen("output.txt", "w");
  if (output_file == NULL) {
    fprintf(stderr, "Error: Could not open output file.\n");
    exit(1);
  }

  switch (algorithm) {
    case 1:
      fcfs(jobs, num_jobs, output_file);
      break;
    case 2:
      sjf(jobs, num_jobs, preemptive, output_file);
      break;
    case 3:
      priority(jobs, num_jobs);
      break;
    default:
      fprintf(stderr, "Error: Invalid scheduling algorithm.\n");
      exit(1);
  }

  fclose(output_file);
  return 0;
}

