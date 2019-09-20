#ifndef PiHoleService_h
#define PiHoleService_h

struct PiHoleSummary
{
  unsigned long dnsQueriesToday;
  unsigned long adsBlockedToday;
  unsigned long dnsQueriesCachedToday;
  unsigned long piHole1Updated;
  unsigned long piHole2Updated;
  String piHole1RecentBlockedDomain;
  String piHole2RecentBlockedDomain;
};

#endif
