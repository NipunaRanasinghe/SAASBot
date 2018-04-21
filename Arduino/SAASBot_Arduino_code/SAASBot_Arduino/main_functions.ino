void check_tools(){
   move_x_y(1300,1300);
 digitalWrite(PIN_VER_EN,HIGH);
 digitalWrite(PIN_VER_DIRB,LOW);
 move_x_y(0,6500);
 digitalWrite(PIN_VER_EN,LOW);
 digitalWrite(PIN_VER_DIRB,HIGH);
 tools.write(seeder);
 delay(300);
 tools.write(weeder);
 digitalWrite(PIN_VER_EN,HIGH);
 digitalWrite(PIN_VER_DIRA,LOW);
 move_x_y(1300,1300);
 digitalWrite(PIN_VER_EN,LOW);
 digitalWrite(PIN_VER_DIRA,HIGH);
 initial_calib();
}


void pickseed(int type){
  if(type>=1 && type<=6){
    Serial.println(type);
   
    int x = 2600-160*(6-type);
    Serial.println(x);
    Serial.println(currentX);
   
   
    move_x_y(x,0);
  }

  else if(type>=7 && type<=12){
    int x = 2600-160*(12-type);
    move_x_y(x,380);
  }
  
}











void initial_calib(){
  vertical_calib();
  delay(200);
  step_y_calib(800);
  step_x_calib(800);
  delay(100);
  
  currentX=2600;
  currentY=0;
  
}

void move_x_y(int x,int y){
   int diffX = abs(x - currentX);
   int diffY = abs(y - currentY);
   bool dirX = x > currentX;
   bool dirY = y > currentY;
   if(x<=2600 && x>=0 && y<=6500 && y>=0){
      step_x_y_smooth(diffX,400,dirX,diffY,400,dirY);
   }
   
   
}


void move_to(int cell){
  vertical_calib();
  delay(1000);
  if(cell==currentCell){
    serverSerial.print("pass\n");
  }
  else{
    move_A_B(cell);
    move_x_y(1210,3100);
    delay(1000);
    serverSerial.print("align\n");
    
    
  }
}

void move_A_B(int cell){
  maxSpeed = 10000;
  int delayX = 12000;
  int delayY = 11700;
  if(currentCell<=2 && cell>=3){
    
    roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, -maxSpeed);
    roboclaw.SpeedAccelM1M2(address2, Acceleration,-maxSpeed, -maxSpeed);
    delay(delayY);
    roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);
    currentCell += 2;
    delay(2000);
  }

   else if(currentCell>=3 && cell<=2){
    roboclaw.SpeedAccelM1M2(address1, Acceleration,maxSpeed, maxSpeed);
    roboclaw.SpeedAccelM1M2(address2, Acceleration,maxSpeed, maxSpeed);
    delay(delayY);
    roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);
    currentCell -= 2;
    delay(2000);
    
  }

  if (currentCell!=2 && (cell-currentCell)==1){   //move right
    roboclaw.SpeedAccelM1M2(address1, Acceleration,maxSpeed, -maxSpeed);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, -maxSpeed, maxSpeed);
    delay(delayX);
    roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);
    currentCell += 1;
    delay(2000);
    
  }

  else if (currentCell!=3 && (cell-currentCell)==-1){   //move left
    roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, maxSpeed);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, maxSpeed, -maxSpeed);
    delay(delayX);
    roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
    roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);
    currentCell -= 1;
    delay(2000);
    
  }

}



void auto_planting(int cell,int pos,int type){
  Serial.println("planting");
  Serial.println(cell);
  Serial.println(pos);
  Serial.println(type);
  
  if(currentY>1300){
    tools.write(seeder);
    delay(500);
    initial_calib();
    delay(500);
  }
  else{
    move_x_y(1300,1300);
    tools.write(seeder);
    delay(500);
    initial_calib();
    delay(500);
  }
  
  pickseed(type);
  delay(500);
  move_down(1200);
  delay(500);
  digitalWrite(PIN_MOTOR_SEED,HIGH);
  delay(1000);
  vertical_calib();
  delay(100);
  move_x_y(plantmap[pos][0],plantmap[pos][1]);
  
  delay(500);
  //vertical_adjust(20);
  move_down(9000);
  delay(500);
  digitalWrite(PIN_MOTOR_SEED,LOW);
  delay(7000);
  tools.write(weeder);
  
 
  //vertical_adjust(6);
  move_down(1500);
  delay(1000);
  move_up(3000);
  
  
}


void auto_watering(int cell,int pos,int amount){
  Serial.println("Watering");
  Serial.println(cell);
  Serial.println(pos);
  Serial.println(amount);
  vertical_calib();
  move_x_y(plantmap[pos][0]+200,plantmap[pos][1]-300);
  delay(1000);
  digitalWrite(PIN_MOTOR_WTR,HIGH);
  int del = 1000 + 1000*amount;
  delay(del);
  digitalWrite(PIN_MOTOR_WTR,LOW);
 
}

void auto_fertilizing(int cell,int pos,int amount){
  Serial.println("Fertilizing");
  Serial.println(cell);
  Serial.println(pos);
  Serial.println(amount);
  vertical_calib();
  move_x_y(plantmap[pos][0]+200,plantmap[pos][1]-300);
  delay(1000);
  digitalWrite(PIN_MOTOR_FERT,HIGH);
  int del = 1000 + 1000*amount;
  delay(del);
  digitalWrite(PIN_MOTOR_FERT,LOW);
}

void live_feed(int cell,int pos){
  Serial.println("Live Feed");
  Serial.println(cell);
  Serial.println(pos);
  vertical_calib();
  move_to(cell);
  move_x_y(plantmap[pos][0]-camera_offset[0],plantmap[pos][1]-camera_offset[1]);
  
 
}

















void manual_watering(int x,int y){
   int new_x = int(map(x,0,500,200,2000));
   int new_y = int(map(y,0,750,200,6500));
   Serial.println(new_x);
   Serial.println(new_y);
   vertical_calib();
   move_x_y(new_x,new_y);
   delay(500);
   digitalWrite(PIN_MOTOR_WTR,HIGH);
   delay(2000);
   digitalWrite(PIN_MOTOR_WTR,LOW);
   
}

void manual_fertilizing(int x,int y){
   Serial.println("Manual Fertilizing");
   int new_x = int(map(x,0,500,200,2000));
   int new_y = int(map(y,0,750,200,6500));
   Serial.println(new_x);
   Serial.println(new_y);
   vertical_calib();
   move_x_y(new_x,new_y);
   delay(500);
   digitalWrite(PIN_MOTOR_FERT,HIGH);
   delay(2000);
   digitalWrite(PIN_MOTOR_FERT,LOW);
   
  
}


void manual_weed_removing(int x,int y){
   Serial.println("Manual Weed Removing");
   vertical_calib();
   int new_x = int(map(x,0,500,200,2000));
   int new_y = int(map(y,0,750,200,6500));
   Serial.println(new_x);
   Serial.println(new_y);
   move_x_y(new_x,new_y);
   move_down(8000);
   tools.write(weeder);
   move_down(2500);
  // vertical_adjust(7);
   delay(1000);
   move_up(3000);
   
}


void manual_moisture_sensing(int x,int y){
   Serial.println("Manual Moisture Sensing");
   int new_x = int(map(x,0,500,200,2000));
   int new_y = int(map(y,0,750,200,6500));
   Serial.println(new_x);
   Serial.println(new_y);
   move_x_y(new_x,new_y);
   move_down(8000);
   tools.write(soilsensor);
   move_down(2500);
  // vertical_adjust(7);
   delay(1000);
   move_up(3000);
}

