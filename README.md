
PROBLEM STATEMENT:
==================

+ We have a gas station with three gas pumps, three smartly dressed attenders and a waiting lane that can
accomodate four cars.
+ This is a premium gas station and their clientele consists only of car owners. Being located in a busy area, space is at a premium and is available only for seven (3 + 4) cars inside the gas station.
+ A car will not enter the gas station if it is filled to the capacity with cars. Once inside, the car enters the waiting lane (a “queue”).
+ When an attender (attached to a gas pump) is free, the car at the head of the queue drives up to that gas pump and gets served. When a car’s filling is done, any attender can accept the payment, but because there is only one ATM machine, payment is accepted for one car customer at a time (note that attenders are needed for this).
+ The attenders divide their time among serving cars, accepting payment and dreaming about having their own cars whilst waiting for a car to serve or a payment to accept.
+ Constraints:
  1. The Cars invoke the following functions in order: enterStation, waitInLine, goToPump, pay, exitStation.
  2. Attenders invoke serveCar and acceptPayment.
  3. Cars cannot invoke enterStation if the gas station is at capacity.
  4. If all the three pumps are busy, a car in the wait lane cannot invoke goToPump until one of the cars being served invokes pay.
  5. The car customer has to pay before the attender can acceptPayment.
  6. The attender must acceptPayment before the car can exitStation.

SOLUTION EXPLAINED:
===================

+ So here we are using semaphores and threads.
+ Threads are used for creating CARS and ATTENDERS.
+ We ar creating two pipes.
1. For communication between Cars and Attenders to exchange information.
2. For communication between Car and ATM Attender to exchange information.
+ There are semaphores for:
1. Maximum Capacity i.e Max Capacity is not crossed
2. For Queue of waiting
3. For All Pumps -> If a pump is free its value is -1 i.e it neds to be taken and value needs to be incremented
4. For ATM
5. For payment of cars i.e whether done or not
6. For cars which have finished filling gas
+ Note; All semaphores are initialized to 0
+ We have virtualized ATM Machine to a PUMP so it has equal chance to be attended
+ We have virtualized payment to filling i.e it is for a special type of car

FLOW OF MAIN:
-------------

+ Initializing all the Semaphores
+ Creating pipe for communication
+ Creating threads of Attenders
+ Starting the Cash Counter
+ Creating threads of Cars

FLOW OF ATTENDER:
-----------------

+ Attender waits for car or special car.
+ A special car is payment acceptance call.
+ Attender get information about car using pipe.
+ If it is a normal car:
1. It sleeps for 1 second i.e fills gas.
2. It signals that the filling of car is finshed.
3. It waits for the car to leave the pump so it can do some other work.
+ If it is a special car
1. It accepts paymentand signals it is done.

FLOW OF CAR:
------------

+ Car waits until the Gas station is completely filled.
+ Once it gets a chance it enters the waiting queue of the pump.
+ Once it has a chance to enter the pump it enters the pump.
+ Now it releases the queue position so other car can come in the queue.
+ Now it signals the attender to fill petrol in the car.
+ Once a attender is alloted petrol is filled.
+ Now the car leaves the pump and proceeds to the atm to pay.
+ Now it again signals the attender to collect payment.
+ Once collected it exits the station i.e it is killed.

FLOW OF ATM:
------------

+ ATM waits until someone is ready with payment;
+ Once it gets one it gets information from pipe about it.
+ It finds a attender to accept payment.
+ It sends this information about the car to the attender.
+ Attender does his work.
+ ATM confirms the payment.
+ It signals the car payment is done.

RUNNING IT:
-----------
1. gcc -pthread GasStation.c
2. ./a.out
Note: By default cars are to the max 100000 which can be changed.

NOTE: EVERYTHING HERE IS ZERO INDEXED.
--------------------------------------
