<!-- リファレンスのテンプレート -->
<!-- readmeにライブラリのリファレンスを記載 -->

# gyro

R6093ジャイロを扱うクラス

# enum Cordinate{
	ROLL,
	PITCH,
	YAW
}

# struct gyro_data_t{
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}roll;
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}pitch;
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}yaw;
}

# class Gyro

## void Gyro::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num,int connect_dead_time = 1000)
シリアルピンの設定を行う

[パラメータ]  
送信ピン番号  
受信ピン番号  
シリアル番号  
通信切断時間

[戻り値]  
なし

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Gyro gyro;

int main(void)
{
    sken_system.init();
    gyro.init(C12,D2,SERIAL5);
    while(1)
    {

    }
}
```

## bool Gyro::update(void)
ジャイロからのデータを更新する

[パラメータ]  
なし

[戻り値]  
成功したらtrue，失敗したらfalse

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Gyro gyro;

int main(void)
{
    sken_system.init();
    gyro.init(C12,D2,SERIAL5);
    while(1)
    {
        gyro.update();
    }
}
```

## bool Gyro::resetAngle(Cordinate coordinate)
ジャイロの角度を0度にリセットする

[パラメータ]  
リセットする軸

[戻り値]  
成功したらtrue，失敗したらfalse

[サンプルコード]
1秒ごとにロール軸をリセットする

``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Gyro gyro;

int main(void)
{
    sken_system.init();
    gyro.init(C12,D2,SERIAL5);
    while(1)
    {
        gyro.update();
        if(sken_system.millis() % 1000 == 0){
            gyro.resetAngle(ROLL);
        }
    }
}
```

## bool Gyro::getConnectFlag(void)
接続確認フラグを取得する

[パラメータ]  
なし

[戻り値]  
接続していたらtrue，接続していなかったらfalse

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Gyro gyro;

int main(void)
{
    sken_system.init();
    gyro.init(C12,D2,SERIAL5);
    while(1)
    {
        gyro.update();
        if(gyro.getConnectFlag())[

        ]
        else{

        }
    }
}
```

## gyro_data_t Gyro::getData(void)
角度等のデータを取得する

[パラメータ]  
なし

[戻り値]  
ジャイロデータ

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Gyro gyro;

gyro_data_t gyro_data;

int main(void)
{
    sken_system.init();
    gyro.init(C12,D2,SERIAL5);
    while(1)
    {
        gyro.update();
        gyro_data = gyro.getData();
    }
}
```