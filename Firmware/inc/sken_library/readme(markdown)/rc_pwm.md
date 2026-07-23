<!-- リファレンスのテンプレート -->
<!-- readmeにライブラリのリファレンスを記載 -->

# rc_pwm

周期20[ms]のラジコンサーボ信号を制御するクラス．

# class RcPwm

## void RcPwm::init(Pin pin,TimerNumber tim_num,TimerChannel tim_ch)
ピンとタイマーの設定を行う

[パラメータ]  
ピン番号    
タイマー番号  
タイマーチャンネル

[戻り値]  
なし

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

RcPwm rc_pwm;

int main(void)
{
    sken_system.init();
    rc_pwm.init(A5,TIMER2,CH1);
    while(1)
    {

    }
}
```

## void RcPwm::turn(double percentage)
出力の割合を設定する関数

[パラメータ]  
出力割合(1000[us]から2000[us]の間を0[%]から100[%]で設定)

[戻り値]  
なし

[サンプルコード]
20[%] (1200[us])で出力する

``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

RcPwm rc_pwm;

int main(void)
{
    sken_system.init();
    rc_pwm.init(A5,TIMER2,CH1);
    while(1)
    {
        rc_pwm.turn(20);
    }
}
```