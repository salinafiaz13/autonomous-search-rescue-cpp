#ifndef DRONE_H
#define DRONE_H

class Drone {
private:
    double x = 0.0;
    double y = 0.0;
    double battery = 100.0;
    double thermal = 0.15;
    double distanceTraveled = 0.0;
    bool foundPerson = false;

    static constexpr double ENERGY_PER_UNIT = 0.55;

public:
    double energyTo(double newX, double newY) const;
    bool canVisitAndReturn(double newX, double newY) const;
    bool moveTo(double newX, double newY);
    void scan(double personX, double personY);
    bool returnHome();

    double getX() const { return x; }
    double getY() const { return y; }
    double getBattery() const { return battery; }
    double getThermal() const { return thermal; }
    double getDistance() const { return distanceTraveled; }
    bool hasFoundPerson() const { return foundPerson; }
};

#endif