#include<bits/stdc++.h>
#define mp make_pair
using namespace std;

enum ProBo { meal, drinks, economy, business}; 
int flightIdGenerator = 0;

class Flight
{
    private:
        string flightId;
        string airline;
        string source;
        string destination;
        int price;
        vector<ProBo> ProBolist;
        string departureTime;
        string arrivalTime; 
        
        int timeToMinutes(string time)  
        {
            int h = stoi(time.substr(0, 2));
            int m = stoi(time.substr(3, 2));
            return h * 60 + m;
        }

    public:
        Flight(string airline, string source, string dest, int cost, vector<ProBo> ProBolist)
        {
            this->airline=airline;
            this->source=source;
            this->destination=dest;
            this->price=cost;
            this->ProBolist=ProBolist;
            setId();
        }

        Flight(string airline, string source, string dest, int cost, vector<ProBo> ProBolist, string dept, string arrival)
        {
            this->airline=airline;
            this->source=source;
            this->destination=dest;
            this->price=cost;
            this->ProBolist=ProBolist;
            this->departureTime=dept;
            this->arrivalTime=arrival;
            setId();
        }

        void setId()
        {
            this->flightId = source+"_"+destination+"_"+airline+"_"+to_string(flightIdGenerator++);
        }

        string getId()
        {
            return flightId;
        }

        int getPrice()
        {
            return price;
        }

        string getAirlines()
        {
            return airline;
        }

        string getSource()
        {
            return source;
        }

        string getDestination()
        {
            return destination;
        }

        vector<ProBo> getProBo()
        {
            return ProBolist;
        }

        string getDepartureTime() 
        { 
            return departureTime; 
        }
        
        string getArrivalTime() 
        { 
            return arrivalTime; 
        }

        int getDepartureMinutes() 
        { 
            return timeToMinutes(departureTime); 
        }

        int getArrivalMinutes() 
        { 
            return timeToMinutes(arrivalTime); 
        }
};

class Route
{
    private:
        int totalPrice;
        vector<Flight> route;

    public:
        Route(int totalPrice, vector<Flight> route)
        {
            this->totalPrice=totalPrice;
            this->route=route;
        }

        void setTotalCost(int cost)
        {
            this->totalPrice = cost;
        }

        void setFlightlist(vector<Flight> newRoute)
        {
            this->route = newRoute;
        }

        int getTotalCost()
        {
            return totalPrice;
        }

        vector<Flight> getFlightlist()
        {
            return route;
        }
};

struct CompareRoute {
    bool operator()(Route& a, Route& b) 
    {
        return a.getTotalCost() > b.getTotalCost();
    }
};

map<string, Flight> flightMap; //id, flight

void register_Flight(string airline, string source, string destination, int cost, vector<ProBo> ProBolist)
{
    Flight newFlight(airline, source, destination, cost, ProBolist);
    flightMap.insert({newFlight.getId(), newFlight});
    cout<<"Flight "<<newFlight.getId()<<" registered from "<<source<<" to "<<destination<<" with "<<airline<<" airlines."<<endl;
    return;
}

void sortFlightsByTime(vector<Flight>& flights) {
    sort(flights.begin(), flights.end(), []( Flight& a, Flight& b) {
        if (a.getDepartureMinutes() == b.getDepartureMinutes()) {
            return a.getArrivalMinutes() < b.getArrivalMinutes();
        }
        return a.getDepartureMinutes() < b.getDepartureMinutes();
    });
}

vector<Flight> routeWithMinHops(string source, string destination, vector<Flight>& flights)
{
    map<string, vector<Flight>> flightMapSource;
    for (auto& flight : flights) {
        flightMapSource[flight.getSource()].push_back(flight);
    }

    queue<vector<Flight>> q;
    set<string> visited;

    if (flightMapSource.count(source)) {
        for (auto& flight : flightMapSource[source]) {
            q.push({ flight });
        }
        visited.insert(source);
    }

    while (!q.empty()) {
        auto currentRoute = q.front();
        q.pop();

        Flight& lastFlight = currentRoute.back();
        if (lastFlight.getDestination() == destination) {
            return currentRoute;
        }

        if (flightMapSource.count(lastFlight.getDestination())) {
            for (auto& nextFlight : flightMapSource[lastFlight.getDestination()]) {
                if (!visited.count(nextFlight.getDestination())) {
                    auto newRoute = currentRoute;
                    newRoute.push_back(nextFlight);
                    q.push(newRoute);
                }
            }
        }
        visited.insert(lastFlight.getDestination());
    }

    return {};
}

