#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib/sala.h"

#define TOKEN_COUNT 8
#define TOKEN_SIZE 32

char tokens[TOKEN_COUNT][TOKEN_SIZE];
int capacidad;

// TODO

int reservar();

int liberar();



// ----

int read_input() {
  char c;
  int i, j;

  // Limpiamos el buffer de entrada
  for (int i = 0; i < TOKEN_COUNT; i++) {
    memset(tokens[i], 0, TOKEN_SIZE);
  }

  // Leemos la entrada del usuario
  i = 0;
  j = 0;
  while( (c = getchar()) != '\n' && c != EOF ) {
    if (i == TOKEN_SIZE) {
      return -1;
    }
    if (c == ' ') {
      j++;
      i = 0;
      if (j == TOKEN_COUNT) {
        return -2;
      }
      continue;
    }
    tokens[j][i++] = c;
  }
  fflush(stdin);
  return 0;
}

void print_help() {
  printf("Comados disponibles:\n");
  printf("\t- help -> Muestra ayuda para usar el shell.\n");
  printf("\t- quit -> Salir del programa.\n");
  printf("\t- clear -> Limpia la pantalla.\n");
}

int main(int argc, char** argv){
  if (argc < 3) return -1;

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

    if (read_input() < 0) {
      printf("Ha ocurrido un error.\nPara ver la lista de comandos válidos use 'help'.\n");
    }
    

    if (strcmp(tokens[0], "quit") == 0) {
      printf("Saliendo del sistema.\n");
      sleep(1);
      break;
    } else if (strcmp(tokens[0], "help") == 0) {
      print_help();
    } else if (strcmp(tokens[0], "clear") == 0) {
      printf("\e[1;1H\e[2J");
    }
  }
  elimina_sala();
  return 0;
}
