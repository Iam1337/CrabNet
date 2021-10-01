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

#include <map>
#include "StolenNetwork.h"
#include "RakNetStatistics.h"


std::map <RakPeerInterface*, Packet* > _packetsStorage;
std::map <RakPeerInterface*, BitStream* > _streamsStorage;

RakNetStatistics* _tempStatistics = new RakNetStatistics();
char _tempStatisticsBuffer[8096];

// PEER
RAK_DLL_EXPORT RakPeerInterface* PEER_CreateInstance()
{
	RakPeerInterface* peer = RakPeerInterface::GetInstance();
	BitStream* bitStream = new BitStream();

	// Add peer to storage.
	_packetsStorage.insert(std::make_pair(peer, nullptr));
	_streamsStorage.insert(std::make_pair(peer, bitStream));

	return peer;
}

RAK_DLL_EXPORT void PEER_DestroyInstance(RakPeerInterface* peer)
{
	if (_packetsStorage[peer] != nullptr)
	{
		peer->DeallocatePacket(_packetsStorage[peer]);
		_packetsStorage.erase(peer);
	}

	if (_streamsStorage[peer] != nullptr)
	{
		delete _streamsStorage[peer];
		_streamsStorage.erase(peer);
	}

	RakPeerInterface::DestroyInstance(peer);
}

// SERVER
RAK_DLL_EXPORT int PEER_SetupServer(RakPeerInterface* peer, const char* serverHost, unsigned short serverPort, unsigned short maxConnections)
{
	SocketDescriptor socketDescriptor(serverPort, serverHost);

	int startupResult = peer->Startup(maxConnections, &socketDescriptor, 1);
	if (startupResult == CRABNET_STARTED)
	{
		peer->SetMaximumIncomingConnections(maxConnections);
		peer->SetOccasionalPing(true);
	}

	return startupResult;
}

RAK_DLL_EXPORT void PEER_CloseConnection(RakPeerInterface* peer, unsigned long long guidId)
{
	// Get address by guid.
	RakNetGUID guid(guidId);
	SystemAddress address = peer->GetSystemAddressFromGuid(guid);

	// Close connection.
	peer->CloseConnection(address, true);
}

// CLIENT
RAK_DLL_EXPORT int PEER_SetupClient(RakPeerInterface* peer, const char* serverHost, unsigned short serverPort, unsigned int retries, unsigned int retryDelay, unsigned int timeout)
{
	SocketDescriptor socketDescriptor;

	int startupResult = peer->Startup(1, &socketDescriptor, 1);
	if (startupResult == CRABNET_STARTED)
	{
		int connectionResult = (int)peer->Connect(serverHost, serverPort, 0, 0, 0, 0, retries, retryDelay, timeout);
		if (connectionResult == CONNECTION_ATTEMPT_STARTED)
		{
			peer->SetOccasionalPing(true);
		}

		startupResult = 100 + connectionResult;
	}

	return startupResult;
}

// RECEIVE
RAK_DLL_EXPORT bool PEER_Receive(RakPeerInterface* peer)
{
	Packet* packet = peer->Receive();
	if (packet != 0)
	{
		// Dealloc previous packet.
		if (_packetsStorage[peer] != nullptr)
			peer->DeallocatePacket(_packetsStorage[peer]);

		// Update packet in storage.
		_packetsStorage[peer] = packet;

		return true;
	}

	return false;
}

RAK_DLL_EXPORT unsigned int PACKET_GetLength(RakPeerInterface* peer)
{
	Packet* packet = _packetsStorage[peer];
	return packet->length;
}

RAK_DLL_EXPORT unsigned long long PACKET_GetGUID(RakPeerInterface* peer)
{
	Packet* packet = _packetsStorage[peer];
	RakNetGUID guid = packet->guid;

	return guid.g;
}

RAK_DLL_EXPORT const char* PACKET_GetAddressPtr(RakPeerInterface* peer)
{
	Packet* packet = _packetsStorage[peer];
	SystemAddress address = packet->systemAddress;

	return address.ToString(false);
}

