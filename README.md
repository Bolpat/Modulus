# Modulus
Modulus is a program to list or test for irreducible polynomials of (ℤ/*p*ℤ)[*x*], where *p* is a prime number.

It is under development. Do not expect anything, not even compiling.
Until the final version, you can find bold elements in the TODO.

## Strategy
### Thoughts
Create a class for polynomials over ℤ/*p*ℤ. In particular, this class must provide efficient creation of instances, addition and multiplication.
Everything else is not necessary and therefore only interesting for refactoring.
There will be some degree *n* given, up to which the program has to find the irreducible polynomials.
Therefor at first generate all¹ polynomials for each degree up to *n*.
This can be done with *p*-adic increment, and interpret the digits of this number as coefficients.
These polynomials will then be sieved sequentially in degree, so that lower degrees are always sieved first and working on higher ones means to assume the lower ones are already sieved.
Then, for each degree, find all additive partitions².
For each partition, the numbers in the partition indicate the degrees of polynomials to be multiplied.
So build the cross-product of these sets, and for each product polynomial, remove it from the list as it is reducible.

Like that, the sieved list only contains irreducible polynomials, and no irreducible polynomials will be accidently removed.

¹ Because ℤ/*p*ℤ is a finite set, there is a finite number of polynomials of any degree.

² An additive partition of a number *x* is a list of positive numbers that sum up to *x*.
  There is only a finite number of partitions for any number.

### Examine and Improve the Strategy
  * Do not generate all additive partitions.
    (ℤ/*p*ℤ)[*x*]'s multiplication is commutative, so two partitions work equal if they just have different ordering.
    Pick one canonical representative of each kind.
  * Don't create the same polynomial more than once.
    This happens iff in the cross-product mentioned above, some set occours more than once.
    Then (because the set is always ran through the same order) we can simply ignore elements been ran over in the same run.
    It is very difficult to explain&nbsp;…
  * For a fiexed degree, the partitions are independent, so there is potential for parallel execution.

## C++-Standard
Modulus is explicitly developed in C++14, Clang (version 3.5 and later) supports all features with `-std=c++14`-flag³.
All contributors are free to use.
For details, see the [LLVM Site](http://clang.llvm.org/cxx_status.html).
Today GCC does not implement all C++14 features.

After finishing there may be an extra branch or code package to be compiled in lower standards.

³ For Clang 3.4, use `-std=c++1y` instead.

## TODO
The bold elements are being worked on.
  - [x] Create class for ℤ/*p*ℤ.
    - [x] Make it a template class with *p* template parameter.
    - [x] Behaves as expected even if *p* is not prime.
    - [x] Debug class.
    - [ ] Optional⁴: Throw exception for division by zero divisor.
  - [x] Create class for polynomials with coefficients of ℤ/*p*ℤ.
    - [x] Make it a template class with type *T* an arbitrary template parameter.
    - [x] Make it a template class with arbitrary type for degrees.
    - [x] Make class compile.
    - [ ] Optional⁴: debug (ℤ/2ℤ)[*x*]'s `divmod`.
    - [ ] Optional⁴: efficient(!) plug-in operator.
  - [x] Create function to find all canonical additive partitions.
  - [x] Create function to generate the polynomials.
  - [x] Create function to sieve.
    - [x] Improve speed with better data structure, esp. use `set` or `unordered_set` instead of `list`.
    - [x] Make parallel execution possible.
      - [ ] Check if speed improvement outweighs parallel overhead
    - [x] Split sieve in two functions:
      - [x] One to return the irreducible polynomials,
      - [x] One to return the map for each reducible polynomial to its decomposition.
      - [ ] Try to either merge these functions or make clear it is not possible.
  - [x] Create main.
  - [ ] Command-line options:
    - [x] Help: how-to-use.
    - [x] Print results to file, if the user wants that.
    - [ ] Style of output; e.&nbsp;g. human readable, CSV etc.
    - [ ] Read the input from file, if the user wants that.
    - [ ] Feedback of file input; e.&nbsp;g. for ill-formed input, ignore or message or abort?
  - [ ] Compiler option: `_MAX_P` (usage `-D_MAX_P=p`) for setting the maximum prime number available.

⁴ This feature is not needed for sieveing polynomials, but useful for someone using the files for other stuff.

## Giudelines
These guidelines are not forced, but a guide to make code readable.
  * Spaces are explicitely but not exclusively present
    * after commas,
    * after semicolons except at end of line, doubled between full statements,
    * around binary arithmetric/bit operators.
  * Spaces are absent
    * before commas,
    * before semicolons,
    * after unary operator symbols.
  * Indent is 4 spaces, no tabs.
  * Opening braces eiher
    * are closed on the same line, or
    * have a separate line for their own.
  * `if` … `else if` … `else`, if they are followed by simple single statements, should have tabular style and the statements must be finished on the same line.
  * `for`, `while`, `if`, etc. can follow up without braces if everything fits in one line.
