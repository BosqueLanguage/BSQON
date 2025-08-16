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
    (Some<Main@DatabaseIndex> 0)
    (Some<CString> 0)
    (Some<Nat> 0)
    (List<Main@Entry> 0)
    (List<CString> 0)
    (List<Nat> 0)
    (List<Main@FormatEntry> 0)
    (Main@Entry 0)
    (Main@FormatEntry 0)
    (Main@Format 0)
    (Main@DatabaseIndex 0)
    (Main@Database 0)
    (ListOps@Vector0<Main@Entry> 0)
    (ListOps@Vector0<CString> 0)
    (ListOps@Vector0<Nat> 0)
    (ListOps@Vector0<Main@FormatEntry> 0)
    (ListOps@Vector1<Main@Entry> 0)
    (ListOps@Vector1<CString> 0)
    (ListOps@Vector1<Nat> 0)
    (ListOps@Vector1<Main@FormatEntry> 0)
    (ListOps@Vector2<Main@Entry> 0)
    (ListOps@Vector2<CString> 0)
    (ListOps@Vector2<Nat> 0)
    (ListOps@Vector2<Main@FormatEntry> 0)
    (ListOps@Vector3<Main@Entry> 0)
    (ListOps@Vector3<CString> 0)
    (ListOps@Vector3<Nat> 0)
    (ListOps@Vector3<Main@FormatEntry> 0)
    (Main@NumRecordsOp 0)
    (Main@EndOp 0)
    (Main@ListOp 0)
    (Main@GotoRecordOp 0)
    (Main@NextOp 0)
    (Main@PreviousOp 0)
    (Main@StatusOp 0)
    (Main@AddOp 0)
    (Main@ModifyOp 0)
    (Main@RemoveOp 0)
    (@Term 0)
    ) (
        ((Some<Main@DatabaseIndex>-mk (Some<Main@DatabaseIndex>-value Main@DatabaseIndex)))
        ((Some<CString>-mk (Some<CString>-value CString)))
        ((Some<Nat>-mk (Some<Nat>-value Nat)))
        ((List<Main@Entry>-mk (List<Main@Entry>-value @Term)))
        ((List<CString>-mk (List<CString>-value @Term)))
        ((List<Nat>-mk (List<Nat>-value @Term)))
        ((List<Main@FormatEntry>-mk (List<Main@FormatEntry>-value @Term)))
        ((Main@Entry-mk (Main@Entry-items List<CString>)))
        ((Main@FormatEntry-mk (Main@FormatEntry-header CString) (Main@FormatEntry-entries List<CString>)))
        ((Main@Format-mk (Main@Format-entries List<Main@FormatEntry>) (Main@Format-vcount Nat)))
        ((Main@DatabaseIndex-mk (Main@DatabaseIndex-imap List<Nat>) (Main@DatabaseIndex-curr Nat)))
        ((Main@Database-mk (Main@Database-dbname @Term) (Main@Database-entries List<Main@Entry>) (Main@Database-format Main@Format) (Main@Database-index @Term) (Main@Database-fnum @Term)))
        (ListOps@Vector0<Main@Entry>-mk)
        (ListOps@Vector0<CString>-mk)
        (ListOps@Vector0<Nat>-mk)
        (ListOps@Vector0<Main@FormatEntry>-mk)
        ((ListOps@Vector1<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-i0 Main@Entry)))
        ((ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-i0 CString)))
        ((ListOps@Vector1<Nat>-mk (ListOps@Vector1<Nat>-i0 Nat)))
        ((ListOps@Vector1<Main@FormatEntry>-mk (ListOps@Vector1<Main@FormatEntry>-i0 Main@FormatEntry)))
        ((ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-i0 Main@Entry) (ListOps@Vector2<Main@Entry>-i1 Main@Entry)))
        ((ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-i0 CString) (ListOps@Vector2<CString>-i1 CString)))
        ((ListOps@Vector2<Nat>-mk (ListOps@Vector2<Nat>-i0 Nat) (ListOps@Vector2<Nat>-i1 Nat)))
        ((ListOps@Vector2<Main@FormatEntry>-mk (ListOps@Vector2<Main@FormatEntry>-i0 Main@FormatEntry) (ListOps@Vector2<Main@FormatEntry>-i1 Main@FormatEntry)))
        ((ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i0 Main@Entry) (ListOps@Vector3<Main@Entry>-i1 Main@Entry) (ListOps@Vector3<Main@Entry>-i2 Main@Entry)))
        ((ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-i0 CString) (ListOps@Vector3<CString>-i1 CString) (ListOps@Vector3<CString>-i2 CString)))
        ((ListOps@Vector3<Nat>-mk (ListOps@Vector3<Nat>-i0 Nat) (ListOps@Vector3<Nat>-i1 Nat) (ListOps@Vector3<Nat>-i2 Nat)))
        ((ListOps@Vector3<Main@FormatEntry>-mk (ListOps@Vector3<Main@FormatEntry>-i0 Main@FormatEntry) (ListOps@Vector3<Main@FormatEntry>-i1 Main@FormatEntry) (ListOps@Vector3<Main@FormatEntry>-i2 Main@FormatEntry)))
        (Main@NumRecordsOp-mk)
        (Main@EndOp-mk)
        (Main@ListOp-mk)
        ((Main@GotoRecordOp-mk (Main@GotoRecordOp-ridx Nat)))
        (Main@NextOp-mk)
        (Main@PreviousOp-mk)
        (Main@StatusOp-mk)
        ((Main@AddOp-mk (Main@AddOp-entry Main@Entry)))
        ((Main@ModifyOp-mk (Main@ModifyOp-entry Main@Entry)))
        (Main@RemoveOp-mk)
        (
            (@Term-None-mk)
            ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;;
            (@Term-Some<Main@DatabaseIndex>-mk (@Term-Some<Main@DatabaseIndex>-value Some<Main@DatabaseIndex>))
            (@Term-Some<CString>-mk (@Term-Some<CString>-value Some<CString>))
            (@Term-Some<Nat>-mk (@Term-Some<Nat>-value Some<Nat>))
            ;;no content -- ;;--ENTITY_TERM_CONSTRUCTORS--;;
            (@Term-ListOps@Vector0<Main@Entry>-mk (@Term-ListOps@Vector0<Main@Entry>-value ListOps@Vector0<Main@Entry>))
            (@Term-ListOps@Vector0<CString>-mk (@Term-ListOps@Vector0<CString>-value ListOps@Vector0<CString>))
            (@Term-ListOps@Vector0<Nat>-mk (@Term-ListOps@Vector0<Nat>-value ListOps@Vector0<Nat>))
            (@Term-ListOps@Vector0<Main@FormatEntry>-mk (@Term-ListOps@Vector0<Main@FormatEntry>-value ListOps@Vector0<Main@FormatEntry>))
            (@Term-ListOps@Vector1<Main@Entry>-mk (@Term-ListOps@Vector1<Main@Entry>-value ListOps@Vector1<Main@Entry>))
            (@Term-ListOps@Vector1<CString>-mk (@Term-ListOps@Vector1<CString>-value ListOps@Vector1<CString>))
            (@Term-ListOps@Vector1<Nat>-mk (@Term-ListOps@Vector1<Nat>-value ListOps@Vector1<Nat>))
            (@Term-ListOps@Vector1<Main@FormatEntry>-mk (@Term-ListOps@Vector1<Main@FormatEntry>-value ListOps@Vector1<Main@FormatEntry>))
            (@Term-ListOps@Vector2<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-value ListOps@Vector2<Main@Entry>))
            (@Term-ListOps@Vector2<CString>-mk (@Term-ListOps@Vector2<CString>-value ListOps@Vector2<CString>))
            (@Term-ListOps@Vector2<Nat>-mk (@Term-ListOps@Vector2<Nat>-value ListOps@Vector2<Nat>))
            (@Term-ListOps@Vector2<Main@FormatEntry>-mk (@Term-ListOps@Vector2<Main@FormatEntry>-value ListOps@Vector2<Main@FormatEntry>))
            (@Term-ListOps@Vector3<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-value ListOps@Vector3<Main@Entry>))
            (@Term-ListOps@Vector3<CString>-mk (@Term-ListOps@Vector3<CString>-value ListOps@Vector3<CString>))
            (@Term-ListOps@Vector3<Nat>-mk (@Term-ListOps@Vector3<Nat>-value ListOps@Vector3<Nat>))
            (@Term-ListOps@Vector3<Main@FormatEntry>-mk (@Term-ListOps@Vector3<Main@FormatEntry>-value ListOps@Vector3<Main@FormatEntry>))
            (@Term-Main@NumRecordsOp-mk (@Term-Main@NumRecordsOp-value Main@NumRecordsOp))
            (@Term-Main@EndOp-mk (@Term-Main@EndOp-value Main@EndOp))
            (@Term-Main@ListOp-mk (@Term-Main@ListOp-value Main@ListOp))
            (@Term-Main@GotoRecordOp-mk (@Term-Main@GotoRecordOp-value Main@GotoRecordOp))
            (@Term-Main@NextOp-mk (@Term-Main@NextOp-value Main@NextOp))
            (@Term-Main@PreviousOp-mk (@Term-Main@PreviousOp-value Main@PreviousOp))
            (@Term-Main@StatusOp-mk (@Term-Main@StatusOp-value Main@StatusOp))
            (@Term-Main@AddOp-mk (@Term-Main@AddOp-value Main@AddOp))
            (@Term-Main@ModifyOp-mk (@Term-Main@ModifyOp-value Main@ModifyOp))
            (@Term-Main@RemoveOp-mk (@Term-Main@RemoveOp-value Main@RemoveOp))
        )
    )
)

;;no content -- ;;--VFIELD_ACCESS--;;

(define-fun @SubtypeOf-Option<Main@DatabaseIndex>((tt @Term)) Bool (or (is-@Term-Some<Main@DatabaseIndex>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<CString>((tt @Term)) Bool (or (is-@Term-Some<CString>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-Option<Nat>((tt @Term)) Bool (or (is-@Term-Some<Nat>-mk tt) (= tt @Term-None-mk)))
(define-fun @SubtypeOf-ListOps@Vector<Main@Entry>((tt @Term)) Bool (or (is-@Term-ListOps@Vector0<Main@Entry>-mk tt) (is-@Term-ListOps@Vector1<Main@Entry>-mk tt) (is-@Term-ListOps@Vector2<Main@Entry>-mk tt) (is-@Term-ListOps@Vector3<Main@Entry>-mk tt)))
(define-fun @SubtypeOf-ListOps@Vector<CString>((tt @Term)) Bool (or (is-@Term-ListOps@Vector0<CString>-mk tt) (is-@Term-ListOps@Vector1<CString>-mk tt) (is-@Term-ListOps@Vector2<CString>-mk tt) (is-@Term-ListOps@Vector3<CString>-mk tt)))
(define-fun @SubtypeOf-ListOps@Vector<Nat>((tt @Term)) Bool (or (is-@Term-ListOps@Vector0<Nat>-mk tt) (is-@Term-ListOps@Vector1<Nat>-mk tt) (is-@Term-ListOps@Vector2<Nat>-mk tt) (is-@Term-ListOps@Vector3<Nat>-mk tt)))
(define-fun @SubtypeOf-ListOps@Vector<Main@FormatEntry>((tt @Term)) Bool (or (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk tt) (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk tt) (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk tt) (is-@Term-ListOps@Vector3<Main@FormatEntry>-mk tt)))
(define-fun @SubtypeOf-Main@DatabaseOperation((tt @Term)) Bool (or (is-@Term-Main@NumRecordsOp-mk tt) (is-@Term-Main@EndOp-mk tt) (is-@Term-Main@ListOp-mk tt) (is-@Term-Main@GotoRecordOp-mk tt) (is-@Term-Main@NextOp-mk tt) (is-@Term-Main@PreviousOp-mk tt) (is-@Term-Main@StatusOp-mk tt) (is-@Term-Main@AddOp-mk tt) (is-@Term-Main@ModifyOp-mk tt) (is-@Term-Main@RemoveOp-mk tt)))

(declare-const Nat@zero Nat) (declare-const Nat@zero-cc-temp (@Result Nat))
(declare-const Nat@one Nat) (declare-const Nat@one-cc-temp (@Result Nat))

;;no content -- ;;--PRE_FUNCS--;;

(define-fun Core@CStringOps@s_concat2 ((s1 CString) (s2 CString)) CString (str.++ s1 s2))

(define-fun Core@CStringOps@s_empty ((s CString)) Bool (= s ""))

(define-fun Core@NumericOps@s_natToCString ((v Nat)) CString (str.from_int v))

(define-fun Core@XCore@s_blockingfailure<List<CString>> () (@Result List<CString>) ((as @Result-err (@Result List<CString>)) @err-other))

(define-fun Core@XCore@s_blockingfailure<List<Main@Entry>> () (@Result List<Main@Entry>) ((as @Result-err (@Result List<Main@Entry>)) @err-other))

(define-fun Core@XCore@s_blockingfailure<List<Main@FormatEntry>> () (@Result List<Main@FormatEntry>) ((as @Result-err (@Result List<Main@FormatEntry>)) @err-other))

(define-fun Core@XCore@s_blockingfailure<List<Nat>> () (@Result List<Nat>) ((as @Result-err (@Result List<Nat>)) @err-other))

(define-fun Core@ListOps@s_list_empty<CString> ((l List<CString>)) Bool
    (is-@Term-ListOps@Vector0<CString>-mk (List<CString>-value l))
)

(define-fun List<CString>@empty ((this List<CString>)) Bool
    (Core@ListOps@s_list_empty<CString> this)
)

(define-fun Core@ListOps@s_list_reduce<CString-CString> ((l List<CString>) (init CString) (f (Array CString CString CString))) CString
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<CString>-value ll)))
            init
        )
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
        (let ((r0 (select f init (ListOps@Vector1<CString>-i0 $ll))))
            r0
        ))
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
        (let ((r0 (select f init (ListOps@Vector2<CString>-i0 $ll))))
        (let ((r1 (select f r0 (ListOps@Vector2<CString>-i1 $ll))))
            r1
        )))
                (let (($ll (@Term-ListOps@Vector3<CString>-value ll)))
        (let ((r0 (select f init (ListOps@Vector3<CString>-i0 $ll))))
        (let ((r1 (select f r0 (ListOps@Vector3<CString>-i1 $ll))))
        (let ((r2 (select f r1 (ListOps@Vector3<CString>-i2 $ll))))
            r2
        )))))))
    )
)

