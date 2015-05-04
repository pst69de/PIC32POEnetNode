/* 
 * File:    POEnet.cpp
 * Author:  Patrick O. Ehrmann (pst69@pst69.de)
 * License: Creative Commons Zero (https://creativecommons.org/publicdomain/zero/1.0/)
 *
 * Created on 2015-03-13
 * Description:
 *   Call interface tinyxml for app loop core
 * Major Changes:
 *   Version 0: alpha development
 */

#include <string.h>
#include "POEnet.h"
#include "tinyxml2.h"
#include "system_config.h"
#include "app.h"

tinyxml2::XMLDocument POEnetNode( true, tinyxml2::COLLAPSE_WHITESPACE);
tinyxml2::XMLDocument POEnetCommand( true, tinyxml2::COLLAPSE_WHITESPACE);
// const char for non-relevant command
const char POEnet_empty[] = "       ";
const char POEnet_error[] = "!error!";
const char POEnet_noop[] = "noop";
const char POEnet_reset[] = "reset";
const char POEnet_net[] = "net";
const char POEnet_node[] = "node";
const char POEnet_id[] = "id";
const char POEnet_name[] = "name";
const char POEnet_time[] = "time";
const char POEnet_analog[] = "analog";
const char POEnet_value[] = "value";
const char POEnet_numerator[] = "numerator";
const char POEnet_denominator[] = "denominator";
const char POEnet_offset[] = "offset";
const char POEnet_unit[] = "unit";
const char POEnet_digital[] = "digital";
const char POEnet_lovalue[] = "lovalue";
const char POEnet_hivalue[] = "hivalue";
const char POEnet_switch[] = "switch";
const char POEnet_pwm[] = "pwm";
const char POEnet_text[] = "text";
const char POEnet_action[] = "action";
int  *node_id;
char *node_name;
int  *time_hours;
int  *time_minutes;
int  *time_seconds;

void POEnet_Node_Init(int *id, char *name, int *hours, int *minutes, int *seconds) {
    node_id = id;
    node_name = name;
    time_hours = hours;
    time_minutes = minutes;
    time_seconds = seconds;
    POEnetNode.Clear();
    tinyxml2::XMLElement *elemNode = POEnetNode.NewElement( &POEnet_node[0]);
    elemNode->SetAttribute(&POEnet_id[0],*node_id);
    elemNode->SetAttribute(&POEnet_name[0],node_name);
    POEnetNode.InsertFirstChild( elemNode);
}


void POEnet_AddAnalog(int id, float *Value, float *numerator, float *denominator, float *offset, char *unit) {
    tinyxml2::XMLElement *elemAnalog = POEnetNode.NewElement( &POEnet_analog[0]); 
	// keep the order of id, value and others
    elemAnalog->SetAttribute(&POEnet_id[0], id);
    elemAnalog->SetAttribute(&POEnet_value[0], Value);
    elemAnalog->SetAttribute(&POEnet_numerator[0], numerator);
    elemAnalog->SetAttribute(&POEnet_denominator[0], denominator);
    elemAnalog->SetAttribute(&POEnet_offset[0], offset);
    elemAnalog->SetAttribute(&POEnet_unit[0], unit, APP_STRING_SIZE);
    POEnetNode.RootElement()->InsertEndChild( elemAnalog);
}

void POEnet_AddDigital(int id, int *Value, char *loVal, char *hiVal) {
    tinyxml2::XMLElement *elemDigital = POEnetNode.NewElement( &POEnet_digital[0]); 
	// keep the order of id, value and others
    elemDigital->SetAttribute(&POEnet_id[0], id);
    elemDigital->SetAttribute(&POEnet_value[0], Value);
    elemDigital->SetAttribute(&POEnet_lovalue[0], loVal, APP_STRING_SIZE);
    elemDigital->SetAttribute(&POEnet_hivalue[0], hiVal, APP_STRING_SIZE);
    POEnetNode.RootElement()->InsertEndChild( elemDigital);
}

