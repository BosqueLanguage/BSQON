package com.bosque.rental;

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

public abstract class RentalInfoService{
	abstract RentalAvailabilityInfo getRentalAvailabilityInfo();
}
