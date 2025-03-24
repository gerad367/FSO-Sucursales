#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void crea_sucursal(const char* ciudad, int capacidad) {
  char buff[8];
  pid_t pid;
  memset(buff, 0, 8);
  // Comprobamos los parámetros de la sucursal
  if (capacidad <= 0 || sprintf(buff, "%d", capacidad) < 0) {
    printf("Valor de capacidad inválido.\n");
    return;
  }
  if (ciudad == NULL) {
    printf("Nombre inválido.\n");
    return;
  }
  
  // printf("Intentando crear la sucursal.\n");
  // Creamos la sucursal
  pid = fork();
  
  if (pid < 0) {
    printf("Ha ocurrido un error al lanzar un nuevo proceso.\n");
  }
  
  if (pid != 0) return;
  
  if(execlp("gnome-terminal", "gnome-terminal", "--wait", "--", "./mshell", ciudad, buff, NULL) == -1) {
    printf("Ha ocurrido un error al ejecutar la sucursal.\n");
  }
}

int main() {
  char buff[128];
  int status, capacidad;

  printf("Bienvenido al sistema de gestión de sucursales.");
  printf("Si quiere crear una nueva sucursal, introduzca el nombre de esta.\n");
  printf("Si lo que desea es salir del programa, escriba 'Salir'.\n");
  
  while (1) {
    memset(buff, 0, 128);
    scanf("%[a-zA-Z ]", buff);

    if (strcmp(buff, "Salir") == 0) {
      printf("Cerrando el sistema, el programa se apagará cuando se cierren las pestañas de las sucursales.\n");
      break;
    }

    printf("Introduzca la capacidad de la sucursal: ");
    scanf("%d", &capacidad);
    
    crea_sucursal(buff, capacidad);
  }
  if (wait(&status) == -1) {
    printf("Ha ocurrido un error con la sucursal\n");
  }

  return 0;
}