void POEnet_AddSwitch(int id, int *Value, char *loVal, char *hiVal) {
    tinyxml2::XMLElement *elemSwitch = POEnetNode.NewElement( &POEnet_switch[0]); 
	// keep the order of id, value and others
    elemSwitch->SetAttribute(&POEnet_id[0], id);
    elemSwitch->SetAttribute(&POEnet_value[0], Value);
    elemSwitch->SetAttribute(&POEnet_lovalue[0], loVal, APP_STRING_SIZE);
    elemSwitch->SetAttribute(&POEnet_hivalue[0], hiVal, APP_STRING_SIZE);
    POEnetNode.RootElement()->InsertEndChild( elemSwitch);
}

void POEnet_NodeDump(char *buffer) {
    tinyxml2::XMLPrinter myOut(true);
    POEnetNode.Print(&myOut);
    strncpy(buffer,myOut.CStr(),APP_BUFFER_SIZE);
}

void POEnet_GetNewNodeId() {
    tinyxml2::XMLElement *aNode;
    tinyxml2::XMLElement *myNode;
    aNode = POEnetCommand.RootElement()->FirstChildElement(&POEnet_node[0]);
    if (!aNode) {
        // no node yet, we start with 1
        *node_id = 1;
    } else {
        while (aNode->NextSiblingElement(&POEnet_node[0])) { aNode = aNode->NextSiblingElement(&POEnet_node[0]);}
        aNode->QueryIntAttribute(&POEnet_id[0],node_id);
        *node_id++;
    }
    myNode = POEnetCommand.NewElement(&POEnet_node[0]);
    myNode->SetAttribute(&POEnet_id[0],*node_id);
    myNode->SetAttribute(&POEnet_name[0],node_name);
    POEnetCommand.RootElement()->InsertEndChild(myNode);
}

void POEnet_DescribeNode() {
    // Test an insertion
    //POEnetCommand.RootElement()->InsertEndChild( POEnetCommand.NewElement(&POEnet_noop[0]));
    // copy all elements of POEnetNode.RootElement() to POEnetCommand.RootElement()
    tinyxml2::XMLElement *anElement;
    tinyxml2::XMLElement *copiedElement;
    const tinyxml2::XMLAttribute *anAttribute;
    anElement = POEnetNode.RootElement()->FirstChildElement();
    while (anElement) {
        //vvv doesn't work as expected
        //POEnetCommand.RootElement()->InsertEndChild( anElement->ShallowClone( &POEnetCommand));
        copiedElement = POEnetCommand.NewElement( anElement->Name());
        // POETODO: copy attributes
        //copiedElement->SetAttribute( &POEnet_noop[0], 0);
    	// to keep the order of id, value and others
        anAttribute = anElement->FirstAttribute();
        copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
        anAttribute = anAttribute->Next();
        copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
        /* it seems getting a to long result on dirstributing all attributes
         * the id and value maybe enough on a node capability questionaire
         * we can post other properties on a specific interrogation
        if (!strcmp(anElement->Name(), &POEnet_analog[0])) {
            copiedElement->SetAttribute( &POEnet_noop[0], 0);
            //anAttribute = anAttribute->Next();
            //copiedElement->SetAttribute( anAttribute->Name(), &POEnet_noop[0]);
        }
        if (!strcmp(anElement->Name(), &POEnet_digital[0])) {
            copiedElement->SetAttribute( &POEnet_noop[0], 0);
            //anAttribute = anAttribute->Next();
            //copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
            //anAttribute = anAttribute->Next();
            //copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
        }
        if (!strcmp(anElement->Name(), &POEnet_switch[0])) {
            copiedElement->SetAttribute( &POEnet_noop[0], 0);
            //anAttribute = anAttribute->Next();
            //copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
            //anAttribute = anAttribute->Next();
            //copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
        }
        // */
        /*
        for( anAttribute = anElement->FirstAttribute(); anAttribute != 0; anAttribute = anAttribute->Next() ) {
            copiedElement->SetAttribute( anAttribute->Name(), anAttribute->Value());
        }
        // */
        POEnetCommand.RootElement()->InsertEndChild( copiedElement);
        anElement = anElement->NextSiblingElement();
    }
}

