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
(declare-datatype Main@EMAIL ( (Main@EMAIL-mk (Main@EMAIL-value CString)) ))
(declare-datatype Main@ALPHANUMERIC ( (Main@ALPHANUMERIC-mk (Main@ALPHANUMERIC-value CString)) ))
(declare-datatype Main@USER_NAME ( (Main@USER_NAME-mk (Main@USER_NAME-value CString)) ))

;;
;; Entity datatypes 
;;
(declare-datatypes (
    (Some<Main@USER_NAME> 0)
    (Some<Main@EMAIL> 0)
    ;;no content -- ;;--COLLECTION_DECLS--;;
    (Main@UserDTO 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ((Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-value Main@USER_NAME)))
        ((Some<Main@EMAIL>-mk (Some<Main@EMAIL>-value Main@EMAIL)))
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@UserDTO-mk (Main@UserDTO-email @Term) (Main@UserDTO-password Main@ALPHANUMERIC) (Main@UserDTO-name @Term)))
        ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;;
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            (@Term-Some<Main@USER_NAME>-mk (@Term-Some<Main@USER_NAME>-value Some<Main@USER_NAME>))
            (@Term-Some<Main@EMAIL>-mk (@Term-Some<Main@EMAIL>-value Some<Main@EMAIL>))
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

(define-fun @SubtypeOf-Option<Main@USER_NAME>((tt @Term)) Bool (or (is-@Term-Some<Main@USER_NAME>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<Main@EMAIL>((tt @Term)) Bool (or (is-@Term-Some<Main@EMAIL>-mk tt) (= tt @Term-None-mk)))

;;no content -- ;;--GLOBAL_DECLS--;;

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Main@main ((user Main@UserDTO)) (@Result Main@UserDTO)
    (ite (not (or (or (= (Main@UserDTO-name user) @Term-None-mk) (not (= (Some<Main@USER_NAME>-value (@Term-Some<Main@USER_NAME>-value (Main@UserDTO-name user))) (Main@USER_NAME-mk "James Chen")))) (or (= (Main@UserDTO-email user) @Term-None-mk) (not (= (Some<Main@EMAIL>-value (@Term-Some<Main@EMAIL>-value (Main@UserDTO-email user))) (Main@EMAIL-mk "jch270@uky.edu")))) (not (= (Main@UserDTO-password user) (Main@ALPHANUMERIC-mk "12345678"))))) ((as @Result-err (@Result Main@UserDTO)) @err-other)
        (@Result-ok user)
    )
)

;;no content -- ;;--GLOBAL_IMPLS--;;

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

(define-fun @Validate-Main@ALPHANUMERIC ((v Main@ALPHANUMERIC)) Bool
    (and (@Validate-CString (Main@ALPHANUMERIC-value v)) (str.in.re (Main@ALPHANUMERIC-value v) (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9")))))
)
(define-fun @Validate-Main@EMAIL ((v Main@EMAIL)) Bool
    (and (@Validate-CString (Main@EMAIL-value v)) (str.in.re (Main@EMAIL-value v) (re.++ (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_"))) (re.* (re.++ (str.to.re ".") (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_")))) (str.to.re "@") (re.+ (re.++ (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_"))) (str.to.re "."))) (re.+ (re.union (re.range "a" "z") (re.range "A" "Z"))))))
)
(define-fun @Validate-Main@USER_NAME ((v Main@USER_NAME)) Bool
    (and (@Validate-CString (Main@USER_NAME-value v)) (str.in.re (Main@USER_NAME-value v) ((_ re.loop 1 50) re.allchar)))
)
(define-fun @Validate-Some<Main@USER_NAME> ((v Some<Main@USER_NAME>)) Bool
    (@Validate-Main@USER_NAME (Some<Main@USER_NAME>-value v))
)
(define-fun @Validate-Option<Main@USER_NAME> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Main@USER_NAME> (@Term-Some<Main@USER_NAME>-value v)) true)
)
(define-fun @Validate-Some<Main@EMAIL> ((v Some<Main@EMAIL>)) Bool
    (@Validate-Main@EMAIL (Some<Main@EMAIL>-value v))
)
(define-fun @Validate-Option<Main@EMAIL> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Main@EMAIL> (@Term-Some<Main@EMAIL>-value v)) true)
)
(define-fun @Validate-Main@UserDTO ((v Main@UserDTO)) Bool
    (and (@Validate-Option<Main@EMAIL> (Main@UserDTO-email v)) (@Validate-Main@ALPHANUMERIC (Main@UserDTO-password v)) (@Validate-Option<Main@USER_NAME> (Main@UserDTO-name v)))
)
(define-fun @ValidateRoot-Main@ALPHANUMERIC ((v Main@ALPHANUMERIC)) Bool
    (@Validate-Main@ALPHANUMERIC v)
)
(define-fun @ValidateRoot-Main@EMAIL ((v Main@EMAIL)) Bool
    (@Validate-Main@EMAIL v)
)
(define-fun @ValidateRoot-Main@USER_NAME ((v Main@USER_NAME)) Bool
    (@Validate-Main@USER_NAME v)
)
(define-fun @ValidateRoot-Some<Main@USER_NAME> ((v Some<Main@USER_NAME>)) Bool
    (@Validate-Some<Main@USER_NAME> v)
)
(define-fun @ValidateRoot-Option<Main@USER_NAME> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Main@USER_NAME> (@Term-Some<Main@USER_NAME>-value v)) true)
)
(define-fun @ValidateRoot-Some<Main@EMAIL> ((v Some<Main@EMAIL>)) Bool
    (@Validate-Some<Main@EMAIL> v)
)
(define-fun @ValidateRoot-Option<Main@EMAIL> ((v @Term)) Bool
    (ite (not (= v @Term-None-mk)) (@Validate-Some<Main@EMAIL> (@Term-Some<Main@EMAIL>-value v)) true)
)
(define-fun @ValidateRoot-Main@UserDTO ((v Main@UserDTO)) Bool
    (@Validate-Main@UserDTO v)
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare-const user Main@UserDTO)
(declare-const res (@Result Main@UserDTO))

(assert (= res (Main@main user)))
(assert (= res ((as @Result-err (@Result Main@UserDTO)) @err-other)))

(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

