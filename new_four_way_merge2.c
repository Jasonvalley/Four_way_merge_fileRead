#include <stdio.h>
#include <stdlib.h>
void generateData(char *filename);//generate list with initialSortSize sorted
enum generateData{generatedDataRange=2000000,generatedDataSize=2000000,initialSortSize=10000};
void mergeSort(int *list);
void mergePass(int *list,int size);
void merge(int *list,int firstStart,int firstEnd,int end);
void fourWayMergeSort(char *list);
void fourWayMergePass(char *list,char *tempList,int size);
void fourWayMerge(int value[4],int *minValue,int *minPosition);
void printFile(char *file);
int main(void)
{
    generateData("input.txt");
    fourWayMergeSort("input.txt");
    printFile("input.txt");
}
void generateData(char *filename)
{
    FILE *f=fopen(filename,"w");
    int sortedList[initialSortSize],i,sortCount=0;
    for(i=0;i<generatedDataSize;i++){
        sortedList[i-sortCount*initialSortSize]=rand()%generatedDataRange+1;
        if((i+1)%initialSortSize==0){
            //sort list
            mergeSort(sortedList);
            sortCount++;
            //input
            int j;
            for(j=0;j<initialSortSize;j++){
                fprintf(f,"%d ",sortedList[j]);
            }
            fprintf(f,"-1 ");
        }
    }
}
void mergeSort(int *list)
{
    int size=1;
    while(size<initialSortSize){
        mergePass(list,size);
        size*=2;
    }
}
void mergePass(int *list,int size)
{
    int listSize=initialSortSize,mergeCount=0;
    while(mergeCount*size*2+size*2-1<=listSize){
        merge(list,mergeCount*size*2,mergeCount*size*2+size-1,mergeCount*size*2+size*2-1);
        mergeCount++;
    }
    //Two set is left
    if(mergeCount*size*2+size-1<listSize){
        merge(list,mergeCount*2*size,mergeCount*2*size+size-1,listSize-1);
    }
}
void merge(int *list,int firstStart,int firstEnd,int end)
{
    int firstLoc=firstStart,secondLoc=firstEnd+1,listLoc=0,tempList[end-firstStart+1];
    while(firstLoc<=firstEnd&&secondLoc<=end){
        if(list[firstLoc]<list[secondLoc]){
            tempList[listLoc]=list[firstLoc];
            listLoc++;  firstLoc++;
        }
        else{
            tempList[listLoc]=list[secondLoc];
            listLoc++;  secondLoc++;
        }
    }
    if(firstLoc<=firstEnd){
        for(;firstLoc<=firstEnd;){
            tempList[listLoc]=list[firstLoc];
            listLoc++;  firstLoc++;
        }
    }
    else{
        for(;secondLoc<=end;){
            tempList[listLoc]=list[secondLoc];
            listLoc++;  secondLoc++;
        }
    }
    int i;
    for(i=firstStart;i<=end;i++){
        list[i]=tempList[i-firstStart];
    }
}
void fourWayMergeSort(char *list)
{
    int size=initialSortSize;
    while(size<generatedDataSize){
        fourWayMergePass(list,"temp.txt",size);
        size*=4;
        fourWayMergePass("temp.txt",list,size);
        size*=4;
    }
}
void fourWayMergePass(char *list,char *tempList,int size)
{
    FILE *f[generatedDataSize/size+7];
    FILE *tempFile=fopen(tempList,"w");
    //initialize position;
    int i;
    for(i=0;i<generatedDataSize/size;i++){
        f[i]=fopen(list,"r");
        fseek(f[i],i*size,0);
    }
    for(i=generatedDataSize/size;i<generatedDataSize/size+7;i++){
        f[i]=NULL;
    }
    //merge pass
    int listLoc=0;
    while(1){
        //check end
        for(i=listLoc;i<listLoc+4;i++){
            if(f[i]!=NULL){
                break;
            }
        }
        if(i==listLoc+4)    break;
        //merge
            //intitialize merge
        int minValue=0,minPosition=0,value[4];
        for(i=listLoc;i<listLoc+4;i++){
            if(f[i]==NULL){
                value[i-listLoc]=-1;
            }
            else{
                fscanf(f[i],"%d ",&value[i-listLoc]);
            }
        }
        while(1){
            fourWayMerge(value,&minValue,&minPosition);
            if(minValue==-1)    break;
            fprintf(tempFile,"%d ",minValue);
            fscanf(f[minPosition+listLoc],"%d ",&value[minPosition]);
        }
        if(size*2<generatedDataSize)
        {
            fprintf(tempFile,"-1 ");
        }
        listLoc+=4;
    }
}
typedef struct tree{
    int data;
    int position;
}tree;
void fourWayMerge(int value[4],int *minValue,int *minPosition)
{
    tree tree1[8];
    //initialize tree
    int i;
    *minPosition=0;
    for(i=4;i<8;i++){
        tree1[i].data=value[i-4];
        tree1[i].position=i-4;
    }
    //sort
    for(i=3;i>0;i--){
        if(tree1[i*2].data<tree1[i*2+1].data){
            if(tree1[i*2].data==-1){
                tree1[i]=tree1[i*2+1];
            }
            else
            {
                tree1[i]=tree1[i*2];
            }
        }
        else{
            if(tree1[i*2+1].data==-1){
                tree1[i]=tree1[i*2];
            }
            else
            {
                tree1[i]=tree1[i*2+1];
            }
        }
    }
    //return
    *minPosition=tree1[1].position;
    *minValue=tree1[1].data;
}
void printFile(char *file)
{
    int count=0;
    FILE *f=fopen(file,"r");
    while(!feof(f)){
        int d;
        fscanf(f,"%d ",&d);
        printf("%d ",d);
        count++;
    }
    printf("\n\n%d",count);
    fclose(f);
}
