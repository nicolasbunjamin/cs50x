# This program approximates the grade level
# needed to comprehend some text
# Prompt: https://cs50.harvard.edu/x/2020/psets/2/substitution/
from curses.ascii import isalpha


def main():
    key = input("key: ")
    char_check(key)
    alpha_check(key)
    repetition_check(key)

    plaintext = input("plaintext: ")
    encipher(key.lower(), plaintext)


def char_check(s):
    if len(s) != 26:
        print("Key must contain 26 alphabetic characters")
        exit(1)


def alpha_check(s):
    for i in list(range(26)):
        if not isalpha(s[i]):
            print("Key must only contain alphabetic characters")
            exit(1)


def repetition_check(s):
    repetition = False
    for i in list(range(26)):
        for j in list(range(i)):
            if s[i] == s[j]:
                repetition = True
        for k in list(range(i + 1, 26)):
            if s[i] == s[k]:
                repetition = True
    if repetition:
        print("Key must not contain repeated characters")
        exit(1)


def encipher(s, t):
    text = [char for char in t]
    for i in list(range(len(text))):
        if text[i].isupper():
            text[i] = s[ord(text[i]) - 65].upper()
        if text[i].islower():
            text[i] = s[ord(text[i]) - 97].lower()
        print(text[i], end="")
    print("")


main()
