package com.example.aes2file;


import java.io.File;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	
	static public String PATH = "/AES2FILE/";
	static public String APP_PATH = "";
	static
	{
		File dateDir = Environment.getExternalStorageDirectory();
		APP_PATH = dateDir.getAbsolutePath() + PATH;// PRIVATE_PATH;//
	}
	
	static boolean createFolder(String strFolder)
	{
		File file = new File(strFolder);
		if (!file.exists())
		{
			if (file.mkdirs()) //
			{
				//AppConfig.logD("mkdir: ok" + strFolder);
				return true;
			}
			else
			{
				//AppConfig.logD("mkdir: failed" + strFolder);
				return false;
			}
		}
		return true;
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        createFolder(APP_PATH);
        Button btnAes = (Button) this.findViewById(R.id.btn_aes);
        btnAes.setOnClickListener(new View.OnClickListener()
		{

			@Override
			public void onClick(View v)
			{
				// TODO Auto-generated method stub
				save2file(APP_PATH, "test");
			}
		});
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    public native int save2file(String filePath, String fileData);
    
    static {
        System.loadLibrary("aes");
    }
}
