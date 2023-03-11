# Cache Test Plan
## Case 1
Goal: Write to L1 Data cache and verify output
Result: Line was updated
1. Write to line [1 2903874]
2. Print cache [9 2903874]
## Case 2
Goal: Data Read request, line is in L1
Result: Log Hit
1. Write to line [1 2903874]
2. Read line [0 2903874]
3. Log Hit
## Case 3
Goal: Data Read request, line is not in L1
Result: Log Miss
1. Read line [0 2903874]
2. Log Miss
## Case 4
Goal: Instruction Read request, line is in L1
Result: Log Hit
1. Write to line [1 2903874]
2. Read line [2 2903874]
3. Log Hit
## Case 5
Goal: Instruction Read request, line is not in L1
Result: Log Miss
1. Read line [2 2903874]
2. Log Miss
## Case 6
Goal: Read request from L2, Line is in L1
Result: Log Hit
1. Write to line [1 2903874]
2. Read line [4 2903874]
3. Log Hit
## Case 7
Goal: Read request from L2, L1 Line is not in L1
Result: Log Miss
1. Read line [4 2903874]
2. Log Miss
## Case 8
Goal: Invalidate line in cache
Result: Invalid MESI bit on line and log miss
1. Write to line [1 2903874]
2. Read line [0 2903874]
3. Invalidate line [3 2903874]
4. Read line [0 2903874]
5. Log miss
## Case 9
Goal: Reset Cache
Result: Empty Cache
1. Write to line [1 2903874]
2. Print Cache [9 2903874]
3. Reset Cache [8 0000000]
4. Print Cache [9 2903874]