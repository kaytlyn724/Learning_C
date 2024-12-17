#|
Author: Kaytlyn Daffern 
Date: 9/17/22
Description: Introduction to Scheme using Dr. Racket. Understanding functional programming through multiple coding opportunities and examples.
|#



;Helper functions for Part 1

(define addThree (lambda (x y z)(+ x y z))) ;helper function for Part 1, adds 3 numbers together with input

(define addTwo (lambda (x y)(+ x y))) ;helper function for Part 1, adds 2 numbers together with input

(define subTwo (lambda (x y)(- x y))) ;helper function for Part 1, subtracts the second number from the first with input

(define multTwo (lambda (x y)(* x y))) ;helper function for Part 1, multiplies 2 numbers together with input

(define divTwo (lambda (x y)(/ x y))) ;helper function for Part 1, divides the first number by the other with input


;Part 1-5 of Part 1

(define run1 (lambda ()(addThree 4 5 6))) ;for run1, adds the 3 numbers asked together. when run1 is called this functions runs without input

(define run2 (lambda ()(divTwo(multTwo 5 (addTwo 4 5)) 2))) ;for run2, first adds 2 numbers, then multiplies that value by 5 and divides by 2, when run2 is called this function runs without input

(define run3 (lambda ()(addTwo(subTwo 100 (multTwo (divTwo 20 5)(multTwo 3 3))) (divTwo 120 10)))) ;for run3, uses multiple helper functions to solve the math problem

(define run4 (lambda ()(multTwo 5 (addTwo 4 (divTwo (addTwo (addTwo 10 10) (multTwo 5 8)) (addTwo 10 2)))))) ;for run4, uses multiple helper functions like the previous to solve the math problem

(define run5 (lambda ()(addTwo (divTwo (subTwo (divTwo (divTwo (multTwo (addTwo 3 5) (addTwo 6 4)) 2) 2) 5) 3) (addTwo (divTwo (addTwo (multTwo 2 10) (multTwo 5 4)) 2) (multTwo 4 5))))) ;for run5, uses multiple helper functions like the previous running without input



;Helper functions for Part 2

(define 3Pow (lambda (x)(* x x x))) ;for Part 2, takes one value and multiplies it to the 3rd power

(define 2Pow (lambda (x) (* x x))) ;for Part 2, takes one value and squares it


;Part A-F of Part 2

(define ff6-Damage (lambda (damage defense)(floor (addTwo (divTwo (multTwo damage (subTwo 255 defense)) 256) 1)))) ;Part A, calculates damage similar to final fantasy 6 by taking two inputs of damage and defense and uses previous functions to solve and round down

(define ffX-baseDamage (lambda (stat dmgValue)(multTwo (addTwo (floor (divTwo (3Pow stat) 32)) 32) (floor (divTwo dmgValue 16))))) ;Part B, calculates damage similar to final fantasy 10, takes 2 inputs and solves using helper functions

(define ffX-defenseNum (lambda (defense)(addTwo (floor (divTwo (2Pow (subTwo defense 280.4)) 110)) 16))) ;Part C, calculates defense with 1 input defense using helper methods

(define ffX-reduceDamage (lambda (baseDmg defenseNum)(floor (divTwo (multTwo baseDmg defenseNum) 730)))) ;Part D, calculates reduced damage with 2 inputs baseDmg and defenseNUm using helper methods

(define ffX-finalDamage (lambda (reduceDmg defense)(floor (divTwo (multTwo reduceDmg (subTwo 730 (floor(divTwo (subTwo (multTwo defense 51)(floor (divTwo (2Pow defense) 11))) 10)))) 730)))) ;Part E, calculates final Damage using 2 inputs and helper methods

(define ffX-calculate-damage (lambda (stat dmgValue defense)(ffX-finalDamage (ffX-reduceDamage (ffX-baseDamage stat dmgValue) (ffX-defenseNum defense)) defense))) ;Part F, puts together all of the previous formulas to calculate the final damage



