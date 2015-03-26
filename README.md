# Modulus
Program to list or test for irreducible polynomials of (ℤ/pℤ)[x], where p is a prime number.

The program is under development. Do not expect anything, not even compiling.
Before there is a 'final' version, you can find the TODOs and guidelines.

## Strategy
### Thoughts
Create class for polynomials over ℤ/pℤ. This class must provide efficient creation of instances, addition and multiplication.
Everything else is not that important.
There will be some degree n given, up to which the program has to find the irreducible polynomails.
Therefor at first generate all¹ polynomials for each degree up to n.
This can be done with p-adic increment, and interpret the digits of this number as coefficients.
These polynomials will then be sieved sequentially in degree, so that lower degrees are always sieved first and working on higher ones means to assume the lower ones are already sieved.
Then, for each degree, find all additive decompositions².
For each decomposition, the numbers in the decomposition indicate the degrees of polynomials to be multiplied.
So build the cross-product of these sets, and for each product polynomial, remove it from the list, as it is reducible.

Like that, the sieved list only contains irreducible polynomials, and no irreducible polynomials will accidently be removed.

¹ Because ℤ/pℤ is a finite set, there is a finite number of polynomials of any degree.

² An additive decomposition of a number x is a list of positive numbers that sum up to x. There is only a finite number of decompositions for any number.

### Examine and Imporove the Strategy
* Do not generate all additive decompositions. (ℤ/pℤ)[x]'s multiplication is commutative, so two decompositions work equal if they just have different ordering. Pick one canonical representative of each kind.
* Don't create the same polynomial more than once. (TODO: Explain how...)
* The decompositions are independent, so there is potential for parallel execution.

## C++-Standard
Modulus is explicitly developed in C++14, Clang 3.5 supports all features. All contributors are free to use them.

After finishing there maybe will be an extra branch or code package to be compiled in lower standards.

## TODO
- [x] create class for elements of ℤ/pℤ.
  - [x] make it a template class with p template parameter.
  - [x] Behaves as expected even if p is not prime.
  - [x] debug class.
  - [ ] optional³: Throw exception for division by zero divisor. (Division will not be needed)
- [x] create class for polynomials with coefficients of ℤ/pℤ.
  - [x] make it a template class with type T an arbitrary template parameter.
  - [x] make it a template class with arbitrary type for degrees.
  - [ ] **make class work.**
  - [ ] **debug class.**
  - [ ] optional³: (efficient) plug-in operator.
- [x] create function to find all canoncal additive decomposoitions.
- [x] create function to generate the polynomials.
- [x] create function to sieve.
  - [ ] improve speed with better data structure, esp. use `set` or `unordered_set` instead of `list`.
  - [ ] use parallel execution if speed improvement outweighs parallel overhead.
- [x] create main.
  - [x] help: how-to-use.
  - [x] print results to file, if the user wants that.
  - [ ] read the input from file, if the user wants that.

³ This feature is not needed for sieveing polynomials, but maybe useful for someone using the files for other stuff.

## Giudelines
* Spaces are explicitely but not exclusively present
    * after commas,
    * after semicolons,
    * around binary arithmetric/bit operators.
* Spaces are absent
    * before commas,
    * before semicolons,
    * after unary operator symbols.
* Indent is 4, no tabs.
* Opening braces eiher
    * are closed on the same line, or
    * have a separate line for their own.
* `if` ... `else if` ... `else`, if they are followed by simple single statements, should have tabular style and the statements must be finished on the same line. 
* `for`, `while`, `if` etc. can follow up without braces if everything fits in one line.
