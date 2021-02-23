# CONSOLE APPLICATION: Poker Project

## Run The Code:
simply download the latest release and run the executable

## The Project:
This project implements a knockout texas hold'em poker game in c++. 
This game is designed using OOP, where each class is divided into a .h and a .cpp file.
Currently, this project runs on the console terminal and can either be played in the 
consoleor just with bots, where you see them play each other until there is one winner 
left.
In this project, I attempt to implement this game as efficiently as possible.

## Structure:
- Game, this class manages the game flow.
- PotsHandler, this class manages the pots in the game, as when someone can't call anymore
a new pot needs to be opened for other players' further betting.
- Pot, this class represents a poker pot that keeps track of who contributed to it and how 
much each player contributed.
- Player, this class is an abstract class that represents a player and includes all the 
actions that are necessary for the game to run. All actual players inherit from this class.
- Decision, this class represents a decision that players can make and contains the action 
they want to take, and the amount of chips associated with that action.
- Table, this class represents the table that the game is being played on, which contains a 
deck of cards and the open cards that are on the table (flop, turn, river).
- Deck, this class represents a deck of 52 cards (w/o jokers).
- Hand, this class represents a player's hand (2 cards).
- HandAttempt, this class represents a hand in poker (5 card combinations) which contains 
the type of the hand (high card, pair, two pair, etc.) and a hashcode for compairing purposes.
- Card, this class represents a card (a symbol and a value).

## Future Features:
- a smarter bot.
- a GUI.
- online play.

### feedback:
Any feedback to make my code better would be much appreciated, feel free to email me @ 
talbenyehezkel@gmail.com