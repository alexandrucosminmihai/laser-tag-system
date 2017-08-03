// Laser Tag game managing system, Mihai Alexandru Cosmin
#include <iostream>
#include <cstring>
#include <cstdio>
#include "./lasertag.h"

int main()
{
    int i, j, noSensors, d, noTeam1, noTeam2;
    int noNode, score1, score2;
    char line[600], aux[50], *p, *saveptr;
    LaserTag kernel;
    scanf("%d", &noSensors);
    // initialise the sensors adiacency matrix
    kernel.initializeSensors(noSensors);
    // add the distances between sensors
    for (i = 0; i < noSensors; ++i)
    {
        for (j = 0; j < noSensors; ++j)
        {
            if (i == j)
            {
                kernel.setSensors(i, j, 0);
            }
            else
            {
                scanf("%d", &d);
                kernel.setSensors(i, j, d);
            }
        }
    }
    scanf("%d %d", &noTeam1, &noTeam2);
    kernel.initializePlayers(noTeam1 + noTeam2);
    noNode = -1;
    fgets(line, 600, stdin);
    fgets(line, 600, stdin);

    p = strtok_r(line, " \n\r", &saveptr);
    while (p != nullptr)
    {
        strcpy(aux, p);
        aux[strlen(aux)] = '\0';
        // set for every player a node number that I'll use in the
        // duels adiacency matrix
        kernel.addPlayer(aux, 0, ++noNode);
        p = strtok_r(nullptr, " \n\r", &saveptr);
    }
    fgets(line, 600, stdin);
    // add the players from the second team in the hashtable
    p = strtok_r(line, " \n\r", &saveptr);
    while (p != nullptr)
    {
        strcpy(aux, p);
        aux[strlen(aux)] = '\0';
        // set for every player a node number that I'll use in the
        // duels adiacency matrix
        kernel.addPlayer(aux, 1, ++noNode);
        p = strtok_r(nullptr, " \n\r", &saveptr);
    }
     // the rounds related input begins
    score1 = 0;
    score2 = 0;
    fgets(line, 600, stdin);  // first round begins
    while (1)
    {
        int sensor;
        char name1[50];
        kernel.newRound();
        fgets(line, 600, stdin);
        while (strstr(line, "GAME_") == nullptr &&
                strstr(line, "END_CHAMPIONSHIP") == nullptr)
        {  // while a new round hasn't begun
            if (line[0] >= '0' && line[0] <= '9')
            {  // this means that the line contains input from sensors
                p = strtok_r(line, ":", &saveptr);
                sensor = atoi(p);  // sensor number
                // player's name
                p = strtok_r(nullptr, " \n\r", &saveptr);
                strcpy(aux, p);
                aux[strlen(aux)] = '\0';
                kernel.updateSensor(aux, sensor);
            }
            else  // if the line contains duel input
            {   // first player's name
                p = strtok_r(line, " \n\r", &saveptr);
                strcpy(name1, p);
                name1[strlen(name1)] = '\0';
                p = strtok_r(nullptr, " \n\r", &saveptr);  //  ->
                // second player's name
                p = strtok_r(nullptr, " \n\r", &saveptr);
                strcpy(aux, p);
                aux[strlen(aux)] = '\0';
                kernel.updateDuels(name1, aux);
            }
            fgets(line, 600, stdin);
        }
        if (strstr(line, "END_CHAMPIONSHIP") != nullptr)
        {   // calculate the winning chance
            kernel.chanceWinner(&score1, &score2);
            break;
        }
        else  // leave the while loop because a new round begins
        {
             // check which team won and update the score
            int winner = kernel.roundWinner();
            if (winner == 0)
            {
                ++score1;
            }
            else
            {
                ++score2;
            }
        }
    }
    kernel.topShooters();
    kernel.topExplorers();
    kernel.topFireExchange();
    kernel.finalScore(score1, score2);  // for the sake of code decency
    return 0;
}
