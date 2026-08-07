/*
 * File: Amusement_Park_Ticket_System.c
 * Author: Project Team
 * Purpose: Integrates the File, User, Ride Display, Eligibility,
 *          Selection, Delete, Billing and Exit modules for the
 *          Amusement Park Ticket Generator.
 */

#define _CRT_SECURE_NO_WARNINGS//define this to avoid warnings about using unsafe functions like scanf and strcpy in Visual Studio

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "user_ride_display.h"
#include "eligibility_selection.h"
#include "delete_billing.h"
#include "file_exit.h"

 /*=========================================================
                          CONSTANTS
 =========================================================*/

#define PHONE_LENGTH 11
#define EMAIL_LENGTH 100

#define DEFAULT_RIDE_FILENAME "rides.csv"
#define HISTORY_FILENAME "booking_history.txt"
#define TICKET_FILENAME "ticket.txt"

#define HISTORY_LINE_LENGTH 512
#define HISTORY_BLOCK_LENGTH 4096

 /*=========================================================
                      CONTACT STRUCTURE
 =========================================================*/

typedef struct ContactDetails//define a structure to hold contact details
{
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
} ContactDetails;

/*=========================================================
                      INPUT FUNCTIONS
=========================================================*/

static void clearInputBuffer(void)//function to clear the input buffer to avoid issues with scanf and fgets
{
    int character;

	while ((character = getchar()) != '\n' &&//loop until the end of the line or EOF
        character != EOF)
    {
        /* Discard remaining characters. */
    }
}

static int readInteger(//function to read an integer from the user
    const char* prompt,
    int* value)
{
    if (prompt == NULL ||
		value == NULL)//check for null pointers
    {
        return 0;
    }

    printf("%s", prompt);

	if (scanf("%d", value) != 1)//if scanf fails to read an integer
    {
		clearInputBuffer();//clear the input buffer to avoid issues with subsequent input

        printf(
            "Input error: Please enter a valid whole number.\n");

        return 0;
    }

	clearInputBuffer();//clear the input buffer to avoid issues with subsequent input

    return 1;
}

static int readText(//function to read a line of text from the user
	const char* prompt,//prompt to display to the user
    char text[],
    size_t textSize)
{
	size_t length;//variable to hold the length of the input text

    if (prompt == NULL ||
        text == NULL ||
		textSize == 0)//check for null pointers and zero size
    {
        return 0;
    }

    printf("%s", prompt);

    if (fgets(
        text,
        (int)textSize,
		stdin) == NULL)//if fgets fails to read a line of text
    {
        return 0;
    }

    length = strlen(text);

    if (length > 0 &&
		text[length - 1] == '\n')//if the last character is a newline, replace it with a null terminator
    {
		text[length - 1] = '\0';//replace the newline with a null terminator
    }
    else
    {
		clearInputBuffer();//clear the input buffer to avoid issues with subsequent input
    }

    return 1;
}

static int readIntegerInRange(//function to read an integer from the user and ensure it is within a specified range
    const char* prompt,
    int minimum,
    int maximum)
{
    int value;

	while (1)//loop until a valid integer within the specified range is entered
    {
        if (readInteger(
            prompt,
			&value) == 1)//if readInteger succeeds
        {
            if (value >= minimum &&
				value <= maximum)//if the value is within the specified range
            {
                return value;
            }

            printf(
                "Input error: Enter a value between %d and %d.\n",
                minimum,
                maximum);
        }
    }
}

/*=========================================================
                  CONTACT VALIDATION
=========================================================*/

static int isValidPhoneNumber(//function to validate a phone number
    const char* phone)
{
    int index;

	if (phone == NULL ||//check for null pointer
		strlen(phone) != 10)//check if the length of the phone number is not equal to 10
    {
        return 0;
    }

    for (index = 0;
        index < 10;
		index++)//loop through each character in the phone number
    {
        if (phone[index] < '0' ||
			phone[index] > '9')//check if the character is not a digit
        {
            return 0;
        }
    }

    return 1;
}

