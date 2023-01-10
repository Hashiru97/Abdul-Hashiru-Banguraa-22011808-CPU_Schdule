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
