""" Imports a CSV file of Hogwarts students data into a SQL database """

from cs50 import SQL
from sys import argv
import csv


def main():
    # Checks if user provides the data source (CSV)
    if len(argv) != 2:
        print("Usage: $ python import.py characters.csv")
        exit()

    else:
        with open(argv[1], "r") as source:
            # Reads CSV source file
            # into three dicts: 'name', 'house', and 'birth.'
            reader = csv.DictReader(source)

            with open("students.db", "w") as database:
                # Opens SQL output file
                db = SQL("sqlite:///students.db")

                # Creates table within database
                db.execute("CREATE TABLE students \
                            ('first' TEXT, \
                            'middle' TEXT, \
                            'last' TEXT, \
                            'house' TEXT, \
                            'birth' TEXT);")

                for row in reader:
                    # Splits 'name' dict into three variables
                    name = row['name'].split()
                    first = parse(name)[0]
                    middle = parse(name)[1]
                    last = parse(name)[2]

                    house = row['house']
                    birth = row['birth']

                    db.execute("INSERT INTO students (first, middle, last, house, birth) \
                    VALUES (?, ?, ?, ?, ?);", first, middle, last, house, birth)

                # TODO
                # Check if students.db has been successfully updated.


def parse(name):
    """
    Parses the full name of a Hogwarts student
    and returns a list containing
    their first, middle (if applicable), and last name.

    Assumes that students' names either have two or three words.
    If a student doesn't have a middle name, it will return None.
    """
    if len(name) == 3:
        return name
    else:
        return [name[0], None, name[1]]


main()
