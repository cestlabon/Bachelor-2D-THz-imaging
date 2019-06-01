import serial
import time

arduino1=serial.Serial('COM6', 9600)
arduino2=serial.Serial('COM3', 9600)
time.sleep(2)

print("Enter 0 to start homing and 1 to start measurement")

while 1:

    
    datafromUser=input()        

    if datafromUser == '0':        # if user writes 0 it is sendt to the arduione serial port
        arduino1.write(b'0')
        print("Homing")
    elif datafromUser == '1':      # if user writes 0 it is sendt to the arduione serial port
        arduino1.write(b'1')
        print("Measurment Started")
    
        steps_taken =[]           # list to store the steps taken            
        empty_list = []           # empty list to store the data
        data = []                 # list to store the data measurements
        
        
        while True:
            arduino1.flushInput()
            for i in range(1):
                b = arduino1.readline()         # read a byte string
                string_n = b.decode()           # decode byte string into Unicode  
                string = string_n.rstrip()       # remove \n and \r
                #print(string)                    # for debugging
                steps_taken.append(string)            # add to the end of data list
                #print(len(steps_taken))        # for checking only one command is added each time

                
            
            if len(steps_taken) != len(empty_list):  # compares steps_taken with the empty list
                arduino2.flushInput()
                for i in range(1):
                    c = arduino2.readline()         # read a byte string
                    string2_n = c.decode()           # decode byte string into Unicode  
                    string2 = string2_n.rstrip()      # remove \n and \r
                    print(string2)                    # prints the measurements
                    data.append(string2)             # add to the end of data list
                for i in range(len(data)):           # loop for changing array every 50 measurements  
                    output=''
                    col = 0
                    for i in data:
                        output +="%s," % i           #write an element of the array to the output and append a comma
                        col += 1                     #count the number of elements on the current line
                        if col==50:                  #if 50 elements have been entered, append a new line and restart the count
                            output += "\n"
                            col = 0
                    file_data = open(r"data.txt", "w") #open a text file 
                    file_data.write(output)            # write the output to the text file
                    file_data.close()                  #close the file object
                empty_list.append('1')                 # adds an integer to the empty list so it has the same length as steps_taken
               
