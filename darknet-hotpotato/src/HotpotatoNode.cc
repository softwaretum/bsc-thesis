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
#include "IPAddressResolver.h"

Define_Module(HotpotatoNode);

void HotpotatoNode::initialize(int stage)
{

    DarknetBaseNode::initialize(stage);

    int peerID = par("dest_ID");
    DarknetNode* peer = new DarknetNode;
    peer->nodeID = peerID;
    peer->address = IPAddressResolver().resolve(par("dest_address"));
    peer->port = par("dest_port");
    peer->active = true;
    peers.insert(std::pair<int, DarknetNode*>(peerID,peer));

}

void HotpotatoNode::sendMessage(DarknetMessage* msg) {
    if(!peers.size()) {
        // peer list empty -> raise exception?
        return;
    }
    DarknetNode *destPeer;
    if(peers.find(msg->destNodeID) != peers.end()) {
//        destPeer = peers[msg->destNodeID];
        destPeer = peers[0];
    }else {
        std::map<int, DarknetNode*>::iterator iter = peers.begin();
        std::advance(iter, dblrand() * peers.size());
        destPeer = iter->second;
    }
    sendPacket(msg,&(destPeer->address),destPeer->port);
}
