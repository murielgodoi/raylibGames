#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    char name[30];
    int score;
} Leader;

//------------------------------------------------------------------------------------------
// Leader methods
//------------------------------------------------------------------------------------------

Leader* readLeader(){
    Leader *leaderboard = calloc(10, sizeof(Leader));

    FILE* file = fopen("data/leader.dat","rb");
    
    if (!file){
        printf("Leader não encontrado. Criando um vazio");
        for (size_t i = 0; i < 10; i++){
            leaderboard[i].name[0] = '\0';
            leaderboard[i].score = -1;
        }//for
    }else{
        fread(leaderboard,sizeof(Leader),10,file);
        fclose(file);
    }//else
    return leaderboard;
}//readLeader

void writeLeader(Leader *leaderboard){

    FILE* file = fopen("data/leader.dat","wb");
    
    if (!file) {
        perror("Error on Leader save");
    } else {
        fwrite(leaderboard ,sizeof(Leader) ,10 ,file);
        fclose(file);
    }// else

    return;

}//writeLeader

void addLeader(Leader* leaderboard, char name[], int score){

    int pos = 9;
    Leader newLeader;
    strcat(newLeader.name, name);
    newLeader.score = score;

    Leader temp = leaderboard[pos];
    while(leaderboard[pos].score <= newLeader.score || pos != 0 ){
        leaderboard[pos] = leaderboard[pos-1];
        pos--;
    }//while]
    if(pos != 9){
        leaderboard[pos] = temp;
    }//if

}// addLeader

void printLeader(Leader *leaderboard){

    printf("\n|  # | Name           | Score |\n");
    for (int i = 0; i < 10; i++){
        //if(leaderboard[i].score != -1)
        printf("| %2d | $-20s | %5d |\n",i+1 ,leaderboard[i].name, leaderboard[i].score);
    }//for
    return;

}//writeLeader

int main(){

    Leader* leaderboard = NULL;

    printLeader(leaderboard);
    addLeader(leaderboard, "Muriel", 50);
    printLeader(leaderboard);

    return 0;

}//main