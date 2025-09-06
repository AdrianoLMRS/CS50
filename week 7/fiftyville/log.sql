-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = "Humphrey Street";
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- Littering took place at 16:36. No known witnesses.

SELECT * FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND minute = 15;
-- Nothing...
SELECT * FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 16 AND minute = 36;
-- Nothing...

SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2024;
-- Ruth:	Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene:	I don"t know the thief"s name, but it was someone I recognized. Earlier this morning, before I arrived at Emma"s bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond:	As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ...
-- Three witnesses

-- Get info about the witnesses
SELECT * FROM people WHERE name IN ("Ruth", "Eugene", "Raymond");
-- 280744	Eugene	(666) 555-5774	9584465633	47592FJ
-- 430845	Ruth	(772) 555-5770	NULL	HZB4129
-- 937274	Raymond	(125) 555-8030	NULL	Y18DLY3

-- Ruth
SELECT license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25
    AND activity = "exit";
-- License plates:
-- 5P2BI95     94KL13X     6P58WS2     4328GD8     G412CB7     L93JTIZ     322W7JE     0NTHK55

SELECT name, phone_number, passport_number FROM people
    WHERE license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "322W7JE", "0NTHK55");

-- Vanessa      (725) 555-4692      2963008352
-- Barry        (301) 555-4174      7526138472
-- Iman         (829) 555-5269      7049073643
-- Sofia        (130) 555-0289      1695452385
-- Luca         (389) 555-5198      8496433585
-- Diana        (770) 555-1861      3592750733
-- Kelsey       (499) 555-9472      8294398571
-- Bruce        (367) 555-5533      5773159633

-- Let's see the phone calls
SELECT caller, receiver, duration FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2024
    AND (
        caller IN ("(725) 555-4692", "(301) 555-4174", "(829) 555-5269", "(130) 555-0289", "(389) 555-5198", "(770) 555-1861", "(499) 555-9472", "(367) 555-5533")
        OR receiver IN ("(725) 555-4692", "(301) 555-4174", "(829) 555-5269", "(130) 555-0289", "(389) 555-5198", "(770) 555-1861", "(499) 555-9472", "(367) 555-5533")
    );

-- May be useful in future
-- (130) 555-0289	(996) 555-8899	51
-- (499) 555-9472	(892) 555-8872	36
-- (544) 555-8087	(389) 555-5198	595
-- (301) 555-4174	(711) 555-3007	583
-- (367) 555-5533	(375) 555-8161	45
-- (609) 555-5876	(389) 555-5198	60
-- (367) 555-5533	(344) 555-9601	120
-- (068) 555-0183	(770) 555-1861	371
-- (367) 555-5533	(022) 555-4052	241
-- (499) 555-9472	(717) 555-1342	50
-- (770) 555-1861	(725) 555-3243	49
-- (725) 555-4692	(821) 555-5262	456
-- (367) 555-5533	(704) 555-5790	75

-- Eugene
SELECT account_number, amount FROM atm_transactions
    WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- 28500762	 48
-- 28296815	 20
-- 76054385	 60
-- 49610011	 50
-- 16153065	 80 (suspicious amount)
-- 25506511	 20
-- 81061156	 30
-- 26013199	 35

SELECT person_id FROM bank_accounts WHERE account_number IN ("28500762", "28296815", "76054385", "49610011", "16153065", "25506511", "81061156", "26013199");

SELECT * FROM people WHERE id IN ("686048", "514354", "458378", "395717", "396669", "467400", "449774", "438727");

-- Kenny     (826) 555-1652     9878712108     30G67EN
-- Iman      (829) 555-5269     7049073643     L93JTIZ
-- Benista   (338) 555-6650     9586786673     8X428L0
-- Taylor    (286) 555-6063     1988161715     1106N58
-- Brooke    (122) 555-4581     4408372428     QX4YZN3
-- Luca      (389) 555-5198     8496433585     4328GD8
-- Diana     (770) 555-1861     3592750733     322W7JE
-- Bruce     (367) 555-5533     5773159633     94KL13X

-- Raymond

-- Less than a minute calls (ruth select)
-- (130) 555-0289	(996) 555-8899	51
-- (499) 555-9472	(892) 555-8872	36
-- (367) 555-5533	(375) 555-8161	45
-- (499) 555-9472	(717) 555-1342	50
-- (770) 555-1861	(725) 555-3243	49

SELECT id, origin_airport_id, destination_airport_id, hour, minute FROM flights
    WHERE year = 2024 AND month = 7 AND day = 29 GROUP BY hour LIMIT 1;
-- 36   8   4   8   20

SELECT passport_number, seat FROM passengers WHERE flight_id = 36
    AND passport_number IN ("9878712108", "7049073643", "9586786673", "1988161715", "4408372428", "8496433585", "3592750733", "5773159633");

-- 5773159633	4A -> Bruce (Both in Ruth and Eugene hint)
-- 1988161715	6D -> Taylor
-- 9878712108	7A -> Kenny
-- 8496433585	7B -> Luca (Both in Ruth and Eugene hint)

SELECT name, phone_number, license_plate FROM people WHERE name = "Bruce" OR name = "Luca";
-- Luca	(389) 555-5198	4328GD8
-- Bruce	(367) 555-5533	94KL13X

-- Luca did not make a call less than a minute but Bruce did.
-- Who Bruce called?
-- (367) 555-5533	(375) 555-8161	45 (Less than a min)
SELECT name, passport_number, license_plate FROM people WHERE phone_number = "(375) 555-8161";
-- Robin	NULL	4V16VO0
-- Bruce -> thief? & Robin -> accomplice?

SELECT seat FROM passengers WHERE flight_id = 36 AND passport_number = NULL;
-- Nothing...  Robin did not take that flight.

SELECT * FROM bank_accounts
    WHERE account_number IN ("28500762", "28296815", "76054385", "49610011", "16153065", "25506511", "81061156", "26013199");
-- 49610011	686048	2010
-- 26013199	514354	2012
-- 16153065	458378	2012
-- 28296815	395717	2014
-- 25506511	396669	2014
-- 28500762	467400	2014
-- 76054385	449774	2015
-- 81061156	438727	2018

SELECT name FROM people WHERE id IN ("686048", "514354", "458378", "395717", "396669", "467400", "449774", "438727");
-- Bruce withdraw the cash.

-- Bruce license plate
SELECT hour, minute, activity FROM bakery_security_logs WHERE license_plate = "94KL13X";
-- 8	23	entrance
-- 10	18	exit

-- Pretty sure Bruce was the duck thief and Robin was the accomplice.

SELECT destination_airport_id FROM flights WHERE id = 36;
SELECT * FROM airports WHERE id = 4;
-- 4	LGA	LaGuardia Airport	New York City
-- The flight was to New York City.
