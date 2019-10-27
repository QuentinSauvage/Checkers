# Checkers
Checkers game with an AI made during my first year of Master's degree.
The AI uses the minimax algorithm and alpha-beta pruning.
The network section was made by our teacher.

#Build and Run
```shell
make && ./exec <network> <n> <color> <host>
```
network: 0 to play offline, 1 to play online.

n: Indicates the number of AI playing (0, 1 or 2).

color (if network set to true or playing against AI): Choose the color of the player.

host (if network set to true): Host adress.

For instance, if you want to play offline with two players, enter this command:
```shell
make && ./exec 0 0
```

Against AI, locally:
```shell
make && ./exec 0 1 'B'
```
```shell
make && ./exec 0 1 'N'
```
