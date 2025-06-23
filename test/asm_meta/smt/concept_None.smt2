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
            (@Term-mk-None)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            (@Term-Some<Int>-mk (@Term-Some<Int>-value Some<Int>))
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

(define-fun @SubtypeOf-Option<Int>((tt @Term)) Bool (or (is-@Term-Some<Int>-mk tt) (= tt @Term-mk-None)))

(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((f @Term)) (@Result @Term)
        (let ((@tmp-2-0 (let ((@tmp-1-0 (ite (not (not (= f @Term-mk-None))) (as @Result-err-other (@Result Int)) (@Result-ok (Some<Int>-value (@Term-Some<Int>-value f)))))) (ite (not (is-@Result-ok @tmp-1-0)) (ite (= @tmp-1-0 (as @Result-err-trgt (@Result Int))) (as @Result-err-trgt (@Result Bool)) (as @Result-err-other (@Result Bool))) (@Result-ok (= (@Result-value @tmp-1-0) 3)))))) (ite (not (is-@Result-ok @tmp-2-0)) (ite (= @tmp-2-0 (as @Result-err-trgt (@Result Bool))) (as @Result-err-trgt (@Result @Term)) (as @Result-err-other (@Result @Term))) 
        (ite (@Result-value @tmp-2-0)
            (@Result-ok (@Term-Some<Int>-mk (Some<Int>-mk 0)))
            (@Result-ok f)
        )
    ))
)

(assert (= Int@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Int@zero-cc-temp))
(assert (= Int@zero (@Result-value Int@zero-cc-temp)))
(assert (= Int@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Int@one-cc-temp))
(assert (= Int@one (@Result-value Int@one-cc-temp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const f @Term)
(declare-const res (@Result @Term))
(assert (= res (Main@main f)))

(assert (= res (as @Result-err-other (@Result @Term))))

(check-sat)
(get-model)


