#include "kernel/types.h"
#include "user/user.h"

void prime_proc(int parent_fd) {
  int p[2];
  pipe(p);
  int c_start;
  if ((read(parent_fd, &c_start, sizeof(int)) == 0)) {
    close(parent_fd);
    return;
  }
  printf("prime %d\n", c_start);

  int pid = fork();
  if (pid == 0) {
    close(p[1]);
    prime_proc(p[0]);
  } else if (pid > 0) {
    close(p[0]);
    int val;
    while (read(parent_fd, &val, sizeof(int)) != 0) {
      if ((val % c_start) != 0) {
        write(p[1], &val, sizeof(int));
      }
    }

    close(parent_fd);
    close(p[1]);
    wait((int*) 0);
  } else {
    fprintf(2, "fork failed\n");
    exit(1);
  }
}

int main(int argc, char *argv) {
  int p[2];
  pipe(p);
  for (int i=2; i <= 35; i++) {
    write(p[1], &i, sizeof(int));
  }
  close(p[1]);
  prime_proc(p[0]);
  exit(0);
}