static int isValidEmail(
	const char* email)//function to validate an email address
{
	const char* requiredEnding = "@gmail.com";//the required ending for a valid email address

	size_t emailLength;//variable to hold the length of the email address
	size_t endingLength;//variable to hold the length of the required ending
	size_t localPartLength;//variable to hold the length of the local part of the email address
	size_t index;// variable to hold the index of the current character in the email address

    if (email == NULL)
    {
        return 0;
    }

	emailLength = strlen(email);//get the length of the email address
	endingLength = strlen(requiredEnding);//get the length of the required ending

	if (emailLength <= endingLength)//check if the length of the email address is less than or equal to the length of the required ending
    {
        return 0;
    }

    if (strcmp(
        email + emailLength - endingLength,
		requiredEnding) != 0)//check if the email address does not end with the required ending
    {
        return 0;
    }

    localPartLength =
		emailLength - endingLength;//calculate the length of the local part of the email address    

    if (localPartLength == 0)
    {
        return 0;
    }

    for (index = 0;
        index < localPartLength;
		index++)//loop through each character in the local part of the email address
    {
        if (email[index] == ' ' ||
            email[index] == '\t' ||
			email[index] == '@')//check if the character is a space, tab, or '@' symbol
        {
            return 0;
        }
    }

    return 1;
}

static int getContactDetails(
	ContactDetails* contact)//function to get the contact details from the user
{
    if (contact == NULL)
    {
        return 0;
    }

    printf(
        "\n=================== CONTACT INFORMATION ===================\n");

    while (1)
    {
        if (readText(
			"Enter phone number (exactly 10 digits): ",//prompt the user to enter a phone number
            contact->phone,
			sizeof(contact->phone)) == 0)//if readText fails to read a phone number
        {
            return 0;
        }

        if (isValidPhoneNumber(
			contact->phone) == 1)//if the phone number is valid
        {
            break;
        }

        printf(
            "Phone error: The phone number must contain exactly "
            "10 numeric digits.\n");
    }

	while (1)//loop until a valid email address is entered
    {
        if (readText(
            "Enter Gmail address: ",
            contact->email,
			sizeof(contact->email)) == 0)//if readText fails to read an email address
        {
            return 0;
        }

        if (isValidEmail(
			contact->email) == 1)//if the email address is valid
        {
            break;
        }

        printf(
            "Email error: Enter a valid address ending in "
            "@gmail.com.\n");
    }

    printf(
        "Contact information was saved successfully.\n");

    return 1;
}

/*=========================================================
                   BOOKING HISTORY MODULE
=========================================================*/

static void appendHistoryLine(//function to append a line of text to a block of text, ensuring that the block does not exceed its maximum size
	char block[],//the block of text to append to
	size_t blockSize,//the maximum size of the block of text
	const char* line)//the line of text to append
{
	size_t currentLength;// variable to hold the current length of the block of text
	size_t availableLength;// variable to hold the available length of the block of text

    if (block == NULL ||
        blockSize == 0 ||
		line == NULL)//check for null pointers and zero size
    {
        return;
    }

	currentLength = strlen(block);//get the current length of the block of text

	if (currentLength >= blockSize - 1)//check if the current length of the block of text is greater than or equal to the maximum size minus 1 (to leave space for the null terminator)
    {
        return;
    }

    availableLength =
		blockSize - currentLength - 1;//calculate the available length of the block of text (maximum size minus current length minus 1 for the null terminator)

    strncat(
        block,
        line,
		availableLength);//append the line of text to the block of text, ensuring that it does not exceed the available length

	block[blockSize - 1] = '\0';//ensure that the block of text is null-terminated
}

