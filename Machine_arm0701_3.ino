#include<Servo.h>
#include<time.h>

Servo power;
Servo base;
Servo jointLow;
Servo jointHigh;

/* 
 *  基本參數 
 */

// 有關旋轉角度的參數
float DistanceToBlackboard = 5.0; // 機器手臂到黑板的水平距離
float BlackboardAngle = 90.0; // 黑板與水平地面夾角
float r1 = 5.0; // 機器手臂第一段(靠近底座那段)的長度
float r2 = 5.0; // 機器手臂第二段的長度
double theta1 = 15.0; //宣告base, jointLow, jointHigh馬達的旋轉角度
double theta2, theta3; 
float Distance; //手臂基座到板擦接觸白板的中心點之x軸距離

// 有關擦拭過程需要用到的參數
float motorSpeed = 1; //在給定v速度下單位時間移動距離； motor.write(v)
int X_range = 3.0; //定義滑軌長度
int eraserWidth = 7; // 板擦長度
int BC_x = 0; //當擦拭範圍過大需要移動車子時，用來存x2

// 宣告從樹梅派取得的參數
int x1; //要擦的範圍，左下角(x1, y1)及右上角(x2,y2)
int y1;
int x2;
int y2;

int img_boardLength_x = 992-211;
int img_boardLength_y = 554-54;
float real_board_x = 13;
float real_board_y = 10;

void setup() {
  // put your setup code here, to run once:
  // 機器手臂控制腳位
  power.attach(5);
  base.attach(6);
  jointLow.attach(7);
  jointHigh.attach(8);
  // 初始化
  base.write(0);
  jointLow.write(90);
  jointHigh.write(90);
}

void loop() {
  /*
   *  車子需要移位幾次，如果要擦的範圍(x2-x1)超過滑軌最大長度時，擦完需要移動車子繼續擦
   */
  float local_x1 = (x1 - 211) / img_boardLength_x * real_board_x;
  float local_x2 = (x2 - 211) / img_boardLength_x * real_board_x;
  float local_y1 = (y1 - 54) / img_boardLength_y * real_board_y;
  float local_y2 = (y1 - 54) / img_boardLength_y * real_board_y;
  
  int executionTimes = (local_x2-local_x1) / X_range ; 
  if ( (local_x2-local_x1 * (local_x2 / local_x1)) - X_range != 0){
      executionTimes += 1;
  }
  if(executionTimes > 1){
    BC_x = local_x2;
  }

  // 移動到x1
  float t1 = millis();
  power.write(30); // 給定一個速度
  float t2 = millis();
  while ((t2-t1) < (local_x1/motorSpeed)){
    t2 = millis();
  }
  power.write(0);
    
  /*  計算角度
   *  解聯立方程式
    r1*cos(90-theta1) + r2*cos(90-theta1-theta2) = DistanceToBlackboard
    r1*sin(90-theta1) + r2*sin(90-theta1-theta2) = y2
    -> cot(90-theta1) + cot(90-theta1-theta2) = DistanceToBlackboard / y2
  */
  
  theta2 = 90 - theta1 - atan(1/((DistanceToBlackboard / y2)-(1/tan(90-theta1))));
  // theta2 無解?

  /*
   * 執行擦黑板動作
   */
  for(int i = 0; i < executionTimes; i++){
    // 如果擦拭範圍過大需要移動車子時，需要對單次(x軸上)擦拭邊界做出對應修正
    local_x1 = local_x1 + i * X_range;
    float fix_x2; // x2針對單次移動範圍做出的修正
    if (BC_x != 0){
       if ( i != executionTimes -1){
          fix_x2 = local_x1 + X_range;
        }
       else{
          fix_x2 = local_x2;
        }
    }
    else{
      fix_x2 = local_x2;  
    }
      // 控制馬達旋轉角度(+90)
      jointHigh.write(270-theta1-theta2-BlackboardAngle);
          delay(500);
      for(int i = 0; i < 91+theta2; i++){
          jointLow.write(i);
          delay(30);
      }
      delay(500);
      base.write(theta1);

      /*
       * 控制板擦在y方向上的移動
       */
       // 計算在y方向需要移動幾次
      int yaxis_times = (local_y2-local_y1) / eraserWidth;
      if (((local_y2-local_y1 * (local_y2 / local_y1)) - eraserWidth) != 0){
          yaxis_times += 1;
        }
      // 在y方向做移動，並開始擦黑板
      for (int j = 0; j < yaxis_times; j++){
          // 重複擦拭
          for (int j = 0 ; j < 5 ; j++){
            // 方案二: 紀錄固定轉速下的移動速度，手動計算大約時間
            float movingTime = (local_x2-x1) / motorSpeed;
            float t1 = millis();
            power.write(30); //方向x1 -> x2轉
            float t2 = millis();
            while ((t2 - t1) < movingTime){
              t2 = millis();
            }
            power.write(0);
            delay(30);

            // 反轉
            t1 = millis();
            power.write(-30); //方向x2 -> x1轉
            t2 = millis();
            while ((t2 - t1) < movingTime){
              t2 = millis();
            }
          }
          
          //更新手臂基座到板擦中心之x軸距離
          theta2 = 90 - theta1 - atan(1/((DistanceToBlackboard / (y2-(eraserWidth)*(j+1)))-(1/tan(90-theta1))));

          jointHigh.write(270-theta1-theta2-BlackboardAngle);
          delay(500);
          for(int i = 0; i < 91+theta2; i++){
            jointLow.write(i);
            delay(30);
          }
          delay(500);
          base.write(theta1);
      }
      
      /* 
       * 擦完黑板後復位 
       */
      // x軸歸位
      float t1 = millis();
      power.write(-30); // 給定一個反方向速度
      float t2 = millis();
      while ((t2-t1) < (x1/motorSpeed)){
        t2 = millis();
      }
      power.write(0);

      // 角度歸位(垂直)
      base.write(0);
      jointLow.write(90);
      jointHigh.write(90);

  }
}
