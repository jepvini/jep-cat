#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_LEN     115
#define SPEED_FACTOR 42069

typedef _Atomic(double) atomic_double;

volatile atomic_double status;

long *
get_cpu_status(long *cpu_status)
{
  FILE *stat;
  char line[BUFF_LEN], *line_it;

  line_it = line;

  stat = fopen("/proc/stat", "r");
  if (stat == NULL) exit(1);

  while ((*line_it = fgetc(stat)) && (*line_it != '\n')) {
    line_it++;
  }
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

  return cpu_status;
}

void *
get_load(void *args)
{
  long cpu_status_t0[10], cpu_status_t1[10], cpu_status_delta[10];
  long *buffer, sum;

  buffer = cpu_status_t0;
  buffer = get_cpu_status(cpu_status_t0);

  while (1) {
    sleep(1);
    buffer = cpu_status_t1;
    buffer = get_cpu_status(cpu_status_t1);

    for (int i = 0; i < 10; i++) {
      cpu_status_delta[i] = cpu_status_t1[i] - cpu_status_t0[i];
      cpu_status_t0[i]    = cpu_status_t1[i];
    }

    sum = 0;
    for (int i = 0; i < 10; i++)
      sum += cpu_status_delta[i];

    status = 1 - (double)cpu_status_delta[3] / sum;

    // printf("%f\n", status);
  }

  return NULL;
}

void *
cat(void *args)
{
  while (1) {
    while (status < 0.15) {
      fflush(stdout);
      printf("\n");
      usleep(500000);
    }
    fflush(stdout);
    printf("\n");
    usleep(SPEED_FACTOR / status);
    while (status < 0.15) {
      fflush(stdout);
      printf("\n");
      usleep(500000);
    }
    fflush(stdout);
    printf("\n");
    usleep(SPEED_FACTOR / status);
    while (status < 0.15) {
      fflush(stdout);
      printf("\n");
      usleep(500000);
    }
    fflush(stdout);
    printf("\n");
    usleep(SPEED_FACTOR / status);
    while (status < 0.15) {
      fflush(stdout);
      printf("\n");
      usleep(500000);
    }
    fflush(stdout);
    printf("\n");
    usleep(SPEED_FACTOR / status);
    while (status < 0.15) {
      fflush(stdout);
      printf("\n");
      usleep(500000);
    }
    fflush(stdout);
    printf("\n");
    usleep(SPEED_FACTOR / status);
  }
  return NULL;
}

int
main(int argc, char *argv[])
{
  pthread_t get_load_thread, cat_thread;

  printf("\n");
  // Creating a new thread
  pthread_create(&get_load_thread, NULL, get_load, NULL);
  sleep(2);
  pthread_create(&cat_thread, NULL, cat, NULL);

  pthread_join(get_load_thread, NULL);
  return EXIT_SUCCESS;
}