static void displayPreviousBookings(
	const ContactDetails* contact)//function to display previous bookings for a given contact
{
	FILE* historyFile;//pointer to the booking history file

	char line[HISTORY_LINE_LENGTH];//buffer to hold a line of text from the booking history file
	char bookingBlock[HISTORY_BLOCK_LENGTH];//buffer to hold a block of text for a single booking

	char phoneMarker[50];//buffer to hold the phone marker for searching in the booking history file
	char emailMarker[150];//buffer to hold the email marker for searching in the booking history file

	int insideBooking = 0;//flag to indicate whether we are currently inside a booking block in the booking history file
	int bookingMatches = 0;//flag to indicate whether the current booking block matches the given contact
	int foundBooking = 0;//flag to indicate whether any matching bookings were found

	if (contact == NULL)//check for null pointer
    {
        return;
    }

    historyFile = fopen(
        HISTORY_FILENAME,
		"r");//open the booking history file for reading

	if (historyFile == NULL)//check if the booking history file could not be opened
    {
        printf(
            "\nNo previous booking history was found.\n");

        return;
    }

    snprintf(
        phoneMarker,
        sizeof(phoneMarker),
        "Phone: %s",
		contact->phone);//create a phone marker string to search for in the booking history file    

    snprintf(
        emailMarker,
        sizeof(emailMarker),
        "Email: %s",
		contact->email);//create an email marker string to search for in the booking history file

    bookingBlock[0] = '\0';

    printf(
        "\n================ PREVIOUS BOOKINGS =================\n");

    while (fgets(
        line,
        sizeof(line),
		historyFile) != NULL)//read each line from the booking history file
    {
        if (strncmp(
            line,
            "BOOKING_START",
			13) == 0)//check if the line indicates the start of a booking block
        {
            insideBooking = 1;
            bookingMatches = 0;
            bookingBlock[0] = '\0';

            continue;
        }

		if (insideBooking == 1)//check if we are currently inside a booking block
        {
            if (strncmp(
                line,
                "BOOKING_END",
				11) == 0)//check if the line indicates the end of a booking block
            {
				if (bookingMatches == 1)//check if the current booking block matches the given contact
                {
                    printf("%s", bookingBlock);

                    printf(
                        "----------------------------------------------------\n");

                    foundBooking = 1;
                }

                insideBooking = 0;
                bookingMatches = 0;
                bookingBlock[0] = '\0';

                continue;
            }

            appendHistoryLine(
                bookingBlock,
                sizeof(bookingBlock),
				line);//append the current line to the booking block

            if (strstr(
                line,
                phoneMarker) != NULL ||
                strstr(
                    line,
					emailMarker) != NULL)//check if the line contains the phone or email marker for the given contact
            {
                bookingMatches = 1;
            }
        }
    }

	fclose(historyFile);//close the booking history file

	if (foundBooking == 0)//check if no matching bookings were found
    {
        printf(
            "No previous rides were found for this phone "
            "number or Gmail address.\n");
    }

    printf(
        "====================================================\n");
}

