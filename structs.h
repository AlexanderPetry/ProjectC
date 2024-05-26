#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct attribute {
  int count;
  char *name;
  char *value;
};

struct item_data {
  char *name;
  char *index;
  char *url;
};

struct item_node {
  int count;
  struct item_data *data;
  struct item_node *node;
};

struct value {
  int quantity;
  char *unit;
};

struct damage {
  char *damage_dice;
  struct item_data damage_type;
};

struct range {
  int normal_range;
  int long_range; // supposed to be just long, but int long; doesnt work
};

struct armor_class {
  int base;
  bool dex_bonus;
};

struct general_item {
  char *desc;
  char *special;
  char *index;
  char *name;
  struct item_data equipment_category;

  // for armor
  char *armor_category;
  struct armor_class armor_class;
  int str_minimum;
  bool stealth_disadvantage;

  // for weapons
  char *weapon_category;
  char *weapon_range;
  char *category_range;
  struct damage damage;
  struct range range;

  // for vehicles
  char *vehicle_category;

  // for gear
  struct item_data gear_category;

  struct value cost;
  int weight;
  struct item_data *properties;
  char *url;

  struct general_item *contents;
};