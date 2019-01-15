#include "World.h"
#include <stdio.h>
#include <thread>
#include <chrono>
#include <csignal>
//lower values for faster playback:
#define PLAYBACK_SPEED_MICRO_S 100000
#define WAIT_FOR_MICRO_S(VAL) std::this_thread::sleep_for(std::chrono::microseconds(VAL)) 
bool exitStatus = false;

void signalHandler(int signum)
{
    exitStatus = true;
}
int main()
{
    unsigned width,height;
    char option;
    printf("This simulation has open borders, meaning the outer layers continues on the other side.\n");
    printf("Enter height and width: ");
    scanf("%u %u",&height,&width);
    printf("Enter 1 for fully random generation,2 for custom seed generation and any other character for manual:\n");
    scanf(" %c",&option);
    World world(width,height,option);
    printf("Press Ctrl+ C to exit..\n");
    WAIT_FOR_MICRO_S(1000000);
    signal(SIGINT, signalHandler);  
    while(!exitStatus)
    {
        WAIT_FOR_MICRO_S(PLAYBACK_SPEED_MICRO_S);
        world.GameUpdate();
    }
    return 0;
}