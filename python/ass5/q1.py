# Assignment 5 - Q1

def writeToFile(content, filename):
    'appends content to the filename'
    outfile=open(filename, 'w')
    outfile.write(content)
    outfile.close()

def fileContent(filename):
   'returns the content of file filename'
   infile=open(filename, 'r')
   content=infile.read()
   infile.close()
   return content
    

#Get data and write to file
def first_part():
    
    #Place holders
    name = []
    price = []
    amount = []

    #Continuously asking
    c = 'y'
    while c == 'y':
        content = input('Enter product following the form [name],[unit price],[amount] E.g. Bomb,1000,1 : ')
        content = content.split(',')
        if len(content) != 3:
            print("Wrong format! Check again")
            continue
        else:
            #Extract data
            name.append(content[0])
            price.append(content[1])
            amount.append(content[2])
            
            #Check data
            if len(content[0]) == 0 or len(content[1]) == 0 or len(content[2])==0:
                print("Missing input !")
                continue
            else:
                if not(content[1].isdigit()) or  not(content[2].isdigit()):#Check digit or greater than 0
                    print('Unit price and amount must be number or greater than 0 !')
                    continue
                elif eval(content[1]) <= 0 or eval(content[2]) <= 0:
                    print('price and amount must be greater than 0')
                    continue
                    
            c = input('Continue? (y/n): ')
            
    content = ''           
    if c == 'n':
        for j in range(len(name)):
            content = content + name[j] + ';' + price[j] + ';' + amount[j] + '\n'

        writeToFile(content,'products.txt')
        return True
    else:
        return False

#Read from file
def second_part():
    content = fileContent('products.txt')
    return content



#Main 
if first_part(): #Write to file sucess?
    #Read from file
    content = second_part().split('\n')
    #Search
    choice = input('Search:\n1. Name\n2. Unit price\n3. Amount\n Choose 1,2 or 3: ')
    key = input('Enter search key: ')

    try:
        if (int(choice) == 2 or int(choice) == 3) and int(key) < 0:
            raise ArithmeticError
    except ArithmeticError:
        print('ArithmeticError is raised')
    except:
        print('Wrong input')
    else:    
        print('\n\n\nFound: ')
        for i in range(len(content) - 1):
            temp = content[i].split(';')#Split ';'
            if str(key) == temp[int(choice)-1]:#Check match
                print(content[i])
    finally:
        print('\nQuit')

    
    
