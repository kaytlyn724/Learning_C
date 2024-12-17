#|
Author: Kaytlyn Daffern
Date: 9/23/22
Description: Use Scheme to create the Sieve of Eratosthenes, the method for finding all primes below some max number
|#

;Part 1 Making a list from 2 to n (input) using right fold

(define make-2-n-list
  (lambda (upto) ;takes 1 input as the maximum value 
    (if (= 2 upto) ;checks if the value is equal to our minimum 2
        (list upto) ;turns the value into a list
        (append (make-2-n-list (- upto 1)) (list upto))))) ;adds each value to the list and recursively calls the function again with 1 less of the maximum value going left (right fold) of the last value

;Part 2 Checking to see if the first value is devisible by the second

(define divisible-by?
  (lambda (dividend divisor) ;takes 2 inputs in one to divide the other 
    (if (eq? dividend divisor) ;checks if the 2 values are equal and returns false if so
        #f
    (if (= 0 (modulo dividend divisor)) ;checks if the mod of the two is 0 and if so returns true of not returns false
        #t
        #f))))

;Part 3 Checking to see if the first value is not divisible by the second with currying

(define not-divisible-by
  (lambda (divisor) ;takes in 1 input originally that will be checked to divide the other
    (lambda (dividend) ;takes in the second input that will be divided by
      (not (divisible-by? dividend divisor))))) ;first nots the divisible-by? function and calls it with the two inputs

;Part 4 Creates a function to filter a list based on the fuction and list parameters given

(define filter-list
  (lambda (func inputlist) ;takes in a function and a list
    (if (null? inputlist) ;checks if the list is null and returns an empty list if so but if not continues to the next if
        '()
        (if (eqv? (func (car inputlist)) #t) ;checks if the value from the inputlist in the function is t
            (append (list (car inputlist)) (filter-list func (cdr inputlist))) ;if it is true it appends the value to the new list to output and calls the function recursively
            (filter-list func (cdr inputlist)) ;otherwise moves onto the next value recursively
            ))))

;Part 5 Creating a simplified Sieve

(define primes-under-n ;helper function for finding all primes under a specific number
  (lambda (inputlist upto x) ;takes 3 inputs, list a max and a current number
    (if (<= x upto) ;checks if the current number is less than or equal to the max
        (primes-under-n (filter-list (not-divisible-by x) inputlist) upto (+ x 1)) ;if so it calls the function recursively with the filter list and divisble by x and only takes the ones that arent divisble by the current val and goes up 1
        (append '() inputlist) ;if the x is greater than the max it ends the list
        )))

(define prime-sieve 
  (lambda (upto) ;takes one input to find all primes under that number
    (primes-under-n (make-2-n-list upto) (floor (sqrt upto)) 2))) ;uses the primes-under-n function with 2 to the value as the list and the squareroot of the value to set as max and 2 for the start val