# This program takes height between 1 and 8 as an input
# and prints out a pyramid with that height
# Prompt: https://cs50.harvard.edu/x/2020/psets/1/mario/more

height = input("Height: ")

while height.isdigit() == False or int(height) < 1 or int(height) > 8:
    height = input("Height: ")


for i in list(range(1, int(height) + 1)):
    # for each row (starting from 1 instead of 0)

    for j in list(range(int(height) - i)):
        # print " " (height - row) times
        print(" ", end="")

    for k in list(range(i)):
        # print "#" (row) times
        print("#", end="")

    print("  ", end="")
    # print the gap between the pyramids

    for k in list(range(i)):
        # print "#" (row) times
        print("#", end="")

    print("")
    # go to the next line
