#include "World.h"
#include <time.h>
#include <thread>
#include <chrono>
#include <csignal>
#define WAIT_FOR_S(VAL) std::this_thread::sleep_for(std::chrono::seconds(VAL))
inline int positive_modulo(int i, int n)
{
    return (i % n + n) % n;
}
//============Class definitions
World::~World()
{
    for (int i = 0; i < height; ++i)
        delete[] Blockstatus[i];
    delete[] Blockstatus;
}
World::World(unsigned w, unsigned h, char option)
{
    width = w;
    height = h;
    Blockstatus = new Status *[height];
    for (int i = 0; i < height; ++i)
        Blockstatus[i] = new Status[width];
    if(option == '1')
    {
        seed = time(0);
        Randomize();
    }
    else if(option == '2')
    {
        printf("Enter a seed: ");
        scanf("%u",&seed);
        Randomize();
    }
    else
        Manual();
}
void World::Manual()
{
    seed = -1;
    for(;;)
    {
        MapUpdate();
        printf("Enter Y and X coordinates of where you want to toggle(Enter 0 to exit): ");
        unsigned x ,y;
        scanf("%u",&y);
        if(y == 0)
        {
            MapUpdate();
            printf("Now processing input..\n");
            WAIT_FOR_S(1);
            break;
        }
        scanf("%u",&x);
        if(x == 0)
        {
            MapUpdate();
            printf("Now processing input..\n");
            WAIT_FOR_S(1);
            break;
        }
        if(x > width || y > height)
        {
            system("clear");
            printf("Incorrect input..\n");
            continue;
        } 
        if(Blockstatus[y-1][x-1] == Alive)
            Blockstatus[y-1][x-1] = Dead;
        else
            Blockstatus[y-1][x-1] = Alive;        
    }
}
//basic randomazation, does not support custom changes yet
void World::Randomize()
{
    //one in x chance of getting alive(x is also a variable for better randomization):
    srand(seed);
    int x = rand() % 19 + 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            int random = rand() % x; // Alive or dead
            Blockstatus[i][j] = (random == 1) ? Alive : Dead;
        }
    }
    MapUpdate();
    char edit;
    printf("Seed: %d\nOk..Press 1 to edit, anything else to continue:",seed);
    scanf(" %c",&edit);
    if(edit == '1')
        Manual();
}
void World::MapUpdate()
{
    system("clear");
    printf(" ");
    for (int i = 0; i < width; i++)
        printf("%2u", i+1);
    printf("\n");
    for (int i = 0; i < height; i++)
    {
        printf("%2u",i+1);
        for (int j = 0; j < width; j++)
            printf((Blockstatus[i][j] == Alive) ? FILLEDSQUARE : EMPTYSQUARE );
        printf("\n");
    }
}
void World::GameUpdate()
{
    MapUpdate();
    if(seed == -1)
        printf("Generation %u \tSeed Not available for manual changes yet \n",generation++);
    else
        printf("Generation %u \tSeed %d\n",generation++,seed);
    bool ToggleQueue[height][width];
    //check blocks first
    for(int i = 0 ; i < height ; i++)
    {
        for(int j = 0 ; j < width ; j++)
        {
            ToggleQueue[i][j] = 0;
            int neighborAliveCounter = 0;
            if(Blockstatus[positive_modulo(i-1,height)][j] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[positive_modulo(i-1,height)][positive_modulo(j-1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[positive_modulo(i-1,height)][positive_modulo(j+1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[i][positive_modulo(j-1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[i][positive_modulo(j+1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[positive_modulo(i+1,height)][positive_modulo(j-1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[positive_modulo(i+1,height)][j] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[positive_modulo(i+1,height)][positive_modulo(j+1,width)] == Alive)
                neighborAliveCounter++;
            if(Blockstatus[i][j] == Alive && neighborAliveCounter < 2)
                ToggleQueue[i][j] = 1;
            else if(Blockstatus[i][j] == Alive && neighborAliveCounter > 3)
                ToggleQueue[i][j] = 1;
            else if(Blockstatus[i][j] == Dead && neighborAliveCounter == 3)
                ToggleQueue[i][j] = 1;
        }
    }
    //replace them after
    for(int i = 0 ; i < height ; i++)
    {
        for(int j = 0 ; j < width ; j++)
        {
            if(ToggleQueue[i][j] == 1)
            {
                if(Blockstatus[i][j] == Alive)
                    Blockstatus[i][j] = Dead;
                else
                    Blockstatus[i][j] = Alive;
            }
        }
    }
}

