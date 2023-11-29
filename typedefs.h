/*
 * Created By: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains definition of Enum and Structs
 */
#include "dependencies.h"

#ifndef EXAMPLE_STRUCTS_H
#define EXAMPLE_STRUCTS_H

/* Included at top so all typedefs can use definition easily*/
typedef struct Coordinate {
    int x;
    int y;
} Cord;

/* Enums */
typedef enum Type {
    STREET = 0,
    STREET_E,
    STREET_W,
    AVENUE,
    AVENUE_N,
    AVENUE_S,
    JUNCTION,
    BUILDING,
#ifdef TASK6
    STABLE,
    CHARGER,
    BOTH,
    CONSTRUCTION,
    ACCIDENT
#endif
}TILE_TYPE;

typedef enum Status {
    IDLE,
    PICKUP,
    TRANSIT,
    DROPOFF,
    LOADING,
    UNLOADING,
    RECHARGING,
}AEDV_STATUS;

typedef enum Visit {
    NO,
    YES
}VISITED;
typedef enum OP {READ_BINARY, WRITE_BINARY, READ_TEXT, WRITE_TEXT} OPERATION;
typedef enum BLDG_TYPE { CHG = 0, STB, BOTH, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD { N, NE, NW, S, SE, SW, E, W, INVALID_QUAD }QUADRANT_TYPE;

/* Structs */
typedef struct Tile {
    Cord location;
    bool isOccupied;
    TILE_TYPE Type;
    bool validDirection[4]; //[SOUTH,NORTH,EAST,WEST] (typdef in dependencies.h).
}Tile;

typedef struct order {
    Cord pickUp;
    Cord dropOff;
    int pickupFloor;
    int dropFloor;
}OrderData;

typedef struct event {
    char type;
    int time;
    int originID;
    int destinationID;
    int weight;
}EventData;
/* Need to add +1 to listed length */
#define FIRSTNAMELEN 11
#define LASTNAMELEN 16
#define BUILDINGLEN 3
typedef struct {
    int custNum;
    char firstName[FIRSTNAMELEN];
    char lastName[LASTNAMELEN];
    char building[BUILDINGLEN];
    char entrance[BUILDINGLEN];
    int floor;
}Customer;

typedef struct bldg {
    Cord location;
    BUILDING_TYPE type;
    QUADRANT_TYPE quad;
}BuildingData;

typedef struct EventNode {
    EventData eventData;
    struct EventNode *nextEvent;
}EventNode;

typedef struct OrderNode {
    OrderData data;
    struct OrderNode *nextOrder;
}OrderNode;

typedef struct TileNode {
    struct TileNode *parent;
    struct TileNode *queuePrev;
    struct TileNode *visitedPrev;
    Cord coordinate;
}TileNode;

typedef struct InstructionNode {
    struct InstructionNode *child;
    Cord nextPosition;
}InstructionNode;

typedef struct queue {
    TileNode * front;
    TileNode * rear;
}queue;

typedef struct AEDV {
    int EVIN;
    int distanceTravelled;
    int loadingDelay;
    int unloadingDelay;
    Cord position;
    Cord pickUp;
    Cord dropOff;
    Vector2 drawSize;
    Color color;
    AEDV_STATUS currStatus;
    InstructionNode * nextMove;
}AEDV;

typedef struct Node {
    AEDV data;
    struct Node *next;
}AEDVNode;

#endif //EXAMPLE_STRUCTS_H
