/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
  +----------------------------------------------------------------------+
  | PHP Version 5 |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license, |
  | that is bundled with this package in the file LICENSE, and is |
  | available through the world-wide-web at the following url: |
  | http://www.php.net/license/3_01.txt |
  | If you did not receive a copy of the PHP license and are unable to |
  | obtain it through the world-wide-web, please send a note to |
  | license@php.net so we can mail you a copy immediately. |
  +----------------------------------------------------------------------+
  | Author: Sara Golemon <pollita@php.net> |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_HASH_EQUALS_H
#define PHP_HASH_EQUALS_H

#include "src/php_cryptopp.h"

extern const zend_function_entry hash_equals_function[];

#if PHP_API_VERSION < 20131106
PHP_FUNCTION(hash_equals);
#endif

#endif /* PHP_HASH_EQUALS_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
