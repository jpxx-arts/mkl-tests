- Para compilar:
```console
foo@bar:~$ make
```
- Para executar (LAPACK):
```console
foo@bar:~$ make run ARGS="<number_rows> <number_columns> <type> <matrix_type> <ops> <show> <seed> <specific_test>"
```

- Para executar (BLAS):
```console
foo@bar:~$ make run ARGS="<A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>"
```
