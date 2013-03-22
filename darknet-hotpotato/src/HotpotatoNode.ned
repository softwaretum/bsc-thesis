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

package darknet_hotpotato;

simple HotpotatoNode like inet.applications.udpapp.UDPApp { //darknetsim.DarknetBaseNode {
    parameters:
        int dest_port;
        int dest_id;
        volatile string dest_address;
        
        
        int local_port;
        int node_id;
        
    gates:
	    input udpIn;
	    output udpOut;
    
}