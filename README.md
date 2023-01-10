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