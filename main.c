#include "readings.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 2

int main(){
    SensorLog *log = malloc(sizeof(SensorLog));
    if(log == NULL){
        return 0;
    }
    log->capacity = INITIAL_CAPACITY;
    log->data = malloc(sizeof(SensorReadings) * log->capacity);
    if(log->data == NULL){
        free(log);
        return 0;
    }
    log->size = 0;
    char filename[100];
    printf("What is the file name you wish to use: ");
    if(fgets(filename, 100, stdin) == NULL){
        free(log);
        printf("FAILED TO GET FILE NAME!\n");
        return 0;
    }
    filename[strcspn(filename, "\n")] = '\0';
    write_to_file(filename);

    FILE *data_csv = fopen(filename, "r");
    if(data_csv == NULL){
        free(log);
        return 0;
    }

    while(1){
        SensorReadings temp_reading;
        if(log->size > log->capacity){
            log->capacity += 2;
            log->data = realloc(log->data, sizeof(SensorReadings) * 
                    log->capacity);
        }
        if(get_reading(&temp_reading, data_csv) != 1){
            break;
        }
        log->data[log->size] = temp_reading;
        log->size++;
    }
    fclose(data_csv);

    print_data(log);

    int voltage_spike = 0;
    int current_spike = 0;
    int bad_readings = 0;
    int good_readings = 0;

    int overheat = 0;
    //upper/lower voltage/current constraints
    double up_volt;
    double low_volt;
    double up_cur;
    double low_cur;
    double safe_temp;
    printf("Maximum safe voltage: ");
    scanf("%lf", &up_volt);
    printf("Minimum safe voltage: ");
    scanf("%lf", &low_volt);
    printf("Maxmimum safe current: ");
    scanf("%lf", &up_cur);
    printf("Minimum safe current: ");
    scanf("%lf", &low_cur);
    printf("Safe temperature: ");
    scanf("%lf", &safe_temp);



    Result result = compute(log);
    
    for(int i = 0; i < log->size; i++){
        
        int status = detect_bad_readings(&log->data[i], up_volt, low_volt,
                up_cur, low_cur, safe_temp);
        if(status == 4){
            overheat += 1;
        }
        else if(status == 2){
            current_spike += 1;
        }
        else if(status == 3){
            voltage_spike += 1;
        }
        else if(status == 1){
            bad_readings += 1;
        }
        else{
            good_readings += 1;
        }

        
    }
    printf("-----------------SUMMARY--------------------\n");
    printf("OVERHEAT: %d\n", overheat);
    printf("VOLTAGE SPIKE: %d\n", voltage_spike);
    printf("CURRENT_SPIKE: %d\n", current_spike);
    printf("GOOD READINGS: %d\n", good_readings);
    printf("BAD READINGS: %d\n", bad_readings);
    printf("MAX VOLTAGE: %.2lf V\n", result.max_voltage);
    printf("MIN CURRENT: %.2lf A\n", result.min_voltage);
    printf("AVERAGE TIME: %.2lf s\n", result.avg_time);
    printf("AVERAGE POWER: %.2lf W\n", result.avg_power);
    printf("TOTAL ENERGY: %.2lf J\n", result.total_energy);

    free_memory(log);
    free(log);
        
}
