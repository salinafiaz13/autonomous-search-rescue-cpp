#include "Drone.h"
#include <algorithm>
#include <cmath>

// Calculate battery needed to fly to a location
double Drone::energyTo(double newX, double newY) const {
    return std::hypot(newX - x, newY - y) * ENERGY_PER_UNIT;
}

// Check whether enough battery remains
// for the next location AND the trip home
bool Drone::canVisitAndReturn(double newX, double newY) const {
    double outbound = energyTo(newX, newY);
    double inbound = std::hypot(newX, newY) * ENERGY_PER_UNIT;

    // Keep an extra 12% battery reserve
    return battery >= outbound + inbound + 12.0;
}

// Move to a specific coordinate
bool Drone::moveTo(double newX, double newY) {
    double cost = energyTo(newX, newY);

    if (cost > battery) {
        return false;
    }

    distanceTraveled += std::hypot(newX - x, newY - y);

    battery = std::max(0.0, battery - cost);

    x = newX;
    y = newY;

    return true;
}

// Simulate thermal detection
void Drone::scan(double personX, double personY) {
    double distanceToPerson = std::hypot(
        personX - x,
        personY - y
    );

    if (distanceToPerson <= 3.0) {
        thermal = 0.95;
    }
    else if (distanceToPerson <= 14.0) {
        thermal = 0.40;
    }
    else {
        thermal = 0.15;
    }

    if (thermal > 0.75) {
        foundPerson = true;
    }
}

// Return the drone to home base
bool Drone::returnHome() {
    return moveTo(0.0, 0.0);
}