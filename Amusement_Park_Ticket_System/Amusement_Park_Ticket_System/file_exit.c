/*
 * File: file_exit.c
 * Author: Pratyasa Roy
 * Purpose: Implements ASCII ride-file loading, final-ticket file saving,
 *          resource cleanup and normal application exit.
 *
 * This version contains two documented seeded defects for issue testing:
 *
 * Bug 1: Ride IDs without the R prefix are rejected.
 * Bug 2: saveTicketDetails stores the wrong final total.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "file_exit.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_LINE_LENGTH 256

 /*=========================================================
                          FILE MODULE
 =========================================================*/

 /*
  * Removes leading and trailing whitespace.
  */
static void trimText(char* text)
{
    char* start;
    char* end;
    size_t length;

    if (text == NULL)
    {
        return;
    }

    start = text;

    while (*start != '\0' &&
        isspace((unsigned char)*start))
    {
        start++;
    }

    if (start != text)
    {
        memmove(
            text,
            start,
            strlen(start) + 1);
    }

    length = strlen(text);
    end = text + length;

    while (end > text &&
        isspace((unsigned char)*(end - 1)))
    {
        end--;
    }

    *end = '\0';
}

/*
 * BUG 1:
 * This parser accepts only IDs beginning with R.
 *
 * Accepted:
 * R101,Ride Name,25.00,12,140,120
 *
 * Incorrectly rejected:
 * 101,Ride Name,25.00,12,140,120
 */
 /*
  * Parses both R-prefixed and numeric ride IDs.
  */
static int parseRideLine(
    const char* line,
    Ride* ride)
{
    Ride parsedRide;
    int parsedFields;
    char extraText[2];

    if (line == NULL ||
        ride == NULL)
    {
        return 0;
    }

    memset(
        &parsedRide,
        0,
        sizeof(parsedRide));

    memset(
        extraText,
        0,
        sizeof(extraText));

    /*
     * First try an R-prefixed ID:
     * R101,Ride Name,25.00,12,140,120
     */
    parsedFields = sscanf_s(
        line,
        " R%d , %49[^,] , %f , %d , %d , %d %1s",
        &parsedRide.id,
        parsedRide.name,
        (unsigned int)sizeof(parsedRide.name),
        &parsedRide.price,
        &parsedRide.minAge,
        &parsedRide.minHeight,
        &parsedRide.maxWeight,
        extraText,
        (unsigned int)sizeof(extraText));

    /*
     * If that fails, try a numeric ID:
     * 101,Ride Name,25.00,12,140,120
     */
    if (parsedFields != 6)
    {
        memset(
            &parsedRide,
            0,
            sizeof(parsedRide));

        memset(
            extraText,
            0,
            sizeof(extraText));

        parsedFields = sscanf_s(
            line,
            " %d , %49[^,] , %f , %d , %d , %d %1s",
            &parsedRide.id,
            parsedRide.name,
            (unsigned int)sizeof(parsedRide.name),
            &parsedRide.price,
            &parsedRide.minAge,
            &parsedRide.minHeight,
            &parsedRide.maxWeight,
            extraText,
            (unsigned int)sizeof(extraText));
    }

    if (parsedFields != 6)
    {
        return 0;
    }

    trimText(parsedRide.name);

    if (parsedRide.id <= 0 ||
        parsedRide.name[0] == '\0' ||
        parsedRide.price < 0.0f ||
        parsedRide.minAge < 0 ||
        parsedRide.minHeight < 0 ||
        parsedRide.maxWeight < 0)
    {
        return 0;
    }

    *ride = parsedRide;

    return 1;
}

/*
 * Loads ride records from a CSV file.
 */
int loadRideData(
    const char* filename,
    Ride rides[],
    int* rideCount)
{
    FILE* inputFile = NULL;
    char line[FILE_LINE_LENGTH];
    char* contentStart;
    int loadedCount = 0;
    errno_t openResult;

    if (rideCount != NULL)
    {
        *rideCount = 0;
    }

    if (filename == NULL ||
        filename[0] == '\0' ||
        rides == NULL ||
        rideCount == NULL)
    {
        printf(
            "File error: Invalid ride-file information.\n");

        return 0;
    }

    openResult = fopen_s(
        &inputFile,
        filename,
        "r");

    if (openResult != 0 ||
        inputFile == NULL)
    {
        printf(
            "File error: Could not open %s.\n",
            filename);

        return 0;
    }

    while (fgets(
        line,
        sizeof(line),
        inputFile) != NULL)
    {
        contentStart = line;

        while (*contentStart != '\0' &&
            isspace((unsigned char)*contentStart))
        {
            contentStart++;
        }

        /*
         * Ignore blank lines and comments.
         */
        if (*contentStart == '\0' ||
            *contentStart == '#')
        {
            continue;
        }

        /*
         * Ignore the CSV header.
         */
        if (strncmp(
            contentStart,
            "RideID",
            6) == 0)
        {
            continue;
        }

        if (loadedCount >= FILE_MODULE_MAX_RIDES)
        {
            printf(
                "File error: The ride file contains too many records.\n");

            fclose(inputFile);
            *rideCount = 0;

            return 0;
        }

        if (parseRideLine(
            contentStart,
            &rides[loadedCount]) == 0)
        {
            printf(
                "File error: Invalid ride record was found.\n");

            fclose(inputFile);
            *rideCount = 0;

            return 0;
        }

        loadedCount++;
    }

    if (ferror(inputFile))
    {
        printf(
            "File error: The ride file could not be read correctly.\n");

        fclose(inputFile);
        *rideCount = 0;

        return 0;
    }

    if (fclose(inputFile) != 0)
    {
        printf(
            "File error: The ride file could not be closed correctly.\n");

        *rideCount = 0;

        return 0;
    }

    if (loadedCount == 0)
    {
        printf(
            "File error: The file contains no valid ride records.\n");

        return 0;
    }

    *rideCount = loadedCount;

    return 1;
}

