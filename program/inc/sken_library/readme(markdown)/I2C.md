# I2C
I2C通信を行う

# class I2C
## void I2C::init(Pin scl,Pin sda,I2CSelect i2c,int clockspeed)
I2C通信の初期化

[パラーメタ]  
sclピン  
sdaピン   
I2C番号  
クロックスピード[Hz]  

[サンプルコード]  
I2C通信初期
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

I2C i2c

int main(void){
  sken_system.init();
  i2c.init(B8,B9,I2C1,400000);
  
  while(true){
  
  }
}
```

## void I2C::write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout = 100)
送信関数  

[パラメータ]  
デバイスアドレス  
メモリーアドレス  
メモリーアドレスサイズ
送信データアドレス
送信データサイズ
タイムアウト時間[ms]  

[サンプルコード]  
8バイトのデータを送信
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

I2C i2c
uint8_t data[8] = {1,2,3,4,5,6,7,8};

int main(void){
  sken_system.init();
  i2c.init(B8,B9,I2C1,400000);
  
  while(true){
    i2c.write(0x01,0xA0,data,8);
  }
}
```

## void I2C::read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout = 100)
受信関数

[パラメータ]  
デバイスアドレス  
メモリーアドレス  
メモリーアドレスサイズ
受信データアドレス
受信データサイズ
タイムアウト時間[ms]  

[サンプルコード]  
8バイトのデータを受信
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

I2C i2c
uint8_t data[8];

int main(void){
  sken_system.init();
  i2c.init(B8,B9,I2C1,400000);
  
  while(true){
    i2c.read(0x01,0xA0,data,8);
  }
}
```
