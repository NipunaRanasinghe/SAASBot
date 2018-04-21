void vertical_calib(){
   digitalWrite(PIN_VER_EN,HIGH);
   digitalWrite(PIN_VER_DIRA,LOW);
   
   while(digitalRead(PIN_SW_VER)==HIGH){
    
   }
   digitalWrite(PIN_VER_DIRA,HIGH);
   digitalWrite(PIN_VER_EN,LOW);
}

void move_down(int del){
  
   digitalWrite(PIN_VER_EN,HIGH);
   digitalWrite(PIN_VER_DIRB,LOW);
   delay(del);
   digitalWrite(PIN_VER_DIRB,HIGH);
   digitalWrite(PIN_VER_EN,LOW);
 
}


void move_up(int del){
  
   digitalWrite(PIN_VER_EN,HIGH);
   digitalWrite(PIN_VER_DIRA,LOW);
   delay(del);
   digitalWrite(PIN_VER_DIRA,HIGH);
   digitalWrite(PIN_VER_EN,LOW);
 
}


void vertical_adjust(int height){
  
  delay(50);
  long cur = sonar.ping_median(20);
  int current = sonar.convert_cm(cur);
 
  if (current>height){
       digitalWrite(PIN_VER_EN,HIGH);
       digitalWrite(PIN_VER_DIRB,LOW);
       do {
         cur = sonar.ping_median(20);
         current = sonar.convert_cm(cur);
         Serial.println(current);
       }
       while(current>height);
       
  
    digitalWrite(PIN_VER_DIRB,HIGH);  
    digitalWrite(PIN_VER_EN,LOW);
  }

  else if (current<height){
       digitalWrite(PIN_VER_EN,HIGH);
       digitalWrite(PIN_VER_DIRA,LOW);
       do {
         cur = sonar.ping_median(20);
         current = sonar.convert_cm(cur);
         Serial.println(current);
       }
       while(current<height);

   digitalWrite(PIN_VER_DIRA,HIGH);  
   digitalWrite(PIN_VER_EN,LOW);
   
  }
  
}

