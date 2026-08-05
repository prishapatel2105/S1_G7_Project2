#include "pch.h"
#include "CppUnitTest.h"

#include <cstdlib>
#include <cstring>

extern "C"
{
#include "../Amusement_Park_Ticket_System/eligibility_selection.h"

    /*
     * Include the implementation directly so the unit-test project
     * can access the eligibility and selection functions.
     *
     * Do not separately add eligibility_selection.c to UnitTest2.
     */
#include "../Amusement_Park_Ticket_System/eligibility_selection.c"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
    TEST_CLASS(EligibilitySelectionTests)
    {
    private:

        /*
         * Creates valid User test data using named fields.
         * This avoids dependency on structure field order.
         */
        static User createTestUser(
            const char* name,
            int age,
            int height,
            int weight)
        {
            User user = {};

            strcpy_s(
                user.name,
                sizeof(user.name),
                name);

            user.age = age;
            user.height = height;
            user.weight = weight;

            return user;
        }

        /*
         * Creates valid Ride test data using named fields.
         * Ride fields:
         * id, name, price, minAge, minHeight, maxWeight.
         */
        static Ride createTestRide(
            int id,
            const char* name,
            float price,
            int minimumAge,
            int minimumHeight,
            int maximumWeight)
        {
            Ride ride = {};

            ride.id = id;

            strcpy_s(
                ride.name,
                sizeof(ride.name),
                name);

            ride.price = price;
            ride.minAge = minimumAge;
            ride.minHeight = minimumHeight;
            ride.maxWeight = maximumWeight;

            return ride;
        }

    public:

        /*
         * Test 1:
         * A valid user who meets every restriction should be eligible.
         */
        TEST_METHOD(Test_CheckEligibility_ValidUser_ReturnsOne)
        {
            User user = createTestUser(
                "Prisha",
                20,
                160,
                55);

            Ride ride = createTestRide(
                1,
                "Roller Coaster",
                20.00f,
                15,
                120,
                100);

            int result = checkEligibility(
                &user,
                &ride);

            Assert::AreEqual(1, result);
        }

        /*
         * Test 2:
         * A user below the minimum age should be ineligible.
         */
        TEST_METHOD(Test_CheckEligibility_UnderAge_ReturnsZero)
        {
            User user = createTestUser(
                "Prisha",
                10,
                150,
                40);

            Ride ride = createTestRide(
                1,
                "Thrill Ride",
                25.00f,
                15,
                120,
                100);

            int result = checkEligibility(
                &user,
                &ride);

            Assert::AreEqual(0, result);
        }

        /*
         * Test 3:
         * A user below the minimum height should be ineligible.
         */
        TEST_METHOD(Test_CheckEligibility_LowHeight_ReturnsZero)
        {
            User user = createTestUser(
                "Daksh",
                20,
                100,
                60);

            Ride ride = createTestRide(
                2,
                "Tower Ride",
                30.00f,
                12,
                140,
                100);

            int result = checkEligibility(
                &user,
                &ride);

            Assert::AreEqual(0, result);
        }

        /*
         * Test 4:
         * A user above the maximum weight should be ineligible.
         */
        TEST_METHOD(Test_CheckEligibility_OverWeight_ReturnsZero)
        {
            User user = createTestUser(
                "Pratyasa",
                22,
                170,
                120);

            Ride ride = createTestRide(
                3,
                "Flying Ride",
                35.00f,
                18,
                150,
                80);

            int result = checkEligibility(
                &user,
                &ride);

            Assert::AreEqual(0, result);
        }

        /*
         * Test 5:
         * An existing ride ID should be identified as a duplicate.
         */
        TEST_METHOD(Test_IsDuplicateRide_WhenRideExists_ReturnsOne)
        {
            Ride cart[2] =
            {
                createTestRide(
                    1,
                    "Roller Coaster",
                    20.00f,
                    12,
                    120,
                    100),

                createTestRide(
                    2,
                    "Water Ride",
                    15.00f,
                    10,
                    100,
                    90)
            };

            int result = isDuplicateRide(
                cart,
                2,
                1);

            Assert::AreEqual(1, result);
        }

        /*
         * Test 6:
         * A ride ID not in the cart should not be a duplicate.
         */
        TEST_METHOD(Test_IsDuplicateRide_WhenRideDoesNotExist_ReturnsZero)
        {
            Ride cart[2] =
            {
                createTestRide(
                    1,
                    "Roller Coaster",
                    20.00f,
                    12,
                    120,
                    100),

                createTestRide(
                    2,
                    "Water Ride",
                    15.00f,
                    10,
                    100,
                    90)
            };

            int result = isDuplicateRide(
                cart,
                2,
                5);

            Assert::AreEqual(0, result);
        }

        /*
         * Test 7:
         * An eligible ride should be added successfully.
         */
        TEST_METHOD(Test_AddRideToCart_EligibleRide_AddsSuccessfully)
        {
            Ride* cart = nullptr;
            int rideCount = 0;
            int capacity = 0;

            User user = createTestUser(
                "Prisha",
                20,
                160,
                55);

            Ride ride = createTestRide(
                1,
                "Ferris Wheel",
                15.00f,
                10,
                120,
                100);

            int result = addRideToCart(
                &cart,
                &rideCount,
                &capacity,
                &ride,
                &user);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1, rideCount);
            Assert::IsNotNull(cart);

            if (cart != nullptr)
            {
                Assert::AreEqual(1, cart[0].id);

                Assert::AreEqual(
                    0,
                    strcmp(
                        "Ferris Wheel",
                        cart[0].name));

                free(cart);
                cart = nullptr;
            }
        }

        /*
         * Test 8:
         * Adding the same ride twice should fail on the second attempt.
         */
        TEST_METHOD(Test_AddRideToCart_DuplicateRide_ReturnsZero)
        {
            Ride* cart = nullptr;
            int rideCount = 0;
            int capacity = 0;

            User user = createTestUser(
                "Prisha",
                20,
                160,
                55);

            Ride ride = createTestRide(
                1,
                "Ferris Wheel",
                15.00f,
                10,
                120,
                100);

            int firstResult = addRideToCart(
                &cart,
                &rideCount,
                &capacity,
                &ride,
                &user);

            Assert::AreEqual(1, firstResult);
            Assert::AreEqual(1, rideCount);

            int secondResult = addRideToCart(
                &cart,
                &rideCount,
                &capacity,
                &ride,
                &user);

            Assert::AreEqual(0, secondResult);
            Assert::AreEqual(1, rideCount);

            free(cart);
            cart = nullptr;
        }

        /*
         * Test 9:
         * The generated ticket ID should contain six digits.
         */
        TEST_METHOD(Test_GenerateTicketId_ReturnsSixDigitNumber)
        {
            int ticket = generateTicketId();

            Assert::IsTrue(ticket >= 100000);
            Assert::IsTrue(ticket <= 999999);
        }

        /*
         * Test 10:
         * Displaying an empty cart should return safely.
         */
        TEST_METHOD(Test_DisplayCart_EmptyCart)
        {
            Ride* cart = nullptr;

            try
            {
                displayCart(
                    cart,
                    0);

                Assert::IsTrue(true);
            }
            catch (...)
            {
                Assert::Fail(
                    L"displayCart threw an unexpected exception.");
            }
        }
    };
}