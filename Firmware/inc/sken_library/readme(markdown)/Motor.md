# Motor
モーターを制御するためのクラス

# class Motor
## void Motor::init(Pin pin_a,Pin pin_p,Pin pin_b,TimerNumber timer,TimerChannel ch);
芝ドラのピン、タイマー、チャンネルの設定  

[パラメータ]  
Aピン  
PWMピン  
Bピン  
タイマー番号  
タイマーチャンネル  

[戻り値]  
なし

[サンプルコード]  
C2をAピン、C3をBピン、C8をPWMピンに設定
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

Motor motor;

int main(void){
  sken_system.init();
  
  motor.init(C2,C8,C3,TIMER8,CH3);
  
  while(true){
    
  }
}
```

## void Motor::init(MtrPin mtr_pin,Pin pin,TimerNumber timer,TimerChannel ch);
黒ドラのピン、タイマー、チャンネルの設定  

[パラメータ]  
"Apin" or "Bpin"
ピン番号
タイマー番号
タイマーチャンネル

[戻り値]  
なし

[サンプルコード]  
A0をAピン、A1をBピンに設定
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

Motor motor;

int main(void){
  sken_system.init();
  
  motor.init(Apin,A0,TIMER5,CH1);
  motor.init(Bpin,A1,TIMER5,CH2);
  
  while(true){
    
  }
}
```

## void Motor::write(int val);
モーターを回す関数  

[パラメータ]  
PWM値（-100~100）  
負の数は逆回転を表す  

[戻り値]  
なし

[サンプルコード]  
モータをPWM値30で正回転
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

Motor motor;

int main(void){
  sken_system.init();
  
  motor.init(Apin,A0,TIMER5,CH1);
  motor.init(Bpin,A1,TIMER5,CH2);
  
  while(true){
    motor.write(30);
  }
}
```

## void Motor::stop();
モータを停止させる関数  

[パラメータ]  
なし

[戻り値]  
なし

[サンプルコード]  
モータを停止させる
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

Motor motor;

int main(void){
  sken_system.init();
  
  motor.init(Apin,A0,TIMER5,CH1);
  motor.init(Bpin,A1,TIMER5,CH2);
  
  while(true){
    motor.stop();
  }
}
```
