#include "Drone.h"
#include "Report.h"

#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    Drone drone;

    // Location of the simulated missing person
    const Point target{25, 15};

    // Store all search locations
    std::vector<Point> searchPath;

    // Build a zigzag search pattern
    for (int row = 0; row < 4; ++row) {
        int y = 5 + row * 10;

        // Move left to right
        if (row % 2 == 0) {
            for (int x = 5; x <= 45; x += 10) {
                searchPath.push_back({
                    double(x),
                    double(y)
                });
            }
        }

        // Move right to left
        else {
            for (int x = 45; x >= 5; x -= 10) {
                searchPath.push_back({
                    double(x),
                    double(y)
                });
            }
        }
    }

    // Record the drone's starting information
    std::vector<Reading> history{
        {0, 0, 0, 100, 0.15, "LAUNCH"}
    };

    std::string outcome = "SEARCH_COMPLETE_NO_TARGET";
    int step = 0;

    std::cout
        << "=== AUTONOMOUS SEARCH & RESCUE MISSION ===\n";

    std::cout
        << "Home: (0, 0) | Planned points: "
        << searchPath.size() << "\n";

    std::cout << std::fixed << std::setprecision(1);

    // Visit each search location
    for (const Point& p : searchPath) {

        // Check if enough battery remains
        if (!drone.canVisitAndReturn(p.x, p.y)) {
            outcome = "LOW_BATTERY_RETURN";

            std::cout
                << "Battery reserve needed: "
                << "ending search safely.\n";

            break;
        }

        // Fly to the next location
        drone.moveTo(p.x, p.y);

        // Scan for the missing person
        drone.scan(target.x, target.y);

        ++step;

        // Save the reading
        history.push_back({
            step,
            drone.getX(),
            drone.getY(),
            drone.getBattery(),
            drone.getThermal(),
            drone.hasFoundPerson()
                ? "TARGET_FOUND"
                : "SCAN"
        });

        // Display current mission information
        std::cout
            << "Step " << step
            << " | Position ("
            << drone.getX() << ", "
            << drone.getY() << ")"
            << " | Battery "
            << drone.getBattery() << "%"
            << " | Thermal "
            << drone.getThermal()
            << "\n";

        // Stop searching if target is found
        if (drone.hasFoundPerson()) {
            outcome = "TARGET_FOUND";

            std::cout
                << ">>> Possible missing person detected!\n";

            break;
        }
    }

    // Return home
    bool returned = drone.returnHome();

    // Record final location
    history.push_back({
        ++step,
        drone.getX(),
        drone.getY(),
        drone.getBattery(),
        drone.getThermal(),
        returned ? "RETURN_HOME" : "RETURN_FAILED"
    });

    // Generate CSV and graphs
    saveReports(history, searchPath, target);

    // Print mission summary
    std::cout << "\n=== MISSION SUMMARY ===\n";

    std::cout
        << "Result: " << outcome << "\n";

    std::cout
        << "Points searched: "
        << step - 1 << " of "
        << searchPath.size() << "\n";

    std::cout
        << "Distance traveled: "
        << drone.getDistance()
        << " units\n";

    std::cout
        << "Battery remaining: "
        << drone.getBattery()
        << "%\n";

    std::cout
        << "Returned home: "
        << (returned ? "YES" : "NO")
        << "\n";

    std::cout
        << "Saved: mission_log.csv "
        << "and 3 SVG graphs in assets/\n";

    return returned ? 0 : 1;
}