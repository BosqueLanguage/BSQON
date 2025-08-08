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
(declare-datatype Main@ALPHANUMERIC ( (Main@ALPHANUMERIC-mk (Main@ALPHANUMERIC-value CString)) ))
(declare-datatype Main@EMAIL ( (Main@EMAIL-mk (Main@EMAIL-value CString)) ))
(declare-datatype Main@USER_PHONE ( (Main@USER_PHONE-mk (Main@USER_PHONE-value CString)) ))
(declare-datatype Main@USER_NAME ( (Main@USER_NAME-mk (Main@USER_NAME-value CString)) ))
(declare-datatype Main@USER_ADDRESS ( (Main@USER_ADDRESS-mk (Main@USER_ADDRESS-value CString)) ))

;;
;; Entity datatypes 
;;
(declare-datatypes (
    (Some<Main@EMAIL> 0)
    (Some<Main@USER_NAME> 0)
    (Some<Main@USER_PHONE> 0)
    (Some<Main@USER_ADDRESS> 0)
    ;;no content -- ;;--COLLECTION_DECLS--;;
    (Main@UserDTO 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ((Some<Main@EMAIL>-mk (Some<Main@EMAIL>-value Main@EMAIL)))
        ((Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-value Main@USER_NAME)))
        ((Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-value Main@USER_PHONE)))
        ((Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-value Main@USER_ADDRESS)))
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@UserDTO-mk (Main@UserDTO-email @Term) (Main@UserDTO-password Main@ALPHANUMERIC) (Main@UserDTO-name @Term) (Main@UserDTO-phone @Term) (Main@UserDTO-address @Term) (Main@UserDTO-id Int)))
        ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;;
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            (@Term-Some<Main@EMAIL>-mk (@Term-Some<Main@EMAIL>-value Some<Main@EMAIL>))
            (@Term-Some<Main@USER_NAME>-mk (@Term-Some<Main@USER_NAME>-value Some<Main@USER_NAME>))
            (@Term-Some<Main@USER_PHONE>-mk (@Term-Some<Main@USER_PHONE>-value Some<Main@USER_PHONE>))
            (@Term-Some<Main@USER_ADDRESS>-mk (@Term-Some<Main@USER_ADDRESS>-value Some<Main@USER_ADDRESS>))
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            ;;no content -- ;;--DATATYPE_TERM_CONSTRUCTORS--;;
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

