# ECE485-585 Cache Project
# Team 20
[Adam Martinez](https://github.com/ajm27pdx)\
[Demetrius Van Sickle](https://github.com/HailStorm32)\
[Jinghan Zhang](jinghan2@pdx.edu)\
[Josue Tristan](https://github.com/jtristan123)\
[Wallace McKenzie](https://github.com/Poly-guru)

NOTE: 
Useses counter based LRU with 111 being the least recently used line

## Usage
```
./main [mode:(0,1,2)] [filename]
```
### Mode
0 (default) - Displays only the required summary of usage statistics and responses to 9s in the trace file and nothing else.\
1 - Displays everything from Mode 0 and displays communication messages to the L2 cache.
2 - Displays everything from Modes 0 & 1 as well as extra debug messages


### Filename
Defaults to supplied test.txt


### Example

`./main 2 test.txt`

