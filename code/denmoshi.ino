void denmoshi()
{
  if(dengmoshi==0)                                 //关灯灯效
  {
    FastLED.setBrightness( BRIGHTNESS );
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show(); 
  }
  if(dengmoshi==1)                                 //开灯灯效
  {
    FastLED.setBrightness( BRIGHTNESS );
    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.show(); 
  }
  
  if(dengmoshi==2)                                 //随机点变
  {
    FastLED.setBrightness( BRIGHTNESS );
    fadetoblack();
    FastLED.show();
    delay(freq_flash/8); 
  }
  if(dengmoshi==3)                                 //渐变模式
  {
    FastLED.setBrightness( BRIGHTNESS );
    pride();
    FastLED.show();  
    delay(freq_flash/2); 
  }
  if(dengmoshi==4)                                 //幻彩模式
  {
    FastLED.setBrightness( BRIGHTNESS );
    juggle();
    FastLED.show();
    delay(freq_flash/8); 
  }
  if(dengmoshi==5)                                 //循环模式
  {
    FastLED.setBrightness( BRIGHTNESS );
    cylon();
    FastLED.show();
  }
  if(dengmoshi==6)                                 //火焰模式
  {
    FastLED.setBrightness( BRIGHTNESS );
    Fire2012();
    FastLED.show();
    delay(freq_flash/2); 
  }
  if(dengmoshi==7)                                 //彩虹渐变模式
  {
    EVERY_N_MILLISECONDS( 10 ) { gHue++; } 
    FastLED.setBrightness( BRIGHTNESS );
    rainbow() ;
    FastLED.show();
    delay(freq_flash/40); 
  }
  if(dengmoshi==8)                                 //阻尼效果
  {
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } 
    FastLED.setBrightness( BRIGHTNESS );
    bpm();
    FastLED.show();
  }
  if(dengmoshi==9)                                 //音乐律动模式1
  {
    FastLED.setBrightness( BRIGHTNESS );
    music_mode_1();
    FastLED.show();
  }
  if(dengmoshi==10)                                 //音乐律动模式2
  {
    FastLED.setBrightness( BRIGHTNESS );
    music_mode_2();
    FastLED.show();
  }
} 

  
