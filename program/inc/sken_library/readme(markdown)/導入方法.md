# 注意事項
開発にはSystem Workbench for STM32を使用してください  

# sken_library
以下の環境を想定して作られています.
* マイコン:STM32F446RE
* 開発環境:SW4STM32(CANはLegacyを使用)

# SW4STM32のインストール方法
インストーラを起動し，指示に従って進めていけばインストールできます．  
最初にプロジェクトを作成するときにHALライブラリのダウンロードが入るのでプロキシのない環境でネットにつないでください．  
インストーラのダウンロードにアカウント登録が必要でダウンロードしずらいので共有リンクを貼っておきます  
https://drive.google.com/drive/folders/1HYSE1UfOjAgIycmSkYiIlxAeubYQNDB2?usp=sharing  

# 導入方法
SW4STM32での方法を示す
## 文字コードの設定
1. SW4のWindow->Preferences->General->Workspaceを選択する
2. 左下のText file encodingをOther:UTF-8に設定する

### プロジェクトの作成
1. File->New->C++ Project
2. Project nameを決める
3. Ac6 STM32 MCU Projectを選択してNextを押す
4. 何も押さずにNext
5. Series:STM32F4，Board:NUCLEO-F446REを選択してNext(Finishを押さないように注意)
6. Hardware Abstraction Layerを選択してFinishを押す(動作確認を行ったHALのバージョンは1.24.0)
7. srcフォルダ内のmain.cをmain.cppに変更する
8. incフォルダを右クリックしてProperties->C/C++ Buildの中のExclude resource from buildのチェックボックスを外す
9. はじめにBuild(トンカチマーク)を行う
10. 一度ビルドすると書き込みはRun(再生ボタンマーク)で行える

### ライブラリの使用方法
1. IDE上のWindow->Show View->Outlineを押す
2. IDE右のOutlineからstm32f4xx.h->stm32f4xx_hal.h->stm32f4xx_hal_conf.hに行く
3. #define HAL_CAN_MODULE_ENABLEDをコメントアウトし，#define HAL_CAN_LEGACY_MODULE_ENABLEDのコメントアウトを外す
4. プロジェクトのincフォルダにsken_libraryを入れる(gitのサブモジュールを利用すると良いらしい)
5. main.cppに#include sken_library/include.hを追加する
6. main関数の一番最初にsken_system.init();を追加する

### その他
STM32でのデバッグにはSTM Studioがおすすめです．
インストーラのダウンロードリンクはこちらになります．
https://drive.google.com/drive/folders/11QA8bdeQBj_PZ3CjXhFK4LQze_Z-JIG4?usp=sharing



