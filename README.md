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

