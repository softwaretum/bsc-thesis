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

#include <omnetpp.h>
#include "DarknetBaseNode.h"
#include "IPAddressResolver.h"
//#include "UDPControlInfo_m.h"


//Define_Module(DarknetBaseNode);

void DarknetBaseNode::initialize(int stage)
{
//    peers = std::map<int, DarknetNode>();
    nodeID = par("node_id");
    localPort = par("local_port");
    bindToPort(localPort);
}


void DarknetBaseNode::sendPacket(cPacket* pkg, IPvXAddress* destAddr, int destPort)
{
    sendToUDP(pkg, localPort, (*destAddr), destPort);
}

void DarknetBaseNode::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        // maybe a timer / internal message handling?
        // or a packet that we want so send..
        sendMessage(check_and_cast<DarknetMessage*>(msg));
    }
    else
    {
        processIncomingPacket(check_and_cast<DarknetMessage*>(msg));
    }
}


void DarknetBaseNode::processIncomingPacket(DarknetMessage *msg)
{
    if (msg->destNodeID != nodeID) {
        if(msg->TTL > 0) {
            msg->TTL -= 1;
            sendMessage(msg);
        }
        //forward message
    }

    //distinguish MessageTypes
}
