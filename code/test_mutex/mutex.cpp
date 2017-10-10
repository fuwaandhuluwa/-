#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
using namespace std;

pthread_mutex_t mutex_x = PTHREAD_MUTEX_INITIALIZER;
const int max_ticket_num = 20;
int total_ticket_num = max_ticket_num;
// Test mutex 

void * sell_ticket(void *arg){

    for(int i = 0; i < max_ticket_num; i++)
    {
        pthread_mutex_lock(&mutex_x);
        if(total_ticket_num > 0)
        {
            cout<<"sell the "<<(20 - total_ticket_num + 1)<<"th ticket"<<endl;
            total_ticket_num--;
        }
        pthread_mutex_unlock(&mutex_x);
        sleep(1);
    }
    return 0;
}

int main()
{
    pthread_t tids[2];
    for(int i = 0; i < 2; ++i)
    {
        int iRet = pthread_create(&tids[i], NULL, sell_ticket, NULL);
        if(iRet)
        {
            cout<<"pthread_creat error!"<<endl;
            return iRet;
        }
    }

    sleep(20);
    void* retval;
    for(int i = 0; i < 2; ++i)
    {
        int iRet = pthread_join(tids[i], &retval);
        if(iRet)
        {
            cout<<"pthread_join error!"<<endl;
            return iRet;
        }
        cout<<"retval: "<<retval<<endl;
    }
    return 0;
}