(define-fun @SubtypeOf-Option<Main@EMAIL>((tt @Term)) Bool (or (is-@Term-Some<Main@EMAIL>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<Main@USER_NAME>((tt @Term)) Bool (or (is-@Term-Some<Main@USER_NAME>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<Main@USER_PHONE>((tt @Term)) Bool (or (is-@Term-Some<Main@USER_PHONE>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<Main@USER_ADDRESS>((tt @Term)) Bool (or (is-@Term-Some<Main@USER_ADDRESS>-mk tt) (= tt @Term-None-mk)))

(declare-const Nat@zero Nat) (declare-const Nat@zero-cc-temp (@Result Nat))
(declare-const Nat@one Nat) (declare-const Nat@one-cc-temp (@Result Nat))
(declare-const Int@zero Int) (declare-const Int@zero-cc-temp (@Result Int))
(declare-const Int@one Int) (declare-const Int@one-cc-temp (@Result Int))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Core@CStringOps@s_empty ((s CString)) Bool (= s ""))

(define-fun Core@CStringOps@s_size ((s CString)) Nat (str.len s))

(define-fun CString@empty ((this CString)) Bool
    (Core@CStringOps@s_empty this)
)

(define-fun CString@size ((this CString)) Nat
    (Core@CStringOps@s_size this)
)

(define-fun Main@userEmailNotEmpty ((x @Term)) Bool
    (ite (= x @Term-None-mk)
        (let (($x none))
            false
        )
        (ite (not (CString@empty (Main@EMAIL-value (Some<Main@EMAIL>-value (@Term-Some<Main@EMAIL>-value x))))) (<= (CString@size (Main@EMAIL-value (Some<Main@EMAIL>-value (@Term-Some<Main@EMAIL>-value x)))) 50) false)
    )
)

(define-fun Main@UserDTO_$_invariant0 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (Main@userEmailNotEmpty $email)
)

(define-fun Main@UserDTO_$_invariant1 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (ite (>= (CString@size (Main@ALPHANUMERIC-value $password)) 6) (<= (CString@size (Main@ALPHANUMERIC-value $password)) 50) false)
)

(define-fun Main@userNameNotEmpty ((x @Term)) Bool
    (ite (= x @Term-None-mk)
        (let (($x none))
            false
        )
        (not (CString@empty (Main@USER_NAME-value (Some<Main@USER_NAME>-value (@Term-Some<Main@USER_NAME>-value x)))))
    )
)

(define-fun Main@UserDTO_$_invariant2 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (Main@userNameNotEmpty $name)
)

(define-fun Main@userPhoneNotEmpty ((x @Term)) Bool
    (ite (= x @Term-None-mk)
        (let (($x none))
            false
        )
        (ite (not (CString@empty (Main@USER_PHONE-value (Some<Main@USER_PHONE>-value (@Term-Some<Main@USER_PHONE>-value x))))) (<= (CString@size (Main@USER_PHONE-value (Some<Main@USER_PHONE>-value (@Term-Some<Main@USER_PHONE>-value x)))) 20) false)
    )
)

(define-fun Main@UserDTO_$_invariant3 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (Main@userPhoneNotEmpty $phone)
)

(define-fun Main@userAddressNotEmpty ((x @Term)) Bool
    (ite (= x @Term-None-mk)
        (let (($x none))
            false
        )
        (ite (not (CString@empty (Main@USER_ADDRESS-value (Some<Main@USER_ADDRESS>-value (@Term-Some<Main@USER_ADDRESS>-value x))))) (<= (CString@size (Main@USER_ADDRESS-value (Some<Main@USER_ADDRESS>-value (@Term-Some<Main@USER_ADDRESS>-value x)))) 100) false)
    )
)

(define-fun Main@UserDTO_$_invariant4 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (Main@userAddressNotEmpty $address)
)

(define-fun Main@UserDTO_$_invariant5 (($email @Term) ($password Main@ALPHANUMERIC) ($name @Term) ($phone @Term) ($address @Term) ($id Int)) Bool
    (> $id 0)
)

(define-fun Main@getCustomer ((id Int)) (@Result Main@UserDTO)
    (let ((@tmpe-customer_found (ite (not (Main@UserDTO_$_invariant0 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (ite (not (Main@UserDTO_$_invariant1 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (ite (not (Main@UserDTO_$_invariant2 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (ite (not (Main@UserDTO_$_invariant3 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (ite (not (Main@UserDTO_$_invariant4 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (ite (not (Main@UserDTO_$_invariant5 (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other) (@Result-ok (Main@UserDTO-mk (@Term-Some<Main@EMAIL>-mk (Some<Main@EMAIL>-mk (Main@EMAIL-mk "jch270@uky.edu"))) (Main@ALPHANUMERIC-mk "12345678") (@Term-Some<Main@USER_NAME>-mk (Some<Main@USER_NAME>-mk (Main@USER_NAME-mk "James Chen"))) (@Term-Some<Main@USER_PHONE>-mk (Some<Main@USER_PHONE>-mk (Main@USER_PHONE-mk "+12 111-111-1111"))) (@Term-Some<Main@USER_ADDRESS>-mk (Some<Main@USER_ADDRESS>-mk (Main@USER_ADDRESS-mk "123 Fake Street"))) id)))))))))) (ite (not (is-@Result-ok @tmpe-customer_found))  @tmpe-customer_found (let ((customer_found (@Result-value @tmpe-customer_found)))
        (ite (not (= (Main@UserDTO-id customer_found) id)) ((as @Result-err (@Result Main@UserDTO)) @err-other)
            (@Result-ok customer_found)
        )
    )))
)

(define-fun Main@main ((cust_id Int)) (@Result Main@UserDTO)
    (let ((@tmpe-user (Main@getCustomer cust_id))) (ite (not (is-@Result-ok @tmpe-user))  @tmpe-user (let ((user (@Result-value @tmpe-user)))
        (@Result-ok user)
    )))
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

(define-fun @Validate-Main@ALPHANUMERIC ((v Main@ALPHANUMERIC)) Bool
    (and (@Validate-CString (Main@ALPHANUMERIC-value v)) (str.in.re (Main@ALPHANUMERIC-value v) (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9")))))
)
(define-fun @Validate-Main@EMAIL ((v Main@EMAIL)) Bool
    (and (@Validate-CString (Main@EMAIL-value v)) (str.in.re (Main@EMAIL-value v) (re.++ (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_"))) (re.* (re.++ (str.to.re ".") (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_")))) (str.to.re "@") (re.+ (re.++ (re.+ (re.union (re.range "a" "z") (re.range "A" "Z") (re.range "0" "9") (str.to.re "-") (str.to.re "_"))) (str.to.re "."))) (re.+ (re.union (re.range "a" "z") (re.range "A" "Z"))))))
)
(define-fun @Validate-Main@USER_ADDRESS ((v Main@USER_ADDRESS)) Bool
    (and (@Validate-CString (Main@USER_ADDRESS-value v)) (str.in.re (Main@USER_ADDRESS-value v) (re.+ (re.union (re.range "0" "9") (re.range "a" "z") (re.range "A" "Z") (re.range " " ",") (str.to.re ".") (str.to.re "#") (str.to.re "(") (str.to.re ")")))))
)
(define-fun @Validate-Main@USER_NAME ((v Main@USER_NAME)) Bool
    (and (@Validate-CString (Main@USER_NAME-value v)) (str.in.re (Main@USER_NAME-value v) ((_ re.loop 1 50) re.allchar)))
)
(define-fun @Validate-Main@USER_PHONE ((v Main@USER_PHONE)) Bool
    (and (@Validate-CString (Main@USER_PHONE-value v)) (str.in.re (Main@USER_PHONE-value v) (re.++ (re.opt (str.to.re "+")) (re.range "1" "9") (re.opt (re.range "0" "9")) (re.* (str.to.re " ")) (re.opt (str.to.re "(")) ((_ re.loop 3 3) (re.range "0" "9")) (re.opt (str.to.re ")")) (re.opt (re.union (str.to.re " ") (str.to.re "-"))) ((_ re.loop 3 3) (re.range "0" "9")) (re.opt (re.union (str.to.re " ") (str.to.re "-"))) ((_ re.loop 4 4) (re.range "0" "9")))))
)
(define-fun @Validate-Some<Main@USER_ADDRESS> ((v Some<Main@USER_ADDRESS>)) Bool
    (@Validate-Main@USER_ADDRESS (Some<Main@USER_ADDRESS>-value v))
)
(define-fun @Validate-Option<Main@USER_ADDRESS> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_ADDRESS>-mk v) (@Validate-Some<Main@USER_ADDRESS> (@Term-Some<Main@USER_ADDRESS>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Some<Main@USER_PHONE> ((v Some<Main@USER_PHONE>)) Bool
    (@Validate-Main@USER_PHONE (Some<Main@USER_PHONE>-value v))
)
(define-fun @Validate-Option<Main@USER_PHONE> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_PHONE>-mk v) (@Validate-Some<Main@USER_PHONE> (@Term-Some<Main@USER_PHONE>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Some<Main@USER_NAME> ((v Some<Main@USER_NAME>)) Bool
    (@Validate-Main@USER_NAME (Some<Main@USER_NAME>-value v))
)
(define-fun @Validate-Option<Main@USER_NAME> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_NAME>-mk v) (@Validate-Some<Main@USER_NAME> (@Term-Some<Main@USER_NAME>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Some<Main@EMAIL> ((v Some<Main@EMAIL>)) Bool
    (@Validate-Main@EMAIL (Some<Main@EMAIL>-value v))
)
(define-fun @Validate-Option<Main@EMAIL> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@EMAIL>-mk v) (@Validate-Some<Main@EMAIL> (@Term-Some<Main@EMAIL>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Main@UserDTO ((v Main@UserDTO)) Bool
    (and (@Validate-Option<Main@EMAIL> (Main@UserDTO-email v)) (@Validate-Main@ALPHANUMERIC (Main@UserDTO-password v)) (@Validate-Option<Main@USER_NAME> (Main@UserDTO-name v)) (@Validate-Option<Main@USER_PHONE> (Main@UserDTO-phone v)) (@Validate-Option<Main@USER_ADDRESS> (Main@UserDTO-address v)) (@Validate-Int (Main@UserDTO-id v)) (Main@UserDTO_$_invariant0 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)) (Main@UserDTO_$_invariant1 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)) (Main@UserDTO_$_invariant2 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)) (Main@UserDTO_$_invariant3 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)) (Main@UserDTO_$_invariant4 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)) (Main@UserDTO_$_invariant5 (Main@UserDTO-email v) (Main@UserDTO-password v) (Main@UserDTO-name v) (Main@UserDTO-phone v) (Main@UserDTO-address v) (Main@UserDTO-id v)))
)
(define-fun @ValidateRoot-Main@ALPHANUMERIC ((v Main@ALPHANUMERIC)) Bool
    (@Validate-Main@ALPHANUMERIC v)
)
(define-fun @ValidateRoot-Main@EMAIL ((v Main@EMAIL)) Bool
    (@Validate-Main@EMAIL v)
)
(define-fun @ValidateRoot-Main@USER_ADDRESS ((v Main@USER_ADDRESS)) Bool
    (@Validate-Main@USER_ADDRESS v)
)
(define-fun @ValidateRoot-Main@USER_NAME ((v Main@USER_NAME)) Bool
    (@Validate-Main@USER_NAME v)
)
(define-fun @ValidateRoot-Main@USER_PHONE ((v Main@USER_PHONE)) Bool
    (@Validate-Main@USER_PHONE v)
)
(define-fun @ValidateRoot-Some<Main@USER_ADDRESS> ((v Some<Main@USER_ADDRESS>)) Bool
    (@Validate-Some<Main@USER_ADDRESS> v)
)
(define-fun @ValidateRoot-Option<Main@USER_ADDRESS> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_ADDRESS>-mk v) (@Validate-Some<Main@USER_ADDRESS> (@Term-Some<Main@USER_ADDRESS>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Some<Main@USER_PHONE> ((v Some<Main@USER_PHONE>)) Bool
    (@Validate-Some<Main@USER_PHONE> v)
)
(define-fun @ValidateRoot-Option<Main@USER_PHONE> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_PHONE>-mk v) (@Validate-Some<Main@USER_PHONE> (@Term-Some<Main@USER_PHONE>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Some<Main@USER_NAME> ((v Some<Main@USER_NAME>)) Bool
    (@Validate-Some<Main@USER_NAME> v)
)
(define-fun @ValidateRoot-Option<Main@USER_NAME> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@USER_NAME>-mk v) (@Validate-Some<Main@USER_NAME> (@Term-Some<Main@USER_NAME>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Some<Main@EMAIL> ((v Some<Main@EMAIL>)) Bool
    (@Validate-Some<Main@EMAIL> v)
)
(define-fun @ValidateRoot-Option<Main@EMAIL> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@EMAIL>-mk v) (@Validate-Some<Main@EMAIL> (@Term-Some<Main@EMAIL>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Main@UserDTO ((v Main@UserDTO)) Bool
    (@Validate-Main@UserDTO v)
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;Find some way to make all of the functions visible in the API.

;;Only used to expose Main@getCustomer to the extractor...

(declare-const customer_id Int)
(assert (> customer_id 0))
(assert (@Validate-Int customer_id))
(declare-const return_val (@Result Main@UserDTO))

;;MockgetCustomer is required to expose the functions to the cpp API z3::model.
(declare-fun MockTest (Int) (@Result Main@UserDTO))
(assert (= (MockTest customer_id) (Main@getCustomer customer_id)))
(assert (= return_val (MockTest customer_id))) 


(check-sat)
(get-model)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
