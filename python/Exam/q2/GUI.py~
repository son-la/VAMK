from tkinter import *
from tkinter.ttk import *

import DB_functions


#Graphical User Interface and Business Logics
class GUI:
    #Start applications
    def Start(self):
        self.__db_con = DB_functions.DBConnection('product.db','product')
        self.__create_environment()
        
    #Create environment    
    def __create_environment(self):

        #Root frame configuration
        self.__root = Tk()
        self.__root.geometry('500x600')
        self.__root.wm_title('Products Management')
        
        self.__root.grid_columnconfigure(index = 0, weight = 1)
        self.__root.grid_columnconfigure(index = 1, weight = 10)

        self.__root.grid_rowconfigure(index = 0, weight = 1)
        self.__root.grid_rowconfigure(index = 1, weight = 1)

        #Create components
        self.__create_insert()
        self.__create_search()
        self.__create_dataTable()
        self.__create_search_result()
        
        #Start building
        self.__root.mainloop()

##########Search Result##################
    def __create_search_result(self):
        #Search result frame
        self.__srFrame = Frame(self.__root)
        self.__srFrame.grid(row = 1, column = 1, sticky = E, padx = 10, pady = 20)
        #Title
        srLabel = Label(self.__srFrame, text = 'Search result')
        srLabel.grid(row = 0, column = 0)
        
        #Table
        self.__srtv = Treeview(self.__srFrame)
        self.__srtv['columns'] = ('name', 'amount', 'price')
        self.__srtv.column('#0', width = 50, anchor=W)
        self.__srtv.heading('#0', text = 'ID')
        self.__srtv.column('name', width=50, anchor='center')
        self.__srtv.heading('name', text='Name')
        self.__srtv.column('amount', width=50, anchor='center')
        self.__srtv.heading('amount', text='Amount')
        self.__srtv.column('price', width=50, anchor='center')
        self.__srtv.heading('price', text='Price')
        
        self.__srtv.grid(row = 1, column = 0)


        
##########Data Table###################
    def __create_dataTable(self):
        
        #Data table frame
        self.__dtFrame = Frame(self.__root)
        self.__dtFrame.grid(row = 0, column = 1, sticky = E, padx = 10, pady = 20)
        #Title
        dtLabel = Label(self.__dtFrame, text = 'Data table')
        dtLabel.grid(row = 0, column = 0)
        
        #Table
        data = self.__db_con.select_all()
        self.__tv = Treeview(self.__dtFrame)
        self.__tv['columns'] = ('name', 'amount', 'price')
        self.__tv.column('#0', width = 50, anchor=W)
        self.__tv.heading('#0', text = 'ID')
        self.__tv.column('name', width=50, anchor='center')
        self.__tv.heading('name', text='Name')
        self.__tv.column('amount', width=50, anchor='center')
        self.__tv.heading('amount', text='Amount')
        self.__tv.column('price', width=50, anchor='center')
        self.__tv.heading('price', text='Price')

        #Write data to table
        for i in range(len(data)):
            self.__tv.insert("" , 0, text=data[i][0], values=(data[i][1],data[i][2], data[i][3]))

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
        self.__insertFrame.grid_columnconfigure(index=0,weight=1)
        self.__insertFrame.grid_columnconfigure(index=1,weight=1)

        #Items

        #Title
        self.__insertLabel = Label(self.__insertFrame,text='New data')
        self.__insertLabel.grid(row = 0, column = 0, columnspan = 2)
        
        #Name
        self.__inameLabel = Label(self.__insertFrame,text='Name')
        self.__inameLabel.grid(row = 1, column = 0, sticky=W)
        self.__inameEntry = Entry(self.__insertFrame)
        self.__inameEntry.grid(row = 1, column = 1)
        
        #Amount
        self.__iamountLabel = Label(self.__insertFrame,text='Amount')
        self.__iamountLabel.grid(row = 2, column = 0, sticky=W)
        self.__iamountEntry = Entry(self.__insertFrame)
        self.__iamountEntry.grid(row = 2, column = 1)
        
        #Price
        self.__ipriceLabel = Label(self.__insertFrame,text='Price')
        self.__ipriceLabel.grid(row = 3, column = 0, sticky=W)
        self.__ipriceEntry = Entry(self.__insertFrame)
        self.__ipriceEntry.grid(row = 3, column = 1)
        
        #Insert Button
        self.__insertButton = Button(self.__insertFrame,text = 'Insert',command=self.__insert)
        self.__insertButton.grid(row = 4, column = 1, sticky=E)


