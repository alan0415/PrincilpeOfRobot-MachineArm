#include<Servo.h>

Servo power; // 動力馬達
Servo base;  // 基座馬達
Servo jointLower; // 低位關節馬達
Servo jointHigher; // 高位關節馬達

/* 
 *  變數宣告 
 */
// 初始各個馬達角度
int init_base_angle = 25;
int init_jointLower_angle = 25;
int init_jointHigher_angle = 25;

// 目標馬達角度
int target_base_angle;
int target_jointLower_angle;
int target_jointHigher_angle;

// 清除區域的座標取得
int x1, x2, y1, y2;

// 實際板擦範圍
// int higher_x;
// int higher_y;
// int lower_x;
// int lower_y;

/* 
 * 擦黑板時的參數調用 
 */
 
// 動力馬達從原點移動到x1的移動時間
unsigned long power_start;
unsigned long power_stop;

// 從x1移動到x2的時間
unsigned long erase_start;
unsigned long erase_stop;



void setup() {
  // 機器手臂控制腳位
  power.attach();
  base.attach();
  jointLower.attach();
  jointHigher.attach();
  
}


// 更新原點與目標點的水平距離(x軸)，重疊時跳出迴圈
void x_distance(float target_x){
  
}

void moving(unsigned long period){
   unsigned long start_time = millis();
   while ( ( millis() - start_time ) < period){
      int i = 1; //空迴圈
    }
}

/* 
 *  動力馬達移動至指定範圍
 */
// minum x 後在上升機器手臂(Y軸平移)至 y軸 Maximum
void powerMotor_initialize(){
  // x軸移動
  power_start = millis();
  power.write(135);
  x_distance();
  power_stop = millis();
  power.write(90);

  // y軸移動
}


void Xaxis_erase(){
  
  erase_start = millis();
  power.write(135);
  x_distance(x2); //傳入x2的位置，當原點到達x2位置時跳出迴圈
  power.write(90);
  erase_stop = millis();
  
  // 連續執行
  for(int i = 0; i < 5; i++){
    if (i % 2 == 0){
      // 由右往左
      power.write(45);
      moving(erase_stop - erase_start);
      power.write(90); 
    }
    if (i % 2 == 1){
      // 由左往右
      power.write(135);
      moving(erase_stop - erase_start);
      power.write(90);
    }
  }
}

void powerMotor_return(){
    // 此時手臂位置在(x1,y1)
    power.write(45);
    moving(power_stop - power_start);
    power.write(90);
}

// 機器手臂主程式
void machine_arm_execute(){
  // 初始化， 移動馬達到指定位置並定義作業範圍
  powerMotor_initialize();

  
  // y軸上移動
  for(){
    Xaxis_erase(); //y軸移動一個單位，便可以沿著X軸清理
  }
  
  Xaxis_erase();

  //完成這次擦黑板的動作後
  powerMotor_return();
}
void loop() {
  // put your main code here, to run repeatedly:
  
  // 呼叫機器手臂擦黑板
  machine_arm_excute();
}
