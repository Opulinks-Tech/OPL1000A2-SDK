/*
   Generate pulse on specific GPIO pin 
   Pulse type and duration is determined by COM port command 
   support 4 ports, from #12 to #9 
   Version 0.2 
   Author: Dong Liu, 2017.06.19
   
   Revision History: 
   Version 0.3, Dong Liu, 07/25/2017
   (1) use two serials ports, UART0 and UART1 
   (2) every UART port can control four Reset pins, depend on caller  
   (3) every UART port can control two buzzers 
   
   Version 0.4, Dong Liu, 12/14/2017
   (1) update time stamp calculation function 
   (2) add Btool1 and Btool2 reset signal to support new arduino board  
   
   Version 0.5, Dong Liu, 09/04/2018
    This version is developed for CBS multiple download function. 
    It supports these features:
    (1) remove liquid LCD feature. 
    (2) support 10 relay control that use IO 12~3, corresponding to index P1~P10, N1~N10 
    (3) pin 45 is used to detect button press state 
    (4) pin 44 is used to LED light display which indicate action items.     
    (5) keep one buzzer which use pin13  
    
   CBS mega2560 arduino firmware, based on original SL1000 v0.5, Dong Liu, 09/11/2018
   (1) add a macro definition which change PWM mode setting. one is used for Relay control, the 
       other is used to generate reset signal directly. 
   (2) modify buzzer opt function, make it output with high after play sound    
   
   v0.4, Dong Liu, 09/19/2018
   (1) UART1/UART2 baud rate is set to 115200 
   
   v0.5, Dong Liu, 10/27/2018
   (1) serial1 and serial2 UART port output trigger signal method is changed.
   (2) change cbs string to mp (mass production)    
 */

String SW_VERSION = "v0.5";

#define  POSITIVE_PULSE   0 
#define  NEGTIVE_PULSE    1 

#define CH1_RESET     0 
#define CH2_RESET     1 
#define CH3_RESET     2 
#define CH4_RESET     3 
#define CH5_RESET     4 
#define CH6_RESET     5 
#define CH7_RESET     6 
#define CH8_RESET     7 
#define CH9_RESET     8 
#define CH10_RESET    9 

#define MIN_SIGNAL_NUM CH1_RESET
#define MAX_SIGNAL_NUM CH10_RESET
#define MAX_RESET_CHANNEL  10 

#define RET_FAIL      1 
#define RET_SUCCESS   0 

#define SERIAL_PORT0  0 
#define SERIAL_PORT1  1 
#define SERIAL_PORT2  2 

#define SERIAL_BAUD_RATE  9600 
#define WORK_BAUD_RATE  115200 

#define BUZZER_1         0
#define BUZZER_2         1

#define BUZZER_FREQ1_HZ    440 
#define BUZZER_FREQ2_HZ    300  
#define BUZZER_DELAY_TIME  200
#define BUZZER_BLANK_TIME  200 

#define MIN_FREQ_HZ     200 
#define MAX_FREQ_HZ     1000 

#define MIN_DELAY_MS    50 
#define MAX_DELAY_MS    2000

#define MAX_GPIO_PIN_NUM  '10' // support 10 reset channels 
#define MAX_BUZZER_NUM    '2' 
#define MAX_LED_NUM       '1' 

/* ----------- sub-soutines ----------------*/
const int CH1_Pin = 12;
const int CH2_Pin = 11;
const int CH3_Pin = 10;
const int CH4_Pin = 9;
const int CH5_Pin = 8;
const int CH6_Pin = 7;
const int CH7_Pin = 6;
const int CH8_Pin = 5;
const int CH9_Pin = 4;
const int CH10_Pin = 3;

const int LED_Pin = 44;
const int TRIGGER_Pin = 45;

const int Buzzer1_Pin = 13;  // Correspond to BUZZER_1

int PortType = -1; // 0 is ASP , 1 is MSQ, 2 is RF.
int PulseType = -1; // 0 is positive , 1 is negtive, other unknown 
int PulseTimeInMs = 0; // define the pulse width in ms unit 

int Buzzer_Index = -1; 
int Buzzer_Freq = BUZZER_FREQ1_HZ;

int Blink_Mode = -1;
int Led_Index = -1; 

int  flag_buzzer_set = 0;
int  flag_GPIO_set = 0;
int  flag_led_display = 0;

int buttonState = 0;  // detect trigger button state 

#define USE_ROGER_SUGGESTION  1 

void debug_output_str(int serial_idx, char *log_str)
{
  if(serial_idx == SERIAL_PORT0) 
      Serial.println(log_str);
  else if(serial_idx == SERIAL_PORT1) 
      Serial1.println(log_str); 
  else if(serial_idx == SERIAL_PORT2) 
      Serial2.println(log_str);       
}

