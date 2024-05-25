#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readFile(char *filename, struct item **itemlist);
int printFileData(struct item *item);
int printItemData(struct item *item);
int cleanUpData(struct item *item);
int readItemAttributes(char *filename, struct general_item item);

int main() {
  struct item *itemList = NULL;
  struct general_item item;
  // readFile("equipment.json", &itemList);
  // cleanUpData(itemList);
  //  printItemData(itemList->node);
  readItemAttributes("shortsword.json", item);
  // printFileData(itemList);

  return 0;
}

int readItemAttributes(char *filename, struct general_item item) {
  FILE *filePointer = fopen(filename, "r");
  if (filePointer == NULL) {
    perror("File opening failed");
    return -1;
  }

  char buffer[4098 * 8];
  char *parsing = NULL;
  int counter = 1;

  while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
    parsing = buffer;
    char *token =
        parsing; // strsep(&parsing, ","); //to not skip the first attribute

    while (token) {
      counter++;
      token = strsep(&parsing, ",");
      printf("%i) %s\n", counter, token);
    }

    fclose(filePointer);
    return 0;
  }
}

int readFile(char *filename, struct item **itemlist) {
  FILE *filePointer = fopen(filename, "r");
  if (filePointer == NULL) {
    perror("File opening failed");
    return -1;
  }

  char buffer[4098 * 8];
  char *parsing = NULL;
  char *parsing2 = NULL;
  int counter = 1;
  int counter2 = 1;

  struct item *lastNode = NULL;
  struct item *headerNode = NULL;

  while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
    parsing = buffer;
    char *token = parsing; // strsep(&parsing, "{");

    while (token) {
      counter++;
      token = strsep(&parsing, "{");

      headerNode = (struct item *)calloc(1, sizeof(struct item));
      headerNode->node = lastNode;
      headerNode->count = counter;

      counter2 = 0;
      parsing2 = token;
      char *token2 = strsep(&parsing2, ",");

      while (token2) {
        counter2++;
        if (counter2 == 1)
          headerNode->index = strdup(token2);
        if (counter2 == 2)
          headerNode->name = strdup(token2);
        if (counter2 == 3)
          headerNode->url = strdup(token2);
        if (counter2 == 4) {
          if (headerNode->url) {
            char *temp = strdup(headerNode->url);
            free(headerNode->url);
            headerNode->url = (char *)malloc(strlen(temp) + strlen(token2) + 1);
            strcpy(headerNode->url, temp);
            strcat(headerNode->url, token2);
            free(temp);
          } else {
            headerNode->url = strdup(token2);
          }
        }
        token2 = strsep(&parsing2, ",");
      }
      lastNode = headerNode;
    }
    *itemlist = headerNode;
  }

  fclose(filePointer);
  return 0;
}

int printFileData(struct item *item) {
  while (item != NULL) {
    printf("%i | %s | %s | %s \n", item->count, item->index, item->name,
           item->url);
    item = item->node;
  }
  return 0;
}

int printItemData(struct item *item) {
  printf("%i | %s | %s | %s \n", item->count, item->index, item->name,
         item->url);
  return 0;
}

int cleanUpData(struct item *item) {
  while (item != NULL) {
    if (item->name != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->name, "\"");
      char *token = strsep(&item->name, "\"");
      item->name = token;
    }

    if (item->index != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->index, "\"");
      char *token = strsep(&item->index, "\"");
      item->index = token;
    }

    if (item->url != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->url, "\"");
      char *token = strsep(&item->url, "\"");
      item->url = token;
    }

    item = item->node;
  }
  return 0;
}
