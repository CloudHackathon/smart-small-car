
*/
#include <Servo.h>
//#include <MsTimer2.h>
#include <EEPROM.h> 

int ledpin = 13;//设置系统启动指示灯
int ENA = 5;//L298使能A
int ENB = 6;//L298使能B
int INPUT2 = 7;//电机接口1
int INPUT1 = 8;//电机接口2
int INPUT3 = 12;//电机接口3
int INPUT4 = 13;//电机接口4
int num;//定义电机标志位

int Echo = A5;  // 定义超音波信号接收脚位  
int Trig = A4;  // 定义超音波信号发射脚位
int Input_Detect_LEFT = A3;    //定义小车左侧红外
int Input_Detect_RIGHT = A2;  //定义小车右侧红外
int Input_Detect = A1;//定义小车前方红外
int Carled = A0;//定义小车车灯接口
int Cruising_Flag = 0;
int Pre_Cruising_Flag = 0 ;
int Left_Speed_Hold = 200;//定义左侧速度变量
int Right_Speed_Hold = 200;//定义右侧速度变量


#define MOTOR_GO_FORWARD  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体前进	                            
#define MOTOR_GO_BACK	  {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    //车体前进
#define MOTOR_GO_RIGHT	  {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体前进
#define MOTOR_GO_LEFT	  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    //车体前进
#define MOTOR_GO_STOP	  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,LOW);}    //车体前进


void forward(int num)
{
	switch(num)
	{
		case 1:MOTOR_GO_FORWARD;return;
		case 2:MOTOR_GO_FORWARD;return;
		case 3:MOTOR_GO_BACK;return;
		case 4:MOTOR_GO_BACK;return;
		case 5:MOTOR_GO_LEFT;return;
		case 6:MOTOR_GO_LEFT;return;
		case 7:MOTOR_GO_RIGHT;return;
		case 8:MOTOR_GO_RIGHT;return;
		default:return;		
	}
}

void back(int num)
{
		switch(num)
	{
		case 1:MOTOR_GO_BACK;return;
		case 2:MOTOR_GO_BACK;return;
		case 3:MOTOR_GO_FORWARD;return;
		case 4:MOTOR_GO_FORWARD;return;
		case 5:MOTOR_GO_RIGHT;return;
		case 6:MOTOR_GO_RIGHT;return;
		case 7:MOTOR_GO_LEFT;return;
		case 8:MOTOR_GO_LEFT;return;
		default:return;		
	}
}
void left(int num)
{
		switch(num)
	{
		case 1:MOTOR_GO_LEFT;return;
		case 2:MOTOR_GO_RIGHT;return;
		case 3:MOTOR_GO_LEFT;return;
		case 4:MOTOR_GO_RIGHT;return;
		case 5:MOTOR_GO_FORWARD;return;
		case 6:MOTOR_GO_BACK;return;
		case 7:MOTOR_GO_FORWARD;return;
		case 8:MOTOR_GO_BACK;return;
		default:return;	
	}
}
void right(int num)
{
		switch(num)
	{
		case 1:MOTOR_GO_RIGHT;return;
		case 2:MOTOR_GO_LEFT;return;
		case 3:MOTOR_GO_RIGHT;return;
		case 4:MOTOR_GO_LEFT;return;
		case 5:MOTOR_GO_BACK;return;
		case 6:MOTOR_GO_FORWARD;return;
		case 7:MOTOR_GO_BACK;return;
		case 8:MOTOR_GO_FORWARD;return;	
		default:return;
	}
}


int Left_Speed[11]={90,106,122,138,154,170,186,203,218,234,255};//左侧速度档位值
int Right_Speed[11]={90,106,122,138,154,170,186,203,218,234,255};//右侧速度档位值

//Servo servo1;// 创建舵机#1号
//Servo servo2;// 创建舵机#2号
Servo servo3;// 创建舵机#3号
Servo servo4;// 创建舵机#4号
Servo servo5;// 创建舵机#5号
Servo servo6;// 创建舵机#6号
Servo servo7;// 创建舵机#7号
Servo servo8;// 创建舵机#8号



//byte angle1=60;//舵机#1初始值
//byte angle2=60;//舵机#2初始值
byte angle3=60;//舵机#3初始值
byte angle4=60;//舵机#4初始值
byte angle5=60;//舵机#5初始值
byte angle6=120;//舵机#6初始值
byte angle7=60;//舵机#7初始值
byte angle8=60;//舵机#8初始值


