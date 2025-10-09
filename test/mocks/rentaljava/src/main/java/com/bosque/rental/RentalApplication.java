package com.bosque.rental;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

enum Reason {
	approved,
	insufficientAvailability,
	closedDueToConditions,
}


class Decision {
	public final int val;
	public final Reason reason;

	public Decision(int val, Reason reason){
		this.val = val;
		this.reason = reason;
	}

	public int getVal(){
		return this.val;
	}

	public Reason getReason(){
		return this.reason;
	}
}

@SpringBootApplication
public class RentalApplication {

	public static WindCategory categorizeWinds(int windSpeed){
		if(windSpeed < 10) {
			return WindCategory.calm;
		}
		else {
			if (windSpeed < 20) {
				return WindCategory.high;
			}
			else {
				if(windSpeed < 30) {
					return WindCategory.windy;
				}
				else {
					return WindCategory.dangerous;
				}
			}
		}
	}

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
		SpringApplication.run(RentalApplication.class, args);

		int requestedQuantity = 3; 
		boolean allowPartials = false;

		Decision decision = decideRental(forecast,rentalinfo,requestedQuantity,allowPartials);

		if(decision.reason == Reason.approved){
			assert decision.val <= (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if(decision.reason == Reason.approved && !allowPartials){
			assert decision.val == requestedQuantity;
		}
	}
}
