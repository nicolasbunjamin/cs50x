-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT name FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE courthouse_security_logs.year = 2020 AND courthouse_security_logs.month = 7 AND courthouse_security_logs.day = 28
AND courthouse_security_logs.hour = 10 AND courthouse_security_logs.minute >= 15 AND courthouse_security_logs.minute <= 25 AND courthouse_security_logs.activity = 'exit'
AND atm_transactions.year = 2020 AND atm_transactions.month = 7 AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Fifer Street' AND atm_transactions.transaction_type = 'withdraw'
AND phone_calls.year = 2020 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration <= 60
AND airports.city = 'Fiftyville' AND flights.year = 2020 AND flights.month = 7 AND flights.day = 29 ORDER BY flights.hour LIMIT 1;

SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE phone_calls.year = 2020 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration <= 60
AND phone_calls.caller =
(SELECT phone_number FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE courthouse_security_logs.year = 2020 AND courthouse_security_logs.month = 7 AND courthouse_security_logs.day = 28
AND courthouse_security_logs.hour = 10 AND courthouse_security_logs.minute >= 15 AND courthouse_security_logs.minute <= 25 AND courthouse_security_logs.activity = 'exit'
AND atm_transactions.year = 2020 AND atm_transactions.month = 7 AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Fifer Street' AND atm_transactions.transaction_type = 'withdraw'
AND phone_calls.year = 2020 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration <= 60
AND airports.city = 'Fiftyville' AND flights.year = 2020 AND flights.month = 7 AND flights.day = 29 ORDER BY flights.hour LIMIT 1);

SELECT city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
JOIN passengers ON flights.id = passengers.flight_id
WHERE flights.year = 2020 AND flights.month = 7 AND flights.day = 29 ORDER BY flights.hour LIMIT 1;