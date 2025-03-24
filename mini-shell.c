#include <stdio.h>

int main(int argc, char** argv){
  printf("El programa ha recibido %d parámetros\n %s\n", argc, argv[0]);
  if (argc > 1)
    printf("%s\n", argv[1]);
  if (argc > 2)
    printf("%s\n", argv[2]);
  int num;
  scanf("%d", &num);
  return 0;
}
