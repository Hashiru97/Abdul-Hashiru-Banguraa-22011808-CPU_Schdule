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

 
