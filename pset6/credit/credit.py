# This program determines if a credit card is
# AMEX, MASTERCARD, VISA, INVALID, or of unknown identity


def main():
    credit_card = str()
    while not check_input(credit_card):
        credit_card = input("Credit Card Number: ")
    luhns_algorithm(credit_card)
    check_valid(credit_card)
    check_brand(credit_card)


def check_input(string):
    # check if the input from user can be converted into (long) integer
    try:
        int(string)
        return True
    except ValueError:
        return False


def luhns_algorithm(string):
    doubled_digits = []
    remaining_digits = []
    luhn_total = int()
    for i in list(range(-2, -(len(string) + 1), -2)):
        # take every other digit starting from the 2nd to last
        doubled_digits.append(int(string[i]) * 2)
        # double it
    for doubled_digit in doubled_digits:
        for j in [0, 1]:
            # take every digit of the product (e.g. 12 becomes 1 and 2)
            luhn_total += int(doubled_digit / 10 ** j % 10)
            # add the sum to luhn_total
    for k in list(range(-1, -(len(string) + 1), -2)):
        # take the rest of the digits (the ones NOT doubled)
        remaining_digits.append(int(string[k]))
    for digit in remaining_digits:
        # add the sum to luhn_total
        luhn_total += digit
    return int(luhn_total % 10) == 0


def check_valid(string):
    if luhns_algorithm(string):
        return
    else:
        print("INVALID")
        exit()


def check_brand(string):
    if string[0:2] == "34" or string[0:2] == "37":
        print("AMEX")
    elif 51 <= int(string[0:2]) <= 55:
        print("MASTERCARD")
    elif string[0] == "4":
        print("VISA")
    else:
        print("UNKNOWN")


main()
