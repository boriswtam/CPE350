#ifndef RUN_MOTOR_H
#define RUN_MOTOR_H

struct run_motor_struct {
        int supply_voltage;
	float duty_cycle;
	int frequency;
};

void run_motor(int handle, struct run_motor_struct * rms);

#endif
