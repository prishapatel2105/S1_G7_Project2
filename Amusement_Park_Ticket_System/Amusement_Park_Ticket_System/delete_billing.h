/*
 * File: delete_billing.h
 * Author: Naitik Visana
 * Purpose: Declares the public functions used by the Delete and Billing
 *          Modules.
 */
 /**
  * @file delete_billing.h
  * @brief Ride deletion and billing functions.
  *
  * This module provides functionality for removing rides from
  * the user's cart and calculating the final ticket bill.
  */

#ifndef DELETE_BILLING_H
#define DELETE_BILLING_H

#include "user_ride_display.h"

#define BILLING_DISCOUNT_RATE 0.20f
#define BILLING_DISCOUNT_MINIMUM_RIDES 5

typedef struct Ticket
{
    int ticketId;
    int rideCount;
    float subtotal;
    float discountPercentage;
    float discountAmount;
    float finalTotal;
} Ticket;

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Author: Naitik Visana
     * Inputs: Pointer to the ride cart, pointer to the ride count, and the numeric
     *         ride ID to remove, such as 101 for R101.
     * Outputs: Updates the cart and ride count. Returns 1 when deletion succeeds;
     *          otherwise, returns 0.
     * Purpose: Removes a selected ride while preserving the order of all remaining
     *          rides.
     */
     /**
  * @brief Removes a ride from the user's cart.
  *
  * @param cart User's selected ride cart.
  * @param cartCount Pointer to the number of rides in the cart.
  * @param rideId ID of the ride to remove.
  *
  * @return 1 if the ride was successfully removed.
  * @return 0 if the ride was not found or deletion failed.
  */
    int deleteRideFromCart(Ride* cart, int* rideCount, int rideId);

    /*
     * Author: Naitik Visana
     * Inputs: Final ride cart and number of selected rides.
     * Outputs: Returns the subtotal of all selected ride prices.
     * Purpose: Calculates the cost before applying a discount.
     */
    float calculateTotal(const Ride* cart, int rideCount);

    /*
     * Author: Naitik Visana
     * Inputs: Final ride cart, ride count, ticket ID from Selection and Ticket
     *         structure pointer.
     * Outputs: Populates Ticket and returns 1 when billing succeeds; otherwise,
     *          returns 0.
     * Purpose: Calculates subtotal, discount amount and final payable total.
     */
     /**
  * @brief Calculates the bill for the selected rides.
  *
  * Calculates the subtotal, applicable discount,
  * discount amount, and final total.
  *
  * @param cart Array containing selected rides.
  * @param rideCount Number of selected rides.
  * @param ticketId Ticket identification number.
  * @param ticket Pointer to the Ticket structure to populate.
  *
  * @return 1 if the bill was calculated successfully.
  * @return 0 if billing could not be completed.
  */
    int calculateBill(
        const Ride* cart,
        int rideCount,
        int ticketId,
        Ticket* ticket);

    /*
     * Author: Naitik Visana
     * Inputs: User, final ride cart, ride count, final total and ticket ID.
     * Outputs: Displays the completed ticket and billing summary.
     * Purpose: Shows visitor information, rides, subtotal, discount and final total.
     */
    void displayBill(
        const User* user,
        const Ride* cart,
        int rideCount,
        float totalAmount,
        int ticketId);

#ifdef __cplusplus
}
#endif

#endif