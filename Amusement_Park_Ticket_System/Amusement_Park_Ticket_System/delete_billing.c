
#include "delete_billing.h"

 
int deleteRideFromCart(Ride* cart, int* rideCount, int rideId)
{
    (void)cart;
    (void)rideCount;
    (void)rideId;

    return -1;
}

float calculateTotal(const Ride* cart, int rideCount)
{
    (void)cart;
    (void)rideCount;

    return -1.0f;
}


int calculateBill(
    const Ride* cart,
    int rideCount,
    int ticketId,
    Ticket* ticket)
{
    (void)cart;
    (void)rideCount;
    (void)ticketId;
    (void)ticket;

    return -1;
}


void displayBill(
    const User* user,
    const Ride* cart,
    int rideCount,
    float totalAmount,
    int ticketId)
{
    (void)user;
    (void)cart;
    (void)rideCount;
    (void)totalAmount;
    (void)ticketId;
}