void debug_output_char(int serial_idx, char one_char)
{
  if(serial_idx == SERIAL_PORT0) 
      Serial.println(one_char,BYTE);
  else if(serial_idx == SERIAL_PORT1) 
      Serial1.println(one_char,BYTE); 
  else if(serial_idx == SERIAL_PORT2) 
      Serial2.println(one_char,BYTE);      
}

void play_sound(int buzzer_num, int freq_hz, int delay_ms)
{
  // now we only support on buzzer 
  int buzzer_pin; 
  if(buzzer_num == BUZZER_1)
    buzzer_pin = Buzzer1_Pin;
   
  if(freq_hz < MIN_FREQ_HZ || freq_hz > MAX_FREQ_HZ)
    freq_hz = BUZZER_FREQ1_HZ;

  if(delay_ms < MIN_DELAY_MS || delay_ms > MAX_DELAY_MS) 
    delay_ms = BUZZER_DELAY_TIME;
    
  tone(buzzer_pin, freq_hz, delay_ms);
  delay(BUZZER_BLANK_TIME);

  // turn off tone 
  noTone(buzzer_pin);
  
  pinMode(buzzer_pin, OUTPUT); 
  digitalWrite(buzzer_pin, HIGH);
}

/* 
   Function: blink certain LED 
   Blink pattern support: 
     short blink mode: Total 2 seconds. keep light 0.5s, blink 1s with 0.2s internal, then keep light 0.5s  
     continious blink: keep light 200ms, then keep off 200ms  
   input parameter: led_index -> 1 or 2. Currently only support 1 
                    work_mode -> 0~3. 0/1: off,on; 2 is short blink, 3 continious blink mode.  
*/
#define KEEP_LED_OFF       0
#define KEEP_LED_ON        1 
#define SHORT_BLINK        2 
#define CONTINIOUS_BLINK   3 

void led_blink(int led_index, int work_mode)
{
  // now we only support on buzzer 
  int pinNumber, i, widthinMs ;
  int blink_pattern[3] = {300,100,300};
  int long_pattern = 200;
  
  if (led_index == 1)
      pinNumber = LED_Pin;
  
  switch (work_mode)
  {
    case KEEP_LED_OFF: digitalWrite(pinNumber, HIGH); break;
    case KEEP_LED_ON: digitalWrite(pinNumber, LOW); break;    
    case SHORT_BLINK: {
          widthinMs = blink_pattern[0];  
          for (i=0;i<1;i++)
          {
            digitalWrite(pinNumber, LOW);   
            delay(widthinMs); 	
            digitalWrite(pinNumber, HIGH); 
            delay(widthinMs);    
          }
          
          widthinMs = blink_pattern[1];  
          for (i=0;i<5;i++)
          {
            digitalWrite(pinNumber, LOW);   
            delay(widthinMs); 	
            digitalWrite(pinNumber, HIGH); 
            delay(widthinMs);    
          }
          
          widthinMs = blink_pattern[2]; 
          for (i=0;i<1;i++)
          {
            digitalWrite(pinNumber, LOW);   
            delay(widthinMs); 	
            digitalWrite(pinNumber, HIGH);
            delay(widthinMs);     
          }          
          break;
    }
    case CONTINIOUS_BLINK: {
            digitalWrite(pinNumber, LOW);   
            delay(long_pattern); 	
            digitalWrite(pinNumber, HIGH); 
            delay(long_pattern);       
       break;
    }
    default: break;
  }
}

