/*
==============================================
功能:UWB資料讀取
================================================
*/
byte xcm_position,ycm_position,O_position;
String  sub_xcm,sub_ycm,xcm_value,ycm_value;
void Receive_UWB()  
{
    UWB_RcvChar = (*UartPort).read();


    UWB_InData += UWB_RcvChar;
    if(UWB_RcvChar=='}'){
      UWB_data=UWB_InData;
      if(UWB_InData.length()>2){
       
        xcm_position=UWB_data.indexOf("Xcm");
        
        ycm_position=UWB_data.indexOf("Ycm");
        O_position=UWB_data.indexOf("O");
        sub_xcm=UWB_InData.substring(xcm_position,ycm_position);
        sub_ycm=UWB_InData.substring(ycm_position,O_position);

        xcm_value=sub_xcm.substring(sub_xcm.indexOf(":")+1,sub_xcm.indexOf(","));
        ycm_value=sub_ycm.substring(sub_ycm.indexOf(":")+1,sub_ycm.indexOf(","));

        //若terminal下Follow指令或是遙控器切至跟隨模式則進入跟隨
        if(Mode_setting.follow_mode){
            Follow_Mode(&xcm_value,&ycm_value);
        }
      }
      UWB_InData=""; 
    }
}
/*
==============================================
功能:接收terminal serial資料
================================================
*/
void command()
{
      RcvChar = Serial.read();
      commandReceived = true;   //確認serial port有無資料輸入

        if(RcvChar=='.'||RcvChar=='\n')        //EX:P.10.5.0.10
        {
          data[Command_counter]=InData;  //data[0]=M,data[1]=10,data[2]=5,data[3]=0,data[4]=10
          InData="";                     //InData 存入一組字串後clear
          Command_counter++;

        }else
        {
          InData += RcvChar;    //InData先後存入P=>10=>5=>0
          delay(10);
        }
}
/*
==============================================
功能:執行command模式，指令範例:"Start.","Pause."
Start.:開始接受UWB資料
Pause.:停止接收UWB資料
Forward.:往前
Backward.:往後
Right.:向右
Left.:向左
Follow.:開始跟隨
Stop.:停止移動
================================================
*/
void process()                                      
{
     #ifdef Show_command                            
      for(int i=0;i<Command_counter;i++)
      {
        Serial.println(data[i]);
      }
      #endif
      if(data[0]=="Start"){
        UWB.Stop_Recieve=false;
        Serial.println("UWB Start");
      }else if(data[0]=="Pause"){
        UWB.Stop_Recieve=true;
        Serial.println("UWB Pause");
      }else if(data[0]=="Forward"){
         Forward(0,0);
        Serial.println("Forward");
      }else if(data[0]=="Stop"){
        _Stop();
        Serial.println("Stop");
      }else if(data[0]=="Backward"){
        Backward(0,0);
        Serial.println("Backward");

      }else if(data[0]=="Right"){
        Right(0,1);
        Serial.println("Right");
      }else if(data[0]=="Left"){
        Left(1,0);
        Serial.println("Left");
      }else if(data[0]=="Follow"){
        UWB.Stop_Recieve=false;
        Mode_setting.follow_mode=true;
        Serial.println("Follow");

      }
}
