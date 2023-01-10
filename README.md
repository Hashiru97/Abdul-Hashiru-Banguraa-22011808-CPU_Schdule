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
