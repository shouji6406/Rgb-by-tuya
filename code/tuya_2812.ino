#include <FastLED.h>
#include <TuyaWifi.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
 #include <avr/power.h> 
#endif

#define DATA_PIN    11        //灯带引脚
#define LED_TYPE    WS2812B   //灯带类型
#define COLOR_ORDER RGB       //灯带色序
#define NUM_LEDS    30        //灯珠数量
CRGB    leds[NUM_LEDS];

#define BUTTON_PIN  5         //场景切换按键

#define MIC_PIN     A4        //麦克风引脚
 
#define MIC_Btn     6         //麦克风律动模式切换按键

#define LED_PIN     9         //配网指示灯

boolean oldState = HIGH;

int     mode     = 0;         // 场景模式, 0-6

byte    dengmoshi=0;          //灯模式标志
int     BRIGHTNESS = 150;     //初始亮度值
int     freq_flash = 200;     //律动节奏(数值越大律动越快)

int     key_pin = 7;          

TuyaWifi my_device;

unsigned char led_state = 0;  //灯带开关灯状态

#define DPID_SWITCH_LED   20  //开关DPID
#define DPID_WORK_MODE    21  //模式DPID
#define DPID_BRIGHT_VALUE 22  //亮度值DPID


///* Current device DP values */
unsigned char dp_bool_value = 0;
unsigned char dp_enum_value = 0;

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 * dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
   声明所有DP点
*/
unsigned char dp_array[][2] = {
  {DPID_SWITCH_LED, DP_TYPE_BOOL},                 //开关
  {DPID_WORK_MODE, DP_TYPE_ENUM},                  //模式
  {DPID_BRIGHT_VALUE, DP_TYPE_VALUE},              //亮度值
};

unsigned char pid[] = {" "};                       //***********************************************************PID
unsigned char mcu_ver[] = {"1.0.0"};

unsigned long last_time = 0;

uint8_t gHue = 0; 

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
  
  pinMode(key_pin, INPUT_PULLUP);
  pinMode(MIC_Btn, INPUT_PULLUP);

  my_device.init(pid, mcu_ver);
  my_device.set_dp_cmd_total(dp_array, 17);
  my_device.dp_process_func_register(dp_process);
  my_device.dp_update_all_func_register(dp_update_all);

  last_time = millis();
  
}

void loop() {
  if (digitalRead(key_pin) == LOW) {
    delay(80);
    if (digitalRead(key_pin) == LOW) {
      my_device.mcu_set_wifi_mode(SMART_CONFIG);
    }
  }
  my_device.uart_service();

  /* LED blinks when network is being connected */
  if ((my_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (my_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (my_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW)) {
    if (millis()- last_time >= 500) {
      last_time = millis();

      if (led_state == LOW) {
        led_state = HIGH;
      } else {
        led_state = LOW;
      }
      digitalWrite(LED_PIN, led_state);
    }
  }else
  digitalWrite(LED_PIN, LOW);
  
  denmoshi();
 
  boolean newState = digitalRead(BUTTON_PIN); 

  if((newState == LOW) && (oldState == HIGH)) {
    delay(20);

    if(newState == LOW) {      
      if(++mode > 6) mode = 0; 
      switch(mode) {           
        case 0:
          dengmoshi=0;
          break;
        case 1:
          dengmoshi=7;
          break;
        case 2: 
          FastLED.clear();
          dengmoshi=2;
          break;
        case 3:
          dengmoshi=3;
          break;
        case 4:
          dengmoshi=4;
          break;
        case 5:
          FastLED.clear();
          dengmoshi=5;
          break;
        case 6:
          dengmoshi=8;
          break;
      }
    }
  }
  oldState = newState;
}


/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)    
{
  switch(dpid) {
    case DPID_SWITCH_LED:                                                             //***************************************开关灯
      dp_bool_value = my_device.mcu_get_dp_download_data(dpid, value, length); /* Get the value of the down DP command */
      if (dp_bool_value) {
        dengmoshi=1;         //执行灯效1＝白光
      } else {
        dengmoshi=0;         //关灯
      }
      my_device.mcu_dp_update(dpid, value, length);
    break;
    
    case DPID_WORK_MODE:                               //***********************************************************************************************************
    dp_enum_value = my_device.mcu_get_dp_download_data(dpid, value, length); /* Get the value of the down DP command */
      switch(dp_enum_value){      //DPID21
        case 0: // white mode
          dengmoshi=1;
        break;
        case 1: // colour mode
          FastLED.clear();
          dengmoshi=2;        
        break;
        case 3: // music mode
          FastLED.clear();
          dengmoshi=8;      
        break;
        case 4: // breath mode
          FastLED.clear();
          dengmoshi=4;   
        break;
        case 5: // circle mode
          FastLED.clear();
          dengmoshi=5;
        break;
        case 6: // pride mode
          FastLED.clear();
          dengmoshi=3;     
        break;
        case 7: // rainbow mode
          dengmoshi=7;  
        break;
        case 8: // moonlight mode　　音乐模式
          my_device.mcu_dp_update(DPID_SWITCH_LED, value, length);
          FastLED.clear();
          dengmoshi=10;  
        break;
      }
      //Status changes should be reported.
      my_device.mcu_dp_update(dpid, value, length);
    break;
    
    case DPID_BRIGHT_VALUE://亮度
      BRIGHTNESS = my_device.mcu_get_dp_download_data(DPID_BRIGHT_VALUE, value, length);
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();
      my_device.mcu_dp_update(dpid, BRIGHTNESS, length);
      break;
      
    default:break;
  }
  return SUCCESS;
}


void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_SWITCH_LED, led_state, 1);
  my_device.mcu_dp_update(DPID_WORK_MODE, dp_enum_value, 1);
  my_device.mcu_dp_update(DPID_BRIGHT_VALUE, BRIGHTNESS, 1);
}