(define-fun List<CString>@reduce<CString> ((this List<CString>) (init CString) (f (Array CString CString CString))) CString
    (ite (Core@ListOps@s_list_empty<CString> this)
        init
        (Core@ListOps@s_list_reduce<CString-CString> this init f)
    )
)

(define-fun CString@concat ((strs List<CString>)) CString
    (ite (List<CString>@empty strs)
        ""
        (List<CString>@reduce<CString> strs "" (lambda ((acc CString) (s CString)) (Core@CStringOps@s_concat2 acc s)))
    )
)

(define-fun CString@empty ((this CString)) Bool
    (Core@CStringOps@s_empty this)
)

(define-fun CString@joinAll ((sep CString) (strs List<CString>)) CString
    (ite (List<CString>@empty strs)
        ""
        (List<CString>@reduce<CString> strs "" (lambda ((acc CString) (s CString)) (ite (CString@empty acc) s (Core@CStringOps@s_concat2 (Core@CStringOps@s_concat2 acc sep) s))))
    )
)

(define-fun Core@ListOps@s_list_allof<Main@Entry> ((l List<Main@Entry>) (p (Array Main@Entry Bool))) Bool
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@Entry>-value ll)))
            true
        )
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
        (let ((p0 (p (ListOps@Vector1<Main@Entry>-i0 $ll))))
            p0
        ))
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
        (let ((p0 (p (ListOps@Vector2<Main@Entry>-i0 $ll))))
            (ite (not p0)
                false
                (let ((p1 (p (ListOps@Vector2<Main@Entry>-i1 $ll))))
                    p1
                )
            )
        ))
                (let (($ll (@Term-ListOps@Vector3<Main@Entry>-value ll)))
        (let ((p0 (p (ListOps@Vector3<Main@Entry>-i0 $ll))))
            (ite (not p0)
                false
                (let ((p1 (p (ListOps@Vector3<Main@Entry>-i1 $ll))))
                    (ite (not p1)
                        false
                        (let ((p2 (p (ListOps@Vector3<Main@Entry>-i2 $ll))))
                            p2
                        )
                    )
                )
            )
        )))))
    )
)

(define-fun Core@ListOps@s_list_allof<Nat> ((l List<Nat>) (p (Array Nat Bool))) Bool
    (let ((ll (List<Nat>-value l)))
        (ite (is-@Term-ListOps@Vector0<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Nat>-value ll)))
            true
        )
        (ite (is-@Term-ListOps@Vector1<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Nat>-value ll)))
        (let ((p0 (p (ListOps@Vector1<Nat>-i0 $ll))))
            p0
        ))
        (ite (is-@Term-ListOps@Vector2<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Nat>-value ll)))
        (let ((p0 (p (ListOps@Vector2<Nat>-i0 $ll))))
            (ite (not p0)
                false
                (let ((p1 (p (ListOps@Vector2<Nat>-i1 $ll))))
                    p1
                )
            )
        ))
                (let (($ll (@Term-ListOps@Vector3<Nat>-value ll)))
        (let ((p0 (p (ListOps@Vector3<Nat>-i0 $ll))))
            (ite (not p0)
                false
                (let ((p1 (p (ListOps@Vector3<Nat>-i1 $ll))))
                    (ite (not p1)
                        false
                        (let ((p2 (p (ListOps@Vector3<Nat>-i2 $ll))))
                            p2
                        )
                    )
                )
            )
        )))))
    )
)

(define-fun Core@ListOps@s_list_create_1<CString> ((v1 CString)) List<CString>
    (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk v1)))
)

(define-fun Core@ListOps@s_list_create_2<CString> ((v1 CString) (v2 CString)) List<CString>
    (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk v1 v2)))
)

(define-fun Core@ListOps@s_list_create_2<Main@FormatEntry> ((v1 Main@FormatEntry) (v2 Main@FormatEntry)) List<Main@FormatEntry>
    (List<Main@FormatEntry>-mk (@Term-ListOps@Vector2<Main@FormatEntry>-mk (ListOps@Vector2<Main@FormatEntry>-mk v1 v2)))
)

(define-fun Core@ListOps@s_list_create_3<CString> ((v1 CString) (v2 CString) (v3 CString)) List<CString>
    (List<CString>-mk (@Term-ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-mk v1 v2 v3)))
)

(define-fun Core@ListOps@s_list_create_3<Main@Entry> ((v1 Main@Entry) (v2 Main@Entry) (v3 Main@Entry)) List<Main@Entry>
    (List<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-mk v1 v2 v3)))
)

(define-fun Core@ListOps@s_list_create_empty<CString> () List<CString>
    (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
)

(define-fun Core@ListOps@s_list_create_empty<Main@Entry> () List<Main@Entry>
    (List<Main@Entry>-mk (@Term-ListOps@Vector0<Main@Entry>-mk ListOps@Vector0<Main@Entry>-mk))
)

(define-fun Core@ListOps@s_list_create_empty<Main@FormatEntry> () List<Main@FormatEntry>
    (List<Main@FormatEntry>-mk (@Term-ListOps@Vector0<Main@FormatEntry>-mk ListOps@Vector0<Main@FormatEntry>-mk))
)

(define-fun Core@ListOps@s_list_create_empty<Nat> () List<Nat>
    (List<Nat>-mk (@Term-ListOps@Vector0<Nat>-mk ListOps@Vector0<Nat>-mk))
)

(define-fun Core@ListOps@s_list_delete<Main@Entry> ((l List<Main@Entry>) (idx Nat)) List<Main@Entry>
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
            (List<Main@Entry>-mk (@Term-ListOps@Vector0<Main@Entry>-mk ListOps@Vector0<Main@Entry>-mk))
        )
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
            (ite (= idx 0)
                (List<Main@Entry>-mk (@Term-ListOps@Vector1<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-i1 $ll))))
                (List<Main@Entry>-mk (@Term-ListOps@Vector1<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-i0 $ll))))
            )
        )
                (let (($ll ll))
        (let ((ll3 (@Term-ListOps@Vector3<Main@Entry>-value $ll)))
            (ite (= idx 0)
                (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i1 ll3) (ListOps@Vector3<Main@Entry>-i2 ll3))))
                (ite (= idx 1)
                    (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i0 ll3) (ListOps@Vector3<Main@Entry>-i2 ll3))))
                    (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i0 ll3) (ListOps@Vector3<Main@Entry>-i1 ll3))))
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_empty<Main@Entry> ((l List<Main@Entry>)) Bool
    (is-@Term-ListOps@Vector0<Main@Entry>-mk (List<Main@Entry>-value l))
)

(define-fun Core@ListOps@s_list_empty<Main@FormatEntry> ((l List<Main@FormatEntry>)) Bool
    (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk (List<Main@FormatEntry>-value l))
)

(define-fun Core@ListOps@s_list_empty<Nat> ((l List<Nat>)) Bool
    (is-@Term-ListOps@Vector0<Nat>-mk (List<Nat>-value l))
)

(define-fun Core@ListOps@s_list_firstk<CString> ((l List<CString>) (k Nat)) List<CString>
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<CString>-value ll)))
            (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
        )
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
            (ite (= k 0)
                (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
                (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-i0 $ll))))
            )
        )
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
            (ite (= k 0)
                (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
                (ite (= k 1)
                    (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk (ListOps@Vector2<CString>-i0 $ll))))
                    (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-i0 $ll) (ListOps@Vector2<CString>-i1 $ll))))
                )
            )
        )
                (let (($ll (@Term-ListOps@Vector3<CString>-value ll)))
            (ite (= k 0)
                (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
                (ite (= k 2)
                    (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk (ListOps@Vector3<CString>-i0 $ll) (ListOps@Vector3<CString>-i1 $ll))))
                    (ite (= k 1)
                        (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk (ListOps@Vector3<CString>-i0 $ll))))
                        (List<CString>-mk (@Term-ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-i0 $ll) (ListOps@Vector3<CString>-i1 $ll) (ListOps@Vector3<CString>-i2 $ll))))
                    )
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_get<CString> ((l List<CString>) (idx Nat)) CString
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
            (ListOps@Vector1<CString>-i0 $ll)
        )
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
            (ite (= idx 0)
                (ListOps@Vector2<CString>-i0 $ll)
                (ListOps@Vector2<CString>-i1 $ll)
            )
        )
                (let (($ll ll))
        (let ((ll3 (@Term-ListOps@Vector3<CString>-value $ll)))
            (ite (= idx 0)
                (ListOps@Vector3<CString>-i0 ll3)
                (ite (= idx 1)
                    (ListOps@Vector3<CString>-i1 ll3)
                    (ListOps@Vector3<CString>-i2 ll3)
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_get<Main@Entry> ((l List<Main@Entry>) (idx Nat)) Main@Entry
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
            (ListOps@Vector1<Main@Entry>-i0 $ll)
        )
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
            (ite (= idx 0)
                (ListOps@Vector2<Main@Entry>-i0 $ll)
                (ListOps@Vector2<Main@Entry>-i1 $ll)
            )
        )
                (let (($ll ll))
        (let ((ll3 (@Term-ListOps@Vector3<Main@Entry>-value $ll)))
            (ite (= idx 0)
                (ListOps@Vector3<Main@Entry>-i0 ll3)
                (ite (= idx 1)
                    (ListOps@Vector3<Main@Entry>-i1 ll3)
                    (ListOps@Vector3<Main@Entry>-i2 ll3)
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_get<Nat> ((l List<Nat>) (idx Nat)) Nat
    (let ((ll (List<Nat>-value l)))
        (ite (is-@Term-ListOps@Vector1<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Nat>-value ll)))
            (ListOps@Vector1<Nat>-i0 $ll)
        )
        (ite (is-@Term-ListOps@Vector2<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Nat>-value ll)))
            (ite (= idx 0)
                (ListOps@Vector2<Nat>-i0 $ll)
                (ListOps@Vector2<Nat>-i1 $ll)
            )
        )
                (let (($ll ll))
        (let ((ll3 (@Term-ListOps@Vector3<Nat>-value $ll)))
            (ite (= idx 0)
                (ListOps@Vector3<Nat>-i0 ll3)
                (ite (= idx 1)
                    (ListOps@Vector3<Nat>-i1 ll3)
                    (ListOps@Vector3<Nat>-i2 ll3)
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_map<Main@FormatEntry-Nat> ((l List<Main@FormatEntry>) (f (Array Main@FormatEntry Nat))) List<Nat>
    (let ((ll (List<Main@FormatEntry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@FormatEntry>-value ll)))
            (List<Nat>-mk (@Term-ListOps@Vector0<Nat>-mk ListOps@Vector0<Nat>-mk))
        )
        (ite (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@FormatEntry>-value ll)))
        (let ((v0 (f (ListOps@Vector1<Main@FormatEntry>-i0 $ll))))
            (List<Nat>-mk (@Term-ListOps@Vector1<Nat>-mk (ListOps@Vector1<Nat>-mk v0)))
        ))
        (ite (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@FormatEntry>-value ll)))
        (let ((v0 (f (ListOps@Vector2<Main@FormatEntry>-i0 $ll))))
        (let ((v1 (f (ListOps@Vector2<Main@FormatEntry>-i1 $ll))))
            (List<Nat>-mk (@Term-ListOps@Vector2<Nat>-mk (ListOps@Vector2<Nat>-mk v0 v1)))
        )))
                (let (($ll (@Term-ListOps@Vector3<Main@FormatEntry>-value ll)))
        (let ((v0 (f (ListOps@Vector3<Main@FormatEntry>-i0 $ll))))
        (let ((v1 (f (ListOps@Vector3<Main@FormatEntry>-i1 $ll))))
        (let ((v2 (f (ListOps@Vector3<Main@FormatEntry>-i2 $ll))))
            (List<Nat>-mk (@Term-ListOps@Vector3<Nat>-mk (ListOps@Vector3<Nat>-mk v0 v1 v2)))
        )))))))
    )
)

