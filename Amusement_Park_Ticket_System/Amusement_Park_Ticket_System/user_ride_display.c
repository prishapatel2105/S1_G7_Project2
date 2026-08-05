/*
 * File: user_ride_display.c
 * Author: Prisha Bhaveshkumar Patel
 * Purpose: Implements the User and Ride Display modules for the
 *          Amusement Park Ticket Generator.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "user_ride_display.h"

#include <stdio.h>
#include <string.h>

 /*=========================================================
                     RIDE CATALOG DATA
 =========================================================*/

static const char RIDE_NAMES[RIDE_CATALOG_SIZE][RIDE_NAME_LENGTH] =
{
    "Thunder Coaster",
    "Sky Ferris Wheel",
    "Bumper Cars",
    "Haunted Mansion",
    "Water Splash Adventure",
    "Giant Drop Tower",
    "Pirate Ship",
    "Tea Cup Spin",
    "Flying Swing",
    "Mini Roller Coaster",
    "Jungle Train",
    "Log Flume",
    "Space Simulator",
    "Carousel",
    "Free Fall Extreme"
};

static const char RIDE_CATEGORIES[RIDE_CATALOG_SIZE][30] =
{
    "Roller Coaster",
    "Family Ride",
    "Family Ride",
    "Horror Ride",
    "Water Ride",
    "Thrill Ride",
    "Family Thrill",
    "Kids Ride",
    "Family Ride",
    "Kids Coaster",
    "Scenic Ride",
    "Water Ride",
    "Simulator Ride",
    "Classic Ride",
    "Extreme Thrill"
};

static const float RIDE_PRICES[RIDE_CATALOG_SIZE] =
{
    25.00f,
    10.00f,
    15.00f,
    20.00f,
    18.00f,
    30.00f,
    16.00f,
    8.00f,
    12.00f,
    14.00f,
    9.00f,
    19.00f,
    22.00f,
    7.00f,
    35.00f
};

static const int RIDE_MINIMUM_AGES[RIDE_CATALOG_SIZE] =
{
    12,
    0,
    8,
    10,
    6,
    14,
    8,
    4,
    6,
    7,
    0,
    10,
    12,
    0,
    16
};

static const int RIDE_MINIMUM_HEIGHTS[RIDE_CATALOG_SIZE] =
{
    140,
    0,
    120,
    130,
    110,
    145,
    120,
    90,
    100,
    110,
    0,
    125,
    135,
    0,
    150
};

static const int RIDE_MAXIMUM_WEIGHTS[RIDE_CATALOG_SIZE] =
{
    120,
    150,
    130,
    120,
    120,
    110,
    130,
    100,
    120,
    100,
    150,
    120,
    130,
    100,
    110
};

/*=========================================================
                       USER MODULE
=========================================================*/

/*
 * Author: Prisha Bhaveshkumar Patel
 * Inputs: Pointer to a User structure.
 * Outputs: Populates the User structure and returns 1 when successful;
 *          otherwise, returns 0.
 * Purpose: Initializes default user information for startup and testing.
 */
int getUserDetails(User* user)
{
    if (user == NULL)
    {
        return 0;
    }

    snprintf(
        user->name,
        sizeof(user->name),
        "%s",
        "Guest User");

    user->age = 18;
    user->height = 170;
    user->weight = 70;

    return 1;
}

/*=========================================================
                    RIDE DISPLAY MODULE
=========================================================*/

/*
 * Author: Prisha Bhaveshkumar Patel
 * Inputs: Ride array and maximum array capacity.
 * Outputs: Populates the ride array and returns the number of rides;
 *          otherwise, returns 0.
 * Purpose: Initializes the built-in amusement park ride catalogue.
 */
int initializeRides(
    Ride rides[],
    int maximumRides)
{
    int index;

    if (rides == NULL)
    {
        return 0;
    }

    if (maximumRides < RIDE_CATALOG_SIZE)
    {
        return 0;
    }

    for (index = 0;
        index < RIDE_CATALOG_SIZE;
        index++)
    {
        /*
         * Ride IDs follow the project specification:
         * R101 through R115.
         */
        rides[index].id = 101 + index;

        snprintf(
            rides[index].name,
            sizeof(rides[index].name),
            "%s",
            RIDE_NAMES[index]);

        rides[index].price =
            RIDE_PRICES[index];

        rides[index].minAge =
            RIDE_MINIMUM_AGES[index];

        rides[index].minHeight =
            RIDE_MINIMUM_HEIGHTS[index];

        rides[index].maxWeight =
            RIDE_MAXIMUM_WEIGHTS[index];
    }

    return RIDE_CATALOG_SIZE;
}

