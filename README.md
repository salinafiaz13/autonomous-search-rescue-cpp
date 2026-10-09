Autonomous Search & Rescue Drone Simulator

A C++ project that simulates an autonomous drone searching for a missing person. The drone follows a planned search route, monitors battery levels, detects a target using simulated thermal readings, and automatically returns to home base.

Features

* Autonomous Navigation: Follows a zigzag search route using coordinates.
* Thermal Detection: Simulates identifying a missing person based on distance.
* Battery Management: Tracks energy consumption and checks whether the drone has enough battery to return safely.
* Automatic Return: Returns to home base after detecting a target or reaching its battery safety limit.
* Mission Reports: Records mission information in a CSV file.
* Visualizations: Generates graphs showing the drone’s search route, battery usage, and thermal readings.

Technologies Used

C++17, Object-Oriented Programming (OOP), CSV, SVG, and VS Code.

Mission Visualizations

Search Route

Battery Usage

Thermal Sensor

How to Run

Open the project folder in VS Code.

Compile the program in the terminal:

clang++ -std=c++17 main.cpp Drone.cpp Report.cpp -o rescue_sim

Run the simulation:

./rescue_sim

The program displays the mission results and generates a CSV report and three SVG visualizations.

Project Purpose

I created this project to strengthen my C++ programming and systems engineering skills through a practical simulation. It demonstrates how navigation, simulated sensors, battery management, and automated decision-making can work together within one system.

Note: This is an educational simulation using fictional sensor data, not a real drone control system.
