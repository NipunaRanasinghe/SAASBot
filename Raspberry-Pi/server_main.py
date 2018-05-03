import socket
import sys
import serial
from importlib import import_module
import os
from importlib import import_module
import os
import cv2
import numpy as np;
import time
import math

##########################################################

ser = serial.Serial('/dev/ttyUSB0',115200)

HOST = '192.168.0.100'   
PORT = 8888 

 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Socket created'

#realiza el bind socket
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()
     
print 'Socket bind complete'
 
#inicia  socket
s.listen(10)
print 'Socket now listening'


remove_plants=[]
data_remove=[]

def detect_weeds():

    cap = cv2.VideoCapture(0)
    _, img = cap.read()
    _, img = cap.read()

    
    frame = cv2.resize(img, (600, 600)) 


    # Minimum required radius of enclosing circle of contour
    MIN_RADIUS = 2
    BLACK = [0,0,0]

    print frame.shape[:2]


    (wi,hi)=frame.shape[:2]
    frame1= cv2.rectangle(frame,(0,0),(hi,wi),(0,0,0),10)

    # Blur image to remove noise
    img_filter = cv2.GaussianBlur(frame1.copy(), (3, 3), 0)

    # Convert BGR to HSV
    hsv = cv2.cvtColor(img_filter, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
    lower_blue = np.array([30,110,100])
    upper_blue = np.array([120,255,255])


    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)


    # Taking a matrix of size 5 as the kernel
    kernel = np.ones((5,5), np.uint8)
    kernel2 = np.ones((20,20), np.uint8)
    



    img_erosion_ = cv2.erode(mask, kernel, iterations=1)
    img_erosion = cv2.dilate(img_erosion_, kernel2, iterations=1)

    #img_final= cv2.rectangle(frame,(100,0),(200,100),(0,255,0),4)
    #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.....

    # Set up the SimpleBlobdetector with default parameters.
    params = cv2.SimpleBlobDetector_Params()
     
    # Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 256;
     
    # Filter by Area.
    params.filterByArea = True
    params.minArea = 1100
     
    # Filter by Circularity
    params.filterByCircularity = False
    params.minCircularity = 0.01
     
    # Filter by Convexity
    params.filterByConvexity = False
    params.minConvexity = 0.1
     
    # Filter by Inertia
    params.filterByInertia =False
    params.minInertiaRatio = 0.2
     
    detector = cv2.SimpleBlobDetector_create(params)

    # Detect blobs
    reversemask=255-img_erosion
    keypoints = detector.detect(reversemask)

    
    global remove_plants
    global data_remove
    remove_plants= []
    data_remove=[]


    for k in keypoints:
            x=int(k.pt[0])
            y=int(k.pt[1])
            
            
            dis=int(math.sqrt((300-x)**2+((300-y)**2)))
        
            if dis>100:
                remove_plants.append(k)
                data_remove.append({"x":x,"y":y})
           
                    
            flag=0;
            print "\n"


    print "Remove %d Plants" % len(remove_plants)
    
    # Draw green circles around detected blobs
    #im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (255,0,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    im_with_keypoints1 = cv2.drawKeypoints(frame, remove_plants, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    #im_with_keypoints = cv2.drawKeypoints(frame, save_plants, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)	



    # open windows with original image, mask, res, and image with keypoints marked
    #cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask)
    #cv2.imshow('res',res)     
    #cv2.imshow('img_erosion',img_erosion) 
    #cv2.imshow('img_final',img_final)
    cv2.imshow('reversemask',reversemask)
    cv2.imshow("Plants to remove" ,im_with_keypoints1)       
    #cv2.imshow("Plants to safe" ,im_with_keypoints)    
    #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>........



    #cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask)
    #cv2.imshow('res',res)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    return
    
  





