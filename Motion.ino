

void _Stop()
{
    analogWrite(Motor.Motor_Pin[0], 0);
    analogWrite(Motor.Motor_Pin[1], 0);
    Serial.println("Stop");

}

void Backward(int Delta_Left_PWM,int Delta_Right_PWM)
{
    int pwm=Bandwidth2PWM(Motor.Backward_min_speed_us);
    analogWrite(Motor.Motor_Pin[0], pwm+Delta_Left_PWM);
    analogWrite(Motor.Motor_Pin[1], pwm+Delta_Right_PWM);
    Serial.println("Backward");
}

void Forward(int Delta_Left_PWM,int Delta_Right_PWM)
{
  //    max_PWM=37~min_PWM=45 越小速度越快

    int pwm=Bandwidth2PWM(Motor.Foward_intial_speed_L_us);
    analogWrite(Motor.Motor_Pin[0], pwm-Delta_Left_PWM);
    analogWrite(Motor.Motor_Pin[1], pwm-Delta_Right_PWM);
    Serial.println("Forward");
}
 void Left(int Delta_Left_PWM,int Delta_Right_PWM)
{
    int pwm=Bandwidth2PWM(Motor.Foward_intial_speed_L_us);
    analogWrite(Motor.Motor_Pin[0], pwm-Delta_Left_PWM);
    analogWrite(Motor.Motor_Pin[1], pwm-Delta_Right_PWM);
    Serial.println("Left");
}
void Right(int Delta_Left_PWM,int Delta_Right_PWM)
{
    int pwm=Bandwidth2PWM(Motor.Foward_intial_speed_L_us);
    analogWrite(Motor.Motor_Pin[0], pwm-Delta_Left_PWM);
    analogWrite(Motor.Motor_Pin[1], pwm-Delta_Right_PWM);
    Serial.println("Right");

}

void Follow_Mode(String * angle,String* distance){

  /*================================
    -- L_Wheel        Angle>0
   ------Car--------------------
    -- R_Wheel        Angle<0
    
    IF Angle>0,Then Turn Left
    ELSE Then Turn Right
    ===============================
  */
  if ((*distance).toInt()>Motor.Start_Follow_Distance){

        if((*angle).toInt()>-Motor.Limit_Angle && (*angle).toInt()<Motor.Limit_Angle){
                         /*
                          * ================================
                            Forward
                            IF Constant_Velocity_Distance>distance>Start_Follow_Distance:Velocity constant
                            ELSE IF distance>Constant_Velocity_Distance:Acceleration
                            ELSE IF distance<Start_Follow_Distance:Stop
                            ===============================
                         */
                     if((*distance).toInt()<=Motor.Constant_Velocity_Distance &&(*distance).toInt()>Motor.Start_Follow_Distance){
                         Delta_Left_PWM=0;
                         Delta_Right_PWM=0;
                         Forward(Delta_Left_PWM,Delta_Right_PWM);
        
                    }else if((*distance).toInt()>Motor.Constant_Velocity_Distance){
                        Delta_Left_PWM=map((*distance).toInt()-Motor.Constant_Velocity_Distance,0,50,0,3);
                        Delta_Right_PWM=map((*distance).toInt()-Motor.Constant_Velocity_Distance,0,50,0,3);
                        Forward(Delta_Left_PWM,Delta_Right_PWM);
  
                    }
        }else if((*angle).toInt()<-Motor.Limit_Angle){
                      Delta_Right_PWM=PID((*angle).toInt());
                      Delta_Left_PWM=-int(Delta_Right_PWM/3);
  
                      PWM_protect(&Delta_Left_PWM,&Delta_Right_PWM);
                      Left(Delta_Left_PWM,Delta_Right_PWM);
  
        }else if((*angle).toInt()>Motor.Limit_Angle){
                      Delta_Left_PWM=PID((*angle).toInt());
                      Delta_Right_PWM=-int(Delta_Left_PWM/3);
  
                      PWM_protect(&Delta_Left_PWM,&Delta_Right_PWM);
                      Right(Delta_Left_PWM,Delta_Right_PWM);
                      Serial.print("turn Right_");
  
        }
  
  }else{
    _Stop();                    
  }
      #ifdef showdata
        int pwm=Bandwidth2PWM(Motor.Foward_intial_speed_L_us);
        Serial.print("Left_PWM:");Serial.print(pwm-Delta_Left_PWM);Serial.print(",");
        Serial.print("Right_PWM:");Serial.print(pwm-Delta_Right_PWM);Serial.print(",");
        Serial.print("angle:");Serial.print(*angle);Serial.print(",");
        Serial.print("distance:");Serial.println(*distance);
      #endif
}
void PWM_protect(int* Delta_Left_PWM, int* Delta_Right_PWM){
  
  if(*Delta_Left_PWM>abs(Motor.Foward_max_PWM-Motor.Foward_min_PWM)){
      *Delta_Left_PWM=abs(Motor.Foward_max_PWM-Motor.Foward_min_PWM);
    }

  if(*Delta_Right_PWM>abs(Motor.Foward_max_PWM-Motor.Foward_min_PWM)){
      *Delta_Right_PWM=abs(Motor.Foward_max_PWM-Motor.Foward_min_PWM);
    }

}
/*
==============================================
功能:Bandwidth(us)轉PWM,0~8196==>0~255
================================================
*/
int Bandwidth2PWM(int bandwidth){
    return map(bandwidth,Motor.speed_min_us,Motor.Cycle_time_us,0,255);
}