RAK_DLL_EXPORT unsigned short PACKET_GetPort(RakPeerInterface* peer)
{
	Packet* packet = _packetsStorage[peer];
	SystemAddress address = packet->systemAddress;

	return packet->systemAddress.GetPort();
}

RAK_DLL_EXPORT bool PACKET_ReadBytes(RakPeerInterface* peer, unsigned char* bytes)
{
	Packet* packet = _packetsStorage[peer];

	if (packet == nullptr)
		return false;

	memcpy(bytes, packet->data, packet->length);

	return true;
}

// SEND
RAK_DLL_EXPORT void PACKET_StartPacket(RakPeerInterface* peer)
{
	BitStream* bitStream = _streamsStorage[peer];
	bitStream->Reset();
}

RAK_DLL_EXPORT void PACKET_WriteBytes(RakPeerInterface* peer, const char* bytes, unsigned int size)
{
	//BitStream buffer(bytes, size, true);
	BitStream* bitStream = _streamsStorage[peer];
	bitStream->Write(bytes, size);
}

RAK_DLL_EXPORT int PACKET_SendPacketUnicast(RakPeerInterface* peer, unsigned long long guidId, unsigned char priority, unsigned char reliability, unsigned char channel)
{
	//Get peer bitstream.
	BitStream* bitStream = _streamsStorage[peer];

	// Get address by guid.
	RakNetGUID guid(guidId);
	SystemAddress address = peer->GetSystemAddressFromGuid(guid);

	// Send data.
	return peer->Send(bitStream, (PacketPriority)priority, (PacketReliability)reliability, channel, address, false);
}

RAK_DLL_EXPORT int PACKET_SendPacketBroadcast(RakPeerInterface* peer, unsigned char priority, unsigned char reliability, unsigned char channel)
{
	BitStream* bitStream = _streamsStorage[peer];

	return peer->Send(bitStream, (PacketPriority)priority, (PacketReliability)reliability, (char)channel, UNASSIGNED_SYSTEM_ADDRESS, true);
}

// SHARED
RAK_DLL_EXPORT void PEER_GetStatistics(RakPeerInterface* peer, unsigned long long guidId, RakNetStatistics* statistics)
{
	if (guidId == 0)
	{
		peer->GetStatistics(UNASSIGNED_SYSTEM_ADDRESS, statistics);
	}
	else
	{
		RakNetGUID guid(guidId);
		SystemAddress address = peer->GetSystemAddressFromGuid(guid);

		peer->GetStatistics(address, statistics);
	}
}

RAK_DLL_EXPORT const char* PEER_GetStatisticsStringPtr(RakPeerInterface* peer, unsigned long long guidId, int verbosityLevel)
{
	if (guidId == 0)
	{
		peer->GetStatistics(UNASSIGNED_SYSTEM_ADDRESS, _tempStatistics);
	}
	else
	{
		RakNetGUID guid(guidId);
		SystemAddress address = peer->GetSystemAddressFromGuid(guid);

		peer->GetStatistics(address, _tempStatistics);
	}

	StatisticsToString(_tempStatistics, _tempStatisticsBuffer, verbosityLevel);

	return _tempStatisticsBuffer;
}

RAK_DLL_EXPORT int PEER_GetAveragePing(RakPeerInterface* peer, unsigned long long guidId)
{
	// Get address by guid.
	RakNetGUID guid(guidId);
	SystemAddress address = peer->GetSystemAddressFromGuid(guid);

	return peer->GetAveragePing(address);
}

RAK_DLL_EXPORT int PEER_GetLastPing(RakPeerInterface* peer, unsigned long long guidId)
{
	// Get address by guid.
	RakNetGUID guid(guidId);
	SystemAddress address = peer->GetSystemAddressFromGuid(guid);

	return peer->GetLastPing(address);
}

RAK_DLL_EXPORT int PEER_GetLowestPing(RakPeerInterface* peer, unsigned long long guidId)
{
	// Get address by guid.
	RakNetGUID guid(guidId);
	SystemAddress address = peer->GetSystemAddressFromGuid(guid);

	return peer->GetLowestPing(address);
}