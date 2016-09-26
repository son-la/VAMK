# Assignment 5 - Q2
def fileContent(filename):
    'returns the content of file filename'
    try:
       infile=open(filename, 'r')
    except Exception as e:
        print(str(e))
        return ''
    else:
       content=infile.read()
       infile.close()
       return content



content = fileContent('products.txt')
if len(content) != 0:
    content = content.split('\n')
    print(content)
    print('Value each product')
    total = 0
    try:
        for i in range(len(content) - 1):
            temp = content[i].split(';')
            try:
                if int(temp[1])<0 or int(temp[2])<0:
                    raise ArithmeticError
            except ArithmeticError:
                print('ArithmeticError error is raised !!!')
            else:
                
                value = int(temp[1])*int(temp[2])
                total = total + value
                print(temp[0],': ',value)
    except Exception as e:
        print('Check file format for error: ',str(e))
    else:
        print('\n\nSum: ',total)
    
