from cs50 import get_int

while True:
    cc = get_int("Insert C/C number: ")
    if cc > 0:
        break

cc_length = len(str(cc))

# Check valid lengths
if cc_length not in (13, 15, 16):
    print("INVALID")
    exit()

# Luhn checksum calculation
checksum = 0
alternate = False
num = cc

while num > 0:
    digit = num % 10
    if alternate:
        digit *= 2
        checksum += digit // 10 + digit % 10
    else:
        checksum += digit
    alternate = not alternate
    num //= 10

# If checksum is invalid
if checksum % 10 != 0:
    print("INVALID")
    exit()

# Extract first two digits
first_two = int(str(cc)[:2])
first_one = int(str(cc)[0])

# Determine card type
if (first_two in (34, 37)) and cc_length == 15:
    print("AMEX")
elif first_one == 4 and cc_length in (13, 16):
    print("VISA")
elif 51 <= first_two <= 55 and cc_length == 16:
    print("MASTERCARD")
else:
    print("INVALID")
