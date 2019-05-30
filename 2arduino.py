import serial
import time

arduino1=serial.Serial('COM6', 9600)
arduino2=serial.Serial('COM3', 9600)
time.sleep(2)

print("Enter 0 to start homing and 1 to start measurement")

while 1:

    
    datafromUser=input()

    if datafromUser == '0':
        arduino1.write(b'0')
        print("homing")
    elif datafromUser == '1':
        arduino1.write(b'1')
        print("steps")
    
        henrik =[]                       # empty list to store the data
        empty_list = ['1']
        data = []
        
        
        while True:
            arduino1.flushInput()
            
            #if arduino1.inWaiting() > 0:
            for i in range(1):
                b = arduino1.readline()         # read a byte string
                string_n = b.decode()  # decode byte string into Unicode  
                string = string_n.rstrip() # remove \n and \r
                #flt = float(string)        # convert string to float
                #print(flt)
                #print(string)
                henrik.append(string)           # add to the end of data list
                #print(len(henrik))
                #time.sleep(2.1)            # wait (sleep) 0.1 seconds
            
            if len(henrik) != len(empty_list):
                arduino2.flushInput()
                #if arduino2.inWaiting() > 0:
                for i in range(1):
                    c = arduino2.readline()         # read a byte string
                    string2_n = c.decode()  # decode byte string into Unicode  
                    string2 = string2_n.rstrip() # remove \n and \r
                    #flt = float(string2)        # convert string to float
                    #print(flt)
                    #print(string2)
                    data.append(string2)           # add to the end of data list
                    #print(len(call))
                file_data = open(r"data.txt", "w")
                for i in range(len(data)):
                    file_data.write(data[i] + '\n')
                file_data.close()
                empty_list.append('1')
                empty_list.append('1')


                
                

      
                



count = 0
ledData = []


    
       


