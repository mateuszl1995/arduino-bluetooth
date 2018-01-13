
#import serial
#import time

#print("Start")
#port="/dev/rfcomm1" #This will be different for various devices and on windows it will probably be a COM port.
#bluetooth=serial.Serial(port, 9600)#Start communications with the bluetooth unit

#print("Connected")
#bluetooth.flushInput() #This gives the bluetooth a little kick
#print("Ping")
#while 1==1:
#	x = raw_input('Provide string: ');
#	print x
#	bluetooth.write(x)#These need to be bytes not unicode, plus a number
#	input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
#	print(input_data.decode())#These are bytes coming in so a decode is needed
#	time.sleep(0.1) #A pause between bursts
#bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
#print("Done")

from tkinter import *
import bluetooth

print( "Searching for devices...")
print( "")
nearby_devices = bluetooth.discover_devices()
num = 0
print( "Select your device by entering its coresponding number...")
for i in nearby_devices:
	num+=1
	print( num , ": " , bluetooth.lookup_name( i ))

selection = int(input("> ")) - 1
print( "You have selected", bluetooth.lookup_name(nearby_devices[selection]))
bd_addr = nearby_devices[selection]
print(bd_addr)
port = 1

class Application(Frame):

    sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )

    def disconnect(self):
        self.sock.close()
        
    def on(self):
        data = "H"
        self.sock.send(data)

    def off(self):
        data = "L"
        self.sock.send(data)

    def createWidgets(self):
        self.QUIT = Button(self)
        self.QUIT["text"] = "QUIT"
        self.QUIT["fg"]   = "red"
        self.QUIT["command"] =  self.quit

        self.QUIT.pack({"side": "left"})

        self.disconnectFrom = Button(self)
        self.disconnectFrom["text"] = "Disconnect"
        self.disconnectFrom["fg"]   = "darkgrey"
        self.disconnectFrom["command"] =  self.disconnect

        self.disconnectFrom.pack({"side": "left"})

        self.turnOn = Button(self)
        self.turnOn["text"] = "On",
        self.turnOn["fg"] = "green"
        self.turnOn["command"] = self.on

        self.turnOn.pack({"side": "left"})

        self.turnOff = Button(self)
        self.turnOff["text"] = "Off"
        self.turnOff["fg"] = "red"
        self.turnOff["command"] = self.off

        self.turnOff.pack({"side": "left"})

    def __init__(self, master=None):
        self.sock.connect((bd_addr, port))
        print(bd_addr,  port)
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

root = Tk()
app = Application(master=root)
app.mainloop()
root.destroy()
