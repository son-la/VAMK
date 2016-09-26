seq = [25,39,52,68,20,89]
fil = lambda x : 0 if (x < 50) else 2 if (x <= 75) and ( x > 50) else 3
print(list(map(fil,seq)))