static int saveBookingHistory(//function to save the booking history for a given user, contact, and selected rides
    const User* user,
    const ContactDetails* contact,
    const Ride* cart,
    int rideCount,
    const Ticket* ticket)
{
	FILE* historyFile;//pointer to the booking history file
    int index;

    if (user == NULL ||
        contact == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticket == NULL ||
		ticket->ticketId <= 0)//check for null pointers and invalid ride count or ticket ID
    {
        return 0;
    }

    historyFile = fopen(
        HISTORY_FILENAME,
		"a");//open the booking history file for appending

	if (historyFile == NULL)//check if the booking history file could not be opened
    {
        printf(
            "History error: booking_history.txt could not be opened.\n");

        return 0;
    }

    fprintf(
        historyFile,
		"BOOKING_START\n");//write the start of a booking block to the booking history file

    fprintf(
        historyFile,
        "Phone: %s\n",
		contact->phone);//write the phone number to the booking history file

    fprintf(
        historyFile,
        "Email: %s\n",
		contact->email);//write the email address to the booking history file

    fprintf(
        historyFile,
        "Visitor: %s\n",
		user->name);//write the visitor name to the booking history file

    fprintf(
        historyFile,
        "Ticket ID: %d\n",
		ticket->ticketId);//write the ticket ID to the booking history file

    fprintf(
        historyFile,
		"Selected Rides:\n");//write the header for the selected rides to the booking history file

    for (index = 0;
        index < rideCount;
		index++)//loop through each selected ride and write its details to the booking history file
    {
        fprintf(
            historyFile,
            "R%d - %s - $%.2f\n",
            cart[index].id,
            cart[index].name,
			cart[index].price);//write the ride ID, name, and price to the booking history file
    }

    fprintf(
        historyFile,
        "Subtotal: $%.2f\n",
		ticket->subtotal);//write the subtotal to the booking history file

    fprintf(
        historyFile,
        "Discount Percentage: %.0f%%\n",
		ticket->discountPercentage);// write the discount percentage to the booking history file

    fprintf(
        historyFile,
        "Discount Amount: $%.2f\n",
		ticket->discountAmount);//write the discount amount to the booking history file

    fprintf(
        historyFile,
        "Final Total: $%.2f\n",
		ticket->finalTotal);//write the final total to the booking history file

    fprintf(
        historyFile,
        "BOOKING_END\n\n");

	if (fclose(historyFile) != 0)//check if the booking history file could not be closed correctly
    {
        printf(
            "History error: booking_history.txt could not be "
            "closed correctly.\n");

        return 0;
    }

    return 1;
}

/*=========================================================
                    DISPLAY FUNCTIONS
=========================================================*/

static void displayWelcomeMessage(void)//function to display the welcome message for the amusement park ticket system
{
    printf("\n");
    printf("============================================================\n");
    printf("             WELCOME TO THE AMUSEMENT PARK\n");
    printf("============================================================\n");
    printf("Select your rides and generate your final ticket.\n");
}

static void displayMainMenu(void)//function to display the main menu for the amusement park ticket system
{
    printf("\n");
    printf("===================== MAIN MENU =============================\n");
    printf("1. Select Ride\n");
    printf("2. View Selected Rides\n");
    printf("3. Delete Ride\n");
    printf("4. Generate Ticket\n");
    printf("5. Exit\n");
    printf("============================================================\n");
}

/*=========================================================
                       USER MODULE
=========================================================*/

/**
 * @brief Registers a new user for the amusement park ticket system.
 *
 * Collects and validates the visitor's name, age, height, and weight,
 * then stores the information in the User structure.
 *
 * @param user Pointer to the User structure to be populated.
 *
 * @return 1 if the user was registered successfully.
 * @return 0 if the user pointer is NULL or registration fails.
 */
static int registerUser(
	User* user)//function to register a user by collecting their name, age, height, and weight
{
	char name[USER_NAME_LENGTH];//buffer to hold the user's name

    int age;
    int height;
    int weight;

	if (user == NULL)//check for null pointer
    {
        return 0;
    }

    printf(
        "\n==================== USER INFORMATION ======================\n");

	while (1)//loop until a valid name is entered
    {
        if (readText(
            "Enter visitor name: ",
            name,
            sizeof(name)) == 1 &&
			name[0] != '\0')//check if the name is not empty
        {
            break;
        }

        printf(
            "User error: The visitor name cannot be empty.\n");
    }

	while (1)//loop until a valid age is entered
    {
        if (readInteger(
            "Enter age: ",
            &age) == 1 &&
            age >= 1 &&
			age <= 120)//check if the age is between 1 and 120
        {
            break;
        }

        printf(
            "User error: Age must be between 1 and 120.\n");
    }

	while (1)//loop until a valid height is entered
    {
        if (readInteger(
            "Enter height in centimeters: ",
            &height) == 1 &&
            height >= 1 &&
			height <= 300)//check if the height is between 1 and 300 cm
        {
            break;
        }

        printf(
            "User error: Height must be between 1 and 300 cm.\n");
    }

	while (1)//loop until a valid weight is entered
    {
        if (readInteger(
            "Enter weight in kilograms: ",
            &weight) == 1 &&
            weight >= 1 &&
			weight <= 500)//check if the weight is between 1 and 500 kg
        {
            break;
        }

        printf(
            "User error: Weight must be between 1 and 500 kg.\n");
    }

    snprintf(
        user->name,
        sizeof(user->name),
        "%s",
		name);//copy the name to the user structure

    user->age = age;
    user->height = height;
    user->weight = weight;

    printf(
        "\nUser information was registered successfully.\n");

    return 1;
}

