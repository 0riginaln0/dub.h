// dub_dsl.h – Scoped DSL for the dub.h library.
//
// Usage:
//   #include "dub_dsl.h"   // defines macros (assumes a 'Dub dub' variable)
//   ... use MATCH("..."), PARSE_INT(&x) etc.
//   #include "dub_dsl.h"   // undefines all macros
//
// The macros assume a variable named 'dub' of type Dub is in scope.
// If you need a different name, you can #define DUB_VAR your_var before
// including the file (but then you must also undefine it before the second
// inclusion, or use a consistent approach).

#ifdef DUB_DSL_SCOPED

#undef MATCH
#undef PARSE_INT
#undef PARSE_UINT
#undef PARSE_PORT
#undef PARSE_EMAIL
#undef PARSE_STRING
#undef DESC
#undef DESCF

#undef DUB_DSL_SCOPED

#else

#ifndef DUB_VAR
#define DUB_VAR dub
#endif

#define MATCH(s)            dub_match(&DUB_VAR, (s))

#define PARSE_INT(out)      dub_parse_integer(&DUB_VAR, (out))
#define PARSE_UINT(out)     dub_parse_unsigned_integer(&DUB_VAR, (out))
#define PARSE_PORT(out)     dub_parse_port(&DUB_VAR, (out))
#define PARSE_EMAIL(out)    dub_parse_email(&DUB_VAR, (out))
#define PARSE_STRING(out)   dub_parse_string(&DUB_VAR, (out))

#define DESC(desc, file)    dub_desc(&DUB_VAR, (desc), (file))

#define DUB_DSL_SCOPED

#endif