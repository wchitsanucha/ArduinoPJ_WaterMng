#include <Wire.h>
#include <TimeLib.h>

int pump1_state = 1;
int pump2_state = 1;

//Input pin
const int WS1 = D5;
const int WS2 = D6;

//Output pin
const int PUMP_1 = D7;
const int PUMP_2 = D8;

const int setHour = 7;
const int setMinute = 15;

void setup()
{
	pinMode(WS1, INPUT);
	pinMode(WS2, INPUT);
	pinMode(PUMP_1, OUTPUT);
	pinMode(PUMP_2, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	if(time_check() && pump1_state && pump2_state)
	{
		water_pump(PUMP_1, 1);
		water_pump(PUMP_2, 1);
		while(pump1_state || pump2_state)
		{
			sensor_check();
		}
	}
	else
	{
		water_pump(PUMP_1, 0);
		water_pump(PUMP_2, 0);
	}
}

int time_check()
{
	if(RTC.isrunning())
	{
		DateTime now = RTC.now();
		if(now.hour() == setHour && now.minute() >= setMinute)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if(hour() == setHour && minute() >= setMinute)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		
	}

	Serial.println("Fault to read time.");
	return 0;
}

void sensor_check()
{
	if(digitalRead(WS1) == 0)
	{
		water_pump(PUMP_1, 0);
	}
	else
	{
		//Do nothing
	}

	if(digitalRead(WS2) == 0)
	{
		water_pump(PUMP_2, 0);
	}
	else
	{
		//Do nothing
	}
}

void water_pump(int pump_number, int state)
{
	if(pump_number == PUMP_1)
	{
		digitalWrite(PUMP_1, state);
		pump1_state = state;
	}
	else if(pump_number == PUMP_2)
	{
		digitalWrite(PUMP_2, state);
		pump2_state = state;
	}
	else
	{
		//Do nothing
	}
}