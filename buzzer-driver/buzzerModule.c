#include "buzzerModule.h"

void buzzer(int passedFile)
{
	char *noAlarm = "0";
	int i = 0, ret = 0;
	char buffer[1];

	// turn on buzzer for three seconds and turn off for 1 second... repeat
	for(i = 0; i < 3; i++)
	{
		digitalWrite(buzzerPin, HIGH);
		sleep(2);
		digitalWrite(buzzerPin, LOW);
		sleep(1);
	}
	
	// reset file
	if(alarmLock == 0)
	{
		alarmLock = 1;
//		pthread_mutex_lock(&alarmLock);
		lseek(passedFile, 0, SEEK_SET);
		ret = write(passedFile, noAlarm, 1);
//		pthread_mutex_lock(&alarmLock);
		alarmLock = 0;
	}
	if(ret == -1)
		syslog(LOG_ERR, "file was not reset to 0");
}

int main()
{
	char buffer[1];
	char *fileStarter = "0";
	int ret = 3, buzzerFile;

	if(wiringPiSetup() == -1)
	{
		syslog(LOG_ERR, "wiringPi failed");
		return -1;
	}
	
	pinMode(buzzerPin, OUTPUT);
	digitalWrite(buzzerPin, LOW);
	
	// setup file
	if((buzzerFile = open("/bin/buzzerAlarm", O_RDWR | O_CREAT, 0777)) == -1)
	{
		syslog(LOG_ERR, "buzzer file was not found or opened");
		return -1;
	}
	
	// initialize file
	lseek(buzzerFile, 0, SEEK_SET);
	write(buzzerFile, fileStarter, 1);
	
	while(1)
	{
		// check file for alarm
		if(alarmLock == 0)
		{
			alarmLock = 1;
//			pthread_mutex_unlock(&alarmLock);
			lseek(buzzerFile, 0, SEEK_SET);
			ret = read(buzzerFile, buffer, 1);
//			pthread_mutex_unlock(&alarmLock);
			alarmLock = 0;
		}
		
		if(ret == 0)
			syslog(LOG_ERR, "nothing in buzzer file to read");
		if(ret == -1)
			syslog(LOG_ERR, "buzzer file read failed");

		ret = atoi(buffer);
		// file contains "1"
		if(ret == 1)
			buzzer(buzzerFile);
	}	

	close(buzzerFile);
}