/*=========================================================
                   RIDE SELECTION MODULE
=========================================================*/

static int addMysteryRide(
    const Ride rides[],
    int rideCount,
    Ride** cart,
    int* cartCount,
    int* cartCapacity,
	const User* user)//function to add a mystery ride to the user's cart if they have selected 6 rides
{
    int startIndex;
    int attempt;
    int selectedIndex;

	Ride mysteryRide;//variable to hold the mystery ride to be added to the cart

    if (rides == NULL ||
        rideCount <= 0 ||
        cart == NULL ||
        cartCount == NULL ||
        cartCapacity == NULL ||
		user == NULL)//check for null pointers and invalid ride count
    {
        return 0;
    }

    /*
     * Generate a random starting index.
     */
    startIndex = rand() % rideCount;

    /*
     * Search the entire catalogue from the random starting point.
     */
    for (attempt = 0;
        attempt < rideCount;
		attempt++)//loop through the entire ride catalogue starting from the random index
    {
        selectedIndex =
            (startIndex + attempt) % rideCount;//

        /*
         * Do not award a ride already selected.
         */
        if (isDuplicateRide(
            *cart,
            *cartCount,
			rides[selectedIndex].id) == 1)//check if the selected ride is already in the user's cart
        {
            continue;
        }

        /*
         * The mystery ride must still be safe for the visitor.
         */
        if (checkEligibility(
            user,
			&rides[selectedIndex]) != 1)//check if the selected ride is eligible for the user based on their age, height, and weight
        {
            continue;
        }

		mysteryRide = rides[selectedIndex];//copy the selected ride to the mystery ride variable

        /*
         * The mystery ride is free.
         */
		mysteryRide.price = 0.00f;//    set the price of the mystery ride to 0.00

        if (addRideToCart(
            cart,
            cartCount,
            cartCapacity,
            &mysteryRide,
			user) != 1)//check if the mystery ride could not be added to the user's cart
        {
            return 0;
        }

        printf("\n");
        printf("============================================================\n");
        printf("                  MYSTERY RIDE UNLOCKED!\n");
        printf("============================================================\n");
        printf(
            "You selected 6 rides and received one free mystery ride!\n");

        printf(
            "Mystery Ride: R%d - %s\n",
            mysteryRide.id,
            mysteryRide.name);

        printf(
            "Mystery Ride Price: $0.00\n");

        printf(
            "============================================================\n");

        return 1;
    }

    printf(
        "\nNo eligible mystery ride was available.\n");

    return 0;
}
/**
 * @brief Allows the user to select rides and add them to the cart.
 *
 * Displays available rides, accepts ride selections, verifies eligibility,
 * and awards a free mystery ride after six valid selections.
 *
 * @param rides Array containing available rides.
 * @param rideCount Number of available rides.
 * @param cart Pointer to the user's ride cart.
 * @param cartCount Pointer to the number of rides currently selected.
 * @param cartCapacity Pointer to the current cart capacity.
 * @param user Pointer to the registered user.
 * @param mysteryRideAwarded Pointer to the mystery-ride status flag.
 */
