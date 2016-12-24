package com.wf.crossstruct;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	TextView  tv;
	Second obj;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        obj = new Second();
        tv = new TextView(this);
        
        tv.setText( obj.stringCheatAAA() );
        setContentView(tv);      
        
        emu_Init_so_Android();
        //Toast.makeText(getApplicationContext(), emu_getVideoData(), Toast.LENGTH_SHORT).show();
        
        emu_init(1, 2, "biosPath");
        emu_loadRom("romPath", "cheatPath");
        emu_coreLoop(1, 2);
        Log.e("emu_getVideoData", emu_getVideoData());
        Log.e("emu_getVideoWidth", "emu_getVideoWidth:" + emu_getVideoWidth());
        Log.e("emu_getVideoHeight", "emu_getVideoHeight:" + emu_getVideoHeight());
        
        emu_setButton(1, 2);
        emu_exit();
    }   

    public static native void emu_Init_so_Android();//此函数必须最先调用

    public static native int emu_init(int mode, int lang, String biosPath);
    public static native int emu_loadRom(String romPath, String cheatPath);
    public static native int emu_coreLoop(int draw, int sound);
    public static native String emu_getVideoData();
    public static native int emu_getVideoWidth();
    public static native int emu_getVideoHeight();
    public static native int emu_setButton(int player, int value);

    public static native void emu_exit();
    
    static {
        System.loadLibrary("emulator");
    }
    
    //jni要调用的回调函数
	static void setVibrate(int intensityL, int intensityR) {
		Log.e("setVibrate", "MainActivity-->setVibrate : " + String.valueOf(intensityL) + "--" + String.valueOf(intensityR));
	}
}
