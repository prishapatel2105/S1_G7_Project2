/*
 * File: Amusement_Park_Ticket_System.c
 * Author: Project Team
 * Purpose: Integrates the File, User, Ride Display, Eligibility,
 *          Selection, Delete, Billing and Exit modules for the
 *          Amusement Park Ticket Generator.
 */

#define _CRT_SECURE_NO_WARNINGS

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

typedef struct ContactDetails
{
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
} ContactDetails;

/*=========================================================
                      INPUT FUNCTIONS
=========================================================*/

static void clearInputBuffer(void)
{
    int character;

    while ((character = getchar()) != '\n' &&
        character != EOF)
    {
        /* Discard remaining characters. */
    }
}

static int readInteger(
    const char* prompt,
    int* value)
{
    if (prompt == NULL ||
        value == NULL)
    {
        return 0;
    }

    printf("%s", prompt);

    if (scanf("%d", value) != 1)
    {
        clearInputBuffer();

        printf(
            "Input error: Please enter a valid whole number.\n");

        return 0;
    }

    clearInputBuffer();

    return 1;
}

static int readText(
    const char* prompt,
    char text[],
    size_t textSize)
{
    size_t length;

    if (prompt == NULL ||
        text == NULL ||
        textSize == 0)
    {
        return 0;
    }

    printf("%s", prompt);

    if (fgets(
        text,
        (int)textSize,
        stdin) == NULL)
    {
        return 0;
    }

    length = strlen(text);

    if (length > 0 &&
        text[length - 1] == '\n')
    {
        text[length - 1] = '\0';
    }
    else
    {
        clearInputBuffer();
    }

    return 1;
}