/*
 * Author: Prisha Bhaveshkumar Patel
 * Inputs: Numeric ride ID.
 * Outputs: Returns the matching category or NULL when the ride ID
 *          is invalid.
 * Purpose: Retrieves the category associated with a ride.
 */
const char* getRideCategoryById(
    int rideId)
{
    int index;

    /*
     * Support project ride IDs R101 through R115.
     */
    if (rideId >= 101 &&
        rideId <= 115)
    {
        index = rideId - 101;
    }
    /*
     * Also support IDs 1 through 15 for existing unit tests.
     */
    else if (rideId >= 1 &&
        rideId <= RIDE_CATALOG_SIZE)
    {
        index = rideId - 1;
    }
    else
    {
        return NULL;
    }

    return RIDE_CATEGORIES[index];
}

/*
 * Author: Prisha Bhaveshkumar Patel
 * Inputs: Ride array and number of rides.
 * Outputs: Displays all ride information in a tabular format.
 * Purpose: Shows ride IDs, names, categories, restrictions and prices.
 */
void displayRides(
    const Ride rides[],
    int rideCount)
{
    int index;
    const char* category;

    char ageText[20];
    char heightText[20];
    char weightText[20];

    if (rides == NULL ||
        rideCount <= 0)
    {
        printf(
            "No rides are available.\n");

        return;
    }

    printf("\n");
    printf("========================================================================================================================\n");
    printf("                                           AMUSEMENT PARK RIDE LIST\n");
    printf("========================================================================================================================\n");

    printf(
        "%-9s %-28s %-18s %-13s %-16s %-16s %-12s\n",
        "Ride ID",
        "Ride Name",
        "Category",
        "Min Age",
        "Min Height",
        "Max Weight",
        "Price CAD");

    printf("------------------------------------------------------------------------------------------------------------------------\n");

    for (index = 0;
        index < rideCount;
        index++)
    {
        category = getRideCategoryById(
            rides[index].id);

        if (category == NULL)
        {
            category = "Unknown";
        }

        if (rides[index].minAge == 0)
        {
            snprintf(
                ageText,
                sizeof(ageText),
                "%s",
                "All Ages");
        }
        else
        {
            snprintf(
                ageText,
                sizeof(ageText),
                "%d",
                rides[index].minAge);
        }

        if (rides[index].minHeight == 0)
        {
            snprintf(
                heightText,
                sizeof(heightText),
                "%s",
                "None");
        }
        else
        {
            snprintf(
                heightText,
                sizeof(heightText),
                "%d cm",
                rides[index].minHeight);
        }

        if (rides[index].maxWeight == 0)
        {
            snprintf(
                weightText,
                sizeof(weightText),
                "%s",
                "None");
        }
        else
        {
            snprintf(
                weightText,
                sizeof(weightText),
                "%d kg",
                rides[index].maxWeight);
        }

        printf(
            "R%-8d %-28s %-18s %-13s %-16s %-16s $%-11.2f\n",
            rides[index].id,
            rides[index].name,
            category,
            ageText,
            heightText,
            weightText,
            rides[index].price);
    }

    printf("========================================================================================================================\n");
}

/*
 * Author: Prisha Bhaveshkumar Patel
 * Inputs: Ride array, ride count and numeric ride ID.
 * Outputs: Returns a pointer to the matching Ride or NULL.
 * Purpose: Finds a selected ride in the ride catalogue.
 */
const Ride* getRideById(
    const Ride rides[],
    int rideCount,
    int rideId)
{
    int index;

    if (rides == NULL ||
        rideCount <= 0)
    {
        return NULL;
    }

    for (index = 0;
        index < rideCount;
        index++)
    {
        if (rides[index].id == rideId)
        {
            return &rides[index];
        }
    }

    return NULL;
}