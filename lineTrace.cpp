#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"
#include "mbed.h"
#include "app_config.h"
#include "Zumo.h"
#include "Milkcocoa.h"

extern void onpush(MQTT::MessageData &md);
Serial pc(USBTX, USBRX);
Zumo zumo;

static int speed = 80;
static int steer = 0;

void onpush(MQTT::MessageData &md)
{
	pc.printf("onpush\n\r");
	MQTT::Message &message = md.message;
	DataElement de = DataElement((char *)message.payload);
	int v = de.getInt("z");
}

const int normalSpeed = 100;
const int refreshFlame = 1;
int leftSpeed;
int rightSpeed;
int fixSpeed = 4;

void task_main(intptr_t exinf)
{
	IrBitField_T irbits;
	leftSpeed = fixSpeed;
	rightSpeed = fixSpeed;
	while (1)
	{
		changeSpeed();
		zumo.driveTank(leftSpeed, rightSpeed);
		dly_tsk(refreshFlame);
	}
}

void changeSpeed()
{
	zumo.readIr(irbits);
	if (irbits.center * irbits.right * irbits.left)
	{
		leftSpeed = 0;
		rightSpeed = 0;
	}
	else if (irbits.center)
	{
		leftSpeed -= fixSpeed;
		rightSpeed += fixSpeed;
	}
	else if (irbits.right)
	{
		leftSpeed += fixSpeed;
		rightSpeed -= fixSpeed;
	}
	else
	{
		if (leftSpeed != rightSpeed)
		{
			leftSpeed = normalSpeed;
			rightSpeed = normalSpeed;
		}
		leftSpeed += fixSpeed;
		rightSpeed += fixSpeed;
	}
}
