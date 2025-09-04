enum Reason {
	approved,
	insufficientAvailability,
	closedDueToConditions,
}

enum WindCategory {
    calm,
    windy,
    high,
    dangerous
}

class Decision<val,reason>{
	public final int val;
	public final Reason reason;

	public Decision(int val, Reason reason){
		this.val = val;
		this.reason = reason;
	}
}

class TempRange{
	int high;
	int low;

	public TempRange(int high, int low){
		this.high = high;
		this.low = low;

		assert this.low <= this.high : "Low temp must not be higher than high temp";
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
}

class RentalAvailabilityInfo {
    int inventory;
    int reservations;
    int returns;


	public RentalAvailabilityInfo(int inventory, int reservations, int returns){
		this.inventory = inventory;
		this.reservations = reservations;
		this.returns = returns;

		assert (this.inventory + this.returns) >= this.reservations : "Total inventory must be larger than the number of reservations.";
	}
}

public class RentalDecision{

	// public static Forecast getForecast(){
	// }
	//
	// public static RentalAvailabilityInfo getRentalAvailabilityInfo(){
	// }

	public static Decision decideRental(Forecast forecast, RentalAvailabilityInfo rentalinfo,int requestedQuantity, boolean allowPartials){
		boolean isClosed = ((categorizeWinds(forecast.winds.max) == WindCategory.dangerous) || forecast.info == ForecastDetail.thunderstorms);
		int availability = (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;

		if(isClosed) {
			return new Decision(0, Reason.closedDueToConditions);
		}
		else if (requestedQuantity <= availability) {
				return new Decision(requestedQuantity, Reason.approved);
		}
		else {
			if(allowPartials) {
				return new Decision(availability, Reason.approved);
			}
			else {
				return new Decision(0, Reason.insufficientAvailability);
			}
		}
	}

	public static void main(String[] args){
		int requestedQuantity = 0; 
		boolean allowPartials = false;
		
		// Forecast forecast = getForecast();
		// RentalAvailabilityInfo rentalinfo = getRentalAvailabilityInfo();

		Decision decision = decideRental(forecast,rentalinfo,requestedQuantity,allowPartials);

		if(decision.reason == Reason.approved){
			assert decision.val <= (rental.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if(decision.reason == Reason.approved && !allowPartials){
			assert decision.val == requestedQuantity;
		}
	}
}
