# SWProCon
* C++でNintendo SwitchのPro Controllerの信号を読み取ってくれるすごいやつだよ。
* 先輩のライブラリ[RasPiDS3](https://github.com/Owl8/RasPiDS3)を参考にしています。
* GCCとかでコンパイルするときは　-std=c++11 -pthread のオプションを付けてください。
	* C++11に対応したコンパイラでつかおうね。
* ProControllerクラスのインスタンスを作って使ってください(test.cppとか参考にして)。
* ペアリング方法については各自で調べてね。

---
## これはリファレンスだ
```cpp
SWPC::ProController::ProController(const char* fileName = "/dev/input/js0", bool precision = false, int timeout = 0)
```
* コンストラクタです。
* fileNameには用いるJoyStickのファイルの場所を入れてください。
* precisionをtrueにすると精密モードになり、アナログ値が65536段階の数値で返ってくるようになります。
	* falseだと256段階です。
* コントローラが接続されるまでtimeout秒待ちます。
	* timeoutが0だとずっと待ちます。

```cpp
bool SWPC::ProController::connectedCheck()
```
* コントローラと接続出来たかどうかを返します。

```cpp
void SWPC::ProController::precisionMode(bool precision = true)
```
* 精密モードの切り替えを行います。

```cpp
void SWPC::ProController::yReverseSet(bool setVar = true)
```
* スティックのY軸の値の符号を反転させます。
    * デフォルトだと奥が負で手前が正なのでそれだとちょっと使いにくい人向け。
    * **値は非精密モードで-127~128になります。**

```cpp
void SWPC::ProController::update()
```
* ボタン, スティック, 加速度の情報が更新されます。
    * このメソッドを呼ばないと更新されません。
* プログラムのメインループに書くなどしてこまめに呼んでやってください。

```cpp
void SWPC::ProController::precisionMode(bool precision = true)
```
* 精密モードの切り替えを行います。

```cpp
bool SWPC::ProController::button(ButtonsNum button, bool onlyFlag = false)
```
* 0~17, または B, A, Y, X, L, R, ZL, ZR, MINUS, PLUS, LEFT_STICK, RIGHT_STICK, HOME, SS, UP, RIGHT, DOWN, LEFT でボタンを指定するとそのボタンが押されてるかどうかを返します(押されてるとtrue)。
* 第二引数にtrueを入れると第一引数で指定されたボタンだけが押されている時のみtrueを返します。

```cpp
int SWPC::ProController::stick(SticksNum stick)
```
* 0~5, またはLEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, CROSS_X, CROSS_Y でスティックの軸またはL2かR2を指定するとその軸の傾きやボタンがどのくらい押し込まれているかが返ってきます。
* スティックは最小値や最大値まで出ません。

```cpp
bool SWPC::ProController::press(ButtonsNum button)
```
* 引数はbutton()の第一引数と同じです. 指定されたボタンが前回のupdate()では押されておらず, 今回のupdate()で押されている状態に変化した場合のみ, trueを返します.
* つまりボタンが押されたフレームだけを判定するのに使えます.

```cpp
bool SWPC::ProController::release(ButtonsNum button)
```
* 使い方はpress()と同じで、こちらは押されている状態から離された場合にtrueを返します。
