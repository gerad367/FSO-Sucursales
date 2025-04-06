#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void handler() {
  printf("\nSe ha cerrado una sala.\n");
}

void crea_sucursal(const char* ciudad, int capacidad) {
  char buff_cap[8];
  char buff_pid[8];
  pid_t pid;
  memset(buff_cap, 0, 8);
  memset(buff_pid, 0, 8);
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
  char buff[128];
  int status, capacidad;

  signal(SIGUSR1, handler);

  printf("Bienvenido al sistema de gestión de sucursales.\n");
  printf("Si quiere crear una nueva sucursal, introduzca el nombre de esta.\n");
  printf("Si lo que desea es salir del programa, escriba 'Salir'.\n");
  
  while (1) {
    memset(buff, 0, 128);
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
