# Glan Document

This file contains how the language structure is designed within the context and constraints of C++. This file will detail how Glan is implemented in the source code rather than the usage of the language.

## Class & Concept Structure


```mermaid
---
title: Concept Hierarchy
---
classDiagram

class ANY_STATEMENT {
  + operator string()
}

class ANY_EXPR {
  + operator string()
  + typename TYPE
}

class ANY_VAR {
  + string NAME
}
ANY_EXPR --|> ANY_VAR

class ANY_READONLY {
  + string NAME
}
ANY_EXPR --|> ANY_READONLY

class ANY_IMMEDIATE {
  + TYPE::CPP_TYPE VAL
}
ANY_EXPR --|> ANY_IMMEDIATE

class ANY_BUILTIN {
  + string NAME
}
ANY_EXPR --|> ANY_BUILTIN

class ANY_TYPE {
  + string KEYWORD
  + typename CPP_TYPE
}
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