import tkinter as tk
import os
from tkinter import *
from tkinter import simpledialog
import serial
import threading
from PIL import Image, ImageTk
import os

import math

PL_VALS = 6
GM_VALS = 1
NUM_SINGLE = 3

red = "#FF0000"
green = "#00FF00"

gamemode = 0
ser = -1

gameVals = [[0]*PL_VALS, [0]*PL_VALS, [0]*GM_VALS]

class gamestate:
    def __init__(self, score1, score2, shots1, shots2, dist1, dist2, tot1, tot2, pbs, pbd, pbt):
        self.score1 = score1 #these are all TKINTER LABELS, not values/ints/strings
        self.score2 = score2
        self.shots1 = shots1
        self.shots2 = shots2
        self.dist1 = dist1
        self.dist2 = dist2
        self.tot1 = tot1
        self.tot2 = tot2
        self.pbs = pbs
        self.pbd = pbd
        self.pbt = pbt

old_vals = [["0"]*PL_VALS, ["0"]*PL_VALS, ["0"]*GM_VALS]

# Function to read data from serial port
# def read_serial(ser, label):
#     while True:
#         try:
#             data = ser.readline().decode().strip()  # Read data from serial port
#             # Assuming data format is "Score: <score>"
#             if data.startswith("Score:"):
#                 score = data.split(":")[1].strip()
#                 label.config(text="Score: " + score)  # Update label text with score
#         except Exception as e:
#             print("Error reading serial:", e)
def select_background(mode):
    init_backgrounds = ["multi.1png", "noneset.png", "error.png", "single1.png", "zen.png"]

    if mode == 3: 
        bg_select = 1
    elif mode == -3:
        bg_select = -11
    else:
        bg_select = mode

    update_background(init_backgrounds[bg_select+2])


def init_labels():
    global root
    global ser
    global dot_label
    global gamemode

    background_colour = "#000000"
    text_colour = "#FFFFFF"

    select_background(gamemode)

    score1 = score2 = shots1 = shots2 = dist1 = dist2 = tot1 = tot2 = pbs = pbd = pbt = 0

    score1 = Label(root, text=old_vals[0][0], font=("Eras Bold ITC",90), fg=text_colour, bg=background_colour)
    score1.place(x=85, y=125)

    shots1 = Label(root, text=old_vals[0][1], font=("Eras Bold ITC",45), fg=text_colour, bg=background_colour)
    shots1.place(x=85, y=75)

    if gamemode < 0:
        score2 = Label(root, text=old_vals[1][0], font=("Eras Bold ITC",90), fg=text_colour, bg=background_colour)
        score2.place(x=1210, y=125, anchor = 'ne')

        shots2 = Label(root, text=old_vals[1][1], font=("Eras Bold ITC",45), fg=text_colour, bg=background_colour)
        shots2.place(x=1210, y=75, anchor = 'ne')

        dot_label = Label(root, text = "    ", font=("Eras Bold ITC",20), fg=green, bg=green)
        dot_label.place(x = 250, y = 283)

    if abs(gamemode) != 2:
        dist1 = Label(root, text="0", font=("Eras Bold ITC",20), fg=text_colour, bg=red)
        dist1.place(x=(560 if gamemode < 0 else 745), y=385, anchor = 'n')

        tot1 = Label(root, text=old_vals[0][2], font=("Eras Bold ITC",70), fg=text_colour, bg=background_colour)
        tot1.place(x=(85 if gamemode < 0 else 1200), y=(350 if gamemode < 0 else 140), anchor = ('nw' if gamemode < 0 else 'ne'))

    if gamemode < 0 and gamemode != -2:
        dist2 = Label(root, text="0", font=("Eras Bold ITC",20), fg=text_colour, bg=red)
        dist2.place(x=750, y=385, anchor = 'ne')

        tot2 = Label(root, text=old_vals[1][2], font=("Eras Bold ITC",70), fg=text_colour, bg=background_colour)
        tot2.place(x=1210, y=350, anchor = 'ne')

    if gamemode == 1:
        pbs = Label(root, text=old_vals[0][3], font=("Eras Bold ITC",30), fg=text_colour, bg=background_colour)
        pbs.place(x=110, y=570)

        pbd = Label(root, text=old_vals[0][4], font=("Eras Bold ITC",30), fg=text_colour, bg=background_colour)
        pbd.place(x=1150, y=500, anchor = 'n')

        pbt = Label(root, text=old_vals[0][5], font=("Eras Bold ITC",30), fg=text_colour, bg=background_colour)
        pbt.place(x=1200, y=570, anchor = 'n')

    global state
    state = gamestate(score1, score2, shots1, shots2, dist1, dist2, tot1, tot2, pbs, pbd, pbt)
    
    # if ser!=-1:
    #     serial_thread = threading.Thread(target=read_serial, args=([ser]))
    #     serial_thread.daemon = True
    #     serial_thread.start()
    # else: raise Exception(f"Invalid Serial Number ({ser})")

    serial_thread = threading.Thread(target=read_serial, args=([ser]))
    serial_thread.daemon = True
    serial_thread.start()

