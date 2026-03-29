# calign
Command line alignment tool

## How to use
```
./calign [SEQ 1] [SEQ 2]
```

for example:
```
user@device:~$ ./calign GCATGCG GATT

        G   C   A   T   G   C   G 
    0  -1  -2  -3  -4  -5  -6  -7 
G  -1   1   0  -1  -2  -3  -4  -5 
A  -2   0   0   1   0  -1  -2  -3 
T  -3  -1  -1   0   2   1   0  -1 
T  -4  -2  -2  -1   1   1   0  -1 

Score -3
GCATGCG
G-AT--T
```
