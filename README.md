# 🎢 Amusement Park Ticket Generator

A console-based **Amusement Park Ticket Generator** developed in **C** as a team software project.

The system allows visitors to enter their personal information, browse available amusement park rides, check ride eligibility, select and remove rides, calculate the final ticket cost, apply discounts, generate a ticket, and save booking information to files.

---

## 📌 Project Overview

The Amusement Park Ticket Generator provides a complete ride-ticket booking process through a menu-driven console application.

The system collects visitor information such as name, age, height, weight, phone number, and email address. It then displays available rides and determines whether the visitor satisfies each ride's safety requirements.

Eligible rides can be added to a dynamic booking cart. Users can review their selected rides, remove rides, generate their final bill, and save their ticket information.

---

## ✨ Features

* Visitor registration
* Phone number and Gmail validation
* 15 amusement park rides loaded from a CSV file
* Ride categories and pricing
* Age eligibility checking
* Height eligibility checking
* Weight eligibility checking
* Dynamic ride-selection cart
* Duplicate ride prevention
* View selected rides
* Delete rides from the cart
* Automatic six-digit ticket ID generation
* Automatic billing calculation
* **20% discount when 5 or more rides are selected**
* **Free Mystery Ride after selecting 6 rides**
* Mystery Ride eligibility verification
* Ticket generation
* Ticket information saved to `ticket.txt`
* Booking history saved to `booking_history.txt`
* Previous booking lookup using phone number or Gmail address
* Safe memory cleanup before program termination
* Input validation and error handling
* Unit testing for project modules

---

## 🎟️ Available Rides

The application currently contains 15 rides.

| Ride ID | Ride Name              |  Price |
| ------- | ---------------------- | -----: |
| R101    | Thunder Coaster        | $25.00 |
| R102    | Sky Ferris Wheel       | $10.00 |
| R103    | Bumper Cars            | $15.00 |
| R104    | Haunted Mansion        | $20.00 |
| R105    | Water Splash Adventure | $18.00 |
| R106    | Giant Drop Tower       | $30.00 |
| R107    | Pirate Ship            | $16.00 |
| R108    | Tea Cup Spin           |  $8.00 |
| R109    | Flying Swing           | $12.00 |
| R110    | Mini Roller Coaster    | $14.00 |
| R111    | Jungle Train           |  $9.00 |
| R112    | Log Flume              | $19.00 |
| R113    | Space Simulator        | $22.00 |
| R114    | Carousel               |  $7.00 |
| R115    | Free Fall Extreme      | $35.00 |

Each ride also contains minimum-age, minimum-height, and maximum-weight requirements where applicable.

---

## 💰 Discount System

The Billing Module automatically applies a:

### 20% Discount

when the visitor has **5 or more rides** in the final cart.

The billing system calculates:

1. Subtotal
2. Discount percentage
3. Discount amount
4. Final payable amount

---

## 🎁 Mystery Ride

Visitors who successfully select **6 rides** receive an additional **Mystery Ride for free**.

The Mystery Ride:

* Costs `$0.00`
* Is selected randomly
* Cannot duplicate an already selected ride
* Must still satisfy the visitor's eligibility requirements
* Is awarded only once during the booking session

---

## 🧩 System Modules

### 1. User Module

Responsible for collecting and validating visitor information.

**Information collected:**

* Name
* Age
* Height
* Weight

---

### 2. Ride Display Module

Responsible for managing and displaying the available ride catalogue.

Each ride contains:

* Ride ID
* Ride name
* Price
* Minimum age
* Minimum height
* Maximum weight

---

### 3. Eligibility Module

Determines whether a visitor is allowed to use a particular ride.

The system checks the visitor's:

* Age
* Height
* Weight

A ride can only be added to the cart if all required eligibility conditions are satisfied.

---

### 4. Selection Module

Handles ride selection and manages the visitor's dynamic cart.

Responsibilities include:

* Adding rides
* Preventing duplicate selections
* Dynamically managing cart memory
* Generating ticket IDs
* Displaying selected rides

---

### 5. Delete Module

Allows visitors to remove a previously selected ride.

After a ride is deleted, the remaining rides stay in their original order and the cart count is updated.

---

### 6. Billing Module

Calculates the final price of all selected rides.

Responsibilities include:

* Calculating subtotal
* Applying the 20% discount
* Calculating discount amount
* Calculating final total
* Creating ticket information
* Displaying the final billing summary

---

### 7. File Module

Handles reading and writing project data.

The module:

* Loads ride information from `rides.csv`
* Saves the generated ticket
* Stores booking information
* Maintains booking history

---

### 8. Exit Module

Handles safe program termination.

