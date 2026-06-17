# Movie Rental System

A console-based Movie Rental Management System developed in C++ that enables users to browse, rent, purchase, and return movies while maintaining transaction records and user authentication.

## Features

### User Module

* User Signup and Login Authentication
* Browse Available Movies
* Rent Movies with Automatic Return Dates
* Purchase Movies
* Return Rented Movies
* Search Movies by Title
* Search Movies by Genre
* View Rented Movies
* View Purchased Movies
* View Total Charges
* Personal Transaction History

### Admin Module

* Secure Admin Login
* Add New Movies
* Delete Existing Movies
* View Complete Movie Inventory
* Monitor User Charges and Transactions

## Data Storage

The system uses file-based persistence:

```text
movies.txt                -> Movie Database
users.txt                 -> Registered Users
<username>_transactions.txt -> User Transaction History
```

## Technologies Used

* C++
* Object-Oriented Programming (OOP)
* File Handling
* STL
* String Streams
* Date & Time Utilities

## Project Structure

```text
Movie_Rental_System/
├── movie_rental.cpp
├── movies.txt
├── users.txt
└── <username>_transactions.txt
```

## Key Concepts Implemented

* Classes and Objects
* Encapsulation
* File I/O
* User Authentication
* Data Persistence
* Search and Filtering
* Transaction Management

## Sample Functionalities

* Rent a movie and automatically generate a return date.
* Purchase movies permanently.
* Track user-specific rental and purchase history.
* Calculate total charges incurred by a user.
* Manage movie inventory through an admin interface.

## Author

**Sarthak Sehgal**

Indian Institute of Technology Kanpur
