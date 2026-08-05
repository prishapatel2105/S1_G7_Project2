/*
 * File: eligibility_selection.h
 * Author: Daksh Bajaj
 * Purpose: Declares the public functions used for ride eligibility
 *          checking and ride selection in the Amusement Park Ticket Generator.
 */

#ifndef ELIGIBILITY_SELECTION_H
#define ELIGIBILITY_SELECTION_H

#include "user_ride_display.h"

#define INITIAL_CART_CAPACITY 10

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Author: Daksh Bajaj
     * Inputs: A constant pointer to a User and a constant pointer to a Ride.
     * Outputs: Returns 1 when the user satisfies every ride requirement;
     *          otherwise, returns 0.
     * Purpose: Determines whether a visitor is eligible for a selected ride.
     */
    int checkEligibility(const User* user, const Ride* ride);

    /*
     * Author: Daksh Bajaj
     * Inputs: Cart pointer, ride count, cart capacity, selected ride and user.
     * Outputs: Returns 1 when the ride is added; otherwise, returns 0.
     * Purpose: Adds an eligible, non-duplicate ride to the dynamic cart.
     */
    int addRideToCart(
        Ride** cart,
        int* rideCount,
        int* capacity,
        const Ride* ride,
        const User* user);

    /*
     * Author: Daksh Bajaj
     * Inputs: Cart pointer, ride count and selected ride ID.
     * Outputs: Returns 1 when the ride ID is already present; otherwise, returns 0.
     * Purpose: Prevents the same ride from being added more than once.
     */
    int isDuplicateRide(const Ride* cart, int rideCount, int rideId);

    /*
     * Author: Daksh Bajaj
     * Inputs: None.
     * Outputs: Returns a six-digit ticket ID.
     * Purpose: Generates a unique ticket ID for the current booking session.
     */
    int generateTicketId(void);

    /*
     * Author: Daksh Bajaj
     * Inputs: Cart pointer and number of selected rides.
     * Outputs: Displays the selected ride names and prices.
     * Purpose: Allows the visitor to review the current booking cart.
     */
    void displayCart(const Ride* cart, int rideCount);

#ifdef __cplusplus
}
#endif

#endif /* ELIGIBILITY_SELECTION_H */