(define-fun Core@ListOps@s_list_mapidx<CString-CString>$T ((l List<CString>) (f (Array CString Nat (@Result CString)))) (@Result List<CString>)
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<CString>-value ll)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk)))
        )
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
        (let ((@tmpe-v0 (select f (ListOps@Vector1<CString>-i0 $ll) 0))) (ite (not (is-@Result-ok @tmpe-v0))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v0)) (let ((v0 (@Result-value @tmpe-v0)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk v0))))
        ))))
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
        (let ((@tmpe-v0 (select f (ListOps@Vector2<CString>-i0 $ll) 0))) (ite (not (is-@Result-ok @tmpe-v0))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v0)) (let ((v0 (@Result-value @tmpe-v0)))
        (let ((@tmpe-v1 (select f (ListOps@Vector2<CString>-i1 $ll) 1))) (ite (not (is-@Result-ok @tmpe-v1))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v1)) (let ((v1 (@Result-value @tmpe-v1)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk v0 v1))))
        )))))))
                (let (($ll (@Term-ListOps@Vector3<CString>-value ll)))
        (let ((@tmpe-v0 (select f (ListOps@Vector3<CString>-i0 $ll) 0))) (ite (not (is-@Result-ok @tmpe-v0))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v0)) (let ((v0 (@Result-value @tmpe-v0)))
        (let ((@tmpe-v1 (select f (ListOps@Vector3<CString>-i1 $ll) 1))) (ite (not (is-@Result-ok @tmpe-v1))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v1)) (let ((v1 (@Result-value @tmpe-v1)))
        (let ((@tmpe-v2 (select f (ListOps@Vector3<CString>-i2 $ll) 2))) (ite (not (is-@Result-ok @tmpe-v2))  ((as @Result-err (@Result List<CString>)) (@Result-etag @tmpe-v2)) (let ((v2 (@Result-value @tmpe-v2)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-mk v0 v1 v2))))
        )))))))))))))
    )
)

(define-fun Core@ListOps@s_list_mapidx<Main@Entry-Nat> ((l List<Main@Entry>) (f (Array Main@Entry Nat Nat))) List<Nat>
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@Entry>-value ll)))
            (List<Nat>-mk (@Term-ListOps@Vector0<Nat>-mk ListOps@Vector0<Nat>-mk))
        )
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
        (let ((v0 (select f (ListOps@Vector1<Main@Entry>-i0 $ll) 0)))
            (List<Nat>-mk (@Term-ListOps@Vector1<Nat>-mk (ListOps@Vector1<Nat>-mk v0)))
        ))
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
        (let ((v0 (select f (ListOps@Vector2<Main@Entry>-i0 $ll) 0)))
        (let ((v1 (select f (ListOps@Vector2<Main@Entry>-i1 $ll) 1)))
            (List<Nat>-mk (@Term-ListOps@Vector2<Nat>-mk (ListOps@Vector2<Nat>-mk v0 v1)))
        )))
                (let (($ll (@Term-ListOps@Vector3<Main@Entry>-value ll)))
        (let ((v0 (select f (ListOps@Vector3<Main@Entry>-i0 $ll) 0)))
        (let ((v1 (select f (ListOps@Vector3<Main@Entry>-i1 $ll) 1)))
        (let ((v2 (select f (ListOps@Vector3<Main@Entry>-i2 $ll) 2)))
            (List<Nat>-mk (@Term-ListOps@Vector3<Nat>-mk (ListOps@Vector3<Nat>-mk v0 v1 v2)))
        )))))))
    )
)

(define-fun Core@ListOps@s_list_push_back<CString> ((l List<CString>) (v CString)) (@Result List<CString>)
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<CString>-value ll)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk v))))
        )
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk (ListOps@Vector1<CString>-i0 $ll) v))))
        )
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
            (@Result-ok (List<CString>-mk (@Term-ListOps@Vector3<CString>-mk (ListOps@Vector3<CString>-mk (ListOps@Vector2<CString>-i0 $ll) (ListOps@Vector2<CString>-i1 $ll) v))))
        )
                (let (($ll (@Term-ListOps@Vector3<CString>-value ll)))
            Core@XCore@s_blockingfailure<List<CString>>
        ))))
    )
)

(define-fun Core@ListOps@s_list_push_back<Main@Entry> ((l List<Main@Entry>) (v Main@Entry)) (@Result List<Main@Entry>)
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@Entry>-value ll)))
            (@Result-ok (List<Main@Entry>-mk (@Term-ListOps@Vector1<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-mk v))))
        )
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
            (@Result-ok (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-i0 $ll) v))))
        )
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
            (@Result-ok (List<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-i0 $ll) (ListOps@Vector2<Main@Entry>-i1 $ll) v))))
        )
                (let (($ll (@Term-ListOps@Vector3<Main@Entry>-value ll)))
            Core@XCore@s_blockingfailure<List<Main@Entry>>
        ))))
    )
)

(define-fun Core@ListOps@s_list_push_back<Main@FormatEntry> ((l List<Main@FormatEntry>) (v Main@FormatEntry)) (@Result List<Main@FormatEntry>)
    (let ((ll (List<Main@FormatEntry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@FormatEntry>-value ll)))
            (@Result-ok (List<Main@FormatEntry>-mk (@Term-ListOps@Vector1<Main@FormatEntry>-mk (ListOps@Vector1<Main@FormatEntry>-mk v))))
        )
        (ite (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@FormatEntry>-value ll)))
            (@Result-ok (List<Main@FormatEntry>-mk (@Term-ListOps@Vector2<Main@FormatEntry>-mk (ListOps@Vector2<Main@FormatEntry>-mk (ListOps@Vector1<Main@FormatEntry>-i0 $ll) v))))
        )
        (ite (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@FormatEntry>-value ll)))
            (@Result-ok (List<Main@FormatEntry>-mk (@Term-ListOps@Vector3<Main@FormatEntry>-mk (ListOps@Vector3<Main@FormatEntry>-mk (ListOps@Vector2<Main@FormatEntry>-i0 $ll) (ListOps@Vector2<Main@FormatEntry>-i1 $ll) v))))
        )
                (let (($ll (@Term-ListOps@Vector3<Main@FormatEntry>-value ll)))
            Core@XCore@s_blockingfailure<List<Main@FormatEntry>>
        ))))
    )
)

(define-fun Core@ListOps@s_list_push_back<Nat> ((l List<Nat>) (v Nat)) (@Result List<Nat>)
    (let ((ll (List<Nat>-value l)))
        (ite (is-@Term-ListOps@Vector0<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Nat>-value ll)))
            (@Result-ok (List<Nat>-mk (@Term-ListOps@Vector1<Nat>-mk (ListOps@Vector1<Nat>-mk v))))
        )
        (ite (is-@Term-ListOps@Vector1<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Nat>-value ll)))
            (@Result-ok (List<Nat>-mk (@Term-ListOps@Vector2<Nat>-mk (ListOps@Vector2<Nat>-mk (ListOps@Vector1<Nat>-i0 $ll) v))))
        )
        (ite (is-@Term-ListOps@Vector2<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Nat>-value ll)))
            (@Result-ok (List<Nat>-mk (@Term-ListOps@Vector3<Nat>-mk (ListOps@Vector3<Nat>-mk (ListOps@Vector2<Nat>-i0 $ll) (ListOps@Vector2<Nat>-i1 $ll) v))))
        )
                (let (($ll (@Term-ListOps@Vector3<Nat>-value ll)))
            Core@XCore@s_blockingfailure<List<Nat>>
        ))))
    )
)

(define-fun Core@ListOps@s_list_reduce<Main@FormatEntry-@EList<CString-List<CString>>>$T ((l List<Main@FormatEntry>) (init (@EList-2 CString List<CString>)) (f (Array (@EList-2 CString List<CString>) Main@FormatEntry (@Result (@EList-2 CString List<CString>))))) (@Result (@EList-2 CString List<CString>))
    (let ((ll (List<Main@FormatEntry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Main@FormatEntry>-value ll)))
            (@Result-ok init)
        )
        (ite (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@FormatEntry>-value ll)))
        (let ((@tmpe-r0 (select f init (ListOps@Vector1<Main@FormatEntry>-i0 $ll)))) (ite (not (is-@Result-ok @tmpe-r0))  @tmpe-r0 (let ((r0 (@Result-value @tmpe-r0)))
            (@Result-ok r0)
        ))))
        (ite (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@FormatEntry>-value ll)))
        (let ((@tmpe-r0 (select f init (ListOps@Vector2<Main@FormatEntry>-i0 $ll)))) (ite (not (is-@Result-ok @tmpe-r0))  @tmpe-r0 (let ((r0 (@Result-value @tmpe-r0)))
        (let ((@tmpe-r1 (select f r0 (ListOps@Vector2<Main@FormatEntry>-i1 $ll)))) (ite (not (is-@Result-ok @tmpe-r1))  @tmpe-r1 (let ((r1 (@Result-value @tmpe-r1)))
            (@Result-ok r1)
        )))))))
                (let (($ll (@Term-ListOps@Vector3<Main@FormatEntry>-value ll)))
        (let ((@tmpe-r0 (select f init (ListOps@Vector3<Main@FormatEntry>-i0 $ll)))) (ite (not (is-@Result-ok @tmpe-r0))  @tmpe-r0 (let ((r0 (@Result-value @tmpe-r0)))
        (let ((@tmpe-r1 (select f r0 (ListOps@Vector3<Main@FormatEntry>-i1 $ll)))) (ite (not (is-@Result-ok @tmpe-r1))  @tmpe-r1 (let ((r1 (@Result-value @tmpe-r1)))
        (let ((@tmpe-r2 (select f r1 (ListOps@Vector3<Main@FormatEntry>-i2 $ll)))) (ite (not (is-@Result-ok @tmpe-r2))  @tmpe-r2 (let ((r2 (@Result-value @tmpe-r2)))
            (@Result-ok r2)
        )))))))))))))
    )
)

(define-fun Core@ListOps@s_list_set<Main@Entry> ((l List<Main@Entry>) (idx Nat) (v Main@Entry)) List<Main@Entry>
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Main@Entry>-value ll)))
            (List<Main@Entry>-mk (@Term-ListOps@Vector1<Main@Entry>-mk (ListOps@Vector1<Main@Entry>-mk v)))
        )
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Main@Entry>-value ll)))
            (ite (= idx 0)
                (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk v (ListOps@Vector2<Main@Entry>-i1 $ll))))
                (List<Main@Entry>-mk (@Term-ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-mk (ListOps@Vector2<Main@Entry>-i0 $ll) v)))
            )
        )
                (let (($ll ll))
        (let ((ll3 (@Term-ListOps@Vector3<Main@Entry>-value $ll)))
            (ite (= idx 0)
                (List<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-mk v (ListOps@Vector3<Main@Entry>-i1 ll3) (ListOps@Vector3<Main@Entry>-i2 ll3))))
                (ite (= idx 1)
                    (List<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i0 ll3) v (ListOps@Vector3<Main@Entry>-i2 ll3))))
                    (List<Main@Entry>-mk (@Term-ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-mk (ListOps@Vector3<Main@Entry>-i0 ll3) (ListOps@Vector3<Main@Entry>-i1 ll3) v)))
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_shiftk<CString> ((l List<CString>) (k Nat)) List<CString>
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<CString>-value ll)))
            (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
        )
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<CString>-value ll)))
            (ite (= k 0)
                l
                (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
            )
        )
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<CString>-value ll)))
            (ite (= k 0)
                l
                (ite (= k 1)
                    (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk (ListOps@Vector2<CString>-i1 $ll))))
                    (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
                )
            )
        )
                (let (($ll (@Term-ListOps@Vector3<CString>-value ll)))
            (ite (= k 0)
                l
                (ite (= k 2)
                    (List<CString>-mk (@Term-ListOps@Vector1<CString>-mk (ListOps@Vector1<CString>-mk (ListOps@Vector3<CString>-i2 $ll))))
                    (ite (= k 1)
                        (List<CString>-mk (@Term-ListOps@Vector2<CString>-mk (ListOps@Vector2<CString>-mk (ListOps@Vector3<CString>-i1 $ll) (ListOps@Vector3<CString>-i2 $ll))))
                        (List<CString>-mk (@Term-ListOps@Vector0<CString>-mk ListOps@Vector0<CString>-mk))
                    )
                )
            )
        ))))
    )
)

