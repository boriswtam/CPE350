#include <stdio.h>
#include <stdlib.h>

void printColSection(int num);
void printLine(int num);
void printAccelerometer();
void printRPMSensor();
void printTempSensor();
void printCurrentSensor();
void printMic();
void printFaults();

int main(int argc, char *  argv[]) {
   printAccelerometer();
   printRPMSensor();
   printTempSensor();
   printCurrentSensor();
   printMic();
   printFaults();
   printLine(96);
   
}

void printFaults() {
   printLine(96);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printf("Faults");
   printColSection(17);
   printf(" Fault 1:");
   printColSection(14);
   printf(" Fault 2:");
   printColSection(14);
   printf(" Fault 3:");
   printColSection(14);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
}

void printMic() {
   printLine(96);
   printColSection(23);
   printColSection(71);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
   printf("Sound Detector");
   printColSection(9);
   printf(" Hz:");
   printColSection(67);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
}

void printCurrentSensor() {
   printLine(96);
   printColSection(23);
   printColSection(71);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
   printf("Current Sensor");
   printColSection(9);
   printf(" Amps:");
   printColSection(65);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
   printColSection(23);
   printColSection(71);
   printf("\n");
}

void printTempSensor() {
   printLine(96);
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printf("Temp Sensor");
   printColSection(12);
   printf(" deg F:");
   printColSection(28);
   printf(" deg C:");
   printColSection(28);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
}

void printRPMSensor() {
   printLine(96);
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printf("RPM Sensor");
   printColSection(13);
   printf(" Motor RPM:");
   printColSection(24);
   printf(" Prop RPM:");
   printColSection(25);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
   printColSection(23);
   printColSection(35);
   printColSection(35);
   printf("\n");
}

void printAccelerometer() {
   printLine(96);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printf("Accelerometer");
   printColSection(10);
   printf(" x:");
   printColSection(20);
   printf(" y:");
   printColSection(20);
   printf(" z:");
   printColSection(20);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printColSection(23);
   printf("\n");
}


void printColSection(int num) {
   int i;
   for (i = 0; i < num; i++) {
      printf(" ");
   }
   printf("|");
}

void printLine(int num) {
   int i;
   for (i = 0; i < num; i++) {
      printf("-");
   }
   printf("\n");
}
