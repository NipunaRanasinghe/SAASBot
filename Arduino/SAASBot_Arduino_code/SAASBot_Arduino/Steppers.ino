void step_y_smooth(long count, int Delay,bool Dir){            //count, pulse_delay(millis), dir
  digitalWrite(PIN_EN_Y,HIGH);
  digitalWrite(PIN_DIR_Y,Dir);
  long rem = count;
  long cur_delay = 2000;
  long rem_cur_delay = 0;
  long max_delay = 2000;
  for(int x=0; x<count; x++){
    rem = count-x;
    
    cur_delay = 1000 - 2*x;
    rem_cur_delay = 1000 - 2*rem;
    max_delay = max(cur_delay,rem_cur_delay);
    if(max_delay<Delay){
      max_delay = Delay;
    }
    
    digitalWrite(PIN_STEP_Y,HIGH);
    delayMicroseconds(max_delay);
    digitalWrite(PIN_STEP_Y,LOW);
    delayMicroseconds(max_delay);
   // Serial.println(max_delay);
    
  }

  digitalWrite(PIN_EN_Y,LOW);
}

void step_y_calib(int Delay){

  digitalWrite(PIN_EN_Y,HIGH);
  digitalWrite(PIN_DIR_Y,0);   //forward
  
  long cur_delay = 2000;
 
  long max_delay = 2000;
  
  for(int x=0; digitalRead(PIN_SW1_Y) || digitalRead(PIN_SW2_Y); x++){
    
    cur_delay = 1000 - x;
    
    max_delay = max(cur_delay,Delay);
    
    
    digitalWrite(PIN_STEP_Y,HIGH);
    delayMicroseconds(max_delay);
    digitalWrite(PIN_STEP_Y,LOW);
    delayMicroseconds(max_delay);
   // Serial.println(max_delay);
    
  }

  digitalWrite(PIN_EN_Y,LOW);

  
  
}






void step_x_calib(int Delay){

  digitalWrite(PIN_EN_X,HIGH);
  digitalWrite(PIN_DIR_X,1);   //right
  
  long cur_delay = 2000;
 
  long max_delay = 2000;
  
  for(int x=0; digitalRead(PIN_SW_X); x++){
    
    cur_delay = 1000 - x;
    
    max_delay = max(cur_delay,Delay);
    
    
    digitalWrite(PIN_STEP_X,HIGH);
    delayMicroseconds(max_delay);
    digitalWrite(PIN_STEP_X,LOW);
    delayMicroseconds(max_delay);
   // Serial.println(max_delay);
    
  }

  digitalWrite(PIN_EN_X,LOW);

}

















void step_x_y_smooth(long xcount, int xDelay,bool xDir, long ycount, int yDelay,bool yDir){            //count, pulse_delay(millis), dir
  digitalWrite(PIN_EN_Y,HIGH);
  digitalWrite(PIN_DIR_Y,yDir);
  digitalWrite(PIN_EN_X,HIGH);
  digitalWrite(PIN_DIR_X,xDir);


  long yrem = ycount;
  long ycur_delay = 1000;
  long yrem_cur_delay = 0;
  long ymax_delay = 1000;
  
  long xrem = xcount;
  long xcur_delay = 1000;
  long xrem_cur_delay = 0;
  long xmax_delay = 1000;

  int x=0;
  int y=0;
  
  while(x<xcount || y<ycount){


    if(x<xcount){
    xrem = xcount-x;
    xcur_delay = 800 - x;
    xrem_cur_delay = 800 - xrem;
    xmax_delay = max(xDelay,max(xcur_delay,xrem_cur_delay));
    if(y<ycount){
      xmax_delay=xmax_delay/2;
    }
    
    digitalWrite(PIN_STEP_X,HIGH);
    delayMicroseconds(xmax_delay);
    digitalWrite(PIN_STEP_X,LOW);
    delayMicroseconds(xmax_delay);
    }

    if(y<ycount){
    yrem = ycount-y;
    ycur_delay = 1000 - y;
    yrem_cur_delay = 1000 - yrem;
    ymax_delay = max(yDelay,max(ycur_delay,yrem_cur_delay));
    if(x<xcount){
      ymax_delay=ymax_delay/2;
    }

    digitalWrite(PIN_STEP_Y,HIGH);
    delayMicroseconds(ymax_delay);
    digitalWrite(PIN_STEP_Y,LOW);
    delayMicroseconds(ymax_delay);
    }

    x++;
    y++;

  }
   
  

  digitalWrite(PIN_EN_Y,LOW);
  digitalWrite(PIN_EN_X,LOW);

  currentX += (2*xDir-1)*xcount;
  currentY += (2*yDir-1)*ycount;

}



void step_x_y_initial(long ycount, int yDelay,bool yDir, long xcount, int xDelay,bool xDir){            //count, pulse_delay(millis), dir
  digitalWrite(PIN_EN_Y,HIGH);
  digitalWrite(PIN_DIR_Y,yDir);
  digitalWrite(PIN_EN_X,HIGH);
  digitalWrite(PIN_DIR_X,xDir);


  long yrem = ycount;
  long ycur_delay = 1000;
  long yrem_cur_delay = 0;
  long ymax_delay = 1000;
  
  long xrem = xcount;
  long xcur_delay = 1000;
  long xrem_cur_delay = 0;
  long xmax_delay = 1000;

  int x=0;
  int y=0;
  
  while(x<xcount || y<ycount){


    if(x<xcount){
    xrem = xcount-x;
    xcur_delay = 800 - x;
    xrem_cur_delay = 800 - xrem;
    xmax_delay = max(xDelay,max(xcur_delay,xrem_cur_delay));
    if(y<ycount){
      xmax_delay=xmax_delay/2;
    }
    
    digitalWrite(PIN_STEP_X,HIGH);
    delayMicroseconds(xmax_delay);
    digitalWrite(PIN_STEP_X,LOW);
    delayMicroseconds(xmax_delay);
    }

    if(y<ycount){
    yrem = ycount-y;
    ycur_delay = 1000 - y;
    yrem_cur_delay = 1000 - yrem;
    ymax_delay = max(yDelay,max(ycur_delay,yrem_cur_delay));
    if(x<xcount){
      ymax_delay=ymax_delay/2;
    }

    digitalWrite(PIN_STEP_Y,HIGH);
    delayMicroseconds(ymax_delay);
    digitalWrite(PIN_STEP_Y,LOW);
    delayMicroseconds(ymax_delay);
    }

    x++;
    y++;

  }
   
  

  digitalWrite(PIN_EN_Y,LOW);
  digitalWrite(PIN_EN_X,LOW);

}
