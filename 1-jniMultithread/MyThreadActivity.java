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
        //��ť����
        mButton.setOnClickListener(new View.OnClickListener() 
        {
            
            @Override
            public void onClick(View v) 
            {
                // TODO Auto-generated method stub
                //����JNI�еĺ���������JNI�е��߳�
                mainThread();
                
            }
        });
        //��ʼ��JNI����
        setJNIEnv();
    }
         
    //��JNI�е��̻߳ص�
    private static void fromJNI(int i)
    {
        Log.v("Java------>", ""+i);
    }
    
    //���ط���
    private native void mainThread();
    private native void setJNIEnv(); 
    
    static
    {
        //���ض�̬��
        System.loadLibrary("JNIThreads");
    }
    
}