void  set_pulse(int  portType,  int  pulseType, int  widthinMs)
{
  int pinNumber = 0, i = 0;
  int portIdxArray[MAX_RESET_CHANNEL] = {CH1_RESET,CH2_RESET,CH3_RESET,CH4_RESET,CH5_RESET,
                                        CH6_RESET,CH7_RESET,CH8_RESET,CH9_RESET,CH10_RESET};
  int pinNumArray[MAX_RESET_CHANNEL] = {CH1_Pin,CH2_Pin,CH3_Pin,CH4_Pin,CH5_Pin,
                                        CH6_Pin,CH7_Pin,CH8_Pin,CH9_Pin,CH10_Pin};                                       
  for (i=0;i<MAX_RESET_CHANNEL;i++)
  {
     if (portIdxArray[i] == portType)
     {
       pinNumber = pinNumArray[i];
       break;
     }          
  }

  if (widthinMs < 0)
    widthinMs = 500; // 500ms is defined value ! 
	
  if(portType >= MIN_SIGNAL_NUM && portType <= MAX_SIGNAL_NUM)  // it is an avliable port 
  {
     if(pulseType == POSITIVE_PULSE)
	 {
       // modified according to Roger's suggestion
#ifdef USE_ROGER_SUGGESTION       
       pinMode(pinNumber, OUTPUT); 
       digitalWrite(pinNumber, HIGH);   
       delay(widthinMs); 
       digitalWrite(pinNumber, LOW); 
       pinMode(pinNumber, INPUT); 
#else        
       digitalWrite(pinNumber, HIGH);   
       delay(widthinMs); 	
       digitalWrite(pinNumber, LOW);	
#endif        
	 }
      else if (pulseType == NEGTIVE_PULSE)
     {  
       // modified according to Roger's suggestion
#ifdef USE_ROGER_SUGGESTION          
       pinMode(pinNumber, OUTPUT); 
       digitalWrite(pinNumber, LOW);   
       delay(widthinMs); 
       digitalWrite(pinNumber, HIGH);
       pinMode(pinNumber, INPUT);  
#else     	      
       digitalWrite(pinNumber, LOW);   
       delay(widthinMs); 	
       digitalWrite(pinNumber, HIGH);
#endif        	 
     }
  }
}

int check_serial_avliable(int serial_idx)
{
  if(serial_idx == SERIAL_PORT0) 
      return Serial.available();
  else if(serial_idx == SERIAL_PORT1) 
      return Serial1.available();
  else if(serial_idx == SERIAL_PORT2) 
      return Serial2.available();      
}

int read_serial(int serial_idx)
{
  if(serial_idx == SERIAL_PORT0) 
      return Serial.read();
  else if(serial_idx == SERIAL_PORT1) 
      return Serial1.read();
  else if(serial_idx == SERIAL_PORT2) 
      return Serial2.read();      
}

