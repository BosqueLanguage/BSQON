;;
;;Template file for building SMTLIB models of Bosque code
;;

(declare-datatype @Result (par (T) (
    (@Result-err-trgt)
    (@Result-err-other) 
    (@Result-ok (@Result-value T))
)))

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
    (Main@Foo 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Foo-mk (Main@Foo-x Int) (Main@Foo-y Int)))
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

;;NLA options
(declare-fun @NLA_I_mult (Int Int) Int)
(declare-fun @NLA_I_div (Int Int) Int)

(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((f Main@Foo)) (@Result Int)
    (let ((k (+ (Main@Foo-x f) (Main@Foo-y f))))
        (ite (not (> k (Main@Foo-x f))) (as @Result-err-other (@Result Int))
            (@Result-ok k)
        )
    )
)

(assert (= Int@zero-cc-temp (@Result-ok 0)))
(assert ((_ is @Result-ok) Int@zero-cc-temp))
(assert (= Int@zero (@Result-value Int@zero-cc-temp)))
(assert (= Int@one-cc-temp (@Result-ok 1)))
(assert ((_ is @Result-ok) Int@one-cc-temp))
(assert (= Int@one (@Result-value Int@one-cc-temp)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;Above is SMTLIB code generated from the Bosque Code
;;Below is the setup for checking for an error -- if we can trigger 
;;then error then the entire formula is satisfiable and we want get the value 
;;for the argument "f"

(declare-const f Main@Foo)
(declare-const res (@Result Int))
(assert (= res (Main@main f)))

(assert (= res @Result-err-other))

(check-sat)
(get-model)
