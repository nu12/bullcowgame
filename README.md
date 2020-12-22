# Bull Cow Game

Bull Cow Game is part of Unreal 4 Course by GameDev.tv at Udemy.

Check it out: [link](https://www.udemy.com/course/unrealcourse/)

## About

Bull Cow Game is a fun word guess game introduced by GameDev.tv as part of their overwhelming Unreal Engine 4 Course. This project is a tribute to the original one and a showcase of the capabilities developed with the course. It aims to keep the same overall idea of the game created in the course, but leveraging elements and technics covered in latter modules, like physics handling, spawning and grabbing actors, spawning emitters, sounds & other effects, as well as some other elements not covered in the course like UI animations, cinematics, and others.


## Rules

The objective of the game is to guess a series of words. To do so, you have to interact with the Bull-Cow-Game-Machine (a.k.a "The machine").

The game starts when you pull the machine lever for the first time. Each round, it will select a word and give you some letters - not all of them belong to the word, but it's your job to figure which ones to use. All words are isograms: they don't have repeated letters.

To enter your guess, you place the letters on the cuts above the machine lights - there are a total of 8 lights and 8 cuts - and pull the lever. The lights will change their color to indicate the result: 

* A white letter means that you need to provide a letter for that position
* A green light means that the letter is in the right position (a.k.a "bull") 
* A yellow light means that the letter is in the wrong position (a.k.a "cow")
* A red light means one out of three thing: 1) there is no letter in the related cut, 2) there are two or more letter in the related cut, which is not allowed, or 3) the letter in the cut simply don't belong to the word.
* Letter in cuts where the light is turned off won't be evaluated.

When all light are green, it means that you successfuly guessed the word and the next time the lever is pulled, a new one will be selected. Repeat the process until you clear all levels - or until the time's up!

Pulling the lever will pause and resume the game. While in pause, the timer won't tick, but you won't be able to grab anything. Use this time to think about the next move. To be able to grab again, pull the lever to resume the game.