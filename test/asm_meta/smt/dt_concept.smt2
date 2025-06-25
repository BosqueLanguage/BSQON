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
    (Main@Prim 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ((Some<Int>-mk (Some<Int>-value Int)))
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Prim-mk (Main@Prim-a Int) (Main@Prim-b BigNat) (Main@Prim-c CString) (Main@Prim-d CString) (Main@Prim-e Bool) (Main@Prim-f @Term)))
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

(define-fun Main@main ((p Main@Prim)) (@Result Main@Prim)
    (ite (not (= (let ((@tmp-2-5 (let ((@tmp-1-0 (ite (not (not (= (Main@Prim-f p) @Term-mk-None))) (as @Result-err-other (@Result Int)) (@Result-ok (Some<Int>-value (@Term-Some<Int>-value (Main@Prim-f p))))))) (ite (not (is-@Result-ok @tmp-1-0)) (ite (= @tmp-1-0 (as @Result-err-trgt (@Result Int))) (as @Result-err-trgt (@Result Bool)) (as @Result-err-other (@Result Bool))) (@Result-ok (not (= (@Result-value @tmp-1-0) 3))))))) (ite (not (is-@Result-ok @tmp-2-5)) @tmp-2-5 (@Result-ok (or (not (= (Main@Prim-a p) -52)) (not (= (Main@Prim-b p) 77)) (not (= (Main@Prim-c p) "Sphinx of black quartz, judge my vow.")) (not (= (Main@Prim-d p) "Mr. Jock, TV quiz PhD, bags few lynx.")) (not (= (Main@Prim-e p) false)) (@Result-value @tmp-2-5))))) (@Result-ok true))) (let ((@failure (let ((@tmp-2-5 (let ((@tmp-1-0 (ite (not (not (= (Main@Prim-f p) @Term-mk-None))) (as @Result-err-other (@Result Int)) (@Result-ok (Some<Int>-value (@Term-Some<Int>-value (Main@Prim-f p))))))) (ite (not (is-@Result-ok @tmp-1-0)) (ite (= @tmp-1-0 (as @Result-err-trgt (@Result Int))) (as @Result-err-trgt (@Result Bool)) (as @Result-err-other (@Result Bool))) (@Result-ok (not (= (@Result-value @tmp-1-0) 3))))))) (ite (not (is-@Result-ok @tmp-2-5)) @tmp-2-5 (@Result-ok (or (not (= (Main@Prim-a p) -52)) (not (= (Main@Prim-b p) 77)) (not (= (Main@Prim-c p) "Sphinx of black quartz, judge my vow.")) (not (= (Main@Prim-d p) "Mr. Jock, TV quiz PhD, bags few lynx.")) (not (= (Main@Prim-e p) false)) (@Result-value @tmp-2-5))))))) (ite (= @failure (@Result-ok false)) (as @Result-err-other (@Result Main@Prim)) (ite (= @failure (as @Result-err-trgt (@Result Bool))) (as @Result-err-trgt (@Result Main@Prim)) (as @Result-err-other (@Result Main@Prim)))))
        (@Result-ok p)
    )
)

(assert (= Int@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Int@zero-cc-temp))
(assert (= Int@zero (@Result-value Int@zero-cc-temp)))
(assert (= Int@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Int@one-cc-temp))
(assert (= Int@one (@Result-value Int@one-cc-temp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const p Main@Prim)
(declare-const res (@Result Main@Prim))
(assert (= res (Main@main p)))

(assert (= res (as @Result-err-other (@Result Main@Prim))))
(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
