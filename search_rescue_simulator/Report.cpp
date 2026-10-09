#include "Report.h"
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace {

// Create a map showing the drone's route
void routeSvg(
    const std::vector<Reading>& history,
    const std::vector<Point>& planned,
    Point target
) {
    std::ofstream out("assets/mission_route.svg");

    out << R"(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 780 560">
<rect width="780" height="560" rx="20" fill="#0c1424"/>
<text x="45" y="43" fill="#f5f8ff" font-size="24"
 font-family="Arial" font-weight="bold">
SEARCH &amp; RESCUE | MISSION ROUTE
</text>
<text x="45" y="68" fill="#a7b8d0" font-size="13"
 font-family="Arial">
Simulated coordinates - search route and target
</text>
)";

    auto sx = [](double x) {
        return 90 + x * 10;
    };

    auto sy = [](double y) {
        return 485 - y * 10;
    };

    // Draw the map grid
    for (int n = 0; n <= 50; n += 10) {
        if (n <= 40) {
            out << "<line x1='90' y1='" << sy(n)
                << "' x2='590' y2='" << sy(n)
                << "' stroke='#2b3b53'/>\n";
        }

        out << "<line x1='" << sx(n)
            << "' y1='85' x2='" << sx(n)
            << "' y2='485' stroke='#2b3b53'/>\n";

        out << "<text x='" << sx(n)
            << "' y='510' text-anchor='middle'"
            << " fill='#8ea5c2' font-size='12'>"
            << n << "</text>\n";

        if (n <= 40) {
            out << "<text x='67' y='" << sy(n) + 5
                << "' text-anchor='end'"
                << " fill='#8ea5c2' font-size='12'>"
                << n << "</text>\n";
        }
    }

    // Draw every planned search point
    for (const Point& p : planned) {
        out << "<circle cx='" << sx(p.x)
            << "' cy='" << sy(p.y)
            << "' r='5' fill='#4b5a73'/>\n";
    }

    // Draw the actual flight path
    for (std::size_t i = 1; i < history.size(); ++i) {
        const Reading& a = history[i - 1];
        const Reading& b = history[i];

        bool home = b.event == "RETURN_HOME";

        out << "<line x1='" << sx(a.x)
            << "' y1='" << sy(a.y)
            << "' x2='" << sx(b.x)
            << "' y2='" << sy(b.y)
            << "' stroke='"
            << (home ? "#ffc857" : "#3fe0c5")
            << "' stroke-width='4'";

        if (home) {
            out << " stroke-dasharray='8 6'";
        }

        out << "/>\n";
    }

    // Red circle = target
    out << "<circle cx='" << sx(target.x)
        << "' cy='" << sy(target.y)
        << "' r='10' fill='#ff5b70'"
        << " stroke='#ffacb6' stroke-width='2'/>\n";

    // Blue circle = home
    out << "<circle cx='" << sx(0)
        << "' cy='" << sy(0)
        << "' r='9' fill='#63aaff'"
        << " stroke='#e0f0ff' stroke-width='2'/>\n";

    out << R"(
<text x="617" y="124" fill="#f5f8ff"
 font-size="17" font-weight="bold">LEGEND</text>

<circle cx="630" cy="155" r="6" fill="#63aaff"/>
<text x="648" y="160" fill="#d6e2f3">Home</text>

<circle cx="630" cy="185" r="6" fill="#ff5b70"/>
<text x="648" y="190" fill="#d6e2f3">Target</text>

<line x1="618" y1="214" x2="641" y2="214"
 stroke="#3fe0c5" stroke-width="4"/>
<text x="650" y="219" fill="#d6e2f3">Search</text>

<line x1="618" y1="246" x2="641" y2="246"
 stroke="#ffc857" stroke-width="4"
 stroke-dasharray="7 5"/>
<text x="650" y="251" fill="#d6e2f3">Return</text>

<text x="45" y="540" fill="#788da9" font-size="12">
Educational simulation - not real aircraft navigation
</text>
</svg>
)";
}

