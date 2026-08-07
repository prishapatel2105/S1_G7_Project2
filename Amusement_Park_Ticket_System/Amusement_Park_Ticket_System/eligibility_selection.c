/**
 * @file eligibility_selection.c
 * @author Daksh Bajaj
 * @brief Implements ride eligibility checking, ride selection,
 *        dynamic cart management, and ticket ID generation.
 */


#include "eligibility_selection.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 /*
  * Checks whether a user meets all ride restrictions.
  */
  /**
   * @brief Checks whether a user is eligible for a selected ride.
   *
   * Verifies the user's age, height, and weight against the
   * minimum age, minimum height, and maximum weight restrictions
   * of the selected ride.
   *
   * @param user Pointer to the User structure containing visitor information.
   * @param ride Pointer to the Ride structure being checked.
   *
   * @return 1 if the user meets all ride requirements.
   * @return 0 if the user is not eligible or if the input information is invalid.
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
 /**
  * @brief Checks whether a ride is already present in the cart.
  *
  * Searches the selected ride cart for a ride with the specified
  * ride ID.
  *
  * @param cart Array containing the selected rides.
  * @param rideCount Number of rides currently stored in the cart.
  * @param rideId ID of the ride to search for.
  *
  * @return 1 if the ride already exists in the cart.
  * @return 0 if the ride is not found or the input is invalid.
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
 /**
  * @brief Adds an eligible and non-duplicate ride to the cart.
  *
  * Checks the visitor's eligibility and verifies that the ride
  * has not already been selected. The function dynamically
  * allocates or expands the ride cart when additional capacity
  * is required.
  *
  * @param cart Pointer to the dynamically allocated ride cart.
  * @param rideCount Pointer to the number of rides currently selected.
  * @param capacity Pointer to the current cart capacity.
  * @param ride Pointer to the ride being added.
  * @param user Pointer to the visitor's User information.
  *
  * @return 1 if the ride is successfully added.
  * @return 0 if the ride cannot be added due to invalid input,
  *         ineligibility, duplication, or memory allocation failure.
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
 /**
  * @brief Generates a six-digit ticket ID.
  *
  * Seeds the random number generator once using the current time
  * and generates a ticket ID between 100000 and 999999.
  * The function prevents the newly generated ID from being the
  * same as the immediately previous ticket ID.
  *
  * @return A randomly generated six-digit ticket ID.
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
 /**
  * @brief Displays the rides currently selected by the user.
  *
  * Displays each selected ride's position, ride ID, name,
  * and price. If the cart is empty, an appropriate message
  * is displayed.
  *
  * @param cart Array containing the selected rides.
  * @param rideCount Number of rides currently selected.
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