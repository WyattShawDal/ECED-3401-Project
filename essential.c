/*
 *Author: Wyatt Shaw & Cameron Archibald
 *Created on: 2023-11-12
 *Module Info: Contains Frame and Camera update functions as well as querying functions, which make the core "loop" functions declared in main
*/
#include "dependencies.h"

struct STATUS_PRINT stprint[] = {
        {IDLE, "IDLE"},
        {RESET_PICKUP, "RESET_PICKUP"},
        {RESET_DROPOFF, "RESET_DROPOFF"},
        {PICKUP, "PICKUP"},
        {DROPOFF,"DROPOFF"},
        {LOADING,"LOADING"},
        {UNLOADING, "UNLOADING"},
        {ETGOHOME,"ETGOHOME"}
};
// Module Functions Definition
void UpdateDrawFrame(void) {

    CommandHandler();
    // begins drawing a new frame
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    //draws the current time above the row indicators
    DrawText(TextFormat("Current Time: %d", frameCount), 0, -40, DEFAULTFONTSIZE, RAYWHITE);
    // Draw the map into the window using tile data generated in main
    UpdateMap();
    // Intelligent Order Assignment
    AEDVHandler();
    // Calculate Paths if necessary
    OneWayNavigation();
    // Draw the new movements
    AEDVNode* curr = ActiveList;
    //loops through active list
    while(curr != NULL) {
        //draw new positions for active aedvs
        DrawVehicleMovements(curr);
        //update active vehicles stats
        UpdateVehicleStats(&curr);
        curr = curr->next;
    }
    curr = InactiveList;
    while(curr != NULL) {
        //draw inactive aedvs
        DrawVehicleMovements(curr);
        //update idle stats
        UpdateVehicleStats(&curr);
        curr = curr->next;
    }
    //update the 'time'
    frameCount++;
    EndDrawing();
}
/*Camera Control , Utilizes raylib commands and camera function */
void CameraControl() {
    //allows for camera panning
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))  {
        Vector2 delta  = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);

        camera.target = Vector2Add(camera.target, delta);
    }

    float wheel = GetMouseWheelMove();
    if(wheel != NOMOVEMENT) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;
        const float zoomIncrement = 0.125f; //float value to adjust the level of zoom

        camera.zoom += (wheel*zoomIncrement);
        if(camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
    }
}

void QueryVehicleInfo() {
    int vehicleIndex;
    printf("Enter Vehicle EVIN : ");
    scanf("%d", &vehicleIndex);

    //Search active list
    AEDVNode * vehicle = FindInList(ActiveList,vehicleIndex);

    if(vehicle == NULL)
        //If not in active list, search inactive list
        vehicle = FindInList(InactiveList,vehicleIndex);

    if(vehicle == NULL)
        //Not in either list
        printf("No AEDV in either list with EVIN = %d\n\n", vehicleIndex);
    else
        //Vehicle found, print stats
        PrintVehicleStats(vehicle->data,IN_DEPTH);
}
void PrintVehicleStats(AEDV vehicle, int level) {
    if(level == QUICK) {
        printf("Quick stats:\n"
               "Status = %d\n"
               "Order Number = %d\n"
               "Total Orders = %d\n\n",
               vehicle.currStatus,  vehicle.currentOrderNumber,  vehicle.orderCount);
    }
    else if (level == IN_DEPTH) {
        printf("In depth stats:\n"
               "Status = %s\n"
               "Order Number = %d\n"
               "Total Orders = %d\n"
               "Current Battery = %lf\n"
               "Max Battery = %lf\n"
               "Recharge Rate = %lf\n"
               "Driving Rate = %lf\n"
               "Idle Rate = %lf\n"
               "Ticks Moving = %d\n"
               "Ticks Idle = %d\n\n",
               stprint[vehicle.currStatus].statusString,  vehicle.currentOrderNumber,  vehicle.orderCount,
               vehicle.stats.currentBattery,vehicle.stats.maxBattery, vehicle.stats.rechargeRate,
               vehicle.stats.drivingRate, vehicle.stats.idleRate, vehicle.stats.ticksMoving, vehicle.stats.ticksIdle);
    }
}

void GetCommands(int startup) {
    //print a startup message
    if(startup == SETUP)
        printf("\nWelcome to the AEDV delivery system. This system employs various commands for I/O\n"
               "This program was written by: Wyatt Shaw & Cameron Archibald for ECED 3401\n\n");
    //print out all hte commands
    printf("Here is a list of useful commands, you may call this list again by pressing 'H'\n"
           "Query vehicle information: 'F'\n"
           "List all deliveries: 'D'\n"
           "List a customer's deliveries: 'S'\n"
           "Print a specific delivery: 'P'\n"
           "List all vehicle file entries: 'V'\n"
           "List vehicle file entries for a specific vehicle: 'E'\n"
           "Modify a vehicle's stats: 'M'\n"
           "Zoom: Mouse Wheel up/down\n"
           "Pan: Hold Right Click\n"
           "Exit: ESC\n\n");

}
void CommandHandler() {
    if(IsKeyPressed(KEY_H)) {
        GetCommands(CALLED);
    }
    if(IsKeyPressed(KEY_D))
        QueryDeliveryInfo(ALL, 0);

    if(IsKeyPressed(KEY_F)) {
        QueryVehicleInfo();
    }
    if(IsKeyPressed(KEY_S)) {
        int custID;
        printf("Enter the customer ID to search: ");
        scanf("%d", &custID);
        QueryDeliveryInfo(CUSTOMER, custID);
    }
    if(IsKeyPressed(KEY_P)) {
        int packageID;
        printf("Enter the package ID to search: ");
        scanf("%d",&packageID);
        QueryDeliveryInfo(PACKAGE,packageID);
    }
    if(IsKeyPressed(KEY_V)) {
        PrintVehicleFile(0,ALL);
    }
    if(IsKeyPressed(KEY_E)) {
        int EVIN;
        printf("Enter the EVIN to search: ");
        scanf("%d",&EVIN);
        PrintVehicleFile(EVIN,VEHICLE);
    }
    if(IsKeyPressed(KEY_M)) {
        int EVIN;
        printf("Enter the EVIN to search: ");
        scanf("%d",&EVIN);
        AEDVNode* vehicle;
        if((vehicle = FindInList(ActiveList,EVIN)) == NULL)
            vehicle = FindInList(InactiveList,EVIN);

        if(vehicle == NULL)
            printf("No vehicle with EVIN %d\n\n",EVIN);
        else
            ModifyAEDVStats(&vehicle);
    }
}
