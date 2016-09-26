from tkinter import *
from tkinter.ttk import *
import datetime
import DB_functions


#Graphical User Interface and Business Logics
class GUI:
    #Start applications
    def Start(self):
        self.__db_con = DB_functions.DBConnection('concert.db','concert')
        self.__create_environment()
        
    #Create environment    
    def __create_environment(self):

        #Root frame configuration
        self.__root = Tk()
        self.__root.geometry('800x400')
        self.__root.wm_title('Concert Management')
        
        self.__root.grid_columnconfigure(index = 0, weight = 1)
        self.__root.grid_columnconfigure(index = 1, weight = 10)


        #Create components
        self.__create_insert()
        self.__create_dataTable()
        
        #Start building
        self.__root.mainloop()

##########Data Table###################
    def __create_dataTable(self):
        
        #Data table frame
        self.__dtFrame = Frame(self.__root)
        self.__dtFrame.grid(row = 0, column = 1, padx = 10, pady = 20)
        #Title
        dtLabel = Label(self.__dtFrame, text = 'Data table')
        dtLabel.grid(row = 0, column = 0)
        
        #Table
        data = self.__db_con.select_all()
        self.__tv = Treeview(self.__dtFrame)
        self.__tv['columns'] = ('title', 'date', 'time', 'price')
        self.__tv.column('#0', width = 50, anchor=W)
        self.__tv.heading('#0', text = 'ID')
        self.__tv.column('title', width=150, anchor='center')
        self.__tv.heading('title', text='Title')
        self.__tv.column('date', width=80, anchor='center')
        self.__tv.heading('date', text='Date')
        self.__tv.column('time', width=80, anchor='center')
        self.__tv.heading('time', text='Time')
        self.__tv.column('price', width=80, anchor='center')
        self.__tv.heading('price', text='Price')

        #Write data to table
        for i in range(len(data)):
            self.__tv.insert("" , 0, text=data[i][0], values=(data[i][1],data[i][2], data[i][3], data[i][4]))

        self.__tv.grid(row = 1, column = 0)
                  
     
##########Insert frame #######################   
    def __create_insert(self):
        self.__insertFrame = Frame(self.__root)
        self.__insertFrame.grid(row = 0, column = 0, sticky = W, padx = 10, pady = 20)

        #Configuration
        self.__insertFrame.grid_rowconfigure(index=0,weight=1)
        self.__insertFrame.grid_rowconfigure(index=1,weight=1)
        self.__insertFrame.grid_rowconfigure(index=2,weight=1)
        self.__insertFrame.grid_rowconfigure(index=3,weight=1)
        self.__insertFrame.grid_rowconfigure(index=4,weight=1)
        self.__insertFrame.grid_rowconfigure(index=5,weight=1)
        self.__insertFrame.grid_columnconfigure(index=0,weight=1)
        self.__insertFrame.grid_columnconfigure(index=1,weight=1)

        #Items

        #Title
        self.__insertLabel = Label(self.__insertFrame,text='New data')
        self.__insertLabel.grid(row = 0, column = 0, columnspan = 2)
        
        #Name
        self.__titleLabel = Label(self.__insertFrame,text='Title')
        self.__titleLabel.grid(row = 1, column = 0, sticky=W)
        self.__titleEntry = Entry(self.__insertFrame)
        self.__titleEntry.grid(row = 1, column = 1)

        #Date
        self.__dateLabel = Label(self.__insertFrame,text='Date (E.g. 12/12/12)')
        self.__dateLabel.grid(row = 2, column = 0, sticky=W)
        self.__dateEntry = Entry(self.__insertFrame)
        self.__dateEntry.grid(row = 2, column = 1)

        #Time
        self.__timeLabel = Label(self.__insertFrame,text='Time (E.g. 14:20)')
        self.__timeLabel.grid(row = 3, column = 0, sticky=W)
        self.__timeEntry = Entry(self.__insertFrame)
        self.__timeEntry.grid(row = 3, column = 1)
        
        
        #Price
        self.__priceLabel = Label(self.__insertFrame,text='Price')
        self.__priceLabel.grid(row = 4, column = 0, sticky=W)
        self.__priceEntry = Entry(self.__insertFrame)
        self.__priceEntry.grid(row = 4, column = 1)
        
        #Insert Button
        self.__insertButton = Button(self.__insertFrame,text = 'Insert',command=self.__insert)
        self.__insertButton.grid(row = 5, column = 1, sticky=E)


###### Binding functions ###############

#Insert function############
    def __insert(self):
        #Get input
        name = self.__titleEntry.get()
        date = self.__dateEntry.get()
        time = self.__timeEntry.get()
        price = self.__priceEntry.get()

        #Check input
        if name=="" or date=="" or time=="" or price =="":
            print("Missing input")
            return
        else:
            try:
                price = eval(price)
                if price <= 0:
                    print("price must be > 0")
                    return
                datetime.datetime.strptime(str(date),'%d/%m/%y')#Check conversion
                datetime.datetime.strptime(str(time),'%H:%M')#Check conversion
            except Exception:
                print("Wrong input")
                return

        #Insert data
        self.__db_con.insert(name,date,time,price)
        
        #Clear input
        self.__titleEntry.delete(0, END)
        self.__dateEntry.delete(0, END)
        self.__timeEntry.delete(0, END)
        self.__priceEntry.delete(0, END)

        #Update data table
        self.__tv.delete(*self.__tv.get_children());
        data = self.__db_con.select_all()
        for i in range(len(data)):
            self.__tv.insert("" , 0, text=data[i][0], values=(data[i][1],data[i][2], data[i][3], data[i][4]))



       

