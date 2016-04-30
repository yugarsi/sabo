# sabo
Size aware buffer overflow prevention library in C

> Yugarsi Ghosh, Sailik Sengupta

## Project test case runner (a example to use the runscripts)

##### test1.c - PASSED

```bash
./runner.sh -f overflow_prevention/test1.c -a "`python -c "print 'a'*248 +'b'*18"`"
```

##### test2.c - PASSED

```bash
./runner.sh -f overflow_prevention/test2.c -a "`python -c "print 'a'*248 +'b'*18"`"
```

##### test3.c - PASSED

```bash
./runner.sh -f overflow_prevention/test3.c -a "`python -c "print 'a'*499 +'b'*18 + ' ' + '37_aa ' + '755'"`"
```

More details for running can be found in `runner.sh`

#### Safe memory allocation functions
+ malloc
+ free  
+ realloc

#### Safe functions:
+ strcpy
+ gets
+ fgets
+ strncpy
+ sprintf 
+ strcat 
