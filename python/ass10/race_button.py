from tkinter import *
def swap(event, bt):
    global bt1,bt2
    if bt == 'bt1':
        bt2.destroy()
    else:
        bt1.destroy()
def up(event):
    bt1.place(x=0,y=0)

root = Tk()
root.geometry("200x250")
root.wm_title('Race')
#root.configure(width = 300, height = 300)

bt1 = Button(root, text = 'Button 1')
bt2 = Button(root, text = 'Button 2')

bt1.bind('<Button-1>',lambda event: swap(event,'bt1'))
bt2.bind('<Button-1>',lambda event: swap(event,'bt2'))
bt2.bind('<Enter>',up)

bt1.pack(side=LEFT)
bt2.pack(side=RIGHT)

root.mainloop()
