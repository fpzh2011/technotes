# Linux�̻߳���

## �̷߳���

�̷߳�Ϊ���֣�joinable or detached��man pthread_create
Ĭ�ϡ�����Ҳ�����ڴ����߳���ָ����

�̴߳���
����˳��P309��
man pthread_create
Unless real-time scheduling policies are being employed, after a call to pthread_create(), it is indeterminate which thread��the caller or the new thread��will next execute.

�̱߳�ʶ����ͬʵ�ֵĽṹ��һ������һ����������pthread_t gettid proc/xxx/task
���뷽����Ψһ�ԡ�id�ȽϷ���
self equal
���Ա����á�`man pthreads` �� Thread IDs �ڡ�A thread ID may be reused after a terminated thread has been joined,  or a detached thread has terminated. 


��ȡ�̷߳���ֵ��joinһ�����ǻ�ȡ�߳��˳��룬һ�������ͷ��߳���Դ��
����߳�ͬʱjoinһ���̵߳���Ϊ����δ����ġ�joinһ��֮ǰ�Ѿ�join�����̣߳�Ҳ��δ����ġ�
joinһ��joinable�߳�ʧ�ܣ��������ʬ�̣߳�ռ��ϵͳ��Դ��


����ѡ�� -pthread
����ѡ�� -lpthread

�߳���ֹ������ֵ��
man pthread_exit()
�������߳��⣬���̵߳������������أ���������ʽ����pthread_exit()��
Performing a return from the start function of any thread other than the main thread results in an implicit call to pthread_exit(), using the function's return value as the thread's exit status.
����������߳��˳����������̼߳������У����߳���Ҫ����pthread_exit()������exit()��
To allow other threads to continue execution, the main thread should terminate by calling pthread_exit() rather than exit(3).
���ǣ����߳��˳�֮��/proc/xxxx/task�У���Ȼ�����̶߳�Ӧ��Ŀ¼��




���ݹ���
P308 ��2��
P358


��ѯ�߳�ʵ��
getconf GNU_LIBPTHREAD_VERSION




