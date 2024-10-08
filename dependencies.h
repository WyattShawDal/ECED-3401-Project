
/*
 * Created By: Wyatt Shaw and Cameron Archibald
 * Date: October 31st 2023
 * Module Info : Contains required libraries as well as function prototypes
 *
 */

#ifndef EXAMPLE_DEPENDENCIES_H
#define EXAMPLE_DEPENDENCIES_H

/* C Headers */
#include "stdio.h" // included for regular c functionality printf, scanf etc
#include "stdlib.h"
#include <malloc.h> // included for dynamic allocation using malloc
#include <ctype.h> // included for tolower
#include "stdbool.h"
#include <string.h>
/* Raylib Headers */
#include "raylib.h" // included for graphical output
#include "raymath.h" // required by raylib.h
#include "rlgl.h" //required by raylib.h

/* Project Headers */
#include "typedefs.h" // included for function prototypes
#include "map_operation.h" // included for functions that change the window
#include "navigations.h" // inlcuded for functions that relate to the movement and navigation of AEDVs
#include "inits.h" // included for functions that run at startup
#include "linkedlist.h"
#include "files.h"
#include "handlers.h"
#include "essentials.h"
#include "one_way.h"

/* Magic Number Defines */
    //directions
    #define SOUTH 0
    #define NORTH 1
    #define EAST 2
    #define WEST 3
    //default settings
    #define DEFAULTZOOM 0.94f
    #define DEFAULTOFFSET 45
    #define DEFAULTFONTSIZE 20

    #define frameRate 10 // Ticks/second
    #define delaySecsPerFloor 25 //15 seconds up, 10 seconds down
    #define delayScale 200 // divide by this number to scale the delay

    //others
    #define EVINBASE 10000
    #define EVINMAX 10050
    #define CUSTOMERBASE 1000
    #define DELIVERYBASE 500
    #define NOMOVEMENT 0
    #define MAXSTRLEN 100
    #define TILESHIFT 2
    #define BLOCKSIZE 4
    #define FONTSCALING 1.426
    #define DELIVERYNAMELEN 15




/* Global Externs */
extern int MAX_COLS, MAX_ROWS, cellWidth, cellHeight, frameCount;
extern const int screenWidth;
extern const int screenHeight;

extern Tile** dynamicMap;

extern Camera2D camera;

extern AEDVNode * ActiveList;
extern AEDVNode * InactiveList;
extern EventNode * EventList;
extern OrderNode * OrderList;
extern BuildingNode * StableList;
extern BuildingNode * ChargerList;

extern queue * notVisitedQueue;
extern queue * visitedQueue;

extern FILE* RelCustomerFileDescriptor;
extern FILE* BuildFileDescriptor;
extern FILE* DeliveryFileDescriptor;
extern FILE* LastDeliveryDescriptor;
extern FILE* VehicleFileDescriptor;




#endif //EXAMPLE_DEPENDENCIES_H
