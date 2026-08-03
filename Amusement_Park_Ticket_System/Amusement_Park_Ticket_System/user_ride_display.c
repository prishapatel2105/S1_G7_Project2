/*
 * File: user_ride_display.c
 * Author: Prisha Bhaveshkumar Patel
 * Purpose: Implements the User and Ride Display modules for the
 *          Amusement Park Ticket Generator.
 */

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
    "Thrill Ride",
    "Water Ride",
    "Thrill Ride",
    "Family Ride",
    "Kids Ride",
    "Thrill Ride",
    "Kids Ride",
    "Family Ride",
    "Water Ride",
    "Simulator",
    "Kids Ride",
    "Thrill Ride"
};

static const float RIDE_PRICES[RIDE_CATALOG_SIZE] =
{
    15.00f,
    8.00f,
    7.50f,
    12.00f,
    14.00f,
    13.50f,
    10.00f,
    6.00f,
    11.00f,
    7.00f,
    8.50f,
    13.00f,
    12.50f,
    5.00f,
    16.00f
};

static const int RIDE_MINIMUM_AGES[RIDE_CATALOG_SIZE] =
{
    12,
    5,
    7,
    10,
    10,
    14,
    8,
    3,
    10,
    5,
    3,
    10,
    8,
    3,
    14
};

static const int RIDE_MINIMUM_HEIGHTS[RIDE_CATALOG_SIZE] =
{
    140,
    100,
    110,
    120,
    130,
    150,
    120,
    90,
    130,
    100,
    90,
    130,
    120,
    80,
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

int getUserDetails(User* user)
{
    /*
     * Return failure when the supplied pointer is invalid.
     */

    if (user == NULL)
    {
        return 0;
    }

    /*
     * Valid test data is used so unit tests can run without
     * waiting for keyboard input.
     */

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

int initializeRides(Ride rides[], int maximumRides)
{
    int index;

    /*
     * The complete catalogue requires enough space for all
     * RIDE_CATALOG_SIZE rides.
     */

    if (rides == NULL)
    {
        return 0;
    }

    if (maximumRides < RIDE_CATALOG_SIZE)
    {
        return 0;
    }

    /*
     * Copy the catalogue data into the supplied Ride array.
     */

    for (index = 0; index < RIDE_CATALOG_SIZE; index++)
    {
        rides[index].id = index + 1;

        snprintf(
            rides[index].name,
            sizeof(rides[index].name),
            "%s",
            RIDE_NAMES[index]);

        rides[index].price = RIDE_PRICES[index];
        rides[index].minAge = RIDE_MINIMUM_AGES[index];
        rides[index].minHeight = RIDE_MINIMUM_HEIGHTS[index];
        rides[index].maxWeight = RIDE_MAXIMUM_WEIGHTS[index];
    }

    return RIDE_CATALOG_SIZE;
}

const char* getRideCategoryById(int rideId)
{
    /*
     * Ride IDs begin at 1 and end at RIDE_CATALOG_SIZE.
     */

    if (rideId < 1 || rideId > RIDE_CATALOG_SIZE)
    {
        return NULL;
    }

    return RIDE_CATEGORIES[rideId - 1];
}

void displayRides(const Ride rides[], int rideCount)
{
    int index;
    const char* category;

    /*
     * Do nothing when the array is invalid or contains no rides.
     */

    if (rides == NULL || rideCount <= 0)
    {
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                  AMUSEMENT PARK RIDES\n");
    printf("============================================================\n");

    for (index = 0; index < rideCount; index++)
    {
        category = getRideCategoryById(rides[index].id);

        printf("Ride ID: %d\n", rides[index].id);
        printf("Name: %s\n", rides[index].name);

        if (category != NULL)
        {
            printf("Category: %s\n", category);
        }
        else
        {
            printf("Category: Unknown\n");
        }

        printf("Price: $%.2f\n", rides[index].price);
        printf("Minimum age: %d\n", rides[index].minAge);
        printf("Minimum height: %d cm\n", rides[index].minHeight);
        printf("Maximum weight: %d kg\n", rides[index].maxWeight);
        printf("------------------------------------------------------------\n");
    }
}

const Ride* getRideById(
    const Ride rides[],
    int rideCount,
    int rideId)
{
    int index;

    if (rides == NULL || rideCount <= 0)
    {
        return NULL;
    }

    for (index = 0; index < rideCount; index++)
    {
        if (rides[index].id == rideId)
        {
            return &rides[index];
        }
    }

    return NULL;
}