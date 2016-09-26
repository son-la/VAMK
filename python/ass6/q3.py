import re

regex1 = r"[+][0-9]{3}[-][0-9]{2}[-][0-9]{7}"
regex2 = r"[0-9]{6}[-][0-9]{3}[A-Z]"
regex3 = r"[0-9]{3}"
regex4 = r"[A-Z][a-z]{2}[.] [0-3]?[0-9]. [0-9]{3}"

test1 = "+358-40-1345678"
test2 = "120570-467W"
test3 = "132"
test4 = "Jan. 1. 2014"

re1 = re.compile(regex1)
re2 = re.compile(regex2)
re3 = re.compile(regex3)
re4 = re.compile(regex4)

print(re1.search(test1))
print(re2.search(test2))
print(re3.search(test3))
print(re4.search(test4))
