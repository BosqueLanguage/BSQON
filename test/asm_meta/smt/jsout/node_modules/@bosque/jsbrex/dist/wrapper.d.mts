/**
 * @param {any[]} rsystem
 * @returns {string[] | null}
 * @throws {TypeError}
 */
export function loadConstAndValidateRESystem(rsystem: any[]): string[] | null;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateStringLiteral(litstr: string): string;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateCStringLiteral(litstr: string): string;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function escapeStringLiteral(litstr: string): string;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function escapeCStringLiteral(litstr: string): string;
/**
 *
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns
 * @returns {boolean}
 * @throws {TypeError}
 */
export function accepts(pattern: string, input: string, inns: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns
 * @returns {boolean}
 * @throws {TypeError}
 */
export function startsWith(pattern: string, input: string, inns: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns
 * @returns {boolean}
 * @throws {TypeError}
 */
export function endsWith(pattern: string, input: string, inns: string): boolean;
/**
 * @param {string} pattern
 * @param {string} inns
 * @returns {string}
 * @throws {TypeError}
 */
export function getBSQIRForm(pattern: string, inns: string): string;
/**
 * @param {string} pattern
 * @param {string} inns
 * @returns {string}
 * @throws {TypeError}
 */
export function getSMTForm(pattern: string, inns: string): string;
//# sourceMappingURL=wrapper.d.mts.map