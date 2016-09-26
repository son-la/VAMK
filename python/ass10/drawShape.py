from tkinter import *

#Here we define an event handler

def drawHandler():
    global shapeListBox, heightEntry, widthEntry, canvas
    canvas.delete("all")
    sel = shapeListBox.curselection()
    
    height = heightEntry.get()
    width = widthEntry.get()
    
        
    if len(sel) == 0 or height=="" or width=="":
        print("Missing input")
        return
    else:
        try:
            height = eval(height)
            width = eval(width)
        except Exception:
            print("Wrong input")
            return
        if height > 100 or width > 280 or height < 0 or width < 0:
            print("Correct the size !!")
            return
        if sel[0] == 0:
            canvas.create_rectangle(10,50,10+width,50+height,fill="blue")
        elif sel[0] == 1:
            canvas.create_oval(10,50,10+width,50+height,fill="blue")

root = Tk()
root.resizable(0,0)
root.wm_title("Drawing")


#Here we define a ListBox widget
shapeLabel = Label(root, text="Select shape")
shapeLabel.grid(row = 0,column = 0)

shapeListBox = Listbox(highlightcolor='red')
shapeListBox.insert(1, "Rectangle")
shapeListBox.insert(2, "Oval")

shapeListBox.grid(row = 1, column = 0, rowspan = 2)

#Here we bind left mouse button double-click event handler


shapeLabel = Label(root, text="Dimension")
shapeLabel.grid(row = 0,column = 1, columnspan = 2)

heightEntry=Entry(root)
widthEntry=Entry(root)

heightEntry.grid(row = 1, column = 2)
widthEntry.grid(row = 2, column = 2)

heightLabel = Label(root, text="Height(0 to 100)")
heightLabel.grid(row = 1,column = 1)

widthLabel = Label(root, text="Width(0 to 280)")
widthLabel.grid(row = 2,column = 1)

button=Button(root, text='Draw',command=drawHandler)
button.grid(row = 3, column = 1)

#Here we define another Listbox widget

canvas=Canvas(root,height=150,width=300)
canvas.grid(row = 4, column = 0, columnspan=3)

root.mainloop()

