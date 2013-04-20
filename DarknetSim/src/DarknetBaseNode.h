//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef DARKNETBASENODE_H_
#define DARKNETBASENODE_H_

#include <omnetpp.h>
#include <UDPAppBase.h>
//#include "messages/DarknetMessage.h"
//#include "messages/MaintenanceMessage.h"
#include "darknetmessage_m.h"

typedef struct {
    std::string nodeID;
    IPvXAddress address;
    int port;
} DarknetPeer;

//typedef struct {
//    std::string nodeID;
//    IPvXAddress address;
//    int port;
//    simtime_t lastSeen;
//} DarknetConnection;


class DarknetBaseNode : public UDPAppBase  {
public:
    DarknetBaseNode() {};
    virtual ~DarknetBaseNode() { };

protected:

    std::string nodeID;
    int localPort;
    std::map<std::string, DarknetPeer*> peers;
//    std::map<std::string, DarknetPeer*> connections;

    //things you probably don't have to change
    virtual void sendPacket(DarknetMessage* pkg, IPvXAddress& destAddr, int destPort);
    virtual void sendMessage(DarknetMessage* msg);
    virtual void handleMessage(cMessage* msg);
    virtual int numInitStages() const { return 4; }
    virtual void addPeer(std::string nodeID, IPvXAddress& destAddr, int destPort);
    virtual DarknetMessage* makeRequest(std::string nodeID);


    //things you probably want to implement or extend
    virtual void initialize(int stage);
    virtual void handleIncomingMessage(DarknetMessage* msg);
    virtual void handleForeignMessage(DarknetMessage* msg) { forwardMessage(msg); };
    virtual void handleRequest(DarknetMessage* msg);
    virtual void forwardMessage(DarknetMessage* msg);


    //things you have to implement
    virtual void connectPeer(std::string nodeID, IPvXAddress* destAddr = NULL, int destPort = 0) = 0;
    virtual DarknetPeer* findNextHop(DarknetMessage* msg) = 0;
    virtual void handleSelfMessage(cMessage* msg) = 0;


    // higher level interface for DarknetNodes
 //   virtual void connectPeer(IPvXAddress* destAddr, int destPort);

 //   virtual void handleMaintenanceMessage(MaintenanceMessage* msg);
 //   virtual void handleRequestMessage(DarknetMessage* msg);


};

#endif /* DARKNETBASENODE_H_ */