def update(label, data):
    label.config(text=data)

def set_com(ser_num):
    if os.name == 'nt': 
        port_name = 'COM' + str(ser_num)
    else:  
        port_name = '/dev/ttyUSB' + str(ser_num)
    return port_name

def continue_game():
    global ser_port
    global gamemode
    data = "1"
    ser_port.write(data.encode('utf-8'))

    select_background(gamemode)

def read_serial(ser):
    print("red")
    p1_distance_set = False
    ready = False
    global old_vals
    global ser_port
    
    while True:
        if ser_port.in_waiting != 0:
            global gamemode
            global background
            global state
            global root

            #port = set_com(ser)
            
            i=0
            data_ls = []
            while True:
                character = ser_port.read(1).decode('utf-8')
                if character == '\n':
                    break
                data_ls.append(1)
                data_ls[i] = character
                i += 1
            
            data = ''.join(data_ls)

            print(f"Received: {data}")

            if data == "":
                return

            if data[0] == 'R':
                ready = True
                continue
            elif ready == False:
                continue

            if abs(gamemode) != 2 and data[0] == 'D': #distance measurement is set
                if gamemode > 0:
                    update_background("single2.png")
                    state.dist1.config(background=green)
                elif gamemode < 0:
                    if p1_distance_set:
                        update_background("multidist.png")
                        state.dist2.config(background=green)
                    else:
                        update_background("homeset.png")
                        state.dist1.config(background=green)
                        p1_distance_set = True

            elif abs(gamemode) != 2 and data[0] == 'S':
                distance = int(data.lstrip('S'))
                if p1_distance_set:
                    update(state.dist2, distance)
                else:
                    update(state.dist1, distance)

            elif data[0] == 'W':
                if data[1] == '0':
                    update_background(background, "over.png")
                elif data[1] == '1':
                    update_background(background, "whome.png")
                elif data[1] == '2':
                    update_background(background, "waway.png")
                else:
                    print("error")

                B = Button(text = "Play Again?", command = continue_game())
                B.place(x=500, y=100)

            
            elif data.count(',') == 12:
                new_vals = [[0]*PL_VALS, [0]*PL_VALS, [0]*GM_VALS]
                new_vals_list = data.split(",")
                new_vals[0] = new_vals_list[0:PL_VALS] #values for player 1
                new_vals[1] = new_vals_list[PL_VALS:2*PL_VALS] #values for player 2
                new_vals[2] = new_vals_list[2*PL_VALS:(2*PL_VALS+GM_VALS)] #game values

                if gamemode == 0:
                    return
                
                if new_vals[0][0] != old_vals[0][0]:
                    update(state.score1, new_vals[0][0])
                    old_vals[0][0] = new_vals[0][0]

                if new_vals[0][1] != old_vals[0][1]:
                    update(state.shots1, new_vals[0][1])
                    old_vals[0][1] = new_vals[0][1]
                    
                if abs(gamemode) != 2:
                    if new_vals[0][2] != old_vals[0][2]:
                        update(state.tot1, new_vals[0][2])
                        old_vals[0][2] = new_vals[0][2]
                
                if gamemode == 1:
                    if new_vals[0][3] != old_vals[0][3]:
                        update(state.pbs, new_vals[0][3])
                        old_vals[0][3] = new_vals[0][3]

                    if new_vals[0][4] != old_vals[0][4]:
                        update(state.pbd, new_vals[0][4])
                        old_vals[0][4] = new_vals[0][4]

                    if new_vals[0][5] != old_vals[0][5]:
                        update(state.pbt, new_vals[0][5])
                        old_vals[0][5] = new_vals[0][5]
                        
                elif gamemode < 0:
                    if new_vals[1][0] != old_vals[1][0]:
                        update(state.score2, new_vals[1][0])
                        old_vals[1][0] = new_vals[1][0]
                    
                    if new_vals[1][1] != old_vals[1][1]:
                        update(state.shots2, new_vals[1][1])
                        old_vals[1][1] = new_vals[1][1]

                    if new_vals[2][0] != old_vals[2][0]:
                        switchTurn(int(new_vals[2][0]))
                        old_vals[2][0] == new_vals[2][0]
                    
                    if gamemode != -2:
                        if new_vals[1][2] != old_vals[1][2]:
                            update(state.tot2, new_vals[1][2])
                            old_vals[1][2] = new_vals[1][2]
                else:
                    print("Dodgy")
        else:
            pass

def switchTurn(turn):
    global root
    global dot_label

    dot_label.place(x = 250+780*turn, y = 283)

