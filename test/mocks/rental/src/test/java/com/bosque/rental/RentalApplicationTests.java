package com.bosque.rental;

import org.junit.jupiter.api.Test;
import org.mockito.stubbing.Answer;
import org.springframework.boot.test.context.SpringBootTest;

import net.bytebuddy.agent.ByteBuddyAgent.AttachmentProvider.ForEmulatedAttachment;

import static org.mockito.Mockito.*;

@SpringBootTest
class RentalApplicationTests {

	@Test
	void testGetForecastService(){
		ForecastService forecastservice = mock();

        when(forecastservice.getForecast())
			.thenReturn(
				new Forecast(
					new TempRange(47, 22),
					new WindSpeedInfo(12, 20, WindDirection.east),
					ForecastDetail.snow)
			);

		Forecast forecast = forecastservice.getForecast();

		assert forecast.temp.high == 47;
		assert forecast.temp.low == 22;

		assert forecast.winds.min == 12;
		assert forecast.winds.max == 20;
		assert forecast.winds.winddirection == WindDirection.east;

		assert forecast.info == ForecastDetail.snow;

		verify(forecastservice).getForecast();
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

	@Test
	void contextLoads() {
	}

}
