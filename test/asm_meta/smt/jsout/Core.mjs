"use strict";
let _$consts = {};

import { $VRepr, _$softfails, _$supertypes, _$fisSubtype, _$fisNotSubtype, _$fasSubtype, _$fasNotSubtype, _$None, _$not, _$negate, _$add, _$sub, _$mult, _$div, _$bval, _$fkeq, _$fkeqopt, _$fkneq, _$fkneqopt, _$fkless, _$fnumeq, _$fnumless, _$fnumlesseq, _$exhaustive, _$abort, _$assert, _$formatchk, _$invariant, _$validate, _$precond, _$softprecond, _$postcond, _$softpostcond, _$memoconstval, _$accepts } from "./runtime.mjs";
import { _$setnone_lit, _$parsemap, _$emitmap, _$parseBSQON, _$emitBSQON } from "./bsqon.mjs";

let _$rv = {};

export const CStringOps = {
    "s_empty": (s) => { return s === ""; },
    "s_size": (s) => { return BigInt(s.length); }
};

export const Option = {
    "<Main::EMAIL>": Object.create(Object.prototype, {
        $tsym: { value: Symbol.for("Option<Main::EMAIL>") },
        $create: { value: (value) => {
            return Object.create(Option["<Main::EMAIL>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { return parser.testAndConsumeIfNone() ? _$None : parser.parseValue("Some<Main::EMAIL>"); } },
        $emitAPI: { value: (emitter, value) => { return value.$emitAPI(emitter, value); } }
    }),
    "<Main::USER_NAME>": Object.create(Object.prototype, {
        $tsym: { value: Symbol.for("Option<Main::USER_NAME>") },
        $create: { value: (value) => {
            return Object.create(Option["<Main::USER_NAME>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { return parser.testAndConsumeIfNone() ? _$None : parser.parseValue("Some<Main::USER_NAME>"); } },
        $emitAPI: { value: (emitter, value) => { return value.$emitAPI(emitter, value); } }
    }),
    "<Main::USER_PHONE>": Object.create(Object.prototype, {
        $tsym: { value: Symbol.for("Option<Main::USER_PHONE>") },
        $create: { value: (value) => {
            return Object.create(Option["<Main::USER_PHONE>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { return parser.testAndConsumeIfNone() ? _$None : parser.parseValue("Some<Main::USER_PHONE>"); } },
        $emitAPI: { value: (emitter, value) => { return value.$emitAPI(emitter, value); } }
    }),
    "<Main::USER_ADDRESS>": Object.create(Object.prototype, {
        $tsym: { value: Symbol.for("Option<Main::USER_ADDRESS>") },
        $create: { value: (value) => {
            return Object.create(Option["<Main::USER_ADDRESS>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { return parser.testAndConsumeIfNone() ? _$None : parser.parseValue("Some<Main::USER_ADDRESS>"); } },
        $emitAPI: { value: (emitter, value) => { return value.$emitAPI(emitter, value); } }
    }),
    "<Int>": Object.create(Object.prototype, {
        $tsym: { value: Symbol.for("Option<Int>") },
        $create: { value: (value) => {
            return Object.create(Option["<Int>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { return parser.testAndConsumeIfNone() ? _$None : parser.parseValue("Some<Int>"); } },
        $emitAPI: { value: (emitter, value) => { return value.$emitAPI(emitter, value); } }
    })
}

export const None = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("None") }
});

export const Some = {
    "<Main::EMAIL>": Object.create($VRepr, {
        $tsym: { value: Symbol.for("Some<Main::EMAIL>") },
        $create: { value: (value) => {
            return Object.create(Some["<Main::EMAIL>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { parser.checkSpecialCons("some"); return Some["<Main::EMAIL>"].$create(parser.parseSingleArg(true, "value", "Main::EMAIL")); } },
        $emitAPI: { value: (emitter, value) => { return "Some<Main::EMAIL>{" + emitter.emitValue("Main::EMAIL", value.value) + "}"; } },
        $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
    }),
    "<Main::USER_NAME>": Object.create($VRepr, {
        $tsym: { value: Symbol.for("Some<Main::USER_NAME>") },
        $create: { value: (value) => {
            return Object.create(Some["<Main::USER_NAME>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { parser.checkSpecialCons("some"); return Some["<Main::USER_NAME>"].$create(parser.parseSingleArg(true, "value", "Main::USER_NAME")); } },
        $emitAPI: { value: (emitter, value) => { return "Some<Main::USER_NAME>{" + emitter.emitValue("Main::USER_NAME", value.value) + "}"; } },
        $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
    }),
    "<Main::USER_PHONE>": Object.create($VRepr, {
        $tsym: { value: Symbol.for("Some<Main::USER_PHONE>") },
        $create: { value: (value) => {
            return Object.create(Some["<Main::USER_PHONE>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { parser.checkSpecialCons("some"); return Some["<Main::USER_PHONE>"].$create(parser.parseSingleArg(true, "value", "Main::USER_PHONE")); } },
        $emitAPI: { value: (emitter, value) => { return "Some<Main::USER_PHONE>{" + emitter.emitValue("Main::USER_PHONE", value.value) + "}"; } },
        $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
    }),
    "<Main::USER_ADDRESS>": Object.create($VRepr, {
        $tsym: { value: Symbol.for("Some<Main::USER_ADDRESS>") },
        $create: { value: (value) => {
            return Object.create(Some["<Main::USER_ADDRESS>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { parser.checkSpecialCons("some"); return Some["<Main::USER_ADDRESS>"].$create(parser.parseSingleArg(true, "value", "Main::USER_ADDRESS")); } },
        $emitAPI: { value: (emitter, value) => { return "Some<Main::USER_ADDRESS>{" + emitter.emitValue("Main::USER_ADDRESS", value.value) + "}"; } },
        $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
    }),
    "<Int>": Object.create($VRepr, {
        $tsym: { value: Symbol.for("Some<Int>") },
        $create: { value: (value) => {
            return Object.create(Some["<Int>"], { value: { value: value, enumerable: true } });
        } },
        $parseAPI: { value: (parser) => { parser.checkSpecialCons("some"); return Some["<Int>"].$create(parser.parseSingleArg(true, "value", "Int")); } },
        $emitAPI: { value: (emitter, value) => { return "Some<Int>{" + emitter.emitValue("Int", value.value) + "}"; } },
        $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
    })
}

export const Bool = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("Bool") }
});

export const Nat = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("Nat") },
    _$consts: { value: {} },
    zero: { value: function() { return 0n; } },
    one: { value: function() { return 1n; } }
});

export const Int = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("Int") },
    _$consts: { value: {} },
    zero: { value: function() { return 0n; } },
    one: { value: function() { return 1n; } }
});

export const CString = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("CString") },
    empty:  { value: function() {
        let $this$ = this;
        return CStringOps.s_empty($this$);
    } },
    size:  { value: function() {
        let $this$ = this;
        return CStringOps.s_size($this$);
    } }
});

export const CRegex = Object.create(Object.prototype, {
    $tsym: { value: Symbol.for("CRegex") }
});

//_$supertypes for none is a special case in code (not emitted)
_$supertypes[Symbol.for("Some<Main::EMAIL>")] = [Symbol.for("Option<Main::EMAIL>")].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Some<Main::USER_NAME>")] = [Symbol.for("Option<Main::USER_NAME>")].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Some<Main::USER_PHONE>")] = [Symbol.for("Option<Main::USER_PHONE>")].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Some<Main::USER_ADDRESS>")] = [Symbol.for("Option<Main::USER_ADDRESS>")].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Some<Int>")] = [Symbol.for("Option<Int>")].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Bool")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Nat")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Int")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("CString")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("CRegex")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});

_$parsemap["Option<Main::EMAIL>"] = Option["<Main::EMAIL>"].$parseAPI;
_$parsemap["Option<Main::USER_NAME>"] = Option["<Main::USER_NAME>"].$parseAPI;
_$parsemap["Option<Main::USER_PHONE>"] = Option["<Main::USER_PHONE>"].$parseAPI;
_$parsemap["Option<Main::USER_ADDRESS>"] = Option["<Main::USER_ADDRESS>"].$parseAPI;
_$parsemap["Option<Int>"] = Option["<Int>"].$parseAPI;
_$parsemap["Some<Main::EMAIL>"] = Some["<Main::EMAIL>"].$parseAPI;
_$parsemap["Some<Main::USER_NAME>"] = Some["<Main::USER_NAME>"].$parseAPI;
_$parsemap["Some<Main::USER_PHONE>"] = Some["<Main::USER_PHONE>"].$parseAPI;
_$parsemap["Some<Main::USER_ADDRESS>"] = Some["<Main::USER_ADDRESS>"].$parseAPI;
_$parsemap["Some<Int>"] = Some["<Int>"].$parseAPI;

_$emitmap["Option<Main::EMAIL>"] = Option["<Main::EMAIL>"].$emitAPI;
_$emitmap["Option<Main::USER_NAME>"] = Option["<Main::USER_NAME>"].$emitAPI;
_$emitmap["Option<Main::USER_PHONE>"] = Option["<Main::USER_PHONE>"].$emitAPI;
_$emitmap["Option<Main::USER_ADDRESS>"] = Option["<Main::USER_ADDRESS>"].$emitAPI;
_$emitmap["Option<Int>"] = Option["<Int>"].$emitAPI;
_$emitmap["Some<Main::EMAIL>"] = Some["<Main::EMAIL>"].$emitAPI;
_$emitmap["Some<Main::USER_NAME>"] = Some["<Main::USER_NAME>"].$emitAPI;
_$emitmap["Some<Main::USER_PHONE>"] = Some["<Main::USER_PHONE>"].$emitAPI;
_$emitmap["Some<Main::USER_ADDRESS>"] = Some["<Main::USER_ADDRESS>"].$emitAPI;
_$emitmap["Some<Int>"] = Some["<Int>"].$emitAPI;