void POEnet_SetTime() {
    // time command should be <time>08:57:32</time>
    char newTime[APP_STRING_SIZE];
    strncpy( &newTime[0], POEnetCommand.RootElement()->GetText(), APP_STRING_SIZE);
    newTime[2] = '\0';
    newTime[5] = '\0';
    newTime[8] = '\0';
    // Can ask XMLUtil::ToInt if conversion was successful
    tinyxml2::XMLUtil::ToInt( &newTime[0], time_hours);
    tinyxml2::XMLUtil::ToInt( &newTime[3], time_minutes);
    tinyxml2::XMLUtil::ToInt( &newTime[6], time_seconds);
}

void POEnet_SetAnalog(tinyxml2::XMLElement *eleAnalog) {
    int myId = eleAnalog->IntAttribute( &POEnet_id[0]);
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    float newValue;
    tinyxml2::XMLElement *myAnalog;
    tinyxml2::XMLElement *anElement;
    myAnalog = POEnetNode.RootElement()->FirstChildElement(&POEnet_analog[0]);
    while (myAnalog) {
        if (myAnalog->IntAttribute( &POEnet_id[0]) == myId) {
            // found my Analog input -> check for numerator, denominator, unit subelements
            anElement = eleAnalog->FirstChildElement(&POEnet_numerator[0]);
            if (anElement) {
                // found the numerator -> query for new value
                if (anElement->QueryFloatText(&newValue) == tinyxml2::XML_SUCCESS) {
                    // Only if resolvable value
                    myAnalog->SetAttribute( &POEnet_numerator[0], newValue);
                }
            }
            anElement = eleAnalog->FirstChildElement(&POEnet_denominator[0]);
            if (anElement) {
                // found the denominator -> query for new value
                if (anElement->QueryFloatText(&newValue) == tinyxml2::XML_SUCCESS) {
                    // Only if resolvable value
                    myAnalog->SetAttribute( &POEnet_denominator[0], newValue);
                }
            }
            anElement = eleAnalog->FirstChildElement(&POEnet_offset[0]);
            if (anElement) {
                // found the denominator -> query for new value
                if (anElement->QueryFloatText(&newValue) == tinyxml2::XML_SUCCESS) {
                    // Only if resolvable value
                    myAnalog->SetAttribute( &POEnet_offset[0], newValue);
                }
            }
            anElement = eleAnalog->FirstChildElement(&POEnet_unit[0]);
            if (anElement) {
                // found the unit
                if (strlen(anElement->GetText())) {
                    myAnalog->SetAttribute( &POEnet_unit[0], anElement->GetText());
                }
            }
            myAnalog = 0;
        } else {
            myAnalog = myAnalog->NextSiblingElement(&POEnet_analog[0]);
        }
    }
}

void POEnet_GetAnalog(tinyxml2::XMLElement *eleAnalog) {
    int myId = eleAnalog->IntAttribute( &POEnet_id[0]);
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    tinyxml2::XMLElement *myAnalog;
	tinyxml2::XMLElement *anElement;
    myAnalog = POEnetNode.RootElement()->FirstChildElement(&POEnet_analog[0]);
    while (myAnalog) {
        if (myAnalog->IntAttribute( &POEnet_id[0]) == myId) {
            // 1st Version, put Value as Text in
            //eleAnalog->InsertFirstChild( eleAnalog->GetDocument()->NewText(myAnalog->Attribute(&POEnet_value[0])));
            // 2nd Version, put Value as value Attribute in and add configurational parameters as elements
            eleAnalog->SetAttribute(&POEnet_value[0], myAnalog->Attribute(&POEnet_value[0]));
            anElement = eleAnalog->GetDocument()->NewElement(&POEnet_numerator[0]);
            anElement->SetAttribute(&POEnet_value[0], myAnalog->Attribute(&POEnet_numerator[0]));
            eleAnalog->InsertEndChild( anElement);
            anElement = eleAnalog->GetDocument()->NewElement(&POEnet_denominator[0]);
            anElement->SetAttribute(&POEnet_value[0], myAnalog->Attribute(&POEnet_denominator[0]));
            eleAnalog->InsertEndChild( anElement);
            anElement = eleAnalog->GetDocument()->NewElement(&POEnet_offset[0]);
            anElement->SetAttribute(&POEnet_value[0], myAnalog->Attribute(&POEnet_offset[0]));
            eleAnalog->InsertEndChild( anElement);
            anElement = eleAnalog->GetDocument()->NewElement(&POEnet_unit[0]);
            anElement->SetAttribute(&POEnet_value[0], myAnalog->Attribute(&POEnet_unit[0]));
            eleAnalog->InsertEndChild( anElement);
            myAnalog = 0;
        } else {
            myAnalog = myAnalog->NextSiblingElement(&POEnet_analog[0]);
        }
    }
}

