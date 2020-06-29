#include<Servo.h>

Servo power;
Servo base;
Servo jointLow;
Servo jointHigh;

/* 
 *  基本參數 
 */

// 有關旋轉角度的參數
float DistanceToBlackboard = 5.0; // 機器手臂到黑板的水平距離
float BlackboardAngle = 70.0; // 黑板與水平地面夾角
float r1 = 5.0; // 機器手臂第一段(靠近底座那段)的長度
float r2 = 5.0; // 機器手臂第二段的長度
float theta1, theta2, theta3; //宣告base, jointLow, jointHigh馬達的旋轉角度

// 有關擦拭過程需要用到的參數
float shift_angle;

int X_range = 3; //定義滑軌長度

// 安全係數

// 宣告從樹梅派取得的參數
int x1; //要擦的範圍，左下角(x1, y1)及右上角(x2,y2)
int y1;
int x2;
int y2;

void setup() {
  // put your setup code here, to run once:
  // 機器手臂控制腳位
  power.attach(5);
  base.attach(6);
  jointLow.attach(7);
  jointHigh.attach(8);
  // 初始化
  base.write(0);
  jointLow.write(0);
  jointHigh.write(0);
}

void loop() {
  /*
   *  車子需要移位幾次，如果要擦的範圍(x2-x1)超過滑軌最大長度時，擦完需要移動車子繼續擦
   */
  int executionTimes = (x2-x1) / X_range ; 
  if ( (x2-x1) % X_range != 0){
      executionTimes += 1;
  }
  /* 解聯立方程式
    r1*cos(90-theta1) + r2*cos(90-theta1-theta2) = DistanceToBlackboard
    r1*sin(90-theta1) + r2*sin(90-theta1-theta2) = y2
  */
  for(int i = 0; i < executionTimes; i++){
      jointHigh.write(180-theta1-theta2-BlackboardAngle);
      jointLow.write(theta2);
      base.write(theta1);

      // Loop(包住重複擦拭的Loop): 控制板擦在y方向上的移動
      
      // 重複擦拭
      for (int j = 0 ; j < 5 ; j++){
         power.write(120); //方向x1 -> x2轉
         // 方案一: 搭配樹梅派定位，當移動到x2時停下並記錄移動時間
         // 方案二: 紀錄固定轉速下的移動速度，手動計算大約時間
         power.detach();
         power.attach(5);
         power.write(60); //x2->x1
         power.detach();
         power.attach(5);
      }
  }
}
