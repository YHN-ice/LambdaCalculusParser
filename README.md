![Arithmic Check](https://github.com/YHN-ice/LambdaCalculusParser/actions/workflows/c-cpp.yml/badge.svg?event=push)

# $\lambda$ Calculus
## tokens

`[A-Z]` for symbol (i.e., name of a $\lambda$ calculus)

`[a-z]` for identifier inside $\lambda$ calculus (i.e., variable names, free or bound)

`$` for start of function definition

`.` for delimiter of argument and body

## algebra[^1]

define $0$:

```
O=$x.$y.y
```

define successor

```
S=$w.$j.$i.j((w j)i)
```

then one should be

```
I = S O
```

try `make && ./test<demo.in`, and the $\sum_0^{12}i$ and $6!$ should be calculated and output.

## TODO

- default left associative apply not implemented yet
- and identifier should be separated with blank for now
- $Y R I$ should be written as $(Y R) I$ explicitly
- performance optimization, caused _OOM_ when calculating $\sum_0^{36}i$
  - (the order of reduction/apply, outermost or innermost, for now just greedily apply if possible)
- dynamic library, read in function definition, translate to lambda calculus, and capture any further calling to make it run in lambda calculus framework (in hope of reduced recursive call stack space)
- **practical/most crucial/theoretical challenge**: what kind of functions(with costive recursive calls) can be interpreted as lambda calculus and run in the paradigm of lambda reduction

## Reference

[^1]: [Rojas R. A Tutorial Introduction to the Lambda Calculus. FU Berlin[R]. WS-96/98, 1997.](https://personal.utdallas.edu/~gupta/courses/apl/lambda.pdf)