(define-fun Core@ListOps@s_list_size<CString> ((l List<CString>)) Nat
    (let ((ll (List<CString>-value l)))
        (ite (is-@Term-ListOps@Vector0<CString>-mk ll)
        0
        (ite (is-@Term-ListOps@Vector1<CString>-mk ll)
        1
        (ite (is-@Term-ListOps@Vector2<CString>-mk ll)
        2
                3)))
    )
)

(define-fun Core@ListOps@s_list_size<Main@Entry> ((l List<Main@Entry>)) Nat
    (let ((ll (List<Main@Entry>-value l)))
        (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk ll)
        0
        (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk ll)
        1
        (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk ll)
        2
                3)))
    )
)

(define-fun Core@ListOps@s_list_size<Nat> ((l List<Nat>)) Nat
    (let ((ll (List<Nat>-value l)))
        (ite (is-@Term-ListOps@Vector0<Nat>-mk ll)
        0
        (ite (is-@Term-ListOps@Vector1<Nat>-mk ll)
        1
        (ite (is-@Term-ListOps@Vector2<Nat>-mk ll)
        2
                3)))
    )
)

(define-fun Core@ListOps@s_list_sum<Nat> ((l List<Nat>) (zero Nat)) Nat
    (let ((ll (List<Nat>-value l)))
        (ite (is-@Term-ListOps@Vector0<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector0<Nat>-value ll)))
            zero
        )
        (ite (is-@Term-ListOps@Vector1<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector1<Nat>-value ll)))
            (ListOps@Vector1<Nat>-i0 $ll)
        )
        (ite (is-@Term-ListOps@Vector2<Nat>-mk ll)
        (let (($ll (@Term-ListOps@Vector2<Nat>-value ll)))
            (+ (ListOps@Vector2<Nat>-i0 $ll) (ListOps@Vector2<Nat>-i1 $ll))
        )
                (let (($ll (@Term-ListOps@Vector3<Nat>-value ll)))
            (+ (+ (ListOps@Vector3<Nat>-i0 $ll) (ListOps@Vector3<Nat>-i1 $ll)) (ListOps@Vector3<Nat>-i2 $ll))
        ))))
    )
)

(define-fun List<CString>@firstK ((this List<CString>) (n Nat)) List<CString>
    (ite (Core@ListOps@s_list_empty<CString> this)
        Core@ListOps@s_list_create_empty<CString>
        (Core@ListOps@s_list_firstk<CString> this n)
    )
)

(define-fun List<CString>@size ((this List<CString>)) Nat
    (Core@ListOps@s_list_size<CString> this)
)

(define-fun List<CString>@firstK_$_precond0 ((this List<CString>) (n Nat)) Bool
    (<= n (List<CString>@size this))
)

(define-fun List<CString>@get ((this List<CString>) (i Nat)) CString
    (Core@ListOps@s_list_get<CString> this i)
)

(define-fun List<CString>@get_$_precond0 ((this List<CString>) (i Nat)) Bool
    (< i (List<CString>@size this))
)

(define-fun List<CString>@mapIdx<CString>$T ((this List<CString>) (f (Array CString Nat (@Result CString)))) (@Result List<CString>)
    (ite (Core@ListOps@s_list_empty<CString> this)
        (@Result-ok Core@ListOps@s_list_create_empty<CString>)
        (Core@ListOps@s_list_mapidx<CString-CString>$T this f)
    )
)

(define-fun List<CString>@shiftK ((this List<CString>) (n Nat)) List<CString>
    (ite (Core@ListOps@s_list_empty<CString> this)
        Core@ListOps@s_list_create_empty<CString>
        (Core@ListOps@s_list_shiftk<CString> this n)
    )
)

(define-fun List<CString>@shiftK_$_precond0 ((this List<CString>) (n Nat)) Bool
    (<= n (List<CString>@size this))
)

(define-fun List<Main@Entry>@allOf ((this List<Main@Entry>) (p (Array Main@Entry Bool))) Bool
    (ite (Core@ListOps@s_list_empty<Main@Entry> this)
        true
        (Core@ListOps@s_list_allof<Main@Entry> this p)
    )
)

(define-fun List<Main@Entry>@delete ((this List<Main@Entry>) (i Nat)) List<Main@Entry>
    (Core@ListOps@s_list_delete<Main@Entry> this i)
)

(define-fun List<Main@Entry>@size ((this List<Main@Entry>)) Nat
    (Core@ListOps@s_list_size<Main@Entry> this)
)

(define-fun List<Main@Entry>@delete_$_precond0 ((this List<Main@Entry>) (i Nat)) Bool
    (< i (List<Main@Entry>@size this))
)

(define-fun List<Main@Entry>@empty ((this List<Main@Entry>)) Bool
    (Core@ListOps@s_list_empty<Main@Entry> this)
)

(define-fun List<Main@Entry>@get ((this List<Main@Entry>) (i Nat)) Main@Entry
    (Core@ListOps@s_list_get<Main@Entry> this i)
)

(define-fun List<Main@Entry>@get_$_precond0 ((this List<Main@Entry>) (i Nat)) Bool
    (< i (List<Main@Entry>@size this))
)

(define-fun List<Main@Entry>@lastIndex ((this List<Main@Entry>)) (@Result Nat)
    (ite (< (Core@ListOps@s_list_size<Main@Entry> this) 1) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (Core@ListOps@s_list_size<Main@Entry> this) 1)))
)

(define-fun List<Main@Entry>@lastIndex_$_precond0 ((this List<Main@Entry>)) Bool
    (not (List<Main@Entry>@empty this))
)

(define-fun List<Main@Entry>@mapIdx<Nat> ((this List<Main@Entry>) (f (Array Main@Entry Nat Nat))) List<Nat>
    (ite (Core@ListOps@s_list_empty<Main@Entry> this)
        Core@ListOps@s_list_create_empty<Nat>
        (Core@ListOps@s_list_mapidx<Main@Entry-Nat> this f)
    )
)

(define-fun List<Main@Entry>@pushBack ((this List<Main@Entry>) (v Main@Entry)) (@Result List<Main@Entry>)
    (Core@ListOps@s_list_push_back<Main@Entry> this v)
)

(define-fun List<Main@Entry>@set ((this List<Main@Entry>) (i Nat) (v Main@Entry)) List<Main@Entry>
    (Core@ListOps@s_list_set<Main@Entry> this i v)
)

(define-fun List<Main@Entry>@set_$_precond0 ((this List<Main@Entry>) (i Nat) (v Main@Entry)) Bool
    (< i (List<Main@Entry>@size this))
)

(define-fun List<Main@FormatEntry>@map<Nat> ((this List<Main@FormatEntry>) (f (Array Main@FormatEntry Nat))) List<Nat>
    (ite (Core@ListOps@s_list_empty<Main@FormatEntry> this)
        Core@ListOps@s_list_create_empty<Nat>
        (Core@ListOps@s_list_map<Main@FormatEntry-Nat> this f)
    )
)

(define-fun List<Main@FormatEntry>@reduce<@EList<CString-List<CString>>>$T ((this List<Main@FormatEntry>) (init (@EList-2 CString List<CString>)) (f (Array (@EList-2 CString List<CString>) Main@FormatEntry (@Result (@EList-2 CString List<CString>))))) (@Result (@EList-2 CString List<CString>))
    (ite (Core@ListOps@s_list_empty<Main@FormatEntry> this)
        (@Result-ok init)
        (Core@ListOps@s_list_reduce<Main@FormatEntry-@EList<CString-List<CString>>>$T this init f)
    )
)

(define-fun List<Nat>@allOf ((this List<Nat>) (p (Array Nat Bool))) Bool
    (ite (Core@ListOps@s_list_empty<Nat> this)
        true
        (Core@ListOps@s_list_allof<Nat> this p)
    )
)

(define-fun List<Nat>@get ((this List<Nat>) (i Nat)) Nat
    (Core@ListOps@s_list_get<Nat> this i)
)

(define-fun List<Nat>@size ((this List<Nat>)) Nat
    (Core@ListOps@s_list_size<Nat> this)
)

(define-fun List<Nat>@get_$_precond0 ((this List<Nat>) (i Nat)) Bool
    (< i (List<Nat>@size this))
)

(define-fun List<Nat>@sum ((this List<Nat>)) Nat
    (ite (Core@ListOps@s_list_empty<Nat> this)
        0
        (Core@ListOps@s_list_sum<Nat> this 0)
    )
)

(define-fun Main@DatabaseIndex_$_invariant0 (($imap List<Nat>) ($curr Nat)) Bool
    (< $curr (List<Nat>@size $imap))
)

(define-fun Main@Database@indexQuickCheck ((idx @Term) (entrycount Nat)) Bool
    (ite (= idx @Term-None-mk)
        (let (($idx none))
            true
        )
        (let (($idx (Some<Main@DatabaseIndex>-value (@Term-Some<Main@DatabaseIndex>-value idx))))
            (ite (= (List<Nat>@size (Main@DatabaseIndex-imap $idx)) entrycount) (< (Main@DatabaseIndex-curr $idx) entrycount) false)
        )
    )
)

(define-fun Main@Database_$_invariant0 (($dbname @Term) ($entries List<Main@Entry>) ($format Main@Format) ($index @Term) ($fnum @Term)) Bool
    (Main@Database@indexQuickCheck $index (List<Main@Entry>@size $entries))
)

(define-fun Main@Database@setIndex ((this Main@Database) (atidx Nat)) (@Result (@EList-2 Main@Database Main@DatabaseIndex))
    (let ((nidx (List<Main@Entry>@mapIdx<Nat> (Main@Database-entries this) (lambda ((e Main@Entry) (i Nat)) i))))
    (let ((@tmpe-nindex (ite (not (Main@DatabaseIndex_$_invariant0 nidx atidx)) ((as @Result-err (@Result Main@DatabaseIndex)) @err-other) (@Result-ok (Main@DatabaseIndex-mk nidx atidx))))) (ite (not (is-@Result-ok @tmpe-nindex))  ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmpe-nindex)) (let ((nindex (@Result-value @tmpe-nindex)))
        (let ((@tmp-1-0 (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Term-Some<Main@DatabaseIndex>-mk (Some<Main@DatabaseIndex>-mk nindex)) (Main@Database-fnum this))) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Term-Some<Main@DatabaseIndex>-mk (Some<Main@DatabaseIndex>-mk nindex)) (Main@Database-fnum this)))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmp-1-0)) (@Result-ok (@EList-2-mk (@Result-value @tmp-1-0) nindex))))
    ))))
)

(define-fun Main@Database@ensureIndex ((this Main@Database) (atidx @Term)) (@Result (@EList-2 Main@Database Main@DatabaseIndex))
    (ite (= (Main@Database-index this) @Term-None-mk)
        (let (($tidx none))
            (ite (= atidx @Term-None-mk)
                (let (($atidx none))
                    (Main@Database@setIndex this 0)
                )
                (let (($atidx (Some<Nat>-value (@Term-Some<Nat>-value atidx))))
                    (Main@Database@setIndex this $atidx)
                )
            )
        )
        (let (($tidx (Some<Main@DatabaseIndex>-value (@Term-Some<Main@DatabaseIndex>-value (Main@Database-index this)))))
            (ite (= atidx @Term-None-mk)
                (let (($atidx none))
                    (@Result-ok (@EList-2-mk this $tidx))
                )
                (let (($atidx (Some<Nat>-value (@Term-Some<Nat>-value atidx))))
                (let ((@tmpe-dbidx (ite (not (Main@DatabaseIndex_$_invariant0 (Main@DatabaseIndex-imap $tidx) $atidx)) ((as @Result-err (@Result Main@DatabaseIndex)) @err-other) (@Result-ok (Main@DatabaseIndex-mk (Main@DatabaseIndex-imap $tidx) $atidx))))) (ite (not (is-@Result-ok @tmpe-dbidx))  ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmpe-dbidx)) (let ((dbidx (@Result-value @tmpe-dbidx)))
                    (let ((@tmp-1-0 (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Term-Some<Main@DatabaseIndex>-mk (Some<Main@DatabaseIndex>-mk dbidx)) (Main@Database-fnum this))) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Term-Some<Main@DatabaseIndex>-mk (Some<Main@DatabaseIndex>-mk dbidx)) (Main@Database-fnum this)))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmp-1-0)) (@Result-ok (@EList-2-mk (@Result-value @tmp-1-0) dbidx))))
                ))))
            )
        )
    )
)

