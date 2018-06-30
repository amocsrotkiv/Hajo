#pragma once

enum ECapabilityType {
	CAPABILITY_TYPE_MOTOR,
	CAPABILITY_TYPE_SENSOR_VOLTAGE,
	CAPABILITY_TYPE_SENSOR_TEMPERATURE
	// majd kibővítjük
}

#define defNAME_STRING_LENGTH	32
struct stCapability { // szenzor, motor vagy valami kis egyszerű szirszar, kb 1db GPIO bonyolultságú
	ECapabilityType	eType;
	short			sCapabilityID;
	char			szName[defNAME_STRING_LENGTH];
	short			sMaxVal;
	short			sMinVal;
}


// a Command_* struktúrákat az esp küldi az arduino-nak
// a Response_* struktúrákat az arduino küldi az esp-nek

enum EPacketType {
	COMMAND_TYPE_REQUEST_CAPABILITIES = 1, // eszköz képességeinek lekérése (pl max motor sebesség)
	RESPONSE_TYPE_REQUEST_CAPABILITIES, // eszköz képességeinek vissza küldése
	
	COMMAND_TYPE_CAPABILITY_UPDATE, // tulajdonság értékének beállítása, pl motor vagy led vagy akármi
	RESPONSE_TYPE_CAPABILITY_UPDATE, // szenzor érték frissítés
}

/*
A protokol az az, hogy minden üzenet áll 2 byteból, ami az előbbi enumeációban felel meg egy értéknek
Ez alapján lehet switchelni, hogy melyik struktúrát fogjuk kiolvasni az alábbiak közül:
*/

#define defMAX_CAPABILITIES_NUM	32
// struct Command_RequestCapabilities {}
struct stResponse_RequestCapabilities {
	short			sNumCapabilities;
	stCapability	capability_buff[defMAX_CAPABILITIES_NUM];
}


struct stCommand_CapabilityUpdate {
	short	sCapabilityID;
	short	sVal;
}
struct stResponse_CapabilityUpdate {
	short	sCapabilityID;
	short	sVal;
}

/*
példa implementáció (arduino oldal):
(olvasás)
if (Serial.available()) {
	short sCommandID = (Serial.read() << 0x08) | Serial.read(); // BidEndian short olvasás
	switch (sCommandID) {
		case COMMAND_TYPE_REQUEST_CAPABILITIES:
			// ez a parancs csak a parancsból áll, azt kéri, hogy RESPONSE_TYPE_REQUEST_CAPABILITIES-al válaszoljunk
			sendCapabilities();
		break;
		case COMMAND_TYPE_CAPABILITY_UPDATE:
			stCommand_CapabilityUpdate stUpdateData;
			Serial.readBytes(&stUpdateData, sizeof(stUpdateData));
			
			// stUpdateData-al lehet baszakodni
		break;
		default:
			// ismeretlen parancs érkezett
		break;
	}
}

(írás)
void sendCapabilities () {
	stResponse_RequestCapabilities stCapabilities;
	stCapabilities.sNumCapabilities = 1;
	stCapabilities.capability_buff[0].eType = CAPABILITY_TYPE_MOTOR;
	stCapabilities.capability_buff[0].sCapabilityID = 1;
	stCapabilities.capability_buff[0].szName = "Jobb motor"; // ebbe nem vagyok biztos, hogy const char*-ot így lehet char[32]-hüz rendelni, ha nem, akkor string cpy
	stCapabilities.capability_buff[0].sMinVal = -1024;
	stCapabilities.capability_buff[0].sMaxVal = 1024;
	
	Serial.write(&stCapabilities, sizeof(stCapabilities));
}
*/




