package com.nan.thread;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MyThreadActivity extends Activity 
{
    private Button mButton = null; 
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mButton = (Button)this.findViewById(R.id.button);
        //按钮监听
        mButton.setOnClickListener(new View.OnClickListener() 
        {
            
            @Override
            public void onClick(View v) 
            {
                // TODO Auto-generated method stub
                //调用JNI中的函数来启动JNI中的线程
                mainThread();
                
            }
        });
        //初始化JNI环境
        setJNIEnv();
    }
         
    //由JNI中的线程回调
    private static void fromJNI(int i)
    {
        Log.v("Java------>", ""+i);
    }
    
    //本地方法
    private native void mainThread();
    private native void setJNIEnv(); 
    
    static
    {
        //加载动态库
        System.loadLibrary("JNIThreads");
    }
    
}