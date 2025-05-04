#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LEN_NAME 128
#define MAX_SIZE_BUFF 8

void handler(int sig, siginfo_t *t, void *v) {
  int fd = open("fifo", O_RDONLY);
  if (fd == -1) {
    perror("Error al abrir el FIFO");
    exit(-1);
  }
  char* name = malloc(MAX_LEN_NAME);
  if (name == NULL) {
    perror("Error al reservar memoria dinámica");
    exit(-1);
  }
  if (read(fd, name, MAX_LEN_NAME) == -1) {
    perror("Error al leer el nombre de la sala");
    exit(-1);
  }
  close(fd);
  printf("Se ha cerrado la sala %s.\n", name);
  if (t->si_value.sival_int == 0) {
    printf("La sala estaba llena.\n");
  } else {
    printf("la sala no estaba llena.\n");
  }
}

void crea_sucursal(const char* ciudad, int capacidad) {
  char buff_cap[MAX_SIZE_BUFF];
  char buff_pid[MAX_SIZE_BUFF];
  pid_t pid;
  memset(buff_cap, 0, MAX_SIZE_BUFF);
  memset(buff_pid, 0, MAX_SIZE_BUFF);
  // Comprobamos los parámetros de la sucursal
  if (capacidad <= 0 || sprintf(buff_cap, "%d", capacidad) < 0) {
    printf("Valor de capacidad inválido.\n");
    return;
  }
  if (ciudad == NULL) {
    printf("Nombre inválido.\n");
    return;
  }
  sprintf(buff_pid, "%d", getpid());
  
  // printf("Intentando crear la sucursal.\n");
  // Creamos la sucursal
  pid = fork();
  
  if (pid < 0) {
    printf("Ha ocurrido un error al lanzar un nuevo proceso.\n");
  }
  
  if (pid != 0) return;
  
  if(execlp("gnome-terminal", "gnome-terminal", "--wait", "--", "./mshell", ciudad, buff_cap, buff_pid, NULL)  -1) {
    printf("Ha ocurrido un error al ejecutar la sucursal.\n");
  }
}

int main() {
  char buff[MAX_LEN_NAME];
  int status, capacidad;

  mkfifo("fifo", 0777);

  struct sigaction sa;
  sa.sa_flags = SA_RESTART | SA_SIGINFO;
  sa.sa_sigaction = handler;
  sigaction(SIGUSR1, &sa, NULL);

  printf("Bienvenido al sistema de gestión de sucursales.\n");
  printf("Si quiere crear una nueva sucursal, introduzca el nombre de esta.\n");
  printf("Si lo que desea es salir del programa, escriba 'Salir'.\n");
  
  while (1) {
    memset(buff, 0, MAX_LEN_NAME);
    printf("\nNombre de la sucursal: ");
    scanf("%[a-zA-Z ]", buff);

    if (strcmp(buff, "Salir") == 0) {
      printf("Cerrando el sistema, el programa se apagará cuando se cierren las pestañas de las sucursales.\n");
      break;
    }

    printf("Introduzca la capacidad de la sucursal: ");
    if (scanf("%d", &capacidad) == EOF) {
      printf("Error al convertir el número.");
      continue;
    }
    getchar();

    if (capacidad <= 0) {
      printf("La capacidad indicada no es válida.\n");
      sleep(1);
      continue;
    }
    
    crea_sucursal(buff, capacidad);
    sleep(1);
  }
  while (wait(&status) > 0) {}

  return 0;
}
