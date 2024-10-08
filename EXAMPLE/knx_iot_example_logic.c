/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022 Cascoda Ltd
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Cascoda Limited.
 *    integrated circuit in a product or a software update for such product, must
 *    reproduce the above copyright notice, this list of  conditions and the following
 *    disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Cascoda Limited nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * 4. This software, whether provided in binary or any other form must not be decompiled,
 *    disassembled, reverse engineered or otherwise modified.
 *
 *  5. This software, in whole or in part, must only be used with a Cascoda Limited circuit.
 *
 * THIS SOFTWARE IS PROVIDED BY CASCODA LIMITED "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CASCODA LIMITED OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

#include <ctype.h>
#include "knx_iot_example.h"
#include "cascoda-util/cascoda_tasklet.h"
#include "oc_core_res.h"
#include "api/oc_knx_dev.h"
#include "api/oc_knx_sec.h"
#include "api/oc_knx_fp.h"
#include "port/dns-sd.h"
#include "oc_knx.h"
#include "port/dns-sd.h"
#include "openthread/thread.h"
#include "openthread/ping_sender.h"
#include "platform.h"
#include "manufacturer_storage.h"
 

#define SCHEDULE_NOW 0



uint8_t user_interaction_occurred = 0;


///////////////////////////////////////////////////////////////////////////////
//               BUTTON Functions                                            //
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//              App logic initialisation                                     //
///////////////////////////////////////////////////////////////////////////////

void logic_initialize()
{
 
}

void logic_role_changed()
{
 
}

bool logic_is_role_screen()
{ 
}
