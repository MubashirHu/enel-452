Elevator control system

- based on Selective collective operation (Reference : http://www.electrical-knowhow.com/2012/04/elevator-control-system.html).
- Most common, remembers and answers calls in one direction then reverses. When trip complete, programmed to return to a home landing.
- 
This Elevator-Control-System consists of:

1. Up and Down Buttons at each Floor as well as a current floor indicator.
2. Inside the elevator
  - Each floor is listed
  - Opening door message sequence for passenger entry
  - Closing door message sequence for passenger exit
  - Emergency button to enter maintenance mode

Command-line-interface:
  - Passengers can make requests from any floor (which will be simulated through commands such as '1u','2u', '2d'...etc)
  - Passengers will select the target floor when inside of the elevator through command line ('1','2','3'...until 8)

 A status window that would inlude:
 - current floor
 - target floor
 - Status of the elevator (whether it is at HOME position, ARRIVED, NOT ARRIVED..etc)
 - Elevator directior (important as in Selective colletive operation, requests of the elevators current direction are prioritized)
 - Whether passengers are in the elevator car
 - Whether elevator is in maintenance mode or not
