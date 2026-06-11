# Sensor Data Analyzer

A C program that reads sensor measurements from a file, stores them in a dynamically growing array, computes statistics, and detects abnormal sensor readings.

## Features

- Dynamic memory allocation using `malloc`, `realloc`, and `free`
- File input/output using `fopen`, `getline`, `fprintf`, and `fclose`
- Parsing sensor records using `strtok_r`
- Dynamic array resizing as more readings are added
- Statistical calculations
  - Maximum voltage
  - Minimum current
  - Average time
  - Average power
  - Total energy
- Detection of:
  - Voltage spikes
  - Current spikes
  - Overheating conditions
  - Bad readings
- Valgrind-clean memory management

## Technologies Used

- C
- GCC
- Linux
- Git
- GitHub
- Valgrind

## Build

Compile the program with:

```bash
gcc -g -Wall -Wextra -o sensor main.c readings.c
```

## Run

```bash
./sensor
```

## Example Input

```text
Project 1|30|44|4.4|0.2
Project 2|13|47|2.2|0.4
```

Format:

```text
NAME|TIME|TEMPERATURE|VOLTAGE|CURRENT
```

## Example Output

```text
OVERHEAT: 0
VOLTAGE SPIKE: 0
CURRENT SPIKE: 0
GOOD READINGS: 1
BAD READINGS: 1

MAX VOLTAGE: 4.40 V
MIN CURRENT: 0.20 A
AVERAGE TIME: 21.50 s
AVERAGE POWER: 0.88 W
TOTAL ENERGY: 75.68 J
```

## What I Practiced

This project was created to practice:

- Structs
- Pointers
- Dynamic memory management
- File processing
- String tokenization
- Error handling
- Git and GitHub workflows
