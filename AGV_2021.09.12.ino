/*
==============================================
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

================================================
*/

//#define Show_command

#define showdata


/*
==============================================
功能:讀取遙控器Reciever的Chanel PWM5，切換模式參數
Follow mode:1700~2000,遙控器Switch_C最上
Command mode:1500,遙控器Switch_C置中
Remote_mode:1300~1300,遙控器Switch_C最下
================================================
*/
struct _Mode_setting{
  const int Interrupt_Index=0;
  bool follow_mode=false;
  const int Relay_PIN[2]={4,5};
  int Follow_mode_threshold_max=2000;
  int Follow_mode_threshold_min=1700;
  int command_mode_threshold_min=1500;
  int Remote_mode_threshold_max=1300;
  int Remote_mode_threshold_min=1000;
}Mode_setting;
/*
==============================================
功能:UWB參數
================================================
*/
struct _UWB{
  bool Stop_Recieve=true;
  bool start_follow=false;
}UWB;
HardwareSerial* UartPort;
/*
==============================================
功能:Motor參數
Original Freq:66HZ
V   H_Voltage(um)  L_Voltage(um) total(um)       PWM
Backward:  1912           12844       15000       32
STOP:     1500           13256       15000       25.2
Forward: 1088           13664       15000       18 

Timer2 change to Freq:122HZ
V   H_Voltage(us)  L_Voltage(us) total(us)       PWM
Backward:  1912           6196       8196         59
STOP:     1500           6696       8196         46 
Forward: 1088           7108       8196         33
   
================================================
*/
struct _Motor{

  int Foward_intial_speed_L_us=1200;
  int Foward_intial_speed_R_us=1200;
  
  const int Motor_Pin[2]={9,10};
  int Limit_Angle=40;
  int Start_Follow_Distance=100;//cm
  int Constant_Velocity_Distance=Start_Follow_Distance+10;//cm
  
  int Foward_max_speed_us=1200;
  int Foward_max_PWM=37;//由Foward_max_speed_us換算成PWM
  int Foward_min_speed_us=1480;
  int Foward_min_PWM=45;//由Foward_min_speed_us換算成PWM

  
  int Backward_max_speed_us=1800;
  int Backward_max_PWM=56;//由Backward_max_speed_us換算成PWM
  int Backward_min_speed_us=1550;
  int Backward_min_PWM=48;//由Backward_min_speed_us換算成PWM
  int Cycle_time_us=8196;
  int speed_min_us=0;
}Motor;
volatile int pwm_value = 0;//用interrupt計算遙控器接收器pwm
volatile int prev_time = 0;//用interrupt計算遙控器接收器pwm

//-----Command----------
boolean commandReceived = false;
String InData;
char RcvChar;
String data[10];
String UWB_InData;
char UWB_RcvChar;
String UWB_data;
int Delta_Left_PWM=0,Delta_Right_PWM=0;
int Command_counter;
//-------------------------

void setup() {
  digitalWrite(Mode_setting.Relay_PIN[0],HIGH);
  digitalWrite(Mode_setting.Relay_PIN[1],HIGH);
  attachInterrupt(0, rising, RISING);
  for(int i=0;i<sizeof(Motor.Motor_Pin);i++)
  {
    pinMode(Motor.Motor_Pin[i],OUTPUT);
  }
  for(int i=0;i<sizeof(Mode_setting.Relay_PIN);i++)
  {
    pinMode(Mode_setting.Relay_PIN[i],OUTPUT);
  }
  UartPort = &Serial2;
  (*UartPort).begin(115200);

  Serial.begin(115200); 


  //change Timer2  to Freq:122HZ
  int ffff = 6;
   TCCR2B = TCCR2B & 0xF8  | ffff;
}

void loop() {
  //讀取serial command
 serial_event();

 //讀取UWB data
 if(!UWB.Stop_Recieve){

    while((*UartPort).available()>0)
     {
              Receive_UWB();        
              if(UWB.Stop_Recieve){
                    break;
                }   
              serial_event();
      }
 }

}

void serial_event(){
     commandReceived=false;
     Command_counter = 0;
     while(Serial.available())
     {
            command();         
     }
     if(commandReceived == true)
      {    
         process();
      }   
}
