# Bosque Object Notation -- BSQON

Bosque Object Notation (BSQON) is a combination of a type-system based language for describing data formats (or configuration files) and a specialized literal format. 

The specification language is structured as a _programming language type system_. This design [technical report](https://github.com/BosqueLanguage/BSQON/blob/main/docs/publications/bsqon_techreport.pdf) provides a powerful set of features for describing complex data formats and structures and, just like any other programming language, enables us to compose, package, type-check, version, and provide iDE support for these specifications. This language provides a rich set of builtin types and type system feature that are specifically designed to support the needs of data formats and serialization (and AI or human agents working with them). It goes further by providing [bosque](https://github.com/BosqueLanguage/BosqueCore) expression language for defining constraints and default values that cannot be expressed in a simple type system. This expression language is unique as, it is a _regularized programming language_, and is thus amenable to static analysis and verification -- we are targeting fuzzing, mocking, and version checking on our roadmap.

The literal format language is designed to be easily human, AI, and machine processable. The format supports nice to have features, like comments, flexible syntactic decorations to ease reading/writing them, and efficient parsing. Further, in conjunction with the type language, we can generate parsers, command completion, checkers, and other IDE support tools automatically! This literal format provides specialized formats for all the builtin types and convenient notation for custom designed types -- providing in a readable format that is resistant to typos, missing fields, and other common sources of errors that occur in literal/config data objects.

## More Docs and Examples

```
%%%%
%% Data Specification
type Identifier = CString of /[A-Za -z][A-Za -z0 -9_ ]{0 -9}/ a;
type CUSIP = CString of /[0 -9]{6}[0 -9A-Z ]{3}/ a;

type Quantity = BigNat;
    
entity SaleOrder {
    field id: Identifier;
    field instrument: CUSIP;
    field quantity: Quantity;
}
        
entity DaySalesSummary {
    field available: Quantity;
    field startAvailable: Quantity;
    field orders: List<SaleOrder>;
            
    validate orders.unique(pred(a, b) => a.id !== b.id);
    validate startAvailable - orders.sumOf<Quantity>(fn(a) => a.quantity) == available;
}

%%%%
%% Literal
DaySalesSummary{
    available: 800N_Quantity,
    startAvailable: 1000N_Quantity,
    orders: [
        SaleOrder{ 'A1'Identifier, '123456789'CUSIP, 100N_Quantity },
        SaleOrder{ 'A2'Identifier, '123456789'CUSIP, 100N_Quantity }
    ]
}

%%%%
%% Literal (with errors)
DaySalesSummary{
    available: 900N_Quantity,                                         %% fails startAvailable - orders check
    startAvailable: 1000.5,                                           %% fails type check
    orders: [
        SaleOrder{ 'A1'Identifier, '123$56789'CUSIP, 100N_Quantity }, %% fails CUSIP format check
        SaleOrder{ 'A1'Identifier, '123456789'CUSIP, 100N_Quantity }  %% fails invariant on unique id
    ]
}
```
