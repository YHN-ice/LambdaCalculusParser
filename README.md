![Arithmic Check](https://github.com/YHN-ice/LambdaCalculusParser/actions/workflows/c-cpp.yml/badge.svg?event=push)

# $\lambda$ Calculus

An experiment interpreter for Lambda Calculus proposed by *Alonzo Church*[^1], featureing

+ Complete parsing for variables, functions and applications in Lambda system
+ A read–eval–print loop interpreter in which you can define your own lambda terms and run beta reduction on expression
+ A driver program which will read in user-defined function-generator and run it recursively with given numeric argument
+ For recursive call, optimized evaluation such that recursive invocation is deferred in a best-effort manner, leaveraging the reference referential transparency of pure functional language.

## Syntax

Lambda calculus terms fall in one of three class, i.e., variable, function or application

First, the terminal tokens includes:

```
SYMBOL: [A-Z]+
IDENTIFIER: [a-z]+
EQ: =
LPAREN: (
RPAREN: )
DOT: .
LAMBDA: $
```



Second, the rules to build expressions and assignments, where assignments are just storing an expression in memory with associated name.

```
assignment  : SYMBOL EQ expression

expression  : expression_r
            | expression_l 
            ;
            
expression_r: SYMBOL 
            | IDENTIFIER 
            | function
            | LPAREN expression RPAREN
            ;

expression_l: application;

function    : LAMBDA IDENTIFIER DOT expression;

application : expression expression_r;
```



In the rule of expression above, we separated application from other expressions (functions, variables) so that tail part of application can't be another application, thus realized left associative for chained applications, e.g.,
$$
X\ Y\ Z \equiv ((X Y) Z)
$$
The rules are quite straightforward, we use lowercase identifiers to make variable and use uppercase words as symbols to store a lambda term (which could be varaible, function or application) in memory. If a (uppercase) symbol is encountered parsing an definition of lambda term, the corresponding lambda term stored in memory will be constructed and used in place of that symbol.

As an example, you could input the following lines in the interactive repl program (built by `make lambda_repl`)

```
T = ($x.$y.x)
F = ($x.$y.y)
N = $x.(x F)T
N T
```

And the result would be `($x.($y.y))`, which is $F$.

## Algebra[^2]

To do some arithmetic operation using lambda system. We need to first represent numbers and arithmetic operators in form of lambda terms.

| Lambda                                                       | Arithmetic |
| ------------------------------------------------------------ | ---------- |
| `T = ($x.$y.x)`                                           | True |
| `F = ($x.$y.y)`  | False |
| `N = $x.(x F)T`                                                             | Logic Negation |
|`Z = $x.((x F)N)F`|x<=0|
|`S = $w.$n.$m.n((w n)m)`|x+1|
|`O = $s.$z.z`|0|
|`I = S O `|1|
|`P = $n.((n K)($z.(z O)O))F`|x-1[^3]|
|`M = $x.$y.$z.x(y z)`|x*y|
|`G = $x.$y.Z(x P y)`|x>=y|
|`R = $x.((x F)N)T`|x >= 1|

Then 4*7+9 could be expressed as `NINE S (M FOUR SEVEN) `

## Recursion

In lambda calculus, we we cannot write the function to bedefined itself in its body. So we need a trick to fulfill recursion. We first define the generator for the recursive function, then pass it to Y combinator to get the desired recursive function. Some examples of recursive function generators are placed in directory `input`, which could be read by driver program, try `./bin/driver FACT 5`. 

```
Y = ($y.($x.y(x x))($x.y(x x)))
FACTORIALGENERATOR = $factorial.$a.(G I a)(I)((M a (factorial(I P a))))
FACTORIAL = Y FACTORIALGENERATOR
FACTORIAL 4
>> 24
```

## Eval Driver

A clean entry point to eval recursive function, run as `./bin/driver FACT 5`.

It will run as the following steps 

1. try to initialize with the predefined lambda calculus in `/input/demo.in`
2. read the file `input/FACT.lambda`, which should store an assignment `FACT=...`
3. evaluate  `Y  FACT FIVE` where `FIVE=S(S(S(S(I))))`. 

To add a customized recursive function, put a new lambda function definition in `input/<function name>.lambda`, and run `./bin/driver <function name> <0,1,2...>`[^4]

## TODO

- [x] default left associative apply not implemented yet
  - and identifier should be separated with blank for now
  - $Y R I$ should be written as $(Y R) I$ explicitly

- [x] performance optimization, used to cause _OOM_ when calculating $\sum_{i=0}^{36}i$
  - (the order of reduction/apply, outermost or innermost, for now just greedily apply if possible)
  - solved by recursive call optimization.
- [ ] dynamic library, read in function definition, translate to lambda calculus, and capture any further calling to make it run in lambda calculus framework (in hope of reduced recursive call stack space)
  - [x] First attempt, LLVM plugin

- [ ] **practical/most crucial/theoretical challenge**: what kind of functions(with costive recursive calls) can be interpreted as lambda calculus and run in the paradigm of lambda reduction

- [ ] **Fix error** when calculate `M FOUR SEVEN S NINE`, which is correct when calculated as  `NINE S (M FOUR SEVEN)`
      - So I find out the root of the bug, its when we brought a free variable in to a bounded term, i.e., body of a function abstraction. Here is a scenario that happens in a reduction as `(((M (S(S(O)))) (S(O)) S) I)`
        ```+($m. ++( _($w.($n.($m.(n ((w n) m)))))_  ((($s.($z.z)) ($w.($n.($m.(n ((w n) m)))))) m)  )++ )+ ```
        The replace ment happens in the range surrouned by `++`, the `_(.)_` is a function to be applied. The second term contains a `m` variable that is bounded by the outermost `$m`. When replacing the `w` in `$n.$m.(n ((w n) m) ), the `m` will be accidently brought in the intermost function, and thus bounded by wrongly by that `$m`. To fix that problem, we need a non-conflicting naming schema.

## Reference

[^1]: B Paul. Alonzo Church. An unsolvable problem of elementary number theory. American journal of mathematics, vol. 58 (1936), pp. 345–363.[J]. Journal of Symbolic Logic, 1936, 1(2):73-74.
[^2]: [Rojas R. A Tutorial Introduction to the Lambda Calculus. FU Berlin[R]. WS-96/98, 1997.](https://personal.utdallas.edu/~gupta/courses/apl/lambda.pdf)
[^3]: any number less than 0 will evaluate to 0 in this representation
[^4]:  naming of `<function name>` should conform with the token definition of `SYMBOL`
