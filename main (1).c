#include "structs.h"
#include "functions.h"

int readFile(char *filename, struct item_node *itemlist);
int printFileData(struct item_node *item);
int printItemData(struct item_node *item);
int cleanUpData(struct item_node *item);
int readItemAttributes(char *filename, struct attribute *attribute);
int printAttributesRAW(struct attribute *attributes, int size);
int cleanUpAttributes(struct general_item item, struct attribute *attributes,int size);

int main() {
  struct item_data *itemList = NULL;
  struct general_item item;
  item.properties = malloc(10 * sizeof(struct item_data));
  // readFile("equipment.json", &itemList);
  // cleanUpData(itemList);
  //  printItemData(itemList->node);
  struct attribute *attributes = malloc(100 * sizeof(struct attribute));
  readItemAttributes("shortsword.json", attributes);
  cleanUpAttributes(item, attributes,31);
  //readItemAttributes("shortsword.json", item, attributes);
  printf("%s\n\n", attributes[25].value);
  printf("testing");
  // printFileData(itemList);
  free(attributes);
  return 0;
}

int readItemAttributes(char *filename,  struct attribute *attributes) {
    FILE *filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        perror("File opening failed");
        return -1;
    }

    char buffer[4098 * 8];
    char *parsing = NULL;
    int item_counter = 1;
    int counter = 1;
    int temp_counter = 0;
    bool open = false;

    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        parsing = buffer;
        char *token = parsing; 

        while ((token = strsep(&parsing, ",")) != NULL) { 
            strsep(&token, "\"");

            char *out1 = strsep(&token, "\"");


            if (strcmp(out1, "equipment_category") == 0) {
                temp_counter = 2;
            } else if (strcmp(out1, "cost") == 0) {
                temp_counter = 1;
            } else if (strcmp(out1, "damage") == 0) {
                temp_counter = 3;
            }

            if(countChar(token, '[') != countChar(token, ']')){
                if(countChar(token, '[') > countChar(token, ']')){
                    open = true;
                } else if(countChar(token, '[') < countChar(token, ']')){
                    open = false;
                }
            }

            if(open){
                temp_counter = 1;
            }

            strsep(&token, ":");
            attributes[item_counter].count = counter;
            attributes[item_counter].name = out1;
            attributes[item_counter].value = token;
            int x= item_counter;
            printf("%i) %i / %i  | %s | %s\n", x, open, attributes[x].count, attributes[x].name,
                attributes[x].value);

            if (temp_counter <= 0) {
                counter++;
            } else {
                temp_counter--;
            }
            item_counter++;
        } 
    }
    fclose(filePointer); 

    return 0;
}

