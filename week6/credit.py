while True:
    num = input("Number: ")
    if num.isdigit():
        num = int(num)
        break

length = len(str(abs(num)))
header = num // 10**(length-2)
case = ""

if length == 15 and (header == 34 or header == 37):
    case = "AMEX"

elif length == 16 and (header in range(51, 56)):
    case = "MASTERCARD"

elif (length == 13 or length == 16) and (header in range(40, 50)):
    case = "VISA"

# 校验
if case:
    total, i = 0, 0
    while num > 0:
        digit = num % 10
        num = num // 10
        if i % 2 == 1:  # 从倒数第二位开始乘2
            digit *= 2
            if digit > 9:  # 如果乘2后大于9，需要减去9（相当于加上两个数字）
                digit -= 9
        total += digit
        i += 1

    if total % 10 == 0:
        print(case)
    else:
        print("INVALID")
else:
    print("INVALID")