void POEnet_SetDigital(tinyxml2::XMLElement *eleDigital) {
    // POETODO: checking for lovalue, hivalue settings; translation with loValue & hiValue 
    int myId = eleDigital->IntAttribute( &POEnet_id[0]);
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    tinyxml2::XMLElement *myDigital;
    tinyxml2::XMLElement *anElement;
    myDigital = POEnetNode.RootElement()->FirstChildElement(&POEnet_analog[0]);
    while (myDigital) {
        if (myDigital->IntAttribute( &POEnet_id[0]) == myId) {
            // found my Digital input -> check for lovalue, hivalue subelements
            anElement = eleDigital->FirstChildElement(&POEnet_lovalue[0]);
            if (anElement) {
                // found the unit
                if (strlen(anElement->GetText())) {
                    myDigital->SetAttribute( &POEnet_lovalue[0], anElement->GetText());
                }
            }
            anElement = eleDigital->FirstChildElement(&POEnet_hivalue[0]);
            if (anElement) {
                // found the unit
                if (strlen(anElement->GetText())) {
                    myDigital->SetAttribute( &POEnet_hivalue[0], anElement->GetText());
                }
            }
            myDigital = 0;
        } else {
            myDigital = myDigital->NextSiblingElement(&POEnet_analog[0]);
        }
    }
}

void POEnet_GetDigital(tinyxml2::XMLElement *eleDigital) {
    // POETODO: translation with loValue & hiValue 
    int myId = eleDigital->IntAttribute( &POEnet_id[0]);
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    tinyxml2::XMLElement *myDigital;
	tinyxml2::XMLElement *anElement;
    myDigital = POEnetNode.RootElement()->FirstChildElement(&POEnet_digital[0]);
    while (myDigital) {
        if (myDigital->IntAttribute( &POEnet_id[0]) == myId) {
            // 1st Version, put Value as Text in
            //eleDigital->InsertFirstChild( eleDigital->GetDocument()->NewText(myDigital->Attribute(&POEnet_value[0])));
            // 2nd Version, put Value as value Attribute in and add configurational parameters as elements
            eleDigital->SetAttribute(&POEnet_value[0], myDigital->Attribute(&POEnet_value[0]));

            anElement = eleDigital->GetDocument()->NewElement(&POEnet_lovalue[0]);
            anElement->SetAttribute(&POEnet_value[0], myDigital->Attribute(&POEnet_lovalue[0]));
            eleDigital->InsertEndChild( anElement);
            
            anElement = eleDigital->GetDocument()->NewElement(&POEnet_hivalue[0]);
            anElement->SetAttribute(&POEnet_value[0], myDigital->Attribute(&POEnet_hivalue[0]));
            eleDigital->InsertEndChild( anElement);
            
            myDigital = 0;
        } else {
            myDigital = myDigital->NextSiblingElement(&POEnet_digital[0]);
        }
    }
}

void POEnet_SetSwitch(tinyxml2::XMLElement *eleSwitch) {
    // POETODO: translation with loValue & hiValue 
    int myId = eleSwitch->IntAttribute( &POEnet_id[0]);
    int newValue;
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    tinyxml2::XMLElement *mySwitch;
    tinyxml2::XMLElement *anElement;
    mySwitch = POEnetNode.RootElement()->FirstChildElement(&POEnet_switch[0]);
    while (mySwitch) {
        if (mySwitch->IntAttribute( &POEnet_id[0]) == myId) {
            // look for setable value 
            if (eleSwitch->QueryIntText(&newValue) == tinyxml2::XML_SUCCESS) {
                // Only if resolvable value; translation with loValue & hiValue not yet implemented
                mySwitch->SetAttribute( &POEnet_value[0], newValue);
            }
            // check for lovalue, hivalue setting
            anElement = eleSwitch->FirstChildElement(&POEnet_lovalue[0]);
            if (anElement) {
                // found the unit
                if (strlen(anElement->GetText())) {
                    mySwitch->SetAttribute( &POEnet_lovalue[0], anElement->GetText());
                }
            }
            anElement = eleSwitch->FirstChildElement(&POEnet_hivalue[0]);
            if (anElement) {
                // found the unit
                if (strlen(anElement->GetText())) {
                    mySwitch->SetAttribute( &POEnet_hivalue[0], anElement->GetText());
                }
            }
            mySwitch = 0;
        } else {
            mySwitch = mySwitch->NextSiblingElement(&POEnet_switch[0]);
        }
    }
}