def align(data):
    

    # Setup SimpleBlobDetector parameters.
    params = cv2.SimpleBlobDetector_Params()

    # Change thresholds
    params.minThreshold = 10
    params.maxThreshold = 250


    # Filter by Area.
    params.filterByArea = True
    params.minArea = 300
    params.maxArea = 4000

    # Filter by Circularity
    params.filterByCircularity = True
    params.minCircularity = 0.1

    # Filter by Convexity
    params.filterByConvexity = True
    params.minConvexity = 0.87

    # Filter by Inertia
    params.filterByInertia = True
    params.minInertiaRatio = 0.1

    # Create a detector with the parameters
    detector = cv2.SimpleBlobDetector(params)

    ver = (cv2.__version__).split('.')
    if int(ver[0]) < 3 :
        detector = cv2.SimpleBlobDetector(params)
    else : 
        detector = cv2.SimpleBlobDetector_create(params)

        
    vidcap = cv2.VideoCapture(0)


    for x in range(10):
        success,image = vidcap.read()
    count = 0
    success = True
    flg=0
    while success:
      
      strt = time.time()
      success,image = vidcap.read()
      
      #print('Read a new frame: ', success)
      #cv2.imshow("Keypoints", image)
      #cv2.waitKey(1)
      #cv2.imwrite("frame%d.jpg" % count, image)     # save frame as JPEG file
      count += 1


      #channels = cv2.split(image)

      image = cv2.resize(image,(300, 300));
      imgray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) #channels[2] #
      ret, thresh = cv2.threshold(imgray, 190, 255, cv2.THRESH_BINARY_INV)
      #thresh = cv2.adaptiveThreshold(channels[2],255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,11,2)
      #cv2.imshow("Keypoints", thresh)
      #cv2.waitKey(1)

      # Detect blobs.
      keypoints = detector.detect(thresh)

      x=[]
      y=[]
     
      if len(keypoints)==2:
          flg=0
          
          for k in keypoints:
             x.append(round(k.pt[0]))
             y.append(round(k.pt[1]))
          
          midPointX = int(round((x[0]+x[1])/2))
          midPointY = int(round((y[0]+y[1])/2))
          if(x[0]!=x[1]):
              gradient = int(1000*(y[0]-y[1])/(x[0]-x[1]))
          else:
              gradient = 9999
          if gradient>9999:
              gradient=9999
              
          if abs(midPointX-150)<5:
              midPointX = 150
              
          if abs(midPointY-150)<5:
              midPointY = 150

          if abs(gradient-1000)<50:
              gradient = 1000
              
          print(midPointX-150,midPointY-150,gradient)
          ser.write('AA'+(str(midPointX)).zfill(3)+(str(midPointY)).zfill(3)+(str(gradient)).zfill(4)+'\n')
          print 'AA'+(str(midPointX)).zfill(3)+(str(midPointY)).zfill(3)+(str(gradient)).zfill(4)+'\n'
          if midPointX==150 and midPointY==150 and gradient==1000:
             ser.write(data)
             return
            
      else:
          flg+=1
          
         
      if flg>=20:
          print "No Markers Detected: Align operation Aborted"
          return
         
      # Draw detected blobs as red circles.
      # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
      # the size of the circle corresponds to the size of blob



      im_with_keypoints = cv2.drawKeypoints(thresh, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

      # Show blobs
      cv2.imshow("Keypoints", im_with_keypoints)
      cv2.waitKey(1)
    #  print time.time()-strt

    ser.write(data)
    return










      
      
































#now conversa con  client
i = 0
while s:
    
    #esperando conecccion 
    conn, addr = s.accept()
    data = conn.recv(1024)
    #print 'Connected with ' + addr[0] + ':' + str(addr[1])
    if not data: break
    data = data[:-1]
    print data
    
    if ( data[0]=='M' and data[1]=='C'):
        ser.write('AC'+data[2]+'\n')
        print 'AC'+data[2]
        
        read_serial = ser.readline()
        while read_serial!="align\n" and read_serial!="pass\n":
            read_serial = ser.readline()
              
        if read_serial=='align\n':
            print "Align Operation Started"
            align(data)
        else:
            print "Pass Command Recieved: Ignorng Align Operation"
            ser.write(data)



    elif ( data[0]=='M' and data[1]=='I'):
        ser.write(data)
        
        read_serial = ser.readline()
        while read_serial!="align\n" and read_serial!="pass\n":
            read_serial = ser.readline()
              
        if read_serial=='align\n':
            print "Align Operation Started"
            align('Null\n')
        else:
            print "Pass Command Recieved: Ignorng Align Operation"
            



    elif data[0]=='A' and data[1]=='L':
        if data[2]=='S':
            d=0
        else:
            #flag_stream = 1
            ser.write(data)
          # app.run(host='192.168.0.100', threaded=True)
          
    elif data[0]=='A' and data[1]=='R':
        ser.write(data)
        read_serial = ser.readline()
        while read_serial!="ACK\n":
            read_serial = ser.readline()
        print 'ack recieved'
        detect_weeds()
        print 'return'
        print remove_plants
        if len(remove_plants):
            st = 'AK'+data[2:-1]+str(data_remove[0]['x']).zfill(3)+str(data_remove[0]['y']).zfill(3)+'\n'
            ser.write(st)
            print st
        else:
            ser.write(data)
        
        
    else:
        print 'other command'
        ser.write(data)
              
s.close()





