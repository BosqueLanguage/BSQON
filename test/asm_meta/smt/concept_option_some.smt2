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
    (Some<Int> 0)
    ;;no content -- ;;--COLLECTION_DECLS--;;
    ;;no content -- ;;--ENTITY_DECLS--;;
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ((Some<Int>-mk (Some<Int>-value Int)))
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ;;no content -- ;;--ENTITY_CONSTRUCTORS--;;
        ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;;
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            (@Term-Some<Int>-mk (@Term-Some<Int>-value Some<Int>))
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

(define-fun @SubtypeOf-Option<Int>((tt @Term)) Bool (or (is-@Term-Some<Int>-mk tt) (= tt @Term-None-mk)))

(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((f @Term)) (@Result @Term)
    (let ((x (@Term-Some<Int>-mk (Some<Int>-mk 3))))
        (ite (not (= (let ((@tmp-1-1 (ite (not (not (= f @Term-None-mk))) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (Some<Int>-value (@Term-Some<Int>-value f)))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (or (= x @Term-None-mk) (not (= (Some<Int>-value (@Term-Some<Int>-value x)) (@Result-value @tmp-1-1))))))) (@Result-ok true))) (let ((@failure (let ((@tmp-1-1 (ite (not (not (= f @Term-None-mk))) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (Some<Int>-value (@Term-Some<Int>-value f)))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (or (= x @Term-None-mk) (not (= (Some<Int>-value (@Term-Some<Int>-value x)) (@Result-value @tmp-1-1))))))))) (ite (= @failure (@Result-ok false)) ((as @Result-err (@Result @Term)) @err-other) ((as @Result-err (@Result @Term)) (@Result-etag @failure))))
            (@Result-ok f)
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
(declare-const SMV_LIST_SIZE_MAX Int) (assert (<= SMV_LIST_SIZE_MAX 3))

(define-fun @Validate-None ((v None)) Bool true)
(define-fun @Validate-Bool ((v Bool)) Bool true)
(define-fun @Validate-Nat ((v Nat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Int ((v Int)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigNat ((v BigNat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigInt ((v BigInt)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Float ((v Float)) Bool (and (<= -1024.0 v) (<= v 1024.0)))
(define-fun @Validate-CString ((v CString)) Bool (and (<= (str.len v) SMV_STR_LENGTH) (str.in.re v (re.* (re.union (str.to.re "\u{9}") (re.range " " "~"))))))
(define-fun @Validate-String ((v String)) Bool (<= (str.len v) SMV_STR_LENGTH))

(define-fun @Validate-Some<Int> ((v Some<Int>)) Bool
    (@Validate-Int (Some<Int>-value v))
)
(define-fun @Validate-Option<Int> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Int> (@Term-Some<Int>-value v)) true)
)
(define-fun @ValidateRoot-Some<Int> ((v Some<Int>)) Bool
    (@Validate-Some<Int> v)
)
(define-fun @ValidateRoot-Option<Int> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Int> (@Term-Some<Int>-value v)) true)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const f @Term)
(declare-const res (@Result @Term))

(assert (= res (Main@main f)))
(assert (= res ((as @Result-err (@Result @Term)) @err-other)))

(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

