#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib/sala.h"

#define INPUTSIZE 16

void read_input(char* buffer) {
  char c;
  int i = 0;
  memset(buffer, 0, INPUTSIZE);
  while( (c = getchar()) != '\n' && c != EOF ) {
    if (i == INPUTSIZE) {
      break;
    }
    buffer[i++] = c;
  }
}

void print_help() {
  printf("Comados disponibles:\n");
  printf("\t- help -> Muestra ayuda para usar el shell.\n");
  printf("\t- quit -> Salir del programa.\n");
  printf("\t- clear -> Limpia la pantalla.\n");
}

int main(int argc, char** argv){
  if (argc < 3) return -1;
  char input[INPUTSIZE];
  int capacidad;

  capacidad = atoi(argv[2]);
  if (capacidad <= 0) {
    printf("El valor de la capacidad no es válido.\n");
    sleep(2);
    return -1;
  }

  if (crea_sala(capacidad) == -1) {
    printf("Ha ocurrido un error al crear la sala.\n");
    sleep(2);
    return -1;
  }

  printf("Bienvenido a la shell interactiva de gestión de salas.\n");
  printf("Para mostrar ayuda introduzca 'help'.\n\n");
  
  while (1) {
    printf("[mini-shell@%s]$ -> ", argv[1]);
    read_input(input);

    if (strcmp(input, "quit") == 0) {
      printf("Saliendo del sistema.\n");
      sleep(1);
      break;
    } else if (strcmp(input, "help") == 0) {
      print_help();
    } else if (strcmp(input, "clear") == 0) {
      printf("\e[1;1H\e[2J");
    }
  }
  elimina_sala();
  return 0;
}
