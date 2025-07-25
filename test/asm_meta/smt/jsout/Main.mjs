"use strict";
let _$consts = {};

import { $VRepr, _$softfails, _$supertypes, _$fisSubtype, _$fisNotSubtype, _$fasSubtype, _$fasNotSubtype, _$None, _$not, _$negate, _$add, _$sub, _$mult, _$div, _$bval, _$fkeq, _$fkeqopt, _$fkneq, _$fkneqopt, _$fkless, _$fnumeq, _$fnumless, _$fnumlesseq, _$exhaustive, _$abort, _$assert, _$formatchk, _$invariant, _$validate, _$precond, _$softprecond, _$postcond, _$softpostcond, _$memoconstval, _$accepts } from "./runtime.mjs";
import { _$setnone_lit, _$parsemap, _$emitmap, _$parseBSQON, _$emitBSQON } from "./bsqon.mjs";

import * as $Core from "./Core.mjs";

let _$rv = {};

export function re_SAFEIDENTIFIERCHAR() { return _$memoconstval(_$consts, "Main::re_SAFEIDENTIFIERCHAR", () => '/[a-zA-Z0-9-_]/c'); }

export function re_USERNAME() { return _$memoconstval(_$consts, "Main::re_USERNAME", () => '/ .{1, 50} /c'); }

export function userEmailNotEmpty(x) {
    if(x._$isNone()) {
        let $x = x._$asNone("expected None but got Some @ mock.bsq:33"); {
            return false;
        }
    }

    return (!$Core.CString.empty.call(x.value.value)) && ($Core.CString.size.call(x.value.value) <= 50n);
}

export function userNameNotEmpty(x) {
    if(x._$isNone()) {
        let $x = x._$asNone("expected None but got Some @ mock.bsq:40"); {
            return false;
        }
    }

    return !$Core.CString.empty.call(x.value.value);
}

export function userPhoneNotEmpty(x) {
    if(x._$isNone()) {
        let $x = x._$asNone("expected None but got Some @ mock.bsq:47"); {
            return false;
        }
    }

    return (!$Core.CString.empty.call(x.value.value)) && ($Core.CString.size.call(x.value.value) <= 20n);
}

export function userAddressNotEmpty(x) {
    if(x._$isNone()) {
        let $x = x._$asNone("expected None but got Some @ mock.bsq:54"); {
            return false;
        }
    }

    return (!$Core.CString.empty.call(x.value.value)) && ($Core.CString.size.call(x.value.value) <= 100n);
}

export function getCustomer(id) {
    const user = UserDTO.$create($Core.Some["<Int>"].$create(5n), $Core.Some["<Main::EMAIL>"].$create(EMAIL.$create("jch270@uky.edu")), ALPHANUMERIC.$create("12345678"), $Core.Some["<Main::USER_NAME>"].$create(USER_NAME.$create("James Chen")), $Core.Some["<Main::USER_PHONE>"].$create(USER_PHONE.$create("+12 111-111-1111")), $Core.Some["<Main::USER_ADDRESS>"].$create(USER_ADDRESS.$create("123 Fake Street")));
    return user;
}

export function main(id) {
    const user = getCustomer(id);
    return user;
}

