import socket
import sys
import serial
from importlib import import_module
import os
from importlib import import_module
import os
from flask import Flask, render_template, Response
from camera_opencv import Camera

flag_stream = 1

app = Flask(__name__)


@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')


def gen(camera):
    """Video streaming generator function."""
    while True:
        print 'k'
        read_serial = ser.readline()
        if read_serial=='ALS\n':
            print 'break'
            break
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
    return

@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    return Response(gen(Camera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')



def shutdown_server():
    func.request.environ.get('werkzeug.server.shutdown')
    if func in None:
        raise RuntimeError('Not running with the werkzeug server')

    func()

@app.route('/shutdown',methods=['POST'])
def shutdown():
    shutdown_server()
    return 'server shutting down..'
#######################################################################
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







def align(data):


    import cv2
    import numpy as np;
    import time 

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

    elif data[0]=='A' and data[1]=='L':
        print 'i'
        if data[2]=='S':
            d=0
        else:
            #flag_stream = 1
            ser.write(data)
          # app.run(host='192.168.0.100', threaded=True)
        
    else:
        print 'other command'
        ser.write(data)
              
s.close()





