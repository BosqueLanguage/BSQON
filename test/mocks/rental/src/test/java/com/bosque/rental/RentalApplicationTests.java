package com.bosque.rental;

import org.junit.jupiter.api.Test;
import org.mockito.stubbing.Answer;
import org.springframework.boot.test.context.SpringBootTest;

import net.bytebuddy.agent.ByteBuddyAgent.AttachmentProvider.ForEmulatedAttachment;

import static org.mockito.Mockito.*;

import java.util.Map;

@SpringBootTest
class RentalApplicationTests extends RentalApplication {

	@Test
	void testRentalDecisionFailure(){
		RentalInfoService rentalinfoservice = mock();
		when(rentalinfoservice.getRentalAvailabilityInfo())
			.thenReturn(new RentalAvailabilityInfo(30, 20, 2));

		RentalAvailabilityInfo rentalinfo = rentalinfoservice.getRentalAvailabilityInfo();
		verify(rentalinfoservice).getRentalAvailabilityInfo();

		ForecastService forecastservice = mock();
		when(forecastservice.getForecast("JFK"))
			.thenReturn( new Forecast(
				new TempRange(82, 76),
				new WindSpeedInfo(12, 30, WindDirection.west),
				ForecastDetail.snow));

		Forecast forecast = forecastservice.getForecast("JFK");
		verify(forecastservice).getForecast("JFK");


		int requestedQuantity = 500;
		boolean allowPartials = true;

		Decision decision = decideRental(forecast,rentalinfo,requestedQuantity,allowPartials);

		if(decision.reason == Reason.approved){
			assert decision.val <= (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if(decision.reason == Reason.approved && !allowPartials){
			assert decision.val == requestedQuantity;
		}
	}

	@Test
	void testRentalDecisionSuccess(){
		RentalInfoService rentalinfoservice = mock();
		when(rentalinfoservice.getRentalAvailabilityInfo())
			.thenReturn(new RentalAvailabilityInfo(30, 20, 2));

		RentalAvailabilityInfo rentalinfo = rentalinfoservice.getRentalAvailabilityInfo();
		verify(rentalinfoservice).getRentalAvailabilityInfo();

		ForecastService forecastservice = mock();
		when(forecastservice.getForecast("JFK"))
			.thenReturn( new Forecast(
				new TempRange(82, 76),
				new WindSpeedInfo(12, 30, WindDirection.west),
				ForecastDetail.snow));

		Forecast forecast = forecastservice.getForecast("JFK");
		verify(forecastservice).getForecast("JFK");


		int requestedQuantity = 20;
		boolean allowPartials = true;

		Decision decision = decideRental(forecast,rentalinfo,requestedQuantity,allowPartials);

		if(decision.reason == Reason.approved){
			assert decision.val <= (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if(decision.reason == Reason.approved && !allowPartials){
			assert decision.val == requestedQuantity;
		}
	}

	@Test
	void testGetForecastService(){
		Map<String, Forecast> forecasts = Map.of(
			"XPL", new Forecast(
				new TempRange(87, 65),
				new WindSpeedInfo(40, 70, WindDirection.east),
				ForecastDetail.thunderstorms),
			"JFK", new Forecast(
				new TempRange(82, 76),
				new WindSpeedInfo(12, 30, WindDirection.west),
				ForecastDetail.snow),
			"FRA", new Forecast(
				new TempRange(61, 50),
				new WindSpeedInfo(22, 80, WindDirection.south),
				ForecastDetail.fog),
			"AOR", new Forecast(
				new TempRange(71, 60),
				new WindSpeedInfo(31, 70, WindDirection.north),
				ForecastDetail.sun),
			"AID", new Forecast(
				new TempRange(80, 78),
				new WindSpeedInfo(15, 20, WindDirection.east),
				ForecastDetail.showers)
		);

		ForecastService forecastservice = mock();

		forecasts.forEach((iata,forecast) -> 
			{
				when(forecastservice.getForecast(iata))
					.thenReturn(forecast);

				Forecast mckforecast = forecastservice.getForecast(iata);
				assert mckforecast.temp.high == forecast.temp.high;
				assert mckforecast.temp.low == forecast.temp.low;

				assert mckforecast.winds.min == forecast.winds.min;
				assert mckforecast.winds.max == forecast.winds.max;
				assert mckforecast.winds.winddirection == forecast.winds.winddirection;

				assert mckforecast.info == forecast.info;

				verify(forecastservice).getForecast(iata);
			});
	}

	@Test 
	void testGetRentalInfoService(){
		RentalInfoService rentalservice = mock();

        when(rentalservice.getRentalAvailabilityInfo())
            .thenReturn(new RentalAvailabilityInfo(30, 20, 2));

        RentalAvailabilityInfo info = rentalservice.getRentalAvailabilityInfo();

        assert info.inventory == 30;
        assert info.reservations == 20;
        assert info.returns == 2;

        verify(rentalservice).getRentalAvailabilityInfo();
	}
}
