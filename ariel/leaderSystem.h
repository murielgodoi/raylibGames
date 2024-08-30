#ifndef LEADERSYSTEM_H
    #define LEADERSYSTEM_H

    #define SIZE 10
    #define MAX_LENGTH 20

    typedef struct{
        char name[MAX_LENGTH];
        int score;
    } Leader;



    //------------------------------------------------------------------------------------------
    // Leader methods
    //------------------------------------------------------------------------------------------
    Leader* readLeader();


    void writeLeader(Leader *leaderboard);

    void addLeader(Leader* leaderboard, char name[], int score);

    void printLeader(Leader *leaderboard);

#endif