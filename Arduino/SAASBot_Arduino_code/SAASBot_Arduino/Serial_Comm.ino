char buff[32];


void sendPacket_EMG(){
  serverSerial.print("EMG\n");
}


int check_data(){
  bool Ready = 0;
  int cnt = 0;
  while(1){
    if(Ready){
      parseLine(cnt);
      int ret = cnt;
      cnt = 0;
      return ret;
    }
    else{
      if(serverSerial.available()){
        Serial.print("Buff: ");
        Serial.println(serverSerial.available());
        while(serverSerial.available()){
           delay(1);
           char c = serverSerial.read();
          
           buff[cnt++] = c;
           
          // Serial.println(cnt);
           if(c== '\n'){
              Ready = 1;
              Serial.println("Recieved");
              Serial.println(buff);
              break;
           }
        }
      }
      else{
        return cnt;
        Serial.println("E");
      }
    }
  }
}


void parseLine(int len){
  //Serial.println(buff);
  
  if(buff[0]=='A'){
    if(buff[1]=='P'){   //Planting
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      int type = buff[5]-'0';
      move_to(cell);
      auto_planting(cell,pos,type);
    }

    else if(buff[1]=='W'){   // Watering
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      int amount = buff[5]-'0';
      move_to(cell);
      auto_watering(cell,pos,amount);
    }

    else if(buff[1]=='F'){  // Fertilizer
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      int amount = buff[5]-'0';
      move_to(cell);
      auto_fertilizing(cell,pos,amount);
    }

    else if(buff[1]=='L'){  // Live Feed
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      live_feed(cell,pos);
    }

    else if(buff[1]=='R'){  // Find(Remove) Weeds
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      Serial.println("CHECK");
      live_feed(cell,pos);
      delay(2000);
      serverSerial.print("ACK\n");
  
      
    }

     else if(buff[1]=='K'){  // Kill Weeds
      int cell = buff[2]-'0';
      int pos = 10*(buff[3]-'0') + (buff[4]-'0');
      int wx = 100*(buff[5]-'0')+ 10*(buff[6]-'0') + (buff[7]-'0');
      int wy = 100*(buff[8]-'0')+ 10*(buff[9]-'0') + (buff[10]-'0');
      int tr_x = int(map(wx,0,600,900,-1000));
      int tr_y = int(map(wy,0,600,1500,-1500));

      move_x_y(plantmap[pos][0]+tr_x,plantmap[pos][1]+tr_y);
      delay(1000);
      move_down(8000);
      tools.write(weeder);
      move_down(2500);
      delay(3000);
      move_up(3000);
      
      delay(2000);
 
      
    }

     else if(buff[1]=='C'){  // Auto move to Cell
      Serial.println("Moving to Cell");
      int cell = buff[2]-'0';
      move_to(cell);
    }

    
    
    else if(buff[1]=='A'){
      int errorX = 100*(buff[2]-'0')+ 10*(buff[3]-'0') + (buff[4]-'0') - 150;
      int errorY = 100*(buff[5]-'0')+ 10*(buff[6]-'0') + (buff[7]-'0') - 150;
      int errorAng = 1000*(buff[8]-'0')+ 100*(buff[9]-'0') + 10*(buff[10]-'0') + (buff[11]-'0')-1000;
      Serial.println(errorX);
      Serial.println(errorY);
      Serial.println(errorAng);

      if(errorAng!=0){
        if(errorAng>0){   //dakunata          
          int Acc = 30000;
          maxSpeed = 10000;
          roboclaw.SpeedAccelM1M2(address1, Acc,maxSpeed, -maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,maxSpeed, -maxSpeed);
          int temp = errorAng*2 + 50;
          int del = min(150,temp);
          delay(del);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
          
         
       }
     
        else if(errorAng<0){
          int Acc = 30000;
          maxSpeed = 10000;
          roboclaw.SpeedAccelM1M2(address1, Acc,-maxSpeed, maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,-maxSpeed, maxSpeed);
          int temp = -errorAng*2 + 50;
          int del = min(150,temp);
          delay(del);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
         
        }
        
      }

      else if(errorX!=0){
        if(errorX<0){    //right
          int Acc = 200000;
          maxSpeed = 100000;
          roboclaw.SpeedAccelM1M2(address1, Acc,maxSpeed, -maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,-maxSpeed, maxSpeed);
          int temp =  -errorX*10 + 70;
          int del = min(250,temp);
          Serial.println(del);
          delay(del);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
          
        }

        else if(errorX>0){    //left
          int Acc = 200000;
          maxSpeed = 100000;
          roboclaw.SpeedAccelM1M2(address1, Acc,-maxSpeed, maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,maxSpeed, -maxSpeed);
          int temp = errorX*10 + 70;
          int del = min(250,temp);
          Serial.println(del);
          delay(del);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
          
        }
        
      }



       else if(errorY!=0){
        if(errorY<0){    //right
          int Acc = 30000;
          maxSpeed = 10000;
          roboclaw.SpeedAccelM1M2(address1, Acc,-maxSpeed, -maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,-maxSpeed, -maxSpeed);
          int temp = -errorY*8 + 50;
          int del = min(150,temp);
          Serial.println(del);
          delay(del);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
          
        }

        else if(errorY>0){    //left
          int Acc = 30000;
          maxSpeed = 10000;
          roboclaw.SpeedAccelM1M2(address1, Acc,maxSpeed, maxSpeed);
          roboclaw.SpeedAccelM1M2(address2, Acc,maxSpeed, maxSpeed); 
          int temp = errorY*8 + 50;
          int del = min(150,temp);
          Serial.println(del);
          delay(del);
          delay(60);
          roboclaw.SpeedAccelM1M2(address1, Acc,0,0);
          roboclaw.SpeedAccelM1M2(address2, Acc,0,0);
          
        }
        
      }
      
    }
    
  }


  else if(buff[0]=='M'){   // Manual
    if(buff[1]=='W'){    //Watering
      int x = 100* (buff[2]-'0') + 10 * (buff[3]-'0') + (buff[4]-'0');
      int y = 100* (buff[5]-'0') + 10 * (buff[6]-'0') + (buff[7]-'0');
      manual_watering(x,y);
    }

    else if(buff[1]=='F'){  // Fertilizing
      int x = 100* (buff[2]-'0') + 10 * (buff[3]-'0') + (buff[4]-'0');
      int y = 100* (buff[5]-'0') + 10 * (buff[6]-'0') + (buff[7]-'0');
      manual_fertilizing(x,y);
    }

    else if(buff[1]=='R'){   //Removing Weeds
      int x = 100* (buff[2]-'0') + 10 * (buff[3]-'0') + (buff[4]-'0');
      int y = 100* (buff[5]-'0') + 10 * (buff[6]-'0') + (buff[7]-'0');
      manual_weed_removing(x,y);
    }

    else if(buff[1]=='M'){   //Moisture Sensing
      int x = 100* (buff[2]-'0') + 10 * (buff[3]-'0') + (buff[4]-'0');
      int y = 100* (buff[5]-'0') + 10 * (buff[6]-'0') + (buff[7]-'0');
      manual_moisture_sensing(x,y);
    }



    else if(buff[1]=='I'){   //Moisture Sensing
      move_x_y(1210,3100);
      delay(1000);
      serverSerial.print("align\n");
    }

    

    else if(buff[1]=='A'){   //Moisture Sensing
      char mov = buff[2];
      maxSpeed = 10000;
      int del = 20;
      if(mov=='U'){
         roboclaw.SpeedAccelM1M2(address1, Acceleration, maxSpeed, maxSpeed);
         roboclaw.SpeedAccelM1M2(address2, Acceleration, maxSpeed, maxSpeed);
         delay(del);
      }
      else if(mov=='D'){\
         roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, -maxSpeed);
         roboclaw.SpeedAccelM1M2(address2, Acceleration,-maxSpeed, -maxSpeed);
         delay(del);
      }

      else if(mov=='L'){
         roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, maxSpeed);
         roboclaw.SpeedAccelM1M2(address2, Acceleration, maxSpeed, -maxSpeed);
         delay(del);
      }

      else if(mov=='R'){
        roboclaw.SpeedAccelM1M2(address1, Acceleration,maxSpeed, -maxSpeed);
        roboclaw.SpeedAccelM1M2(address2, Acceleration, -maxSpeed, maxSpeed);
        delay(del);
      }
      

      else if(mov=='C'){
        roboclaw.SpeedAccelM1M2(address1, Acceleration, maxSpeed, -maxSpeed);
        roboclaw.SpeedAccelM1M2(address2, Acceleration, maxSpeed, -maxSpeed);
        delay(del);
      }

      else if(mov=='A'){
        roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, maxSpeed);
        roboclaw.SpeedAccelM1M2(address2, Acceleration,-maxSpeed, maxSpeed);
        delay(del);
      }

      else if(mov=='S'){
        roboclaw.SpeedAccelM1M2(address1, Acceleration,0, 0);
        roboclaw.SpeedAccelM1M2(address2, Acceleration,0, 0);
        delay(del);
      }
    }
  } 

  
  
}




