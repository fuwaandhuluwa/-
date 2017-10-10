# 多线程模拟火车售票系统

多线程相当于一个并发系统，一般同时可执行多个任务，可以有效提高计算机资源的利用率。如果多个任务可以共享资源，特别是同时写入某个变量的时候，就需要解决同步的问题。

在并发的情况下，指令执行的先后顺序由内核决定。
同一个线程内部，指令按先后顺序执行，不同的线程之间的指令很难说清楚是哪一个会先执行。如果运行的结果依赖于不同线程执行的先后的话，那么就会造成竞争条件，
在这样的状况下，计算机的结果很难预知，所以应该尽量避免竞争条件的形成。最常见的解决竞争条件的方法是将原先分离的两个指令构成不可分割的原子操作，而其他
任务不能插入到原子操作中。

对于多线程的程序来说，同步是指在一定的时间内只允许某个线程访问某个特定的资源。而在此段时间内，不允许其他的线程访问该资源。

实现同步的几种方式：
     互斥锁（mutex）、条件变量（condition variable）、读写锁（reader-writer lock）和信号量（semaphore）

1.互斥锁（mutex）
       互斥锁（mutex）是一个特殊的变量，它有两个状态  lock 和 unlock。互斥锁一般被设置成全局变量。pthread_mutex_t mutex_x = PTHREAD_MUTEX_INITIALIZER; 打开的互斥锁
	可以由某个线程获取。一旦获得，这个互斥锁会锁上，此后只有该线程有权打开，其他线程要想获得互斥锁的线程，会等待再次打开的时候。我们可以将互斥锁想象成只能容纳一个人
	的卫生间，当某个人进去的时候，可以从卫生间里便将洗手间的门锁上，其他人只能在互斥锁外面等待那个认出来，才能进去。但在外边等待的人没有排队，谁先看到洗手间空了，谁
	就可以先冲过去。
	   所以线程在pthread_mutex_lock(pthread_mutex_t *mutex_x) he pthread_mutex_unlock(pthread_mutex_t *mutex_x)之间的操作就构成了原子操作，不会被其他线程影响。
	   互斥锁的使用过程中，主要有pthread_mutex_init()、pthread_mutex_lock()、pthread_mutex_unlock()、pthread_mutex_destory()几个函数。
2.条件变量（condition variable）
        如果线程正在等待共享数据内某个条件的出现，它可能会重复对互斥对象锁定和解锁，每次都会检查共享数据结构，以查找某个值。但这是在浪费时间和资源，而且这种繁忙的
	查询效率非常低下。
	    在每次检查之间，可以让调用线程短暂的进入睡眠，比如睡眠3秒，但由此线程代码就无法快速响应。真正需要的是：当线程在等待满足某个条件时使线程进入睡眠状态，一旦
	条件满足，就唤醒因等待特定条件而睡眠的线程。如果能够做到这一点，线程代码将是非常高效的，并且不会占用宝贵的互斥锁对象。而这正是条件变量能做的事！
		条件变量通过允许线程阻塞和等待另一个线程发送信号的方法弥补互斥锁的不足，它常和互斥锁一起使用。使用时，条件变量被用来阻塞一个线程，当条件不满足时，线程往往解开
	相应的互斥锁并等待条件发生变化。一旦其他线程改变了条件变量，它将通知相应的条件变量唤醒一个或者多个正在被此条件变量阻塞的线程，这些线程将重新锁定互斥锁并重新测试条
    件是否满足。
		条件变量相关的函数
	（1）创建：
	    条件变量和护持锁一样，都有静态和动态两种创建方式
			静态方式使用PTHREAD_COND_INITIALIZER常量，函数原型  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
			动态方式使用pthread_cond_init函数，pthread_cond_init函数原型：int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t * cond_attr);
			当cond_attr为NULL时，使用默认属性。LinuxThreads实现条件变量不支持属性，因此cond_attr参数实际被忽略。
	（2）注销：
		注销一个条件变量需要调用pthread_cond_destroy(),它的原型是：int pthread_cond_destroy(pthread_cond_t * cond);
		只有在没有现成在该条件变量上等待的时候才能注销这个条件变量，否则返回EBUY。因为Linux实现的条件变量没有分配什么资源，所以注销动作只检查是否有等待线程。
	（3）等待：
		等待条件有两种方式-----条件等待pthread_cond_wait()和计时等待ptrhead_cond_timedwait()。其中计时等待若果在给定时刻前没条件没有满足，则返回ETIMEOUT，结束等待。
		两种等待方式都必须和一个互斥锁配合使用，以防止多个线程同时请求的竞争条件。互斥锁必须是普通所或者适应锁，PTHREAD_MUTEX_TIMED_NP OR PTHREAD_MUTEX_ADAPTIVE_NP
	（4）激发：
		激发条件有两种形式pthread_cond_signal()激活一个等待该条件的线程，存在多个等待线程时，按入队顺序激活其中一个；而pthread_cond_broadcast()则激活所有的等待线程。
3.读写锁（reader-writer lock）
		在一些程序中存在读写锁的问题，也就是说，对某些资源的访问会存在两种可能的情况，一种是访问必须是排他性的，就是独占的意思，这种操作成写操作；另一种情况就是访问
	方式可以共享的，就是说可以有多个线程同时去访问某个资源，这种操作就称之为读操作。这个问题模型是从对文件的读写操作中引申出来的。
		读写锁的3种状态：
		（1）当读写锁是写加锁状态时，在这个锁解锁之前，所有试图对这个锁加锁的线程都会被堵塞。
		（2）当读写锁是读加锁状态时，所有试图以读模式进行加锁的线程都会获得访问权，但是以写模式加锁的线程会被阻塞。
		（3）当读写锁在读模式的所状态时，如果另有线程试图以写模式加锁，读写锁通常会阻塞随后的读模式的加锁请求，这样可以避免读模式锁长期占用，而等待的写模式锁长期阻塞。
    强读者同步和强写者同步， 分别对应的典型系统是：图书馆查阅系统和航班订票系统
		
4.信号量（semaphore）
		线程还可以通过信号量进行通信。信号量和互斥锁的区别：互斥锁只允许一个县城进入临界区，而信号量允许多个线程同时进入临界区。要实用信号量同步，需要包含头文件semaphore.h
	信号量函数的名字都以sem_开头。
	    常用的信号量函数：
		（1）sem_init函数
			int sem_init(sem_t *sem, int pshared, unsigned int value);
		    该函数用于初始化有sem指向的信号对象，设置它的共享选项，并给它一个初始的值。pshared控制信号量的类型，其值为0表示是当前进程的局部信号量，否则就可以在多个线程
			之间共享。value为sem的初始值，调用成功时返回0，失败返回-1.
		（2）sem_wait()函数
		    以原子操作将信号量的值减一。 int sem_wait(sem_t  *sem);调用成功时返回0，失败返回-1.
		（3）sem_post()函数
		    以原子操作将信号量的值加1. int sem_post(sem_t  *sem);调用成功时返回0，失败返回-1.
		（4）sem_destroy()函数
		    该函数用于对用完的信号量进行清理。 int sem_destroy(sem_t * sem);调用成功时返回0，失败返回-1.
		