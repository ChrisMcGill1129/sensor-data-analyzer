#ifndef READINGS_H
#define READINGS_H
#include <stdio.h>

typedef struct{
    double time;
    double temp_c;
    char *design_name;
    double voltage;
    double current;
} SensorReadings;

typedef struct{
    double avg_power;
    double max_voltage;
    double min_voltage;
    double avg_time;
    double avg_temp;
    double total_energy;
} Result;

typedef struct{
    SensorReadings *data;
    int size;
    int capacity;
} SensorLog;

//use a .csv file, use getline and strtok to tokenize each chunk in each line
//separated by a '|' in order to create a single SensorReadings struct that
//will be added to the SensorLog struct in main.c, return 1 on success, 0
//on failure to read line
int get_reading(SensorReadings *reading, FILE *filename);
//will compute the average and any other information/computations needed
//or possible for a given SensorReadings struct by doing a for loop
//and passing in log->data[i] into compute
Result compute(const SensorLog *log);
//once more, pass in a single log->data[i] into this function, and then
//it will scan through each object in said struct to determine whether
//or not its a bad reading based on the data provided
int detect_bad_readings(SensorReadings *reading, double upper_voltage,
        double lower_voltage, double upper_current, double lower_current,
        double temp);
//will use it to open the file for writing using a while loop, until the user
//wishes to exit and for each loop, the user will enter a string with the
//structure: design name|time|temp|voltage|current
void write_to_file(const char *filename);
//use a while loop to loop through the array of SensorReadings structs in
//our data array and print them to the terminal
void print_data(SensorLog *log);
//will go through our log struct, and free log->data[i].design_name, and then
//free the memory for the data array
void free_memory(SensorLog *log);

#endif
