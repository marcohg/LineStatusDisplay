from tkinter import *
import sys
import os

if os.environ.get('DISPLAY','') == '':
    print('no display found. Using :0.0')
    os.environ.__setitem__('DISPLAY', ':0.0')


root = Tk()

# Creating a Label Widget
my_label = Label(root, text="Hello Chato!")
# Shoving it into the screen
my_label.pack()

root.mainloop()

