/* Copyright 2000 Pace Micro Technology plc
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
/*
 * unmodsqz - reverses the effect of modsqz
 *
 * Copyright (C) Acorn Computers Ltd. 1998
 */
#ifndef unmodsqz_h_included
#define unmodsqz_h_included

typedef enum error_codes {
  ok,
  errcode_NOT_SQUEEZED,
  errcode_NO_MEM,
  errcode_BAD_DECODE,

  errcode_NO_FILENAME,
  errcode_BAD_OPTION,
  errcode_BAD_PARAMETERS,
  errcode_NOT_MODULE,
  errcode_NO_LOAD,
  errcode_NO_SAVE,
  errcode_NO_PARAMS
} error_code;

/* unsqueeze_module reverses the modsqz that has been applied to a module.
 *
 * Inputs:
 *
 *   ptr:  Pointer to the block of memory holding the compressed data
 *
 * Outputs:
 *
 *   output:   Pointer to a malloc'ed memory block holding uncompressed
 *             module
 *
 *   output_len:  Size of the uncompressed module pointed to by output.
 *
 *
 * Assumptions:
 *
 *   bytesex_reverse() has been called if necessary for the target.
 *
 */
extern error_code unsqueeze_module(
  int32 *ptr,
  void **output,
  size_t *output_len);

#endif
