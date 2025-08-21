#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_LEN      115 // cpu + 2 spaces + 10 int with a space between them
#define SPEED_FACTOR  42069  // increase for a faster cat
#define HALF_A_SECOND 500000 // in us
#define WAKEUP_LOAD   0.15   // load under this will make the cat sleep


_Atomic volatile double status;

void
get_cpu_status(long *cpu_status)
{
  FILE *stat;
  char line[BUFF_LEN], *line_it;

  line_it = line;

  stat = fopen("/proc/stat", "r");
  if (stat == NULL) exit(1);

  while ((*line_it = fgetc(stat)) && (*line_it != '\n'))
    line_it++;

  *line_it = ' ';
  line_it++;
  *line_it = '\0';
  fclose(stat);

  line_it = line;
  line_it += 5;

  for (int i = 0; i < 10; i++) {
    cpu_status[i] = strtol(line_it, NULL, 10);
    while (*line_it != ' ')
      line_it++;
    line_it++;
  }
}

void *
get_load(void *args)
{
  long cpu_status_t0[10], cpu_status_t1[10], cpu_status_delta[10];
  long sum;

  // T0
  get_cpu_status(cpu_status_t0);

  while (1) {
    usleep(HALF_A_SECOND);
    get_cpu_status(cpu_status_t1);

    // gets the delta load and update T0
    for (int i = 0; i < 10; i++) {
      cpu_status_delta[i] = cpu_status_t1[i] - cpu_status_t0[i];
      cpu_status_t0[i]    = cpu_status_t1[i];
    }

    sum = 0;
    for (int i = 0; i < 10; i++)
      sum += cpu_status_delta[i];

    // evaluate the load percentage
    status = 1 - (double)cpu_status_delta[3] / sum;
  }

  return NULL;
}

// output function
void *
cat(void *args)
{
  while (1) {
    while (status < WAKEUP_LOAD) {
      fflush(stdout);
      puts("");
      usleep(HALF_A_SECOND);
    }
    fflush(stdout);
    puts("");
    usleep(SPEED_FACTOR / status);
    while (status < WAKEUP_LOAD) {
      fflush(stdout);
      puts("");
      usleep(HALF_A_SECOND);
    }
    fflush(stdout);
    puts("");
    usleep(SPEED_FACTOR / status);
    while (status < WAKEUP_LOAD) {
      fflush(stdout);
      puts("");
      usleep(HALF_A_SECOND);
    }
    fflush(stdout);
    puts("");
    usleep(SPEED_FACTOR / status);
    while (status < WAKEUP_LOAD) {
      fflush(stdout);
      puts("");
      usleep(HALF_A_SECOND);
    }
    fflush(stdout);
    puts("");
    usleep(SPEED_FACTOR / status);
    while (status < WAKEUP_LOAD) {
      fflush(stdout);
      puts("");
      usleep(HALF_A_SECOND);
    }
    fflush(stdout);
    puts("");
    usleep(SPEED_FACTOR / status);
  }
  return NULL;
}

int
main(int argc, char *argv[])
{
  pthread_t get_load_thread, cat_thread;

  // while I wait for the thread to start
  puts("");

  // Create get load
  pthread_create(&get_load_thread, NULL, get_load, NULL);

  sleep(1);

  // Create cat
  pthread_create(&cat_thread, NULL, cat, NULL);

  // If it crash the program finish
  pthread_join(get_load_thread, NULL);
  return 1;
}
