#lang sicp

(define (stripNegativesDuplicateEvens lst)
  (cond
    ((null? lst) '())                                
    ((negative? (car lst)) (stripNegativesDuplicateEvens (cdr lst)))
    ((even? (car lst)) (cons (car lst)         
                             (cons (car lst)
                                   (stripNegativesDuplicateEvens (cdr lst))))) 
    (else (cons (car lst) (stripNegativesDuplicateEvens (cdr lst))))))

(stripNegativesDuplicateEvens '())
(stripNegativesDuplicateEvens '(-1 -2 -3))
(stripNegativesDuplicateEvens '(1 2 -2 3 -3 4 5 -6))
(stripNegativesDuplicateEvens '(4 2 -2 3 -3 8 5 -6 1 7 9 10))
(stripNegativesDuplicateEvens '(10 -1 9 6 7 8 5 2 3 4 1 0))