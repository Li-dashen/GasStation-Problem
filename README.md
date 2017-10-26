
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

+ **By default a total of 10000 cars will be generated.**
