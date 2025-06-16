;;
;;Template file for building SMTLIB models of Bosque code
;;

(declare-datatype @Result (par (T) (
    (@Result-err-trgt)
    (@Result-err-other) 
    (@Result-ok (@Result-value T))
)))

(declare-datatypes (
    (@EList-2 2)
    (@EList-3 3)
    (@EList-4 4)
    ) (
        (par (T1 T2) ((@EList-2-mk (_1 T1) (_2 T2))) )
        (par (T1 T2 T3) ((@EList-3-mk (_1 T1) (_2 T2) (_3 T3))) )
        (par (T1 T2 T3 T4) ((@EList-4-mk (_1 T1) (_2 T2) (_3 T3) (_4 T4))) )
    )
)

;;
;; Primitive datatypes 
;;
(declare-datatype None ((none)))
;;Bool is Bool
(define-sort Nat () Int)
;;Int is Int
(define-sort BigNat () Int)
(define-sort BigInt () Int)
(define-sort Float () Real)
(define-sort CString () String)
;;String is String

;;no content -- ;;--ENUM_DECLS--;;
;;no content -- ;;--TYPEDECL_DECLS--;;

;;
;; Entity datatypes 
;;
(declare-datatypes (
    ;;no content -- ;;--SPECIAL_DECLS--;;
    ;;no content -- ;;--COLLECTION_DECLS--;;
    (Main@Nats 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Nats-mk (Main@Nats-a Nat) (Main@Nats-b Nat) (Main@Nats-c BigNat) (Main@Nats-d BigNat)))
        ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;;
        (
            (@Term-mk-None)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--SPECIAL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

;;no content -- ;;--SUBTYPE_PREDICATES--;;

(declare-const Nat@zero Nat) (declare-const Nat@zero-cc-temp (@Result Nat))
(declare-const Nat@one Nat) (declare-const Nat@one-cc-temp (@Result Nat))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((n Main@Nats)) (@Result Main@Nats)
    (ite (not (or (not (= (Main@Nats-a n) 55)) (not (= (Main@Nats-b n) 0)) (not (= (Main@Nats-c n) 375)) (not (= (Main@Nats-d n) 222)))) (as @Result-err-other (@Result Main@Nats))
        (@Result-ok n)
    )
)

(assert (= Nat@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Nat@zero-cc-temp))
(assert (= Nat@zero (@Result-value Nat@zero-cc-temp)))
(assert (= Nat@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Nat@one-cc-temp))
(assert (= Nat@one (@Result-value Nat@one-cc-temp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const nm Main@Nats)
(declare-const res (@Result Main@Nats))
(assert (= res (Main@main nm)))

(assert (= res (as @Result-err-other (@Result Main@Nats))))
(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