int cleanUpAttributes(struct general_item item, struct attribute *attributes, int size) {
  for (int x = 1; x <= size; x++) {
    printf("%s\n", attributes[x].name);
    if (strcmp(attributes[x].name, "equipment_category") == 0) {
      struct item_data equipment;
      int count = attributes[x].count;
      char *value_copy = strdup(attributes[x].value); 
      
      strsep(&value_copy, "\"");
      strsep(&value_copy, ":");
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      equipment.index = val;
      printf("\t %s\n", val);
      
      int counter = 0;
      int z = 1;
      while(attributes[x + z].count == count){
        counter += 1;
        if (strcmp(attributes[x + z].name, "name") == 0){equipment.name = attributes[x + z].value;}
        if (strcmp(attributes[x + z].name, "url") == 0){equipment.url = attributes[x + z].value;}
        printf("\t %s\n", attributes[x + z].value);
        z += 1;
        }
      item.equipment_category = equipment;
      x += counter;
      //free(value_copy);
    }

    else if (strcmp(attributes[x].name, "cost") == 0) {
      struct value cost;
      int count = attributes[x].count;
      char *value_copy = strdup(attributes[x].value); 

      strsep(&value_copy, "\"");
      strsep(&value_copy, ":");
      cost.quantity = atoi(value_copy);
      printf("\t %i\n", atoi(value_copy));

      int counter = 0;
      int z = 1;
      while(attributes[x + z].count == count){
        counter += 1;
        if (strcmp(attributes[x + z].name, "unit") == 0){cost.unit = attributes[x + z].value;}
        //printf("test123\n");
        printf("\t %s\n", attributes[x + z].value);
        z += 1;
        }
      item.cost = cost;
      x += counter;
      //free(value_copy);
    }

    else if (strcmp(attributes[x].name, "damage") == 0) {
      struct damage damage;
      //printf("test\n");
      int count = attributes[x].count;
      char *value_copy = strdup(attributes[x].value); 

      strsep(&value_copy, "\"");
      strsep(&value_copy, ":");
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");

      printf("\t %s\n", val);
      damage.damage_dice = val;

      int counter = 0;
      int z = 1;
      while(attributes[x + z].count == count){
        counter += 1;
        if (strcmp(attributes[x + z].name, "damage_type") == 0){
          char *value_copy1 = strdup(attributes[x + z].value); 
  
          strsep(&value_copy1, "\"");
          strsep(&value_copy1, ":");
          strsep(&value_copy1, "\"");
          char *val1 = value_copy1;
          val1 =strsep(&val1, "\"");
          damage.damage_type.index = val1;
          //free(value_copy1);
        
        }
        if (strcmp(attributes[x + z].name, "name") == 0){damage.damage_type.name = attributes[x + z].value;}
        if (strcmp(attributes[x + z].name, "url") == 0){damage.damage_type.name = attributes[x + z].value;}
        printf("\t %s\n", attributes[x + z].value);
        z += 1;
      }
      item.damage = damage;
      x += counter;
      //free(value_copy);
      
    }

    else if (strcmp(attributes[x].name, "properties") == 0) {
      struct item_data *properties = malloc(50 * sizeof(struct attribute));
      int count = attributes[x].count;
      char *value_copy = strdup(attributes[x].value); 

      strsep(&value_copy, "\"");
      strsep(&value_copy, ":");
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      //printf("\t %s \n", val);
      properties[0].index = val;

      int counter = 0;
      int item_counter = 0;
      int z = 1;
      while(attributes[x + z].count == count){
        counter += 1;
        if (strcmp(attributes[x + z].name, "index") == 0){properties[item_counter].index = attributes[x + z].value;
          item_counter += 1;
          }
        if (strcmp(attributes[x + z].name, "name") == 0){properties[item_counter].name = attributes[x + z].value;}
        if (strcmp(attributes[x + z].name, "url") == 0){properties[item_counter].url = attributes[x + z].value;}
        printf("\t %s \n", attributes[x + z].value);
        z += 1;
        }
      x += counter;
 

      if (item.properties == NULL) {
          // Handle memory allocation failure
          printf("Memory allocation failed for item.properties\n");
          // Free previously allocated memory
          for (int i = 0; i < 10; i++) {
              free(properties[i].index);
              free(properties[i].name);
              free(properties[i].url);
          }
          free(properties);
          free(value_copy);
          return -1;
      }

      for (int i = 0; i < 2; i++) {
        if(properties[i].index != NULL){
          //printf("%s\n", properties[i].index);
          item.properties[i].index = strdup(properties[i].index);
          item.properties[i].name = strdup(properties[i].name);
          item.properties[i].url = strdup(properties[i].url);
        }
      }
      //printf("%s\n", item.properties[0].index);

      /*for (int i = 0; i < 10; i++) { // should be freed but keep getting invalid pointer error
        free(properties[i].index);
        free(properties[i].name);
        free(properties[i].url);
      }*/
      
      free(properties);
    }

    else if (strcmp(attributes[x].name, "desc") == 0) {
      printf("\t %s\n", attributes[x].value);
      item.desc = attributes[x].value;
    }

    else if (strcmp(attributes[x].name, "special") == 0) {
      printf("\t %s\n", attributes[x].value);
      item.special = attributes[x].value;
    }

    else if (strcmp(attributes[x].name, "index") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.index = val;
    }

    else if (strcmp(attributes[x].name, "name") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.name = val;
    }

    else if (strcmp(attributes[x].name, "weapon_category") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.weapon_category = val;
    }

    else if (strcmp(attributes[x].name, "weapon_range") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.weapon_range = val;
    }

    else if (strcmp(attributes[x].name, "weight") == 0) {
      item.weight = atoi(attributes[x].value);
      printf("\t %s\n", attributes[x].value);
    }

    else if (strcmp(attributes[x].name, "url") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.url = val;
    }

    else if (strcmp(attributes[x].name, "contents") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.contents = val;
    }

    else if (strcmp(attributes[x].name, "range") == 0) {
      struct range range;
      int count = attributes[x].count;
      char *value_copy = strdup(attributes[x].value); 

      strsep(&value_copy, "\"");
      strsep(&value_copy, ":");
      range.normal_range = atoi(value_copy);
      printf("\t %i\n", atoi(value_copy));

      int counter = 0;
      int z = 1;
      while(attributes[x + z].count == count){
        counter += 1;
        if (strcmp(attributes[x + z].name, "long_range") == 0){range.long_range = atoi(attributes[x + z].value);}
        //printf("test123\n");
        printf("\t %s\n", attributes[x + z].value);
        z += 1;
        }
      item.range = range;
      x += counter;
      //free(value_copy);
    }

    else if (strcmp(attributes[x].name, "category_range") == 0) {
      char *value_copy = strdup(attributes[x].value); 
      strsep(&value_copy, "\"");
      char *val = value_copy;
      val =strsep(&val, "\"");
      printf("\t %s\n", val);
      item.category_range = val;
    }
  }
  return 0;
}

