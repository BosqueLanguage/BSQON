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
    (List<Int> 0)
    ;;no content -- ;;--ENTITY_DECLS--;;
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ((List<Int>-mk (List<Int>-value (Seq Int))))
        ;;no content -- ;;--ENTITY_CONSTRUCTORS--;;
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

(declare-const Nat@zero Nat) (declare-const Nat@zero-cc-temp (@Result Nat))
(declare-const Nat@one Nat) (declare-const Nat@one-cc-temp (@Result Nat))
(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Core@ListOps@s_list_create_empty<Int> () List<Int> (List<Int>-mk (as seq.empty (Seq Int))))

(define-fun Core@ListOps@s_list_get<Int> ((l List<Int>) (idx Nat)) Int (seq.nth (List<Int>-value l) idx))

(define-fun Core@ListOps@s_list_push_back<Int> ((l List<Int>) (v Int)) List<Int> (List<Int>-mk (seq.++ (List<Int>-value l) (seq.unit v))))

(define-fun Core@ListOps@s_list_size<Int> ((l List<Int>)) Nat (seq.len (List<Int>-value l)))

(define-fun List<Int>@get ((this List<Int>) (i Nat)) Int
    (Core@ListOps@s_list_get<Int> this i)
)

(define-fun List<Int>@size ((this List<Int>)) Nat
    (Core@ListOps@s_list_size<Int> this)
)

(define-fun List<Int>@get_$_precond0 ((this List<Int>) (i Nat)) Bool
    (< i (List<Int>@size this))
)

(define-fun Main@main ((f List<Int>)) (@Result List<Int>)
    (let ((l (List<Int>-mk (seq.++ (seq.unit 3) (seq.unit 5) (seq.unit 7)))))
        (ite (not (= (let ((@tmp-4-0 (let ((@tmp-1-0 (ite (not (List<Int>@get_$_precond0 f 0)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 0))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-0)) (let ((@tmp-1-1 (ite (not (List<Int>@get_$_precond0 l 0)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 0))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (not (= (@Result-value @tmp-1-0) (@Result-value @tmp-1-1)))))))))) (ite (not (is-@Result-ok @tmp-4-0)) @tmp-4-0 (let ((@tmp-4-1 (let ((@tmp-2-0 (ite (not (List<Int>@get_$_precond0 f 1)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 1))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-2-0)) (let ((@tmp-2-1 (ite (not (List<Int>@get_$_precond0 l 1)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 1))))) (ite (not (is-@Result-ok @tmp-2-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-2-1)) (@Result-ok (not (= (@Result-value @tmp-2-0) (@Result-value @tmp-2-1)))))))))) (ite (not (is-@Result-ok @tmp-4-1)) @tmp-4-1 (let ((@tmp-4-2 (let ((@tmp-3-0 (ite (not (List<Int>@get_$_precond0 f 2)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 2))))) (ite (not (is-@Result-ok @tmp-3-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-3-0)) (let ((@tmp-3-1 (ite (not (List<Int>@get_$_precond0 l 2)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 2))))) (ite (not (is-@Result-ok @tmp-3-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-3-1)) (@Result-ok (not (= (@Result-value @tmp-3-0) (@Result-value @tmp-3-1)))))))))) (ite (not (is-@Result-ok @tmp-4-2)) @tmp-4-2 (@Result-ok (and (@Result-value @tmp-4-0) (@Result-value @tmp-4-1) (@Result-value @tmp-4-2))))))))) (@Result-ok true))) (let ((@failure (let ((@tmp-4-0 (let ((@tmp-1-0 (ite (not (List<Int>@get_$_precond0 f 0)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 0))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-0)) (let ((@tmp-1-1 (ite (not (List<Int>@get_$_precond0 l 0)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 0))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (not (= (@Result-value @tmp-1-0) (@Result-value @tmp-1-1)))))))))) (ite (not (is-@Result-ok @tmp-4-0)) @tmp-4-0 (let ((@tmp-4-1 (let ((@tmp-2-0 (ite (not (List<Int>@get_$_precond0 f 1)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 1))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-2-0)) (let ((@tmp-2-1 (ite (not (List<Int>@get_$_precond0 l 1)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 1))))) (ite (not (is-@Result-ok @tmp-2-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-2-1)) (@Result-ok (not (= (@Result-value @tmp-2-0) (@Result-value @tmp-2-1)))))))))) (ite (not (is-@Result-ok @tmp-4-1)) @tmp-4-1 (let ((@tmp-4-2 (let ((@tmp-3-0 (ite (not (List<Int>@get_$_precond0 f 2)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get f 2))))) (ite (not (is-@Result-ok @tmp-3-0)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-3-0)) (let ((@tmp-3-1 (ite (not (List<Int>@get_$_precond0 l 2)) ((as @Result-err (@Result Int)) @err-other) (@Result-ok (List<Int>@get l 2))))) (ite (not (is-@Result-ok @tmp-3-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-3-1)) (@Result-ok (not (= (@Result-value @tmp-3-0) (@Result-value @tmp-3-1)))))))))) (ite (not (is-@Result-ok @tmp-4-2)) @tmp-4-2 (@Result-ok (and (@Result-value @tmp-4-0) (@Result-value @tmp-4-1) (@Result-value @tmp-4-2))))))))))) (ite (= @failure (@Result-ok false)) ((as @Result-err (@Result List<Int>)) @err-other) ((as @Result-err (@Result List<Int>)) (@Result-etag @failure))))
            (@Result-ok f)
        )
    )
)

(assert (= Nat@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Nat@zero-cc-temp))
(assert (= Nat@zero (@Result-value Nat@zero-cc-temp)))
(assert (= Nat@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Nat@one-cc-temp))
(assert (= Nat@one (@Result-value Nat@one-cc-temp)))
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

(define-fun @Validate-List<Int> ((v List<Int>)) Bool
    (and (<= (seq.len (List<Int>-value v)) SMV_LIST_SIZE_MAX) (and (< 0 (seq.len (List<Int>-value v))) (@Validate-Int (seq.nth (List<Int>-value v) 0))) (and (< 1 (seq.len (List<Int>-value v))) (@Validate-Int (seq.nth (List<Int>-value v) 1))) (and (< 2 (seq.len (List<Int>-value v))) (@Validate-Int (seq.nth (List<Int>-value v) 2))))
)
(define-fun @ValidateRoot-List<Int> ((v List<Int>)) Bool
    (@Validate-List<Int> v)
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const foo List<Int>)
(declare-const res (@Result List<Int>))

(assert (= res (Main@main foo)))
(assert (= res ((as @Result-err (@Result List<Int>)) @err-other)))

(check-sat)
(get-model)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

