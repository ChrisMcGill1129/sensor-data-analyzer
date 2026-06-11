# sensor-data-analyzer
A C program that reads sensor data from files, computes statistics, and detects abnormal readings.


Sensor Data Analyzer

A C project that reads sensor measurements from a file, stores them in a dynamically growing array, computes statistics, and detects abnormal readings.

Features
Dynamic memory allocation using malloc, realloc, and free
File I/O using fopen and getline
Parsing sensor records with strtok_r
Statistical analysis
Voltage spike detection
Current spike detection
Overheat detection
Valgrind-clean memory management
Example Input

Project 1|30|44|4.4|0.2

Project 2|13|47|2.2|0.4

Technologies
C
GCC
Valgrind
Linux
Build

gcc -g -Wall -Wextra -o sensor main.c readings.c

Run

./sensor
