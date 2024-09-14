
//Name:Alhasan Manasra
//Id:1211705
//Section:4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define maxCity 100
#define Line 300
#define infinity INT_MAX
int V;
FILE *cities,*short_path;
char City[maxCity][maxCity];
void menu();
void selectOperation();
void Reader(int graph [][V]);
int readCity(char city[][maxCity]);
void initializeGraph(int graph [][V]);
int getIndex(char city[]);
int minDistance(int distance[],int visited[]);
void Dijkstra(int graph [][V],int parent[V],int dist[V],int distanceCost[V],int src);
void BFS(int graph [][V],int parent[V],int totalDistBFS[],int distanceCost[],int src);
void printPath(char path[],int parent[],int totalDist[], int destination);
void printInFile(char shortPath[][Line],int size);

int main() {

    selectOperation();

    return 0;
}
void menu(){//menu for operation can do
    system("cls");
    printf("Select operation you want:");
    printf("\n1_Lode cities and build the graph");
    printf("\n2_Enter source");
    printf("\n3_Enter Destination");
    printf("\n4_Exit");
    printf("\nEnter your choose:  ");
}
void selectOperation(){//to select operation you want
    int x;
    int flag;
    char source[20];
    char destination[20];
    V = readCity(City);//V is the number of Vertices
    int graph[V][V];
    int parentDijkstra[V];
    int parentBFS[V];
    int totalDistDijkstra[V];
    int totalDistBFS[V];
    int distanceCost[V];
    int distanceCostBFS[V];
    initializeGraph(graph);//make all indexes in the graph equal zero
    char shortestPath[Line][Line];//save the path to print in file
    int count=0;//index in shortestPath array
    int one=0;//to check if we read from file or not
    int src=0;//to check if we read a source or not;
    char path[Line];

    do{
        menu();
        scanf("%d",&x);
        system("cls");
        switch (x) {
            case 1:
                    V = readCity(City);//read city from file and save them in a city array
                    Reader(graph);//read from file to make the graph
                    printf("Cities loaded successfully");
                    one=1;
                    sleep(2);
                    break;
            case 2:
                if(one) {
                    printf("2_Enter source:  ");
                    scanf("%s", source);
                    flag = 0;
                    src=0;
                    for (int i = 0; i < V; ++i) {//check if the city we enter found
                        if (strcmp(source, City[i]) == 0) {
                            flag = 1;
                            src=1;
                            break;
                        }
                    }
                    if (flag) {
                        Dijkstra(graph, parentDijkstra, totalDistDijkstra, distanceCost, getIndex(source));
                        BFS(graph, parentBFS, totalDistBFS, distanceCostBFS, getIndex(source));
                        system("cls");
                        printf("Source City is loaded successfully");
                    } else
                        printf("City doesn't exist!");
                        sleep(2);
                }
                else{
                    printf("we must choose operation 1 to read cities");
                    sleep(4);
                }
                break;
            case 3:
                if(one&src) {
                    printf("3_Enter Destination:  ");
                    scanf("%s", destination);
                    flag = 0;
                    for (int i = 0; i < V; ++i) {//check if the city we enter found
                        if (strcmp(destination, City[i]) == 0) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag) {
                        printf("\nDijkstra: %s", source);
                        printPath(path, parentDijkstra, distanceCost, getIndex(destination));
                        printf("\ntotal distance =%dkm\n", totalDistDijkstra[getIndex(destination)]);
                        sprintf(shortestPath[count++], "Dijkstra from %s to %s: %s %s\ntotal distance =%dkm", source,
                                destination, source, path, totalDistDijkstra[getIndex(destination)]);

                        printf("\nBFS: %s", source);

                        printPath(path, parentBFS, distanceCostBFS, getIndex(destination));
                        printf("\ntotal distance =%dkm\n", totalDistBFS[getIndex(destination)]);
                        sprintf(shortestPath[count++], "\nBFS from %s to %s: %s %s\ntotal distance =%dkm\n", source,
                                destination, source, path, totalDistBFS[getIndex(destination)]);
                        sleep(5);
                    } else {
                        printf("City doesn't exist!");
                        sleep(2);
                    }
                }
                else
                {
                    printf("we must choose operation 1 to read cities and operation 2 to choose source city");
                    sleep(4);
                }

                break;
            case 4:
                printInFile(shortestPath,count);
                printf("\nPaths loaded in a file\nThank you to use our system.Good bye");
                exit(0);

        }
    } while (x);
}
///////////////////////////////////////////////////////////////
void initializeGraph(int graph [][V]){//make all indexes in the graph with initial value equal zero
    for (int i = 0; i < V; ++i)
        for (int j = 0; j <V ; ++j)
            graph[i][j]=0;
}
/////////////////////////////////////////////////////////
int getIndex(char city[]){//return index for city
    int i=0;
    while(i<V)
    {
        if(strcmp(City[i],city)==0)
            return i;
        else
            i++;
    }
    return i;
}
//////////////////////////////////////////////////////////////////////
void Reader(int graph [][V]){//read from file to connect graph

    cities= fopen("cities.txt","r");
    if(cities==NULL)
    {
        printf("file doesn't found!!");
        exit(-1);
    }
    char source[20];
    char dist [20];
    int cost;
        while (fscanf(cities, "%s\t%s\t%dkm", source, dist, &cost) != EOF)
            graph[getIndex(source)][getIndex(dist)] = cost;
      //      graph[getIndex(dist)][getIndex(source)] = cost;

    fclose(cities);
}
//////////////////////////////////////////////////////////////////////
int readCity(char city[][maxCity]){//read from file to save different city in an array of city and return there number
    char source[20];
    char destination [20];
    int cost;
    int count=0;
    cities= fopen("cities.txt","r");

        while (fscanf(cities,"%s\t%s\t%dkm",source,destination,&cost)!=EOF)
        {
            if(count==0)
            {
                strcpy(city[0],source);
                strcpy(city[1],destination);
                count=2;
            }
            else {
                int sourceUnique=1;
                int destUnique=1;
                for (int j = 0; j < count; ++j)
                {
                    if (strcmp(source, city[j])==0)//check if city we read it from file exist in a city array
                        sourceUnique=0;

                    if(strcmp(destination,city[j])==0)//check if city we read it from file exist in a city array
                        destUnique=0;

                }
                if(sourceUnique)//if city we read it the first time we add it in a city array
                {
                    strcpy(city[count++],source);
                }
                if(destUnique)
                {
                    strcpy(city[count++],destination);
                }
            }
        }
    fclose(cities);
    return count;
}
/////////////////////////////////////////////////////////
int minDistance(int distance[],int visited[]){//find vartix with less distance
    int min=infinity;
    int min_index;
    for (int i = 0; i < V; ++i) {
        if(!visited[i]&&distance[i]<=min){
            min=distance[i];
            min_index=i;
        }
    }
    return min_index;
}
///////////////////////////////////////////////////////////////////////////////////////
void Dijkstra(int graph [][V],int parent[V],int totalDist[V],int distance[V],int src) {//find the path from source to destination with minimum cost
    int visited[V];
    for (int i = 0; i < V; i++) {//make all vertices not visited and the distance infinity and with no parent
        totalDist[i] = infinity;
        visited[i] = 0;
        parent[i] = -1;
    }
    totalDist[src] = 0;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(totalDist, visited);
        visited[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && totalDist[u] != infinity && totalDist[u] + graph[u][v] < totalDist[v]) {
                totalDist[v] = totalDist[u] + graph[u][v];//save the total distance from source to destination;
                distance[v]=graph[u][v];//save the distance between to cities
                parent[v] = u;//save the city that we come from it
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void  BFS(int graph [][V],int parent[V],int totalDistBFS[],int distanceCost[],int src){//find the path from source to destination with fewer edges
    int visited[V];
    for (int i = 0; i < V; ++i)//make all vertices not visited and the distance infinity and with no parent
    {
        visited[i]=0;
        parent[i]=-1;
        distanceCost[i]=0;
    }
    totalDistBFS[src]=0;
    int queue[V];
    int front=0,rear=0;
    visited[src]=1;
    queue[rear++]=src;

    while (front<rear)//when queue is not empty
    {
        src=queue[front++];

        for (int i = 0; i < V; ++i)
        {
            if(graph[src][i]&&!visited[i])//check all edges from src if it visited or nut and if it has cost
            {
                visited[i]=1;
                queue[rear++]=i;
                parent[i]=src;
                distanceCost[i]=graph[src][i];//save distance between to cities
                totalDistBFS[i]=totalDistBFS[src]+graph[src][i];//save total distance from source to destination
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////
void printPath(char path[],int parent[],int dist[], int destination) {//print a path with cost between to cities and total distance
    strcpy(path,"");//make path empty
    if (parent[destination] == -1)
        return;

    printPath(path,parent,dist, parent[destination]);
    printf("-> %d -> %s",dist[destination], City[destination]);
    char p[100];
    sprintf(p," -> %d -> %s",dist[destination],City[destination]);//make an equal path from two cities
    strcat(path,p);
}
////////////////////////////////////////////////////////////////////////
void printInFile(char shortPath[][Line],int size){//print Dijkstra and BFS with a path and total cost in file
    short_path= fopen("shortest_distance.txt","w");
    if(short_path==NULL)
    {
        printf("Error couldn't open file");
        return;
    }
    for (int i = 0; i < size; ++i)
        fprintf(short_path,"%s\n",shortPath[i]);
}