int buffer[3];  //串口接收数据缓存
int rec_flag;   //串口接收标志位
int serial_data;
int Uartcount;
int IR_R;
int IR_L;
int IR;
unsigned long Pretime;
unsigned long Nowtime;
unsigned long Costtime;
float Ldistance;

void Open_Light()//开大灯
    {      
      digitalWrite(Carled,HIGH);   //拉低电平，正极接电源，负极接Io口
      delay(1000);             
    }
void Close_Light()//关大灯
    {  
      digitalWrite(Carled, LOW);   //拉低电平，正极接电源，负极接Io口
      delay(1000);             
    }
    
void  Avoiding()//红外避障函数
    {  
      IR = digitalRead(Input_Detect);
       if((IR == HIGH))
       {
          forward(num);;//直行 
          return;            
       }
       if((IR == LOW))
       {
            MOTOR_GO_STOP;//停止
            return;
       }
    }
           
   

void FollowLine()   // 巡线模式
    {  
      IR_L = digitalRead(Input_Detect_LEFT);//读取左边传感器数值
      IR_R = digitalRead(Input_Detect_RIGHT);//读取右边传感器数值
      
      if((IR_L == LOW) && (IR_R == LOW))//两边同时探测到障碍物
      {
        forward(num);//直行 
        return;          
        
      }
      if((IR_L == LOW) && (IR_R == HIGH))//右侧遇到障碍  
      {
        left(num);//左转 
        return;
        
      }
      if((IR_L == HIGH) &&( IR_R == LOW))//左侧遇到障碍 
      {
        right(num);//右转 
        return;
        
      }
      if((IR_L == HIGH) && (IR_R == HIGH))//左右都检测到，就如视频中的那样遇到一道横的胶带
      {
        MOTOR_GO_STOP;//停止
        return;
       }
    }     

char Get_Distence()//测出距离
 {  
      digitalWrite(Trig, LOW);   // 让超声波发射低电压2μs  
      delayMicroseconds(2);  
      digitalWrite(Trig, HIGH);  // 让超声波发射高电压10μs，这里至少是10μs  
      delayMicroseconds(10);  
      digitalWrite(Trig, LOW);    // 维持超声波发射低电压  
      Ldistance = pulseIn(Echo, HIGH);  // 读差相差时间  
      Ldistance= Ldistance/5.8/10;      // 将时间转为距离距离（单位：公分）    
    //  Serial.println(Ldistance);      //显示距离  
      return Ldistance;
      
  }    
  
void Avoid_wave()//超声波避障函数
{
  Get_Distence();
  if(Ldistance < 15)
      {
          MOTOR_GO_STOP;
      }
      else
      {
           forward(num);
      }
}

void Avoid_wave_auto()
{
  Get_Distence();
  if(Ldistance < 15)
  {
    back(num);;
    delay(300);
    MOTOR_GO_STOP;
  }
}

