"""
Prints out the full name and birth year
of all Hogwarts house of a particular house,
sorted by last name and birth year.
"""

from cs50 import SQL
from sys import argv

# Checks if user inputs one of the four Hogwarts houses
if len(argv) != 2:
    print("Usage: $ python roster.py Hufflepuff")
    exit()

# Opens SQL output file
db = SQL("sqlite:///students.db")

# Creates a list of dictionaries containing data from SQL
roster = db.execute("SELECT first, middle, last, birth \
                    FROM students \
                    WHERE students.house=? \
                    ORDER BY last, first;", argv[1])

for row in roster:
    if row['middle'] is None:
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
