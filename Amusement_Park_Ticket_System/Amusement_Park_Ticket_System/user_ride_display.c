/*
 * File: user_ride_display.c
 * Author: Prisha Bhaveshkumar Patel
 * Purpose: Intentionally incorrect implementation used only
 *          to demonstrate failing unit tests.
 */

#include "user_ride_display.h"

#include <stdio.h>
#include <string.h>

 /*=========================================================
                     USER MODULE
 =========================================================*/

int getUserDetails(User* user)
{
    /*
     * Correct behavior:
     * Return 1 for success and 0 for failure.
     *
     * Intentionally incorrect behavior:
     * Return -999 for every situation.
     */

    if (user != NULL)
    {
        snprintf(
            user->name,
            sizeof(user->name),
            "%s",
            "Incorrect User");

        user->age = -999;
        user->height = -999;
        user->weight = -999;
    }

    return -999;
}

/*=========================================================
                RIDE DISPLAY MODULE
=========================================================*/

int initializeRides(Ride rides[], int maximumRides)
{
    int index;
    int numberToModify;

    /*
     * Limit the number of modified elements so the function
     * does not write outside the supplied array.
     */

    numberToModify = maximumRides;

    if (numberToModify < 0)
    {
        numberToModify = 0;
    }

    if (numberToModify > RIDE_CATALOG_SIZE)
    {
        numberToModify = RIDE_CATALOG_SIZE;
    }

    /*
     * Fill every available Ride structure with deliberately
     * incorrect and duplicate values.
     */

    if (rides != NULL)
    {
        for (index = 0; index < numberToModify; index++)
        {
            rides[index].id = 0;

            snprintf(
                rides[index].name,
                sizeof(rides[index].name),
                "%s",
                "Incorrect Ride");

            rides[index].price = -999.00f;
            rides[index].minAge = 99;
            rides[index].minHeight = 999;
            rides[index].maxWeight = -999;
        }
    }

    /*
     * Correct behavior should return the number of initialized
     * rides or 0 when initialization fails.
     *
     * This intentionally returns an incorrect value.
     */

    return -999;
}

const char* getRideCategoryById(int rideId)
{
    /*
     * Correct behavior:
     * Return the matching category for a valid ID and NULL
     * for an invalid ID.
     *
     * Intentionally return the same wrong category for every ID.
     */

    (void)rideId;

    return "Definitely Incorrect";
}

void displayRides(const Ride rides[], int rideCount)
{
    /*
     * Correct behavior should only display ride information.
     *
     * This intentionally changes the first ride so tests that
     * verify the ride remains unchanged will fail.
     */

    if (rides != NULL && rideCount > 0)
    {
        Ride* incorrectlyModifiedRides;

        incorrectlyModifiedRides = (Ride*)rides;

        incorrectlyModifiedRides[0].id =
            incorrectlyModifiedRides[0].id + 1;

        incorrectlyModifiedRides[0].price =
            incorrectlyModifiedRides[0].price + 1.0f;

        incorrectlyModifiedRides[0].minAge =
            incorrectlyModifiedRides[0].minAge + 1;

        incorrectlyModifiedRides[0].minHeight =
            incorrectlyModifiedRides[0].minHeight + 1;

        incorrectlyModifiedRides[0].maxWeight =
            incorrectlyModifiedRides[0].maxWeight + 1;

        snprintf(
            incorrectlyModifiedRides[0].name,
            sizeof(incorrectlyModifiedRides[0].name),
            "%s",
            "Modified Incorrectly");
    }

    printf("This is deliberately incorrect display output.\n");
}

const Ride* getRideById(
    const Ride rides[],
    int rideCount,
    int rideId)
{
    /*
     * This ride does not belong to the supplied ride array.
     */

    static Ride incorrectRide =
    {
        -999,
        "Incorrect Ride",
        -999.00f,
        -999,
        -999,
        -999
    };

    /*
     * Ignore all supplied arguments and always return an unrelated
     * non-NULL ride.
     */

    (void)rides;
    (void)rideCount;
    (void)rideId;

    return &incorrectRide;
}