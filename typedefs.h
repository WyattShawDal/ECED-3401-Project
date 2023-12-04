/*
 * Created By: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains definition of Enum and Structs
 */
#include "dependencies.h"

#ifndef EXAMPLE_STRUCTS_H
#define EXAMPLE_STRUCTS_H
#define FIRSTNAMELEN 11
#define LASTNAMELEN 16
#define BUILDINGLEN 3
#define MAX_ORDER_COUNT 5


/* Included at top so all typedefs can use definition easily*/
typedef struct Coordinate {
    int x;
    int y;
} Cord;

typedef enum EntryType {
    HEADER = 0,
    ENTRY,
}EntryType;

/* Enums */

typedef enum Level {
    QUICK,
    IN_DEPTH,
    FULL
}SEARCH_LEVEL;
typedef enum QuerySearching {
    SEARCHING_ACTIVE,
    SEARCHING_INACTIVE,
    MISSING,
    FOUND,
    SETUP,
    CALLED
}QueryCommands;
typedef enum Type {
    STREET = 0,
    STREET_E,
    STREET_W,
    AVENUE,
    AVENUE_N,
    AVENUE_S,
    JUNCTION,
    BUILDING,
    CHARGER,
    STABLE,
    BOTH,
    CONSTRUCTION,

}TILE_TYPE;

typedef enum PrintMode{
    ALL,
    CUSTOMER,
    PACKAGE,
}PrintMode;
typedef enum Status {
    IDLE,
    RESET_PICKUP,
    RESET_DROPOFF,
    PICKUP,
    TRANSIT,
    DROPOFF,
    LOADING,
    UNLOADING,
    ETGOHOME,
    RECHARGING,
}AEDV_STATUS;

typedef enum Visit {
    NO,
    YES
}VISITED;
typedef enum OP {READ_BINARY, WRITE_BINARY, READ_WRITE_BINARY, READ_TEXT, WRITE_TEXT, INVALID_OP} OPERATION;
typedef enum BLDG_TYPE { CHG = 0, STB, B, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD {NE, NW, SE, SW, INVALID_QUAD }QUADRANT_TYPE;

/* Structs */
typedef struct Tile {
    Cord location;
    TILE_TYPE Type;
    bool validDirection[4]; //[SOUTH,NORTH,EAST,WEST] (typdef in dependencies.h).
}Tile;


typedef struct event {
    char type;
    int time;
    int originID;
    int destinationID;
    int weight;
}EventData;
/* Need to add +1 to listed length */

typedef struct Customer{
    int custNum;
    char firstName[FIRSTNAMELEN];
    char lastName[LASTNAMELEN];
    char building[BUILDINGLEN];
    char entrance[BUILDINGLEN];
    int floor;
}Customer;
typedef struct order {
    Cord pickUp;
    Cord dropOff;
    int pickupFloor;
    int dropFloor;
    int pickupTime;
    int dropTime;
    int nextDelivery;
    int packageNumber;
    Customer activeCustomers[2];
}OrderData;

typedef struct bldg {
    Cord location;
    char buildingLabel[BUILDINGLEN];
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

typedef struct BuildingNode {
    BuildingData data;
    struct BuildingNode* nextBuilding;
}BuildingNode;

typedef struct InstructionNode {
    struct InstructionNode *child;
    Cord nextPosition;
}InstructionNode;

typedef struct queue {
    TileNode * front;
    TileNode * rear;
}queue;

typedef struct VehicleStats {
    double maxBattery;
    double currentBattery;
    double rechargeRate;
    double drivingRate; //discharge
    double idleRate; //stationary
    int ticksMoving;
    int ticksIdle;
    int ticksCharging;
}VehicleStats;

typedef struct AEDV {
    int EVIN;
    int distanceTravelled;
    int currentOrderNumber;
    int orderCount;
    int loadingDelay;
    int unloadingDelay;
    int delay;
    VehicleStats stats;
    Cord destination;
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
    OrderData orderList[MAX_ORDER_COUNT];
    struct Node *next;
}AEDVNode;

typedef struct DeliveryHeader{
    int firstEmptyDelivery;
}DeliveryHeader;

typedef union DeliveryEntry {
    DeliveryHeader header;
    OrderData data;
}DeliveryEntry;

typedef struct LastDeliveryEntry{
    int custNum;
    int lastDelivery;
}LastDeliveryEntry;

#endif //EXAMPLE_STRUCTS_H
