#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"

#include "wireless-planning/net-test.h"
#include "wireless-planning/create-network.h"
#include "wireless-planning/network-config.h" 
#include "wireless-planning/print.h"
#include "wireless-planning/netinfo-reader.h"
#include "wireless-planning/report-2-config-data.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("mysimulation");

int
main (int argc, char *argv[])
{
  Time eos = Seconds (5);

  /* Process command line options */
  CommandLine cmd;
  cmd.AddValue ("netinfo", "Network Information File", netInfoFile);
  cmd.Parse (argc, argv);

  /* Read netinfo and build network */
  NetDataStruct::NetData netData = NetinfoReader::Read ((netInfoFile));
  Print::Netinfo (netData);
  NetworkConfig::NetworkData networkData = Report2ConfigData::NetData2NetworkData (netData);
  Print::NetworkData (networkData);
  CreateNetwork createNetwork;
  NodeContainer nodes = createNetwork.Create (networkData);
  Print::NodeList (nodes);

  /* Setup simulation */
  NetTest netTest;
  netTest.EnablePcap("Josjojauarina 2", 1);
    
  netTest.ApplicationSetup ("Urcos", "Ccatcca", 1.0, 3.0, "10Mbps", 1452, NULL);
  netTest.ApplicationSetup ("Urcos", "Huiracochan", 2.0, 4.5, "10Mbps", 1452, NULL);

  std::vector < string > flowNames;
  flowNames.push_back ("Urcos-Ccatcca");
  flowNames.push_back ("Urcos-Huiracochan");

  /* Setup all the plot system: throughput measurement, gnuplot */
  NetMeasure netMeasure (eos, Seconds (0.1));
  netMeasure.SetupPlot ();
  netMeasure.SetFlowNames (flowNames);
  netMeasure.SetFlowMonitor (nodes);
  netMeasure.GetFlowStats ();

  Simulator::Stop (eos);
  
  NS_LOG_INFO ("Starting simulation...");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  
  return 0;
}