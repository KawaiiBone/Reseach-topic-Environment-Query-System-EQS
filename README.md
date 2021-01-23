# Reseach-topic-Environment-Query-System-EQS
 
 ## What is it
 
It is a feature for an ai, to obtain information from its environment. 
Then collect that information to see via a couple of tests. Which of the information that is collected, 
is the most needed at the moment in time. The way it knows what actions to do, is asking questions from a Behavior tree.

A good example is to know where you have seen food and then if needed, 
go to the nearest place where you can obtain that. 
To collect information it uses Generators to see which one is the best and returns that item to the Behavior tree. 
Via Tests it knows which option to do is best. 


## Implementation of EQS in the zombie game

In the zombie game, it collects information about the buildings and items via a field of view.
 It knows which building it has been to and which items it left behind for later use.