/*
 * Saves a complete ticket record.
 */
int saveTicketRecord(
    const char* filename,
    const User* user,
    const Ride* cart,
    int rideCount,
    const Ticket* ticket)
{
    FILE* outputFile = NULL;
    time_t currentTime;
    struct tm localTimeValue;
    char dateTimeText[40] = "Unavailable";
    int index;
    errno_t openResult;

    if (filename == NULL ||
        filename[0] == '\0' ||
        user == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticket == NULL ||
        ticket->ticketId <= 0)
    {
        printf(
            "File error: Invalid ticket information.\n");

        return 0;
    }

    /*
     * Open using append mode so previous tickets remain saved.
     */
    openResult = fopen_s(
        &outputFile,
        filename,
        "a");

    if (openResult != 0 ||
        outputFile == NULL)
    {
        printf(
            "File error: Could not open %s.\n",
            filename);

        return 0;
    }

    currentTime = time(NULL);

    if (localtime_s(
        &localTimeValue,
        &currentTime) == 0)
    {
        strftime(
            dateTimeText,
            sizeof(dateTimeText),
            "%Y-%m-%d %H:%M:%S",
            &localTimeValue);
    }

    /*
     * Separate each saved ticket clearly.
     */
    fprintf(
        outputFile,
        "\n============================================================\n");

    fprintf(
        outputFile,
        "AMUSEMENT PARK TICKET\n");

    fprintf(
        outputFile,
        "Generated: %s\n",
        dateTimeText);

    fprintf(
        outputFile,
        "Ticket ID: %d\n",
        ticket->ticketId);

    fprintf(
        outputFile,
        "Visitor Name: %s\n",
        user->name);

    fprintf(
        outputFile,
        "Age: %d\n",
        user->age);

    fprintf(
        outputFile,
        "Height: %d cm\n",
        user->height);

    fprintf(
        outputFile,
        "Weight: %d kg\n",
        user->weight);

    fprintf(
        outputFile,
        "\nSelected Rides\n");

    for (index = 0;
        index < rideCount;
        index++)
    {
        fprintf(
            outputFile,
            "R%d,%s,$%.2f\n",
            cart[index].id,
            cart[index].name,
            cart[index].price);
    }

    fprintf(
        outputFile,
        "\nSubtotal: $%.2f\n",
        ticket->subtotal);

    fprintf(
        outputFile,
        "Discount Percentage: %.0f%%\n",
        ticket->discountPercentage);

    fprintf(
        outputFile,
        "Discount Amount: $%.2f\n",
        ticket->discountAmount);

    fprintf(
        outputFile,
        "Final Total: $%.2f\n",
        ticket->finalTotal);

    fprintf(
        outputFile,
        "============================================================\n");

    if (ferror(outputFile))
    {
        printf(
            "File error: Ticket information could not be written.\n");

        fclose(outputFile);

        return 0;
    }

    if (fclose(outputFile) != 0)
    {
        printf(
            "File error: Ticket file could not be closed correctly.\n");

        return 0;
    }

    printf(
        "Ticket was added successfully to %s.\n",
        filename);

    return 1;
}
/*
 * Creates and saves a basic ticket record.
 */
int saveTicketDetails(
    const char* filename,
    const User* user,
    const Ride* cart,
    int rideCount,
    int ticketId,
    float totalAmount)
{
    Ticket ticket;

    if (filename == NULL ||
        filename[0] == '\0' ||
        user == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticketId <= 0 ||
        totalAmount < 0.0f)
    {
        printf(
            "File error: Invalid basic ticket information.\n");

        return 0;
    }

    memset(
        &ticket,
        0,
        sizeof(ticket));

    ticket.ticketId = ticketId;
    ticket.rideCount = rideCount;
    ticket.subtotal = totalAmount;
    ticket.discountPercentage = 0.0f;
    ticket.discountAmount = 0.0f;

    /*
     * Correct assignment.
     */
    ticket.finalTotal = totalAmount;

    return saveTicketRecord(
        filename,
        user,
        cart,
        rideCount,
        &ticket);
}

/*=========================================================
                         EXIT MODULE
=========================================================*/

/*
 * Releases allocated memory and closes an open file.
 */
void cleanupSystem(
    Ride** cart,
    FILE* file)
{
    if (cart != NULL &&
        *cart != NULL)
    {
        free(*cart);
        *cart = NULL;
    }

    if (file != NULL)
    {
        if (fclose(file) != 0)
        {
            printf(
                "Exit warning: A file could not be closed correctly.\n");
        }
    }
}

/*
 * Displays the normal application-exit message.
 */
void exitProgram(void)
{
    printf(
        "Thank you for using the Amusement Park Ticket Generator.\n");

    printf(
        "The application has exited successfully.\n");
}