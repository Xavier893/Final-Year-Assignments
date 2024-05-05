#lang sicp

(define (circleArea radius)
  (let ((pi (/ 22 7)))  ; Bind pi using let
    (if (<= radius 0)
        0  ; Return 0 for radius <= 0
        (* pi (* radius radius)))))  ; Calculate and return area

(define (countDivisors num divisorList)
  (cond ((null? divisorList) 0)  ; If list is empty, return 0 (no divisors)
        ((divides? num (car divisorList))  ; Check if current element divides num
          (+ 1 (countDivisors num (cdr divisorList))))  ; Add 1 and recurse
        (else (countDivisors num (cdr divisorList))))) ; Otherwise, recurse

(define (divides? num divisor)
  (= 0 (remainder num divisor)))  ; Helper function to check divisibility

(define (getEveryEvenElements lst)
  (cond ((null? lst) '())
        ((= (length lst) 1) '())
        (else (getEvenElements lst 1))))

(define (getEvenElements lst currPos)
  (cond ((null? lst) '())
        ((= (remainder currPos 2) 0)
         (cons (car lst) (getEvenElements (cdr lst) (add1 currPos))))
        (else (getEvenElements (cdr lst) (add1 currPos)))))

(define (add1 x)
  (+ x 1))

;(circleArea -1.8)
;(countDivisors 3 `(2 3 1 9 5))
;(countDivisors 10 `(20 50 60))
;(countDivisors 10 `(1 20 30 2 5 40 10 60))
;(getEveryEvenElements `())
;(getEveryEvenElements `(a))
(getEveryEvenElements `(a b c d e f))