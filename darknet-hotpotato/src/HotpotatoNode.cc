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
    DarknetNode peer = { peerID, IPAddressResolver().resolve(par("dest_address")), (int) par("dest_port")};
    peers->insert(std::pair<int, DarknetNode>(peerID,peer));

}

void HotpotatoNode::sendMessage(DarknetMessage* msg) {
    if(!peers->size()) {
        // peer list empty -> raise exception?
        return;
    }
    DarknetNode destPeer;
    if(peers->find(msg->destNodeID) != peers->end()) {
        destPeer = peers[msg->destNodeID];
    }else {
        std::map<int, DarknetNode>::iterator iter = peers->begin();
        std::advance(iter, 0 /* TODO: random iteration */);
        destPeer = iter->second;
    }
    sendPacket(msg,destPeer.address,destPeer.port);
}
