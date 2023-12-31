/*
 * Created By: Wyatt Shaw and Cameron Archibald
 * Date: October 31st 2023
 * Module Info : Contains required libraries as well as function prototypes
 *
 */

#ifndef EXAMPLE_DEPENDENCIES_H
#define EXAMPLE_DEPENDENCIES_H

#define TWOWAY //Allows for two-way navigation
//#define DEBUG //Allows for diagnostic printing
//#define OLD //Functions from one-way experimenting
//#define OLD1 //Fixed readability and efficiency by Cameron on 31st of October

/* C Headers */
#include "stdio.h" // included for regular c functionality printf, scanf etc
#include "stdlib.h"
#include <malloc.h> // included for dynamic allocation using malloc
#include <ctype.h> // included for tolower
#include "stdbool.h"
#include <string.h>
#include <io.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */
/* Raylib Headers */
#include "raylib.h" // included for graphical output
#include "raymath.h" // required by raylib.h
#include "rlgl.h" //required by raylib.h

/* Project Headers */
#include "typedefs.h" // included for function prototypes
#include "drawing.h" // included for functions that change the window
#include "navigations.h" // inlcuded for functions that relate to the movement and navigation of AEDVs
#include "inits.h" // included for functions that run at startup
#include "linkedlist.h"
#include "files.h"
#include "handlers.h"

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
    //others
    #define EVINBASE 10000
    #define CUSTOMERBASE 1000
    #define DELIVERYBASE 500
    #define NOMOVEMENT 0
    #define MAXSTRLEN 100
    #define FILEOK 1
    #define FILE_ERROR -1

/* Macros */
#define TRUNCATE(name)	name[strlen(name)-1] = '\0'


/* Global Externs */
extern int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight, frameCount;
extern Tile** dynamicMap;
extern AEDV *listOfVehicles[4];
extern const int screenWidth;
extern const int screenHeight;
extern Camera2D camera;
extern Node * ActiveList;
extern Node * InactiveList;
extern EventNode * EventList;
extern OrderNode * OrderList;


extern FILE* RelCustomerFileDescriptor;
extern FILE* FileDescriptor;

extern TileNode * queueFront;
extern TileNode * queueRear;
extern queue * notVisitedQueue;
extern queue * visitedQueue;





#endif //EXAMPLE_DEPENDENCIES_H
