package com.freshui.dextamper;

import android.util.Log;

class GameControl{
  
	private Double mScore;
	private double mDuration;
  
  	public GameControl(){
  		mScore = null;
  	}

	public void setScoreHidden(double result)
	{
		this.mScore = Double.valueOf(result*1.8);
	}

	public void setScore(double result)
	{
		if(mScore == null)
			MainActivity.LOG("setScore: multiple sets");
		this.mScore = Double.valueOf(result);
	}
	
	public Double getScore()
	{
		return mScore;
	}
  
	public void setDuration(double paramDouble)
	{
		this.mDuration = Math.max(0.0D, paramDouble);
	}
	  
	public void setDuration2(double paramDouble)
	{
		this.mDuration *= 1.8;
	}	  
}


