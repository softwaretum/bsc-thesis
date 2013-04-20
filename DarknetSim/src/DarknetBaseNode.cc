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
#include <IPAddressResolver.h>
#include "DarknetBaseNode.h"
#include <UDPPacket.h>
#include <cstringtokenizer.h>

void DarknetBaseNode::initialize(int stage) {
    if(stage == 0) {
        nodeID = par("node_id").stdstringValue();
        localPort = par("local_port");
        bindToPort(localPort);
    }else if (stage == 3) {
        const char* port_delimiter = ":";
        std::vector<std::string> v = cStringTokenizer(par("dest_id")).asVector();
        for(std::vector<std::string>::iterator iter = v.begin(); iter != v.end(); iter++) {
            std::vector<std::string> peer_tuple = cStringTokenizer((*iter).c_str(),port_delimiter).asVector(); //split <destID>:<destPort>
            if(peer_tuple.size() == 2) {
                std::string nodeID = peer_tuple[0];
                std::istringstream convert(peer_tuple[1]);
                int port;
                port = convert >> port ? port : 0;  //convert string to int (user 0 on error)
                IPvXAddress ip = IPAddressResolver().resolve(nodeID.c_str());
                addPeer(nodeID, ip, port);
            }else {
                EV << "Error on parsing peer list; this peer seems malformed: " << (*iter);
            }
        }
    }
}

void DarknetBaseNode::sendPacket(DarknetMessage* dmsg, IPvXAddress& destAddr, int destPort) {
    sendToUDP(dmsg, localPort, destAddr, destPort);
}

void DarknetBaseNode::addPeer(std::string nodeID, IPvXAddress& destAddr, int destPort) {
    DarknetPeer* peer = new DarknetPeer;
    peer->nodeID = nodeID;
    peer->address = destAddr;
    peer->port = destPort;
    peers.insert(std::pair<std::string, DarknetPeer*>(nodeID,peer));
}

void DarknetBaseNode::sendMessage(DarknetMessage* msg) {
    DarknetPeer *destPeer = findNextHop(msg);
    if(destPeer != NULL) {
        int size = msg->getVisitedNodesArraySize();
        msg->setVisitedNodesArraySize(size+1);
        msg->setVisitedNodes(size,this->nodeID.c_str());
        sendPacket(msg,destPeer->address,destPeer->port);
    } else {
        EV << "No next hop found for message: " << msg;
        //TODO: implement proper default error handling here
        delete msg;
    }
}

void DarknetBaseNode::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else {
        DarknetMessage* dm = dynamic_cast<DarknetMessage*>(msg);
        if( dm != NULL) {
            if (dm->getDestNodeID() != nodeID) {
                    handleForeignMessage(dm);
            }else handleIncomingMessage(dm);
        } else {
            EV << "received an unknown cMessage: " << msg;
            delete msg;
        }
    }
}

void DarknetBaseNode::handleIncomingMessage(DarknetMessage *msg) {
    switch(msg->getType()) {
    case DM_REQUEST:
        handleRequest(msg);
        break;
     default:
       EV << "received unknown DarknetMessage for this node: " << msg;
       break;
    }
    delete msg;
}

void DarknetBaseNode::forwardMessage(DarknetMessage* msg) {
    int ttl = msg->getTTL();
    if(ttl > 0) {
        msg->setTTL(ttl-1);
        sendMessage(((DarknetMessage*)msg)->dup());
    }else {
        // TODO: inform simulator/user of droped message
        EV << "dropped message";
    }
    delete msg;
}

DarknetMessage* DarknetBaseNode::makeRequest(std::string nodeID) {
    DarknetMessage *msg = new DarknetMessage();
    msg->setSrcNodeID(this->nodeID.c_str());
    msg->setDestNodeID(nodeID.c_str());
    msg->setType(DM_REQUEST);
    return msg;
    sendMessage(msg);
}

void DarknetBaseNode::handleRequest(DarknetMessage* request) {
    DarknetMessage *msg = new DarknetMessage();
    msg->setSrcNodeID(this->nodeID.c_str());
    msg->setDestNodeID(request->getSrcNodeID());
    msg->setType(DM_RESPONSE);
    sendMessage(msg);
}
