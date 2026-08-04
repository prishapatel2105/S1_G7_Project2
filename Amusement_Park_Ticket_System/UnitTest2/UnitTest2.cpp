#include "pch.h"
#include "CppUnitTest.h"

extern "C"
{
#include "../Amusement_Park_Ticket_System/eligibility_selection.h"
/*
 Include the implementation directly so the unit test project links the
 eligibility/selection functions. This avoids a separate project reference.
*/
#include "../Amusement_Park_Ticket_System/eligibility_selection.c"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
    TEST_CLASS(EligibilitySelectionTests)
    {
    public:

        /*
        Test 1:
        Valid user and ride should return eligible
        */
        TEST_METHOD(Test_CheckEligibility_ValidUser_ReturnsOne)
        {
            User user = { 1, "Prisha", 20, 160, 55 };

            Ride ride = { 1, "Roller Coaster", 15, 120, 100, 20.0 };

            int result = checkEligibility(&user, &ride);

            Assert::AreEqual(1, result);
        }


        /*
        Test 2:
        User below minimum age should fail
        */
        TEST_METHOD(Test_CheckEligibility_UnderAge_ReturnsZero)
        {
            User user = { 1, "Prisha", 10, 150, 40 };

            Ride ride = { 1, "Thrill Ride", 15, 120, 100, 25.0 };

            int result = checkEligibility(&user, &ride);

            Assert::AreEqual(0, result);
        }


        /*
        Test 3:
        User height less than required height should fail
        */
        TEST_METHOD(Test_CheckEligibility_LowHeight_ReturnsZero)
        {
            User user = { 1, "Daksh", 20, 100, 60 };

            Ride ride = { 2, "Tower Ride", 12, 140, 100, 30.0 };

            int result = checkEligibility(&user, &ride);

            Assert::AreEqual(0, result);
        }


        /*
        Test 4:
        User weight higher than maximum should fail
        */
        TEST_METHOD(Test_CheckEligibility_OverWeight_ReturnsZero)
        {
            User user = { 1, "Pratyasa", 22, 170, 120 };

            Ride ride = { 3, "Flying Ride", 18, 150, 80, 35.0 };

            int result = checkEligibility(&user, &ride);

            Assert::AreEqual(0, result);
        }


        /*
        Test 5:
        Duplicate ride should not be added
        */
        TEST_METHOD(Test_IsDuplicateRide_WhenRideExists_ReturnsOne)
        {
            Ride cart[2] =
            {
                {1, "Roller Coaster", 12, 120, 100, 20.0},
                {2, "Water Ride", 10, 100, 90, 15.0}
            };


            int result = isDuplicateRide(cart, 2, 1);

            Assert::AreEqual(1, result);
        }


        /*
        Test 6:
        Non duplicate ride should return zero
        */
        TEST_METHOD(Test_IsDuplicateRide_WhenRideDoesNotExist_ReturnsZero)
        {
            Ride cart[2] =
            {
                {1, "Roller Coaster", 12, 120, 100, 20.0},
                {2, "Water Ride", 10, 100, 90, 15.0}
            };


            int result = isDuplicateRide(cart, 2, 5);

            Assert::AreEqual(0, result);
        }


        /*
        Test 7:
        Adding eligible ride should increase cart count
        */
        TEST_METHOD(Test_AddRideToCart_EligibleRide_AddsSuccessfully)
        {
            Ride* cart = NULL;
            int rideCount = 0;
            int capacity = 0;


            User user = { 1, "Prisha", 20, 160, 55 };

            Ride ride = { 1, "Ferris Wheel", 10, 120, 100, 15.0 };


            int result = addRideToCart(
                &cart,
                &rideCount,
                &capacity,
                &ride,
                &user);


            Assert::AreEqual(1, result);
            Assert::AreEqual(1, rideCount);


            free(cart);
        }


        /*
        Test 8:
        Adding duplicate ride should fail
        */
       TEST_METHOD(Test_AddRideToCart_DuplicateRide_ReturnsZero)
       {
           Ride* cart = NULL;
           int rideCount = 0;
           int capacity = 0;

           User user = { 1, "Prisha", 20, 160, 55 };

           Ride ride = {
               1,
               "Ferris Wheel",
               10,
               120,
               100,
               15.0
           };

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
       }


        /*
        Test 9:
        Ticket ID should be six digits
        */
        TEST_METHOD(Test_GenerateTicketId_ReturnsSixDigitNumber)
        {
            int ticket = generateTicketId();


            Assert::IsTrue(ticket >= 100000);
            Assert::IsTrue(ticket <= 999999);
        }


        /*
        Test 10:
        Empty cart should not crash
        */
        TEST_METHOD(Test_DisplayCart_EmptyCart)
        {
            Ride* cart = NULL;

            displayCart(cart, 0);

            Assert::IsTrue(true);
        }

    };
}