static int readIntegerInRange(
    const char* prompt,
    int minimum,
    int maximum)
{
    int value;

    while (1)
    {
        if (readInteger(
            prompt,
            &value) == 1)
        {
            if (value >= minimum &&
                value <= maximum)
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

static int isValidPhoneNumber(
    const char* phone)
{
    int index;

    if (phone == NULL ||
        strlen(phone) != 10)
    {
        return 0;
    }

    for (index = 0;
        index < 10;
        index++)
    {
        if (phone[index] < '0' ||
            phone[index] > '9')
        {
            return 0;
        }
    }

    return 1;
}

static int isValidEmail(
    const char* email)
{
    const char* requiredEnding = "@gmail.com";

    size_t emailLength;
    size_t endingLength;
    size_t localPartLength;
    size_t index;

    if (email == NULL)
    {
        return 0;
    }

    emailLength = strlen(email);
    endingLength = strlen(requiredEnding);

    if (emailLength <= endingLength)
    {
        return 0;
    }

    if (strcmp(
        email + emailLength - endingLength,
        requiredEnding) != 0)
    {
        return 0;
    }

    localPartLength =
        emailLength - endingLength;

    if (localPartLength == 0)
    {
        return 0;
    }

    for (index = 0;
        index < localPartLength;
        index++)
    {
        if (email[index] == ' ' ||
            email[index] == '\t' ||
            email[index] == '@')
        {
            return 0;
        }
    }

    return 1;
}

static int getContactDetails(
    ContactDetails* contact)
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
            "Enter phone number (exactly 10 digits): ",
            contact->phone,
            sizeof(contact->phone)) == 0)
        {
            return 0;
        }

        if (isValidPhoneNumber(
            contact->phone) == 1)
        {
            break;
        }

        printf(
            "Phone error: The phone number must contain exactly "
            "10 numeric digits.\n");
    }

    while (1)
    {
        if (readText(
            "Enter Gmail address: ",
            contact->email,
            sizeof(contact->email)) == 0)
        {
            return 0;
        }

        if (isValidEmail(
            contact->email) == 1)
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

static void appendHistoryLine(
    char block[],
    size_t blockSize,
    const char* line)
{
    size_t currentLength;
    size_t availableLength;

    if (block == NULL ||
        blockSize == 0 ||
        line == NULL)
    {
        return;
    }

    currentLength = strlen(block);

    if (currentLength >= blockSize - 1)
    {
        return;
    }

    availableLength =
        blockSize - currentLength - 1;

    strncat(
        block,
        line,
        availableLength);

    block[blockSize - 1] = '\0';
}

static void displayPreviousBookings(
    const ContactDetails* contact)
{
    FILE* historyFile;

    char line[HISTORY_LINE_LENGTH];
    char bookingBlock[HISTORY_BLOCK_LENGTH];

    char phoneMarker[50];
    char emailMarker[150];

    int insideBooking = 0;
    int bookingMatches = 0;
    int foundBooking = 0;

    if (contact == NULL)
    {
        return;
    }

    historyFile = fopen(
        HISTORY_FILENAME,
        "r");

    if (historyFile == NULL)
    {
        printf(
            "\nNo previous booking history was found.\n");

        return;
    }

    snprintf(
        phoneMarker,
        sizeof(phoneMarker),
        "Phone: %s",
        contact->phone);

    snprintf(
        emailMarker,
        sizeof(emailMarker),
        "Email: %s",
        contact->email);

    bookingBlock[0] = '\0';

    printf(
        "\n================ PREVIOUS BOOKINGS =================\n");

    while (fgets(
        line,
        sizeof(line),
        historyFile) != NULL)
    {
        if (strncmp(
            line,
            "BOOKING_START",
            13) == 0)
        {
            insideBooking = 1;
            bookingMatches = 0;
            bookingBlock[0] = '\0';

            continue;
        }

        if (insideBooking == 1)
        {
            if (strncmp(
                line,
                "BOOKING_END",
                11) == 0)
            {
                if (bookingMatches == 1)
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
                line);

            if (strstr(
                line,
                phoneMarker) != NULL ||
                strstr(
                    line,
                    emailMarker) != NULL)
            {
                bookingMatches = 1;
            }
        }
    }

    fclose(historyFile);

    if (foundBooking == 0)
    {
        printf(
            "No previous rides were found for this phone "
            "number or Gmail address.\n");
    }

    printf(
        "====================================================\n");
}

static int saveBookingHistory(
    const User* user,
    const ContactDetails* contact,
    const Ride* cart,
    int rideCount,
    const Ticket* ticket)
{
    FILE* historyFile;
    int index;

    if (user == NULL ||
        contact == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticket == NULL ||
        ticket->ticketId <= 0)
    {
        return 0;
    }

    historyFile = fopen(
        HISTORY_FILENAME,
        "a");

    if (historyFile == NULL)
    {
        printf(
            "History error: booking_history.txt could not be opened.\n");

        return 0;
    }

    fprintf(
        historyFile,
        "BOOKING_START\n");

    fprintf(
        historyFile,
        "Phone: %s\n",
        contact->phone);

    fprintf(
        historyFile,
        "Email: %s\n",
        contact->email);

    fprintf(
        historyFile,
        "Visitor: %s\n",
        user->name);

    fprintf(
        historyFile,
        "Ticket ID: %d\n",
        ticket->ticketId);

    fprintf(
        historyFile,
        "Selected Rides:\n");

    for (index = 0;
        index < rideCount;
        index++)
    {
        fprintf(
            historyFile,
            "R%d - %s - $%.2f\n",
            cart[index].id,
            cart[index].name,
            cart[index].price);
    }

    fprintf(
        historyFile,
        "Subtotal: $%.2f\n",
        ticket->subtotal);

    fprintf(
        historyFile,
        "Discount Percentage: %.0f%%\n",
        ticket->discountPercentage);

    fprintf(
        historyFile,
        "Discount Amount: $%.2f\n",
        ticket->discountAmount);

    fprintf(
        historyFile,
        "Final Total: $%.2f\n",
        ticket->finalTotal);

    fprintf(
        historyFile,
        "BOOKING_END\n\n");

    if (fclose(historyFile) != 0)
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

static void displayWelcomeMessage(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("             WELCOME TO THE AMUSEMENT PARK\n");
    printf("============================================================\n");
    printf("Select your rides and generate your final ticket.\n");
}

static void displayMainMenu(void)
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

static int registerUser(
    User* user)
{
    char name[USER_NAME_LENGTH];

    int age;
    int height;
    int weight;

    if (user == NULL)
    {
        return 0;
    }

    printf(
        "\n==================== USER INFORMATION ======================\n");

    while (1)
    {
        if (readText(
            "Enter visitor name: ",
            name,
            sizeof(name)) == 1 &&
            name[0] != '\0')
        {
            break;
        }

        printf(
            "User error: The visitor name cannot be empty.\n");
    }

    while (1)
    {
        if (readInteger(
            "Enter age: ",
            &age) == 1 &&
            age >= 1 &&
            age <= 120)
        {
            break;
        }

        printf(
            "User error: Age must be between 1 and 120.\n");
    }

    while (1)
    {
        if (readInteger(
            "Enter height in centimeters: ",
            &height) == 1 &&
            height >= 1 &&
            height <= 300)
        {
            break;
        }

        printf(
            "User error: Height must be between 1 and 300 cm.\n");
    }

    while (1)
    {
        if (readInteger(
            "Enter weight in kilograms: ",
            &weight) == 1 &&
            weight >= 1 &&
            weight <= 500)
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
        name);

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
    const User* user)
{
    int startIndex;
    int attempt;
    int selectedIndex;

    Ride mysteryRide;

    if (rides == NULL ||
        rideCount <= 0 ||
        cart == NULL ||
        cartCount == NULL ||
        cartCapacity == NULL ||
        user == NULL)
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
        attempt++)
    {
        selectedIndex =
            (startIndex + attempt) % rideCount;

        /*
         * Do not award a ride already selected.
         */
        if (isDuplicateRide(
            *cart,
            *cartCount,
            rides[selectedIndex].id) == 1)
        {
            continue;
        }

        /*
         * The mystery ride must still be safe for the visitor.
         */
        if (checkEligibility(
            user,
            &rides[selectedIndex]) != 1)
        {
            continue;
        }

        mysteryRide = rides[selectedIndex];

        /*
         * The mystery ride is free.
         */
        mysteryRide.price = 0.00f;

        if (addRideToCart(
            cart,
            cartCount,
            cartCapacity,
            &mysteryRide,
            user) != 1)
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

static void selectRide(
    const Ride rides[],
    int rideCount,
    Ride** cart,
    int* cartCount,
    int* cartCapacity,
    const User* user,
    int* mysteryRideAwarded)
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
        mysteryRideAwarded == NULL)
    {
        printf(
            "Selection error: Required information is missing.\n");

        return;
    }

    while (selectMore == 1)
    {
        displayRides(
            rides,
            rideCount);

        if (readInteger(
            "\nEnter the numeric ride ID, such as 101: ",
            &rideId) == 0)
        {
            continue;
        }

        selectedRide = getRideById(
            rides,
            rideCount,
            rideId);

        if (selectedRide == NULL)
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
                user) == 1)
            {
                /*
                 * Award the mystery ride only once, immediately
                 * after the sixth selected ride.
                 */
                if (*cartCount == 6 &&
                    *mysteryRideAwarded == 0)
                {
                    if (addMysteryRide(
                        rides,
                        rideCount,
                        cart,
                        cartCount,
                        cartCapacity,
                        user) == 1)
                    {
                        *mysteryRideAwarded = 1;
                    }
                }
            }
        }

        selectMore = readIntegerInRange(
            "\nWould you like to select another ride? "
            "(1 = Yes, 0 = No): ",
            0,
            1);
    }

    printf(
        "\nRide selection completed.\n");

    displayCart(
        *cart,
        *cartCount);
}