int readFile(char *filename, struct item_node *itemlist) {
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

  struct item_node *lastNode = NULL;
  struct item_node *headerNode = NULL;

  while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
    parsing = buffer;
    char *token = parsing; // strsep(&parsing, "{");

    while (token) {
      counter++;
      token = strsep(&parsing, "{");

      headerNode = (struct item *)calloc(1, sizeof(struct item_node));
      headerNode->node = lastNode;
      headerNode->count = counter;

      counter2 = 0;
      parsing2 = token;
      char *token2 = strsep(&parsing2, ",");

      while (token2) {
        counter2++;
        if (counter2 == 1)
          headerNode->data->index = strdup(token2);
        if (counter2 == 2)
          headerNode->data->name = strdup(token2);
        if (counter2 == 3)
          headerNode->data->url = strdup(token2);
        if (counter2 == 4) {
          if (headerNode->data->url) {
            char *temp = strdup(headerNode->data->url);
            free(headerNode->data->url);
            headerNode->data->url =
                (char *)malloc(strlen(temp) + strlen(token2) + 1);
            strcpy(headerNode->data->url, temp);
            strcat(headerNode->data->url, token2);
            free(temp);
          } else {
            headerNode->data->url = strdup(token2);
          }
        }
        token2 = strsep(&parsing2, ",");
      }
      lastNode = headerNode;
    }
    itemlist = headerNode;
  }

  fclose(filePointer);
  return 0;
}

int printFileData(struct item_node *item) {
  while (item != NULL) {
    printf("%i | %s | %s | %s \n", item->count, item->data->index,
           item->data->name, item->data->url);
    item = item->node;
  }
  return 0;
}

int printItemData(struct item_node *item) {
  printf("%i | %s | %s | %s \n", item->count, item->data->index,
         item->data->name, item->data->url);
  return 0;
}

int cleanUpData(struct item_node *item) {
  while (item != NULL) {
    if (item->data->name != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->data->name, "\"");
      char *token = strsep(&item->data->name, "\"");
      item->data->name = token;
    }

    if (item->data->index != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->data->index, "\"");
      char *token = strsep(&item->data->index, "\"");
      item->data->index = token;
    }

    if (item->data->url != NULL) {
      for (int i = 0; i < 3; i++)
        strsep(&item->data->url, "\"");
      char *token = strsep(&item->data->url, "\"");
      item->data->url = token;
    }

    item = item->node;
  }
  return 0;
}