void POEnet_GetSwitch(tinyxml2::XMLElement *eleSwitch) {
    // POETODO: translation with loValue & hiValue 
    int myId = eleSwitch->IntAttribute( &POEnet_id[0]);
    if (!myId) { myId = 1; } // if not (or invalid) specified take 1st 
    tinyxml2::XMLElement *mySwitch;
	tinyxml2::XMLElement *anElement;
    mySwitch = POEnetNode.RootElement()->FirstChildElement(&POEnet_switch[0]);
    while (mySwitch) {
        if (mySwitch->IntAttribute( &POEnet_id[0]) == myId) {
            eleSwitch->SetAttribute(&POEnet_value[0], mySwitch->Attribute(&POEnet_value[0]));

            anElement = eleSwitch->GetDocument()->NewElement(&POEnet_lovalue[0]);
            anElement->SetAttribute(&POEnet_value[0], mySwitch->Attribute(&POEnet_lovalue[0]));
            eleSwitch->InsertEndChild( anElement);

            anElement = eleSwitch->GetDocument()->NewElement(&POEnet_hivalue[0]);
            anElement->SetAttribute(&POEnet_value[0], mySwitch->Attribute(&POEnet_hivalue[0]));
            eleSwitch->InsertEndChild( anElement);

            mySwitch = 0;
        } else {
            mySwitch = mySwitch->NextSiblingElement(&POEnet_switch[0]);
        }
    }
}

void POEnet_Interpret(const char *buffer) {
    POEnetCommand.Parse(buffer);
    if (!POEnetCommand.Error()) {
        if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_reset[0])) {
            // no thing to do, just pass on
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_net[0])) {
            POEnet_GetNewNodeId();
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_node[0])) {
            if (POEnetCommand.RootElement()->IntAttribute(&POEnet_id[0]) == *node_id) {
                POEnet_DescribeNode();
            }
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_time[0])) {
            POEnet_SetTime();
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_analog[0])) {
            if (POEnetCommand.RootElement()->IntAttribute(&POEnet_node[0]) == *node_id) {
                POEnet_SetAnalog(POEnetCommand.RootElement());
                POEnet_GetAnalog(POEnetCommand.RootElement());
            }
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_digital[0])) {
            if (POEnetCommand.RootElement()->IntAttribute(&POEnet_node[0]) == *node_id) {
                POEnet_SetDigital(POEnetCommand.RootElement());
                POEnet_GetDigital(POEnetCommand.RootElement());
            }
        } else if (!strcmp(POEnetCommand.RootElement()->Name(), &POEnet_switch[0])) {
            if (POEnetCommand.RootElement()->IntAttribute(&POEnet_node[0]) == *node_id) {
                POEnet_SetSwitch(POEnetCommand.RootElement());
                POEnet_GetSwitch(POEnetCommand.RootElement());
                // POETODO: Clear element and signal to state machine
            }
        }
    }
}

bool POEnet_GetError(char *tostring) {
    if (POEnetCommand.Error()) {
        strncpy(tostring, POEnetCommand.ErrorName(), APP_STRING_SIZE);
        return true;
    }
    return false; // no error
}

void POEnet_Output(char *buffer) {
    tinyxml2::XMLPrinter myOut(true);
    POEnetCommand.Print(&myOut);
    strcpy(buffer,myOut.CStr());
    POEnetCommand.Clear();
}

void POEnet_GetCommand(char *tostring) {
    char myCommand[APP_STRING_SIZE];
    strncpy(myCommand, POEnetCommand.RootElement()->Name(), APP_STRING_SIZE);
    if (!strcmp(&myCommand[0], &POEnet_reset[0])) {
        strcpy(tostring, &POEnet_noop[0]);
    } else {
        strcpy(tostring, &myCommand[0]);
    }
}

