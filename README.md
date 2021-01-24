# Reseach-topic-Environment-Query-System-EQS
 
 ## What is it
 
It is a feature for an ai, to obtain information from its environment. 
Then collect that information to see via a couple of tests. Which of the information that is collected, 
is the most needed at the moment in time. The way it knows what actions to do, is asking questions from a Behavior tree.

A good example is to know where you have seen food and then if needed, 
go to the nearest place where you can obtain that. 
To collect information it uses Generators to see which one is the best and returns that item to the Behavior tree. 
Via Tests it knows which option to do is best. 

![The ai sees items in a building](/Pictures/ZombieGettingItems.jpg)



# Implementation of EQS in the zombie game

## Quick explanation: 
In the zombie game, it collects information about the buildings and items via a field of view.
It knows which building it has been to and which items it left behind for later use.

## How it is done in the game: 
Via its field of view, it looks at buildings, enemies, and items. 
By looking at the building it knows via location if it has been into this building before. 
Then it searches the building and picks up the items that it needs.
It leaves any item behind that the ai cannot carry or has to many of and remembers those who could be useful later in the game.
This feature is especially useful in late game. 
Now it knows all this information and checks if it needs any of those items left behind. 


## How it behaves in the behavior tree:
while collecting items, it doesn’t collect those who the ai has too many of and when the inventory is full. It remembers the items it has left behind. When its hp or energy is low, isn’t exploring buildings and doesn’t have anything in its inventory to fix the problem. It will go to the nearest location where it has the necessary item it has left behind to collect and use. 




# Result

Now that Environment query system is in the zombie game, 
the ai knows where it left its items behind for later use and go towards them when the time comes when the ai needs it. 
It will avoid houses that are already searched in. 
Except when the ai knows something is good in there for it that it has left behind. 

![The ai sees items in a building and picks the ones it needs](/Pictures/Getting_items.gif)

This is a feature that is very useful for late game. 
Via its information gained then it can survive longer and better.
But the down side with this is, is that it requires you to get there at the late stage of the game. For the feature to be working well.
