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
    (ListOps@Vector0<Int> 0)
    (ListOps@Vector1<Int> 0)
    (ListOps@Vector2<Int> 0)
    (ListOps@Vector3<Int> 0)
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ((List<Int>-mk (List<Int>-value @Term)))
        ;;no content -- ;;--ENTITY_CONSTRUCTORS--;;
        (ListOps@Vector0<Int>-mk)
        ((ListOps@Vector1<Int>-mk (ListOps@Vector1<Int>-i0 Int)))
        ((ListOps@Vector2<Int>-mk (ListOps@Vector2<Int>-i0 Int) (ListOps@Vector2<Int>-i1 Int)))
        ((ListOps@Vector3<Int>-mk (ListOps@Vector3<Int>-i0 Int) (ListOps@Vector3<Int>-i1 Int) (ListOps@Vector3<Int>-i2 Int)))
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--SPECIAL_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            (@Term-ListOps@Vector0<Int>-mk (@Term-ListOps@Vector0<Int>-value ListOps@Vector0<Int>))
            (@Term-ListOps@Vector1<Int>-mk (@Term-ListOps@Vector1<Int>-value ListOps@Vector1<Int>))
            (@Term-ListOps@Vector2<Int>-mk (@Term-ListOps@Vector2<Int>-value ListOps@Vector2<Int>))
            (@Term-ListOps@Vector3<Int>-mk (@Term-ListOps@Vector3<Int>-value ListOps@Vector3<Int>))
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

(define-fun @SubtypeOf-ListOps@Vector<Int>((tt @Term)) Bool (or (is-@Term-ListOps@Vector0<Int>-mk tt) (is-@Term-ListOps@Vector1<Int>-mk tt) (is-@Term-ListOps@Vector2<Int>-mk tt) (is-@Term-ListOps@Vector3<Int>-mk tt)))

(declare-const Nat@zero Nat) (declare-const Nat@zero-cc-temp (@Result Nat))
(declare-const Nat@one Nat) (declare-const Nat@one-cc-temp (@Result Nat))
(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Core@XCore@s_blockingfailure<List<Int>> () (@Result List<Int>) ((as @Result-err (@Result List<Int>)) @err-other))

(define-fun Core@ListOps@s_list_create_empty<Int> () List<Int>
    (List<Int>-mk (@Term-ListOps@Vector0<Int>-mk ListOps@Vector0<Int>-mk))
)

(define-fun Core@ListOps@s_list_push_back<Int> ((l List<Int>) (v Int)) (@Result List<Int>)
    (let ((ll (List<Int>-value l)))
        (ite (is-@Term-ListOps@Vector0<Int>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Int>-value ll)))
            (@Result-ok (List<Int>-mk (@Term-ListOps@Vector1<Int>-mk (ListOps@Vector1<Int>-mk v))))
        )
        (ite (is-@Term-ListOps@Vector1<Int>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Int>-value ll)))
            (@Result-ok (List<Int>-mk (@Term-ListOps@Vector2<Int>-mk (ListOps@Vector2<Int>-mk (ListOps@Vector1<Int>-i0 $ll) v))))
        )
        (ite (is-@Term-ListOps@Vector2<Int>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Int>-value ll)))
            (@Result-ok (List<Int>-mk (@Term-ListOps@Vector3<Int>-mk (ListOps@Vector3<Int>-mk (ListOps@Vector2<Int>-i0 $ll) (ListOps@Vector2<Int>-i1 $ll) v))))
        )
                (let (($ll (@Term-ListOps@Vector3<Int>-value ll)))
            Core@XCore@s_blockingfailure<List<Int>>
        ))))
    )
)

(define-fun Core@ListOps@s_list_size<Int> ((l List<Int>)) Nat
    (let ((ll (List<Int>-value l)))
        (ite (is-@Term-ListOps@Vector0<Int>-mk ll)
        0
        (ite (is-@Term-ListOps@Vector1<Int>-mk ll)
        1
        (ite (is-@Term-ListOps@Vector2<Int>-mk ll)
        2
                3)))
    )
)

(define-fun List<Int>@size ((this List<Int>)) Nat
    (Core@ListOps@s_list_size<Int> this)
)

