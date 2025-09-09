(declare-const x Int)
(declare-const y Int)
(declare-const z Int)

(assert (> x y))
(assert (< (+ x z) y))

(check-sat)
(get-model)
