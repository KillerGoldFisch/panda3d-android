package com.heekinho.android.panda3d;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;

public class DummyActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {       
    	System.loadLibrary("EGL");
    	System.loadLibrary("GLESv1_CM");
    	System.loadLibrary("GLESv2");
    	
    	System.loadLibrary("gnustl_shared");
    	//System.loadLibrary("stlport_shared");
    	
        System.loadLibrary("p3android");
        System.loadLibrary("p3androiddisplay");
        System.loadLibrary("pandagame");

        super.onCreate(savedInstanceState);

        Intent intent = new Intent(DummyActivity.this, android.app.NativeActivity.class);
        DummyActivity.this.startActivity(intent);        
    }

}
