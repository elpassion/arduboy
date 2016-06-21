# SnELk - Legendary game comes back on Arduboy

SnELk is game based on original Snake from Nokia 3210 and you can play it on [Arduboy](https://www.arduboy.com/). Game is written in C++ and you can check source code on [Github](https://github.com/elpassion/arduboy/tree/master/snELk)

##Arduboy
On official site of **Arduboy** we can read:
<img src="https://www.arduboy.com/assets/about/photooriginal-345b5b75595d86d1a1893be8d265c9d347e13f20bf7bb06034cb4e4c5effaad9.jpg" width="200" height="200" />
>Arduboy, the game system the size of a credit card. Create your own games, learn to program or download from a library of open source games for free!

Arduboy is based on [Arduino](https://www.arduino.cc/), that allows us to create game or application in very easy way, the only requirement is basic knowledge of C++. Programming Arduboy is like programming Arduino, but additionally we have library `Arduboy.h` that provides us easy way to print on display and handle input buttons. The only drawback I found is small amount of memory. According to the specification we have 2.5KB RAM, but using [MemoryFree](http://playground.arduino.cc/Code/AvailableMemory) library I've check that empty program have 2376 Bytes free memory, after including `Arduboy.h` we have only 1272 Bytes left.

##Implementation of game

### First approach
My first idea was to build snake as doubly linked list of `SnakePart` structures but it was not efficient enough. It was impossible to have snake longer than 83 because Arduboy runs out of memory. After few optimizations and transforming snake to singly linked list I've achieved max size of snake equal 145. It wasn't enough because game screen has 15 rows and 31 columns which gives us maximum size of snake equal 465.

### Second approach
I've figured out that I loose a lot of memory because I use it too much for single part of snake. If I have 1272 free Bytes and need to keep 465 points of board in memory I need something that is able to represent single part of snake using only 2 Bytes (Floor value from dividing 1272 by 465 is equal 2).

Single array of `int` and one index was enough to represent the snake state. Array contains each point of board, from 0 to 464. Values at index from 0 to `headIndex` are assumed to represent points where snake is on board. Value under index `headIndex + 1` represents position of food, and rest of values represent free points of board.

This solution allows us to create playable game and still have 317 Bytes left.

|   |   |   |   |   |   |
|:-:|:-:|:-:|:-:|:-:|:-:|
| 0 | 1 | 2 | 3 | 4 | 5 |
| 6 | 7 | 8 | 9 | 10| 11|
| 12| 13| 14| 15| 16| 17|
| 18| 19| 20| 21| 22| 23|
Screen - values in array represent each position on board.

| 0 | 1 | 2 | 3 | 4 / headIndex  | 5 | 6 | 7 | ... |  23|
|:-:|:-:|:-:|:-:|:--------------:|:-:|:-:|:-:|:---:|:-:|
| 13| 14| 8 | 2 |        3       | 16| 18| 21| ... | 6 |
Array with game state, snake is present at positions: 13, 14, 8, 2, 3 of screen and food is at position 16.
