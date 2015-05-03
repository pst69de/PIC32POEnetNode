****************************************
  Project wiki  
****************************************

http://wiki69.pst69.homeip.net/index.php/PIC32_POE.net_Node

****************************************
  License 
****************************************
This project is open source. 
The license claimed on my (Patrick O. Ehrmann, pst69@pst69.de) parts of this code is creative commons zero (https://creativecommons.org/publicdomain/zero/1.0/).

There are parts within this project which claim another form of license, not disrupting the open source claim on my files. Respect the claims of other contributed within their files.

Parts within this project that provide other license claims:
----------------------------------------
TinyXML-2 project:
----------------------------------------
tinyxml2.h
tinyxml2.cpp

License
-------

TinyXML-2 is released under the zlib license:

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must 
not claim that you wrote the original software. If you use this 
software in a product, an acknowledgment in the product documentation 
would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and 
must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source 
distribution.

Contributors
------------

Thanks very much to everyone who sends suggestions, bugs, ideas, and 
encouragement. It all helps, and makes this project fun.

The original TinyXML-1 has many contributors, who all deserve thanks
in shaping what is a very successful library. Extra thanks to Yves
Berquin and Andrew Ellerton who were key contributors.

TinyXML-2 grew from that effort. Lee Thomason is the original author
of TinyXML-2 (and TinyXML-1) but TinyXML-2 has been and is being improved
by many contributors.

Thanks to John Mackay at http://john.mackay.rosalilastudio.com for the TinyXML-2 logo!
----------------------------------------
----------------------------------------
These are patched files derived from the original to provide CPP compiler compatibility.
Microchip development platfrom MPLAB Harmony:
----------------------------------------
usb_TokenPID_Default.h
int_CPUCurrentPriorityLevel_Default.h
int_EnableControl_PIC32.h
int_INTCPUPriority_Default.h
int_INTCPUVector_Default.h
int_ShadowRegisterAssign_Unsupported.h
int_VariableOffset_Unsupported.h
int_VectorPriority_Default.h
osal_impl_basic.h
usart_LineControlMode_RXandTXCombined.h

License:
----------------------------------------

Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
----------------------------------------
----------------------------------------

****************************************
Installation hints
****************************************

see actual version on http://wiki69.pst69.homeip.net/index.php/PIC32_POE.net_Node

Source directory of a typical MPLABX project using MPLAB Harmony libraries.

* Assumption on environment
** Windows 7
*** "C:\microchip\harmony" = "&gt;harmony&lt;"
*** Installation of Microchip Harmony library (in Version 1.03) at "&gt;harmony&lt;\v1_03"
*** Projects folder at "&gt;harmony&lt;\projects" = "&gt;projects&lt;"
* Startup of project
** Create folder "&gt;projects&lt;\PIC32POEnetNode"
** Create subfolder "&gt;projects&lt;\PIC32POEnetNode\framework"
** Open MPLABX
*** Create empty project
*** "Choose Project" "Microchip Embedded > Standalone Project"
*** "Select Device" Family "32-bit MCUs (PIC32)" your Device (mine is a "PIC32MX250F128B")
*** "Select Tool", mine is a "Microstick II" which is coded "Starter Kits (PKOB)" within the "Microchip Starter Kits" Group
*** "Select Compiler", as mentioned "XC32 (v1.34) ..."
*** "Select Project Name and Folder" 
*** Project Name: PIC32POEnetNode
*** Project Location: "&gt;projects&lt;\PIC32POEnetNode\framework"
*** Check "Set as main Project"
*** Check "Overwrite existing project"
*** Pick Encoding "UTF-8" 
*** "Finish"
*** Close MPLAB X
* Download Source
** from [https://github.com/pst69de/PIC32POEnetNode GitHub pst69de/PIC32POEnetNode] to "&gt;projects&lt;\PIC32POEnetNode\framework\src" 
** File "&gt;projects&lt;\PIC32POEnetNode\framework\src\configuration.xml" should be copied over "&gt;projects&lt;\PIC32POEnetNode\framework\PIC32POEnetNode.X\nbproject\configuration.xml"
*** if there is a problem with Loading xml after opening MPLAB X again, then delete the contents of &gt;projects&lt;\PIC32POEnetNode\framework\PIC32POEnetNode.X\nbproject\private"
*** also, check for different settings you made with the empty project in respect to the contents of my configuration as shown above
** Files under "&gt;projects&lt;\PIC32POEnetNode\framework\src\harmony_mods" should replace similar found files under "&gt;harmony&lt;\v1_03\framework\peripheral"
*** maybe in future there will be a subrepository patching to resolve this more cleanly
* Working with source
** in source file configuration_bits.cpp there is to define a device individual "#pragma config USERID = 0x1234u" identification, as this used to reidentify the location or usage by the master

****************************************
Casual files
****************************************
There are files contained only for casual working purposes, don't mind the content for your project.
These are:
compiler_errors.txt, for resolving compiler issues later on


****************************************
Further Infomation
****************************************

see http://wiki69.pst69.homeip.net/index.php/PIC32_POE.net_Node