;Helper functions for Part 3

(define cut-front (lambda (list)(cdr list))) ; takes the first value in the list out

(define repeat-recursive 
  (lambda (val repeatNum) ;takes in two values one to repeat and the other for how many to repeat
 (if (= 1 repeatNum) ;checks if the number is at 1 to stop
     (list val)
     (append (repeat-recursive val (- repeatNum 1)) (list val))))) ;otherwise calls function again with one less than the repeat number and appends val together in list


;Part A-E of Part 3

(define get-second-item (lambda (list)(car (cdr list)))) ;Part A, takes the second number in a list and outputs it

(define get-third-item (lambda (list)(car (cdr (cdr list))))) ;Part B, takes the third number in a list and outputs it

(define list-length? ;Part C, takes the input list 
  (lambda (inputlist)
    (if (null? inputlist) ;finds if it is null (empty) and if it is it returns 0 or adds nothing
        0
        (+ 1 (list-length? (cdr inputlist))))))  ;if its not it adds 1 and then calls the function again with the front variable cut from the list

(define arbitrary-cdr ;Part D (works down to 1 to get to the number)
  (lambda (cutnum inputlist) ;takes an input list and number to reach
    (if (null? inputlist) ;checks if the list is empty ande returns false if so
        #f
    (if (= 1 cutnum) ;checks if the list is at 1 to return the list and end the function
        inputlist
        (arbitrary-cdr (- cutnum 1)(cdr inputlist)))))) ;calls the function with the cut list and the 1 less than the number

(define make-list ;Part E (calls repeat-recursive to do the recursive part of this problem)
  (lambda (dupeVal sizeVal) ;takes in two inputs dupeVal and the size of the list
    (if (not (number? sizeVal)) ;checks if the size is not a number and if so it returns an empty list
        '()
    (if (number? sizeVal) ;checks if the size is a number
        (repeat-recursive dupeVal sizeVal))))) ;calls the repeat recursive function with the two values and outputs the list appended together



;Helper functions for Part 4

(define check-if-number ;function to check if every value in a list is a number
  (lambda (inputlist)
    (if (null? inputlist) ;checks if list is empty (from recursion meaning all numbers) and returns 1
        1
    (if (not (number? (car inputlist))) ;checks if the value is not a number and if it isn't it returns 0
        0
    (if (number? (car inputlist)) ;checks if the value is a number
        (check-if-number (cdr inputlist)) ;calls the function again with the cut list to check the rest of the values
        )))))


;Part A-B of Part 4

(define number-list? ;Part A (calls check-if-number) on the input list
  (lambda (inputlist) ;takes an input list
    (if (null? inputlist) ;checks if the list is empty and returns false
        #f
    (if (= 0 (check-if-number inputlist)) ;checks if the list is equal to 0 meaning there are some values that arent numbers and returns false
        #f
    (if (= 1 (check-if-number inputlist)) ;checks if the list is equal to 1 meaning that all values are numbers and returns true
        #t
        )))))   

(define multiply-list ;is given an input list that is strictly numbers from multiply-number-list
  (lambda (inputlist)
    (if (null? inputlist) ;checks if the list is null (used for recursion) and finally multiplies by 1
        1
    (* (car inputlist) (multiply-list (cdr inputlist)))))) ;if not null takes the first number of the list and multiplies it with the recursion of the function with the front number cut off

(define multiply-number-list ;Part B (uses check-if-number and multiply-list) takes an input list
  (lambda (inputlist)
    (if (null? inputlist) ;checks if the list is empty and returns false if true
        #f
    (if (= 0 (check-if-number inputlist)) ;checks if the list is equal to 0 with check-if-number meaning there is at least one value that is not a number and returns false
        #f
    (if (= 1 (check-if-number inputlist)) ;checks if the list is equal to 1 with check-if-number meaning all values in the list are numbers
        (multiply-list inputlist)))))) ;calls the multiply-list with the input and returns it's multiplied value
