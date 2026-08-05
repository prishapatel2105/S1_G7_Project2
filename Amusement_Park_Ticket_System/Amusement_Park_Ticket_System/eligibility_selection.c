/*
 * File: eligibility_selection.c
 * Author: Daksh Bajaj
 * Purpose: Implements ride eligibility checking, ride selection,
 *          dynamic cart management and ticket ID generation.
 */

#include "eligibility_selection.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 /*
  * Checks whether a user meets all ride restrictions.
  */
int checkEligibility(
    const User* user,
    const Ride* ride)
{
    if (user == NULL)
    {
        printf(
            "Eligibility error: User information is missing.\n");

        return 0;
    }

    if (ride == NULL)
    {
        printf(
            "Eligibility error: Ride information is missing.\n");

        return 0;
    }

    if (user->age <= 0 ||
        user->height <= 0 ||
        user->weight <= 0)
    {
        printf(
            "Eligibility error: User information is invalid.\n");

        return 0;
    }

    if (ride->id <= 0 ||
        ride->name[0] == '\0' ||
        ride->price < 0.0f ||
        ride->minAge < 0 ||
        ride->minHeight < 0 ||
        ride->maxWeight < 0)
    {
        printf(
            "Eligibility error: Ride information is invalid.\n");

        return 0;
    }

    if (ride->minAge > 0 &&
        user->age < ride->minAge)
    {
        printf(
            "Ineligible for %s: minimum age is %d.\n",
            ride->name,
            ride->minAge);

        return 0;
    }

    if (ride->minHeight > 0 &&
        user->height < ride->minHeight)
    {
        printf(
            "Ineligible for %s: minimum height is %d cm.\n",
            ride->name,
            ride->minHeight);

        return 0;
    }

    if (ride->maxWeight > 0 &&
        user->weight > ride->maxWeight)
    {
        printf(
            "Ineligible for %s: maximum weight is %d kg.\n",
            ride->name,
            ride->maxWeight);

        return 0;
    }

    printf(
        "Eligible: You may ride %s.\n",
        ride->name);

    return 1;
}

/*
 * Checks whether a ride already exists in the cart.
 */
int isDuplicateRide(
    const Ride* cart,
    int rideCount,
    int rideId)
{
    int index;

    if (cart == NULL ||
        rideCount <= 0 ||
        rideId <= 0)
    {
        return 0;
    }

    for (index = 0;
        index < rideCount;
        index++)
    {
        if (cart[index].id == rideId)
        {
            return 1;
        }
    }

    return 0;
}

/*
 * Adds an eligible, non-duplicate ride to the dynamic cart.
 */
int addRideToCart(
    Ride** cart,
    int* rideCount,
    int* capacity,
    const Ride* ride,
    const User* user)
{
    Ride* resizedCart;
    int newCapacity;

    if (cart == NULL ||
        rideCount == NULL ||
        capacity == NULL ||
        ride == NULL ||
        user == NULL)
    {
        printf(
            "Selection error: Required information is missing.\n");

        return 0;
    }

    if (*rideCount < 0 ||
        *capacity < 0 ||
        *rideCount > *capacity)
    {
        printf(
            "Selection error: Cart information is invalid.\n");

        return 0;
    }

    if (checkEligibility(
        user,
        ride) != 1)
    {
        printf(
            "Selection error: %s cannot be added because "
            "the visitor is not eligible.\n",
            ride->name);

        return 0;
    }

    if (isDuplicateRide(
        *cart,
        *rideCount,
        ride->id) == 1)
    {
        printf(
            "Selection error: %s is already in the cart.\n",
            ride->name);

        return 0;
    }

    if (*cart == NULL)
    {
        newCapacity = INITIAL_CART_CAPACITY;

        resizedCart = (Ride*)malloc(
            (size_t)newCapacity * sizeof(Ride));

        if (resizedCart == NULL)
        {
            printf(
                "Selection error: Cart memory could not be allocated.\n");

            return 0;
        }

        *cart = resizedCart;
        *capacity = newCapacity;
    }
    else if (*rideCount >= *capacity)
    {
        if (*capacity < INITIAL_CART_CAPACITY)
        {
            newCapacity = INITIAL_CART_CAPACITY;
        }
        else
        {
            newCapacity = *capacity * 2;
        }

        if (newCapacity <= *rideCount)
        {
            newCapacity = *rideCount + 1;
        }

        resizedCart = (Ride*)realloc(
            *cart,
            (size_t)newCapacity * sizeof(Ride));

        if (resizedCart == NULL)
        {
            printf(
                "Selection error: Cart memory could not be expanded.\n");

            return 0;
        }

        *cart = resizedCart;
        *capacity = newCapacity;
    }

    (*cart)[*rideCount] = *ride;
    (*rideCount)++;

    printf(
        "%s was added to the cart successfully.\n",
        ride->name);

    return 1;
}

/*
 * Generates a six-digit ticket ID.
 */
int generateTicketId(void)
{
    static int isSeeded = 0;
    static int previousTicketId = 0;

    int ticketId;

    if (isSeeded == 0)
    {
        srand(
            (unsigned int)time(NULL));

        isSeeded = 1;
    }

    ticketId =
        100000 + (rand() % 900000);

    if (ticketId == previousTicketId)
    {
        ticketId++;

        if (ticketId > 999999)
        {
            ticketId = 100000;
        }
    }

    previousTicketId = ticketId;

    return ticketId;
}

/*
 * Displays the current cart.
 */
void displayCart(
    const Ride* cart,
    int rideCount)
{
    int index;

    if (cart == NULL ||
        rideCount <= 0)
    {
        printf(
            "The cart is empty.\n");

        return;
    }

    printf("\nSelected Rides\n");
    printf("--------------\n");

    for (index = 0;
        index < rideCount;
        index++)
    {
        printf(
            "%d. R%d - %s - $%.2f\n",
            index + 1,
            cart[index].id,
            cart[index].name,
            cart[index].price);
    }
}