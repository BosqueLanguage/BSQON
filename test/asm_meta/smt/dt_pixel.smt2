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
    (Main@Pixel 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Pixel-mk (Main@Pixel-r Int) (Main@Pixel-g Int) (Main@Pixel-b Int) (Main@Pixel-a Int)))
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

(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((pix Main@Pixel)) (@Result Int)
    (ite (not (or (not (= (Main@Pixel-r pix) 250)) (not (= (Main@Pixel-g pix) 0)) (not (= (Main@Pixel-b pix) 120)) (not (= (Main@Pixel-a pix) 245)))) (as @Result-err-other (@Result Int))
        (@Result-ok 0)
    )
)

(assert (= Int@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Int@zero-cc-temp))
(assert (= Int@zero (@Result-value Int@zero-cc-temp)))
(assert (= Int@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Int@one-cc-temp))
(assert (= Int@one (@Result-value Int@one-cc-temp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const px Main@Pixel)
(declare-const res (@Result Int))
(assert (= res (Main@main px)))

(assert (= res (as @Result-err-other (@Result Int))))
(check-sat)
(get-model)

