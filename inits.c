/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains functions that handle the initialization/assignments of AEDVs and their Orders
*/
#include "dependencies.h"
void AssignNewOrders() {
    int AEDVNum, tempDestX, tempDestY, AEDVIndex;
    printf("Enter AEDV EVIN: ");
    scanf("%d", &AEDVNum);
    printf("You entered %d\n", AEDVNum);
    AEDVIndex = AEDVNum - EVINBASE; // subtract the base value to get the index of the current AEDV
    if(AEDVIndex > maxAEDV-1) {
        printf("Invalid AEDV Number (Out of Bounds \n)");
    }
    else {
        printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[AEDVIndex]->EVIN);
        scanf("%d %d", &tempDestX,&tempDestY);
        if(isValidDestination(tempDestX, tempDestY)) {
            listOfVehicles[AEDVIndex]->destination.x = tempDestX;
            listOfVehicles[AEDVIndex]->destination.y = tempDestY;
            listOfVehicles[AEDVIndex]->currStatus = TRANSIT;
            return;
        }
        else
            printf("Invalid Destination, F again to reorder\n");
    }
    //Waits until all AEDV's are idle before prompting user to give new directions
#ifdef AUTOORDERS
    int count = 0;
    //Check the amount of IDLE AEDV's
    while(listOfVehicles[count]->currStatus==UNLOADING) {
        count++;
    }
    for (int i = 0; count == maxAEDV && i < maxAEDV; ++i) {
        int tempDestX, tempDestY, isValid = 1;
        while(isValid != 0) {
            printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[i]->EVIN);
            scanf("%d %d", &tempDestX,&tempDestY);
            if(isValidDestination(tempDestX, tempDestY)) {
                listOfVehicles[i]->destination.x = tempDestX;
                listOfVehicles[i]->destination.y = tempDestY;
                listOfVehicles[i]->currStatus = TRANSIT;
                isValid = 0;
            }
            else
                printf("Invalid destination\n");
        }
    }
#endif
}
void AEDVInput() {
    int tempCordX, tempCordY,tempDestX, tempDestY;
    do {
        printf("Number of AEDV's (1-4): ");
        scanf("%d", &maxAEDV);
        if(maxAEDV < 1) {
            printf("No AEDV's to assign orders\n");
        }
        if(maxAEDV > 4) {
            printf("Too many AEDV's to assign\n");
        }
    }while(maxAEDV > 4 || maxAEDV < 1);

    for (int i = 0; i < maxAEDV; ++i) {
        do {
            printf("Input starting location of AEDV [1000%d] x, y: ", i);
            scanf("%d %d", &tempCordX, &tempCordY);
            if(tempCordX < 0 || tempCordX > MAX_COLS || tempCordY < 0 || tempCordY > MAX_ROWS) {
                printf("Starting Out of Bounds\n");
            }
        }while(tempCordX < 0 || tempCordX > MAX_COLS || tempCordY < 0 || tempCordY > MAX_ROWS);

        do {
            printf("Input destination of AEDV [1000%d] x, y: ", i);
            scanf("%d %d", &tempDestX, &tempDestY);
            if(!isValidDestination(tempDestX, tempDestY))
                printf("Invalid destination\n");
        } while(!isValidDestination(tempDestX, tempDestY));
#define LLTest2
#ifdef LLTest2
        AddToListBeginning(&ActiveList, tempCordX, tempCordY, tempDestX, tempDestY, i);
#else
        InitAEDV(listOfVehicles[i], tempCordX, tempCordY, tempDestX, tempDestY, i);
#endif

    }
}

void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    vehicle->EVIN = EVINBASE + identifierCode;
    vehicle->drawSize = (Vector2) {cellWidth, cellHeight};
    vehicle->position =  (Cord){locationX, locationY};
    vehicle->destination =  (Cord){destinationX, destinationY};
    vehicle->color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
    vehicle->currStatus = TRANSIT;
}
void SetupInitialConditions() {
    char defaultTest;
    bool validCharacter = false;
    do {
        printf("Default Test? (Y or N):");
        scanf("%c", &defaultTest);
        if(tolower(defaultTest) == 'y') {
            validCharacter = true;
            maxAEDV = 1;
            MAX_COLS = 21;
            MAX_ROWS = 21;
            cellHeight = screenWidth/MAX_COLS;
            cellWidth = screenWidth/MAX_COLS;
            printf("Dimensions of map defaulted to %d columns by %d rows\n", MAX_COLS, MAX_ROWS);
#define LLTest
#ifdef LLTest
            AddToListBeginning(&ActiveList, 0, 0, 10, 12, 0);
#else
            InitAEDV(listOfVehicles[0], 1, 0, 10, 12, 0);
#endif
        }
        else if(tolower(defaultTest) == 'n'){
            validCharacter = true;
            printf("Number of buildings horizontally: ");
            scanf("%d",&MAX_COLS);
            printf("Number of buildings vertically: ");
            scanf("%d",&MAX_ROWS);
            printf("You inputted %d x %d buildings\n", MAX_COLS, MAX_ROWS);
            MAX_COLS = MAX_COLS * 4 + 1;
            MAX_ROWS = MAX_ROWS * 4 + 1;
            printf("Equals %d x %d tiles\n", MAX_COLS, MAX_ROWS);
            cellHeight = screenWidth/MAX_COLS;
            cellWidth = screenWidth/MAX_COLS;
            AEDVInput();

        }
        //Stored In while loop to allow for bad inputs to be rectified
    } while(!validCharacter);
}

void InitRoutine() {
    /*Raylib Setup Settings */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //allows window resizing
    SetTraceLogLevel(LOG_ERROR); //allows for tracelog output for debugging/warnings
    //Camera Settings to show whole map
    camera.zoom = DEFAULTZOOM;
    camera.target = (Vector2) {.x = -DEFAULTOFFSET, .y = -DEFAULTOFFSET};

    int frameTarget = 50; //Amount of ticks/second
    frameCount = 90;
    //Initialization Functions
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map"); //creates the window
    SetTargetFPS(frameTarget);// Set our simulation to run at x frames-per-second
    ReadEventFile("EventFile.txt"); //read event file to populate list at program beginning
}

void GenerationCheck() {
    printf("Do you want to generate a new map file? Y/N :");
    char check;
    fscanf(stdin, "%c", &check);
    if(toupper(check) == 'Y') {
        OpenTargetFile(WRITE_BINARY, "GenerationFile.dat", &FileDescriptor);

        GenerateBuildFile();
    }
    else printf("Resuming operation with old map..");
}