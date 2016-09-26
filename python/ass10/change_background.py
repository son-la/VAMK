from tkinter import *
import random

def change(event):
    global root
    color = '#%02x%02x%02x' % (random.randrange(0,255),random.randrange(0,255),random.randrange(0,255))
    root.configure(bg=color)
    

root = Tk()
root.wm_title('Change color')
root.bind('<Enter>',change)
root.mainloop()


