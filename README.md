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

