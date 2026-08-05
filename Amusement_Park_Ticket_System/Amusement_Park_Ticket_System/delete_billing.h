/*
 * File: delete_billing.h
 * Author: Naitik Visana
 * Purpose: Declares the public functions used by the Delete and Billing
 *          Modules.
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