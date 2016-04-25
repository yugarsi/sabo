# sabo
Size aware buffer overflow prevention library in C

## Project test case status

##### test1.c - PASSED

```bash
./runner.sh -f overflow_prevention/test1.c -a `python -c "print 'a'*248 +'b'*18"`
```

##### test2.c - PASSED

```bash
./runner.sh -f overflow_prevention/test2.c -a `python -c "print 'a'*248 +'b'*18"`
```

##### test3.c - PASSED

```bash
./runner.sh -f overflow_prevention/test3.c -a "`python -c "print 'a'*499 +'b'*18 + ' ' + '37_aa ' + '755'"`"
```

## TODO

#### Library framework setup for size aware buffer overflow variables: DONE

#### Safe functions:
+ strcpy: DONE
+ gets: DONE
+ fgets:DONE
+ strncpy: DONE
+ sprintf: DONE
+ strcat: DONE

#### Ensuring backward compatibility
+ free: DONE
+ realloc: DONE

others...
