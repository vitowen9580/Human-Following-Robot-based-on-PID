
/*
 * kp:overshoot,ki:steady state error,kd:rising time
 */
unsigned long nowTime,lastTime;
double timeChange;
float kp = 0.08, ki = 0.000, kd =0.01 ;

double  errSum=0,lastErr=0,Output; 

int PID(int angle)
{

  nowTime=millis();
  timeChange = (double)(nowTime - lastTime);

  double error=0-angle;
  errSum += (error * timeChange);

  double dErr = (error - lastErr) / timeChange;

  Output = kp * error + ki * errSum + kd * dErr;

  lastErr = error;


  lastTime = nowTime;
  return abs(Output);
}
 
