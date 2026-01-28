;;
;;Template file for building SMTLIB models of Bosque code
;;

(declare-datatypes () ((@ErrTag @err-trgt @err-other)))

(declare-datatype @Result (par (T) (
    (@Result-err (@Result-etag @ErrTag)) 
    (@Result-ok (@Result-value T))
)))

(declare-datatypes (
    (@EList-2 2)
    (@EList-3 3)
    (@EList-4 4)
    ) (
        (par (T1 T2) ((@EList-2-mk (@EList-2-0 T1) (@EList-2-1 T2))) )
        (par (T1 T2 T3) ((@EList-3-mk (@EList-3-0 T1) (@EList-3-1 T2) (@EList-3-2 T3))) )
        (par (T1 T2 T3 T4) ((@EList-4-mk (@EList-4-0 T1) (@EList-4-1 T2) (@EList-4-2 T3) (@EList-4-3 T4))) )
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
    (Main@Foo 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Foo-mk (Main@Foo-x Int) (Main@Foo-y Int)))
        ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;;
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--SPECIAL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

;;no content -- ;;--SUBTYPE_PREDICATES--;;

(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((f Main@Foo)) (@Result Int)
    (let ((k (+ (Main@Foo-x f) (Main@Foo-y f))))
        (ite (not (> k (Main@Foo-x f))) ((as @Result-err (@Result Int)) @err-other)
            (@Result-ok k)
        )
    )
)

(assert (= Int@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Int@zero-cc-temp))
(assert (= Int@zero (@Result-value Int@zero-cc-temp)))
(assert (= Int@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Int@one-cc-temp))
(assert (= Int@one (@Result-value Int@one-cc-temp)))

(declare-const SMV_I_RANGE Int) (assert (= SMV_I_RANGE 32))
(declare-const SMV_STR_LENGTH Int) (assert (= SMV_STR_LENGTH 16))

(define-fun @Validate-None ((v None)) Bool true)
(define-fun @Validate-Bool ((v Bool)) Bool true)
(define-fun @Validate-Nat ((v Nat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Int ((v Int)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigNat ((v BigNat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigInt ((v BigInt)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Float ((v Float)) Bool (and (<= -1024.0 v) (<= v 1024.0)))
(define-fun @Validate-CString ((v CString)) Bool (and (<= (str.len v) SMV_STR_LENGTH) (str.in.re v (re.* (re.union (str.to.re "\u{9}") (re.range " " "~"))))))
(define-fun @Validate-String ((v String)) Bool (<= (str.len v) SMV_STR_LENGTH))

(define-fun @Validate-Main@Foo ((v Main@Foo)) Bool
    (and (@Validate-Int (Main@Foo-x v)) (@Validate-Int (Main@Foo-y v)))
)
(define-fun @ValidateRoot-Main@Foo ((v Main@Foo)) Bool
    (@Validate-Main@Foo v)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;Above is SMTLIB code generated from the Bosque Code
;;Below is the setup for checking for an error -- if we can trigger 
;;then error then the entire formula is satisfiable and we want get the value 
;;for the argument "f"

(declare-const f Main@Foo)
(assert (@ValidateRoot-Main@Foo f))
(declare-const res (@Result Int))
(assert (= res (Main@main f)))

(assert (= res ((as @Result-err (@Result Int)) @err-other)))

(check-sat)
(get-model)