static void selectRide(
    const Ride rides[],
    int rideCount,
    Ride** cart,
    int* cartCount,
    int* cartCapacity,
    const User* user,
	int* mysteryRideAwarded)//function to allow the user to select rides from the available rides and add them to their cart
{
    int rideId;
    int selectMore = 1;

    const Ride* selectedRide;

    if (rides == NULL ||
        rideCount <= 0 ||
        cart == NULL ||
        cartCount == NULL ||
        cartCapacity == NULL ||
        user == NULL ||
		mysteryRideAwarded == NULL)//check for null pointers and invalid ride count
    {
        printf(
            "Selection error: Required information is missing.\n");

        return;
    }

	while (selectMore == 1)//loop until the user chooses not to select more rides
    {
        displayRides(
            rides,
			rideCount);//display the available rides to the user

        if (readInteger(
            "\nEnter the numeric ride ID, such as 101: ",
			&rideId) == 0)//check if readInteger fails to read a ride ID
        {
            continue;
        }

        selectedRide = getRideById(
            rides,
            rideCount,
			rideId);//check if the selected ride exists in the available rides

		if (selectedRide == NULL)//check if the selected ride does not exist in the available rides
        {
            printf(
                "Selection error: Ride R%d does not exist.\n",
                rideId);
        }
        else
        {
            if (addRideToCart(
                cart,
                cartCount,
                cartCapacity,
                selectedRide,
				user) == 1)//check if the selected ride could be added to the user's cart
            {
                /*
                 * Award the mystery ride only once, immediately
                 * after the sixth selected ride.
                 */
                if (*cartCount == 6 &&
					*mysteryRideAwarded == 0)//check if the user has selected 6 rides and has not yet been awarded a mystery ride
                {
                    if (addMysteryRide(
                        rides,
                        rideCount,
                        cart,
                        cartCount,
                        cartCapacity,
						user) == 1)//check if a mystery ride could be added to the user's cart
                    {
						*mysteryRideAwarded = 1;//	set the mystery ride awarded flag to 1 to indicate that the user has been awarded a mystery ride
                    }
                }
            }
        }

        selectMore = readIntegerInRange(
            "\nWould you like to select another ride? "
            "(1 = Yes, 0 = No): ",
            0,
			1);//prompt the user to select another ride or not
    }

    printf(
        "\nRide selection completed.\n");

    displayCart(
        *cart,
		*cartCount);//display the user's selected rides in their cart
}

/*=========================================================
                       DELETE MODULE
=========================================================*/
/**
 * @brief Deletes a selected ride from the user's cart.
 *
 * Displays the current cart, asks the user for a ride ID,
 * and removes the specified ride from the cart.
 *
 * @param cart Pointer to the user's ride cart.
 * @param cartCount Pointer to the number of rides in the cart.
 */
static void deleteSelectedRide(
    Ride* cart,
	int* cartCount)//function to allow the user to delete a selected ride from their cart
{
    int rideId;

	if (cartCount == NULL)//check for null pointer
    {
        return;
    }

    if (cart == NULL ||
		*cartCount <= 0)//check if the cart is empty or the cart count is less than or equal to 0
    {
        printf(
            "Delete error: There are no rides in the cart.\n");

        return;
    }

    displayCart(
        cart,
		*cartCount);//display the user's selected rides in their cart

    if (readInteger(
        "\nEnter the numeric ride ID to delete: ",
		&rideId) == 0)//check if readInteger fails to read a ride ID
    {
        return;
    }

    deleteRideFromCart(
        cart,
        cartCount,
		rideId);//attempt to delete the selected ride from the user's cart
}

/*=========================================================
                BILLING AND FILE MODULES
=========================================================*/
/**
 * @brief Generates the user's final ticket and bill.
 *
 * Calculates the bill, displays the final total, saves the ticket
 * record, and updates the booking history.
 *
 * @param user Pointer to the registered user.
 * @param contact Pointer to the user's contact information.
 * @param cart Array containing the selected rides.
 * @param cartCount Number of rides in the cart.
 *
 * @return 1 if ticket generation is successful.
 * @return 0 if ticket generation fails.
 */
