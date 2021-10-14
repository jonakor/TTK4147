# EX 7

## Task A
What happens?  
The local variable is correct, but the global is wrong.

## Task B
What was the problem?  
The problem is data race

## Task C
Which one uses more system time, and why?  
Without: 0 sec  
With: 53.440 sec  
  
And the same question, but user time?  
Without: 0.368 sec  
With: 1m 59.928 sec  
  
Why is the real time used lower than the sum of the other two?  
without real is 0.187 sec  
with 1m 29.607 sec


## Task D
The bug is a data race, this happends becauce the other threads can start read the singleton before it is finished init.

## Task E


## Task F
