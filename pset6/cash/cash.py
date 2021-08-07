# This program calculates the minimum number of coins
# that is required to comprise a certain amount of change money
# Prompt: https://cs50.harvard.edu/x/2020/psets/1/cash/
dollars = str()


def check_float(string):
    try:
        float(string)
        return True
    except ValueError:
        return False


while check_float(dollars) == False or float(dollars) <= 0:
    dollars = input("Change: ")

cents = round(float(dollars) * 100)
quarters = int(cents / 25)
dimes = int(cents % 25 / 10)
nickles = int(cents % 25 % 10 / 5)
pennies = int(cents % 25 % 10 % 5)

print(quarters+dimes+nickles+pennies)

# print(quarters, " quarters,", dimes, " dimes,", nickles, " nickels, and", pennies, " pennies.")
