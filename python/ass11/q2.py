from tkinter import *
from urllib import request

def download():
    global linkEntry
    f = open('pic.gif', 'wb')
    f.write(request.urlopen(linkEntry.get()).read())
    f.close()
    print("Downloaded !")

root = Tk()
root.wm_title('Download pictures')


linkLabel=Label(root,text="Enter link")
linkLabel.grid(row = 0, column = 0)

linkEntry = Entry(root)
linkEntry.grid(row = 0, column=1)

dl = Button(root, text='Download', command=download)
dl.grid(row = 1, column = 1)


root.mainloop()