def update_background(newimg):
    global background
    img = ImageTk.PhotoImage(get_image(newimg))
    background.configure(image=img)
    background.image = img

def exit_fullscreen(event):
    root.attributes("-fullscreen", False)

def get_image(filename):
    # Get the directory of the current script
    script_dir = os.path.dirname(__file__)

    # Combine the directory and the filename to get the full path
    image_path = os.path.join(script_dir, os.path.join("Backgrounds", filename))

    image = Image.open(image_path)

    return image

def get_ends():
    global root

    # Create a custom dialog for user input
    class InputDialog(simpledialog.Dialog):
        def body(self, master):
            self.changed_index = -1
            self.changed_value = 0

            # Create and place labels and entry widgets
            tk.Label(master, text="Score:").grid(row=0)
            tk.Label(master, text="Shots:").grid(row=1)
            tk.Label(master, text="Total Distance:").grid(row=2)

            self.entries = []
            for i in range(3):
                entry = tk.Entry(master)
                entry.grid(row=i, column=1)
                entry.bind("<Return>", self.on_enter)
                self.entries.append(entry)

            return self.entries[0]  # Set focus to the first entry

        def on_enter(self, event):
            self.changed_index = -1
            self.changed_value = 0
            for index, entry in enumerate(self.entries):
                try:
                    value = int(entry.get())
                    if entry.get():
                        self.changed_index = index
                        self.changed_value = value
                        break
                except ValueError:
                    continue
            self.ok()  # Close the dialog

        def apply(self):
            self.result = (self.changed_index, self.changed_value)

    # Show the dialog and get the result
    dialog = InputDialog(root)
    return dialog.result

def mode_select(event):
    #print("modselect")
    global gamemode
    global ser_port

    #ser_port.write(str(event.char).encode('utf-8'))

    if gamemode != 0:
        return
    else:
        global state

        try:
            input_mode = int(event.char)
        except: raise Exception(f"Please enter a valid gamemode (must be number - you entered {event.char}")

        two_p = input_mode > NUM_SINGLE
        gamemode = (1-two_p)*input_mode - two_p*(input_mode - NUM_SINGLE)

        output = f"{gamemode}"

        # ser_port.write(output.endcode('utf-8'))

        if abs(gamemode) == 3:
            end_condition = get_ends()
            # output = f"{end_condition[0]}:{end_condition[1]}"
            # ser_port.write(output.endcode('utf-8'))
            # print(f"Sent to serial: {output}")

        if -3 <= gamemode <= 3:
            init_labels()
        else:
            raise Exception(f"Please enter a valid gamemode (out of range). You entered {input_mode}")

# Function to create GUI
def create_gui():
    
    global root
    root = tk.Tk()
    root.title("Scoreboard")
    root.geometry("1280x720")
    #root.attributes("-fullscreen", True)  # Make the window fullscreen

    bg = ImageTk.PhotoImage(get_image("init.png"))
    global background
    background = Label( root, image = bg) 
    background.place(x = 0, y = 0) 

    root.bind('<Key>', mode_select)

    root.mainloop()

def get_ser():
    top = Tk()
    L1 = Label(top, text="Serial Number")
    L1.pack( side = LEFT)
    E1 = Entry(top, bd =5)
    E1.pack(side = RIGHT)

    callback = lambda event: set_ser(E1, top)
    top.bind('<Return>', callback)

    top.mainloop()

def set_ser(E1, top):
    global ser
    ser = int(E1.get())
    top.destroy()

def setup_serial():
    global ser
    port = set_com(ser)

    global ser_port
    ser_port = serial.Serial(
        port=port,\
        baudrate=115200,\
        parity=serial.PARITY_NONE,\
        stopbits=serial.STOPBITS_ONE,\
        bytesize=serial.EIGHTBITS,\
            timeout=0)
    
    if ser_port.is_open:
        print("Serial port is open")


def test_serial():
    global ser
    port = set_com(ser)

    global ser_port
    ser_port = serial.Serial(
        port=port,\
        baudrate=115200,\
        parity=serial.PARITY_NONE,\
        stopbits=serial.STOPBITS_ONE,\
        bytesize=serial.EIGHTBITS,\
            timeout=0)
    
    if ser_port.is_open:
        print("Serial port is open")
    
    while True:
        # Check if there is any data waiting in the buffer
        if ser_port.in_waiting != 0:
            i=0
            message = []
            while True:
                data = ser_port.read(1).decode('utf-8')
                if data == '\n':
                    break
                message.append(1)
                message[i] = data
                i += 1
            
            message_str = ''.join(message)
            # Read the incoming data
            print(f"Received: {message_str}")
        else:
            # No data waiting, you can do other tasks here
            pass


if __name__ == "__main__":
    get_ser()
    
    #test_serial()

    setup_serial()
    create_gui()