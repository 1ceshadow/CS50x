while True:
    a = input("Height:")
    try:
        a = int(a)
    except ValueError:
        continue
    if a in range(1, 9):
        break

for i in range(1, a+1):
    print(" "*(a-i), end="")
    print("#"*i, "", "#"*i)
