#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define CURRENT_GOOD 20
#define CURRENT_BAD 40
#define ACCELEROMETER_GOOD 0
#define ACCELEROMETER_BAD 2
#define RPM_GOOD 40
#define RPM_BAD 25
#define TEMP_GOOD 90
#define TEMP_BAD 140
#define MIC_GOOD 70
#define MIC_BAD 130
#define NUMBER_LINES 10000

float getRPM(int * fault);
float getTemp(int * fault);
float getMic(int * fault);
float ranf();
float getCurrent(int * fault);
float getAccel(int * fault);
float getRPM(int * fault);
void printLine();
float box_muller(float m, float s);

int main() {
    int i;
    for (i = 0; i < NUMBER_LINES; i++) {
        printLine();
    }
    return 0;
}

void printLine() {
    int fault;
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    fault = 0;
    current = getCurrent(&fault);
    accelerometerx = getAccel(&fault);
    accelerometery = getAccel(&fault);
    accelerometerz = getAccel(&fault);
    rpm = getRPM(&fault);
    temp = getTemp(&fault);
    mic = getMic(&fault);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

float ranf() {
    return ((float)rand() / (float)RAND_MAX);
}

float getCurrent(int * fault) {
    int rand_select;
    float current;
    rand_select = rand() % 100;
    if (rand_select < 10) {
        current = box_muller((float)CURRENT_BAD, 3.0);
        *fault = 1;
    }
    else {
        current = box_muller((float)CURRENT_GOOD, 1.0);
    }
    return current;
}

float getAccel(int * fault) {
    int rand_select;
    int rand_select2;
    float accelerometer;
    rand_select = rand() % 100;
    rand_select2 = rand() % 2;
    if (rand_select < 10) {
        if (rand_select2) {
            accelerometer = box_muller((float)ACCELEROMETER_BAD, 3.0);
        }
        else {
            accelerometer = -1 * box_muller((float)ACCELEROMETER_BAD, 3.0);
        }
        *fault = 1;
    }
    else {
        if (rand_select2) {
            accelerometer = box_muller((float)ACCELEROMETER_GOOD, 1.0);
        }
        else {
            accelerometer = -1 * box_muller((float)ACCELEROMETER_GOOD, 1.0);
        }
    }
    return accelerometer;
}

float getRPM(int * fault) {
    int rand_select;
    float rpm;
    rand_select = rand() % 100;
    if (rand_select < 10) {
        rpm = box_muller((float)RPM_BAD, 3.0);
        *fault = 1;
    }
    else {
        rpm = box_muller((float)RPM_GOOD, 1.0);
    }
    return rpm;
}

float getTemp(int * fault) {
    int rand_select;
    float temp;
    rand_select = rand() % 100;
    if (rand_select < 10) {
        temp = box_muller((float)TEMP_BAD, 3.0);
        *fault = 1;
    }
    else {
        temp = box_muller((float)TEMP_GOOD, 1.0);
    }
    return temp;
}

float getMic(int * fault) {
    int rand_select;
    float mic;
    rand_select = rand() % 100;
    if (rand_select < 10) {
        mic = box_muller((float)MIC_BAD, 3.0);
        *fault = 1;
    }
    else {
        mic = box_muller((float)MIC_GOOD, 1.0);
    }
    return mic;
}

/* Box Muller funciton taken from ftp://ftp.taygeta.com/pub/c/boxmuller.c */

float box_muller(float m, float s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}
