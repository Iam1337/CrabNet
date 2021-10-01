/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  Copyright (c) 2016-2018, TES3MP Team
 *  Copyright (c) Vladimir Sigalkin
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

// The simplest wrapper for working with StolenNetwork 

#pragma once

#include "RakNetDefines.h"
#include "RakPeerInterface.h"
#include "BitStream.h"

using namespace RakNet;

extern "C"
{
	// PEERS
	extern RAK_DLL_EXPORT RakPeerInterface* PEER_CreateInstance();

	extern RAK_DLL_EXPORT void PEER_DestroyInstance(RakPeerInterface* peer);

	// SERVER
	extern RAK_DLL_EXPORT int PEER_SetupServer(RakPeerInterface* peer, const char* serverHost, unsigned short serverPort, unsigned short maxConnections);

	extern RAK_DLL_EXPORT void PEER_CloseConnection(RakPeerInterface* peer, unsigned long long guid);

	// CLIENT
	extern RAK_DLL_EXPORT int PEER_SetupClient(RakPeerInterface* peer, const char* serverHost, unsigned short serverPort, unsigned int retries, unsigned int retryDelay, unsigned int timeout);

	// RECEIVE
	extern RAK_DLL_EXPORT bool PEER_Receive(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT unsigned int PACKET_GetLength(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT unsigned long long PACKET_GetGUID(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT const char* PACKET_GetAddressPtr(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT unsigned short PACKET_GetPort(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT bool PACKET_ReadBytes(RakPeerInterface* peer, unsigned char* bytes);

	// SEND
	extern RAK_DLL_EXPORT void PACKET_StartPacket(RakPeerInterface* peer);

	extern RAK_DLL_EXPORT void PACKET_WriteBytes(RakPeerInterface * peer, const char* bytes, unsigned int size);
	   
	extern RAK_DLL_EXPORT int PACKET_SendPacketUnicast(RakPeerInterface* peer, unsigned long long guid, unsigned char priority, unsigned char reliability,unsigned char channel);

	extern RAK_DLL_EXPORT int PACKET_SendPacketBroadcast(RakPeerInterface* peer, unsigned char priority, unsigned char reliability, unsigned char channel);

	// SHARED
	extern RAK_DLL_EXPORT void PEER_GetStatistics(RakPeerInterface* peer, unsigned long long guid, RakNetStatistics* statistics);

	extern RAK_DLL_EXPORT const char* PEER_GetStatisticsStringPtr(RakPeerInterface* peer, unsigned long long guid, int verbosityLevel);

	extern RAK_DLL_EXPORT int PEER_GetAveragePing(RakPeerInterface* peer, unsigned long long guid);

	extern RAK_DLL_EXPORT int PEER_GetLastPing(RakPeerInterface* peer, unsigned long long guid);

	extern RAK_DLL_EXPORT int PEER_GetLowestPing(RakPeerInterface* peer, unsigned long long guid);
}
