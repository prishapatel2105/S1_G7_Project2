#include "pch.h"
#include "CppUnitTest.h"

extern "C"
{
#include "../Amusement_Park_Ticket_System/delete_billing.h"
#include "../Amusement_Park_Ticket_System/delete_billing.c"
}

#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest3
{
    TEST_CLASS(UnitTest3)
    {
    private:

        /*
         * Creates a Ride structure containing valid test data.
         */
        static Ride createTestRide(int rideId, float ridePrice)
        {
            Ride ride = {};

            ride.id = rideId;
            strcpy_s(
                ride.name,
                sizeof(ride.name),
                "Test Ride");

            ride.minAge = 0;
            ride.minHeight = 0;
            ride.maxWeight = 0;
            ride.price = ridePrice;

            return ride;
        }

    public:

        /*
         * Test Case 1
         * Deletes the first ride and checks whether the remaining
         * rides are shifted to the left.
         */
        TEST_METHOD(TC01_DeleteFirstRide_ShiftsRemainingRides)
        {
            Ride cart[3] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 15.00f),
                createTestRide(3, 20.00f)
            };

            int rideCount = 3;

            int result =
                deleteRideFromCart(cart, &rideCount, 1);

            Assert::AreEqual(1, result);
            Assert::AreEqual(2, rideCount);
            Assert::AreEqual(2, cart[0].id);
            Assert::AreEqual(3, cart[1].id);
            Assert::AreEqual(0, cart[2].id);
            Assert::AreEqual(0.0f, cart[2].price, 0.001f);
        }

        /*
         * Test Case 2
         * Deletes a ride from the middle of the cart.
         */
        TEST_METHOD(TC02_DeleteMiddleRide_ShiftsLaterRide)
        {
            Ride cart[3] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 15.00f),
                createTestRide(3, 20.00f)
            };

            int rideCount = 3;

            int result =
                deleteRideFromCart(cart, &rideCount, 2);

            Assert::AreEqual(1, result);
            Assert::AreEqual(2, rideCount);
            Assert::AreEqual(1, cart[0].id);
            Assert::AreEqual(3, cart[1].id);
            Assert::AreEqual(0, cart[2].id);
        }

        /*
         * Test Case 3
         * Deletes the last ride from the cart.
         */
        TEST_METHOD(TC03_DeleteLastRide_KeepsEarlierRides)
        {
            Ride cart[3] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 15.00f),
                createTestRide(3, 20.00f)
            };

            int rideCount = 3;

            int result =
                deleteRideFromCart(cart, &rideCount, 3);

            Assert::AreEqual(1, result);
            Assert::AreEqual(2, rideCount);
            Assert::AreEqual(1, cart[0].id);
            Assert::AreEqual(2, cart[1].id);
            Assert::AreEqual(0, cart[2].id);
        }

        /*
         * Test Case 4
         * Deletes the only ride in the cart.
         */
        TEST_METHOD(TC04_DeleteOnlyRide_MakesCartEmpty)
        {
            Ride cart[1] =
            {
                createTestRide(1, 12.50f)
            };

            int rideCount = 1;

            int result =
                deleteRideFromCart(cart, &rideCount, 1);

            Assert::AreEqual(1, result);
            Assert::AreEqual(0, rideCount);
            Assert::AreEqual(0, cart[0].id);
            Assert::AreEqual(0.0f, cart[0].price, 0.001f);
        }

        /*
         * Test Case 5
         * Attempts to delete a ride ID that is not in the cart.
         */
        TEST_METHOD(TC05_DeleteMissingRide_ReturnsFailure)
        {
            Ride cart[3] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 15.00f),
                createTestRide(3, 20.00f)
            };

            int rideCount = 3;

            int result =
                deleteRideFromCart(cart, &rideCount, 99);

            Assert::AreEqual(0, result);
            Assert::AreEqual(3, rideCount);
            Assert::AreEqual(1, cart[0].id);
            Assert::AreEqual(2, cart[1].id);
            Assert::AreEqual(3, cart[2].id);
        }

        /*
         * Test Case 6
         * Passes a null cart pointer to the delete function.
         */
        TEST_METHOD(TC06_DeleteWithNullCart_ReturnsFailure)
        {
            int rideCount = 2;

            int result =
                deleteRideFromCart(nullptr, &rideCount, 1);

            Assert::AreEqual(0, result);
            Assert::AreEqual(2, rideCount);
        }

        /*
         * Test Case 7
         * Passes a null ride-count pointer.
         */
        TEST_METHOD(TC07_DeleteWithNullRideCount_ReturnsFailure)
        {
            Ride cart[1] =
            {
                createTestRide(1, 10.00f)
            };

            int result =
                deleteRideFromCart(cart, nullptr, 1);

            Assert::AreEqual(0, result);
            Assert::AreEqual(1, cart[0].id);
        }

        /*
         * Test Case 8
         * Calculates the total for multiple selected rides.
         */
        TEST_METHOD(TC08_CalculateTotal_MultipleRides)
        {
            Ride cart[3] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 12.50f),
                createTestRide(3, 15.00f)
            };

            float total = calculateTotal(cart, 3);

            Assert::AreEqual(37.50f, total, 0.001f);
        }

        /*
         * Test Case 9
         * Passes a null cart to calculateTotal.
         */
        TEST_METHOD(TC09_CalculateTotal_NullCartReturnsZero)
        {
            float total = calculateTotal(nullptr, 3);

            Assert::AreEqual(0.00f, total, 0.001f);
        }

        /*
         * Test Case 10
         * Passes a zero ride count to calculateTotal.
         */
        TEST_METHOD(TC10_CalculateTotal_ZeroCountReturnsZero)
        {
            Ride cart[1] =
            {
                createTestRide(1, 10.00f)
            };

            float total = calculateTotal(cart, 0);

            Assert::AreEqual(0.00f, total, 0.001f);
        }

        /*
         * Test Case 11
         * Calculates a bill for fewer than five rides.
         * No discount should be applied.
         */
        TEST_METHOD(TC11_CalculateBill_FourRidesNoDiscount)
        {
            Ride cart[4] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 15.00f),
                createTestRide(3, 20.00f),
                createTestRide(4, 5.00f)
            };

            Ticket ticket = {};

            int result =
                calculateBill(cart, 4, 1001, &ticket);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1001, ticket.ticketId);
            Assert::AreEqual(4, ticket.rideCount);
            Assert::AreEqual(50.00f, ticket.subtotal, 0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountPercentage,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountAmount,
                0.001f);
            Assert::AreEqual(
                50.00f,
                ticket.finalTotal,
                0.001f);
        }

        /*
         * Test Case 12
         * Calculates a bill for exactly five rides.
         * A 20 percent discount should be applied.
         */
        TEST_METHOD(TC12_CalculateBill_ExactlyFiveRidesAppliesDiscount)
        {
            Ride cart[5] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 10.00f),
                createTestRide(3, 10.00f),
                createTestRide(4, 10.00f),
                createTestRide(5, 10.00f)
            };

            Ticket ticket = {};

            int result =
                calculateBill(cart, 5, 1002, &ticket);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1002, ticket.ticketId);
            Assert::AreEqual(5, ticket.rideCount);
            Assert::AreEqual(50.00f, ticket.subtotal, 0.001f);
            Assert::AreEqual(
                20.00f,
                ticket.discountPercentage,
                0.001f);
            Assert::AreEqual(
                10.00f,
                ticket.discountAmount,
                0.001f);
            Assert::AreEqual(
                40.00f,
                ticket.finalTotal,
                0.001f);
        }

        /*
         * Test Case 13
         * Calculates a bill for more than five rides.
         * A 20 percent discount should still be applied.
         */
        TEST_METHOD(TC13_CalculateBill_SixRidesAppliesDiscount)
        {
            Ride cart[6] =
            {
                createTestRide(1, 10.00f),
                createTestRide(2, 20.00f),
                createTestRide(3, 30.00f),
                createTestRide(4, 40.00f),
                createTestRide(5, 50.00f),
                createTestRide(6, 60.00f)
            };

            Ticket ticket = {};

            int result =
                calculateBill(cart, 6, 1003, &ticket);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1003, ticket.ticketId);
            Assert::AreEqual(6, ticket.rideCount);
            Assert::AreEqual(210.00f, ticket.subtotal, 0.001f);
            Assert::AreEqual(
                20.00f,
                ticket.discountPercentage,
                0.001f);
            Assert::AreEqual(
                42.00f,
                ticket.discountAmount,
                0.001f);
            Assert::AreEqual(
                168.00f,
                ticket.finalTotal,
                0.001f);
        }

        /*
         * Test Case 14
         * Passes an invalid ticket ID.
         * The function should fail and reset the Ticket structure.
         */
        TEST_METHOD(TC14_CalculateBill_InvalidTicketIdReturnsFailure)
        {
            Ride cart[1] =
            {
                createTestRide(1, 10.00f)
            };

            Ticket ticket =
            {
                999,
                5,
                100.00f,
                20.00f,
                20.00f,
                80.00f
            };

            int result =
                calculateBill(cart, 1, 0, &ticket);

            Assert::AreEqual(0, result);
            Assert::AreEqual(0, ticket.ticketId);
            Assert::AreEqual(0, ticket.rideCount);
            Assert::AreEqual(
                0.00f,
                ticket.subtotal,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountPercentage,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountAmount,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.finalTotal,
                0.001f);
        }

        /*
         * Test Case 15
         * Attempts billing without any selected rides.
         */
        TEST_METHOD(TC15_CalculateBill_NoRidesReturnsFailure)
        {
            Ticket ticket =
            {
                999,
                5,
                100.00f,
                20.00f,
                20.00f,
                80.00f
            };

            int result =
                calculateBill(nullptr, 0, 1004, &ticket);

            Assert::AreEqual(0, result);
            Assert::AreEqual(0, ticket.ticketId);
            Assert::AreEqual(0, ticket.rideCount);
            Assert::AreEqual(
                0.00f,
                ticket.subtotal,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountPercentage,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.discountAmount,
                0.001f);
            Assert::AreEqual(
                0.00f,
                ticket.finalTotal,
                0.001f);
        }
    };
}   