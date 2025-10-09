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

(declare-datatype Main@WindCategory (Main@WindCategory$calm Main@WindCategory$windy Main@WindCategory$high Main@WindCategory$dangerous))
(declare-datatype Main@WindDirection (Main@WindDirection$north Main@WindDirection$south Main@WindDirection$east Main@WindDirection$west))
(declare-datatype Main@ForecastDetail (Main@ForecastDetail$sun Main@ForecastDetail$showers Main@ForecastDetail$thunderstorms Main@ForecastDetail$snow Main@ForecastDetail$fog))
(declare-datatype Main@Reason (Main@Reason$approved Main@Reason$insufficientAvailability Main@Reason$closedDueToConditions))
;;no content -- ;;--TYPEDECL_DECLS--;;

;;
;; Entity datatypes 
;;
(declare-datatypes (
    ;;no content -- ;;--SPECIAL_DECLS--;;
    ;;no content -- ;;--COLLECTION_DECLS--;;
    (Main@TempRange 0)
    (Main@WindSpeedInfo 0)
    (Main@Forecast 0)
    (Main@RentalAvailabilityInfo 0)
    ;;no content -- ;;--DATATYPE_DECLS--;;
    (@Term 0)
    ) (
        ;;no content -- ;;--SPECIAL_CONSTRUCTORS--;;
        ;;no content -- ;;--COLLECTION_CONSTRUCTORS--;;
        ((Main@TempRange-mk (Main@TempRange-low Int) (Main@TempRange-high Int)))
        ((Main@WindSpeedInfo-mk (Main@WindSpeedInfo-min Nat) (Main@WindSpeedInfo-max Nat) (Main@WindSpeedInfo-windDirection Main@WindDirection)))
        ((Main@Forecast-mk (Main@Forecast-temp Main@TempRange) (Main@Forecast-winds Main@WindSpeedInfo) (Main@Forecast-info Main@ForecastDetail)))
        ((Main@RentalAvailabilityInfo-mk (Main@RentalAvailabilityInfo-inventory Nat) (Main@RentalAvailabilityInfo-reservations Nat) (Main@RentalAvailabilityInfo-returns Nat)))
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

(define-fun Main@RentalAvailabilityInfo_$_invariant0 (($inventory Nat) ($reservations Nat) ($returns Nat)) Bool
    (>= (+ $inventory $returns) $reservations)
)

(define-fun Main@TempRange_$_invariant0 (($low Int) ($high Int)) Bool
    (<= $low $high)
)

(define-fun Main@WindSpeedInfo_$_invariant0 (($min Nat) ($max Nat) ($windDirection Main@WindDirection)) Bool
    (<= $min $max)
)

(define-fun Main@categorizeWinds ((windSpeed Nat)) Main@WindCategory
    (ite (< windSpeed 10)
        Main@WindCategory$calm
        (ite (< windSpeed 20)
            Main@WindCategory$high
            (ite (< windSpeed 30)
                Main@WindCategory$windy
                Main@WindCategory$dangerous
            )
        )
    )
)

(define-fun Main@decide ((forecast Main@Forecast) (rentalinfo Main@RentalAvailabilityInfo) (requestedQuantity Nat) (allowPartials Bool)) (@Result (@EList-2 Nat Main@Reason))
    (let ((isClosed (or (= (Main@categorizeWinds (Main@WindSpeedInfo-max (Main@Forecast-winds forecast))) Main@WindCategory$dangerous) (= (Main@Forecast-info forecast) Main@ForecastDetail$thunderstorms))))
    (let ((@tmpe-availability (ite (< (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)))))) (ite (not (is-@Result-ok @tmpe-availability))  ((as @Result-err (@Result (@EList-2 Nat Main@Reason))) (@Result-etag @tmpe-availability)) (let ((availability (@Result-value @tmpe-availability)))
        (ite isClosed
            (@Result-ok (@EList-2-mk 0 Main@Reason$closedDueToConditions))
            (ite (<= requestedQuantity availability)
                (@Result-ok (@EList-2-mk requestedQuantity Main@Reason$approved))
                (ite allowPartials
                    (@Result-ok (@EList-2-mk availability Main@Reason$approved))
                    (@Result-ok (@EList-2-mk 0 Main@Reason$insufficientAvailability))
                )
            )
        )
    ))))
)

(define-fun Main@main ((forecast Main@Forecast) (rentalinfo Main@RentalAvailabilityInfo) (requestedQuantity Nat) (allowPartials Bool)) (@Result (@EList-2 Nat Main@Reason))
    (let ((windcat (Main@categorizeWinds (Main@WindSpeedInfo-max (Main@Forecast-winds forecast)))))
    (let ((@tmpe-decision (Main@decide forecast rentalinfo requestedQuantity allowPartials))) (ite (not (is-@Result-ok @tmpe-decision))  @tmpe-decision (let ((decision (@Result-value @tmpe-decision)))
        (ite (not (= (ite (= (@EList-2-1 decision) Main@Reason$approved) (let ((@tmp-1-1 (ite (< (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (<= (@EList-2-0 decision) (@Result-value @tmp-1-1))))) (@Result-ok true)) (@Result-ok true))) (let ((@failure (ite (= (@EList-2-1 decision) Main@Reason$approved) (let ((@tmp-1-1 (ite (< (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (+ (Main@RentalAvailabilityInfo-inventory rentalinfo) (Main@RentalAvailabilityInfo-returns rentalinfo)) (Main@RentalAvailabilityInfo-reservations rentalinfo)))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (<= (@EList-2-0 decision) (@Result-value @tmp-1-1))))) (@Result-ok true)))) (ite (= @failure (@Result-ok false)) ((as @Result-err (@Result (@EList-2 Nat Main@Reason))) @err-other) ((as @Result-err (@Result (@EList-2 Nat Main@Reason))) (@Result-etag @failure))))
            (ite (not (ite (and (= (@EList-2-1 decision) Main@Reason$approved) (not allowPartials)) (= (@EList-2-0 decision) requestedQuantity) true)) ((as @Result-err (@Result (@EList-2 Nat Main@Reason))) @err-other)
                (@Result-ok decision)
            )
        )
    ))))
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

(define-fun @Validate-Main@ForecastDetail ((v Main@ForecastDetail)) Bool
    true
)
(define-fun @Validate-Main@WindDirection ((v Main@WindDirection)) Bool
    true
)
(define-fun @Validate-Main@WindSpeedInfo ((v Main@WindSpeedInfo)) Bool
    (and (@Validate-Nat (Main@WindSpeedInfo-min v)) (@Validate-Nat (Main@WindSpeedInfo-max v)) (@Validate-Main@WindDirection (Main@WindSpeedInfo-windDirection v)) (Main@WindSpeedInfo_$_invariant0 (Main@WindSpeedInfo-min v) (Main@WindSpeedInfo-max v) (Main@WindSpeedInfo-windDirection v)))
)
(define-fun @Validate-Main@TempRange ((v Main@TempRange)) Bool
    (and (@Validate-Int (Main@TempRange-low v)) (@Validate-Int (Main@TempRange-high v)) (Main@TempRange_$_invariant0 (Main@TempRange-low v) (Main@TempRange-high v)))
)
(define-fun @Validate-Main@Forecast ((v Main@Forecast)) Bool
    (and (@Validate-Main@TempRange (Main@Forecast-temp v)) (@Validate-Main@WindSpeedInfo (Main@Forecast-winds v)) (@Validate-Main@ForecastDetail (Main@Forecast-info v)))
)
(define-fun @Validate-Main@Reason ((v Main@Reason)) Bool
    true
)
(define-fun @Validate-Main@RentalAvailabilityInfo ((v Main@RentalAvailabilityInfo)) Bool
    (and (@Validate-Nat (Main@RentalAvailabilityInfo-inventory v)) (@Validate-Nat (Main@RentalAvailabilityInfo-reservations v)) (@Validate-Nat (Main@RentalAvailabilityInfo-returns v)) (Main@RentalAvailabilityInfo_$_invariant0 (Main@RentalAvailabilityInfo-inventory v) (Main@RentalAvailabilityInfo-reservations v) (Main@RentalAvailabilityInfo-returns v)))
)
(define-fun @Validate-Main@WindCategory ((v Main@WindCategory)) Bool
    true
)
(define-fun @ValidateRoot-Main@ForecastDetail ((v Main@ForecastDetail)) Bool
    (@Validate-Main@ForecastDetail v)
)
(define-fun @ValidateRoot-Main@WindDirection ((v Main@WindDirection)) Bool
    (@Validate-Main@WindDirection v)
)
(define-fun @ValidateRoot-Main@WindSpeedInfo ((v Main@WindSpeedInfo)) Bool
    (@Validate-Main@WindSpeedInfo v)
)
(define-fun @ValidateRoot-Main@TempRange ((v Main@TempRange)) Bool
    (@Validate-Main@TempRange v)
)
(define-fun @ValidateRoot-Main@Forecast ((v Main@Forecast)) Bool
    (@Validate-Main@Forecast v)
)
(define-fun @ValidateRoot-Main@Reason ((v Main@Reason)) Bool
    (@Validate-Main@Reason v)
)
(define-fun @ValidateRoot-Main@RentalAvailabilityInfo ((v Main@RentalAvailabilityInfo)) Bool
    (@Validate-Main@RentalAvailabilityInfo v)
)
(define-fun @ValidateRoot-Main@WindCategory ((v Main@WindCategory)) Bool
    (@Validate-Main@WindCategory v)
)