(define-fun Main@Database@indexFullCheck ((idx @Term) (entrycount Nat)) Bool
    (ite (= idx @Term-None-mk)
        (let (($idx none))
            true
        )
        (let (($idx (Some<Main@DatabaseIndex>-value (@Term-Some<Main@DatabaseIndex>-value idx))))
            (ite (ite (>= (Main@DatabaseIndex-curr $idx) entrycount) true (not (= (List<Nat>@size (Main@DatabaseIndex-imap $idx)) entrycount)))
                false
                (List<Nat>@allOf (Main@DatabaseIndex-imap $idx) (lambda ((i Nat)) (< i entrycount)))
            )
        )
    )
)

(define-fun Main@FormatEntry@format ((this Main@FormatEntry) (values List<CString>)) (@Result CString)
    (let ((@tmpe-parts (List<CString>@mapIdx<CString>$T (Main@FormatEntry-entries this) (lambda ((vv CString) (ii Nat)) (let ((@tmp-2-0 (let ((@tmp-1-2 (ite (not (List<CString>@get_$_precond0 values ii)) ((as @Result-err (@Result CString)) @err-other) (@Result-ok (List<CString>@get values ii))))) (ite (not (is-@Result-ok @tmp-1-2)) ((as @Result-err (@Result List<CString>)) (@Result-etag @tmp-1-2)) (@Result-ok (Core@ListOps@s_list_create_3<CString> vv ": " (@Result-value @tmp-1-2))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result CString)) (@Result-etag @tmp-2-0)) (@Result-ok (CString@concat (@Result-value @tmp-2-0))))))))) (ite (not (is-@Result-ok @tmpe-parts))  ((as @Result-err (@Result CString)) (@Result-etag @tmpe-parts)) (let ((parts (@Result-value @tmpe-parts)))
    (let ((iidt (CString@joinAll "
  " parts)))
    (let ((hdr (CString@concat (Core@ListOps@s_list_create_2<CString> (Main@FormatEntry-header this) "
"))))
        (@Result-ok (CString@concat (Core@ListOps@s_list_create_3<CString> hdr "  " iidt)))
    )))))
)

(define-fun Main@Format@formatEntry ((this Main@Format) (entry Main@Entry)) (@Result CString)
    (let ((@tmpe@mi (List<Main@FormatEntry>@reduce<@EList<CString-List<CString>>>$T (Main@Format-entries this) (@EList-2-mk "" (Main@Entry-items entry)) (lambda ((acc (@EList-2 CString List<CString>)) (fmt Main@FormatEntry)) (let ((@tmpe-fargs (ite (not (List<CString>@firstK_$_precond0 (@EList-2-1 acc) (List<CString>@size (Main@FormatEntry-entries fmt)))) ((as @Result-err (@Result List<CString>)) @err-other) (@Result-ok (List<CString>@firstK (@EList-2-1 acc) (List<CString>@size (Main@FormatEntry-entries fmt))))))) (ite (not (is-@Result-ok @tmpe-fargs))  ((as @Result-err (@Result (@EList-2 CString List<CString>))) (@Result-etag @tmpe-fargs)) (let ((fargs (@Result-value @tmpe-fargs))) (let ((@tmpe-rrest (ite (not (List<CString>@shiftK_$_precond0 (@EList-2-1 acc) (List<CString>@size (Main@FormatEntry-entries fmt)))) ((as @Result-err (@Result List<CString>)) @err-other) (@Result-ok (List<CString>@shiftK (@EList-2-1 acc) (List<CString>@size (Main@FormatEntry-entries fmt))))))) (ite (not (is-@Result-ok @tmpe-rrest))  ((as @Result-err (@Result (@EList-2 CString List<CString>))) (@Result-etag @tmpe-rrest)) (let ((rrest (@Result-value @tmpe-rrest))) (let ((@tmpe-formatted (let ((@tmp-2-0 (let ((@tmp-1-2 (Main@FormatEntry@format fmt fargs))) (ite (not (is-@Result-ok @tmp-1-2)) ((as @Result-err (@Result List<CString>)) (@Result-etag @tmp-1-2)) (@Result-ok (Core@ListOps@s_list_create_3<CString> (@EList-2-0 acc) "
" (@Result-value @tmp-1-2))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result CString)) (@Result-etag @tmp-2-0)) (@Result-ok (CString@concat (@Result-value @tmp-2-0))))))) (ite (not (is-@Result-ok @tmpe-formatted))  ((as @Result-err (@Result (@EList-2 CString List<CString>))) (@Result-etag @tmpe-formatted)) (let ((formatted (@Result-value @tmpe-formatted))) (@Result-ok (@EList-2-mk formatted rrest))))))))))))))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result CString)) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((res (@EList-2-0 @mi@))) (let ((@_ (@EList-2-1 @mi@))) 
        (@Result-ok res)
    )))))
)

(define-fun Main@Database@printRec ((this Main@Database)) (@Result (@EList-2 CString Main@Database))
    (let ((@tmpe@mi (Main@Database@ensureIndex this @Term-None-mk))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
    (let ((@tmpe-entry (let ((@tmp-2-0 (ite (not (List<Nat>@get_$_precond0 (Main@DatabaseIndex-imap tidx) (Main@DatabaseIndex-curr tidx))) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (List<Nat>@get (Main@DatabaseIndex-imap tidx) (Main@DatabaseIndex-curr tidx)))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result Main@Entry)) (@Result-etag @tmp-2-0)) (ite (not (List<Main@Entry>@get_$_precond0 (Main@Database-entries tdb) (@Result-value @tmp-2-0))) ((as @Result-err (@Result Main@Entry)) @err-other) (@Result-ok (List<Main@Entry>@get (Main@Database-entries tdb) (@Result-value @tmp-2-0)))))))) (ite (not (is-@Result-ok @tmpe-entry))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe-entry)) (let ((entry (@Result-value @tmpe-entry)))
        (let ((@tmp-1-0 (Main@Format@formatEntry (Main@Database-format tdb) entry))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-1-0)) (@Result-ok (@EList-2-mk (@Result-value @tmp-1-0) tdb))))
    ))))))))
)

(define-fun Main@Database@processAdd ((this Main@Database) (op Main@AddOp)) (@Result Main@Database)
    (let ((@tmp-1-1 (List<Main@Entry>@pushBack (Main@Database-entries this) (Main@AddOp-entry op)))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Main@Database)) (@Result-etag @tmp-1-1)) (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (@Result-value @tmp-1-1) (Main@Database-format this) @Term-None-mk @Term-None-mk)) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (@Result-value @tmp-1-1) (Main@Database-format this) @Term-None-mk @Term-None-mk)))))
)

(define-fun Main@Database@processAdd_$_precond0 ((this Main@Database) (op Main@AddOp)) Bool
    (= (List<CString>@size (Main@Entry-items (Main@AddOp-entry op))) (Main@Format-vcount (Main@Database-format this)))
)

(define-fun Main@Database@processEnd ((this Main@Database) (op Main@EndOp)) (@Result (@EList-2 CString Main@Database))
    (ite (List<Main@Entry>@empty (Main@Database-entries this))
        (@Result-ok (@EList-2-mk "Empty DB" this))
        (let ((@tmpe@mi (let ((@tmp-3-0 (let ((@tmp-2-0 (let ((@tmp-1-0 (ite (not (List<Main@Entry>@lastIndex_$_precond0 (Main@Database-entries this))) ((as @Result-err (@Result Nat)) @err-other) (List<Main@Entry>@lastIndex (Main@Database-entries this))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result Some<Nat>)) (@Result-etag @tmp-1-0)) (@Result-ok (Some<Nat>-mk (@Result-value @tmp-1-0))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result @Term)) (@Result-etag @tmp-2-0)) (@Result-ok (@Term-Some<Nat>-mk (@Result-value @tmp-2-0))))))) (ite (not (is-@Result-ok @tmp-3-0)) ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmp-3-0)) (Main@Database@ensureIndex this (@Result-value @tmp-3-0)))))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
                (let ((tdb (@EList-2-0 @mi@))) (let ((@_ (@EList-2-1 @mi@))) 
            (Main@Database@printRec tdb)
        )))))
    )
)

(define-fun Main@Database@processGotoRecord ((this Main@Database) (op Main@GotoRecordOp)) (@Result (@EList-2 CString Main@Database))
    (ite (ite (< (List<Main@Entry>@size (Main@Database-entries this)) (Main@GotoRecordOp-ridx op)) true (= 0 (Main@GotoRecordOp-ridx op)))
        (@Result-ok (@EList-2-mk "Invalid Record Index" this))
        (let ((@tmpe@mi (let ((@tmp-3-0 (let ((@tmp-2-0 (let ((@tmp-1-0 (ite (< (Main@GotoRecordOp-ridx op) 1) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (Main@GotoRecordOp-ridx op) 1))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result Some<Nat>)) (@Result-etag @tmp-1-0)) (@Result-ok (Some<Nat>-mk (@Result-value @tmp-1-0))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result @Term)) (@Result-etag @tmp-2-0)) (@Result-ok (@Term-Some<Nat>-mk (@Result-value @tmp-2-0))))))) (ite (not (is-@Result-ok @tmp-3-0)) ((as @Result-err (@Result (@EList-2 Main@Database Main@DatabaseIndex))) (@Result-etag @tmp-3-0)) (Main@Database@ensureIndex this (@Result-value @tmp-3-0)))))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
                (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
            (Main@Database@printRec tdb)
        )))))
    )
)

(define-fun Main@Database@processList ((this Main@Database) (op Main@ListOp)) (@Result (@EList-2 CString Main@Database))
    (ite (List<Main@Entry>@empty (Main@Database-entries this))
        (@Result-ok (@EList-2-mk "Empty DB" this))
        (let ((@tmpe@mi (Main@Database@ensureIndex this (@Term-Some<Nat>-mk (Some<Nat>-mk 0))))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
                (let ((tdb (@EList-2-0 @mi@))) (let ((@_ (@EList-2-1 @mi@))) 
            (Main@Database@printRec tdb)
        )))))
    )
)

(define-fun Main@Database@processModify ((this Main@Database) (op Main@ModifyOp)) (@Result Main@Database)
    (ite (= (Main@Database-index this) @Term-None-mk)
        (let (($idx none))
            (@Result-ok this)
        )
        (let (($idx (Some<Main@DatabaseIndex>-value (@Term-Some<Main@DatabaseIndex>-value (Main@Database-index this)))))
            (let ((@tmp-2-1 (let ((@tmp-1-0 (ite (not (List<Nat>@get_$_precond0 (Main@DatabaseIndex-imap $idx) (Main@DatabaseIndex-curr $idx))) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (List<Nat>@get (Main@DatabaseIndex-imap $idx) (Main@DatabaseIndex-curr $idx)))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result List<Main@Entry>)) (@Result-etag @tmp-1-0)) (ite (not (List<Main@Entry>@set_$_precond0 (Main@Database-entries this) (@Result-value @tmp-1-0) (Main@ModifyOp-entry op))) ((as @Result-err (@Result List<Main@Entry>)) @err-other) (@Result-ok (List<Main@Entry>@set (Main@Database-entries this) (@Result-value @tmp-1-0) (Main@ModifyOp-entry op)))))))) (ite (not (is-@Result-ok @tmp-2-1)) ((as @Result-err (@Result Main@Database)) (@Result-etag @tmp-2-1)) (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (@Result-value @tmp-2-1) (Main@Database-format this) (Main@Database-index this) @Term-None-mk)) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (@Result-value @tmp-2-1) (Main@Database-format this) (Main@Database-index this) @Term-None-mk)))))
        )
    )
)

(define-fun Main@Database@setCurr ((this Main@Database) (atIdx Nat)) (@Result Main@Database)
    (let ((@tmp-5-3 (let ((@tmp-4-0 (let ((@tmp-3-0 (let ((@tmp-2-0 (let ((@tmp-1 (ite (not (not (= (Main@Database-index this) @Term-None-mk))) ((as @Result-err (@Result Main@DatabaseIndex)) @err-other) (@Result-ok (Some<Main@DatabaseIndex>-value (@Term-Some<Main@DatabaseIndex>-value (Main@Database-index this))))))) (ite (not (is-@Result-ok @tmp-1)) ((as @Result-err (@Result List<Nat>)) (@Result-etag @tmp-1)) (@Result-ok (Main@DatabaseIndex-imap (@Result-value @tmp-1))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result Main@DatabaseIndex)) (@Result-etag @tmp-2-0)) (ite (not (Main@DatabaseIndex_$_invariant0 (@Result-value @tmp-2-0) atIdx)) ((as @Result-err (@Result Main@DatabaseIndex)) @err-other) (@Result-ok (Main@DatabaseIndex-mk (@Result-value @tmp-2-0) atIdx))))))) (ite (not (is-@Result-ok @tmp-3-0)) ((as @Result-err (@Result Some<Main@DatabaseIndex>)) (@Result-etag @tmp-3-0)) (@Result-ok (Some<Main@DatabaseIndex>-mk (@Result-value @tmp-3-0))))))) (ite (not (is-@Result-ok @tmp-4-0)) ((as @Result-err (@Result @Term)) (@Result-etag @tmp-4-0)) (@Result-ok (@Term-Some<Main@DatabaseIndex>-mk (@Result-value @tmp-4-0))))))) (ite (not (is-@Result-ok @tmp-5-3)) ((as @Result-err (@Result Main@Database)) (@Result-etag @tmp-5-3)) (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Result-value @tmp-5-3) (Main@Database-fnum this))) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (Main@Database-entries this) (Main@Database-format this) (@Result-value @tmp-5-3) (Main@Database-fnum this))))))
)

