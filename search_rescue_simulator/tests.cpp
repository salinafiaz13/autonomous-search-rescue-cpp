#include "Drone.h"

#include <cassert>
#include <iostream>

int main() {
    Drone d;

    // Check starting position
    assert(d.getX() == 0);
    assert(d.getY() == 0);

    // Check starting battery
    assert(d.getBattery() == 100);

    // Check battery safety
    assert(d.canVisitAndReturn(25, 15));

    // Check movement
    assert(d.moveTo(10, 5));
    assert(d.getBattery() < 100);

    // Check no person detected far away
    d.scan(30, 15);
    assert(!d.hasFoundPerson());

    // Check detection nearby
    d.scan(10, 5);
    assert(d.hasFoundPerson());

    // Check return home
    assert(d.returnHome());
    assert(d.getX() == 0);
    assert(d.getY() == 0);

    // Check distance tracking
    assert(d.getDistance() > 0);

    // Check distant flight rejection
    assert(!d.canVisitAndReturn(1000, 1000));

    std::cout << "All drone checks passed.\n";
}