(define-fun Main@main ((f List<Int>)) (@Result List<Int>)
    (ite (not (not (= (List<Int>@size f) 0))) ((as @Result-err (@Result List<Int>)) @err-other)
        (@Result-ok f)
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

(define-fun @Validate-None ((v None)) Bool true)
(define-fun @Validate-Bool ((v Bool)) Bool true)
(define-fun @Validate-Nat ((v Nat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Int ((v Int)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigNat ((v BigNat)) Bool (and (<= 0 v) (<= v SMV_I_RANGE)))
(define-fun @Validate-BigInt ((v BigInt)) Bool (and (<= (- SMV_I_RANGE) v) (<= v SMV_I_RANGE)))
(define-fun @Validate-Float ((v Float)) Bool (and (<= -1024.0 v) (<= v 1024.0)))
(define-fun @Validate-CString ((v CString)) Bool (and (<= (str.len v) SMV_STR_LENGTH) (str.in.re v (re.* (re.union (str.to.re "\u{9}") (re.range " " "~"))))))
(define-fun @Validate-String ((v String)) Bool (<= (str.len v) SMV_STR_LENGTH))

(define-fun @Validate-ListOps@Vector3<Int> ((v ListOps@Vector3<Int>)) Bool
    (and (@Validate-Int (ListOps@Vector3<Int>-i0 v)) (@Validate-Int (ListOps@Vector3<Int>-i1 v)) (@Validate-Int (ListOps@Vector3<Int>-i2 v)))
)
(define-fun @Validate-ListOps@Vector2<Int> ((v ListOps@Vector2<Int>)) Bool
    (and (@Validate-Int (ListOps@Vector2<Int>-i0 v)) (@Validate-Int (ListOps@Vector2<Int>-i1 v)))
)
(define-fun @Validate-ListOps@Vector1<Int> ((v ListOps@Vector1<Int>)) Bool
    (@Validate-Int (ListOps@Vector1<Int>-i0 v))
)
(define-fun @Validate-ListOps@Vector0<Int> ((v ListOps@Vector0<Int>)) Bool
    true
)
(define-fun @Validate-ListOps@Vector<Int> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Int>-mk v)
    (@Validate-ListOps@Vector0<Int> (@Term-ListOps@Vector0<Int>-value v))
    (ite (is-@Term-ListOps@Vector1<Int>-mk v)
    (@Validate-ListOps@Vector1<Int> (@Term-ListOps@Vector1<Int>-value v))
    (ite (is-@Term-ListOps@Vector2<Int>-mk v)
    (@Validate-ListOps@Vector2<Int> (@Term-ListOps@Vector2<Int>-value v))
    (ite (is-@Term-ListOps@Vector3<Int>-mk v)
    (@Validate-ListOps@Vector3<Int> (@Term-ListOps@Vector3<Int>-value v))
        false))))
)
(define-fun @Validate-List<Int> ((v List<Int>)) Bool
    (@Validate-ListOps@Vector<Int> (List<Int>-value v))
)
(define-fun @ValidateRoot-ListOps@Vector3<Int> ((v ListOps@Vector3<Int>)) Bool
    (@Validate-ListOps@Vector3<Int> v)
)
(define-fun @ValidateRoot-ListOps@Vector2<Int> ((v ListOps@Vector2<Int>)) Bool
    (@Validate-ListOps@Vector2<Int> v)
)
(define-fun @ValidateRoot-ListOps@Vector1<Int> ((v ListOps@Vector1<Int>)) Bool
    (@Validate-ListOps@Vector1<Int> v)
)
(define-fun @ValidateRoot-ListOps@Vector0<Int> ((v ListOps@Vector0<Int>)) Bool
    (@Validate-ListOps@Vector0<Int> v)
)
(define-fun @ValidateRoot-ListOps@Vector<Int> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Int>-mk v)
    (@Validate-ListOps@Vector0<Int> (@Term-ListOps@Vector0<Int>-value v))
    (ite (is-@Term-ListOps@Vector1<Int>-mk v)
    (@Validate-ListOps@Vector1<Int> (@Term-ListOps@Vector1<Int>-value v))
    (ite (is-@Term-ListOps@Vector2<Int>-mk v)
    (@Validate-ListOps@Vector2<Int> (@Term-ListOps@Vector2<Int>-value v))
    (ite (is-@Term-ListOps@Vector3<Int>-mk v)
    (@Validate-ListOps@Vector3<Int> (@Term-ListOps@Vector3<Int>-value v))
        false))))
)
(define-fun @ValidateRoot-List<Int> ((v List<Int>)) Bool
    (@Validate-List<Int> v)
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const f List<Int>)
(assert(@ValidateRoot-List<Int> f))
(declare-const res (@Result List<Int>))

(assert (= res (Main@main f)))
(assert (= res ((as @Result-err (@Result List<Int>)) @err-other)))

(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

