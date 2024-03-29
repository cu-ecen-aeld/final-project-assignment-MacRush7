#!/usr/bin/python


# Please Note: This is a MySQL test script that can be run on the RPI node running Raspbian. It is not intended for

# integration purposes but demonstrates the functionalities that will be implemented in the final project integration. Thanks.

#

# Online Sources:

#

# https://pythonspot.com/mysql-with-python/

# https://www.a2hosting.com/kb/developer-corner/mysql/managing-mysql-databases-and-users-from-the-command-line


# Secondary resource:

#

# I had worked on a MySQL DB package for a project using RPi in ECEN 5783 EID. I have borrowed some important

# concepts and common practices for DB creation from there in this script. I duly credit the work done by me then and

# I duly acknowledge that it serves as a base for understanding the objectives for creating this script.

#

# For more, please visit the below link:

#

# Source: https://github.com/MacRush7/ecen5783-eid-project1-f19-rushi-poorn/blob/master/Codebase/Test_Files/mysql.py


# Steps to deal with the initial MySQL setup on the RPI node:

#

# Step: 1 - Create a MySQL server that is listening for requests for DB creation, deletion and/or modification as per need.

#

# $ sudo mysql -u root -p

#

# Step: 2 - Authenticate by entering the password created previously for MariaDB setup.

#

# Step: 3 - Enter a name for the database that you would like to create:

# > CREATE DATABASE mydatabase;

# > \q (comment - for exit)

#

# Running these steps successfully ensures that the DB is ready for use.

import MySQLdb


# Connecting to MySQL server

​db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="mydatabase")

print("MySQL Server available...\n")

print("MySQL DB available...\n")

​# Adding auto-commit so that the system updates data in the DB continuously

​db.autocommit(True)

​# Adding a cursor for the DB management

​cur = db.cursor()

​# Creating the database table

​cur.execute("CREATE TABLE mytable (Serialseq INT UNSIGNED NOT NULL AUTO_INCREMENT, Alert INT, No INT, PRIMARY KEY (Serialseq))")

​print("MySQL DB TABLE CREATED...\n")

​# Adding timestamps for the time when the database was modified

​cur.execute("ALTER TABLE `mytable` ADD `TIME_OF_MODIFICATION` TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP")

​# User test values

​for x in range(1, 10):

	alert_no = 1

	f = open("buzzerAlarm", "r")

	if f.mode == 'r':

		contents =f.read()

		if contents == 1:

			cur.execute("INSERT INTO mytable (Alert, No) VALUES (%s, %s)", (alert_no, x))

			print("MySQL TEST TABLE UPDATED...\n")

​
# Reading the last 10 table entries

​cur.execute("SELECT * FROM mytable WHERE Serialseq <= '10'")

​for row in cur.fetchall() :

    print ("Serialseq\t\t", row[0], "\n", "Alert_value:\t\t", row[1], "\n", "Alert_no:\t\t", row[2], "\n", "TIME_OF_MODIFICATION`:\t\t", row[3], "\n\n\n\n")


# Dropping table after the test ends

cur.execute("DROP TABLE mytable")

​print("MySQL TEST TABLE DELETED...\n")
