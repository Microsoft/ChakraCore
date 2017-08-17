/*
 * Copyright 2016 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WABT_VALIDATOR_H_
#define WABT_VALIDATOR_H_

#include "wast-lexer.h"

namespace wabt {

struct Module;
struct Script;
class ErrorHandler;

// Perform all checks on the script. It is valid if and only if this function
// succeeds.
Result ValidateScript(WastLexer*, const Script*, ErrorHandler*);
Result ValidateModule(WastLexer*, const Module*, ErrorHandler*);

}  // namespace wabt

#endif /* WABT_VALIDATOR_H_ */
