# 多线程模拟火车售票系统

多线程相当于一个并发系统，一般同时可执行多个任务，可以有效提高计算机资源的利用率。如果多个任务可以共享资源，特别是同时写入某个变量的时候，就需要解决同步的问题。

在并发的情况下，指令执行的先后顺序由内核决定。
同一个线程内部，指令按先后顺序执行，不同的线程之间的指令很难说清楚是哪一个会先执行。如果运行的结果依赖于不同线程执行的先后的话，那么就会造成竞争条件，
在这样的状况下，计算机的结果很难预知，所以应该尽量避免竞争条件的形成。最常见的解决竞争条件的方法是将原先分离的两个指令构成不可分割的原子操作，而其他
任务不能插入到原子操作中。

对于多线程的程序来说，同步是指在一定的时间内只允许某个线程访问某个特定的资源。而在此段时间内，不允许其他的线程访问该资源。

实现同步的几种方式：
     互斥锁（mutex）、条件变量（condition variable）、读写锁（reader-writer lock）和信号量（semphore）

1.互斥锁（mutex）
       互斥锁（mutex）是一个特殊的变量，它有两个状态  lock 和 unlock。互斥锁一般被设置成全局变量。pthread_mutex_t mutex_x = PTHREAD_MUTEX_INITIALIZER; 打开的互斥锁
	可以由某个线程获取。一旦获得，这个互斥锁会锁上，此后只有该线程有权打开，其他线程要想获得互斥锁的线程，会等待再次打开的时候。我们可以将互斥锁想象成只能容纳一个人
	的卫生间，当某个人进去的时候，可以从卫生间里便将洗手间的门锁上，其他人只能在互斥锁外面等待那个认出来，才能进去。但在外边等待的人没有排队，谁先看到洗手间空了，谁
	就可以先冲过去。
	   所以线程在pthread_mutex_lock(pthread_mutex_t *mutex_x) he pthread_mutex_unlock(pthread_mutex_t *mutex_x)之间的操作就构成了原子操作，不会被其他线程影响。
	   互斥锁的使用过程中，主要有pthread_mutex_init()、pthread_mutex_lock()、pthread_mutex_unlock()、pthread_mutex_destory()几个函数。
	   