/*=========================================================
                       DELETE MODULE
=========================================================*/

static void deleteSelectedRide(
    Ride* cart,
    int* cartCount)
{
    int rideId;

    if (cartCount == NULL)
    {
        return;
    }

    if (cart == NULL ||
        *cartCount <= 0)
    {
        printf(
            "Delete error: There are no rides in the cart.\n");

        return;
    }

    displayCart(
        cart,
        *cartCount);

    if (readInteger(
        "\nEnter the numeric ride ID to delete: ",
        &rideId) == 0)
    {
        return;
    }

    deleteRideFromCart(
        cart,
        cartCount,
        rideId);
}

/*=========================================================
                BILLING AND FILE MODULES
=========================================================*/

static int generateTicket(
    const User* user,
    const ContactDetails* contact,
    const Ride* cart,
    int cartCount)
{
    Ticket ticket = { 0 };
    int ticketId;

    if (user == NULL ||
        contact == NULL ||
        cart == NULL ||
        cartCount <= 0)
    {
        printf(
            "Ticket error: Select at least one ride first.\n");

        return 0;
    }

    ticketId = generateTicketId();

    if (calculateBill(
        cart,
        cartCount,
        ticketId,
        &ticket) != 1)
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
        ticket.ticketId);

    if (saveTicketRecord(
        TICKET_FILENAME,
        user,
        cart,
        cartCount,
        &ticket) != 1)
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
        &ticket) != 1)
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

