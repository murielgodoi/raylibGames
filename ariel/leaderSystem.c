#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"leaderSystem.h"

//------------------------------------------------------------------------------------------
// Leader methods
//------------------------------------------------------------------------------------------

char filename[30] = "data/leader.dat";

Leader* readLeader(){
    Leader *leaderboard = calloc(SIZE, sizeof(Leader));

    FILE* file = fopen(filename,"rb");
    
    if (!file){
        printf("Leader não encontrado. Criando um vazio\n");
        for (size_t i = 0; i < SIZE; i++){
            leaderboard[i].name[0] = '\0';
            leaderboard[i].score = -1;
        }//for
    }else{
        fread(leaderboard, sizeof(Leader), SIZE, file);
        fclose(file);
    }//else
    return leaderboard;
}//readLeader

void writeLeader(Leader *leaderboard){

    FILE* file = fopen(filename,"wb");
    
    if (file == NULL) {
        perror("Error on Leader save");
    } else {
        fwrite(leaderboard ,sizeof(Leader) ,SIZE ,file);
        fclose(file);
    }// else

    return;

}//writeLeader

int addLeader(Leader* leaderboard, char* name, int score){

    int pos = SIZE - 1;
    Leader newLeader;
    strcpy(newLeader.name, name);
    newLeader.score = score;

    while(leaderboard[pos].score <= newLeader.score && pos >= 0 ){
        leaderboard[pos] =  leaderboard[pos-1];
        pos--;
    }//while]
    if(pos != SIZE - 1){
        leaderboard[pos+1] = newLeader;
    }//if

    return pos + 1;

}// addLeader

void printLeader(Leader *leaderboard){

    printf("\n|  # | Name                 | Score |\n");
    for (int i = 0; i < SIZE; i++){
        //if(leaderboard[i].score != -1)
        printf("| %2d | %-20s | %5d |\n",i+1 ,leaderboard[i].name, leaderboard[i].score);
    }//for
    return;

}//writeLeader

bool gotLeaderboard(Leader* leaderboard, int score){
    return leaderboard[SIZE - 1].score <= score;
}

// int main(){

//     Leader* leaderboard = readLeader();

//     printLeader(leaderboard);
//     addLeader(leaderboard, "Muriel", 50);
//     addLeader(leaderboard, "Mantova", 70);
//     addLeader(leaderboard, "Luizao", 60);
//     addLeader(leaderboard, "Marcelo", 90);
//     addLeader(leaderboard, "Adalberto", 30);
//     addLeader(leaderboard, "Maurichan", 50);
//     addLeader(leaderboard, "Fabiao", 70);
//     addLeader(leaderboard, "Wendel", 60);
//     addLeader(leaderboard, "Wagner", 90);
//     addLeader(leaderboard, "Andre do Mato", 30);

//     writeLeader(leaderboard);
    
//     printLeader(leaderboard);

//     return 0;

// }//main

