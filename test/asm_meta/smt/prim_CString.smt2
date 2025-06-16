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
    (Main@Man 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@Man-mk (Main@Man-a CString)))
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

;;no content -- ;;--GLOBAL_DECLS--;;

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((s Main@Man)) (@Result CString)
    (let ((t "Manchester"))
        (ite (not (not (= (Main@Man-a s) t))) (as @Result-err-other (@Result CString))
            (@Result-ok (Main@Man-a s))
        )
    )
)

;;no content -- ;;--GLOBAL_IMPLS--;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const cs Main@Man)
(declare-const res (@Result String)) 
(assert (= res (Main@main cs)))

(assert (= res (as @Result-err-other (@Result String))))
(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

