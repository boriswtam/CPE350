#ifndef RPM_SENSOR_H
#define RPM_SENSOR_H

struct rpm_sensor_struct {
	int rpm;
	int handle;
};

void rpm_sensor_init(struct rpm_sensor_struct * rpm_sensor);

#endif
