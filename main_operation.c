/*******************************************************************************************
*  Author: Wyatt Shaw & Cameron Archibald
*  Date: October 31 2023
*  Module Info: The following module implements 'dumb' AEDV movement, allowing up to four AEDVs to navigate to given destination points
*  Dependency Info: In order to handle graphical output, the program utilizes the open source library Raylib.h
*  https://github.com/raysan5/raylib
*  Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "dependencies.h"
#include "typedefs.h"
#include "one_way.h"

/* Global Variables Definition */
//File Pointers
FILE* FileDescriptor;
FILE* RelCustomerFileDescriptor;

//Screen definitions
const int screenWidth = 1000;
const int screenHeight = 1000;
Camera2D camera = { 0 };
//Int Vars
int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight, frameCount;

//Aedv's REMOVE
static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;

//AEDV Lists
AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};
Node * ActiveList = NULL;
Node * InactiveList = NULL;
//Files Lists
EventNode * EventList = NULL;
OrderNode * OrderList = NULL;
//Map
Tile **dynamicMap;
queue * notVisitedQueue;
queue * visitedQueue;

// Main Entry Point
int main() {

    GenerationCheck(); //check whether user wants to create a new map file or not
    InitRoutine(); // Run all initialization functions

    queueSetup(&notVisitedQueue);
    queueSetup(&visitedQueue);

    EventNode *current = EventList; //create a pointer to linked list root
    int eventTime = current->eventData.time; //get time of first event
    // Main simulation loop
    while (!WindowShouldClose())   // Detect window close button or ESC key
    {
        if(frameCount == eventTime) eventTime = EventHandler(eventTime, &current);

        CameraControl();
        UpdateDrawFrame();
    }
    // De-Initialization
    // Close window and OpenGL context
    CloseWindow();
    //free rows, then free cols
    for (int i = 0; i < MAX_ROWS; ++i) {
        free(dynamicMap[i]);
    }
    free(dynamicMap);
    return 0;
}