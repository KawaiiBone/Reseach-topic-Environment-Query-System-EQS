# Reseach-topic-Environment-Query-System-EQS
 
 ## What is it
 
It is a feature for an ai, to obtain information from its environment. 
Then collect that information to see via a couple of tests. Which of the information that is collected, 
is the most needed at the moment in time. The way it knows what actions to do, is asking questions from a Behavior tree.

A good example is to know where you have seen food and then if needed, 
go to the nearest place where you can obtain that. 
To collect information it uses Generators to see which one is the best and returns that item to the Behavior tree. 
Via Tests it knows which option to do is best. 


# Implementation of EQS in the zombie game

## Quick explanation: 
In the zombie game, it collects information about the buildings and items via a field of view.
It knows which building it has been to and which items it left behind for later use.

## How it is done in the game: 
Via its field of view, it looks at buildings, enemies, and items. 
By looking at the building it knows via location if it has been into this building before. 
Then it searches the building and picks up the items that it needs.
It leaves any item behind that the ai cannot carry and remembers those who could be useful later in the game.
This feature is especially useful in late game. 
Now it knows all this information and checks if it needs any of those items left behind. 

# Result

This is a feature that is very useful for late game. 
Via its information gained then it can survive longer and better.



But the down side with this is, is that it requires you to get there at the late stage of the game. For the feature to be working well.
