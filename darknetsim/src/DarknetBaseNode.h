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
#include "UDPAppBase.h"
#include "messages/DarknetMessage.h"
#include "messages/MaintenanceMessage.h"

struct DarknetNode{
    int nodeID;
    IPvXAddress address;
    int port;
    bool active;
};

class DarknetBaseNode : public UDPAppBase  {
protected:

    int nodeID;
    int localPort;
    std::map<int, DarknetNode*> peers;

    static int counter;


    // higher level interface for DarknetNodes
    virtual void sendMessage(DarknetMessage* msg);
 //   virtual void connectPeer(IPvXAddress* destAddr, int destPort);

 //   virtual void handleMaintenanceMessage(MaintenanceMessage* msg);
 //   virtual void handleRequestMessage(DarknetMessage* msg);


    virtual void handleMessage(cMessage* msg);
    virtual void processIncomingPacket(DarknetMessage* msg);
    virtual void sendPacket(cPacket* pkg, IPvXAddress* destAddr, int destPort); // most likely only a wrapper around sendToUDP


//    virtual int numInitStages() const { return 4; } //TODO: why? why 4?
    virtual void initialize(int stage);
};

#endif /* DARKNETBASENODE_H_ */