(define-fun Main@Database@setCurr_$_precond0 ((this Main@Database) (atIdx Nat)) Bool
    (not (= (Main@Database-index this) @Term-None-mk))
)

(define-fun Main@Database@processNext ((this Main@Database) (op Main@NextOp)) (@Result (@EList-2 CString Main@Database))
    (let ((@tmpe@mi (Main@Database@ensureIndex this @Term-None-mk))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
        (let ((@tmp-2-0 (let ((@tmp-1-1 (ite (not (List<Main@Entry>@lastIndex_$_precond0 (Main@Database-entries this))) ((as @Result-err (@Result Nat)) @err-other) (List<Main@Entry>@lastIndex (Main@Database-entries this))))) (ite (not (is-@Result-ok @tmp-1-1)) ((as @Result-err (@Result Bool)) (@Result-etag @tmp-1-1)) (@Result-ok (>= (Main@DatabaseIndex-curr tidx) (@Result-value @tmp-1-1))))))) (ite (not (is-@Result-ok @tmp-2-0)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-2-0)) 
        (ite (@Result-value @tmp-2-0)
            (@Result-ok (@EList-2-mk "" tdb))
            (let ((@tmpe-ntdb (ite (not (Main@Database@setCurr_$_precond0 tdb (+ (Main@DatabaseIndex-curr tidx) 1))) ((as @Result-err (@Result Main@Database)) @err-other) (Main@Database@setCurr tdb (+ (Main@DatabaseIndex-curr tidx) 1))))) (ite (not (is-@Result-ok @tmpe-ntdb))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe-ntdb)) (let ((ntdb (@Result-value @tmpe-ntdb)))
                (Main@Database@printRec ntdb)
            )))
        )
    )))))))
)

(define-fun Nat@toCString ((this Nat)) CString
    (Core@NumericOps@s_natToCString this)
)

(define-fun Main@Database@processNumRecords ((this Main@Database) (op Main@NumRecordsOp)) CString
    (CString@concat (Core@ListOps@s_list_create_2<CString> "Records Count: " (Nat@toCString (List<Main@Entry>@size (Main@Database-entries this)))))
)

(define-fun Main@Database@processPrevious ((this Main@Database) (op Main@PreviousOp)) (@Result (@EList-2 CString Main@Database))
    (let ((@tmpe@mi (Main@Database@ensureIndex this @Term-None-mk))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
        (ite (= (Main@DatabaseIndex-curr tidx) 0)
            (@Result-ok (@EList-2-mk "" tdb))
            (let ((@tmpe-ntdb (let ((@tmp-1-0 (ite (< (Main@DatabaseIndex-curr tidx) 1) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (- (Main@DatabaseIndex-curr tidx) 1))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result Main@Database)) (@Result-etag @tmp-1-0)) (ite (not (Main@Database@setCurr_$_precond0 tdb (@Result-value @tmp-1-0))) ((as @Result-err (@Result Main@Database)) @err-other) (Main@Database@setCurr tdb (@Result-value @tmp-1-0))))))) (ite (not (is-@Result-ok @tmpe-ntdb))  ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmpe-ntdb)) (let ((ntdb (@Result-value @tmpe-ntdb)))
                (Main@Database@printRec ntdb)
            )))
        )
    )))))
)

(define-fun Main@Database@processRemove ((this Main@Database) (op Main@RemoveOp)) (@Result Main@Database)
    (ite (List<Main@Entry>@empty (Main@Database-entries this))
        (@Result-ok this)
        (let ((@tmpe@mi (Main@Database@ensureIndex this @Term-None-mk))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result Main@Database)) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
                (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
            (let ((@tmp-2-1 (let ((@tmp-1-0 (ite (not (List<Nat>@get_$_precond0 (Main@DatabaseIndex-imap tidx) (Main@DatabaseIndex-curr tidx))) ((as @Result-err (@Result Nat)) @err-other) (@Result-ok (List<Nat>@get (Main@DatabaseIndex-imap tidx) (Main@DatabaseIndex-curr tidx)))))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result List<Main@Entry>)) (@Result-etag @tmp-1-0)) (ite (not (List<Main@Entry>@delete_$_precond0 (Main@Database-entries this) (@Result-value @tmp-1-0))) ((as @Result-err (@Result List<Main@Entry>)) @err-other) (@Result-ok (List<Main@Entry>@delete (Main@Database-entries this) (@Result-value @tmp-1-0)))))))) (ite (not (is-@Result-ok @tmp-2-1)) ((as @Result-err (@Result Main@Database)) (@Result-etag @tmp-2-1)) (ite (not (Main@Database_$_invariant0 (Main@Database-dbname this) (@Result-value @tmp-2-1) (Main@Database-format this) @Term-None-mk @Term-None-mk)) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk (Main@Database-dbname this) (@Result-value @tmp-2-1) (Main@Database-format this) @Term-None-mk @Term-None-mk)))))
        )))))
    )
)

(define-fun Main@Database@processStatus ((this Main@Database) (op Main@StatusOp)) (@Result CString)
    (let ((@tmpe@mi (Main@Database@ensureIndex this @Term-None-mk))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result CString)) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((tdb (@EList-2-0 @mi@))) (let ((tidx (@EList-2-1 @mi@))) 
        (@Result-ok (CString@concat (Core@ListOps@s_list_create_3<CString> (Nat@toCString (Main@DatabaseIndex-curr tidx)) " of " (Nat@toCString (List<Main@Entry>@size (Main@Database-entries this))))))
    )))))
)

(define-fun Main@Database@processDatabaseOperation ((this Main@Database) (op @Term)) (@Result (@EList-2 CString Main@Database))
    (ite (is-@Term-Main@NumRecordsOp-mk op)
    (let (($op (@Term-Main@NumRecordsOp-value op)))
        (@Result-ok (@EList-2-mk (Main@Database@processNumRecords this $op) this))
    )
    (ite (is-@Term-Main@EndOp-mk op)
    (let (($op (@Term-Main@EndOp-value op)))
        (Main@Database@processEnd this $op)
    )
    (ite (is-@Term-Main@ListOp-mk op)
    (let (($op (@Term-Main@ListOp-value op)))
        (Main@Database@processList this $op)
    )
    (ite (is-@Term-Main@GotoRecordOp-mk op)
    (let (($op (@Term-Main@GotoRecordOp-value op)))
        (Main@Database@processGotoRecord this $op)
    )
    (ite (is-@Term-Main@NextOp-mk op)
    (let (($op (@Term-Main@NextOp-value op)))
        (Main@Database@processNext this $op)
    )
    (ite (is-@Term-Main@PreviousOp-mk op)
    (let (($op (@Term-Main@PreviousOp-value op)))
        (Main@Database@processPrevious this $op)
    )
    (ite (is-@Term-Main@StatusOp-mk op)
    (let (($op (@Term-Main@StatusOp-value op)))
        (let ((@tmp-1-0 (Main@Database@processStatus this $op))) (ite (not (is-@Result-ok @tmp-1-0)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-1-0)) (@Result-ok (@EList-2-mk (@Result-value @tmp-1-0) this))))
    )
    (ite (is-@Term-Main@AddOp-mk op)
    (let (($op (@Term-Main@AddOp-value op)))
        (let ((@tmp-2-1 (ite (not (Main@Database@processAdd_$_precond0 this $op)) ((as @Result-err (@Result Main@Database)) @err-other) (Main@Database@processAdd this $op)))) (ite (not (is-@Result-ok @tmp-2-1)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-2-1)) (@Result-ok (@EList-2-mk "" (@Result-value @tmp-2-1)))))
    )
    (ite (is-@Term-Main@ModifyOp-mk op)
    (let (($op (@Term-Main@ModifyOp-value op)))
        (let ((@tmp-3-1 (Main@Database@processModify this $op))) (ite (not (is-@Result-ok @tmp-3-1)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-3-1)) (@Result-ok (@EList-2-mk "" (@Result-value @tmp-3-1)))))
    )
        (let (($op (@Term-Main@RemoveOp-value op)))
        (let ((@tmp-4-1 (Main@Database@processRemove this $op))) (ite (not (is-@Result-ok @tmp-4-1)) ((as @Result-err (@Result (@EList-2 CString Main@Database))) (@Result-etag @tmp-4-1)) (@Result-ok (@EList-2-mk "" (@Result-value @tmp-4-1)))))
    ))))))))))
)

(define-fun Main@Database_$_validate0 (($dbname @Term) ($entries List<Main@Entry>) ($format Main@Format) ($index @Term) ($fnum @Term)) Bool
    (List<Main@Entry>@allOf $entries (lambda ((e Main@Entry)) (= (List<CString>@size (Main@Entry-items e)) (Main@Format-vcount $format))))
)

(define-fun Main@Database_$_validate1 (($dbname @Term) ($entries List<Main@Entry>) ($format Main@Format) ($index @Term) ($fnum @Term)) Bool
    (Main@Database@indexFullCheck $index (List<Main@Entry>@size $entries))
)

(define-fun Main@Format_$_invariant0 (($entries List<Main@FormatEntry>) ($vcount Nat)) Bool
    (= $vcount (List<Nat>@sum (List<Main@FormatEntry>@map<Nat> $entries (lambda ((ee Main@FormatEntry)) (List<CString>@size (Main@FormatEntry-entries ee))))))
)

(define-fun Main@getSampleDB () (@Result Main@Database)
    (let ((entries (Core@ListOps@s_list_create_3<Main@Entry> (Main@Entry-mk (Core@ListOps@s_list_create_3<CString> "Bosque" "Yes" "Hybrid")) (Main@Entry-mk (Core@ListOps@s_list_create_3<CString> "C++" "Yes" "Compiled")) (Main@Entry-mk (Core@ListOps@s_list_create_3<CString> "JavaScript" "No" "JIT")))))
    (let ((@tmpe-format (ite (not (Main@Format_$_invariant0 (Core@ListOps@s_list_create_2<Main@FormatEntry> (Main@FormatEntry-mk "PL" (Core@ListOps@s_list_create_1<CString> "Name")) (Main@FormatEntry-mk "Features" (Core@ListOps@s_list_create_2<CString> "Static Types" "Runtime"))) 3)) ((as @Result-err (@Result Main@Format)) @err-other) (@Result-ok (Main@Format-mk (Core@ListOps@s_list_create_2<Main@FormatEntry> (Main@FormatEntry-mk "PL" (Core@ListOps@s_list_create_1<CString> "Name")) (Main@FormatEntry-mk "Features" (Core@ListOps@s_list_create_2<CString> "Static Types" "Runtime"))) 3))))) (ite (not (is-@Result-ok @tmpe-format))  ((as @Result-err (@Result Main@Database)) (@Result-etag @tmpe-format)) (let ((format (@Result-value @tmpe-format)))
        (ite (not (Main@Database_$_invariant0 @Term-None-mk entries format @Term-None-mk @Term-None-mk)) ((as @Result-err (@Result Main@Database)) @err-other) (@Result-ok (Main@Database-mk @Term-None-mk entries format @Term-None-mk @Term-None-mk)))
    ))))
)

(define-fun Main@testOpOnSample ((op @Term)) (@Result CString)
    (let ((@tmpe-db Main@getSampleDB)) (ite (not (is-@Result-ok @tmpe-db))  ((as @Result-err (@Result CString)) (@Result-etag @tmpe-db)) (let ((db (@Result-value @tmpe-db)))
    (let ((@tmpe@mi (Main@Database@processDatabaseOperation db op))) (ite (not (is-@Result-ok @tmpe@mi))  ((as @Result-err (@Result CString)) (@Result-etag @tmpe@mi)) (let ((@mi@ (@Result-value @tmpe@mi)))
        (let ((res (@EList-2-0 @mi@))) (let ((@_ (@EList-2-1 @mi@))) 
        (@Result-ok res)
    ))))))))
)

