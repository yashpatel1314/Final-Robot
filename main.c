void SensConfig()
{
  SensorType[S1]=sensorEV3_Touch;
  wait1Msec(50);
  SensorType[S2]=sensorEV3_Ultrasonic;
  wait1Msec(50);
  SensorType[S3]=sensorEV3_Color;
  wait1Msec(50);
  SensorType[S4]=sensorEV3_Gyro;
  wait1Msec(50);
  return;
}
float drive_to(int motor_power)//Driving to the black tape
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = motor_power;
	float encoder_value1 = 0;
	while(getColorName(S3)!= colorBlack)
	{
		if(SensorValue[S2] < 3)//Obstacle check loop
		{
			motor[motorA]= motor[motorD] = 0;
			wait1Msec(2000);
			encoder_value1 = nMotorEncoder[motorA];
			motor[motorA] = motor[motorD] = -motor_power;
			while(nMotorEncoder[motorA] > 0)
			{}
			motor[motorA] = motor[motorD] = 0;
			wait1Msec(2000);
			nMotorEncoder[motorA] = 0;
			motor[motorA]= motor[motorD] = motor_power;
		}
	}
	motor[motorA]=motor[motorD] = 0;
	return 2*encoder_value1;
}
float drive_back (int motorpower)//Driving back from the block
{
	float distance = nMotorEncoder[motorA];
	motor[motorA] = motor[motorD] = -motorpower;
	while(nMotorEncoder[motorA] > 0)
	{}
	motor[motorA] = motor[motorD] = 0;
	return 2*distance;
}
void rotate_rob_colour(int motorpower, int color)//Rotates robot until it                                                   sees a certain colour
{
	motor[motorA] = motorpower;
	motor[motorD] = -motorpower;
	while(getColorName(S3) != color)
	{}
	motor[motorA] = motor[motorD] = 0;
}
void claw_close(int motor_power)//Close the claw
{
	motor[motorB] = motor_power;
}
void claw_open(int motor_power)//Opens the claw
{
	nMotorEncoder[motorB] = 0;
	motor[motorB] = -motor_power;
	while(nMotorEncoder[motorB]> -75)
	{}
	motor[motorB] = 0;
	return;
}
void fork_op(int motorpower, bool dir, int multiplier)//Move the fork up                                                         or down
{
	if(dir == 0)//Move the crane up
	{
		motor[motorC] = -motorpower;
		while(SensorValue[S1] == 0)
		{}
		motor[motorC] = 0;
	}
	else//Move the crane down
	{
		int enc_clicks = -nMotorEncoder[motorC];
		nMotorEncoder[motorC] = 0;
		motor[motorC] = motorpower;
		while((multiplier*enc_clicks) > nMotorEncoder[motorC])
		{}
		motor[motorC] = 0;
	}
	return;
}
float encoderlength (float encoder1, float encoder2, float encoder3, float encoder7, float encoder8, float encoder9)//adds up all encoder values                                               and converts them to total distance
{
	float total_distance = (encoder1 + encoder2 + encoder3 +
	encoder7 + encoder8 + encoder9)*(PI*2.75)/180;
	return total_distance;
}
float averageSpeedCalc(int total_distance, int time)//Dividing distance                                                       by time, calculating the speed
{
	float averageSpeed = (total_distance)/(time);
	return averageSpeed;
}

task main()
{
	time1[T1] = 0;
	SensConfig();
	//Initialization of variables
	float encoder1 = 0;
	float encoder2 = 0;
	float encoder3 = 0;
	float encoder_first = 0;
	float encoder_red = 0;
	float encoder_blue = 0;
	//Start-up Procedure
	while(!getButtonPress(buttonEnter))
  	{}
	while(getButtonPress(buttonEnter))
	{}
	//First Leg(Blue)
	encoder_blue = drive_to(30);
	claw_close(15);
	wait1Msec(2000);
	fork_op(100,0,1);
	wait1Msec(1000);
	encoder1 = drive_back(30);
	rotate_rob_colour(20,colorGreen);
	while(getColorName(S3) != colorGreen)
	{}
	nMotorEncoder[motorD] = 0;
	motor[motorA] = motor[motorD] = 30;
	while(15 > (nMotorEncoder[motorD]*2.75*PI)/(180))
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(2000);
	fork_op(20,1,1);
	wait1Msec(1000);
	claw_open(15);
	wait1Msec(1000);
	motor[motorA] = motor[motorD] = -30;
	while(nMotorEncoder[motorD] > 0)
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);
	//Second Leg(Red)
	rotate_rob_colour(20,colorRed);
	while(getColorName(S3) != colorRed)
	{}
	encoder_red = drive_to(30);
	claw_close(15);
	wait1Msec(2000);
	fork_op(100,0,1);
	wait1Msec(1000);
	encoder2 = drive_back(30);
	rotate_rob_colour(20,colorGreen);
	while(getColorName(S3) != colorGreen)
	{}
	nMotorEncoder[motorD] = 0;
	motor[motorA] = motor[motorD] = 30;
	while(15 > (nMotorEncoder[motorD]*2.75*PI)/(180))
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(2000);
	fork_op(20,1,1);
	wait1Msec(1000);
	claw_open(15);
	wait1Msec(1000);
	motor[motorA] = motor[motorD] = -30;
	while(nMotorEncoder[motorD] > 0)
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);
	//Third Leg (White)
	rotate_rob_colour(-20,colorWhite);
	nMotorEncoder[motorD] = 0;
	motor[motorA] = motor[motorD] = 30;
	while(30 > (nMotorEncoder[motorD]*2.75*PI)/(180))
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);
	motor[motorA] = motor[motorD] = -30;
	while(0 < nMotorEncoder[motorD])
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(2000);
	encoder_blue = drive_to(30);
	claw_close(15);
	wait1Msec(2000);
	fork_op(100,0,1);
	wait1Msec(1000);
	encoder3 = drive_back(30);
	rotate_rob_colour(20,colorGreen);
	while(getColorName(S3) != colorGreen)
	{}
	nMotorEncoder[motorD] = 0;
	motor[motorA] = motor[motorD] = 30;
	while(15 > (nMotorEncoder[motorD]*2.75*PI)/(180))
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(2000);
	fork_op(20,1,1);
	wait1Msec(1000);
	claw_open(15);
	wait1Msec(1000);
	motor[motorA] = motor[motorD] = -30;
	while(nMotorEncoder[motorD] > 0)
	{}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);

	float total_distance = encoderlength(encoder1, encoder2, encoder3,         encoder_first, encoder_red , encoder_blue);
	float final_average_speed = averageSpeedCalc(total_distance, time1[T1]);
	displayString(3,"Total Distance:", "%f", total_distance);
	displayString(4,"Total Time: ", "i", time1[T1]);
	displayString(5,"Average Speed: ", "f", final_average_speed);
	wait1Msec(20000);
}
