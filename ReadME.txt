Date:2021.08.27

1.Pin Assignment
  Relay_PIN:D4,D5
  Motor_Pin(Timer2):D9,D10
  UWB_Pin:RX2,TX2
  Mode_Switch(Interrupt0):D2

2.功能介紹
  Follow mode:1700~2000,遙控器Switch_C最上
  Command mode:1500,遙控器Switch_C置中
  Remote_mode:1300~1300,遙控器Switch_C最下
  
3.頁面介紹
  command:負責Terminal Serial,UWB資料
  Interrupt:負責讀取遙控器接收器PWM
  Motion:負責馬達控制
  PID:負責PID設定

4.可調整參數
  Motor.Limit_Angle:UWB角度設定
  Motor.Start_Follow_Distance:開始跟隨距離
  Motor.Foward_intial_speed_L_us,Motor.Foward_intial_speed_R_us:左右輪初始速度
  PID:kp,ki,kd