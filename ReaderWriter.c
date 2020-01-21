#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

#define no_of_elements 99

volatile int arr[no_of_elements];  //array to read and write to
volatile int pos = 0;
sem_t wrt, mutex;
volatile int readcnt = 0;

void *reader(void *arg) {
    sem_wait(&mutex);
    int reader_num = (__intptr_t)arg;

    if(readcnt == 1){
        while(sem_wait(&wrt) != 0){
            printf("\tReader %d can't read because data is in use \n", reader_num);
        }
        sem_wait(&wrt);
    }

    sem_post(&mutex);

    sleep(rand()%7+1);        
   if(pos != 0){
        printf("\tReader %d has read data", reader_num);
        for(int i=0; i<pos; i++){
            printf(" %d", arr[i]);
        } 
        printf("\n");
        
   }
   else{
       printf("\tReader %d has no data to read \n", reader_num);
   }
    
    sem_wait(&mutex);

    readcnt--;
   if(readcnt == 0){
        sem_post(&wrt);  
   }                 

    sem_post(&mutex);


} 


/////////////////////////////////////////

void *writer(void *arg) {
    int writer_num = (__intptr_t)arg;
    
    // writer requests for critical section
    while (sem_wait(&wrt) != 0)
    {
        printf("Writer tried to add to list but it was locked \n");
    }
  
   
    // performs the write
    
    arr[pos] = writer_num;
    pos++;
    printf("Writer %d has edited data \n", writer_num);
    sleep(rand()%5+1);
    // leaves the critical section
    sem_post(&wrt);


} 

//////////////////////////////////////////////////

void main(){
    int readers = 0;
    int writers = 0;
    
    sem_init(&wrt,0,1);
    sem_init(&mutex,0,1);

    printf("Enter number of readers\n");
    if (scanf("%d", &readers) <= 0)
        {
            printf("Scanf error: Only numbers expected\n");
            exit(0);
        }
    if(readers<1){
            printf("Scanf error: There should be at least 1 reader\n");
            exit(0);
    }          
   
    pthread_t rtid[readers];
    printf("Enter number of writers\n");
    scanf("%d", &writers);
    pthread_t wtid[writers];

    for(int i=0;i<readers;i++)
    {
        pthread_create(&wtid[i],NULL,writer,(void *)(__intptr_t)i);
        pthread_create(&rtid[i],NULL,reader,(void *)(__intptr_t)i);
    }
    
    for(int i=0;i<=writers;i++)
    {
        pthread_join(wtid[i],NULL);
        pthread_exit(NULL);
    }

    for(int j=0;j<=readers;j++)
    {
        pthread_join(rtid[j],NULL);
        pthread_exit(NULL);
    }


    // printf("Final Array: \n");
    // for (int i=0; i<pos;i++){
    //     printf("Array element %d is %d \n", i, arr[i]);
    // }

    return;

}