##########Search frame ################
    def __create_search(self):
        self.__sFrame = Frame(self.__root)
        self.__sFrame.grid(row = 1, column = 0, sticky = W, padx = 10)

        #Configuration
        self.__sFrame.grid_rowconfigure(index=0,weight=1)
        self.__sFrame.grid_rowconfigure(index=1,weight=1)
        self.__sFrame.grid_rowconfigure(index=2,weight=1)
        self.__sFrame.grid_rowconfigure(index=3,weight=1)
        self.__sFrame.grid_columnconfigure(index=0,weight=1)
        self.__sFrame.grid_columnconfigure(index=1,weight=1)

        #Items

        #Title
        self.__sLabel = Label(self.__sFrame,text='Search data')
        self.__sLabel.grid(row = 0, column = 0, columnspan = 2)
        
        #Name
        self.__snameLabel = Label(self.__sFrame,text='Name')
        self.__snameLabel.grid(row = 1, column = 0, sticky=W)
        self.__snameEntry = Entry(self.__sFrame)
        self.__snameEntry.grid(row = 1, column = 1)
        
        #Price
        self.__spriceLabel = Label(self.__sFrame,text='Price')
        self.__spriceLabel.grid(row = 2, column = 0, sticky=W)
        self.__spriceEntry = Entry(self.__sFrame)
        self.__spriceEntry.grid(row = 2, column = 1)
        
        #Search Button
        self.__sButton = Button(self.__sFrame,text = 'Search',command=self.__search)
        self.__sButton.grid(row = 3, column = 1, sticky=E)



###### Binding functions ###############

#Insert function############
    def __insert(self):
        #Get input
        name = self.__inameEntry.get()
        amount = self.__iamountEntry.get()
        price = self.__ipriceEntry.get()

        #Check input
        if name=="" or amount=="" or price =="":
            print("Missing input")
            return
        else:
            try:
                amount = eval(amount)
                price = eval(price)
                if amount != round(amount) or amount <= 0:
                    print("amount must be integer and > 0")
                    return
                if price <= 0:
                    print("price must be > 0")
                    return
            except Exception:
                print("Wrong input")
                return

        #Insert data
        self.__db_con.insert(name,amount,price)
        
        #Clear input
        self.__inameEntry.delete(0, END)
        self.__iamountEntry.delete(0, END)
        self.__ipriceEntry.delete(0, END)

        #Update data table
        self.__tv.delete(*self.__tv.get_children());
        data = self.__db_con.select_all()
        for i in range(len(data)):
            self.__tv.insert("" , 0, text=data[i][0], values=(data[i][1],data[i][2], data[i][3]))



#Search function #############
    def __search(self):

        #Clear data
        self.__srtv.delete(*self.__srtv.get_children());            

        name = self.__snameEntry.get()
        price = self.__spriceEntry.get()
        result = []
        #Check input
        if name=="" and price =="":
            print("Missing input")
            return
        else:
            if price == "": #Search by name
                result = self.__db_con.search_name(name)
            elif name == "": #Search by price
                try:
                    price = eval(price)
                    if price <= 0:
                        print("price must be > 0")
                        return
                except Exception:
                    print("Wrong input")
                    return
                result = self.__db_con.search_price(price)
            else: #Search by both name and price
                try:
                    price = eval(price)
                    if price <= 0:
                        print("price must be > 0")
                        return
                except Exception:
                    print("Wrong input")
                    return
                result = self.__db_con.search_name_price(name, price)
                
        #Update search result
        if len(result) != 0:
            for i in range(len(result)):
                self.__srtv.insert("" , 0, text=result[i][0], values=(result[i][1],result[i][2],result[i][3]))



       

