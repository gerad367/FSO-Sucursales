#include "sala.h"
#include <stdlib.h>

struct Sala {
  int capacidad;
  int ocupados;
  int* id_personas;
};

static struct Sala sala = {0, 0};

int reserva_asiento(int id_persona){
  if (sala.capacidad == 0 || id_persona <= 0) return -1; // No existe la sala
  for (int i = 0; i < sala.capacidad; i++) {
    if (sala.id_personas[i] == 0) {
      sala.id_personas[i] = id_persona;
      sala.ocupados++;
      return ++i; // Se devuelve el id del asiento seleccionado
    }
  }
  return -1; // No hay asientos libres
}

int libera_asiento(int id_asiento){
  // No existe la sala, o el id del asiento es inválido
  if (sala.capacidad == 0 || id_asiento <= 0 || id_asiento > sala.capacidad) return -1;
  int id = sala.id_personas[id_asiento - 1];
  if (id <= 0) return -1;
  sala.id_personas[id_asiento - 1] = 0;
  sala.ocupados--;
  return id;
}

int estado_asiento(int id_asiento){
  // No existe la sala, o el id del asiento es inválido
  if (sala.capacidad == 0 || id_asiento <= 0 || id_asiento > sala.capacidad) return -1;
  return sala.id_personas[id_asiento - 1];
}

int asientos_libres(){
  // No existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.capacidad - sala.ocupados;
}

int asientos_ocupados(){
  // No existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.ocupados;
}

int capacidad_sala(){
  // no existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.capacidad;
}

int crea_sala(int capacidad){
  if (sala.capacidad != 0 || capacidad <= 0) return -1; // Ya existe una sala, se debería eliminar la anterior antes de crear otra
  sala.id_personas = malloc(capacidad * sizeof(int));
  if (sala.id_personas == NULL) return -1; // No se ha podido reservar memoria para la sala
  sala.capacidad = capacidad;
  sala.ocupados = 0;
  for (int i = 0; i < capacidad; i++) sala.id_personas[i] = 0;
  return capacidad;
}

int elimina_sala(){
  if (sala.capacidad == 0) return -1; // No existe sala que eliminar
  sala.capacidad = 0;
  sala.ocupados = 0;
  free(sala.id_personas);
  return 0;
}
