# ANSWERS

## TASK A

What happens?  
The computer freezes when allocating memory, after this the memory is full and the swap about 2.3 gb
  
What is the difference between ”Memory” and ”Swap”?  
Memory is the ram, Swap is when you write ram memory to the harddisk. This reduces speed of the memory.  
  
Now change the shape of the matrix by increasing x dim first to 1000, then 10000, and run the
program again. What happens?  
Program is faster on 1000 and much faster on 10000. The memory allocated is less on 1000 and even less on 10000.  
  
Run the program with x dim = 10000 again, but this time write zeros to all the memory:  
Explain why something different happens when the memory is also cleared.  
The program now uses longer time again like in the first question.  
The program now also uses a litle longer time when clearing memory
  
## TASK B
Create an array with some small capacity, then insertBack more elements  
than there is capacity for, and run the program. What happens?  
It seems to work
  
Compile and run the program again, but now add the compiler flags -g -fsanitize=address.  
What happens now?  
We get a heap-buffer-overflow.
  
## TASK C
works

## TASK D
2 4 8 16 32 64 -> 2x gives often better speed.  
2 3 4 6 9 13  -> 1.5x gives better memory optimization.  
What to choose depends on the task.