void Send_Distance()//超声波距离PC端显示
{
  int dis= Get_Distence();
  Serial.write(0xff);
  Serial.write(0x03);
  Serial.write(0x00);
  Serial.write(dis);
  Serial.write(0xff);
  delay(1000);
}
/*
*********************************************************************************************************
** 函数名称 ：Delayed()
** 函数功能 ：延时程序
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void  Delayed()    //延迟40秒等WIFI模块启动完毕
{
    int i;
    for(i=0;i<20;i++)
    {
        digitalWrite(ledpin,LOW);
        delay(1000);
        digitalWrite(ledpin,HIGH);
        delay(1000);
    }
}

/*
*********************************************************************************************************
** 函数名称 ：setup().Init_Steer()
** 函数功能 ：系统初始化（串口、电机、舵机、指示灯初始化）。
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Init_Steer()//舵机初始化(角度为上次保存数值)
{
   // angle1 = EEPROM.read(0x01);//读取寄存器0x01里面的值
   // angle2 = EEPROM.read(0x02);//读取寄存器0x02里面的值
    angle3 = EEPROM.read(0x03);//读取寄存器0x03里面的值
    angle4 = EEPROM.read(0x04);//读取寄存器0x04里面的值
    angle5 = EEPROM.read(0x05);//读取寄存器0x05里面的值
    angle6 = EEPROM.read(0x06);//读取寄存器0x06里面的值
    angle7 = EEPROM.read(0x07);//读取寄存器0x07里面的值
    angle8 = EEPROM.read(0x08);//读取寄存器0x08里面的值
    
    if(angle7 == 255 && angle8 == 255)
    {
       // EEPROM.write(0x01,60);//把初始角度存入地址0x01里面
       // EEPROM.write(0x02,60);//把初始角度存入地址0x02里面
        EEPROM.write(0x03,60);//把初始角度存入地址0x03里面
        EEPROM.write(0x04,60);//把初始角度存入地址0x04里面
        EEPROM.write(0x05,60);//把初始角度存入地址0x05里面
        EEPROM.write(0x06,120);//把初始角度存入地址0x06里面
        EEPROM.write(0x07,60);//把初始角度存入地址0x07里面
        EEPROM.write(0x08,60);//把初始角度存入地址0x08里面
        return;
    }
 
   // servo1.write(angle1);//把保存角度赋值给舵机1
   // servo2.write(angle2);//把保存角度赋值给舵机2
    servo3.write(angle3);//把保存角度赋值给舵机3
    servo4.write(angle4);//把保存角度赋值给舵机4
    servo5.write(angle5);//把保存角度赋值给舵机5
    servo6.write(angle6);//把保存角度赋值给舵机6
    servo7.write(angle7);//把保存角度赋值给舵机7
    servo8.write(angle8);//把保存角度赋值给舵机8
    num = EEPROM.read(0x10);//读取寄存器0x10里面的值
    if(num==0xff)EEPROM.write(0x10,1);
}

void setup()
{
    pinMode(ledpin,OUTPUT); 
    pinMode(ENA,OUTPUT); 
    pinMode(ENB,OUTPUT); 
    pinMode(INPUT1,OUTPUT); 
    pinMode(INPUT2,OUTPUT); 
    pinMode(INPUT3,OUTPUT); 
    pinMode(INPUT4,OUTPUT); 
    pinMode(Input_Detect_LEFT,INPUT);
    pinMode(Input_Detect_RIGHT,INPUT);
    pinMode(Carled, OUTPUT);
    pinMode(Input_Detect,INPUT);
    pinMode(Echo,INPUT);
    pinMode(Trig,OUTPUT);

    Delayed();//延迟40秒等WIFI模块启动完毕
    analogWrite(ENB,Left_Speed_Hold);//给L298使能端B赋值
    analogWrite(ENA,Right_Speed_Hold);//给L298使能端A赋值
    digitalWrite(ledpin,LOW);
    //servo1.attach(SDA);//定义舵机1控制口
    //servo2.attach(SCL);//定义舵机2控制口
    servo3.attach(3);//定义舵机3控制口
    servo4.attach(4);//定义舵机4控制口
    servo5.attach(2);//定义舵机5控制口
    servo6.attach(11);//定义舵机6控制口
    servo7.attach(9);//定义舵机7控制口
    servo8.attach(10);//定义舵机8控制口
    Serial.begin(9600);//串口波特率设置为9600 bps
    Init_Steer();
}
/*
*********************************************************************************************************
** 函数名称 ：loop()
** 函数功能 ：主函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Cruising_Mod()//模式功能切换函数
    {
       
	 if(Pre_Cruising_Flag != Cruising_Flag)
	 {
	     if(Pre_Cruising_Flag != 0)
		 {
		     MOTOR_GO_STOP; 
		 }

    	 Pre_Cruising_Flag =  Cruising_Flag;
	 }	
	switch(Cruising_Flag)
	  {
	   
	   case 2:FollowLine(); return;//巡线模式
	   case 3:Avoiding(); return;//避障模式
	   case 4:Avoid_wave();return;//超声波避障模式
           case 5:Send_Distance();//超声波距离PC端显示
	   default:return;
	  }
        	 
}

void loop()
  {  
    while(1)
    {
        Get_uartdata();
        UartTimeoutCheck();
        Cruising_Mod();
     }  
  }



/*
*********************************************************************************************************
** 函数名称 ：Communication_Decode()
** 函数功能 ：串口命令解码
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Communication_Decode()
{   
    if(buffer[0]==0x00)
    {
        switch(buffer[1])   //电机命令
        {
            case 0x01:MOTOR_GO_FORWARD; return;
	    case 0x02:MOTOR_GO_BACK;    return;
	    case 0x03:MOTOR_GO_LEFT;    return;
            case 0x04:MOTOR_GO_RIGHT;   return;
	    case 0x00:MOTOR_GO_STOP;    return;
           default: return;
        }	
    }
   else if(buffer[0]==0x01)//舵机命令
    {
        if(buffer[2]<1)return;
        switch(buffer[1])
        {
           // case 0x01:angle1 = buffer[2];servo1.write(angle1);return;
           // case 0x02:angle2 = buffer[2];servo2.write(angle2);return;
            case 0x01:if(buffer[2]>170)return;angle3 = buffer[2];servo3.write(angle3);return;
            case 0x02:if(buffer[2]>170)return;angle4 = buffer[2];servo4.write(angle4);return;
            case 0x03:if(buffer[2]>170)return;angle5 = buffer[2];servo5.write(angle5);return;
            case 0x04:if((buffer[2]<105)||(buffer[2]>178))return;angle6 = buffer[2];servo6.write(angle6);return;
            case 0x07:if(buffer[2]>170)return;angle7 = buffer[2];servo7.write(angle7);return;
            case 0x08:if(buffer[2]>170)return;angle8 = buffer[2];servo8.write(angle8);return;
            default:return;
        }
    }
    
   else if(buffer[0]==0x02)//调速
	{
                int i,j;
		if(buffer[2]>10)return;
             
		if(buffer[1]==0x01)//左侧调档
		{
                        i=buffer[2];
			Left_Speed_Hold=Left_Speed[i] ;
                        analogWrite(ENB,Left_Speed_Hold);
		}
                if(buffer[1]==0x02)//右侧调档
                {
                        j=buffer[2];
                        Right_Speed_Hold=Right_Speed[j] ;
                        analogWrite(ENA,Right_Speed_Hold);
                }else return;
        }
    else if(buffer[0]==0x33)//读取舵机角度并赋值
	{
		 Init_Steer();return;
        }
    else if(buffer[0]==0x32)//保存命令
    { 
       // EEPROM.write(0x01,angle1);
       // EEPROM.write(0x02,angle2);
        EEPROM.write(0x03,angle3);
        EEPROM.write(0x04,angle4);
        EEPROM.write(0x05,angle5);
        EEPROM.write(0x06,angle6);
        EEPROM.write(0x07,angle7);
        EEPROM.write(0x08,angle8);
        return;
    }
    	else if(buffer[0]==0x13)//模式切换开关
	{
	    switch(buffer[1])
		{
                  
                  case 0x02: Cruising_Flag = 2; return;//巡线
		  case 0x03: Cruising_Flag = 3; return;//避障
		  case 0x04: Cruising_Flag = 4; return;//雷达避障
                  case 0x05: Cruising_Flag = 5; return;//超声波距离PC端显示
                  case 0x00: Cruising_Flag = 0; return;//正常模式
		  default:Cruising_Flag = 0; return;//正常模式
		}
	}
        else if(buffer[0]==0x05)
    {
        switch(buffer[1])   //
        {
            case 0x00:Open_Light(); return;
	    case 0x02:Close_Light(); return;
            default: return;
        }	
    }
        else if(buffer[0]==0x40)//存储电机标志
	{
	   num=buffer[1];
	   EEPROM.write(0x10,num);
	}
}
/*
*********************************************************************************************************
** 函数名称 ：Get_uartdata()
** 函数功能 ：读取串口命令
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Get_uartdata(void)
{
    static int i;
   
    if (Serial.available() > 0) //判断串口缓冲器是否有数据装入
    {
        serial_data = Serial.read();//读取串口
        if(rec_flag==0)
        {
            if(serial_data==0xff)
            {
                rec_flag = 1;
                i = 0;
               Costtime = 0; 
            }
        }
        else
        {
            if(serial_data==0xff)
            {
                rec_flag = 0;
                if(i==3)
                {
                    Communication_Decode();
                }
                i = 0;
            }
            else
            {
                buffer[i]=serial_data;
                i++;
            }
        }
    }
}
/*
*********************************************************************************************************
** 函数名称 ：UartTimeoutCheck()
** 函数功能 ：串口超时检测
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void UartTimeoutCheck(void)
{
    if(rec_flag == 1)
    {
       Costtime++;  
      if(Costtime == 100000)
      {
           rec_flag = 0;
      }
    }
}
