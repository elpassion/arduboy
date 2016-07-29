# SnELk - Legendary game comes back on Arduboy

SnELk is clone of Snake from Nokia 3210 written in C++ on [Arduboy](https://www.arduboy.com/). Check my source code on [Github](https://github.com/elpassion/arduboy/tree/master/snELk)

## Arduboy
<img src="https://www.arduboy.com/assets/about/photooriginal-345b5b75595d86d1a1893be8d265c9d347e13f20bf7bb06034cb4e4c5effaad9.jpg" width="200" height="200" />

On official site of **Arduboy** we can read:
>Arduboy, the game system the size of a credit card. Create your own games, learn to program or download from a library of open source games for free!

[Arduboy](https://www.arduboy.com/) is portable console based on [Arduino](https://www.arduino.cc/), that allows us to create games and apps very easly. The only requirement is basic knowledge of C++. To develop Arduboy we use the same tools that we use to develop Arduino. Arduboy gives us `Arduboy.h` library that allows us to print on display, handle input buttons and making sounds.

The only drawback I found is small amount of memory. According to the specification we have 2.5KB RAM. I used [MemoryFree](http://playground.arduino.cc/Code/AvailableMemory) to check if it's truth. After I deployed an empty project, 2376 Bytes left, when I included `Arduboy.h` there was only 1272 Bytes.

## Implementation of game

### Naive
My first idea was to build snake as doubly linked list of `SnakePart` structures but it was not efficient enough. It was impossible to have snake that contains more than 83 parts because Arduboy runs out of memory. After few optimizations and transforming snake to singly linked list I've achieved max size of snake equal 145. It still wasn't enough because game screen has 15 rows and 31 columns which gives us maximum size of snake equal 465.

### Need to go deeper
I've figured out that I lose a lot of memory because I use it too much for single part of snake. If I have 1272 free Bytes and need to keep 465 points of board in memory I need something that is able to represent single part of snake using only 2 Bytes (Floor value from dividing 1272 by 465 is equal 2).

Array of `int` and one index was enough to represent the snake state. Array of size 465 contains values from 0 to 464 that represents each point of board. Values at positions from 0 to `headIndex` represent points where parts of snake are. Value at position `headIndex + 1` represents point with food, and rest of values in array represent free points of board.

This solution allows to create playable game and still have 317 Bytes left.

Points of board are arranged in the following order:

|   |   |   |   |   |   |
|:-:|:-:|:-:|:-:|:-:|:-:|
| 0 | 1 | 2 | 3 | 4 | 5 |
| 6 | 7 | 8 | 9 | 10| 11|
| 12| 13| 14| 15| 16| 17|
| 18| 19| 20| 21| 22| 23|

Example array with game state. Snake is present at points: 13, 14, 8, 2, 3, food is at point 16, and rest of points are empty.

| 0 | 1 | 2 | 3 | 4 / headIndex  | 5 | 6 | 7 | ... |  23|
|:-:|:-:|:-:|:-:|:--------------:|:-:|:-:|:-:|:---:|:-:|
| 13| 14| 8 | 2 |        3       | 16| 18| 21| ... | 6 |
