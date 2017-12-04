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

float ranf();
float box_muller(float m, float s);
void generateFault1();
void generateFault2();
void generateFault3();
void generateFault4();
void generateFault5();
void generateNoFault();
void createLine(int d);

int main() {
    int i;
    int randNumber;
    for (i = 0; i < NUMBER_LINES; i++) {
        randNumber = rand() % 10;
        createLine(randNumber);
    }
    return 0;
}

void createLine(int d) {
    switch (d) {
    
    case 0:
        generateFault1();
        break;
    case 1:
        generateFault2();
        break;
    case 2:
        generateFault3();
        break;
    case 3:
        generateFault4();
        break;
    case 4:
        generateFault5();
        break;
    default:
        generateNoFault();
        break;
    }
}
     

float ranf() {
    return ((float)rand() / (float)RAND_MAX);
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

void generateFault1() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 1;
    current = box_muller(CURRENT_BAD, 3.0);
    accelerometerx = box_muller(ACCELEROMETER_BAD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_GOOD, 1.0);
    rpm = box_muller(RPM_GOOD, 1.0);
    temp = box_muller(TEMP_GOOD, 1.0);
    mic = box_muller(MIC_GOOD, 1.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault); 
}

void generateFault2() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 1;
    current = box_muller(CURRENT_GOOD, 1.0);
    accelerometerx = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_BAD, 3.0);
    rpm = box_muller(RPM_GOOD, 1.0);
    temp = box_muller(TEMP_GOOD, 1.0);
    mic = box_muller(MIC_GOOD, 1.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

void generateFault3() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 1;
    current = box_muller(CURRENT_GOOD, 1.0);
    accelerometerx = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_GOOD, 1.0);
    rpm = box_muller(RPM_BAD, 2.0);
    temp = box_muller(TEMP_GOOD, 1.0);
    mic = box_muller(MIC_BAD, 2.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

void generateFault4() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 1;
    current = box_muller(CURRENT_BAD, 2.0);
    accelerometerx = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_GOOD, 1.0);
    rpm = box_muller(RPM_GOOD, 1.0);
    temp = box_muller(TEMP_BAD, 2.0);
    mic = box_muller(MIC_GOOD, 1.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

void generateFault5() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 1;
    current = box_muller(CURRENT_GOOD, 1.0);
    accelerometerx = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_GOOD, 1.0);
    rpm = box_muller(RPM_BAD, 3.0);
    temp = box_muller(TEMP_GOOD, 1.0);
    mic = box_muller(MIC_GOOD, 1.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

void generateNoFault() {
    float current;
    float accelerometerx;
    float accelerometery;
    float accelerometerz;
    float rpm;
    float temp;
    float mic;
    int fault;
    fault = 0;
    current = box_muller(CURRENT_GOOD, 1.0);
    accelerometerx = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometery = box_muller(ACCELEROMETER_GOOD, 1.0);
    accelerometerz = box_muller(ACCELEROMETER_GOOD, 1.0);
    rpm = box_muller(RPM_GOOD, 1.0);
    temp = box_muller(TEMP_GOOD, 1.0);
    mic = box_muller(MIC_GOOD, 1.0);
    printf("%f,%f,%f,%f,%f,%f,%f,%d\n", current, accelerometerx, accelerometery, accelerometerz, rpm, temp, mic, fault);
}

