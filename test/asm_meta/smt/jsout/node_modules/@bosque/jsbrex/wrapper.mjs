import brex from "./lib/binding.js";

/**
 * @param {any[]} rsystem
 * @returns {string[] | null}
 * @throws {TypeError}
 */
export function loadConstAndValidateRESystem(rsystem) {
    return brex.loadConstAndValidateRESystem(rsystem);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateStringLiteral(litstr) {
    return brex.validateStringLiteral(litstr);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateCStringLiteral(litstr) {
    return brex.validateCStringLiteral(litstr);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function escapeStringLiteral(litstr) {
    return brex.escapeStringLiteral(litstr);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function escapeCStringLiteral(litstr) {
    return brex.escapeCStringLiteral(litstr);
}

/**
 * 
 * @param {string} pattern 
 * @param {string} input 
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function accepts(pattern, input, inns) {
    return brex.accepts(pattern, input, inns);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function startsWith(pattern, input, inns) {
    return brex.startsWith(pattern, input, inns);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function endsWith(pattern, input, inns) {
    return brex.endsWith(pattern, input, inns);
}

/**
 * @param {string} pattern
 * @param {string} inns 
 * @returns {string}
 * @throws {TypeError}
 */
export function getBSQIRForm(pattern, inns) {
    return brex.getBSQIRForm(pattern, inns);
}

/**
 * @param {string} pattern
 * @param {string} inns 
 * @returns {string}
 * @throws {TypeError}
 */
export function getSMTForm(pattern, inns) {
    return brex.getSMTForm(pattern, inns);
}

