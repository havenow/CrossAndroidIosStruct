#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#include<jni.h>
#include<android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

/*
����1��
JNIEnv��һ���߳���صı���
JNIEnv ����ÿ�� thread ������Ψһ��
JNIEnv *envָ�벻����Ϊ����̹߳���

����취��

����java�������JavaVMָ��������jvm���õģ����ǿ���ͨ��JavaVM���õ���ǰ�̵߳�JNIEnvָ��.
����ʹ��javaAttachThread��֤ȡ�õ�ǰ�̵߳�Jni��������
static JavaVM *gs_jvm=NULL;
gs_jvm->AttachCurrentThread((void **)&env, NULL);//���ӵ�ǰ�̵߳�һ��Java�����
jclass cls = env->GetObjectClass(gs_object);
jfieldID fieldPtr = env->GetFieldID(cls,"value","I");

����2��

����ֱ�ӱ���һ���߳��е�jobjectָ�뵽ȫ�ֱ�����,Ȼ��������һ���߳���ʹ������

����취��

��env->NewGlobalRef����һ��ȫ�ֱ������������obj(�ֲ�����)���浽ȫ�ֱ�����,�����߳̿���ʹ�����ȫ�ֱ������������java����
ע�⣺������һ�� jobject������Ҫ��ô�����磺
jclass ���� jobject public �̳ж��������࣬��������Ȼ��һ�� jobject����Ҫ����һ�� global reference �Ա��պ�ʹ�á�
�� jmethodID/jfieldID �� jobject û�м̳й�ϵ��������һ�� jobject��ֻ�Ǹ����������Բ����ڱ��ͷ��������⣬�ɱ����ֱ��ʹ�á�
*/

//�߳���
#define NUMTHREADS 5

//ȫ�ֱ���
JavaVM *g_jvm = NULL;
jobject g_obj = NULL;


void *thread_fun(void* arg)
{
    JNIEnv *env;
    jclass cls;
    jmethodID mid;

    //Attach���߳�
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK)//JNIEnv��һ���߳���صı���;JNIEnv ����ÿ�� thread ������Ψһ��;JNIEnv *envָ�벻����Ϊ����̹߳���
    {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    //�ҵ���Ӧ����
    cls = (*env)->GetObjectClass(env,g_obj);
    if(cls == NULL)
    {
        LOGE("FindClass() Error.....");
        goto error; 
    }
    //�ٻ�����еķ���
    mid = (*env)->GetStaticMethodID(env, cls, "fromJNI", "(I)V");
    if (mid == NULL) 
    {
        LOGE("GetMethodID() Error.....");
        goto error;  
    }
    //������java�еľ�̬����
        (*env)->CallStaticVoidMethod(env, cls, mid ,(int)arg);
    

error:    
    //Detach���߳�
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK)
    {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
    }
    

    pthread_exit(0);
}

//��java�����Դ������߳�
JNIEXPORT void Java_com_nan_thread_MyThreadActivity_mainThread( JNIEnv* env, jobject obj)
{
    int i;
    pthread_t pt[NUMTHREADS];
    
    for (i = 0; i < NUMTHREADS; i++)
        //�������߳�
        pthread_create(&pt[i], NULL, &thread_fun, (void *)i);
}


//��java����������JNI����
JNIEXPORT void Java_com_nan_thread_MyThreadActivity_setJNIEnv( JNIEnv* env, jobject obj)
{
    //����ȫ��JVM�Ա������߳���ʹ��
    (*env)->GetJavaVM(env,&g_jvm);
    //����ֱ�Ӹ�ֵ(g_obj = obj)
    g_obj = (*env)->NewGlobalRef(env,obj);//����ֱ�ӱ���һ���߳��е�jobjectָ�뵽ȫ�ֱ�����,Ȼ��������һ���߳���ʹ������
}


//����̬�ⱻ����ʱ���������ϵͳ����
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;    

    //��ȡJNI�汾
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK) 
    {
        LOGE("GetEnv failed!");
            return result;
    }

    return JNI_VERSION_1_4;
}