def chunk(sub):
    if len(sub) <= 1:
        return ''
    else:
        new = [1]*(len(sub) - 1)
        for i in range(len(new)):
            new[i] = sub[i+1]
        return new

def sub_str(sub,word,indx):
    if len(sub) == 0:
        print('Found at ',indx)
        return True
    if (sub[0] == word[0]) and sub_str(chunk(sub),chunk(word),indx):
        return True
    else:
        return False


sub = 'bbb'
word = '012bbb678bbb12'


i = 0
while len(word) >= len(sub):
    if sub[0] == word[0] and sub_str(chunk(sub),chunk(word),i):
        for j in range(len(sub)):
            word = chunk(word)
        i = i + len(sub)
    else:
        word = chunk(word)
        i = i + 1

    
