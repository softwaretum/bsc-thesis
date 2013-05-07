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

#include "HotpotatoNode.h"

#include <string>
#include <vector>

#include <cenvir.h>
#include <cmessage.h>
#include <cstringtokenizer.h>
#include <IPvXAddress.h>

#include "darknetmessage_m.h"

PingTimer::~PingTimer() {
}

Define_Module(HotpotatoNode)

void HotpotatoNode::initialize(int stage) {
    DarknetBaseNode::initialize(stage);

    if (stage == 5) {
        const std::vector<std::string> v = cStringTokenizer(par("ping_id")).asVector();
        for(std::vector<std::string>::const_iterator iter = v.begin(); iter != v.end(); iter++) {
            cMessage *timer = new PingTimer(*iter);
            scheduleAt(1.0, timer);
        }
    }
}

void HotpotatoNode::connectPeer(const std::string& toNodeID, const IPvXAddress& destAddr, int destPort) {
    DarknetMessage *dm = new DarknetMessage();
    dm->setType(DM_CON_SYN);
    dm->setSrcNodeID(nodeID.c_str());
    dm->setDestNodeID(toNodeID.c_str());
    sendPacket(dm, destAddr, destPort);
//    sendMessage(dm);
}

DarknetPeer* HotpotatoNode::findNextHop(DarknetMessage* msg) {
    if(!connections.size()) { // peer list empty -> raise exception? (TODO)
        EV << "ERROR: empty peer list!";
        return NULL;
    }
    std::map<std::string, DarknetPeer>::iterator destPeer = peers.find(msg->getDestNodeID());
    if(destPeer != peers.end() and connections.find(msg->getDestNodeID()) != connections.end()) {
        return &destPeer->second;
    }else {
        std::map<std::string, DarknetConnection>::const_iterator iter = connections.begin();
        std::advance(iter, dblrand() * connections.size());
        //assert(peers.find(iter->first) != peers.end()); ?
        return &peers[iter->first];
    }
}

void HotpotatoNode::handleSelfMessage(cMessage *msg) {
    if(dynamic_cast<PingTimer*>(msg) != NULL) {
        sendMessage(makeRequest(msg->getName()));
    }else {
        EV << "received unknown self-message " << msg;
    }
    delete msg;
}

void HotpotatoNode::handleIncomingMessage(DarknetMessage *msg) {
    switch(msg->getType()) {
    case DM_RESPONSE:
        EV << "received PONG from: " << msg->getSrcNodeID() << endl;
        delete msg;
        break;
    case DM_CON_SYN: {
        std::map<std::string, DarknetPeer>::const_iterator peerIter = peers.find(msg->getSrcNodeID());
        if(peerIter != peers.end()) {
            EV << "received CON_SYN from: " << msg->getSrcNodeID() << endl;
            const DarknetPeer& peer = peerIter->second;
            DarknetMessage *dm = new DarknetMessage();
            dm->setType(DM_CON_ACK);
            dm->setSrcNodeID(msg->getDestNodeID());
            dm->setDestNodeID(msg->getSrcNodeID());
            sendPacket(dm, peer.address, peer.port);
        }
        delete msg;
        break;
    }
    case DM_CON_ACK: {
        DarknetConnection& dc = connections[msg->getSrcNodeID()];
        dc.nodeID = msg->getSrcNodeID();
        dc.lastSeen = simulation.getSimTime();
        EV << "connection to " << dc.nodeID << "established" << endl;
        delete msg;
        break;
    }
    default:
       DarknetBaseNode::handleIncomingMessage(msg);
       break;
    }
}
