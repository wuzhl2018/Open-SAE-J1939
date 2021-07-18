/*
 * SAE_J1939_Request.c
 *
 *  Created on: 14 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "SAE_J1939-21_Transport_Layer.h"

/*
 * Read a PGN request from another ECU about PGN information at this ECU
 * PGN: 0x00EA00 (59904)
 */
void SAE_J1939_Read_Request(J1939 *j1939, uint8_t SA, uint8_t data[]) {
	uint32_t PGN = (data[2] << 16) | (data[1] << 8) | data[0];
	if (PGN == PGN_ACKNOWLEDGEMENT) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
	} else if (PGN == PGN_ADDRESS_CLAIMED){
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_Address_Claimed(j1939, 0xFF); 					/* Broadcast to all ECU */
	} else if (PGN == PGN_COMMANDED_ADDRESS) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
	} else if (PGN == PGN_DM1) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_DM1(j1939, SA);
	} else if (PGN == PGN_DM2) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_DM2(j1939, SA);
	} else if (PGN == PGN_DM3) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_DM3(j1939);
	} else if (PGN == PGN_REQUEST) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
	} else if (PGN == PGN_TP_CM_BAM) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
	} else if (PGN == PGN_TP_DT) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
	} else if (PGN >= PGN_AUXILIARY_VALVE_ESTIMATED_FLOW_0 && PGN <= PGN_AUXILIARY_VALVE_ESTIMATED_FLOW_15) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		//SAE_J1939_Response_Request_Auxiliary_Valve_Estimated_Flow(j1939, PGN & 0xF); /* PGN & 0xF = valve_number */
	} else if (PGN == PGN_GENERAL_PURPOSE_VALVE_ESTIMATED_FLOW){
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		//SAE_J1939_Response_Request_General_Purpose_Valve_Estimated_Flow(j1939, SA);
	} else if (PGN == PGN_SOFTWARE_IDENTIFICATION) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_Software_Identification(j1939, SA);
	} else if (PGN == PGN_ECU_IDENTIFICATION) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_ECU_Identification(j1939, SA);
	} else if (PGN == PGN_COMPONENT_IDENTIFICATION) {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_SUPPORTED, j1939->this_name.function, j1939->this_ECU_address, PGN);
		SAE_J1939_Response_Request_Component_Identification(j1939, SA);
	} else {
		SAE_J1939_Send_Acknowledgement(SA, j1939->this_ECU_address, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_NOT_SUPPORTED, 0xFF, j1939->this_ECU_address, PGN);
	}
	/* Add more else if statements here */
}

/*
 * Request PGN information at other ECU
 * PGN: 0x00EA00 (59904)
 */
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Request(J1939* j1939, uint8_t DA, uint32_t PGN_code) {
	uint8_t PGN[3];
	PGN[0] = PGN_code;														/* PGN least significant bit */
	PGN[1] = PGN_code >> 8;													/* PGN mid bit */
	PGN[2] = PGN_code >> 16;												/* PGN most significant bit */
	uint32_t ID = (0x18EA << 16) | (DA << 8) | j1939->this_ECU_address;
	return CAN_Send_Request(ID, PGN, 100);									/* 100 ms delay */
}