// Create a battery OR thermal sensor graph
void graphSvg(
    const std::vector<Reading>& history,
    bool batteryGraph
) {
    std::ofstream out(
        batteryGraph
            ? "assets/battery_graph.svg"
            : "assets/thermal_graph.svg"
    );

    out << R"(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 780 390">
<rect width="780" height="390" rx="20" fill="#0c1424"/>
)";

    out << "<text x='48' y='44' fill='#f5f8ff'"
        << " font-size='24' font-weight='bold'>"
        << (batteryGraph
            ? "BATTERY LEVEL BY MISSION STEP"
            : "THERMAL SENSOR BY MISSION STEP")
        << "</text>\n";

    const double left = 75;
    const double right = 720;
    const double top = 80;
    const double bottom = 320;
    const double maxY = batteryGraph ? 100.0 : 1.0;

    // Draw horizontal grid lines
    for (int k = 0; k <= 4; ++k) {
        double value = maxY * k / 4.0;
        double y = bottom - (bottom - top) * k / 4.0;

        out << "<line x1='" << left
            << "' y1='" << y
            << "' x2='" << right
            << "' y2='" << y
            << "' stroke='#2b3b53'/>\n";

        out << "<text x='60' y='" << y + 5
            << "' text-anchor='end'"
            << " fill='#94abc9' font-size='12'>"
            << value << "</text>\n";
    }

    // Connect recorded values into a graph
    std::size_t n = history.size();

    out << "<polyline points='";

    for (std::size_t i = 0; i < n; ++i) {
        double x = left + (right - left) * i
            / (n > 1 ? n - 1 : 1);

        double value = batteryGraph
            ? history[i].battery
            : history[i].thermal;

        double y = bottom - (bottom - top)
            * value / maxY;

        out << x << "," << y << " ";
    }

    out << "' fill='none' stroke='"
        << (batteryGraph ? "#3fe0c5" : "#ffb454")
        << "' stroke-width='4'/>\n";

    // Add dots to the graph
    for (std::size_t i = 0; i < n; ++i) {
        double x = left + (right - left) * i
            / (n > 1 ? n - 1 : 1);

        double value = batteryGraph
            ? history[i].battery
            : history[i].thermal;

        double y = bottom - (bottom - top)
            * value / maxY;

        out << "<circle cx='" << x
            << "' cy='" << y
            << "' r='5' fill='#e8f8ff'/>\n";

        out << "<text x='" << x
            << "' y='345' text-anchor='middle'"
            << " fill='#94abc9' font-size='11'>"
            << history[i].step << "</text>\n";
    }

    // Thermal detection threshold
    if (!batteryGraph) {
        double y = bottom - (bottom - top) * 0.75;

        out << "<line x1='75' y1='" << y
            << "' x2='720' y2='" << y
            << "' stroke='#ff5b70'"
            << " stroke-dasharray='7 5'/>\n";
    }

    out << R"(
<text x="390" y="375" text-anchor="middle"
 fill="#94abc9" font-size="13">
Mission step
</text>
</svg>
)";
}

} // End anonymous namespace

// Save all mission data and generate graphics
void saveReports(
    const std::vector<Reading>& readings,
    const std::vector<Point>& planned,
    Point target
) {
    // Create the assets folder automatically
    std::filesystem::create_directories("assets");

    // Create spreadsheet-compatible mission log
    std::ofstream csv("mission_log.csv");

    csv << "step,x,y,battery_percent,thermal,event\n";
    csv << std::fixed << std::setprecision(2);

    for (const auto& r : readings) {
        csv << r.step << ","
            << r.x << ","
            << r.y << ","
            << r.battery << ","
            << r.thermal << ","
            << r.event << "\n";
    }

    // Create the 3 SVG images
    routeSvg(readings, planned, target);
    graphSvg(readings, true);
    graphSvg(readings, false);
}