It releases dynamically allocated memory and closes opened files before the application exits.

---

## 🔄 Program Flow

```text
Start
  │
  ▼
Load Ride Data
  │
  ▼
Enter Contact Information
  │
  ▼
Check Previous Booking History
  │
  ▼
Register Visitor
  │
  ▼
Main Menu
  │
  ├── Select Ride
  │      │
  │      ▼
  │   Check Eligibility
  │      │
  │      ▼
  │   Add to Cart
  │
  ├── View Selected Rides
  │
  ├── Delete Ride
  │
  ├── Generate Ticket
  │      │
  │      ▼
  │   Calculate Bill
  │      │
  │      ▼
  │   Apply Discount
  │      │
  │      ▼
  │   Save Ticket
  │
  └── Exit
         │
         ▼
      Cleanup
```

---

## 📂 Project Structure

```text
S1_G7_Project2/
│
├── README.md
├── .gitignore
│
└── Amusement_Park_Ticket_System/
    │
    ├── Amusement_Park_Ticket_System.slnx
    │
    ├── Amusement_Park_Ticket_System/
    │   │
    │   ├── Amusement_Park_Ticket_System.c
    │   │
    │   ├── user_ride_display.c
    │   ├── user_ride_display.h
    │   │
    │   ├── eligibility_selection.c
    │   ├── eligibility_selection.h
    │   │
    │   ├── delete_billing.c
    │   ├── delete_billing.h
    │   │
    │   ├── file_exit.c
    │   ├── file_exit.h
    │   │
    │   ├── rides.csv
    │   ├── ticket.txt
    │   └── booking_history.txt
    │
    ├── UnitTest1/
    ├── UnitTest2/
    ├── UnitTest3/
    └── UnitTest4/
```

---

## 🛠️ Technologies Used

* **C**
* **C++** for Microsoft Visual Studio unit tests
* Microsoft Visual Studio
* Microsoft C/C++ Compiler
* Git
* GitHub
* CSV file handling
* Text file handling
* Dynamic memory allocation

---

## 🚀 How to Run the Project

### 1. Clone the Repository

```bash
git clone https://github.com/prishapatel2105/S1_G7_Project2.git
```

### 2. Open the Project

Navigate to:

```text
S1_G7_Project2/Amusement_Park_Ticket_System/
```

Open:

```text
Amusement_Park_Ticket_System.slnx
```

using **Microsoft Visual Studio**.

### 3. Build the Solution

In Visual Studio:

```text
Build → Build Solution
```

### 4. Run the Application

Set `Amusement_Park_Ticket_System` as the startup project and run it using:

```text
Ctrl + F5
```

The program uses:

```text
rides.csv
```

as its default ride-data file.

---

## 🖥️ Main Menu

After registration, the application provides the following menu:

```text
===================== MAIN MENU =============================
1. Select Ride
2. View Selected Rides
3. Delete Ride
4. Generate Ticket
5. Exit
============================================================
```

---

## 🧪 Testing

The project contains four Visual Studio unit-test projects:

```text
UnitTest1
UnitTest2
UnitTest3
UnitTest4
```

The tests verify functionality across the application's major modules, including:

* User information handling
* Ride initialization
* Eligibility checking
* Ride selection
* Duplicate prevention
* Ride deletion
* Billing calculations
* Discount calculations
* File operations
* Exit and cleanup functionality

---

## 👥 Team Members

| Team Member                   | Module Responsibilities               |
| ----------------------------- | ------------------------------------- |
| **Prisha Bhaveshkumar Patel** | User Module & Ride Display Module     |
| **Daksh Bajaj**               | Eligibility Module & Selection Module |
| **Naitik Visana**             | Delete Module & Billing Module        |
| **Pratyasa Roy**              | File Module & Exit Module             |

### Integration

The final application integrates all modules through:

```text
Amusement_Park_Ticket_System.c
```

---

## 📄 Generated Files

### `ticket.txt`

Stores the most recently generated ticket, including:

* Visitor information
* Ticket ID
* Selected rides
* Ride prices
* Subtotal
* Discount
* Final total

### `booking_history.txt`

Maintains previous booking records so returning visitors can retrieve earlier bookings using their phone number or Gmail address.

---

## 🎯 Project Objective

The objective of this project is to demonstrate modular software development in C while building a complete amusement park ticket-booking system.

The project demonstrates concepts including:

* Structures
* Arrays
* Pointers
* Dynamic memory allocation
* Functions
* Header files
* Modular programming
* File handling
* Input validation
* Error handling
* Unit testing
* Git and GitHub collaboration
* Software integration

---

## 📚 Repository

**S1_G7_Project2**

Developed collaboratively by **Group 7** as an academic software-development project.
