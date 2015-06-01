#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

// A macro to forbid the compiler to generate copy constrcutor
// and assign function which usually are error-prone.
//
// We will always encounter compile-time errors if we use them unknowingly.
#define DISABLE_COPY_AND_ASSIGN(name) \
name(const name&) = delete; \
name& operator=(const name&) = delete

// Get the resource content defined by blade resource_library rule.
#define STATIC_RESOURCE(resource) \
StringPiece(RESOURCE_##resource, sizeof(RESOURCE_##resource) / sizeof(char))

#endif