export const ALPHANUMERIC = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::ALPHANUMERIC") },
    $create: { value: (value) => {
        _$formatchk(_$accepts('/ [a-zA-Z0-9]+/c', value, "Main"), "failed regex @ mock.bsq:5");
        return Object.create(ALPHANUMERIC, { value: { value: value, enumerable: true } });
    } },
    $createAPI: { value: (value) => {
        _$formatchk(_$accepts('/ [a-zA-Z0-9]+/c', value, "Main"), "failed regex @ mock.bsq:5");
        return Object.create(ALPHANUMERIC, { value: { value: value, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { const vv = ALPHANUMERIC.$createAPI(parser.parseValue("CString")); parser.parseScopedTypeTailing(); return vv; } },
    $emitAPI: { value: (emitter, value) => { return emitter.emitValue("CString", value.value) + "<Main::ALPHANUMERIC>"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

export const EMAIL = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::EMAIL") },
    $create: { value: (value) => {
        _$formatchk(_$accepts('/${Main::re_SAFEIDENTIFIERCHAR}+(\'.\'${Main::re_SAFEIDENTIFIERCHAR})* \'@\' (${Main::re_SAFEIDENTIFIERCHAR}+ \'.\')+ [a-zA-Z]+ /c', value, "Main"), "failed regex @ mock.bsq:6");
        return Object.create(EMAIL, { value: { value: value, enumerable: true } });
    } },
    $createAPI: { value: (value) => {
        _$formatchk(_$accepts('/${Main::re_SAFEIDENTIFIERCHAR}+(\'.\'${Main::re_SAFEIDENTIFIERCHAR})* \'@\' (${Main::re_SAFEIDENTIFIERCHAR}+ \'.\')+ [a-zA-Z]+ /c', value, "Main"), "failed regex @ mock.bsq:6");
        return Object.create(EMAIL, { value: { value: value, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { const vv = EMAIL.$createAPI(parser.parseValue("CString")); parser.parseScopedTypeTailing(); return vv; } },
    $emitAPI: { value: (emitter, value) => { return emitter.emitValue("CString", value.value) + "<Main::EMAIL>"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

export const USER_PHONE = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::USER_PHONE") },
    $create: { value: (value) => {
        _$formatchk(_$accepts('/[+]? [1-9][0-9]?\' \'* \'(\'? [0-9]{3} \')\'? [ -]?[0-9]{3}[ -]?[0-9]{4} /c', value, "Main"), "failed regex @ mock.bsq:8");
        return Object.create(USER_PHONE, { value: { value: value, enumerable: true } });
    } },
    $createAPI: { value: (value) => {
        _$formatchk(_$accepts('/[+]? [1-9][0-9]?\' \'* \'(\'? [0-9]{3} \')\'? [ -]?[0-9]{3}[ -]?[0-9]{4} /c', value, "Main"), "failed regex @ mock.bsq:8");
        return Object.create(USER_PHONE, { value: { value: value, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { const vv = USER_PHONE.$createAPI(parser.parseValue("CString")); parser.parseScopedTypeTailing(); return vv; } },
    $emitAPI: { value: (emitter, value) => { return emitter.emitValue("CString", value.value) + "<Main::USER_PHONE>"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

export const USER_NAME = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::USER_NAME") },
    $create: { value: (value) => {
        _$formatchk(_$accepts('/ ${Main::re_USERNAME} /c', value, "Main"), "failed regex @ mock.bsq:13");
        return Object.create(USER_NAME, { value: { value: value, enumerable: true } });
    } },
    $createAPI: { value: (value) => {
        _$formatchk(_$accepts('/ ${Main::re_USERNAME} /c', value, "Main"), "failed regex @ mock.bsq:13");
        return Object.create(USER_NAME, { value: { value: value, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { const vv = USER_NAME.$createAPI(parser.parseValue("CString")); parser.parseScopedTypeTailing(); return vv; } },
    $emitAPI: { value: (emitter, value) => { return emitter.emitValue("CString", value.value) + "<Main::USER_NAME>"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

export const USER_ADDRESS = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::USER_ADDRESS") },
    $create: { value: (value) => {
        _$formatchk(_$accepts('/[0-9a-zA-Z -,.#()]+/c', value, "Main"), "failed regex @ mock.bsq:15");
        return Object.create(USER_ADDRESS, { value: { value: value, enumerable: true } });
    } },
    $createAPI: { value: (value) => {
        _$formatchk(_$accepts('/[0-9a-zA-Z -,.#()]+/c', value, "Main"), "failed regex @ mock.bsq:15");
        return Object.create(USER_ADDRESS, { value: { value: value, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { const vv = USER_ADDRESS.$createAPI(parser.parseValue("CString")); parser.parseScopedTypeTailing(); return vv; } },
    $emitAPI: { value: (emitter, value) => { return emitter.emitValue("CString", value.value) + "<Main::USER_ADDRESS>"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

export const UserDTO = Object.create($VRepr, {
    $tsym: { value: Symbol.for("Main::UserDTO") },
    $checkinv_21_685: { value: ($id, $email, $password, $name, $phone, $address) => userEmailNotEmpty($email) },
    $checkinv_23_760: { value: ($id, $email, $password, $name, $phone, $address) => ($Core.CString.size.call($password.value) >= 6n) && ($Core.CString.size.call($password.value) <= 50n) },
    $checkinv_25_872: { value: ($id, $email, $password, $name, $phone, $address) => userNameNotEmpty($name) },
    $checkinv_27_948: { value: ($id, $email, $password, $name, $phone, $address) => userPhoneNotEmpty($phone) },
    $checkinv_29_1030: { value: ($id, $email, $password, $name, $phone, $address) => userAddressNotEmpty($address) },
    $create: { value: (id, email, password, name, phone, address) => {
        _$invariant(UserDTO.$checkinv_21_685(id, email, password, name, phone, address), "failed invariant @ mock.bsq:21");
        _$invariant(UserDTO.$checkinv_23_760(id, email, password, name, phone, address), "failed invariant @ mock.bsq:23");
        _$invariant(UserDTO.$checkinv_25_872(id, email, password, name, phone, address), "failed invariant @ mock.bsq:25");
        _$invariant(UserDTO.$checkinv_27_948(id, email, password, name, phone, address), "failed invariant @ mock.bsq:27");
        _$invariant(UserDTO.$checkinv_29_1030(id, email, password, name, phone, address), "failed invariant @ mock.bsq:29");
        return Object.create(UserDTO, { id: { value: id, enumerable: true }, email: { value: email, enumerable: true }, password: { value: password, enumerable: true }, name: { value: name, enumerable: true }, phone: { value: phone, enumerable: true }, address: { value: address, enumerable: true } });
    } },
    $update: { value: function(updates) {
        let vobj = {...this, ...updates};
        _$invariant(UserDTO.$checkinv_21_685(vobj.id, vobj.email, vobj.password, vobj.name, vobj.phone, vobj.address), "failed invariant @ mock.bsq:21");
        _$invariant(UserDTO.$checkinv_23_760(vobj.id, vobj.email, vobj.password, vobj.name, vobj.phone, vobj.address), "failed invariant @ mock.bsq:23");
        _$invariant(UserDTO.$checkinv_25_872(vobj.id, vobj.email, vobj.password, vobj.name, vobj.phone, vobj.address), "failed invariant @ mock.bsq:25");
        _$invariant(UserDTO.$checkinv_27_948(vobj.id, vobj.email, vobj.password, vobj.name, vobj.phone, vobj.address), "failed invariant @ mock.bsq:27");
        _$invariant(UserDTO.$checkinv_29_1030(vobj.id, vobj.email, vobj.password, vobj.name, vobj.phone, vobj.address), "failed invariant @ mock.bsq:29");
        return Object.assign(Object.create(UserDTO), vobj);
    } },
    $createAPI: { value: (id, email, password, name, phone, address) => {
        _$invariant(UserDTO.$checkinv_21_685(id, email, password, name, phone, address), "failed invariant @ mock.bsq:21");
        _$invariant(UserDTO.$checkinv_23_760(id, email, password, name, phone, address), "failed invariant @ mock.bsq:23");
        _$invariant(UserDTO.$checkinv_25_872(id, email, password, name, phone, address), "failed invariant @ mock.bsq:25");
        _$invariant(UserDTO.$checkinv_27_948(id, email, password, name, phone, address), "failed invariant @ mock.bsq:27");
        _$invariant(UserDTO.$checkinv_29_1030(id, email, password, name, phone, address), "failed invariant @ mock.bsq:29");
        return Object.create(UserDTO, { id: { value: id, enumerable: true }, email: { value: email, enumerable: true }, password: { value: password, enumerable: true }, name: { value: name, enumerable: true }, phone: { value: phone, enumerable: true }, address: { value: address, enumerable: true } });
    } },
    $parseAPI: { value: (parser) => { parser.checkConsType("Main::UserDTO"); const vals = parser.parseArgListGeneral([["id", "Option<Int>"],["email", "Option<Main::EMAIL>"],["password", "Main::ALPHANUMERIC"],["name", "Option<Main::USER_NAME>"],["phone", "Option<Main::USER_PHONE>"],["address", "Option<Main::USER_ADDRESS>"]]); return UserDTO.$createAPI(...vals); } },
    $emitAPI: { value: (emitter, value) => { return "Main::UserDTO{" + emitter.emitValue("Option<Int>", value.id) + ", " + emitter.emitValue("Option<Main::EMAIL>", value.email) + ", " + emitter.emitValue("Main::ALPHANUMERIC", value.password) + ", " + emitter.emitValue("Option<Main::USER_NAME>", value.name) + ", " + emitter.emitValue("Option<Main::USER_PHONE>", value.phone) + ", " + emitter.emitValue("Option<Main::USER_ADDRESS>", value.address) + "}"; } },
    $scall: { value: function(name, tt, ...args) { return this[name][tt].call(this, ...args); } }
});

_$supertypes[Symbol.for("Main::ALPHANUMERIC")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Main::EMAIL")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Main::USER_PHONE")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Main::USER_NAME")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Main::USER_ADDRESS")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});
_$supertypes[Symbol.for("Main::UserDTO")] = [].reduce((acc, ks) => { acc[ks] = true; return acc; }, {});

_$parsemap["Main::ALPHANUMERIC"] = ALPHANUMERIC.$parseAPI;
_$parsemap["Main::EMAIL"] = EMAIL.$parseAPI;
_$parsemap["Main::USER_PHONE"] = USER_PHONE.$parseAPI;
_$parsemap["Main::USER_NAME"] = USER_NAME.$parseAPI;
_$parsemap["Main::USER_ADDRESS"] = USER_ADDRESS.$parseAPI;
_$parsemap["Main::UserDTO"] = UserDTO.$parseAPI;

_$emitmap["Main::ALPHANUMERIC"] = ALPHANUMERIC.$emitAPI;
_$emitmap["Main::EMAIL"] = EMAIL.$emitAPI;
_$emitmap["Main::USER_PHONE"] = USER_PHONE.$emitAPI;
_$emitmap["Main::USER_NAME"] = USER_NAME.$emitAPI;
_$emitmap["Main::USER_ADDRESS"] = USER_ADDRESS.$emitAPI;
_$emitmap["Main::UserDTO"] = UserDTO.$emitAPI;

import { loadConstAndValidateRESystem } from "@bosque/jsbrex";
loadConstAndValidateRESystem([
    {
        "nsinfo": {
            "inns": "Main",
            "nsmappings": []
        },
        "reinfos": [
            {
                "name": "re_SAFEIDENTIFIERCHAR",
                "restr": "/[a-zA-Z0-9-_]/c"
            },
            {
                "name": "re_USERNAME",
                "restr": "/ .{1, 50} /c"
            }
        ]
    }
]);
const usefile = process.argv.length === 4 && process.argv[2] === "--file";
import { readFileSync } from "fs";
let input = readFileSync(usefile ? process.argv[3] : 0, 'utf-8');
let args;
try { args = _$parseBSQON(["Int"], input); } catch(pe) { process.stdout.write(`ParseError -- ${pe.message || pe}\n`); process.exit(1); }
let res;
try { res = main(...args); } catch(e) { process.stdout.write(`Error -- ${e.$info || e}\n`); process.exit(1); }
process.stdout.write(_$emitBSQON("Main::UserDTO", res) + '\n');'\n'