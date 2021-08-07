# This program approximates the grade level
# needed to comprehend some text
# Prompt: https://cs50.harvard.edu/x/2020/psets/2/readability/
from curses.ascii import isalpha


def main():
    text = input("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    index = coleman_liau(letters, words, sentences)
    print(grade(index))


def count_letters(s):
    x = 0
    for i in list(range(len(s))):
        if isalpha(s[i]):
            x = x + 1
    return x


def count_words(s):
    x = 0
    if s[0] != " ":  # first word
        x = x + 1
    for i in list(range(len(s))):  # other words
        if s[i - 1] == " " and s[i] != " ":
            x = x + 1
    return x


def count_sentences(s):
    # In this version,
    # mark all ".", "?", and "!" punctuations
    # as end of sentences.
    # "Mr. and Ms. Smith walks the dog."
    # will be counted as three (3) sentences.
    x = 0
    for i in list(range(len(s))):
        if s[i] in {".", "?", "!"}:
            x = x + 1
    return x


def coleman_liau(a, b, c):
    # The Coleman-Liau index of a text is designed
    # to output what (U.S.) grade level is needed to understand the text.
    l = a / b * 100  # average number of letters per 100 words in the text
    s = c / b * 100  # average number of sentences per 100 words in the text.
    return 0.0588 * l - 0.296 * s - 15.8


def grade(i):
    if i < 1:
        return "Before Grade 1"
    elif i > 16:
        return "Grade 16+"
    else:
        return "Grade " + str(int(round(i)))


main()