static int generateTicket(
    const User* user,
    const ContactDetails* contact,
    const Ride* cart,
	int cartCount)//    function to generate a ticket for the user's selected rides, calculate the bill, display the bill, and save the ticket record and booking history
{
    Ticket ticket = { 0 };
    int ticketId;

    if (user == NULL ||
        contact == NULL ||
        cart == NULL ||
		cartCount <= 0)//check for null pointers and invalid cart count
    {
        printf(
            "Ticket error: Select at least one ride first.\n");

        return 0;
    }

	ticketId = generateTicketId();//generate a unique ticket ID for the user's ticket

    if (calculateBill(
        cart,
        cartCount,
        ticketId,
		&ticket) != 1)//check if the bill could not be calculated for the user's selected rides
    {
        printf(
            "Ticket error: Billing could not be completed.\n");

        return 0;
    }

    displayBill(
        user,
        cart,
        cartCount,
        ticket.finalTotal,
		ticket.ticketId);//display the bill for the user's selected rides, including the final total and ticket ID

    if (saveTicketRecord(
        TICKET_FILENAME,
        user,
        cart,
        cartCount,
		&ticket) != 1)//check if the ticket record could not be saved to the ticket file
    {
        printf(
            "File error: ticket.txt could not be saved.\n");

        return 0;
    }

    if (saveBookingHistory(
        user,
        contact,
        cart,
        cartCount,
		&ticket) != 1)//check if the booking history could not be saved to the booking history file
    {
        printf(
            "History warning: The booking was not added to "
            "booking_history.txt.\n");
    }
    else
    {
        printf(
            "Booking history was updated successfully.\n");
    }

    printf(
        "Ticket automatically saved as %s.\n",
        TICKET_FILENAME);

    return 1;
}

/*=========================================================
                       EXIT MODULE
=========================================================*/
/**
 * @brief Confirms whether the user wants to exit the application.
 *
 * Prompts the user to enter Y or N and continues prompting
 * until a valid response is received.
 *
 * @return 1 if the user confirms exit.
 * @return 0 if the user chooses to continue.
 */
static int confirmExit(void)//function to confirm if the user wants to exit the program
{
    char answer[10];

	while (1)//loop until the user enters a valid response (Y/N)
    {
        if (readText(
            "Are you sure you want to exit? (Y/N): ",
            answer,
			sizeof(answer)) == 0)//check if readText fails to read a response
        {
            return 0;
        }

        if ((answer[0] == 'Y' ||
            answer[0] == 'y') &&
			answer[1] == '\0')//check if the response is 'Y' or 'y' and there are no additional characters
        {
            return 1;
        }

        if ((answer[0] == 'N' ||
            answer[0] == 'n') &&
			answer[1] == '\0')//check if the response is 'N' or 'n' and there are no additional characters
        {
            return 0;
        }

        printf(
            "Input error: Enter Y for Yes or N for No.\n");
    }
}

/*=========================================================
                           MAIN
=========================================================*/

