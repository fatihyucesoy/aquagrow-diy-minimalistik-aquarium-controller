#ifndef structs_h
#define structs_h


#include <WString.h>

typedef struct {
  int time;  //34200; 
  int level;  
}LIGHT;

typedef struct {
  int time;  //34200; 
  boolean state;  
}RELAY;

typedef struct {
  int time;  //34200; 
  int pushed;  
  boolean status;   /*Pin Status*/
}FEED;

typedef struct {
    int time;  //34200;    // 9:30
    boolean active;
    String name;
    int pinAddr;
    boolean status;   /*Pin Status*/
    boolean dim_direction;
    int mlperminute;    /*Milliliter per minute, depending on the pump*/
    int mldosing;       /*how much ml should be produced per dosing*/
}PUMP;


#endif
