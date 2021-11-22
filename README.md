# math-expression-parser
Parses a math expression and calculates the result. The allowed operators are
+, -, *, /. () are also supported. Implemented using Polish postfix notation.

Allowed symbols are: +, -, *, /, (, ) and digits:))). It is possible to use any
number of spaces between operators and numbers, but not inside the number, i.e.
between digits. If - sign is the first sign of expression or it is appeared
exactly after (, i.e. unary - is used, number must be written after - sign
without space.