int main(
    int argc,
	char* argv[])//main function for the amusement park ticket system, integrating all modules and handling user interaction
{
	Ride availableRides[FILE_MODULE_MAX_RIDES] = { 0 };//array to hold the available rides loaded from the ride file
	Ride* cart = NULL;//pointer to hold the user's selected rides in their cart

	User user = { 0 };//initialize a User structure to hold the user's information
	ContactDetails contact = { 0 };//initialize a ContactDetails structure to hold the user's contact information

	const char* rideFilename;//pointer to hold the name of the ride file to load

	int availableRideCount = 0;//variable to hold the count of available rides loaded from the ride file
	int cartCount = 0;//variable to hold the count of rides in the user's cart
	int cartCapacity = 0;//variable to hold the capacity of the user's cart (number of rides it can hold)
	int menuChoice;//variable to hold the user's menu choice from the main menu
	int applicationRunning = 1;//flag to indicate whether the application is running (1) or should exit (0)
	int mysteryRideAwarded = 0;//flag to indicate whether the user has been awarded a mystery ride (1) or not (0)

    /*
     * Use the first command-line argument when provided.
     * Otherwise, automatically use rides.csv.
     */
    if (argc >= 2 &&
        argv[1] != NULL &&
		argv[1][0] != '\0')//check if a command-line argument is provided for the ride file name
    {
		rideFilename = argv[1];//set the ride file name to the provided command-line argument
    }
    else
    {
		rideFilename = DEFAULT_RIDE_FILENAME;//set the ride file name to the default ride file name (rides.csv)
    }

	if (atexit(exitProgram) != 0)//register the exitProgram function to be called when the program exits
    {
        printf(
            "Exit error: The exit routine could not be registered.\n");

		return EXIT_FAILURE;//return failure if the exit routine could not be registered
    }

    printf(
        "Loading ride data from %s...\n",
        rideFilename);

    if (loadRideData(
        rideFilename,
        availableRides,
		&availableRideCount) != 1)//check if the ride data could not be loaded from the ride file
    {
        printf(
            "Application error: The ride file could not be loaded.\n");

		return EXIT_FAILURE;//return failure if the ride file could not be loaded
    }

    printf(
        "%d rides were loaded successfully.\n",
        availableRideCount);

    /*
     * Seed the random-number generator once.
     * This is used when selecting the free mystery ride.
     */
    srand(
		(unsigned int)time(NULL));//seed the random number generator with the current time to ensure different random numbers are generated each time the program runs

	displayWelcomeMessage();//display the welcome message for the amusement park ticket system

    if (registerUser(
		&user) != 1)//check if the user registration failed
    {
        printf(
            "Application error: User registration failed.\n");

		return EXIT_FAILURE;//return failure if the user registration failed
    }

    if (getContactDetails(
		&contact) != 1)//check if the contact details could not be collected from the user
    {
        printf(
            "Application error: Contact information could not "
            "be collected.\n");

		return EXIT_FAILURE;//return failure if the contact details could not be collected
    }

    displayPreviousBookings(
		&contact);//display any previous bookings for the user based on their contact information

    displayRides(
        availableRides,
		availableRideCount);//display the available rides to the user

	while (applicationRunning == 1)//loop until the user chooses to exit the application
    {
		displayMainMenu();//display the main menu for the amusement park ticket system

        menuChoice = readIntegerInRange(
            "Enter your menu choice: ",
            1,
			5);//prompt the user to enter their menu choice from the main menu (1-5)

		switch (menuChoice)//handle the user's menu choice using a switch statement
        {
        case 1:
            selectRide(
                availableRides,
                availableRideCount,
                &cart,
                &cartCount,
                &cartCapacity,
                &user,
				&mysteryRideAwarded);//allow the user to select rides from the available rides and add them to their cart
            break;

        case 2:
            displayCart(
                cart,
				cartCount);//display the user's selected rides in their cart
            break;

        case 3:
            deleteSelectedRide(
                cart,
				&cartCount);//allow the user to delete a selected ride from their cart
            break;

        case 4:
            generateTicket(
                &user,
                &contact,
                cart,
				cartCount);//generate a ticket for the user's selected rides, calculate the bill, display the bill, and save the ticket record and booking history
            break;

        case 5:
			if (confirmExit() == 1)//check if the user confirms that they want to exit the application
            {
				applicationRunning = 0;//set the application running flag to 0 to exit the application
            }
			else//if not user will return to the main menu
            {
                printf(
                    "Returning to the main menu.\n");
            }
            break;

		default://handle any invalid menu choice entered by the user
            printf(
                "Menu error: Invalid menu option.\n");
            break;
        }
    }

	cleanupSystem(//clean up any allocated resources and perform necessary cleanup tasks before exiting the application
        &cart,
		NULL);//pass NULL for the file pointer since no file needs to be closed in this case

	return EXIT_SUCCESS;//return success to indicate that the program completed successfully
}