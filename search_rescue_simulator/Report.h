#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>

// Represents a location on the map
struct Point {
    double x;
    double y;
};

// Represents one recorded mission event
struct Reading {
    int step;
    double x;
    double y;
    double battery;
    double thermal;
    std::string event;
};

// Save mission results and create graphs
void saveReports(
    const std::vector<Reading>& readings,
    const std::vector<Point>& planned,
    Point target
);

#endif