(assert (= Nat@zero-cc-temp (@Result-ok 0)))
(assert (is-@Result-ok Nat@zero-cc-temp))
(assert (= Nat@zero (@Result-value Nat@zero-cc-temp)))
(assert (= Nat@one-cc-temp (@Result-ok 1)))
(assert (is-@Result-ok Nat@one-cc-temp))
(assert (= Nat@one (@Result-value Nat@one-cc-temp)))

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

(define-fun @Validate-ListOps@Vector3<CString> ((v ListOps@Vector3<CString>)) Bool
    (and (@Validate-CString (ListOps@Vector3<CString>-i0 v)) (@Validate-CString (ListOps@Vector3<CString>-i1 v)) (@Validate-CString (ListOps@Vector3<CString>-i2 v)))
)
(define-fun @Validate-ListOps@Vector2<CString> ((v ListOps@Vector2<CString>)) Bool
    (and (@Validate-CString (ListOps@Vector2<CString>-i0 v)) (@Validate-CString (ListOps@Vector2<CString>-i1 v)))
)
(define-fun @Validate-ListOps@Vector1<CString> ((v ListOps@Vector1<CString>)) Bool
    (@Validate-CString (ListOps@Vector1<CString>-i0 v))
)
(define-fun @Validate-ListOps@Vector0<CString> ((v ListOps@Vector0<CString>)) Bool
    true
)
(define-fun @Validate-ListOps@Vector<CString> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<CString>-mk v)
    (@Validate-ListOps@Vector0<CString> (@Term-ListOps@Vector0<CString>-value v))
    (ite (is-@Term-ListOps@Vector1<CString>-mk v)
    (@Validate-ListOps@Vector1<CString> (@Term-ListOps@Vector1<CString>-value v))
    (ite (is-@Term-ListOps@Vector2<CString>-mk v)
    (@Validate-ListOps@Vector2<CString> (@Term-ListOps@Vector2<CString>-value v))
    (ite (is-@Term-ListOps@Vector3<CString>-mk v)
    (@Validate-ListOps@Vector3<CString> (@Term-ListOps@Vector3<CString>-value v))
        false))))
)
(define-fun @Validate-List<CString> ((v List<CString>)) Bool
    (@Validate-ListOps@Vector<CString> (List<CString>-value v))
)
(define-fun @Validate-Main@Entry ((v Main@Entry)) Bool
    (@Validate-List<CString> (Main@Entry-items v))
)
(define-fun @Validate-ListOps@Vector3<Main@Entry> ((v ListOps@Vector3<Main@Entry>)) Bool
    (and (@Validate-Main@Entry (ListOps@Vector3<Main@Entry>-i0 v)) (@Validate-Main@Entry (ListOps@Vector3<Main@Entry>-i1 v)) (@Validate-Main@Entry (ListOps@Vector3<Main@Entry>-i2 v)))
)
(define-fun @Validate-ListOps@Vector2<Main@Entry> ((v ListOps@Vector2<Main@Entry>)) Bool
    (and (@Validate-Main@Entry (ListOps@Vector2<Main@Entry>-i0 v)) (@Validate-Main@Entry (ListOps@Vector2<Main@Entry>-i1 v)))
)
(define-fun @Validate-ListOps@Vector1<Main@Entry> ((v ListOps@Vector1<Main@Entry>)) Bool
    (@Validate-Main@Entry (ListOps@Vector1<Main@Entry>-i0 v))
)
(define-fun @Validate-ListOps@Vector0<Main@Entry> ((v ListOps@Vector0<Main@Entry>)) Bool
    true
)
(define-fun @Validate-ListOps@Vector<Main@Entry> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk v)
    (@Validate-ListOps@Vector0<Main@Entry> (@Term-ListOps@Vector0<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk v)
    (@Validate-ListOps@Vector1<Main@Entry> (@Term-ListOps@Vector1<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk v)
    (@Validate-ListOps@Vector2<Main@Entry> (@Term-ListOps@Vector2<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector3<Main@Entry>-mk v)
    (@Validate-ListOps@Vector3<Main@Entry> (@Term-ListOps@Vector3<Main@Entry>-value v))
        false))))
)
(define-fun @Validate-List<Main@Entry> ((v List<Main@Entry>)) Bool
    (@Validate-ListOps@Vector<Main@Entry> (List<Main@Entry>-value v))
)
(define-fun @Validate-Main@FormatEntry ((v Main@FormatEntry)) Bool
    (and (@Validate-CString (Main@FormatEntry-header v)) (@Validate-List<CString> (Main@FormatEntry-entries v)))
)
(define-fun @Validate-ListOps@Vector3<Main@FormatEntry> ((v ListOps@Vector3<Main@FormatEntry>)) Bool
    (and (@Validate-Main@FormatEntry (ListOps@Vector3<Main@FormatEntry>-i0 v)) (@Validate-Main@FormatEntry (ListOps@Vector3<Main@FormatEntry>-i1 v)) (@Validate-Main@FormatEntry (ListOps@Vector3<Main@FormatEntry>-i2 v)))
)
(define-fun @Validate-ListOps@Vector2<Main@FormatEntry> ((v ListOps@Vector2<Main@FormatEntry>)) Bool
    (and (@Validate-Main@FormatEntry (ListOps@Vector2<Main@FormatEntry>-i0 v)) (@Validate-Main@FormatEntry (ListOps@Vector2<Main@FormatEntry>-i1 v)))
)
(define-fun @Validate-ListOps@Vector1<Main@FormatEntry> ((v ListOps@Vector1<Main@FormatEntry>)) Bool
    (@Validate-Main@FormatEntry (ListOps@Vector1<Main@FormatEntry>-i0 v))
)
(define-fun @Validate-ListOps@Vector0<Main@FormatEntry> ((v ListOps@Vector0<Main@FormatEntry>)) Bool
    true
)
(define-fun @Validate-ListOps@Vector<Main@FormatEntry> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector0<Main@FormatEntry> (@Term-ListOps@Vector0<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector1<Main@FormatEntry> (@Term-ListOps@Vector1<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector2<Main@FormatEntry> (@Term-ListOps@Vector2<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector3<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector3<Main@FormatEntry> (@Term-ListOps@Vector3<Main@FormatEntry>-value v))
        false))))
)
(define-fun @Validate-List<Main@FormatEntry> ((v List<Main@FormatEntry>)) Bool
    (@Validate-ListOps@Vector<Main@FormatEntry> (List<Main@FormatEntry>-value v))
)
(define-fun @Validate-ListOps@Vector3<Nat> ((v ListOps@Vector3<Nat>)) Bool
    (and (@Validate-Nat (ListOps@Vector3<Nat>-i0 v)) (@Validate-Nat (ListOps@Vector3<Nat>-i1 v)) (@Validate-Nat (ListOps@Vector3<Nat>-i2 v)))
)
(define-fun @Validate-ListOps@Vector2<Nat> ((v ListOps@Vector2<Nat>)) Bool
    (and (@Validate-Nat (ListOps@Vector2<Nat>-i0 v)) (@Validate-Nat (ListOps@Vector2<Nat>-i1 v)))
)
(define-fun @Validate-ListOps@Vector1<Nat> ((v ListOps@Vector1<Nat>)) Bool
    (@Validate-Nat (ListOps@Vector1<Nat>-i0 v))
)
(define-fun @Validate-ListOps@Vector0<Nat> ((v ListOps@Vector0<Nat>)) Bool
    true
)
(define-fun @Validate-ListOps@Vector<Nat> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Nat>-mk v)
    (@Validate-ListOps@Vector0<Nat> (@Term-ListOps@Vector0<Nat>-value v))
    (ite (is-@Term-ListOps@Vector1<Nat>-mk v)
    (@Validate-ListOps@Vector1<Nat> (@Term-ListOps@Vector1<Nat>-value v))
    (ite (is-@Term-ListOps@Vector2<Nat>-mk v)
    (@Validate-ListOps@Vector2<Nat> (@Term-ListOps@Vector2<Nat>-value v))
    (ite (is-@Term-ListOps@Vector3<Nat>-mk v)
    (@Validate-ListOps@Vector3<Nat> (@Term-ListOps@Vector3<Nat>-value v))
        false))))
)
(define-fun @Validate-List<Nat> ((v List<Nat>)) Bool
    (@Validate-ListOps@Vector<Nat> (List<Nat>-value v))
)
(define-fun @Validate-Main@AddOp ((v Main@AddOp)) Bool
    (@Validate-Main@Entry (Main@AddOp-entry v))
)
(define-fun @Validate-Some<Nat> ((v Some<Nat>)) Bool
    (@Validate-Nat (Some<Nat>-value v))
)
(define-fun @Validate-Option<Nat> ((v @Term)) Bool
    (ite (is-@Term-Some<Nat>-mk v) (@Validate-Some<Nat> (@Term-Some<Nat>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Main@DatabaseIndex ((v Main@DatabaseIndex)) Bool
    (and (@Validate-List<Nat> (Main@DatabaseIndex-imap v)) (@Validate-Nat (Main@DatabaseIndex-curr v)) (Main@DatabaseIndex_$_invariant0 (Main@DatabaseIndex-imap v) (Main@DatabaseIndex-curr v)))
)
(define-fun @Validate-Some<Main@DatabaseIndex> ((v Some<Main@DatabaseIndex>)) Bool
    (@Validate-Main@DatabaseIndex (Some<Main@DatabaseIndex>-value v))
)
(define-fun @Validate-Option<Main@DatabaseIndex> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@DatabaseIndex>-mk v) (@Validate-Some<Main@DatabaseIndex> (@Term-Some<Main@DatabaseIndex>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Main@Format ((v Main@Format)) Bool
    (and (@Validate-List<Main@FormatEntry> (Main@Format-entries v)) (@Validate-Nat (Main@Format-vcount v)) (Main@Format_$_invariant0 (Main@Format-entries v) (Main@Format-vcount v)))
)
(define-fun @Validate-Some<CString> ((v Some<CString>)) Bool
    (@Validate-CString (Some<CString>-value v))
)
(define-fun @Validate-Option<CString> ((v @Term)) Bool
    (ite (is-@Term-Some<CString>-mk v) (@Validate-Some<CString> (@Term-Some<CString>-value v)) (= v @Term-None-mk))
)
(define-fun @Validate-Main@Database ((v Main@Database)) Bool
    (and (@Validate-Option<CString> (Main@Database-dbname v)) (@Validate-List<Main@Entry> (Main@Database-entries v)) (@Validate-Main@Format (Main@Database-format v)) (@Validate-Option<Main@DatabaseIndex> (Main@Database-index v)) (@Validate-Option<Nat> (Main@Database-fnum v)) (Main@Database_$_invariant0 (Main@Database-dbname v) (Main@Database-entries v) (Main@Database-format v) (Main@Database-index v) (Main@Database-fnum v)) (Main@Database_$_validate0 (Main@Database-dbname v) (Main@Database-entries v) (Main@Database-format v) (Main@Database-index v) (Main@Database-fnum v)) (Main@Database_$_validate1 (Main@Database-dbname v) (Main@Database-entries v) (Main@Database-format v) (Main@Database-index v) (Main@Database-fnum v)))
)
(define-fun @Validate-Main@RemoveOp ((v Main@RemoveOp)) Bool
    true
)
(define-fun @Validate-Main@ModifyOp ((v Main@ModifyOp)) Bool
    (@Validate-Main@Entry (Main@ModifyOp-entry v))
)
(define-fun @Validate-Main@StatusOp ((v Main@StatusOp)) Bool
    true
)
(define-fun @Validate-Main@PreviousOp ((v Main@PreviousOp)) Bool
    true
)
(define-fun @Validate-Main@NextOp ((v Main@NextOp)) Bool
    true
)
(define-fun @Validate-Main@GotoRecordOp ((v Main@GotoRecordOp)) Bool
    (@Validate-Nat (Main@GotoRecordOp-ridx v))
)
(define-fun @Validate-Main@ListOp ((v Main@ListOp)) Bool
    true
)
(define-fun @Validate-Main@EndOp ((v Main@EndOp)) Bool
    true
)
(define-fun @Validate-Main@NumRecordsOp ((v Main@NumRecordsOp)) Bool
    true
)
(define-fun @Validate-Main@DatabaseOperation ((v @Term)) Bool
    (ite (is-@Term-Main@NumRecordsOp-mk v)
    (@Validate-Main@NumRecordsOp (@Term-Main@NumRecordsOp-value v))
    (ite (is-@Term-Main@EndOp-mk v)
    (@Validate-Main@EndOp (@Term-Main@EndOp-value v))
    (ite (is-@Term-Main@ListOp-mk v)
    (@Validate-Main@ListOp (@Term-Main@ListOp-value v))
    (ite (is-@Term-Main@GotoRecordOp-mk v)
    (@Validate-Main@GotoRecordOp (@Term-Main@GotoRecordOp-value v))
    (ite (is-@Term-Main@NextOp-mk v)
    (@Validate-Main@NextOp (@Term-Main@NextOp-value v))
    (ite (is-@Term-Main@PreviousOp-mk v)
    (@Validate-Main@PreviousOp (@Term-Main@PreviousOp-value v))
    (ite (is-@Term-Main@StatusOp-mk v)
    (@Validate-Main@StatusOp (@Term-Main@StatusOp-value v))
    (ite (is-@Term-Main@AddOp-mk v)
    (@Validate-Main@AddOp (@Term-Main@AddOp-value v))
    (ite (is-@Term-Main@ModifyOp-mk v)
    (@Validate-Main@ModifyOp (@Term-Main@ModifyOp-value v))
    (ite (is-@Term-Main@RemoveOp-mk v)
    (@Validate-Main@RemoveOp (@Term-Main@RemoveOp-value v))
        false))))))))))
)
(define-fun @ValidateRoot-ListOps@Vector3<CString> ((v ListOps@Vector3<CString>)) Bool
    (@Validate-ListOps@Vector3<CString> v)
)
(define-fun @ValidateRoot-ListOps@Vector2<CString> ((v ListOps@Vector2<CString>)) Bool
    (@Validate-ListOps@Vector2<CString> v)
)
(define-fun @ValidateRoot-ListOps@Vector1<CString> ((v ListOps@Vector1<CString>)) Bool
    (@Validate-ListOps@Vector1<CString> v)
)
(define-fun @ValidateRoot-ListOps@Vector0<CString> ((v ListOps@Vector0<CString>)) Bool
    (@Validate-ListOps@Vector0<CString> v)
)
(define-fun @ValidateRoot-ListOps@Vector<CString> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<CString>-mk v)
    (@Validate-ListOps@Vector0<CString> (@Term-ListOps@Vector0<CString>-value v))
    (ite (is-@Term-ListOps@Vector1<CString>-mk v)
    (@Validate-ListOps@Vector1<CString> (@Term-ListOps@Vector1<CString>-value v))
    (ite (is-@Term-ListOps@Vector2<CString>-mk v)
    (@Validate-ListOps@Vector2<CString> (@Term-ListOps@Vector2<CString>-value v))
    (ite (is-@Term-ListOps@Vector3<CString>-mk v)
    (@Validate-ListOps@Vector3<CString> (@Term-ListOps@Vector3<CString>-value v))
        false))))
)
(define-fun @ValidateRoot-List<CString> ((v List<CString>)) Bool
    (@Validate-List<CString> v)
)
(define-fun @ValidateRoot-Main@Entry ((v Main@Entry)) Bool
    (@Validate-Main@Entry v)
)
(define-fun @ValidateRoot-ListOps@Vector3<Main@Entry> ((v ListOps@Vector3<Main@Entry>)) Bool
    (@Validate-ListOps@Vector3<Main@Entry> v)
)
(define-fun @ValidateRoot-ListOps@Vector2<Main@Entry> ((v ListOps@Vector2<Main@Entry>)) Bool
    (@Validate-ListOps@Vector2<Main@Entry> v)
)
(define-fun @ValidateRoot-ListOps@Vector1<Main@Entry> ((v ListOps@Vector1<Main@Entry>)) Bool
    (@Validate-ListOps@Vector1<Main@Entry> v)
)
(define-fun @ValidateRoot-ListOps@Vector0<Main@Entry> ((v ListOps@Vector0<Main@Entry>)) Bool
    (@Validate-ListOps@Vector0<Main@Entry> v)
)
(define-fun @ValidateRoot-ListOps@Vector<Main@Entry> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Main@Entry>-mk v)
    (@Validate-ListOps@Vector0<Main@Entry> (@Term-ListOps@Vector0<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector1<Main@Entry>-mk v)
    (@Validate-ListOps@Vector1<Main@Entry> (@Term-ListOps@Vector1<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector2<Main@Entry>-mk v)
    (@Validate-ListOps@Vector2<Main@Entry> (@Term-ListOps@Vector2<Main@Entry>-value v))
    (ite (is-@Term-ListOps@Vector3<Main@Entry>-mk v)
    (@Validate-ListOps@Vector3<Main@Entry> (@Term-ListOps@Vector3<Main@Entry>-value v))
        false))))
)
(define-fun @ValidateRoot-List<Main@Entry> ((v List<Main@Entry>)) Bool
    (@Validate-List<Main@Entry> v)
)
(define-fun @ValidateRoot-Main@FormatEntry ((v Main@FormatEntry)) Bool
    (@Validate-Main@FormatEntry v)
)
(define-fun @ValidateRoot-ListOps@Vector3<Main@FormatEntry> ((v ListOps@Vector3<Main@FormatEntry>)) Bool
    (@Validate-ListOps@Vector3<Main@FormatEntry> v)
)
(define-fun @ValidateRoot-ListOps@Vector2<Main@FormatEntry> ((v ListOps@Vector2<Main@FormatEntry>)) Bool
    (@Validate-ListOps@Vector2<Main@FormatEntry> v)
)
(define-fun @ValidateRoot-ListOps@Vector1<Main@FormatEntry> ((v ListOps@Vector1<Main@FormatEntry>)) Bool
    (@Validate-ListOps@Vector1<Main@FormatEntry> v)
)
(define-fun @ValidateRoot-ListOps@Vector0<Main@FormatEntry> ((v ListOps@Vector0<Main@FormatEntry>)) Bool
    (@Validate-ListOps@Vector0<Main@FormatEntry> v)
)
(define-fun @ValidateRoot-ListOps@Vector<Main@FormatEntry> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector0<Main@FormatEntry> (@Term-ListOps@Vector0<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector1<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector1<Main@FormatEntry> (@Term-ListOps@Vector1<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector2<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector2<Main@FormatEntry> (@Term-ListOps@Vector2<Main@FormatEntry>-value v))
    (ite (is-@Term-ListOps@Vector3<Main@FormatEntry>-mk v)
    (@Validate-ListOps@Vector3<Main@FormatEntry> (@Term-ListOps@Vector3<Main@FormatEntry>-value v))
        false))))
)
(define-fun @ValidateRoot-List<Main@FormatEntry> ((v List<Main@FormatEntry>)) Bool
    (@Validate-List<Main@FormatEntry> v)
)
(define-fun @ValidateRoot-ListOps@Vector3<Nat> ((v ListOps@Vector3<Nat>)) Bool
    (@Validate-ListOps@Vector3<Nat> v)
)
(define-fun @ValidateRoot-ListOps@Vector2<Nat> ((v ListOps@Vector2<Nat>)) Bool
    (@Validate-ListOps@Vector2<Nat> v)
)
(define-fun @ValidateRoot-ListOps@Vector1<Nat> ((v ListOps@Vector1<Nat>)) Bool
    (@Validate-ListOps@Vector1<Nat> v)
)
(define-fun @ValidateRoot-ListOps@Vector0<Nat> ((v ListOps@Vector0<Nat>)) Bool
    (@Validate-ListOps@Vector0<Nat> v)
)
(define-fun @ValidateRoot-ListOps@Vector<Nat> ((v @Term)) Bool
    (ite (is-@Term-ListOps@Vector0<Nat>-mk v)
    (@Validate-ListOps@Vector0<Nat> (@Term-ListOps@Vector0<Nat>-value v))
    (ite (is-@Term-ListOps@Vector1<Nat>-mk v)
    (@Validate-ListOps@Vector1<Nat> (@Term-ListOps@Vector1<Nat>-value v))
    (ite (is-@Term-ListOps@Vector2<Nat>-mk v)
    (@Validate-ListOps@Vector2<Nat> (@Term-ListOps@Vector2<Nat>-value v))
    (ite (is-@Term-ListOps@Vector3<Nat>-mk v)
    (@Validate-ListOps@Vector3<Nat> (@Term-ListOps@Vector3<Nat>-value v))
        false))))
)
(define-fun @ValidateRoot-List<Nat> ((v List<Nat>)) Bool
    (@Validate-List<Nat> v)
)
(define-fun @ValidateRoot-Main@AddOp ((v Main@AddOp)) Bool
    (@Validate-Main@AddOp v)
)
(define-fun @ValidateRoot-Some<Nat> ((v Some<Nat>)) Bool
    (@Validate-Some<Nat> v)
)
(define-fun @ValidateRoot-Option<Nat> ((v @Term)) Bool
    (ite (is-@Term-Some<Nat>-mk v) (@Validate-Some<Nat> (@Term-Some<Nat>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Main@DatabaseIndex ((v Main@DatabaseIndex)) Bool
    (@Validate-Main@DatabaseIndex v)
)
(define-fun @ValidateRoot-Some<Main@DatabaseIndex> ((v Some<Main@DatabaseIndex>)) Bool
    (@Validate-Some<Main@DatabaseIndex> v)
)
(define-fun @ValidateRoot-Option<Main@DatabaseIndex> ((v @Term)) Bool
    (ite (is-@Term-Some<Main@DatabaseIndex>-mk v) (@Validate-Some<Main@DatabaseIndex> (@Term-Some<Main@DatabaseIndex>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Main@Format ((v Main@Format)) Bool
    (@Validate-Main@Format v)
)
(define-fun @ValidateRoot-Some<CString> ((v Some<CString>)) Bool
    (@Validate-Some<CString> v)
)
(define-fun @ValidateRoot-Option<CString> ((v @Term)) Bool
    (ite (is-@Term-Some<CString>-mk v) (@Validate-Some<CString> (@Term-Some<CString>-value v)) (= v @Term-None-mk))
)
(define-fun @ValidateRoot-Main@Database ((v Main@Database)) Bool
    (@Validate-Main@Database v)
)
(define-fun @ValidateRoot-Main@RemoveOp ((v Main@RemoveOp)) Bool
    (@Validate-Main@RemoveOp v)
)
(define-fun @ValidateRoot-Main@ModifyOp ((v Main@ModifyOp)) Bool
    (@Validate-Main@ModifyOp v)
)
(define-fun @ValidateRoot-Main@StatusOp ((v Main@StatusOp)) Bool
    (@Validate-Main@StatusOp v)
)
(define-fun @ValidateRoot-Main@PreviousOp ((v Main@PreviousOp)) Bool
    (@Validate-Main@PreviousOp v)
)
(define-fun @ValidateRoot-Main@NextOp ((v Main@NextOp)) Bool
    (@Validate-Main@NextOp v)
)
(define-fun @ValidateRoot-Main@GotoRecordOp ((v Main@GotoRecordOp)) Bool
    (@Validate-Main@GotoRecordOp v)
)
(define-fun @ValidateRoot-Main@ListOp ((v Main@ListOp)) Bool
    (@Validate-Main@ListOp v)
)
(define-fun @ValidateRoot-Main@EndOp ((v Main@EndOp)) Bool
    (@Validate-Main@EndOp v)
)
(define-fun @ValidateRoot-Main@NumRecordsOp ((v Main@NumRecordsOp)) Bool
    (@Validate-Main@NumRecordsOp v)
)
(define-fun @ValidateRoot-Main@DatabaseOperation ((v @Term)) Bool
    (ite (is-@Term-Main@NumRecordsOp-mk v)
    (@Validate-Main@NumRecordsOp (@Term-Main@NumRecordsOp-value v))
    (ite (is-@Term-Main@EndOp-mk v)
    (@Validate-Main@EndOp (@Term-Main@EndOp-value v))
    (ite (is-@Term-Main@ListOp-mk v)
    (@Validate-Main@ListOp (@Term-Main@ListOp-value v))
    (ite (is-@Term-Main@GotoRecordOp-mk v)
    (@Validate-Main@GotoRecordOp (@Term-Main@GotoRecordOp-value v))
    (ite (is-@Term-Main@NextOp-mk v)
    (@Validate-Main@NextOp (@Term-Main@NextOp-value v))
    (ite (is-@Term-Main@PreviousOp-mk v)
    (@Validate-Main@PreviousOp (@Term-Main@PreviousOp-value v))
    (ite (is-@Term-Main@StatusOp-mk v)
    (@Validate-Main@StatusOp (@Term-Main@StatusOp-value v))
    (ite (is-@Term-Main@AddOp-mk v)
    (@Validate-Main@AddOp (@Term-Main@AddOp-value v))
    (ite (is-@Term-Main@ModifyOp-mk v)
    (@Validate-Main@ModifyOp (@Term-Main@ModifyOp-value v))
    (ite (is-@Term-Main@RemoveOp-mk v)
    (@Validate-Main@RemoveOp (@Term-Main@RemoveOp-value v))
        false))))))))))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(declare-const op @Term)
(assert (@ValidateRoot-Main@DatabaseOperation op))

(declare-fun MockTest (@Term) (@Result CString))

(declare-const result (@Result CString))

(assert (= (MockTest op) (Main@testOpOnSample op)))
(assert (= result (MockTest op)))

(check-sat)
(get-model)
