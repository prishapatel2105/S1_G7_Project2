

#include "delete_billing.h"

#include <stdio.h>
#include <string.h>

static void displayRidesBeforeDeletion(const Ride* cart, int rideCount)
{
    int index;

    printf("\nSelected Rides\n");
    printf("--------------\n");

    for (index = 0; index < rideCount; index++)
    {
        printf(
            "%d. R%d - %s - $%.2f\n",
            index + 1,
            cart[index].id,
            cart[index].name,
            cart[index].price);
    }
}


int deleteRideFromCart(Ride* cart, int* rideCount, int rideId)
{
    int deleteIndex = -1;
    int index;

    if (rideCount == NULL)
    {
        printf("Delete error: Ride count is missing.\n");
        return 0;
    }

    if (cart == NULL || *rideCount <= 0)
    {
        printf("Delete error: The selected ride list is empty.\n");
        return 0;
    }

    displayRidesBeforeDeletion(cart, *rideCount);

    for (index = 0; index < *rideCount; index++)
    {
        if (cart[index].id == rideId)
        {
            deleteIndex = index;
            break;
        }
    }

    if (deleteIndex == -1)
    {
        printf(
            "Delete error: Ride R%d was not found in the cart.\n",
            rideId);

        return 0;
    }

    
    for (index = deleteIndex;
        index < *rideCount - 1;
        index++)
    {
        cart[index] = cart[index + 1];
    }

    (*rideCount)--;

    memset(&cart[*rideCount], 0, sizeof(Ride));

    printf("Ride R%d was deleted successfully.\n", rideId);

    return 1;
}


float calculateTotal(const Ride* cart, int rideCount)
{
    float subtotal = 0.0f;
    int index;

    if (cart == NULL || rideCount <= 0)
    {
        return 0.0f;
    }

    
    for (index = 0; index < rideCount; index++)
    {
        subtotal += cart[index].price;
    }

    return subtotal;
}


int calculateBill(
    const Ride* cart,
    int rideCount,
    int ticketId,
    Ticket* ticket)
{
    if (ticket == NULL)
    {
        printf("Billing error: Ticket storage is missing.\n");
        return 0;
    }

    ticket->ticketId = 0;
    ticket->rideCount = 0;
    ticket->subtotal = 0.0f;
    ticket->discountPercentage = 0.0f;
    ticket->discountAmount = 0.0f;
    ticket->finalTotal = 0.0f;

    if (cart == NULL || rideCount <= 0)
    {
        printf("Billing error: No rides have been selected.\n");
        return 0;
    }

    if (ticketId <= 0)
    {
        printf("Billing error: Ticket ID is invalid.\n");
        return 0;
    }

    ticket->ticketId = ticketId;
    ticket->rideCount = rideCount;
    ticket->subtotal = calculateTotal(cart, rideCount);

    if (rideCount >= BILLING_DISCOUNT_MINIMUM_RIDES)
    {
        ticket->discountPercentage =
            BILLING_DISCOUNT_RATE * 100.0f;

        ticket->discountAmount =
            ticket->subtotal * BILLING_DISCOUNT_RATE;
    }

    ticket->finalTotal =
        ticket->subtotal - ticket->discountAmount;

    return 1;
}


void displayBill(
    const User* user,
    const Ride* cart,
    int rideCount,
    float totalAmount,
    int ticketId)
{
    float subtotal;
    float discountAmount = 0.0f;
    float discountPercentage = 0.0f;
    int index;

    if (user == NULL)
    {
        printf("Billing error: User information is missing.\n");
        return;
    }

    if (cart == NULL || rideCount <= 0)
    {
        printf("Billing error: No rides have been selected.\n");
        return;
    }

    subtotal = calculateTotal(cart, rideCount);

    if (rideCount >= BILLING_DISCOUNT_MINIMUM_RIDES)
    {
        discountPercentage =
            BILLING_DISCOUNT_RATE * 100.0f;

        discountAmount =
            subtotal * BILLING_DISCOUNT_RATE;
    }

    printf("\n========================================\n");
    printf("        AMUSEMENT PARK TICKET\n");
    printf("========================================\n");
    printf("Ticket ID: %d\n", ticketId);
    printf("Visitor: %s\n", user->name);
    printf("Age: %d\n", user->age);
    printf("Height: %d cm\n", user->height);
    printf("Weight: %d kg\n", user->weight);
    printf("----------------------------------------\n");
    printf("Selected Rides\n");

    for (index = 0; index < rideCount; index++)
    {
        printf(
            "R%d - %-25s $%.2f\n",
            cart[index].id,
            cart[index].name,
            cart[index].price);
    }

    printf("----------------------------------------\n");
    printf("Subtotal:              $%.2f\n", subtotal);
    printf("Discount Percentage:    %.0f%%\n", discountPercentage);
    printf("Discount Amount:        $%.2f\n", discountAmount);
    printf("Final Total:            $%.2f\n", totalAmount);
    printf("========================================\n");
}