/* function: receive command from serial port, and implement actions. 
   It is either RESRT pin operation, LED display or buzzer play sound operation.
   For RESET operation, command type is: Px/Nx 300#, x = 0~9 indicate port type, pulse type and pulse duration  
   For buzzer operation, command type is: Bx 300#, x = 0~1 indicate buzzer number, tone frequency Hz   
   For LED operation, its command is Lx y# x is LED index, range is 0~N, now we only support one LED, so it is fixed to 0. 
                    y is working mode, definition is below.
                    #define KEEP_LED_OFF       0
                    #define KEEP_LED_ON        1 
                    #define SHORT_BLINK        2 
                    #define CONTINIOUS_BLINK   3   
   Output is global parameters: 
     -- for GPIO reset, they are: PortType, PulseType and ulseTimeInMs
     -- for buzzer, they are: Buzzer_Index, Buzzer_Freq 
     -- for LED, they are: Led_Index and Blink_Mode
   Which kind of operatin is determined by 3 flags: flag_buzzer_set, flag_GPIO_set and flag_led_display   
   Return 0 means success, other means error 
*/ 
int recv_command(int serial_idx)
{
  int ret_status = RET_FAIL, inByte = 0; 
  char rev_string[16] = {0};
  int k, cmd_bytes = 0, exit_flag = 0; 
  int recv_flag = 0, duration_time = 0, tone_freq = 0, led_mode = 0; 
  int flag_buzzer = -1, flag_led = -1; 
  
  // clear rev_string
  for(k=0;k<16;k++)
    rev_string[k] = 0;
    
  k = 0;  	
  while(exit_flag != 1)
  {
     if (check_serial_avliable(serial_idx) > 0)
	 {
	   inByte = read_serial(serial_idx);
           
	   if((inByte == 'P') || (inByte == 'p') || (inByte == 'N') || (inByte == 'n') ||
          (inByte == 'B') || (inByte == 'b') || (inByte == 'L') || (inByte == 'l'))
          {
	     recv_flag = 1;
          }
	   else if(inByte == '#')
	   {
	      recv_flag = 0; 
	      exit_flag = 1;		  
	   }
	   if(recv_flag == 1) // save following characters 
           {
	     rev_string[k] = inByte & 0xFF; 
             
             //debug_output_char(serial_idx,rev_string[k]); 
             debug_output_char(SERIAL_PORT0,rev_string[k]);                         
             k = k + 1;
           }
	 }
      else
         exit_flag = 1;
  }
  cmd_bytes = k;

  if(cmd_bytes > 2)
  {    
      // for debug aim! 
      //debug_output_str(serial_idx,"Received: ");
      debug_output_str(SERIAL_PORT0,"Received: "); 
      for(k=0;k<cmd_bytes;k++)
        //debug_output_char(serial_idx,rev_string[k]);
        debug_output_char(SERIAL_PORT0,rev_string[k]);
      //debug_output_str(serial_idx," ");
      debug_output_str(SERIAL_PORT0," "); 

      // get buzzer number 
      if(rev_string[0] == 'B' || rev_string[0] == 'b' )
      {
	PulseType = -1; 
        flag_led = -1;
        flag_buzzer = 1;
      }
      else 
        flag_buzzer = -1;
    
      if(rev_string[0] == 'L' || rev_string[0] == 'l' )
      {
	PulseType = -1; 
        flag_buzzer = -1;
        flag_led = 1;
      }
      else 
        flag_led = -1;
        
      if(flag_buzzer == 1) 
      {
          if(rev_string[1] < MAX_BUZZER_NUM && rev_string[1] >= '0')
          {
            Buzzer_Index = rev_string[1] - '0';   
          }
              
          // 3rd char shall be blank 
          // get pulse width parameter 
          for(k=3;k<cmd_bytes;k++)
          {  
             if(rev_string[k]>='0' && rev_string[k]<='9') 
               inByte = rev_string[k] - '0';
               
             tone_freq = tone_freq*10 + inByte;
          }
          Buzzer_Freq = tone_freq;
          
          if(Buzzer_Index == BUZZER_1 || Buzzer_Index == BUZZER_2) 
          {
            if((tone_freq >= MIN_FREQ_HZ && tone_freq <= MAX_FREQ_HZ))
            {
              flag_buzzer_set = 1;
              ret_status = RET_SUCCESS; // success 
            }
          }  
      }

      if(flag_led == 1) 
      {
          if(rev_string[1] < MAX_LED_NUM  && rev_string[1] >= '0')
          {
            Led_Index= rev_string[1] - '0';   
          }
              
          // 3rd char shall be blank 
          // get blink work mode 
          for(k=3;k<cmd_bytes;k++)
          {  
             if(rev_string[k]>='0' && rev_string[k]<='9') 
               inByte = rev_string[k] - '0';
               
             led_mode =led_mode*10 + inByte;
          }
          Blink_Mode = led_mode;

          flag_led_display = 1;
          ret_status = RET_SUCCESS; // success  
      }
      
      // get pulse type 
      if(rev_string[0] == 'P' || rev_string[0] == 'p' )
	PulseType = POSITIVE_PULSE; 
      else if(rev_string[0] == 'N' || rev_string[0] == 'n')
	PulseType = NEGTIVE_PULSE;
      else
	PulseType = -1;     
	  
      if(PulseType != -1)
      {
          // get port number , note here the total poar number is limited.  
          if(rev_string[1] < MAX_GPIO_PIN_NUM && rev_string[1] >= '0')
          {
            PortType = rev_string[1] - '0';   
          }
              
          // 3rd char shall be blank 
          // get pulse width parameter 
          for(k=3;k<cmd_bytes;k++)
          {  
             if(rev_string[k]>='0' && rev_string[k]<='9') 
               inByte = rev_string[k] - '0';
               
             duration_time = duration_time*10 + inByte;
          }
          PulseTimeInMs = duration_time;
          
          if(PulseType == POSITIVE_PULSE || PulseType == NEGTIVE_PULSE) 
          {
            if((PortType >= MIN_SIGNAL_NUM && PortType <= MAX_SIGNAL_NUM) && (PulseTimeInMs > 0))
            {
              flag_GPIO_set = 1;
              ret_status = RET_SUCCESS; // success 
            }
          }
      }
  }
  return ret_status;
}

/* function: Calcuate time stamp string 
*/ 
String get_time_stamp(long time_in_second)
{
  String time_stamp_string;
  String hour_str = "0";
  String minute_str = "0";
  String second_str = "0";
  int hour_int,min_int,second_int,tmp;
  
  hour_int = (int)(time_in_second/3600);
  //min_int = (time_in_second - (hour_int*3600))/60; 
  tmp = (time_in_second - (hour_int*3600))%3600;
  min_int = (int)(tmp/60); 
  second_int = time_in_second % 60;
  hour_int = hour_int %100;
  hour_str = String("")+hour_int;
  if(hour_str.length() <= 1) 
  {
    hour_str = "0"+hour_str;
  }
  minute_str = String("")+min_int;
  if(minute_str.length() <= 1) 
  {
    minute_str = "0"+minute_str;
  }
  second_str = String("")+second_int;
  if(second_str.length() <= 1) 
  {
    second_str = "0"+second_str;
  }
  time_stamp_string = hour_str+":"+minute_str+":"+second_str;
  return time_stamp_string;  
} 

