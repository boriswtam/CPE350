#include "ADXL345.h"

int init_ADXL345() {
	int file;
	char *filename = "/dev/i2c-2";
	if ((file = open(filename, O_RDWR)) < 0) {
    		/* ERROR HANDLING: you can check errno to see what went wrong */
    		perror("Failed to open the i2c bus");
   		exit(1);
	}	

	if (ioctl(file, I2C_SLAVE, ADXL345_ADDR) < 0) {
    		printf("Failed to acquire bus access and/or talk to slave.\n");
    		/* ERROR HANDLING; you can check errno to see what went wrong */
    		exit(1);
	}

	char buf[10] = {0};
	float data;
	char channel;

	for (int i = 0; i<4; i++) {
	    // Using I2C Read
	    if (read(file,buf,2) != 2) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to read from the i2c bus.\n");
		//buffer = g_strerror(errno);
		//printf(buffer);
		printf("\n\n");
	    } else { // Needs to be modified for device specifics
		data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
		data = data/4096*5;
		channel = ((buf[0] & 0b00110000)>>4);
		printf("Channel %02d Data:  %04f\n",channel,data);
	    }
	}

	//unsigned char reg = 0x10; // Device register to access
	//buf[0] = reg;

	buf[0] = 0b11110000;
	if (write(file,buf,1) != 1) {
	    /* ERROR HANDLING: i2c transaction failed */
	    printf("Failed to write to the i2c bus.\n");
	    //buffer = g_strerror(errno);
	    //printf(buffer);
	    printf("\n\n");
	}
}

int main(int argc, char * argv[]) {
	init_ADXL345();
}
