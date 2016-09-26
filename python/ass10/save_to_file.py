from tkinter import *

def add(event):
    global itemListBox

    item = itemListBox.get(event.widget.nearest(event.y))
    print("Added!")

    file = open('list.txt','a')
    file.write(item + '\n')
    file.close()


def delete(event):
    global itemListBox

    widget = event.widget
    index = widget.nearest(event.y)
    itemListBox.delete(index);


root = Tk()
root.wm_title('List')

itemListBox = Listbox()
itemListBox.insert(1, "Item1")
itemListBox.insert(2, "Item2")
itemListBox.insert(3, "Item3")

itemListBox.bind("<Double-1>",add)
itemListBox.bind("<Double-3>",delete)


itemListBox.pack()

root.mainloop()
