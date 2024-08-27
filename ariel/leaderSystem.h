#ifndef LEADERSYSTEM_H
    #define LEADERSYSTEM_H

    typedef struct{
        char name[30];
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