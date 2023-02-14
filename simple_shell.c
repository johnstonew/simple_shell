#include <stdio.h>
#include <stdlib>

int main()
{
  char *command = NULL;
  size_t len = 0;
  ssize_t read;
  
  while(true)
  {
    printf("$ ");
    read = getline(&command, &len, stdin);
    printf("%s", command);
  }
  free(command);
  return (0);
}
