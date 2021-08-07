from sys import argv
import csv


def main():
    global repeats
    global dna_sequence
    global max_reps
    global reader

    if len(argv) != 3:
        print("Usage: $ python dna.py database.csv sequence.txt")

    else:
        # Opens CSV database
        with open(argv[1], "r") as database:
            reader = csv.reader(database)

            # Notes the names of Short Tandem Repeats (STR) in database
            repeats = list(next(reader))[1:]

            # Initializes the list of longest runs of consecutive STRs
            max_reps = [0] * len(repeats)

            # Opens DNA sequence for comparison
            file = open(argv[2], "r")
            dna_sequence = file.read()

            # Converts reader into a list for easier manipulation
            reader = list(reader)

            count()
            match()


def count():
    # Initializes counter and tracker
    for i in range(len(repeats)):
        counter = 0
        tracker = 0

        # Traverse the DNA sequence, checking for STRs
        for j in range(len(dna_sequence)):
            if dna_sequence[j:j+len(repeats[i])] == repeats[i]:

                # Tracks the position of the last repetition of an STR
                tracker = j
                # Increments the counter
                counter += 1

                if counter > max_reps[i]:
                    # Updates the record of longest runs of consecutive STRs
                    max_reps[i] = counter

            else:
                # Resets counter to 0 if the consecutive run is broken
                if j-tracker > len(repeats[i]):
                    counter = 0


def match():
    # Initializes matcher
    matcher = [False] * len(repeats)

    for line in (reader):
        for i in range(len(line)):
            # Compares the longest runs counted and the database
            if line[i] == str(max_reps[i-1]):
                matcher[i-1] = True
            else:
                matcher[i-1] = False

        if all(matcher) == True:
            # Prints the DNA owner
            print(line[0])
            exit()

    print("No Match")


main()