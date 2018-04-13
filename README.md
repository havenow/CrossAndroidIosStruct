# NDK多线程调用注意    

https://www.cnblogs.com/lknlfy/archive/2012/03/16/2400786.html     
https://blog.csdn.net/earbao/article/details/51340929     

JNIEnv是一个线程相关的变量

JNIEnv 对于每个 thread 而言是唯一的

JNIEnv *env指针不可以为多个线程共用

但是java虚拟机的JavaVM指针是整个jvm公用的，我们可以通过JavaVM来得到当前线程的JNIEnv指针.

可以使用javaAttachThread保证取得当前线程的Jni环境变量
