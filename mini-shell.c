#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "lib/sala.h"

#define TOKEN_COUNT 8
#define TOKEN_SIZE 32

char tokens[TOKEN_COUNT][TOKEN_SIZE];
int capacidad;
int id, id_asiento;

// TODO

// Reserva <id-persona>

// Libera <id-persona>
int liberar(int id_persona);

// Estado-asiento <id-persona>

// Estado-sala

// ----

void print_estado() {
  if (strcmp(tokens[1], "sala") == 0) {
    printf("La sala tiene %d asientos.\n", capacidad);
    printf("De los cuales %d están ocupados.\n", asientos_ocupados());
    
  } else if (strcmp(tokens[1], "asiento") == 0) {
    if (tokens[2][0] == 0) {
      printf("El formato del comando es: estado asiento id_asiento\n");
      return;
    }
    id_asiento = atoi(tokens[2]);
    if (id_asiento <= 0 || id_asiento > capacidad) {
      printf("ID del asiento inválido.\n");
      return;
    }
    id = estado_asiento(id_asiento);
    if (id == 0) {
      printf("El asiento está libre.\n");
    } else {
      printf("El asiento está ocupado por el usuario con id %d.\n", id);
    }
  } else {
    printf("El formato del comando es:\n\testado sala\n\testado asiento id_asiento\n");
  }
}

int liberar(int id_persona) {
  for (int i = 1; i <= capacidad; i++) {
    if (estado_asiento(i) == id_persona) {
      libera_asiento(i);
      return i;
    }
  }
  return -1;
}

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
    if (i == TOKEN_SIZE - 1) {
      while( (c = getchar()) != '\n' && c != EOF ) {}
      return -1;
    }
    if (c == ' ') {
      j++;
      i = 0;
      if (j == TOKEN_COUNT) {
        while( (c = getchar()) != '\n' && c != EOF ) {}
        return -1;
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
  printf("\t- reservar id_persona -> Reserva, si se puede, un asiento para la persona con el id indicado.\n");
  printf("\t- estado sala -> Muestra información sobre el estado de la sala.\n");
  printf("\t- estado asiento id_asiento -> Muestra información sobre el estado del asiento indicado.\n");
  printf("\t- liberar id_persona -> Libera el primer asiento encontrado ocupado por la persona indicada.\n");
}

int main(int argc, char** argv){
  if (argc < 4) return -1;

  int ppid = atoi(argv[3]);
  
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
      continue;
    }

    if (strcmp(tokens[0], "quit") == 0) {
      printf("Saliendo del sistema.\n");
      sleep(1);
      break;
    } else if (strcmp(tokens[0], "help") == 0) {
      print_help();
    } else if (strcmp(tokens[0], "clear") == 0) {
      printf("\e[1;1H\e[2J");
    } else if (strcmp(tokens[0], "reservar") == 0) {
      id = atoi(tokens[1]);
      if (id <= 0) {
        printf("El formato del comando es: reservar <int id>\n");
        continue;
      }
      if ((id_asiento = reserva_asiento(id)) == -1) {
        printf("Ha ocurrido un error en la reserva.\n");
        continue;
      }
      printf("Se ha reservado el asiento %d al usuario con ID %d.\n", id_asiento, id);
    } else if (strcmp(tokens[0], "estado") == 0) {
      print_estado();
    } else if (strcmp(tokens[0], "liberar") == 0) {
      if (tokens[1][0] == 0 || (id = atoi(tokens[1])) == 0) {
        printf("El formato del comando es: liberar id_persona\n");
        continue;
      }
      if ((id_asiento = liberar(id)) == -1) {
        printf("No se ha liberado ningún asiento.\n");
      } else {
        printf("Se ha liberado el asiento %d.\n", id_asiento);
      }
    } else {
      printf("Comando no reconocido.\n");
    }
  }

  union sigval val;
  val.sival_int = (asientos_ocupados() == capacidad) ? 0 : 1;
  sigqueue(ppid, SIGUSR1, val);
  
  sleep(1);
  elimina_sala();
  return 0;
}
