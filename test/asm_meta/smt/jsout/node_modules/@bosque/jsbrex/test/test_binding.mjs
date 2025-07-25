import {accepts, startsWith, endsWith, validateStringLiteral, validateCStringLiteral, loadConstAndValidateRESystem, escapeStringLiteral, escapeCStringLiteral, getBSQIRForm, getSMTForm} from "../wrapper.mjs";
import assert from "node:assert";

assert(accepts, "The expected function is undefined");
assert(startsWith, "The expected function is undefined");
assert(endsWith, "The expected function is undefined");

assert(accepts('/"hello"/', "hello", "Main"));
assert(!accepts('/"hello"/', "nope", "Main"));

assert(validateStringLiteral, "The expected function is undefined");
assert(validateStringLiteral("hello") === "hello");
assert(validateStringLiteral("a🌵c") === "a🌵c");
assert(validateStringLiteral("a%x1f335;c") === "a🌵c");

assert(validateCStringLiteral, "The expected function is undefined");
assert(validateCStringLiteral("hello") === "hello");
assert(validateCStringLiteral("%x59;") === "Y");

assert.throws(() => validateCStringLiteral("a🌵c"));

assert(escapeStringLiteral("abc") === "abc");
assert(escapeStringLiteral(validateStringLiteral("%%;")) === "%%;");
assert(escapeStringLiteral(validateStringLiteral("%percent;")) === "%%;");

assert(escapeCStringLiteral("abc") === "abc");
assert(escapeCStringLiteral(validateCStringLiteral("%;")) === "%;");
assert(escapeCStringLiteral(validateCStringLiteral("%tick;")) === "%;");


const nsinfo1 = {
    "nsinfo": {
        "inns": "Main",
        "nsmappings": []
    },
    "reinfos": [
        {
            "name": "Foo",
            "restr": '/"abc"/'
        
        },
        {
            "name": "Bar",
            "restr": '/"xyz"/'
        
        },
        {
            "name": "Baz",
            "restr": '/${Foo} "-" ${Bar}/'
        }
    ]
};

const errors1 = loadConstAndValidateRESystem([nsinfo1]);
assert(errors1 === null);

assert(accepts("/${Main::Foo}/", "abc", "Main"));
assert(accepts("/${Main::Baz}/", "abc-xyz", "Main"));

assert(accepts("/${Foo}/", "abc", "Main"));

assert(getBSQIRForm("/${Foo}/", "Main") === '/"abc"/');
assert(getSMTForm("/${Foo}/", "Main") === '(str.to.re "abc")');

assert(getBSQIRForm("/'yep'+/c", "Main") === "/'yep'+/c");
assert(getSMTForm("/'yep'+/c", "Main") === '(re.+ (str.to.re "yep"))');

console.log("Tests passed- everything looks OK!");