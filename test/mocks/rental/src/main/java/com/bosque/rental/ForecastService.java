package com.bosque.rental;

enum WindCategory {
    calm,
    windy,
    high,
    dangerous
}

class TempRange{
	int high;
	int low;

	public TempRange(int high, int low){
		this.high = high;
		this.low = low;

		assert this.low <= this.high : "Low temp must not be higher than high temp";
	}

	public int getHigh(){
		return this.high;
	}

	public int getLow(){
		return this.low;
	}
}

enum WindDirection{
	north,
    south,
    east,
    west
}

class WindSpeedInfo{
	int min;
	int max;

	WindDirection winddirection;

	public WindSpeedInfo(int min, int max, WindDirection winddirection){
		this.min = min;
		this.max = max;
		this.winddirection = winddirection;

		assert this.min <= this.max : "Min wind speed must not be higher than max wind speed";
	}

	public int getMin(){
		return this.min;
	}

	public int getMax(){
		return this.max;
	}

	public WindDirection getWindDirection(){
		return this.winddirection;
	}
}

enum ForecastDetail {
    sun,
    showers,
    thunderstorms,
    snow,
    fog
}

class Forecast{
	TempRange temp;
	WindSpeedInfo winds;
	ForecastDetail info;

	public Forecast(TempRange temp, WindSpeedInfo winds, ForecastDetail info){
		this.temp = temp;
		this.winds = winds;
		this.info = info;
	}

	public TempRange getTemp(){
		return this.temp;
	}

	public WindSpeedInfo getWindInfo(){
		return this.winds;
	}

	public ForecastDetail getForecastDetail(){
		return this.info;
	}
}

public abstract class ForecastService {
	abstract Forecast getForecast(String iatacode);
}
