#ifndef PHP_CRYPTOPP_H
#define PHP_CRYPTOPP_H

#define PHP_CRYPTOPP_EXTNAME "cryptopp"
#define PHP_CRYPTOPP_VERSION "%ext_version%"
#define PHP_CRYPTOPP_NAMESPACE "CryptoPP"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

// include crypto++
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptlib.h"

// TODO macros helpers to call PHP functions
//#if PHP_VERSION_ID >= 50300
# define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
# define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
# define PUSH_EO_PARAM()
# define POP_EO_PARAM()
//#else
//# define ZEND_VM_STACK_GROW_IF_NEEDED(x)
//# define PUSH_PARAM(arg) zend_ptr_stack_push(&EG(argument_stack), arg)
//# define POP_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
//# define PUSH_EO_PARAM() zend_ptr_stack_push(&EG(argument_stack), NULL)
//# define POP_EO_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
//#endif

#define CALL_PHP_FUNCTION_HELPER(func_name, retval, num, param) \
    PUSH_PARAM(param); PUSH_PARAM((void*)num);                  \
    PUSH_EO_PARAM();                                            \
    PHP_FN(func_name)(num, retval, NULL, NULL, 0 TSRMLS_CC);    \
    POP_EO_PARAM();                                             \
    POP_PARAM(); POP_PARAM();

#define CALL_PHP_FUNCTION(func_name, retval)                    \
    PHP_FN(func_name)(0, retval, NULL, NULL, 0 TSRMLS_CC);

#define CALL_PHP_FUNCTION1(func_name, retval, param1)           \
    ZEND_VM_STACK_GROW_IF_NEEDED(2);                            \
    CALL_PHP_FUNCTION_HELPER(func_name, retval, 1, param1);

#define CALL_PHP_FUNCTION2(func_name, retval, param1, param2)   \
    ZEND_VM_STACK_GROW_IF_NEEDED(3);                            \
    PUSH_PARAM(param1);                                         \
    CALL_PHP_FUNCTION_HELPER(func_name, retval, 2, param2);     \
    POP_PARAM();

// TODO
extern zend_module_entry cryptopp_module_entry;
#define phpext_cryptopp_ptr &cryptopp_module_entry;

//%configure_inclusion%

#endif
