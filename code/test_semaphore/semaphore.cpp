#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;

const int CUSTOMER = 10;
/*
  某个行业营业厅同时只能服务两个顾客，有多个顾客来的时候，顾客如果发现窗口已满，就等待，如果有可用的窗口就接受服务。
*/

sem_t sem;

void * get_service(void *arg)
{
   int customer_id = *((int*)arg);
   if(sem_wait(&sem) == 0)
	{
	   usleep(100);/*service time 100*/
	   cout<<"customer "<<customer_id<<" receive service.."<<endl;
	   sem_post(&sem);
	}
}

int main()
{
   //初始化信号量，初始值为2标识，有两个顾客可以同时接受服务
   sem_init(&sem, 0 ,2);
   pthread_t customers[CUSTOMER];
   for(int i = 0; i < CUSTOMER; ++i)
	{
	   int customer_id = i;
	   int iRet = pthread_create(&customers[customer_id],NULL, get_service, &customer_id);
	   if(iRet)
		{
		   perror("pthread_create");
		   return iRet;
		}
	   else
		{
		   cout<<"Customer "<<i<<" arrived"<<endl;
		}
		usleep(10);
	}
   for(int j = 0; j < CUSTOMER; ++j)
	{
	   pthread_join(customers[j],NULL);
	}

    sem_destroy(&sem);
    return 0;
}