vector<Flight> cheapestRoute(string source, string destination, vector<Flight>& flights) 
{
    map<string, vector<Flight>> graph;
    for (auto& flight : flights) {
        graph[flight.getSource()].push_back(flight);
    }
    priority_queue<Route, vector<Route>, CompareRoute> pq;

    map<string, int> minCostMap;
    minCostMap[source] = 0;

    pq.push(Route(0, {}));

    while (!pq.empty()) {
        auto currentRoute = pq.top();
        pq.pop();

        string currentCity;
        if (currentRoute.getFlightlist().empty())
            currentCity = source;
        else
            currentCity = currentRoute.getFlightlist().back().getDestination();

        int currentCost = currentRoute.getTotalCost();
        if (currentCity == destination) {
            return currentRoute.getFlightlist();
        }
        if (graph.count(currentCity)) 
        {
            for (auto& nextFlight : graph[currentCity]) 
            {
                int newCost = currentCost + nextFlight.getPrice();

                if (!minCostMap.count(nextFlight.getDestination()) || newCost < minCostMap[nextFlight.getDestination()]) 
                {
                    auto newRoute = currentRoute.getFlightlist();
                    newRoute.push_back(nextFlight);
                    pq.push(Route(newCost, newRoute));
                    minCostMap[nextFlight.getDestination()] = newCost;
                }
            }
        }
    }

    return {};
}

void printFlight(vector<Flight> flights)
{
    for(auto flight: flights)
    {
        cout<<flight.getSource()<<" to "<<flight.getDestination()<<" via "<<flight.getAirlines()<<" for "<<flight.getPrice()<<endl;
    }

    int totalCost = 0;
    int totalFlights = 0;
    for (auto& flight : flights) {
        totalCost += flight.getPrice();
        totalFlights++;
    }
    cout << "Total Flights = " << totalFlights << "\n";
    cout << "Total Cost = " << totalCost << "\n";
}

void searchFlightsWithFilter(string source, string destination, ProBo proBo) {
    vector<Flight> flightList;
    for(auto flight: flightMap) 
    {
        auto currFlight = flight.second;
        if (find(currFlight.getProBo().begin(), currFlight.getProBo().end(), proBo) != currFlight.getProBo().end()) {
            flightList.push_back(currFlight);
        }
    }

    cout<<"Route with Minimum Hops:"<<endl;
    auto hopsRoute = routeWithMinHops(source, destination, flightList);
    printFlight(hopsRoute);

    cout << "Cheapest Route:"<<endl;
    auto cheapRoute = cheapestRoute(source, destination, flightList);
    printFlight(cheapRoute);
}

void searchFlights(string source, string destination){
    vector<Flight> flightList;
    for(auto flight: flightMap)
    {
        flightList.push_back(flight.second);
    }

    cout<<"Route with Minimum Hops:"<<endl;
    auto hopsRoute = routeWithMinHops(source, destination, flightList);
    printFlight(hopsRoute);

    cout << "Cheapest Route:"<<endl;
    auto cheapRoute = cheapestRoute(source, destination, flightList);
    printFlight(cheapRoute);
}

int main()
{
    register_Flight("JetAir", "DEL", "BLR", 500, {});
    register_Flight("JetAir", "BLR", "LON", 1000, {});
    register_Flight("Delta", "DEL", "LON", 2000, {});
    register_Flight("Delta", "LON", "NYC", 2000, {});
    register_Flight("IndiGo", "LON", "NYC", 2500, {ProBo::meal});
    register_Flight("IndiGo", "DEL", "BLR", 600, {ProBo::meal, economy});
    register_Flight("IndiGo", "BLR", "PAR", 800, {ProBo::meal, business});
    register_Flight("IndiGo", "PAR", "LON", 300, {ProBo::meal});

    searchFlights("DEL", "NYC");

    // Filtered search
    searchFlightsWithFilter("DEL", "NYC", ProBo::meal);
    return 0;
}