static int confirmExit(void)
{
    char answer[10];

    while (1)
    {
        if (readText(
            "Are you sure you want to exit? (Y/N): ",
            answer,
            sizeof(answer)) == 0)
        {
            return 0;
        }

        if ((answer[0] == 'Y' ||
            answer[0] == 'y') &&
            answer[1] == '\0')
        {
            return 1;
        }

        if ((answer[0] == 'N' ||
            answer[0] == 'n') &&
            answer[1] == '\0')
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
    char* argv[])
{
    Ride availableRides[FILE_MODULE_MAX_RIDES] = { 0 };
    Ride* cart = NULL;

    User user = { 0 };
    ContactDetails contact = { 0 };

    const char* rideFilename;

    int availableRideCount = 0;
    int cartCount = 0;
    int cartCapacity = 0;
    int menuChoice;
    int applicationRunning = 1;
    int mysteryRideAwarded = 0;

    /*
     * Use the first command-line argument when provided.
     * Otherwise, automatically use rides.csv.
     */
    if (argc >= 2 &&
        argv[1] != NULL &&
        argv[1][0] != '\0')
    {
        rideFilename = argv[1];
    }
    else
    {
        rideFilename = DEFAULT_RIDE_FILENAME;
    }

    if (atexit(exitProgram) != 0)
    {
        printf(
            "Exit error: The exit routine could not be registered.\n");

        return EXIT_FAILURE;
    }

    printf(
        "Loading ride data from %s...\n",
        rideFilename);

    if (loadRideData(
        rideFilename,
        availableRides,
        &availableRideCount) != 1)
    {
        printf(
            "Application error: The ride file could not be loaded.\n");

        return EXIT_FAILURE;
    }

    printf(
        "%d rides were loaded successfully.\n",
        availableRideCount);

    /*
     * Seed the random-number generator once.
     * This is used when selecting the free mystery ride.
     */
    srand(
        (unsigned int)time(NULL));

    displayWelcomeMessage();

    if (registerUser(
        &user) != 1)
    {
        printf(
            "Application error: User registration failed.\n");

        return EXIT_FAILURE;
    }

    if (getContactDetails(
        &contact) != 1)
    {
        printf(
            "Application error: Contact information could not "
            "be collected.\n");

        return EXIT_FAILURE;
    }

    displayPreviousBookings(
        &contact);

    displayRides(
        availableRides,
        availableRideCount);

    while (applicationRunning == 1)
    {
        displayMainMenu();

        menuChoice = readIntegerInRange(
            "Enter your menu choice: ",
            1,
            5);

        switch (menuChoice)
        {
        case 1:
            selectRide(
                availableRides,
                availableRideCount,
                &cart,
                &cartCount,
                &cartCapacity,
                &user,
                &mysteryRideAwarded);
            break;

        case 2:
            displayCart(
                cart,
                cartCount);
            break;

        case 3:
            deleteSelectedRide(
                cart,
                &cartCount);
            break;

        case 4:
            generateTicket(
                &user,
                &contact,
                cart,
                cartCount);
            break;

        case 5:
            if (confirmExit() == 1)
            {
                applicationRunning = 0;
            }
            else
            {
                printf(
                    "Returning to the main menu.\n");
            }
            break;

        default:
            printf(
                "Menu error: Invalid menu option.\n");
            break;
        }
    }

    cleanupSystem(
        &cart,
        NULL);

    return EXIT_SUCCESS;
}