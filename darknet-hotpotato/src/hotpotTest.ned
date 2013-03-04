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


import inet.nodes.inet.StandardHost;
import inet.nodes.inet.Router;
import inet.networklayer.autorouting.FlatNetworkConfigurator;
import ned.DatarateChannel;



network HotpotTest
{
    submodules:
        configurator: FlatNetworkConfigurator {
            @display("p=24,28;i=block/cogwheel");
            networkAddress = "145.236.0.0";
            netmask = "255.255.0.0";
        }
        host1: StandardHost {
            @display("p=63,118");
        }
        router: Router {
            @display("p=138,54");
        }
        host2: StandardHost {
            @display("p=204,118");
        }
    connections:
        host1.pppg++ <--> DatarateChannel <--> router.pppg++;
        router.pppg++ <--> DatarateChannel <--> host2.pppg++;

}
