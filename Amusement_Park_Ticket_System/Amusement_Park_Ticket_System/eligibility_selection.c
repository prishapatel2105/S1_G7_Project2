/*
 * File: eligibility_selection.c
 * Author: Daksh Bajaj
 * Purpose: Implements ride eligibility checking, ride selection,
 *          and dynamic cart management for the
 *          Amusement Park Ticket Generator.
 */

#include "eligibility_selection.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 /*
  * Author: Daksh Bajaj
  * Inputs: A constant pointer to a User and a constant pointer to a Ride.
  * Outputs: Displays eligibility information and returns 1 when the visitor
  *          satisfies every requirement; otherwise, returns 0.
  * Purpose: Checks the visitor's age, height and weight against the selected
  *          ride's safety requirements without modifying either structure.
  */
int checkEligibility(const User* user, const Ride* ride)
{
    int isEligible = 1;
    int weightRequirementStatus = 1;

    if (user == NULL)
    {
        printf("Eligibility error: User information is missing.\n");
        return 0;
    }

    if (ride == NULL)
    {
        printf("Eligibility error: Ride information is missing.\n");
        return 0;
    }

    if (user->age <= 0 || user->height <= 0 || user->weight <= 0)
    {
        printf("Eligibility error: User information is not initialized.\n");
        return 0;
    }

    /* A minimum value of 0 means that the ride has no minimum requirement. */
    if (ride->minAge > 0 && user->age < ride->minAge)
    {
        printf(
            "Ineligible for %s: minimum age is %d.\n",
            ride->name,
            ride->minAge);

        isEligible = 0;
    }

    if (ride->minHeight > 0 && user->height < ride->minHeight)
    {
        printf(
            "Ineligible for %s: minimum height is %d cm.\n",
            ride->name,
            ride->minHeight);

        isEligible = 0;
    }

    /* A maximum weight of 0 means that the ride has no maximum weight. */
    if (ride->maxWeight > 0 && user->weight > ride->maxWeight)
    {
        printf(
            "Ineligible for %s: maximum weight is %d kg.\n",
            ride->name,
            ride->maxWeight);

        weightRequirementStatus = 0;
        isEligible = 0;
    }

    printf("Weight requirement status: %d\n", weightRequirementStatus);

    if (isEligible == 1)
    {
        printf("Eligible: You may ride %s.\n", ride->name);
    }

    printf("Eligibility status: %d\n", isEligible);

    return isEligible;
}

/*
 * Author: Daksh Bajaj
 * Inputs: Cart pointer, ride count and selected ride ID.
 * Outputs: Returns 1 when the ride ID is already present; otherwise, returns 0.
 * Purpose: Searches the selected rides and prevents duplicate additions.
 */
int isDuplicateRide(const Ride* cart, int rideCount, int rideId)
{
    int index;

    if (cart == NULL || rideCount <= 0)
    {
        return 0;
    }

    for (index = 0; index < rideCount; index++)
    {
        if (cart[index].id == rideId)
        {
            return 1;
        }
    }

    return 0;
}

/*
 * Author: Daksh Bajaj
 * Inputs: Cart pointer, ride count, cart capacity, selected ride and user.
 * Outputs: Updates the cart, count and capacity. Returns 1 when successful;
 *          otherwise, returns 0.
 * Purpose: Checks eligibility and duplicates before adding a selected ride to
 *          dynamically allocated cart memory.
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

    if (cart == NULL || rideCount == NULL || capacity == NULL ||
        ride == NULL || user == NULL)
    {
        printf("Selection error: Required information is missing.\n");
        return 0;
    }

    if (*rideCount < 0 || *capacity < 0)
    {
        printf("Selection error: Cart information is invalid.\n");
        return 0;
    }

    if (checkEligibility(user, ride) != 1)
    {
        printf(
            "Selection error: %s cannot be added because the visitor is not eligible.\n",
            ride->name);
        return 0;
    }

    if (isDuplicateRide(*cart, *rideCount, ride->id) == 1)
    {
        printf("Selection error: %s is already in the cart.\n",
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
            printf("Selection error: Cart memory could not be allocated.\n");
            return 0;
        }

        *cart = resizedCart;
        *capacity = newCapacity;
    }
    else if (*rideCount >= *capacity)
    {
        printf("Cart capacity reached. Expanding the cart.\n");

        newCapacity = (*capacity < INITIAL_CART_CAPACITY)
            ? INITIAL_CART_CAPACITY
            : (*capacity * 2);

        if (newCapacity <= *rideCount)
        {
            newCapacity = *rideCount + 1;
        }

        resizedCart = (Ride*)realloc(
            *cart,
            (size_t)newCapacity * sizeof(Ride));

        if (resizedCart == NULL)
        {
            printf("Selection error: Cart memory could not be expanded.\n");
            return 0;
        }

        *cart = resizedCart;
        *capacity = newCapacity;
    }

    (*cart)[*rideCount] = *ride;
    (*rideCount)++;

    printf("%s was added to the cart successfully.\n",
        ride->name);

    return 1;
}

/*
 * Author: Daksh Bajaj
 * Inputs: None.
 * Outputs: Returns a six-digit ticket ID between 100000 and 999999.
 * Purpose: Seeds the random-number generator once and creates a booking ID.
 */
int generateTicketId(void)
{
    static int isSeeded = 0;
    static int previousTicketId = 0;
    int ticketId;

    if (isSeeded == 0)
    {
        srand((unsigned int)time(NULL));
        isSeeded = 1;
    }

    ticketId = 100000 + (rand() % 900000);

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
 * Author: Daksh Bajaj
 * Inputs: Cart pointer and number of selected rides.
 * Outputs: Displays each selected ride and its price.
 * Purpose: Allows the visitor to review the cart before billing or deletion.
 */
void displayCart(const Ride* cart, int rideCount)
{
    int index;
    if (cart == NULL || rideCount <= 0)
    {
        printf("The cart is empty.\n");
        return;
    }

    printf("\nSelected Rides\n");
    printf("--------------\n");

    for (index = 0; index < rideCount; index++)
    {
        printf(
            "%d. %s - $%.2f\n",
            index + 1,
            cart[index].name,
            cart[index].price);
    }
}