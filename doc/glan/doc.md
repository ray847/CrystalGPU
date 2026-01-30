# Glan Document

This file contains how the language structure is designed within the context and constraints of C++. This file will detail how Glan is implemented in the source code rather than the usage of the language.

## Class & Concept Structure


```mermaid
---
title: Code Generation
---
classDiagram

class PROCEDURE {
  FUNCTION MAIN
  vector~FUNCTION~ FUNCTIONS
  vector~TUPLE~ TUPLES
}
PROCEDURE --* FUNCTION

class FUNCTION {
  SIGNATURE
  BLOCK
}
FUNCTION --* SIGNATURE
FUNCTION --* BLOCK

class SIGNATURE {
  vector<string> PARAMS
  string RETURN_TYPE
  string NAME
}

class BLOCK {
  BLOCK* OUTER
  vector~variant~string, BLOCK~~
  void PUSH(string)
  void PUSH(OP)
  void PUSH()
  void POP()
}

class TYPE {
  string KEYWORD
}

class ATOMIC {
  string USAGE
}
ATOMIC --> BLOCK: Push(string)
FUNCTION --> ATOMIC: Function Call

class TUPLE {
  TYPLE~TYPE~ ITEMS
  ATOMIC operator[]
}
ATOMIC <|-- TUPLE
TYPE *-- TUPLE

class VARIABLE {
  TYPE
  string KEYWORD
}
ATOMIC <|-- VARIABLE
TYPE *-- VARIABLE

class PARAMETER {
  string KEYWORD
}
ATOMIC <|-- PARAMETER
TYPE *-- PARAMETER

class READONLY {
  string KEYWORD
}
ATOMIC <|-- READONLY
TYPE *-- READONLY

class CONSTANT {
  string KEYWORD
}
ATOMIC <|-- CONSTANT
TYPE *-- CONSTANT

class IMMEDIATE {
  string KEYWORD
}
ATOMIC <|-- IMMEDIATE
TYPE *-- IMMEDIATE
```

```mermaid
---
title: Class Hierarchy
---
classDiagram

class ASSIGN_STATEMENT~ANY_VAR, ANY_EXPR~ {
  ANY_VAR VAR
  ANY_EXPR EXPR
  + operator string()
}

class MONO_OP_EXPR~OP~ {
  + operator string()
  + typename TYPE
}
class DUAL_OP_EXPR~OP~ {
  + operator string()
  + typename TYPE
}

class VAR~TYPE~ {
  + string NAME
}

class CONST~TYPE~ {
  + string NAME
}
```