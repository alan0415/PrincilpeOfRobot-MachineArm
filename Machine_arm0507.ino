#include<Servo.h>

Servo power; // 動力馬達
Servo base;  // 基座馬達
Servo jointLower; // 低位關節馬達
Servo jointHigher; // 高位關節馬達

// 變數宣告
int power_teeth = 50; // 動力馬達齒輪齒數
int teeth_range = 200; // 齒條齒數
int moving_range = 5000; // 設定齒條總長(單位mm)

// 座標取得
int x1, x2, y1, y2;

// 實際板擦範圍
int higher_x;
int higher_y;
int lower_x;
int lower_y;

// 擦黑板時所需記錄的參數
int exec_start_angle = 0; //紀錄動力馬達位移到最小x邊界時的角度
int exec_end_angle = 0; // 紀錄動力馬達位移到最大x邊界時的角度
int exec_circle = 0; //紀錄動力馬達從 最小x邊界到最大x邊界的過程 的轉動圈數

void setup() {
  // 機器手臂控制腳位
  power.attach();
  base.attach();
  jointLower.attach();
  jointHigher.attach();
  
}

// 座標轉換
// 以橫向移動軸為x軸，面向黑板最左方為原點
void position_trans(){
  // x軸: 齒條總齒數, y軸: 0-1000
  pass;  
}

// 以圈為單位轉動power motor
void run_a_circle(int num){

}

// 動力馬達移動至指定範圍
// minum x 後在上升機器手臂(Y軸平移)至 y軸 Maximum
void powerMotor_initialize(){
  position_trans();  // 取得清理範圍
  run_circle(lower_x / power_teeth);

  int init_circle = lower_x / power_teeth; // 首次移動到位置時馬達所需轉動的"完整"圈數
  
  // 以圈為單位開始轉動
  for (int i = 0; i < init_circle; i++){
    
  }

  // 轉動，補齊不足一圈的角度
  if ((lower_x % power_teeth) != 0){
    int distance = (lower_x % power_teeth) / 360; // 還需要轉幾度
    for(int i = 0; i <= distance; i+=1){
      power.write(i);
      delay(20);
    }
    exec_start_angle = distance;
  }

  // 計算並記錄動力馬達此次所需轉動圈數、達到最大x邊界時的角度
}


void erase(int times, int start_angle, int run_circle, int end_angle){
  
}


// 機器手臂主程式
void machine_arm_execute(){
  // 初始化， 移動馬達到指定位置並定義作業範圍
  powerMotor_initialize();
  
  
}
void loop() {
  // put your main code here, to run repeatedly:
  
  // 呼叫機器手臂擦黑板
  machine_arm_excute();
}
