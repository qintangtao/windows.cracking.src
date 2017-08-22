package com.freshui.dextamper;

import com.freshui.dextamper.R;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	static {
       System.loadLibrary("dexparser");
    }
	
	private TextView mTextView = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mTextView = (TextView) findViewById(R.id.textView1);
		GameControl res = new GameControl();
			
		res.setScore(1.0);
		
		String text=  String.format("setScore(1.0), but getScore is %f", res.getScore());
		
		mTextView.setText(text);
		Log.d("dexer", "result="+res.getScore());
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	static void LOG(String s){

		Log.d("test", "test"+s);
	}
}
