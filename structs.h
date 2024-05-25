struct item {
  int count;
  char *name;
  char *index;
  char *url;
  struct item *node;
};

struct general_item {
  char *desc;
  char *special;
  char *index;
  char *name;
  int weight;
};