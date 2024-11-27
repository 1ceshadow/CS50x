while True:
    a = input("Height:")

    try:
        a = int(a)
    except ValueError:
        continue
    if (a >= 1 and a <= 8):
        break

for i in range(1, a+1):
    print(" "*(a - i), end="")
    print("#"*i)