/* ----------------------------------------- */

void setup() { 
  String display_str;         
  int pinNumArray[MAX_RESET_CHANNEL] = {CH1_Pin,CH2_Pin,CH3_Pin,CH4_Pin,CH5_Pin,
                                        CH6_Pin,CH7_Pin,CH8_Pin,CH9_Pin,CH10_Pin};  
  int i,pin_num; 
  String outMsg;
  
  Serial.begin(SERIAL_BAUD_RATE);
  Serial1.begin(WORK_BAUD_RATE);    
  Serial2.begin(WORK_BAUD_RATE); 
  
  outMsg = "Opulinks mass production program, "+SW_VERSION;
  Serial.println(outMsg);
#ifdef USE_ROGER_SUGGESTION 
   outMsg = "Use PWM output to driver reset signal directly.";
   Serial.println(outMsg);
#else
   outMsg = "Use relay to generate reset signal.";
   Serial.println(outMsg);
#endif 
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards.
  for (i=0;i<MAX_RESET_CHANNEL;i++)
  {
    pin_num = pinNumArray[i];    
#ifdef USE_ROGER_SUGGESTION  
    pinMode(pin_num, INPUT); // According to Roger's suggestion on Sep.11
#else     
    pinMode(pin_num, OUTPUT); 
    digitalWrite(pin_num, HIGH); // force reset pin to high level 
#endif     
  }  
  // set LED pin characters 
  pinMode(LED_Pin, OUTPUT); 
  digitalWrite(LED_Pin, HIGH);
  
  // currently there is no buzzer 
  // set buzzer pin characters
  pinMode(Buzzer1_Pin, OUTPUT); 
  digitalWrite(Buzzer1_Pin, HIGH);
 
  // set input trigger button pin characters
  pinMode(TRIGGER_Pin, INPUT); 
}

void loop() {
  int UART1_ret_status = RET_FAIL;  
  int UART2_ret_status = RET_FAIL;   
  String outMsg;
  long curr_time_seconds;
  String lcd_display;
  String Timer_string;
  
  // init flag avaliables 
  flag_buzzer_set = 0;
  flag_GPIO_set = 0;
  
  UART1_ret_status = recv_command(SERIAL_PORT1);
  UART2_ret_status = recv_command(SERIAL_PORT2);
  
  if ( Blink_Mode == CONTINIOUS_BLINK)
  {
      // LED display
      led_blink(Led_Index, Blink_Mode); 
      Serial.println("LED: continious blink");     
  }
    
  if((UART1_ret_status == RET_SUCCESS) || (UART2_ret_status == RET_SUCCESS))
  {
    if(flag_buzzer_set == 1)
    {
        // play tone on specific buzzer  
        play_sound(Buzzer_Index, Buzzer_Freq, BUZZER_DELAY_TIME);
        
        // these are debug output
        outMsg = "Buzzer:";
        outMsg.concat(Buzzer_Index);
        outMsg = outMsg + ", Hz:";
        outMsg.concat(Buzzer_Freq);
        outMsg = outMsg + ", ms:";
        outMsg.concat(BUZZER_DELAY_TIME);	
        Serial.println(outMsg);

        // send complete message from port 
        outMsg = "success"; 
        Serial.println(outMsg);    
    }
    
    if((flag_led_display == 1) && (Blink_Mode != CONTINIOUS_BLINK))
    {
        // LED display
        led_blink(Led_Index, Blink_Mode);
        
        // these are debug output
        outMsg = "LED:";
        outMsg.concat(Led_Index);
        outMsg = outMsg + ", mode:";
        outMsg.concat(Blink_Mode);	
        Serial.println(outMsg);

        // send complete message from port 
        outMsg = "success"; 
        Serial.println(outMsg);    
    }
    
    if(flag_GPIO_set == 1)
    {
        // process pulse output operation 
        set_pulse(PortType, PulseType, PulseTimeInMs);
        
        // these are debug output
        outMsg = "Pulse:";
        outMsg.concat(PulseType);
        outMsg = outMsg + ", Port:";
        outMsg.concat(PortType);
        outMsg = outMsg + ", Width:";
        outMsg.concat(PulseTimeInMs);	
        Serial.println(outMsg);

        // send complete message from port 
        outMsg = "success"; 
        Serial.println(outMsg);	
    }     	
  }
  // detect Trigger button status 
  buttonState = digitalRead(TRIGGER_Pin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) { 
    // means button is pressed , send out 'T' character
    Serial1.println("Trigger,PWM45 = Low");
    Serial2.println("Trigger,PWM45 = Low");    
    Serial.println("Trigger,PWM45 = Low");
  }  
}
