#include "readings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

int get_reading(SensorReadings *reading, FILE *filename){
    //initialize line and n, and use getline to get a single
    //line in the file
    char *line = NULL;
    size_t n = 0;
    //if getline failes, return 0 to indicate that the line wasnt read
    if(getline(&line, &n, filename) == -1){
        free(line);
        return 0;
    }

    char *saveptr = NULL;
    //--------------NAME--------------------------
    //tokenize the first part of the line to get the name
    char *token = strtok_r(line, "|", &saveptr);
    if(token == NULL){
        free(line);
        return 0;
    }
    //allocate memory for said name
    reading->design_name = malloc(sizeof(char) * strlen(token) + 1);
    if(reading->design_name == NULL){
        free(line);
        return 0;
    }
    //copy in the name into design_name for the SensorReading struct
    strcpy(reading->design_name, token);

    //---------------TIME-------------------------
    token = strtok_r(NULL, "|", &saveptr);
    //if the token reading fails or we get back the null terminator
    //indicate that the line failed
    if(token == NULL){
        free(line);
        free(reading->design_name);
        return 0;
    }
    
    reading->time = strtod(token, NULL);
    //-----------------TEMPERATURE----------------
    token = strtok_r(NULL, "|", &saveptr);

    if(token == NULL){
        free(line);
        free(reading->design_name);
        return 0;
    }

    reading->temp_c = strtod(token, NULL);
    //----------------VOLTAGE---------------------
    token = strtok_r(NULL, "|", &saveptr);

    if(token == NULL){
        free(line);
        free(reading->design_name);
        return 0;
    }

    reading->voltage = strtod(token, NULL);
    //----------------CURRENT---------------------
    token = strtok_r(NULL, "|", &saveptr);

    if(token == NULL){
        free(line);
        free(reading->design_name);
        return 0;
    }

    reading->current = strtod(token, NULL);

    free(line);
    return 1;

}

void write_to_file(const char* filename){
    //open for writing rather than appending
    FILE *data_csv = fopen(filename, "w");
    if(data_csv == NULL){
        printf("Failed to open %s\n", filename);
        return;
    }
    //do a while loop that only breaks when the user enters 'exit'
    while(1){
        //MAX is a macro of 100
        char choice[MAX];
        //get the users choice
        printf("Continue: Enter 'continue' | EXIT: Enter 'exit': ");
        fgets(choice, MAX, stdin);
        choice[strcspn(choice, "\n")] = '\0';
        //if the users choice is to exit, break the while loop
        if(strcmp(choice, "exit") == 0){
            break;
        }
        //if the user wants to continue, get a reading
        else if(strcmp(choice, "continue") == 0){
            //tell the user the format they must use as to avoid confusion
            printf("Please enter a line in the format: \n");
            printf("NAME|TIME|TEMP|VOLTAGE|CURRENT\n");
            char *line = NULL;
            size_t n = 0;
            //use getline because fgets cant take in an indefinite number
            //of characters, and would need an upper limit
            if(getline(&line, &n, stdin) == -1){
                printf("GETLINE FAILED!\n");
                free(line);
                continue;
            }
            //print the users line to the file
            fprintf(data_csv, "%s", line);
            //free the dynamically allocated lines memory
            free(line);
        }
        //if the user doesnt enter exit or continue, tell them to
        else{
            printf("Please only enter 'continue' or 'exit'\n");
            continue;
        }
    }
    fclose(data_csv);
}

void print_data(SensorLog *log){
    //simply print all of our readings
    for(int i = 0; i < log->size; i++){
        printf("------READING %d-------\n", i+1);
        printf("DESIGN NAME: %s\n", log->data[i].design_name);
        printf("TIME: %.2lf\n", log->data[i].time);
        printf("TEMPERATURE: %.2lf\n", log->data[i].temp_c);
        printf("VOLTAGE: %.2lf\n", log->data[i].voltage);
        printf("CURRENT: %.2lf\n", log->data[i].current);
    }
}

void free_memory(SensorLog *log){
    //free the design name for each data[i]
    for(int i = 0; i < log->size; i++){
        free(log->data[i].design_name);
    }
    //free the final data array for log
    free(log->data);
}
Result compute(const SensorLog *log){
    Result result;
    //initialize for our totals to get averages later
    double total_voltage = 0;
    double total_current = 0;
    double total_time = 0;
    double total_temp = 0;
    //use count to get averages
    double count = 0;
    double max_voltage = 0;
    double total_power = 0;
    double min_voltage = 100000000000000000;
    for(int i = 0; i < log->size; i++){
        //add to each total
        total_voltage += log->data[i].voltage;
        total_current += log->data[i].current;
        total_time += log->data[i].time;
        total_temp += log->data[i].temp_c;
        total_power += log->data[i].voltage * log->data[i].current;
        //update min/max voltage if smaller/greater
        if(log->data[i].voltage > max_voltage){
            max_voltage = log->data[i].voltage;
        }
        if(log->data[i].voltage < min_voltage){
            min_voltage = log->data[i].voltage;
        }
        //increment count
        count++;
    }

    result.avg_time = total_time / count;
    result.avg_power = total_power / count;
    result.total_energy = total_power * total_time;
    result.avg_temp = total_temp / count;
    result.max_voltage = max_voltage;
    result.min_voltage = min_voltage;
    
    return result;

}

int detect_bad_readings(SensorReadings *reading, double upper_volt, 
        double lower_volt, double upper_current, double lower_current,
        double temp){
    //pass in the upper and lower bounds for the voltage/current
    //taken from user input in main to determine which of the readings
    //is bad
    if(reading->voltage > 1.5 * upper_volt){
        //indicates a voltage spike
        return 3;
    }
    else if(reading->current > 1.5 * upper_current){
        //indicates a current spike
        return 2;
    }
    else if(reading->temp_c > temp){
        //indicate overheating
        return 4;
    }
    else if(reading->voltage > upper_volt || reading->voltage < lower_volt){
        //indicates a simple bad reading so I can separate them from spikes
        return 0;
    }
    else if(reading->current > upper_current || reading->current < lower_current){
        return 0;
    }

    //indicates a good reading
    return 1;


}
