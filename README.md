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
