/**
 * <h1>Predefined</h1>
 *
 * <p>Enter the predefined Pascal types, identifiers, and constants
 * into the symbol table.</p>
 *
 * <p>Copyright (c) 2009 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */

/*
*	Lab #4 Assignment Starts Here.
*
*
*/
#include <vector>
#include "Predefined.h"
#include "SymTabEntryImpl.h"
#include "../TypeSpec.h"
#include "../TypeFactory.h"
#include "../SymTabStack.h"
#include "../typeimpl/TypeSpecImpl.h"
#include "../../DataValue.h"

namespace wci { namespace intermediate { namespace symtabimpl {

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::intermediate::typeimpl;

// Predefined types.
TypeSpec *Predefined::integer_type;
TypeSpec *Predefined::real_type;
TypeSpec *Predefined::imag_type;
TypeSpec *Predefined::boolean_type;
TypeSpec *Predefined::char_type;
TypeSpec *Predefined::undefined_type;

// Predefined identifiers.
SymTabEntry *Predefined::integer_id;
SymTabEntry *Predefined::real_id;
SymTabEntry *Predefined::imag_id;
SymTabEntry *Predefined::boolean_id;
SymTabEntry *Predefined::char_id;
SymTabEntry *Predefined::false_id;
SymTabEntry *Predefined::true_id;

void Predefined::initialize(SymTabStack *symtab_stack)
{
    initialize_types(symtab_stack);
    initialize_constants(symtab_stack);
}

/**
 * Initialize the predefined types.
 * @param symTabStack the symbol table stack to initialize.
 */
void Predefined::initialize_types(SymTabStack *symtab_stack)
{
    // Type integer.
    integer_id = symtab_stack->enter_local("integer");
    integer_type = TypeFactory::create_type((TypeForm) TF_SCALAR);
    integer_type->set_identifier(integer_id);
    integer_id->set_definition((Definition) DF_TYPE);
    integer_id->set_typespec(integer_type);

    // Type real.
    real_id = symtab_stack->enter_local("real");
    real_type = TypeFactory::create_type((TypeForm) TF_SCALAR);
    real_type->set_identifier(real_id);
    real_id->set_definition((Definition) DF_TYPE);
    real_id->set_typespec(real_type);

    // Type imaginary.
    imag_id = symtab_stack->enter_local("imaginary");
    imag_type = TypeFactory::create_type((TypeForm) TF_SCALAR);
    imag_type->set_identifier(imag_id);
    imag_id->set_definition((Definition) DF_TYPE);
    imag_id->set_typespec(imag_type);

    // Type boolean.
    boolean_id = symtab_stack->enter_local("boolean");
    boolean_type = TypeFactory::create_type((TypeForm) TF_ENUMERATION);
    boolean_type->set_identifier(boolean_id);
    boolean_id->set_definition((Definition) DF_TYPE);
    boolean_id->set_typespec(boolean_type);

    // Type char.
    char_id = symtab_stack->enter_local("char");
    char_type = TypeFactory::create_type((TypeForm) TF_SCALAR);
    char_type->set_identifier(char_id);
    char_id->set_definition((Definition) DF_TYPE);
    char_id->set_typespec(char_type);

    // Type complex.
    complex_id = symtab_stack->enter_local("complex");
    complex_type = TypeFactory::create_type((TypeForm) TF SCALAR);
    complex_type->set_identifier(complex_id);
    complex_id->set_definition((Definition) DF_TYPE);
    complex_id->set_typespec(complex_type);

    SymTab *csymtab = SymTabFactory::crate_symtab(0);
    SymTabEntry *re_id = csymtab->enter("re");
    SymTabEntry *imag_id = csymtab->enter("im");
    re_id->set_typespec(real_type);
    imag_id->set_typespec(real_type);
    complex_type->set_attribute((TypeKey) RECORD_SYMTAB, new TypeValue(csymtab));

    // Undefined type.
    undefined_type = TypeFactory::create_type((TypeForm) TF_SCALAR);
}

void Predefined::initialize_constants(SymTabStack *symtab_stack)
{
    // Boolean enumeration constant false.
    false_id = symtab_stack->enter_local("false");
    false_id->set_definition((Definition) DF_ENUMERATION_CONSTANT);
    false_id->set_typespec(boolean_type);
    false_id->set_attribute((SymTabKey) CONSTANT_VALUE,
                            new EntryValue(new DataValue(0)));

    // Boolean enumeration constant true.
    true_id = symtab_stack->enter_local("true");
    true_id->set_definition((Definition) DF_ENUMERATION_CONSTANT);
    true_id->set_typespec(boolean_type);
    true_id->set_attribute((SymTabKey) CONSTANT_VALUE,
                           new EntryValue(new DataValue(1)));

    // Add false and true to the boolean enumeration type.
    TypeValue *type_value = new TypeValue();
    type_value->v.push_back(false_id);
    type_value->v.push_back(true_id);
    boolean_type->set_attribute((TypeKey) ENUMERATION_CONSTANTS,
                                type_value);
}

}}}  // namespace wci::intermediate::symtabimpl


