/*
==============================================
功能:讀取reviecer，可用於同時測得High Voltage及Low Voltage時間
================================================
*/
void _CHANGE(){
  volatile int current_time = micros();
  pwm_value=current_time-prev_time;
  prev_time=current_time;
}
 /*
==============================================
功能:讀取reviecer，測得High Voltage時間，定選擇模式
Follow mode:1700~2000,遙控器Switch_C最上
Command mode:1500,遙控器Switch_C置中
Remote_mode:1300~1300,遙控器Switch_C最下
================================================
*/
void rising() {
  prev_time = micros();
  attachInterrupt(Mode_setting.Interrupt_Index, falling, FALLING);
}
 
void falling() {
  pwm_value = micros()-prev_time;
    
  attachInterrupt(Mode_setting.Interrupt_Index, rising, RISING);
  if(pwm_value>Mode_setting.Follow_mode_threshold_min &&  pwm_value<Mode_setting.Follow_mode_threshold_max)
  {
    digitalWrite(Mode_setting.Relay_PIN[0],LOW);
    digitalWrite(Mode_setting.Relay_PIN[1],LOW);
    Mode_setting.follow_mode=true;
    UWB.Stop_Recieve=false;

  }else if(pwm_value>Mode_setting.Remote_mode_threshold_min && pwm_value<Mode_setting.Remote_mode_threshold_max)
  {
    Mode_setting.follow_mode=false;
    UWB.Stop_Recieve=true;
    digitalWrite(Mode_setting.Relay_PIN[0],HIGH);
    digitalWrite(Mode_setting.Relay_PIN[1],HIGH);
  }else if(pwm_value==Mode_setting.command_mode_threshold_min){
    Mode_setting.follow_mode=false;
    UWB.Stop_Recieve=true;
    digitalWrite(Mode_setting.Relay_PIN[0],HIGH);
    digitalWrite(Mode_setting.Relay_PIN[1],HIGH);

  }
  
}
