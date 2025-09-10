package com.bosque.rental;

import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;


import static org.junit.jupiter.api.Assertions.assertEquals;
import org.mockito.Mock;
import static org.mockito.Mockito.*;


import java.util.Map;

@SpringBootTest
class RentalApplicationTests extends RentalApplication {

	@Mock
	RentalInfoService rentalinfoservice;

	@Mock
	ForecastService forecastservice;

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
					new WindSpeedInfo(11, 20, WindDirection.north),
					ForecastDetail.sun),
			"AID", new Forecast(
					new TempRange(80, 78),
					new WindSpeedInfo(15, 20, WindDirection.east),
					ForecastDetail.showers));

	@Test
	void testRentalDecisionFailure() {
		when(rentalinfoservice.getRentalAvailabilityInfo())
				.thenReturn(new RentalAvailabilityInfo(30, 20, 2));

		RentalAvailabilityInfo rentalinfo = rentalinfoservice.getRentalAvailabilityInfo();
		verify(rentalinfoservice).getRentalAvailabilityInfo();

		String iata = "JFK";

		when(forecastservice.getForecast(iata))
				.thenReturn(forecasts.get(iata));

		Forecast forecast = forecastservice.getForecast(iata);
		verify(forecastservice).getForecast(iata);

		int requestedQuantity = 22;
		boolean allowPartials = true;

		Decision decision = decideRental(forecast, rentalinfo, requestedQuantity, allowPartials);

		if (decision.reason == Reason.approved) {
			assert decision.val <= (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if (decision.reason == Reason.approved && !allowPartials) {
			assert decision.val == requestedQuantity;
		}

		assertEquals(Reason.closedDueToConditions, decision.reason);
	}

	@Test
	void testRentalDecisionSuccess() {
		when(rentalinfoservice.getRentalAvailabilityInfo())
				.thenReturn(new RentalAvailabilityInfo(30, 20, 2));

		RentalAvailabilityInfo rentalinfo = rentalinfoservice.getRentalAvailabilityInfo();
		verify(rentalinfoservice).getRentalAvailabilityInfo();

		String iata = "AOR";

		when(forecastservice.getForecast(iata))
				.thenReturn(forecasts.get(iata));

		Forecast forecast = forecastservice.getForecast(iata);
		verify(forecastservice).getForecast(iata);

		int requestedQuantity = 20;
		boolean allowPartials = true;

		Decision decision = decideRental(forecast, rentalinfo, requestedQuantity, allowPartials);

		if (decision.reason == Reason.approved) {
			assert decision.val <= (rentalinfo.inventory + rentalinfo.returns) - rentalinfo.reservations;
		}

		if (decision.reason == Reason.approved && !allowPartials) {
			assert decision.val == requestedQuantity;
		}

		assertEquals(Reason.approved, decision.reason);
	}

	@Test
	void testGetForecastService() {


		forecasts.forEach((iata, forecast) -> {
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
	void testGetRentalInfoService() {

		when(rentalinfoservice.getRentalAvailabilityInfo())
				.thenReturn(new RentalAvailabilityInfo(30, 20, 2));

		RentalAvailabilityInfo info = rentalinfoservice.getRentalAvailabilityInfo();

		assert info.inventory == 30;
		assert info.reservations == 20;
		assert info.returns == 2;

		verify(rentalinfoservice).getRentalAvailabilityInfo();
	}
}
