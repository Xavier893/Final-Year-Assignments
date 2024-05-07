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
(stripNegativesDuplicateEvens '(-6 1))
(stripNegativesDuplicateEvens '(6 8))
(stripNegativesDuplicateEvens '(4 -1 9))
(stripNegativesDuplicateEvens '(10 -1 9 6 7 8 5 2 3 4 1 0))