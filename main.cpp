#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <map>
#include <tuple>
#include <queue>
#include<set>



using namespace std;


// Structure to store package details
struct Package
{
    string agentName;
    string city;
    string details;
    double price;

    Package(const std::string& agent, const std::string& c, const std::string& d, double p)
    : agentName(agent), city(c), details(d), price(p) {}
};



struct BookingRequest
{
    std::string touristName;
    Package package;
    bool confirmed;

    // Flag for VIP status
    bool isVIP;

    // Flag for cancellation request
    bool cancellationRequested;

    // Field to track current location
    string currentLocation;

     // New field to track accommodation change requests
    string accommodationChangeRequest;

    // Email or phone number
    string contactInfo;
    time_t bookingTime;

    // Regular or VIP
    string bookingType;

    // Default constructor
    BookingRequest() : package("", "", "", 0.0), confirmed(false), isVIP(false), cancellationRequested(false), currentLocation(""), accommodationChangeRequest("") {}

    // Custom constructor
    BookingRequest(const std::string& name, const Package& pkg, bool vip = false)
        : touristName(name), package(pkg), confirmed(false), isVIP(vip), cancellationRequested(false), currentLocation(""), accommodationChangeRequest("") {}
};


struct VIPPackage : Package
{
    double vipExtraCost;  // Extra cost for VIP packages
    string vipFacilities; // Additional facilities for VIP packages

    // Constructor to initialize VIPPackage
    VIPPackage(const string& agentName, const string& city, const string& details, double price, double vipExtraCost, const string& vipFacilities)
        : Package{agentName, city, details, price}, vipExtraCost(vipExtraCost), vipFacilities(vipFacilities) {}
};
struct AccommodationChangeRequest
{
    std::string touristName;
    std::string currentAccommodation;
    std::string requestedAccommodation;
    std::string reason;
};

struct MaintenanceIssue
{
    std::string touristName;
    std::string accommodation;
    std::string issueDescription;
};


struct Edge2
{
    int destination;
    int distance;
    int traffic;
    int adjustedWeight;
    int capacity;
    std::string roadType;
    int vehiclesOnRoad;
};

// Emergency details structure
struct EmergencyDetails
 {
    std::string touristName;
    std::string emergencyDescription;
    std::string currentLocation;
    std::string preferredEvacuationMethod;
};

struct EvacuationPlan
 {
    std::string planDetails;
    std::string evacuationRoute;
    std::string contactInfo;

    EvacuationPlan(const std::string& details, const std::string& route, const std::string& contact)
        : planDetails(details), evacuationRoute(route), contactInfo(contact) {}
};

map<string, string> touristCredentials;
queue<BookingRequest> bookingQueue;
queue<BookingRequest> vipBookingQueue;

// Add this line
queue<BookingRequest> cancellationQueue;
map<string, BookingRequest> confirmedBookings;

// Adjacency list for routes
vector<vector<Edge2>> adjList;

// Mapping from index to city name
unordered_map<int, std::string> indexToCity;

// Queue to store evacuation requests
queue<EmergencyDetails> evacuationRequests;

// Global variable for the guide's agency
string currentGuideAgency;

 // Queue to store emergency evacuation requests
queue<std::string> evacuationQueue;
queue<AccommodationChangeRequest> changeRequests;
queue<MaintenanceIssue> maintenanceIssues;
queue<BookingRequest> emergencyQueue;
set<string> flaggedBookings;
queue<BookingRequest> downtimeQueue;
bool systemDown = false;

// Assuming a fixed number of staff members
const int NUM_STAFF = 5;
std::vector<std::queue<BookingRequest>> staffQueues(NUM_STAFF);

// Assuming you have these global variables to track resources
int availableStaff = 10;
int availableVehicles = 5;


// Function prototypes
void mainMenu();
void touristMenu();
void touristRoutePlanning();
void guideMenu();
void adminMenu();
void travelWithGuide();
void travelWithoutGuide();
void chooseTransport(const string& destination);
void displayPackages(const vector<Package>& packages);
void submitBookingRequest(const Package& selectedPackage, const string& touristName, int numberOfPeople, bool isVIP = false);
void processBookingRequests();
void manageVIPBookings();
void processPayment(const BookingRequest& confirmedRequest, int numberOfPeople);
bool authenticateGuide(const string& username, const string& password);
bool authenticateAdmin(const string& username, const string& password);
void routeplan();
void ratings();
void manageEventBookings();
string toLowerCase(const string& str);
void displayAllBookingRequests();
int accomodation();
int adminconnect();
int touristconnect();
int food();
void processCancellationRequests();
void requestCancellation();

//bool isTransportationLimited(const std::vector<Edge2>& edges);

//void displayLimitedTransportationRoutes(const std::vector<std::vector<Edge2>>& adjList, const std::unordered_map<int, std::string>& indexToCity);
void requestEmergencyEvacuation();
void processEmergencyEvacuationRequests();
void updateLocation();
void viewTouristLocations();
void requestAccommodationChange();
void processAccommodationChangeRequests();
void reportMaintenanceIssue();
void processMaintenanceIssues();
void prioritizeRequests();
void loadBalanceRequests();
void notifyUsers();
void handlePeakTimeService();
void reviewFlaggedBookings() ;
bool isFraudulentBooking(const BookingRequest& request);
void processDowntimeRequests();

vector<Package> packages =
 {
    {"Ramesh_Travel", "Delhi", "3 days, 2 nights - Includes hotel, meals, and guided tours", 5000.0},
    {"Sunshine_Tours", "Mumbai", "4 days, 3 nights - Includes hotel, breakfast, and city tours", 7000.0},
    {"Jaipur_Journeys", "Jaipur", "2 days, 1 night - Includes hotel and cultural tours", 4000.0},
    {"Kerala_Adventures", "Kerala", "5 days, 4 nights - Includes houseboat stay, meals, and sightseeing", 12000.0},
    {"Heritage_Travels", "Agra", "1 day - Includes Taj Mahal visit and lunch", 2000.0},
    {"Golden_Triangle", "Delhi, Agra, Jaipur", "7 days - Includes guided tours, hotels, and meals", 15000.0},
    {"Goa_Getaways", "Goa", "3 days, 2 nights - Includes beach resorts and water sports", 10000.0},
    {"Himalayan_Escapes", "Manali", "5 days, 4 nights - Includes hotel, trekking, and adventure sports", 15000.0},
    {"Rajasthan_Royals", "Jodhpur", "3 days, 2 nights - Includes fort tours and local cuisine", 8000.0},
    {"Kashmir_Splendor", "Srinagar", "4 days, 3 nights - Includes houseboat stay and sightseeing", 14000.0},
    {"Eastern_Charms", "Kolkata", "3 days, 2 nights - Includes cultural tours and meals", 6000.0},
    {"Wildlife_Wonders", "Ranthambore", "2 days, 1 night - Includes safari and stay", 7000.0},
    {"Southern_Sojourn", "Chennai", "3 days, 2 nights - Includes temple tours and beaches", 6500.0},
    {"Leh_Expeditions", "Leh", "6 days, 5 nights - Includes trekking, camping, and sightseeing", 20000.0},
    {"Andaman_Bliss", "Port Blair", "5 days, 4 nights - Includes beach stay and water sports", 18000.0}
};

vector<VIPPackage> vipPackages =
{
    VIPPackage("Ramesh_Travel", "Delhi", "3 days, 2 nights - VIP includes hotel, meals, guided tours, and luxury transfers", 7000.0, 2000.0, "Luxury transfers, exclusive sightseeing, VIP lounges"),
    VIPPackage("Sunshine_Tours", "Mumbai", "4 days, 3 nights - VIP includes hotel, breakfast, city tours, and premium meals", 10000.0, 3000.0, "Premium meals, exclusive events, personal guide"),
    VIPPackage("Kerala_Adventures", "Kerala", "5 days, 4 nights - VIP includes houseboat stay, meals, sightseeing, and adventure sports", 15000.0, 4000.0, "Adventure sports, spa services, VIP guided tours"),
    VIPPackage("Goa_Getaways", "Goa", "3 days, 2 nights - VIP includes beach resorts, water sports, and personalized services", 15000.0, 5000.0, "Personalized concierge, private beach access, luxury dining"),
    VIPPackage("Leh_Expeditions", "Leh", "6 days, 5 nights - VIP includes trekking, camping, exclusive sightseeing, and premium accommodations", 25000.0, 5000.0, "Private camping, luxury accommodations, VIP transport")
};

vector<pair<string, string>> guideCredentials =
{
    {"Ramesh_Travel", "ramesh123"},
    {"Sunshine_Tours", "sunshine456"},
    {"Jaipur_Journeys", "jaipur789"},
    {"Kerala_Adventures", "kerala321"},
    {"Heritage_Travels", "heritage654"},
    {"Golden_Triangle", "golden987"},
    {"Goa_Getaways", "goa246"},
    {"Himalayan_Escapes", "himalaya135"},
    {"Rajasthan_Royals", "rajasthan864"},
    {"Kashmir_Splendor", "kashmir975"},
    {"Eastern_Charms", "eastern531"},
    {"Wildlife_Wonders", "wildlife420"},
    {"Southern_Sojourn", "southern357"},
    {"Leh_Expeditions", "leh951"},
    {"Andaman_Bliss", "andaman753"}
};

vector<pair<string, string>> adminCredentials =
{
    {"Admin", "admin@"}
};

int main()
 {
    while (true)
    {
        mainMenu();
    }
    return 0;
}

// ... rest of the code remains the same

void mainMenu()
{
    string username, password;
    int userType;

    cout << "\nWelcome to the Tourism Login System!\n";
    cout << "Please choose your role:\n";
    cout << "1. Tourist\n";
    cout << "2. Tourist Guide\n";
    cout << "3. Administrator\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> userType;

    if (userType == 4)
    {
        cout << "Exiting the system. Goodbye!\n";
        exit(0);
    }

    // Validate user type
    if (userType < 1 || userType > 4)
    {
        cout << "Invalid choice. Please try again.\n";
        return;
    }


    if (userType == 1)
    {
        // Tourist Login or Sign-Up
        int loginChoice;
        cout << "\n1. Log In\n";
        cout << "2. Sign Up\n";
        cout << "Enter your choice: ";
        cin >> loginChoice;

        if (loginChoice == 1)
            {
            // Tourist Log In
            cout << "\nEnter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            if (touristCredentials.find(username) != touristCredentials.end() && touristCredentials[username] == password)
            {
                cout << "\nLogin successful!\n";
                touristMenu(); // Proceed to tourist menu
            }
             else
            {
                cout << "\nInvalid username or password. Please try again.\n";
            }
        }
        else if
            (loginChoice == 2)
            {
            // Tourist Sign Up
            cout << "\nEnter a username: ";
            cin >> username;

            // Check if username already exists
            if (touristCredentials.find(username) != touristCredentials.end())
            {
                cout << "\nUsername already exists. Please choose a different username.\n";
                return;
            }

            cout << "Enter a password: ";
            cin >> password;

            // Save the new user's credentials
            touristCredentials[username] = password;
            cout << "\nSign-up successful! You can now log in.\n";
        }
        else
            {
            cout << "\nInvalid choice. Returning to main menu.\n";
        }
    }
    cout << "\nEnter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    if (userType == 2 && !authenticateGuide(username, password))
    {
        cout << "\nLogin failed: Invalid guide credentials.\n";
        return;
    }

    if (userType == 3 && !authenticateAdmin(username, password))
    {
        cout << "\nLogin failed: Invalid admin credentials.\n";
        return;
    }

    cout << "\nLogin successful!\n";

    if (userType == 2)
    {

        // Set the current guide agency for VIP booking processing
        for (const auto& credential : guideCredentials)
         {
            if (credential.first == username)
             {
                currentGuideAgency = credential.first;
                break;
            }
        }
    }

    switch (userType)
    {
        case 1:
            touristMenu();
            break;

        case 2:
            guideMenu();
            break;

        case 3:
            adminMenu();
            break;

        default:
            cout << "\nInvalid role. Exiting...\n";
            return;
    }
}

bool authenticateGuide(const string& username, const string& password)
{
    for (const auto& credential : guideCredentials)
        {
        if (credential.first == username && credential.second == password)
        {
            return true;
        }
    }
    return false;
}

bool authenticateAdmin(const string& username, const string& password)
{
    for (const auto& credential : adminCredentials)
        {
        if (credential.first == username && credential.second == password)
         {
            return true;
        }
    }

    return false;
}
void requestCancellation()
{
    std::string touristName;
    std::cout << "Enter your name to request a cancellation: ";
    std::cin.ignore();
    std::getline(std::cin, touristName);

    auto it = confirmedBookings.find(touristName);
    if (it != confirmedBookings.end() && it->second.confirmed)
    {
        it->second.cancellationRequested = true;
        cancellationQueue.push(it->second);
        std::cout << "Cancellation request submitted for " << touristName << ".\n";
    }
    else
    {
        std::cout << "No confirmed booking found for " << touristName << ".\n";
    }
}

void processCancellationRequests()
{
    if (cancellationQueue.empty())
    {
        std::cout << "\nNo cancellation requests to process.\n";
        return;
    }

    while (!cancellationQueue.empty())
    {
        BookingRequest request = cancellationQueue.front();
        cancellationQueue.pop();

        std::cout << "\nProcessing cancellation request for " << request.touristName << ":\n";
        std::cout << "   Package: " << request.package.agentName << " - " << request.package.city << "\n";
        std::cout << "   Details: " << request.package.details << "\n";
        std::cout << "   Price per person: Rs. " << request.package.price << "\n";

        char confirmation;
        std::cout << "Confirm cancellation? (y/n): ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y')
        {
            std::cout << "\nBooking cancelled for " << request.touristName << ".\n";
            confirmedBookings.erase(request.touristName); // Remove from confirmed bookings
        }
        else
        {
            std::cout << "\nCancellation request declined for " << request.touristName << ".\n";
            bookingQueue.push(request); // Re-add to booking queue if not cancelled
        }
    }
}

// Function to request emergency evacuation
void requestEmergencyEvacuation()
{
    EmergencyDetails details;
    cout << "Enter your name: ";
    cin.ignore();
    getline(std::cin, details.touristName);

    cout << "Describe the emergency situation: ";
    getline(std::cin, details.emergencyDescription);

    cout << "Enter your current location: ";
    getline(std::cin, details.currentLocation);

    cout << "Preferred method of evacuation (e.g., car, bus, helicopter): ";
    getline(std::cin, details.preferredEvacuationMethod);

    evacuationRequests.push(details);
    std::cout << "Emergency evacuation request submitted for " << details.touristName << ".\n";
}

// Function to process emergency evacuation requests
void processEmergencyEvacuationRequests()
{
    if (evacuationRequests.empty())
        {

        std::cout << "\nNo emergency evacuation requests to process.\n";
        return;
    }

    while (!evacuationRequests.empty())
        {

        EmergencyDetails details = evacuationRequests.front();
        evacuationRequests.pop();

        cout << "\nProcessing emergency evacuation request for " << details.touristName << ":\n";
        cout << "   Emergency: " << details.emergencyDescription << "\n";
        cout << "   Location: " << details.currentLocation << "\n";
        cout << "   Preferred Evacuation Method: " << details.preferredEvacuationMethod << "\n";

        // Suggest evacuation plans

        cout << "Suggested Evacuation Plans:\n";
        cout << "1. Evacuate by " << details.preferredEvacuationMethod << " to the nearest safe location.\n";
        cout << "2. Evacuate by bus to the emergency shelter in the nearby city.\n";
        cout << "3. Evacuate by helicopter to the nearest hospital for medical assistance.\n";

        char confirmation;
        std::cout << "Confirm the plan for " << details.touristName << "? (y/n): ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y')
        {
            cout << "\nEmergency evacuation arranged for " << details.touristName << ".\n";
        }
        else
        {
            cout << "\nEmergency evacuation plan for " << details.touristName << " not confirmed.\n";
        }
    }
}
void updateLocation()
 {

    std::string touristName;
    std::cout << "Enter your name to update your location: ";
    std::cin.ignore();
    std::getline(std::cin, touristName);

    auto it = confirmedBookings.find(touristName);
    if (it != confirmedBookings.end() && it->second.confirmed)
    {
        std::cout << "Enter your current location: ";
        std::getline(std::cin, it->second.currentLocation);
        std::cout << "Location updated for " << touristName << ".\n";
    }
    else
    {
        std::cout << "No confirmed booking found for " << touristName << ".\n";
    }
}

void requestAccommodationChange()
{
    std::string touristName;
    std::cout << "Enter your name to request an accommodation change: ";
    std::cin.ignore();
    std::getline(std::cin, touristName);

    auto it = confirmedBookings.find(touristName);
    if (it != confirmedBookings.end() && it->second.confirmed)
    {

        std::cout << "Enter your new accommodation request details: ";
        std::getline(std::cin, it->second.accommodationChangeRequest);
        std::cout << "Accommodation change request submitted for " << touristName << ".\n";

    }
    else
    {
        std::cout << "No confirmed booking found for " << touristName << ".\n";
    }
}
void reportMaintenanceIssue()
{

    MaintenanceIssue issue;
    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, issue.touristName);

    std::cout << "Enter your accommodation: ";
    std::getline(std::cin, issue.accommodation);

    std::cout << "Describe the issue: ";
    std::getline(std::cin, issue.issueDescription);

    maintenanceIssues.push(issue);
    std::cout << "Maintenance issue reported for " << issue.accommodation << ".\n";

}

void processMaintenanceIssues()
{

    if (maintenanceIssues.empty())
    {

        std::cout << "\nNo maintenance issues to process.\n";
        return;

    }

    while (!maintenanceIssues.empty())
    {

        MaintenanceIssue issue = maintenanceIssues.front();
        maintenanceIssues.pop();

        std::cout << "\nProcessing maintenance issue for " << issue.touristName << " at " << issue.accommodation << ":\n";
        std::cout << "   Issue: " << issue.issueDescription << "\n";

        char confirmation;
        std::cout << "Confirm the issue is resolved? (y/n): ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y')
        {

            std::cout << "\nMaintenance issue resolved for " << issue.accommodation << ".\n";

        }
        else
        {

            std::cout << "\nMaintenance issue for " << issue.accommodation << " not resolved.\n";

        }
    }
}
void touristMenu()
{
    int choice;
    cout << "\nWelcome, Tourist!\n";
    cout << "Please choose an option:\n";
    cout << "1. Travel with a guide\n";
    cout << "2. Travel without a guide\n";
    cout << "3. Get shortest routes to visit different places with minimum costs from your city\n";
    cout << "4. Get a route plan\n";
    cout << "5. Book Accomodation\n";
    cout << "6. Path references\n";
    cout << "7. Get food recommendations\n";
    cout << "8. Reviews and ratings\n";
    cout << "9. Request for cancellation\n";
    cout << "10. Request emergency evacuation\n"; // New option for emergency evacuation
    cout << "11. Update your location\n";
    cout << "12. Submit change request for accommodation\n";
    cout << "13. Report a maintenance issue\n";
    cout << "14. Go back to main menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
     {
        case 1:
            travelWithGuide();

            break;

        case 2:

            travelWithoutGuide();

            break;

        case 3:

            cout << "Tourist Route Planning System\n";

            cout << "=============================\n";

            touristRoutePlanning();

            break;

        case 4:

            routeplan();

            break;

        case 5 :

            accomodation();

            break;

        case 6 :

            touristconnect();

            break;

        case 7:

            food();

            break;

        case 8:

            ratings();

            break;

         case 9:

            requestCancellation(); // Added call to request cancellation

            break;

        case 10:

            requestEmergencyEvacuation(); // Call the function to request emergency evacuation

            break;


        case 11:
            updateLocation(); // Added call to update location

            break;

        case 12:
            requestAccommodationChange(); // Added call to request accommodation change

            break;

        case 13:

            reportMaintenanceIssue(); // Added call to report maintenance issue

            break;

        case 14:

            return;

        default:

            cout << "\nInvalid option. Please try again.\n";

            break;

    }
}



void guideMenu() {
    cout << "\nWelcome, Tourist Guide!\n";
    cout << "Here you can manage booking requests.\n";
    cout << "Please choose an option:\n";
    cout << "1. Manage regular bookings\n";
    cout << "2. Manage VIP bookings\n";
    cout << "3. Display all package booking requests\n";
    cout << "4. Manage event bookings\n";
    cout << "5. Return to main menu\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:

            processBookingRequests(); // Regular booking processing

            break;

        case 2:

            manageVIPBookings(); // VIP booking processing

            break;

        case 3:

            displayAllBookingRequests(); // New option to display all bookings

            break;

        case 4:

            manageEventBookings();

            break;

        case 5:

            return; // Return to the main menu

        default:

            cout << "\nInvalid option. Please try again.\n";

            break;

    }
}


/*
bool isTransportationLimited(const std::vector<Edge2>& edges) {
    for (const auto& edge : edges) {
        if (edge.vehiclesOnRoad >= edge.capacity) {
            return true; // Transportation is limited on this route
        }
    }
    return false;
}


void displayLimitedTransportationRoutes(const std::vector<std::vector<Edge2>>& adjList, const std::unordered_map<int, std::string>& indexToCity) {
    std::cout << "Limited Transportation Routes:\n";
    bool foundLimited = false; // Track if any limited routes are found

    for (size_t i = 0; i < adjList.size(); ++i) {
        for (const auto& edge : adjList[i]) {
            if (edge.vehiclesOnRoad >= edge.capacity) {
                foundLimited = true;
                std::cout << indexToCity.at(i) << " -> " << indexToCity.at(edge.destination)
                          << " (" << edge.distance << " km, " << edge.traffic << "% traffic, "
                          << edge.capacity << " vehicles, " << edge.roadType << ") - Limited\n";
            }
        }
    }

    if (!foundLimited) {
        std::cout << "No limited transportation routes found.\n";
    }
}*/


void displayAllBookingRequests()
 {
    cout << "\nAll Booking Requests:\n";

    // Display regular bookings
    if (bookingQueue.empty())
        {
        cout << "\nNo regular bookings available.\n";
    }
    else
        {

        cout << "\nRegular Bookings:\n";
        queue<BookingRequest> tempQueue = bookingQueue; // Use a temporary queue to iterate without modifying the original
        while (!tempQueue.empty()) {
            BookingRequest request = tempQueue.front();
            tempQueue.pop();
            cout << "Tourist Name: " << request.touristName << "\n";
            cout << "Package: " << request.package.agentName << " - " << request.package.city << "\n";
            cout << "Details: " << request.package.details << "\n";
            cout << "Price: Rs. " << request.package.price << "\n";
            cout << "Status: " << (request.confirmed ? "Confirmed" : "Pending") << "\n";
            cout << "-----------------------------------\n";
        }

        // Option to manage regular bookings
        cout << "\nWould you like to manage regular bookings? (1: Yes, 2: No): ";
        int choice;
        cin >> choice;
        if (choice == 1)
            {
            processBookingRequests();
            return; // Exit this function after managing bookings
        }
    }

    // Display VIP bookings
    if (vipBookingQueue.empty())
        {
        cout << "\nNo VIP bookings available.\n";
    }
    else
        {
        cout << "\nVIP Bookings:\n";
        queue<BookingRequest> tempQueue = vipBookingQueue; // Use a temporary queue to iterate without modifying the original
        while (!tempQueue.empty())
         {
            BookingRequest request = tempQueue.front();
            tempQueue.pop();
            cout << "Tourist Name: " << request.touristName << "\n";
            cout << "Package: " << request.package.agentName << " - " << request.package.city << "\n";
            cout << "Details: " << request.package.details << "\n";
            cout << "Price: Rs. " << request.package.price << "\n";
            cout << "Status: VIP - " << (request.confirmed ? "Confirmed" : "Pending") << "\n";
            cout << "-----------------------------------\n";
        }


        // Option to manage VIP bookings
        cout << "\nWould you like to manage VIP bookings? (1: Yes, 2: No): ";
        int choice;
        cin >> choice;
        if (choice == 1)
            {
            manageVIPBookings();
            return; // Exit this function after managing VIP bookings
        }

    }


    cout << "\nReturning to the guide menu.\n";

}


//Function to update vehicle count on a route
/*void updateVehicleCount(int city1, int city2, int vehicles) {
    for (auto& edge : adjList[city1]) {
        if (edge.destination == city2) {
            edge.vehiclesOnRoad = vehicles;
        }
    }
    for (auto& edge : adjList[city2]) {
        if (edge.destination == city1) {
            edge.vehiclesOnRoad = vehicles;
        }
    }
}
*/


void viewTouristLocations()
{
    if (confirmedBookings.empty())
        {
        std::cout << "\nNo tourists with confirmed bookings.\n";
        return;
    }


    std::cout << "\nTourist Locations:\n";
    for (const auto& pair : confirmedBookings) {
        const BookingRequest& request = pair.second;
        std::cout << "Tourist: " << request.touristName << "\n";
        std::cout << "Current Location: " << request.currentLocation << "\n";
    }

}



void processAccommodationChangeRequests()
 {
    bool foundRequest = false;

    for (auto& pair : confirmedBookings)
        {
        BookingRequest& request = pair.second;
        if (!request.accommodationChangeRequest.empty())
        {
            foundRequest = true;
            std::cout << "\nProcessing accommodation change request for " << request.touristName << ":\n";

            std::cout << "   Current Accommodation: " << request.package.details << "\n";

            std::cout << "   Requested Change: " << request.accommodationChangeRequest << "\n";

            char confirmation;

            std::cout << "Confirm the accommodation change for " << request.touristName << "? (y/n): ";

            std::cin >> confirmation;

            if (confirmation == 'y' || confirmation == 'Y')
                {
                // Apply the accommodation change
                request.package.details = request.accommodationChangeRequest;
                request.accommodationChangeRequest.clear();

                std::cout << "\nAccommodation change confirmed for " << request.touristName << ".\n";
            }
            else
                {
                std::cout << "\nAccommodation change request for " << request.touristName << " not confirmed.\n";
            }
        }

    }

    if (!foundRequest)
        {
        std::cout << "\nNo accommodation change requests to process.\n";
    }

}




// Function to allocate additional resources during peak times
void allocateAdditionalResources()
 {
    std::cout << "Allocating additional resources...\n";

    // Increase the number of available staff
    int additionalStaff = 5; // Example number of additional staff members

    availableStaff += additionalStaff;

    std::cout << "Added " << additionalStaff << " additional staff members. Total available staff: " << availableStaff << "\n";

    // Increase the number of available vehicles
    int additionalVehicles = 3;
    // Example number of additional vehicles

    availableVehicles += additionalVehicles;
    std::cout << "Added " << additionalVehicles << " additional vehicles. Total available vehicles: " << availableVehicles << "\n";

    // You can add more logic here to allocate other types of resources if needed

}


void prioritizeRequests()
 {
    std::cout << "Prioritizing VIP and emergency requests...\n";
    // Process VIP and emergency requests first
    while (!vipBookingQueue.empty())
        {
        BookingRequest request = vipBookingQueue.front();
        vipBookingQueue.pop();
        // Process VIP request
        std::cout << "Processing VIP request for " << request.touristName << "\n";
    }
    while (!emergencyQueue.empty())
    {
        BookingRequest request = emergencyQueue.front();
        emergencyQueue.pop();

        // Process emergency request
        std::cout << "Processing emergency request for " << request.touristName << "\n";
    }
}




// Function to balance the load across available resources
void loadBalanceRequests()
 {
    std::cout << "Balancing the load across available resources...\n";

    // Distribute VIP and emergency requests first
    int staffIndex = 0;
    while (!vipBookingQueue.empty())
        {
        BookingRequest request = vipBookingQueue.front();
        vipBookingQueue.pop();
        staffQueues[staffIndex].push(request);
        staffIndex = (staffIndex + 1) % NUM_STAFF;
    }


    while (!emergencyQueue.empty())
        {
        BookingRequest request = emergencyQueue.front();
        emergencyQueue.pop();
        staffQueues[staffIndex].push(request);
        staffIndex = (staffIndex + 1) % NUM_STAFF;
    }


    // Distribute regular requests
    while (!bookingQueue.empty())
        {
        BookingRequest request = bookingQueue.front();
        bookingQueue.pop();
        staffQueues[staffIndex].push(request);
        staffIndex = (staffIndex + 1) % NUM_STAFF;
    }


    // Display the distribution of requests
    for (int i = 0; i < NUM_STAFF; ++i)
        {
        std::cout << "Staff member " << i + 1 << " has " << staffQueues[i].size() << " requests.\n";
    }

}


void notifyUsers()
{
    std::cout << "Notifying users about potential delays...\n";

    if (confirmedBookings.empty())
        {
        std::cout << "No confirmed bookings to notify.\n";

        return;
    }

    for (const auto& booking : confirmedBookings)
        {
        const BookingRequest& request = booking.second;

        // Simulate sending notification
        std::cout << "Sending notification to " << request.touristName
                  << " (" << request.contactInfo << ") about potential delays.\n";

    }

    std::cout << "All users have been notified.\n";
}



void handlePeakTimeService()
{
    int choice;

    do
        {
        std::cout << "\nHandling peak time service...\n";

        std::cout << "Please choose an option:\n";

        std::cout << "1. Allocate additional resources\n";

        std::cout << "2. Prioritize VIP and emergency requests\n";

        std::cout << "3. Load balance regular requests\n";

        std::cout << "4. Notify users about potential delays\n";

        std::cout << "5. Exit peak time service management\n";

        std::cout << "Enter your choice: ";

        std::cin >> choice;

        switch (choice)
        {
            case 1:

                allocateAdditionalResources();

                break;

            case 2:

                prioritizeRequests();

                break;

            case 3:

                loadBalanceRequests();

                break;

            case 4:

                notifyUsers();

                break;

            case 5:

                std::cout << "Exiting peak time service management.\n";

                break;

            default:

                std::cout << "Invalid option. Please try again.\n";

                break;

        }

    } while (choice != 5);

}



// Function to check for potentially fraudulent bookings
bool isFraudulentBooking(const BookingRequest& request)
{

    // Check if the same name and password have been used for other bookings
    for (const auto& booking : confirmedBookings)
        {
        if (booking.second.touristName == request.touristName && booking.second.contactInfo == request.contactInfo)
         {
            return true;
        }

    }

    return false;

}



// Function to review flagged bookings

void reviewFlaggedBookings()
{
    if (flaggedBookings.empty())
        {
        std::cout << "No flagged bookings to review.\n";
        return;
    }

    std::cout << "Reviewing flagged bookings:\n";

    for (const std::string& touristName : flaggedBookings)
        {
        const BookingRequest& request = confirmedBookings[touristName];

        std::cout << "Tourist: " << request.touristName << "\n";

        std::cout << "Contact Info: " << request.contactInfo << "\n";

        std::cout << "Booking Type: " << request.bookingType << "\n";

        std::cout << "Booking Time: " << std::ctime(&request.bookingTime);
        std::cout << "---------------------------------\n";

    }

}



// Function to process queued requests after downtime
void processDowntimeRequests()
 {
    while (!downtimeQueue.empty())
        {
        bookingQueue.push(downtimeQueue.front());
        downtimeQueue.pop();

    }

    std::cout << "All queued booking requests have been processed.\n";

}



void adminMenu()
{
    cout << "\nWelcome, Administrator!\n";
    cout << "Here you can manage the system settings and user accounts.\n";
    cout << "Please choose an option:\n";
    cout << "1. Manage regular bookings\n";
    cout << "2. Manage VIP bookings\n";
    cout << "3. Display all package booking requests\n";
    cout << "4. Handle connectivity\n";
    cout << "5. Process cancellation requests\n";
    cout << "6. Process emergency evacuation requests\n";// New option for processing evacuation requests
    cout << "7. View tourist locations\n";
    cout << "8. Process accommodation change requests\n";
    cout << "9. Process maintenance issues\n";
    cout << "10. Handle peak time service\n";
    cout << "11. Review flagged(fraud) bookings\n";
    cout << "12. Set system status (Up/Down)\n";
    cout << "Enter your choice: ";

    int choice;

    cin >> choice;


    switch (choice)
    {
        case 1:

            processBookingRequests();

            break;

        case 2:

            manageVIPBookings();

            break;

        case 3:

            displayAllBookingRequests(); // New option to display all bookings

            break;

        case 4:

            adminconnect();

            break;

        case 5:

            processCancellationRequests(); // Added call to process cancellations

            break;

        case 6:

            processEmergencyEvacuationRequests(); // Call the function to process evacuation requests

            break;

         case 7:

            viewTouristLocations(); // Added call to view tourist locations

            break;

         case 8:

            processAccommodationChangeRequests(); // Added call to process accommodation change requests

            break;

        case 9:

            processMaintenanceIssues(); // Added call to process maintenance issues

            break;

        case 10:

            handlePeakTimeService(); // Added call to handle peak time service

            break;

        case 11:

                reviewFlaggedBookings(); // Added call to review flagged bookings

                break;

        case 12:

                cout << "Enter system status (0 for Up, 1 for Down): ";

                std::cin >> systemDown;

                if (!systemDown)
                {
                    processDowntimeRequests();
                }
                break;

        default:

            cout << "\nInvalid option. Please try again.\n";
            break;

    }
}

// Helper function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}



// Struct to represent a ticket booking request
struct TicketRequest
{

    string eventName;   // Name of the event
    int numTickets;     // Number of tickets requested
    bool isVIP;         // Flag for VIP requests

    TicketRequest(const string &e, int n, bool v)
        : eventName(e), numTickets(n), isVIP(v) {}

};



// Custom queue implementation using vector
class CustomQueue
{

private:

    vector<TicketRequest> elements;

public:
    void enqueue(const TicketRequest &request)
    {
        elements.push_back(request);
    }

    TicketRequest dequeue()
    {
        if (elements.empty())
        {
            throw runtime_error("Queue is empty!");
        }
        TicketRequest frontElement = elements.front();
        elements.erase(elements.begin());
        return frontElement;
    }

    bool empty() const
    {
        return elements.empty();
    }

    size_t size() const
    {
        return elements.size();
    }

    void display() const
    {
        for (const auto &request : elements)
            {
            cout << "- Event: " << request.eventName
                 << ", Tickets: " << request.numTickets;
            if (request.isVIP) cout << " [VIP]";
            cout << endl;
        }
    }
};

// Ticket Booking System
class TicketBookingSystem
{
private:
    CustomQueue vipQueue;      // For VIP requests
    CustomQueue regularQueue;  // For regular requests
    int availableTickets;

    // Map to store event names (lowercase) and their costs (regular and VIP)
    unordered_map<string, pair<int, int>> eventCosts;

public:
    TicketBookingSystem(int totalTickets, const vector<tuple<string, int, int>> &eventList)
        : availableTickets(totalTickets)
        {
        for (const auto &event : eventList)
        {
            eventCosts[toLowerCase(get<0>(event))] = {get<1>(event), get<2>(event)};
        }
    }

    // Add a booking request
    void addRequest(const string &eventName, int numTickets, bool isVIP)
    {
        if (eventCosts.find(eventName) == eventCosts.end())
            {
            cout << "Invalid event name. Please try again.\n";
            return;
        }

        if (numTickets > availableTickets)
        {

            cout << "Request denied: Not enough tickets available.\n";
            return;

        }

        TicketRequest request(eventName, numTickets, isVIP);

        if (isVIP)
            {

            vipQueue.enqueue(request);
            cout << "VIP request added to the priority queue.\n";
        }
        else
        {

            regularQueue.enqueue(request);
            cout << "Regular request added to the queue.\n";

        }
    }

    // Process the next booking request and handle payment
    void processRequest()
    {
        if (!vipQueue.empty())
            {
            TicketRequest request = vipQueue.dequeue();
            processBooking(request);
            handlePayment(request);
        }
        else if (!regularQueue.empty())
        {
            TicketRequest request = regularQueue.dequeue();
            processBooking(request);
            handlePayment(request);
        }
        else
            {
            cout << "No requests to process.\n";
        }
    }

    // Display the number of tickets left
    void displayAvailableTickets() const
    {
        cout << "Available tickets: " << availableTickets << endl;
    }

    // Show pending requests in the queue
    void displayPendingRequests() const
    {
        if (vipQueue.empty() && regularQueue.empty())
        {
            cout << "No pending requests.\n";
            return;
        }

        if (!vipQueue.empty())
        {
            cout << "Pending VIP requests:\n";
            vipQueue.display();
        }

        if (!regularQueue.empty())
        {
            cout << "Pending regular requests:\n";
            regularQueue.display();
        }
    }

    // Display available events
    void displayEvents() const
    {
        cout << "Available events in the city:\n";
        for (const auto &event : eventCosts)
            {

            cout << "- " << event.first
                 << " (Regular: ₹" << event.second.first
                 << ", VIP: ₹" << event.second.second << ")\n";

        }
    }

private:
    // Helper to process a booking
    void processBooking(const TicketRequest &request)
    {
        if (request.numTickets <= availableTickets)
            {
            availableTickets -= request.numTickets;
            cout << "Processed " << (request.isVIP ? "[VIP] " : "") << "request for Event " << request.eventName
                 << ": " << request.numTickets << " tickets booked.\n";
        }
        else
            {
            cout << "Request denied: Not enough tickets available.\n";
        }
    }

    // Handle payment
    void handlePayment(const TicketRequest &request)
    {
        if (eventCosts.find(request.eventName) == eventCosts.end())
            {
            cout << "Invalid event data. Payment aborted.\n";
            return;
        }

        int costPerTicket = request.isVIP ? eventCosts[request.eventName].second : eventCosts[request.eventName].first;
        int totalCost = costPerTicket * request.numTickets;

        cout << "Total cost for " << request.numTickets << " tickets to " << request.eventName
             << " (" << (request.isVIP ? "VIP" : "Regular") << "): ₹" << totalCost << endl;

        cout << "Select payment mode:\n";
        cout << "1. Credit Card\n";
        cout << "2. Debit Card\n";
        cout << "3. Digital Wallet\n";
        cout << "4. Cash\n";
        int paymentMode;
        cin >> paymentMode;

        switch (paymentMode)
        {
        case 1:

            cout << "Payment successful using Credit Card.\n";
            break;

        case 2:

            cout << "Payment successful using Debit Card.\n";
            break;

        case 3:

            cout << "Payment successful using Digital Wallet.\n";
            break;

        case 4:

            cout << "Payment successful using Cash.\n";
            break;

        default:
            cout << "Invalid payment mode. Payment aborted.\n";
        }
    }
};




void manageEventBookings()
{
    int totalTickets;
    cout << "Enter total number of tickets available: ";
    cin >> totalTickets;

    vector<tuple<string, int, int>> eventList =
    {
        {"Concert by The Band", 50, 100},
        {"Art Exhibition", 30, 60},
        {"Food Festival", 20, 40},
        {"Tech Conference", 100, 200},
        {"Theater Play", 40, 80}
    };

    TicketBookingSystem system(totalTickets, eventList);

    while (true)
        {
        cout << "\nMenu:\n";
        cout << "1. Book tickets\n";
        cout << "2. Confirm booking\n";
        cout << "3. Display Available Tickets\n";
        cout << "4. Display Pending Requests\n";
        cout << "5. Display Events\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
         {
        case 1:
            {
            string eventName;
            int numTickets;
            char isVIPInput;
            system.displayEvents();
            cout << "Enter the name of the event: ";
            cin.ignore();
            getline(cin, eventName);
            cout << "Enter Number of Tickets: ";
            cin >> numTickets;
            cout << "Is this a VIP request? (y/n): ";
            cin >> isVIPInput;
            bool isVIP = (isVIPInput == 'y' || isVIPInput == 'Y');
            system.addRequest(eventName, numTickets, isVIP);
            break;
        }
        case 2:

            system.processRequest();
            break;

        case 3:

            system.displayAvailableTickets();
            break;

        case 4:

            system.displayPendingRequests();
            break;

        case 5:

            system.displayEvents();
            break;

        case 6:
            return;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}




void travelWithGuide()
{
    cout << "\nAvailable Packages:\n";
    displayPackages(packages);

    int choice;
    cout << "\nEnter the package number you want to book: ";
    cin >> choice;

    if (choice < 1 || choice > packages.size())
    {
        cout << "\nInvalid choice. Please try again.\n";
        return;
    }

    string touristName;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, touristName);

    int numberOfPeople;
    cout << "Enter the number of people for the booking: ";
    cin >> numberOfPeople;

    char vipChoice;
    cout << "Would you like a VIP booking? (y/n): ";
    cin >> vipChoice;

    if (vipChoice == 'y' || vipChoice == 'Y')
        {
        submitBookingRequest(vipPackages[choice - 1], touristName, numberOfPeople, true);
    }
    else
    {
        submitBookingRequest(packages[choice - 1], touristName, numberOfPeople);
    }
}




void travelWithoutGuide()
 {
    // Define categories and corresponding city/state options
    vector<pair<string, vector<string>>> categories =
    {
        {"Hill Stations", {"Manali", "Shimla", "Darjeeling", "Munnar", "Ooty",
                           "Nainital", "Mussoorie", "Mount Abu", "Gangtok",
                           "Coorg", "Gulmarg", "Auli", "Shillong", "Kodaikanal",
                           "Tawang"}},
        {"Beaches", {"Goa", "Pondicherry", "Andaman & Nicobar", "Lakshadweep", "Kanyakumari",
                     "Gokarna", "Puri", "Diu", "Mahabalipuram", "Varkala",
                     "Kovalam", "Alibaug", "Marari", "Tarkarli", "Rameswaram"}},
        {"Historical Places", {"Delhi", "Agra", "Jaipur", "Udaipur", "Khajuraho",
                               "Hampi", "Mysore", "Varanasi", "Lucknow", "Bhopal",
                               "Hyderabad", "Ahmedabad", "Kolkata", "Madurai", "Patna"}},
        {"Adventure Destinations", {"Rishikesh", "Ladakh", "Spiti Valley", "Bir Billing", "Auli",
                                     "Andaman", "Manali", "Sikkim", "Ranthambore", "Jim Corbett",
                                     "Kaziranga", "Kanha", "Bandipur", "Valley of Flowers", "Kanha"}},
        {"Religious Places", {"Varanasi", "Haridwar", "Rishikesh", "Tirupati", "Shirdi",
                              "Amritsar", "Rameswaram", "Dwarka", "Jagannath Puri", "Vaishno Devi",
                              "Somnath", "Kedarnath", "Badrinath", "Mahabalipuram", "Nashik"}}
    };

    cout << "\nYou have chosen to travel without a guide. Let's explore your interests!\n";
    cout << "Choose a category of interest:\n";

    // Display categories
    for (size_t i = 0; i < categories.size(); ++i)
        {
        cout << i + 1 << ". " << categories[i].first << endl;
    }

    int categoryChoice;
    cout << "Enter your choice (1-" << categories.size() << "): ";
    cin >> categoryChoice;

    // Validate category choice
    if (categoryChoice < 1 || categoryChoice > categories.size())
    {
        cout << "\nInvalid choice. Please try again.\n";
        return;
    }



    // Display cities/states in the chosen category
    const vector<string>& cities = categories[categoryChoice - 1].second;
    cout << "\nYou selected: " << categories[categoryChoice - 1].first << endl;
    cout << "Here are 15 cities/states for this category:\n";



    for (size_t i = 0; i < cities.size(); ++i)
    {
        cout << i + 1 << ". " << cities[i] << endl;
    }

    int cityChoice;
    cout << "Enter your choice (1-15): ";
    cin >> cityChoice;



    // Validate city choice
    if (cityChoice < 1 || cityChoice > cities.size())
    {
        cout << "\nInvalid choice. Please try again.\n";
        return;
    }



    // Confirm the selected city/state
    string selectedCity = cities[cityChoice - 1];
    cout << "\nYou have selected " << selectedCity << " for your travel.\n";
    cout << "Enjoy your adventure in " << selectedCity << "!\n";

    chooseTransport(selectedCity);
}



void chooseTransport(const string& destination)
{

    cout << "\nHow would you like to reach " << destination << "?\n";
    cout << "1. Personal Vehicle\n";
    cout << "2. Bus\n";
    cout << "3. Train\n";
    cout << "4. Plane\n";
    cout << "Enter your choice: ";

    int transportChoice;
    cin >> transportChoice;

    string travelDate;
    cout << "\nEnter the date you wish to travel (DD-MM-YYYY): ";
    cin >> travelDate;

    string stayplace;
    cout << "\nEnter your city name: ";
    cin >> stayplace;



    if (transportChoice == 1)
    {
        cout << "\nYou have chosen to travel by Personal Vehicle.\n";
        cout << "Please ensure your vehicle is ready for the journey. Have a safe trip!\n";
        return;
    }



    vector<string> options;
    vector<vector<string>> priceOptions;
    if (transportChoice == 2)
        {
        options =
        {
            "KSRTC Volvo - 09:00 AM",
            "VRL Sleeper - 11:00 AM",
            "SRS Executive - 02:00 PM",
            "RedBus AC - 05:00 PM",
            "Raj National - 08:00 PM",
            "KPN Travels - 10:00 PM",
            "APSTRC Super - 06:00 AM",
            "TNSTC AC - 07:30 AM",
            "Sugama Sleeper - 03:30 AM",
            "Seabird Express - 01:00 PM"
        };

        priceOptions =
        {
            {"AC Single Bed: ₹2050", "AC Double Bed: ₹2460", "Non-AC Single Bed: ₹1640", "Non-AC Double Bed: ₹2050"},
            {"AC Single Bed: ₹1968", "AC Double Bed: ₹2378", "Non-AC Single Bed: ₹1558", "Non-AC Double Bed: ₹1968"},
            {"AC Single Bed: ₹1804", "AC Double Bed: ₹2214", "Non-AC Single Bed: ₹1394", "Non-AC Double Bed: ₹1804"},
            {"AC Single Bed: ₹1640", "AC Double Bed: ₹2050", "Non-AC Single Bed: ₹1230", "Non-AC Double Bed: ₹1640"},
            {"AC Single Bed: ₹1886", "AC Double Bed: ₹2296", "Non-AC Single Bed: ₹1476", "Non-AC Double Bed: ₹1886"},
            {"AC Single Bed: ₹1722", "AC Double Bed: ₹2132", "Non-AC Single Bed: ₹1312", "Non-AC Double Bed: ₹1722"},
            {"AC Single Bed: ₹1804", "AC Double Bed: ₹2214", "Non-AC Single Bed: ₹1394", "Non-AC Double Bed: ₹1804"},
            {"AC Single Bed: ₹1558", "AC Double Bed: ₹1968", "Non-AC Single Bed: ₹1148", "Non-AC Double Bed: ₹1558"},
            {"AC Single Bed: ₹1640", "AC Double Bed: ₹2050", "Non-AC Single Bed: ₹1230", "Non-AC Double Bed: ₹1640"},
            {"AC Single Bed: ₹1886", "AC Double Bed: ₹2296", "Non-AC Single Bed: ₹1476", "Non-AC Double Bed: ₹1886"}
        };
    }
    else if (transportChoice == 3)
        {
        options =
        {
            "Rajdhani Express - 08:00 AM",
            "Shatabdi Express - 09:30 AM",
            "Duronto Express - 01:00 PM",
            "Garib Rath - 04:00 PM",
            "Intercity Express - 07:00 PM",
            "Jan Shatabdi - 09:00 PM",
            "Humsafar Express - 11:00 PM",
            "Vande Bharat - 05:00 AM",
            "Tejas Express - 06:30 AM",
            "Express Superfast - 10:00 AM"
        };
        priceOptions =
        {
            {"Sleeper: ₹1640", "General: ₹1230", "3 Tier AC: ₹2050", "2 Tier AC: ₹2460", "1 Tier AC: ₹3280"},
            {"Sleeper: ₹1476", "General: ₹1066", "3 Tier AC: ₹1804", "2 Tier AC: ₹2296", "1 Tier AC: ₹2870"},
            {"Sleeper: ₹1558", "General: ₹1148", "3 Tier AC: ₹1886", "2 Tier AC: ₹2378", "1 Tier AC: ₹3036"},
            {"Sleeper: ₹984", "General: ₹820", "3 Tier AC: ₹1230", "2 Tier AC: ₹1476", "1 Tier AC: ₹2050"},
            {"Sleeper: ₹820", "General: ₹656", "3 Tier AC: ₹984", "2 Tier AC: ₹1312", "1 Tier AC: ₹1640"},
            {"Sleeper: ₹1804", "General: ₹1394", "3 Tier AC: ₹2214", "2 Tier AC: ₹2706", "1 Tier AC: ₹3526"},
            {"Sleeper: ₹1476", "General: ₹1148", "3 Tier AC: ₹1886", "2 Tier AC: ₹2378", "1 Tier AC: ₹3036"},
            {"Sleeper: ₹1640", "General: ₹1312", "3 Tier AC: ₹2132", "2 Tier AC: ₹2624", "1 Tier AC: ₹3444"},
            {"Sleeper: ₹2050", "General: ₹1640", "3 Tier AC: ₹2460", "2 Tier AC: ₹3116", "1 Tier AC: ₹4100"},
            {"Sleeper: ₹1230", "General: ₹984", "3 Tier AC: ₹1558", "2 Tier AC: ₹1968", "1 Tier AC: ₹2624"}
        };
    }
    else if (transportChoice == 4)
        {
        options =
        {
            "Indigo 6E-101 - 06:00 AM",
            "Air India AI-203 - 08:00 AM",
            "SpiceJet SG-505 - 10:30 AM",
            "GoAir G8-301 - 01:00 PM",
            "Vistara UK-702 - 03:00 PM",
            "Air Asia I5-401 - 05:30 PM",
            "Indigo 6E-702 - 07:00 PM",
            "SpiceJet SG-905 - 09:00 PM",
            "Air India AI-707 - 11:30 PM",
            "GoAir G8-505 - 02:00 AM"
        };
        priceOptions =
        {
            {"Economy: ₹8200", "Business: ₹16400", "First Class: ₹24600"},
            {"Economy: ₹9020", "Business: ₹17220", "First Class: ₹26240"},
            {"Economy: ₹9430", "Business: ₹17630", "First Class: ₹27060"},
            {"Economy: ₹9840", "Business: ₹18040", "First Class: ₹27880"},
            {"Economy: ₹10250", "Business: ₹18450", "First Class: ₹28700"},
            {"Economy: ₹8610", "Business: ₹17220", "First Class: ₹25460"},
            {"Economy: ₹7790", "Business: ₹15990", "First Class: ₹23780"},
            {"Economy: ₹10660", "Business: ₹18860", "First Class: ₹29540"},
            {"Economy: ₹11480", "Business: ₹19680", "First Class: ₹30360"},
            {"Economy: ₹7380", "Business: ₹15580", "First Class: ₹22960"}
        };
    }
    else
        {
        cout << "\nInvalid choice. Returning to main menu.\n";
        return;
    }

    cout << "\nAvailable options on " << travelDate << " from " << stayplace << ":\n";
    for (size_t i = 0; i < options.size(); ++i) {
        cout << i + 1 << ". " << options[i] << endl;

    }

    int optionChoice;
    cout << "Enter your choice: ";
    cin >> optionChoice;

    if (optionChoice < 1 ||options.size())
    {
        cout << "\nInvalid choice. Please select a valid option.\n";
        return;
    }

    cout << "\nAvailable ticket classes for " << options[optionChoice - 1] << ":\n";
    for (size_t i = 0; i < priceOptions[optionChoice - 1].size(); ++i)
    {
        cout << i + 1 << ". " << priceOptions[optionChoice - 1][i] << endl;
    }

    int classChoice;
    cout << "Enter your choice: ";
    cin >> classChoice;

    if (classChoice < 1 || classChoice > priceOptions[optionChoice - 1].size())
    {
        cout << "\nInvalid choice. Please select a valid ticket class.\n";
        return;
    }

    double costPerPerson = stod(priceOptions[optionChoice - 1][classChoice - 1].substr(priceOptions[optionChoice - 1][classChoice - 1].find("₹") + 3));
    int numberOfPeople;
    cout << "Enter the number of people for the booking: ";
    cin >> numberOfPeople;



    double totalCost = costPerPerson * numberOfPeople;
    cout << "\nYou have selected " << options[optionChoice - 1] << " (" << priceOptions[optionChoice - 1][classChoice - 1] << ").\n";
    cout << "Total cost for " << numberOfPeople << " person(s) is ₹" << totalCost << ".\n";



    cout << "\nPlease proceed for the payment-" << "\n";

    // Provide payment mode options to the tourist
    cout << "\nChoose your payment method:\n";
    cout << "1. Credit/Debit Card\n";
    cout << "2. Net Banking\n";
    cout << "3. UPI\n";
    cout << "4. Cash on Arrival\n";
    cout << "Enter your choice: ";
    int paymentChoice;
    cin >> paymentChoice;

    // Handle payment based on the chosen method
    switch (paymentChoice)
    {
        case 1:

            cout << "You chose Credit/Debit Card. Redirecting to payment gateway...\n";
            break;

        case 2:

            cout << "You chose Net Banking. Redirecting to payment gateway...\n";
            break;

        case 3:

            cout << "You chose UPI. Redirecting to payment gateway...\n";
            break;


        case 4:


            cout << "You chose Cash on Arrival.\n";
            break;

        default:

            cout << "\nInvalid choice. Please try again.\n";
            break;

    }
    cout << "\n Your tickets will be booked within a day. Have a great trip!\n";
}

void displayPackages(const vector<Package>& packages)
 {
    for (size_t i = 0; i < packages.size(); ++i)
        {
        cout << i + 1 << ". " << packages[i].agentName << " - " << packages[i].city << endl;
        cout << "   Details: " << packages[i].details << endl;
        cout << "   Price per person: Rs. " << packages[i].price << endl;
    }
}

void submitBookingRequest(const Package& selectedPackage, const string& touristName, int numberOfPeople, bool isVIP)
{
    BookingRequest request(touristName, selectedPackage, isVIP);
    if (isVIP) {
        vipBookingQueue.push(request);
    } else {
        bookingQueue.push(request);
    }
    cout << "\nBooking request for " << touristName << " has been submitted for " << numberOfPeople << " people. Waiting for guide confirmation.\n";
}

void processBookingRequests()
{

    BookingRequest request;
    if (bookingQueue.empty())
    {
        std::cout << "\nNo booking requests to process.\n";
        return;
    }

    while (!bookingQueue.empty())
        {

        BookingRequest request = bookingQueue.front();
        bookingQueue.pop();

        // Check for potentially fraudulent booking
        if (isFraudulentBooking(request))
            {

            std::cout << "\nBooking request from " << request.touristName << " flagged as potentially fraudulent.\n";
            flaggedBookings.insert(request.touristName);
            continue; // Skip processing this booking

        }



        std::cout << "\nProcessing request for " << request.touristName << ":\n";
        std::cout << "   Package: " << request.package.agentName << " - " << request.package.city << "\n";
        std::cout << "   Details: " << request.package.details << "\n";
        std::cout << "   Price per person: Rs. " << request.package.price << "\n";



        if (request.isVIP)
            {

            std::cout << "   Status: VIP Booking\n";

        }
         else
        {
            std::cout << "   Status: Regular Booking\n";
        }

        int numberOfPeople;
        cout << "Enter your contact info (email): ";
        cin >> request.contactInfo;
        std::cout << "Enter the number of people for this booking: ";
        std::cin >> numberOfPeople;

        char confirmation;
        std::cout << "Confirm booking? (y/n): ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y')
            {

            request.confirmed = true; // Mark as confirmed
            confirmedBookings[request.touristName] = request; // Add to confirmed bookings
            std::cout << "\nBooking confirmed for " << request.touristName << ".\n";
            processPayment(request, numberOfPeople); // Pass number of people for total cost calculation

        }
         else
        {

            std::cout << "\nBooking declined for " << request.touristName << ".\n";

        }
    }

     if (systemDown)
        {

        downtimeQueue.push(request);
        std::cout << "System is currently down. Your booking request has been queued.\n";

    }
     else
        {

        bookingQueue.push(request);
        std::cout << "Booking request for " << request.touristName << " has been submitted.\n";

    }
}




void manageVIPBookings()
{

    BookingRequest request;
    if (vipBookingQueue.empty())
        {

        std::cout << "\nNo VIP booking requests to process.\n";
        return;

    }

    while (!vipBookingQueue.empty())
        {

        BookingRequest request = vipBookingQueue.front();
        vipBookingQueue.pop();

        std::cout << "\nProcessing VIP request for " << request.touristName << ":\n";
        std::cout << "   Package: " << request.package.agentName << " - " << request.package.city << "\n";
        std::cout << "   Details: " << request.package.details << "\n";
        std::cout << "   Price per person: Rs. " << request.package.price << "\n";
        std::cout << "   VIP Facilities: " << request.package.details << "\n"; // Update this line if needed

        int numberOfPeople;
        cout << "Enter your contact info (email): ";
        cin >> request.contactInfo;
        std::cout << "Enter the number of people for this VIP booking: ";
        std::cin >> numberOfPeople;

        char confirmation;
        std::cout << "Confirm booking? (y/n): ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y')
            {

            request.confirmed = true; // Mark as confirmed
            confirmedBookings[request.touristName] = request; // Add to confirmed bookings
            std::cout << "\nVIP Booking confirmed for " << request.touristName << ".\n";
            processPayment(request, numberOfPeople); // Pass number of people for total cost calculation

        }
         else
            {

            std::cout << "\nVIP Booking declined for " << request.touristName << ".\n";

        }
    }

     if (systemDown)
        {

        downtimeQueue.push(request);
        std::cout << "System is currently down. Your booking request has been queued.\n";

    }
     else
        {

        bookingQueue.push(request);
        std::cout << "Booking request for " << request.touristName << " has been submitted.\n";

    }
}



void processPayment(const BookingRequest& confirmedRequest, int numberOfPeople)
{

    double totalCost = confirmedRequest.package.price * numberOfPeople;
    cout << "\nProcessing payment for " << confirmedRequest.touristName << "...\n";
    cout << "Payment of Rs. " << totalCost << " for " << numberOfPeople << " people.\n";

    // Sending payment details to tourist
    cout << "\nPayment Confirmation for " << confirmedRequest.touristName << ":" << endl;
    cout << "You have successfully confirmed your booking for " << numberOfPeople << " people." << endl;
    cout << "Booking Details:" << endl;
    cout << "   Package: " << confirmedRequest.package.agentName << " - " << confirmedRequest.package.city << endl;
    cout << "   Details: " << confirmedRequest.package.details << endl;
    cout << "   Total Price: Rs. " << totalCost << endl;

    // Provide payment mode options to the tourist
    cout << "\nChoose your payment method:\n";
    cout << "1. Credit/Debit Card\n";
    cout << "2. Net Banking\n";
    cout << "3. UPI\n";
    cout << "4. Cash on Arrival\n";
    cout << "Enter your choice: ";
    int paymentChoice;
    cin >> paymentChoice;

    // Handle payment based on the chosen method
    switch (paymentChoice)
    {
        case 1:

            cout << "You chose Credit/Debit Card. Redirecting to payment gateway...\n";
            break;

        case 2:

            cout << "You chose Net Banking. Redirecting to payment gateway...\n";
            break;

        case 3:

            cout << "You chose UPI. Redirecting to payment gateway...\n";
            break;

        case 4:

            cout << "You chose Cash on Arrival.\n";
            break;

        default:

            cout << "\nInvalid choice. Please try again.\n";
            break;


    }


    // Option to view the complete schedule
    char viewSchedule;
    cout << "\nWould you like to view the complete schedule of your trip? (y/n): ";
    cin >> viewSchedule;

    if (viewSchedule == 'y' || viewSchedule == 'Y')
        {

        cout << "\nComplete Schedule for Your Trip:\n";
        cout << "Package: " << confirmedRequest.package.agentName << " - " << confirmedRequest.package.city << endl;
        cout << "Details: " << confirmedRequest.package.details << endl;
        cout << "Price per person: Rs. " << confirmedRequest.package.price << endl;
        cout << "Number of People: " << numberOfPeople << endl;
        cout << "Total Cost: Rs. " << totalCost << endl;

        // Additional information (if VIP)
        if (confirmedRequest.isVIP)
            {
            cout << "VIP Facilities: " << vipPackages[0].vipFacilities << endl;
            cout << "VIP Extra Cost: Rs. " << vipPackages[0].vipExtraCost << endl;
        }
    }
    else
    {
        cout << "\nThank you! Enjoy your trip.\n";
    }
}

// Struct to represent an edge
struct Edge1
{

    int src, dest;
    double weight;

};

// Class to represent a graph
class Graph
{

public:
    int V, E; // Number of vertices and edges
    vector<Edge1> edges; // List of edges

    Graph(int V, int E) : V(V), E(E)
    {
        edges.reserve(E);
    }

    void addEdge1(int src, int dest, double weight)
    {
        edges.push_back({src, dest, weight});
    }
};

// Bellman-Ford algorithm to find shortest paths from source
vector<double> bellmanFord(Graph &graph, int src, vector<int> &predecessor)
{
    vector<double> distance(graph.V, numeric_limits<double>::infinity());
    predecessor.resize(graph.V, -1);

    distance[src] = 0.0;

    // Relax all edges |V| - 1 times
    for (int i = 1; i < graph.V; ++i)
        {
        for (auto &edge : graph.edges)
        {
            int u = edge.src, v = edge.dest;
            double weight = edge.weight;
            if (distance[u] != numeric_limits<double>::infinity() && distance[u] + weight < distance[v])
            {
                distance[v] = distance[u] + weight;
                predecessor[v] = u;
            }
        }
    }


    // Check for negative-weight cycles
    for (auto &edge : graph.edges)
    {
        int u = edge.src, v = edge.dest;
        double weight = edge.weight;
        if (distance[u] != numeric_limits<double>::infinity() && distance[u] + weight < distance[v])
        {
            throw runtime_error("Graph contains a negative weight cycle");
        }
    }

    return distance;
}



// Utility function to print the shortest path
void printPath(int vertex, const vector<int> &predecessor, const unordered_map<int, string> &locationMap)
{
    if (vertex == -1)
    {
            return;
    }

    printPath(predecessor[vertex], predecessor, locationMap);
    cout << locationMap.at(vertex) << " ";

}



// Main function for tourist route planning
void touristRoutePlanning()
{

    int numLocations, numRoutes;
    cout << "Enter the number of locations: ";
    cin >> numLocations;
    cout << "Enter the number of routes: ";
    cin >> numRoutes;

    Graph graph(numLocations, numRoutes);
    unordered_map<int, string> locationMap;



    // Input locations
    cout << "Enter location names:" << endl;
    for (int i = 0; i < numLocations; ++i)
    {
        string location;
        cout << "Location " << i + 1 << ": ";
        cin >> ws; // Clear leading whitespace
        getline(cin, location);
        locationMap[i] = location;
    }

    // Input routes
    cout << "Enter routes in the format (source destination weight):" << endl;
    for (int i = 0; i < numRoutes; ++i)
    {

        string srcName, destName;
        double weight;
        cout << "Route " << i + 1 << ": ";
        cin >> ws;
        getline(cin, srcName, ' ');
        getline(cin, destName, ' ');
        cin >> weight;

        int src = -1, dest = -1;
        for (const auto &entry : locationMap)
        {

            if (entry.second == srcName) src = entry.first;
            if (entry.second == destName) dest = entry.first;

        }

        if (src == -1 || dest == -1)
        {
            cout << "Invalid locations. Try again." << endl;
            --i;
            continue;
        }

        graph.addEdge1(src, dest, weight);
    }

    // Source location
    string sourceLocation;
    cout << "Enter the starting location: ";
    cin >> ws;
    getline(cin, sourceLocation);

    int src = -1;
    for (const auto &entry : locationMap)
    {

        if (entry.second == sourceLocation) src = entry.first;

    }

    if (src == -1)
    {

        cout << "Invalid starting location." << endl;
        return;

    }

    // Run Bellman-Ford algorithm
    try
    {
        vector<int> predecessor;
        vector<double> distances = bellmanFord(graph, src, predecessor);

        // Output results
        cout << "\nOptimized Routes from " << sourceLocation << ":\n";
        cout << left << setw(20) << "Destination" << setw(15) << "Distance" << "Path" << endl;
        for (int i = 0; i < numLocations; ++i)
        {

            if (i == src) continue;

            cout << left << setw(20) << locationMap[i];
            if (distances[i] == numeric_limits<double>::infinity())
            {
                cout << setw(15) << "Unreachable" << "N/A" << endl;
            }
            else
            {
                cout << setw(15) << distances[i];
                printPath(i, predecessor, locationMap);
                cout << endl;
            }

        }

    }

    catch (const runtime_error &e)
    {
        cout << e.what() << endl;
    }
}

// Function to calculate the total cost of a given route
int calculateRouteCost(const vector<vector<int>>& distanceMatrix, const vector<int>& route) {
    int totalCost = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalCost += distanceMatrix[route[i]][route[i + 1]];
    }
    // Add cost to return to the starting city
    totalCost += distanceMatrix[route.back()][route[0]];
    return totalCost;
}




// Function to solve TSP using brute force
void solveTSP(const vector<string>& cities, const vector<vector<int>>& distanceMatrix)
{

    int n = cities.size();
    vector<int> route(n);
    for (int i = 0; i < n; ++i)
    {
        route[i] = i;
    }

    int minCost = numeric_limits<int>::max();
    vector<int> bestRoute;

    do
        {

        int currentCost = calculateRouteCost(distanceMatrix, route);
        if (currentCost < minCost)
         {

            minCost = currentCost;
            bestRoute = route;

        }

    }

     while (next_permutation(route.begin() + 1, route.end())); // Fix the first city to reduce redundant calculations

    // Output the best route and its cost
    cout << "Minimum Cost: " << minCost << endl;
    cout << "Best Route: ";
    for (int cityIndex : bestRoute)
    {
        cout << cities[cityIndex] << " -> ";
    }

    cout << cities[bestRoute[0]] << endl; // Return to the starting city

}




void routeplan()
{

    int n;
    cout << "Enter the number of cities: ";
    cin >> n;

    vector<string> cities(n);
    cout << "Enter the names of the cities(First city entered must be the source city):\n";
    for (int i = 0; i < n; ++i)
    {
        cin >> cities[i];
    }

    unordered_map<string, unordered_map<string, int>> distances;
    cout << "Enter the distances between the cities in the format (City1 City2 Distance). Enter 'done' to finish:\n";

    while (true)
    {
        string city1, city2;
        int distance;
        cin >> city1;

        if (city1 == "done")
        {
            break;
        }

        cin >> city2 >> distance;
        distances[city1][city2] = distance;
        distances[city2][city1] = distance; // Assuming symmetric distances

    }

    vector<vector<int>> distanceMatrix(n, vector<int>(n, numeric_limits<int>::max()));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i == j)
             {
                distanceMatrix[i][j] = 0;
            }
          else
            {
                distanceMatrix[i][j] = distances[cities[i]][cities[j]];
            }
        }
    }

    solveTSP(cities, distanceMatrix);
}

// Struct to represent a review
struct Review
{

    string username;
    string reviewText;
    int rating; // Scale 1 to 5
};



// Function to add a review
void addReview(vector<Review> &reviews)
 {
    Review newReview;

    cout << "Enter your username: ";
    cin >> newReview.username;

    cout << "Enter your review: ";
    cin.ignore();
     // Clear the input buffer
    getline(cin, newReview.reviewText);


    cout << "Enter your rating (1-5): ";

    cin >> newReview.rating;

    if (newReview.rating < 1 || newReview.rating > 5)
        {
        cout << "Invalid rating. Please enter a value between 1 and 5." << endl;
        return;
    }


    reviews.push_back(newReview);
    cout << "Thank you for your review!" << endl;
}




// Function to display all reviews
void displayReviews(const vector<Review> &reviews)
 {
    if (reviews.empty())
        {
        cout << "No reviews available yet." << endl;
        return;
    }


    cout << "\nReviews:" << endl;
    for (const auto &review : reviews)
        {
        cout << "Username: " << review.username << endl;

        cout << "Rating: " << review.rating << "/5" << endl;

        cout << "Review: " << review.reviewText << endl;

        cout << "-----------------------------------" << endl;

    }
}



void ratings()
 {
    vector<Review> reviews; // Vector to store reviews

    char choice;
    do
        {
        cout << "\n1. Leave a Review\n2. View Reviews\n3. Exit\nChoose an option: ";
        cin >> choice;

        switch (choice)
        {
            case '1':

                addReview(reviews);

                break;

            case '2':

                displayReviews(reviews);

                break;

            case '3':

                cout << "Exiting..." << endl;

                break;

            default:

                cout << "Invalid choice. Please try again." << endl;

        }

    } while (choice != '3');

}


















// Accommodation class definition
class Accommodation
{
public:
    string name;
    string type;
    double price;
    string location;
    vector<string> amenities;

    Accommodation(string n, string t, double p, string l, vector<string> a)
        : name(n), type(t), price(p), location(l), amenities(a) {}


    // Display accommodation information
    void displayInfo() const {
        cout << "Name: " << name << "\nType: " << type << "\nPrice: ₹" << price << " per night\nLocation: " << location << "\nAmenities: ";
        for (const auto& amenity : amenities)
            {
            cout << amenity << ", ";
        }
        cout << "\n";
    }

};




// Booking class definition
class Booking
{
public:
    Accommodation accommodation;
    string user;
    bool confirmed;

    Booking(const Accommodation& acc, const string& user)
        : accommodation(acc), user(user), confirmed(false) {}

    // Confirm booking
    void confirmBooking()
    {
        confirmed = true;
        cout << "Booking confirmed for " << user << "!\n";
    }


    // Display booking information
    void displayBooking() const
     {
        cout << "User: " << user << "\n";
        accommodation.displayInfo();
        cout << "Status: " << (confirmed ? "Confirmed" : "Not Confirmed") << "\n";
    }
};



// User class definition
class User
{
public:
    string name;
    string password;
    vector<Booking> bookings;

    User(string n, string p) : name(n), password(p) {}

    void addBooking(const Booking& booking)
    {
        bookings.push_back(booking);
    }

    void displayDetails() const
    {
        cout << "Name: " << name << "\n";
    }

    void displayBookings() const
    {
        cout << "\nBooking History:\n";
        for (const auto& booking : bookings)
            {
            booking.displayBooking();
            cout << "\n";
        }
    }

};





// AccommodationPlanner class definition
class AccommodationPlanner
 {
private:
    map<string, vector<Accommodation>> cityAccommodations;
    vector<User> users;
    User* currentUser;
    vector<Accommodation> recommendations;
    string selectedCity;
    string userType;
    double userBudget;
    string userLocation;
    vector<string> userAmenities;

    // Add sample accommodations to cities
    void addSampleAccommodations()
    {
        cityAccommodations["Mumbai"].emplace_back("Hotel Deluxe", "Hotel", 10000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included", "Gym"});
        cityAccommodations["Mumbai"].emplace_back("Beachside Resort", "Resort", 15000, "Beachfront", vector<string>{"Pool", "Spa", "All-Inclusive"});
        cityAccommodations["Mumbai"].emplace_back("Budget Stay", "Hotel", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Mumbai"].emplace_back("Luxury Palace", "Hotel", 25000, "City Center", vector<string>{"Free Wi-Fi", "Pool", "Spa"});
        cityAccommodations["Mumbai"].emplace_back("Beachfront Condo", "Apartment", 12000, "Beachfront", vector<string>{"Free Wi-Fi", "Kitchen", "Laundry"});
        cityAccommodations["Delhi"].emplace_back("Cozy Hostel", "Hostel", 2000, "Downtown", vector<string>{"Free Wi-Fi", "Shared Kitchen"});
        cityAccommodations["Delhi"].emplace_back("Urban Apartment", "Apartment", 8000, "City Center", vector<string>{"Free Wi-Fi", "Kitchen", "Laundry"});
        cityAccommodations["Delhi"].emplace_back("Historical Inn", "Hotel", 12000, "Historical District", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Delhi"].emplace_back("Luxury Suite", "Resort", 35000, "Suburbs", vector<string>{"Pool", "Spa", "All-Inclusive"});
        cityAccommodations["Delhi"].emplace_back("Budget Lodge", "Hostel", 1500, "Outskirts", vector<string>{"Free Wi-Fi", "Shared Kitchen"});
        cityAccommodations["Bangalore"].emplace_back("Mountain Cabin", "Cottage", 12000, "Countryside", vector<string>{"Fireplace", "Hiking Trails"});
        cityAccommodations["Bangalore"].emplace_back("Techie Hub Hotel", "Hotel", 6000, "Tech Park", vector<string>{"Free Wi-Fi", "Breakfast Included", "Gym"});
        cityAccommodations["Bangalore"].emplace_back("Countryside Retreat", "Cottage", 17000, "Countryside", vector<string>{"Fireplace", "Garden", "Hiking Trails"});
        cityAccommodations["Bangalore"].emplace_back("Luxury Villa", "Resort", 23000, "Outskirts", vector<string>{"Pool", "Spa", "Gym"});
        cityAccommodations["Bangalore"].emplace_back("Downtown Studio", "Apartment", 7000, "Downtown", vector<string>{"Free Wi-Fi", "Kitchen"});



        // Adding new cities with sample accommodations
        cityAccommodations["Manali"].emplace_back("Snow Peak Retreat", "Resort", 8000, "Mountain", vector<string>{"Free Wi-Fi", "Spa", "Breakfast Included"});
        cityAccommodations["Manali"].emplace_back("Himalayan Cottage", "Cottage", 5000, "Countryside", vector<string>{"Fireplace", "Garden"});
        cityAccommodations["Manali"].emplace_back("Mountain View Hotel", "Hotel", 7000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Manali"].emplace_back("Riverfront Inn", "Inn", 6000, "Riverside", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Manali"].emplace_back("Adventure Lodge", "Lodge", 4000, "Countryside", vector<string>{"Free Wi-Fi", "Hiking Trails"});




        cityAccommodations["Shimla"].emplace_back("Hilltop Resort", "Resort", 9000, "Mountain", vector<string>{"Free Wi-Fi", "Spa", "Breakfast Included"});
        cityAccommodations["Shimla"].emplace_back("Colonial Heritage Hotel", "Hotel", 12000, "Historical District", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Shimla"].emplace_back("Pinewood Cottage", "Cottage", 6000, "Forest", vector<string>{"Fireplace", "Garden"});
        cityAccommodations["Shimla"].emplace_back("City Center Lodge", "Lodge", 5000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Shimla"].emplace_back("Ridge View Inn", "Inn", 7000, "Ridge", vector<string>{"Free Wi-Fi", "Breakfast Included"});




        cityAccommodations["Ooty"].emplace_back("Tea Garden Resort", "Resort", 10000, "Countryside", vector<string>{"Free Wi-Fi", "Spa", "Breakfast Included"});
        cityAccommodations["Ooty"].emplace_back("Hill View Hotel", "Hotel", 8000, "Mountain", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Ooty"].emplace_back("Lakeside Cottage", "Cottage", 7000, "Lakeside", vector<string>{"Fireplace", "Garden"});
        cityAccommodations["Ooty"].emplace_back("Botanical Garden Inn", "Inn", 6000, "Garden", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Ooty"].emplace_back("Downtown Lodge", "Lodge", 5000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});




        cityAccommodations["Goa"].emplace_back("Beachfront Resort", "Resort", 15000, "Beachfront", vector<string>{"Free Wi-Fi", "Pool", "Spa"});
        cityAccommodations["Goa"].emplace_back("Seaside Hotel", "Hotel", 10000, "Seaside", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Goa"].emplace_back("Island Cottage", "Cottage", 12000, "Island", vector<string>{"Free Wi-Fi", "Garden"});
        cityAccommodations["Goa"].emplace_back("Riverside Inn", "Inn", 8000, "Riverside", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Goa"].emplace_back("Adventure Lodge", "Lodge", 6000, "Countryside", vector<string>{"Free Wi-Fi", "Hiking Trails"});





        cityAccommodations["Gokarna"].emplace_back("Beachside Resort", "Resort", 12000, "Beachfront", vector<string>{"Free Wi-Fi", "Spa", "Breakfast Included"});
        cityAccommodations["Gokarna"].emplace_back("Cliffside Hotel", "Hotel", 10000, "Cliff", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Gokarna"].emplace_back("Jungle Cottage", "Cottage", 8000, "Jungle", vector<string>{"Fireplace", "Garden"});
        cityAccommodations["Gokarna"].emplace_back("Seaview Inn", "Inn", 7000, "Seaside", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Gokarna"].emplace_back("Countryside Lodge", "Lodge", 6000, "Countryside", vector<string>{"Free Wi-Fi", "Hiking Trails"});




        cityAccommodations["Agra"].emplace_back("Taj View Hotel", "Hotel", 15000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Agra"].emplace_back("Mughal Heritage Resort", "Resort", 18000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Agra"].emplace_back("Royal Inn", "Inn", 8000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Agra"].emplace_back("Budget Lodge", "Lodge", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Agra"].emplace_back("Heritage Cottage", "Cottage", 12000, "Historical District", vector<string>{"Fireplace", "Garden"});







        cityAccommodations["Jaipur"].emplace_back("Palace Hotel", "Hotel", 20000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Jaipur"].emplace_back("Royal Heritage Resort", "Resort", 25000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Jaipur"].emplace_back("Pink City Inn", "Inn", 10000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Jaipur"].emplace_back("Budget Lodge", "Lodge", 6000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Jaipur"].emplace_back("Haveli Cottage", "Cottage", 15000, "Historical District", vector<string>{"Fireplace", "Garden"});




        cityAccommodations["Udaipur"].emplace_back("Lake Palace Hotel", "Hotel", 30000, "Lakeside", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Udaipur"].emplace_back("Heritage Resort", "Resort", 25000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Udaipur"].emplace_back("City Center Inn", "Inn", 12000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Udaipur"].emplace_back("Budget Lodge", "Lodge", 8000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Udaipur"].emplace_back("Heritage Cottage", "Cottage", 20000, "Historical District", vector<string>{"Fireplace", "Garden"});




        cityAccommodations["Hampi"].emplace_back("Ruins View Hotel", "Hotel", 8000, "Historical District", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Hampi"].emplace_back("Heritage Resort", "Resort", 12000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Hampi"].emplace_back("Temple View Inn", "Inn", 6000, "Historical District", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Hampi"].emplace_back("Budget Lodge", "Lodge", 4000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Hampi"].emplace_back("Historical Cottage", "Cottage", 10000, "Historical District", vector<string>{"Fireplace", "Garden"});





        cityAccommodations["Mysore"].emplace_back("Palace View Hotel", "Hotel", 10000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Mysore"].emplace_back("Royal Heritage Resort", "Resort", 15000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Mysore"].emplace_back("City Center Inn", "Inn", 7000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Mysore"].emplace_back("Budget Lodge", "Lodge", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Mysore"].emplace_back("Heritage Cottage", "Cottage", 12000, "Historical District", vector<string>{"Fireplace", "Garden"});





        cityAccommodations["Hyderabad"].emplace_back("Charminar View Hotel", "Hotel", 12000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Hyderabad"].emplace_back("Heritage Resort", "Resort", 15000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Hyderabad"].emplace_back("City Center Inn", "Inn", 9000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Hyderabad"].emplace_back("Budget Lodge", "Lodge", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Hyderabad"].emplace_back("Heritage Cottage", "Cottage", 13000, "Historical District", vector<string>{"Fireplace", "Garden"});




        cityAccommodations["Kolkata"].emplace_back("Victoria View Hotel", "Hotel", 10000, "City Center", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Kolkata"].emplace_back("Colonial Heritage Resort", "Resort", 15000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Kolkata"].emplace_back("City Center Inn", "Inn", 8000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Kolkata"].emplace_back("Budget Lodge", "Lodge", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Kolkata"].emplace_back("Heritage Cottage", "Cottage", 12000, "Historical District", vector<string>{"Fireplace", "Garden"});








        cityAccommodations["Ladakh"].emplace_back("Mountain View Hotel", "Hotel", 15000, "Mountain", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Ladakh"].emplace_back("Heritage Resort", "Resort", 18000, "Historical District", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Ladakh"].emplace_back("City Center Inn", "Inn", 10000, "City Center", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Ladakh"].emplace_back("Budget Lodge", "Lodge", 6000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Ladakh"].emplace_back("Heritage Cottage", "Cottage", 20000, "Historical District", vector<string>{"Fireplace", "Garden"});




        cityAccommodations["Bandipur"].emplace_back("Jungle Resort", "Resort", 15000, "Jungle", vector<string>{"Free Wi-Fi", "Spa"});
        cityAccommodations["Bandipur"].emplace_back("Forest View Hotel", "Hotel", 10000, "Forest", vector<string>{"Free Wi-Fi", "Gym"});
        cityAccommodations["Bandipur"].emplace_back("Wildlife Inn", "Inn", 9000, "Wildlife Sanctuary", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Bandipur"].emplace_back("Budget Lodge", "Lodge", 5000, "Downtown", vector<string>{"Free Wi-Fi", "Breakfast Included"});
        cityAccommodations["Bandipur"].emplace_back("Nature Cottage", "Cottage", 12000, "Nature Reserve", vector<string>{"Fireplace", "Garden"});
    }


    // Register new user
    void registerUser()
    {
        string name, password;
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your password: ";
        getline(cin, password);
        users.emplace_back(name, password);
        currentUser = &users.back();
        cout << "Registration successful! Welcome, " << name << ".\n";
    }


    // Log in existing user
    void loginUser()
    {
        string name, password;
        cout << "Enter your name: ";
        getline(cin,name);
        cout << "Enter your password: ";
        getline(cin, password);

        for (auto& user : users)
            {
            if (user.name == name && user.password == password) {
                currentUser = &user;
                cout << "Login successful! Welcome back, " << name << ".\n";
                return;
            }
        }
        cout << "Invalid credentials. Please try again.\n";
        loginUser();

    }


    // Gather user preferences and recommend accommodations
    void gatherPreferencesAndRecommend()
    {
        selectCity(); // Select city
        gatherUserPreferences(); // Gather user preferences

        recommendAccommodations(); // Recommend accommodations based on preferences

    }

    // Gather user preferences
    void gatherUserPreferences()
    {
        cout << "What type of accommodation do you prefer (Hotel, Resort, Hostel, Cottage, Apartment)? ";
        getline(cin, userType);
        cout << "What is your budget per night? ";
        cin >> userBudget;
        cin.ignore();
        cout << "Do you have a location preference (City Center, Beachfront, Downtown, Countryside)? ";
        getline(cin, userLocation);
        cout << "Enter preferred amenities (comma-separated, e.g., Free Wi-Fi, Pool): ";
        string amenities;
        getline(cin, amenities);



        size_t pos = 0;
        string delimiter = ", ";
        while ((pos = amenities.find(delimiter)) != string::npos)
            {
            userAmenities.push_back(amenities.substr(0, pos));
            amenities.erase(0, pos + delimiter.length());
        }
        userAmenities.push_back(amenities);
    }


    // Recommend accommodations based on user preferences
    void recommendAccommodations()
    {
        recommendations.clear(); // Clear previous recommendations
        const auto& cityAcc = cityAccommodations[selectedCity];
        for (const auto& acc : cityAcc) {
            bool matchesType = toLowerCase(acc.type) == toLowerCase(userType);
            bool matchesBudget = acc.price <= userBudget;
            bool matchesLocation = toLowerCase(acc.location) == toLowerCase(userLocation);
            bool matchesAmenities = any_of(userAmenities.begin(), userAmenities.end(), [&](const string& amenity) {
                return any_of(acc.amenities.begin(), acc.amenities.end(), [&](const string& a) {
                    return toLowerCase(a) == toLowerCase(amenity);
                });

            });



            if (matchesType || matchesBudget || matchesLocation || matchesAmenities)
                {
                recommendations.push_back(acc);
            }
        }

        if (recommendations.empty())
            {
            cout << "No recommendations available based on your preferences. Please try again.\n";
        }
        else
            {
            cout << "Recommendations have been updated.\n";
        }
    }



    // Display recommended accommodations
    void displayRecommendations() const

    {
        if (recommendations.empty())
            {
            cout << "No recommendations available. Please gather preferences and recommend accommodations first.\n";
        }
        else
            {
            cout << "\nRecommended Accommodations in " << selectedCity << ":\n";
            for (size_t i = 0; i < recommendations.size(); ++i)
            {
                cout << "\nOption " << i + 1 << ":\n";
                recommendations[i].displayInfo();
            }
        }

    }


    // Display all accommodations
    void displayAllAccommodations() const
    {
        cout << "\nAll Available Accommodations:\n";
        for (const auto& city : cityAccommodations) {
            cout << "\nCity: " << city.first << "\n";
            for (const auto& acc : city.second) {
                acc.displayInfo();
                cout << "\n";
            }

        }

    }


    // Select city
    void selectCity()
    {
        cout << "Available Cities:\n";
        for (const auto& city : cityAccommodations) {
            cout << "- " << city.first << "\n";
        }


        cout << "Please enter the city you want to visit: ";
        getline(cin, selectedCity);
        selectedCity = toLowerCase(selectedCity); // Convert user input to lowercase


        // Check if the selected city exists in the map (case-insensitive)
        bool cityFound = false;
        for (const auto& city : cityAccommodations)
            {
            if (toLowerCase(city.first) == selectedCity)
            {
                selectedCity = city.first; // Use the original case of the city name
                cityFound = true;
                break;
            }
        }



        if (!cityFound)
            {
            cout << "City not found. Please try again.\n";
            selectCity();
        }
        else
            {
            cout << "Selected city: " << selectedCity << "\n";
        }
    }


    // Book accommodation
    void bookAccommodation(int selection)
    {
        const auto& selectedAcc = recommendations[selection - 1];
        cout << "\nYou have chosen to book:\n";
        selectedAcc.displayInfo();
        Booking newBooking(selectedAcc, currentUser->name);
        string confirmation;
        cout << "Would you like to proceed with the booking (yes/no)? ";
        cin >> confirmation;
        cin.ignore();
        if (confirmation == "yes")
            {
            newBooking.confirmBooking();
            currentUser->addBooking(newBooking);
            processPayment(selectedAcc.price);
        }
        else
            {
            cout << "Booking canceled.\n";
        }
    }



    // Process payment
    void processPayment(double amount)
     {
        string cardNumber, expiryDate, cvv;
        cout << "Processing payment of ₹" << amount << "\n";
        cout << "Enter card number: ";
        getline(cin, cardNumber);
        cout << "Enter expiry date (MM/YY): ";
        getline(cin, expiryDate);
        cout << "Enter CVV: ";
        getline(cin, cvv);
        cout << "Payment successful!\n";
    }


    // View user details
    void viewUserDetails()
     {
        currentUser->displayDetails();
    }


    // View booking history
    void viewBookingHistory()
     {
        currentUser->displayBookings();
    }


    // Provide feedback
    void provideFeedback()
    {
        cout << "Please enter your feedback on your accommodation experience: ";
        string feedback;
        getline(cin, feedback);
        cout << "Thank you for your feedback!\n";
    }


    // Display main menu
    void showMenu() const
    {
        cout << "\nAccommodation Planner Menu:\n";
        cout << "1. Register\n";
        cout << "2. Log In\n";
        cout << "3. Display All Accommodations\n";
        cout << "4. Gather Preferences and Recommend Accommodations\n";
        cout << "5. Display Recommendations\n";
        cout << "6. Book Accommodation\n";
        cout << "7. View User Details\n";
        cout << "8. View Booking History\n";
        cout << "9. Provide Feedback\n";
        cout << "10. Exit\n";
        cout << "Please choose an option: ";
    }



public:
    // Constructor
    AccommodationPlanner() : currentUser(nullptr)
    {
        addSampleAccommodations();
    }

    // Run the planner
    void run()
     {
        int choice;
        do
            {
            showMenu();
            cin >> choice;
            cin.ignore();  // Ignore newline character after choice input

            switch (choice)
            {
                case 1:

                    registerUser();

                    break;

                case 2:

                    loginUser();

                    break;

                case 3:

                    displayAllAccommodations();

                    break;

                case 4:

                    if (currentUser != nullptr)
                        {
                        gatherPreferencesAndRecommend();
                    }
                    else
                        {
                        cout << "Please log in first.\n";
                    }
                    break;

                case 5:

                    if (currentUser != nullptr && !recommendations.empty())
                        {
                        displayRecommendations();
                    }
                    else
                        {
                        cout << "No recommendations available. Please log in, gather preferences, and recommend accommodations first.\n";
                    }
                    break;

                case 6:
                    if (currentUser != nullptr && !recommendations.empty())
                        {
                        int selection;
                        cout << "Select the option number you would like to book: ";
                        cin >> selection;
                        cin.ignore();  // Ignore newline character after selection input
                        bookAccommodation(selection);
                    }
                    else
                        {
                        cout << "No recommendations available. Please log in, gather preferences, and recommend accommodations first.\n";
                    }
                    break;

                case 7:
                    if (currentUser != nullptr)
                        {
                        viewUserDetails();
                    }
                    else
                        {
                        cout << "Please log in first.\n";
                    }
                    break;

                case 8:
                    if (currentUser != nullptr)
                        {
                        viewBookingHistory();
                    }
                    else
                        {
                        cout << "Please log in first.\n";
                    }
                    break;

                case 9:
                    if (currentUser != nullptr)
                        {
                        provideFeedback();
                    }
                    else
                        {
                        cout << "Please log in first.\n";
                    }
                    break;

                case 10:

                    cout << "Exiting the Accommodation Planner. Goodbye!\n";
                    break;

                default:

                    cout << "Invalid choice. Please choose a valid option.\n";
                    break;
            }
        } while (choice != 10);

    }
};




// Main function
int accomodation()
{
    AccommodationPlanner planner;
    planner.run();
    return 0;
}

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <climits>
#include <map>
#include <cctype>
#include <functional> // Include this header for std::function

using namespace std;


struct Edge
{
    int destination;
    int distance;
    int traffic;
    int adjustedWeight;
    int capacity;
    string roadType;
    int vehiclesOnRoad;
};


struct Node
{
    int city;
    int distance;

    bool operator>(const Node& other) const
    {
        return distance > other.distance;
    }
};



string toLower(const string& str)
{
    string lowerStr = str;
    std::transform(str.begin(), str.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}


void addEdge(vector<vector<Edge>>& adjList, int u, int v, int distance, int traffic, const string& roadType, int capacity, bool directed = false)
{
    int adjustedWeight = distance * (1 + traffic / 10.0);
    adjList[u].push_back({v, distance, traffic, adjustedWeight, capacity, roadType});
    if (!directed)
    {
        adjList[v].push_back({u, distance, traffic, adjustedWeight, capacity, roadType});
    }
}


void updateTraffic(vector<vector<Edge>>& adjList, int city1, int city2, int newTraffic)
{
    for (Edge& edge : adjList[city1])
    {
        if (edge.destination == city2)
        {
            edge.traffic = newTraffic;
            edge.adjustedWeight = edge.distance * (1 + newTraffic / 10.0);
        }
    }
    for (Edge& edge : adjList[city2])
    {
        if (edge.destination == city1)
        {
            edge.traffic = newTraffic;
            edge.adjustedWeight = edge.distance * (1 + newTraffic / 10.0);
        }
    }
}



void removeRoad(vector<vector<Edge>>& adjList, int city1, int city2)
{
    adjList[city1].erase(remove_if(adjList[city1].begin(), adjList[city1].end(),
                                   [city2](const Edge& edge) { return edge.destination == city2; }),
                         adjList[city1].end());

    adjList[city2].erase(remove_if(adjList[city2].begin(), adjList[city2].end(),
                                   [city1](const Edge& edge) { return edge.destination == city1; }),
                         adjList[city2].end());
}



vector<int> dijkstra(const vector<vector<Edge>>& adjList, int start, int end, int& totalDistance)
{
    int n = adjList.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[start] = 0;
    pq.push({start, 0});

    while (!pq.empty())
    {
        int u = pq.top().city;
        int d = pq.top().distance;
        pq.pop();

        if (d > dist[u])
            continue;

        for (const Edge& edge : adjList[u])
        {
            int v = edge.destination;
            int newDist = d + edge.adjustedWeight;

            if (newDist < dist[v])
            {
                dist[v] = newDist;
                prev[v] = u;
                pq.push({v, newDist});
            }
        }
    }


    vector<int> path;
    for (int at = end; at != -1; at = prev[at])
    {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] != start)
        return {};

    totalDistance = (path.size() > 1) ? dist[end] : -1;
    return path;
}



void displayGraph(const vector<vector<Edge>>& adjList, const unordered_map<int, string>& indexToCity)
{
    cout << "Graph of cities and their connections:\n";
    for (size_t i = 0; i < adjList.size(); ++i)
    {
        cout << indexToCity.at(i) << " -> ";
        for (const Edge& edge : adjList[i])
        {
            cout << indexToCity.at(edge.destination) << " (" << edge.distance << " km, " << edge.traffic << "% traffic, " << edge.capacity << " vehicles, " << edge.roadType << ") ";
        }
        cout << endl;
    }
}



// Function to add local roads within a city
void addLocalRoads(vector<vector<Edge>>& adjList, int cityIndex, const vector<pair<int, int>>& localRoads)
{
    for (const auto& road : localRoads)
    {
        // Adding local roads with short distances, low capacity, and potentially lower traffic
        addEdge(adjList, cityIndex, road.first, road.second, 10, "Local Road", 200);
    }
}



// Structure to represent edges in the graph
struct GraphEdge
{
    int to, weight;
};


// Graph represented as an adjacency list
class Graph1
 {
    int cities;
    vector<vector<GraphEdge>> adjList;

public:
    Graph1(int n) : cities(n), adjList(n) {}

    void addRoad(int u, int v, int weight)
    {
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight}); // Assuming undirected graph
    }


    // Dijkstra's algorithm to find the shortest path
    vector<int> dijkstra(int start, int end)
     {
        vector<int> dist(cities, INT_MAX);
        vector<int> prev(cities, -1);
        dist[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, start});

        while (!pq.empty())
            {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto edge : adjList[u])
                {
                int v = edge.to;
                int weight = edge.weight;
                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }


        vector<int> path;
        for (int at = end; at != -1; at = prev[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (path.size() == 1 && path[0] != start)
            return {};

        return path;
    }


    // Get backup route by excluding a specific road
    vector<int> backupRoute(int start, int end, int excludeU, int excludeV)
    {
        vector<int> dist(cities, INT_MAX);
        vector<int> prev(cities, -1);
        dist[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, start});

        while (!pq.empty())
            {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto edge : adjList[u])
                {
                int v = edge.to;
                int weight = edge.weight;

                // Exclude the specific road
                if ((u == excludeU && v == excludeV) || (u == excludeV && v == excludeU))
                    {
                    continue;
                }

                if (dist[u] + weight < dist[v])
                    {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }


        vector<int> path;
        for (int at = end; at != -1; at = prev[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (path.size() == 1 && path[0] != start)
            return {};

        return path;
    }



    // Adjust weights for night-time optimization
    void adjustForNightTime(vector<pair<int, int>> poorlyLitRoads) {
        for (auto road : poorlyLitRoads) {
            int u = road.first;
            int v = road.second;

            // Increase the weight of poorly lit roads
            for (auto& edge : adjList[u]) {
                if (edge.to == v) edge.weight *= 2;
            }
            for (auto& edge : adjList[v]) {
                if (edge.to == u) edge.weight *= 2;
            }
        }
    }
};




// Penalty tracking: {Violation Name, {Count, Total Penalty}}
unordered_map<string, pair<int, int>> violations;

// Penalty amounts
unordered_map<string, int> penaltyAmount = {
    {"Speeding", 100},
    {"Signal Jumping", 200},
    {"Illegal Parking", 50},
    {"Driving Without License", 300},
    {"Overloading Vehicle", 150}
};



// Structure to represent local places
struct LocalPlace
{
    string name;
    int distance;  // Distance in km to reach from the city center
    int traffic;   // Traffic percentage
    string roadType;  // Type of road (e.g., "Local Road", "City Road")
};


void displayTrafficInfo(const vector<vector<Edge>>& adjList, const unordered_map<int, string>& indexToCity)
{
    cout << "Traffic Information:\n";
    for (size_t i = 0; i < adjList.size(); ++i)
    {
        for (const Edge& edge : adjList[i])
        {
            if (i < edge.destination) // Avoid printing both directions
            {
                cout << indexToCity.at(i) << " -> " << indexToCity.at(edge.destination)
                     << " (" << edge.distance << " km, " << edge.traffic << "% traffic, " << edge.capacity << " vehicles, " << edge.roadType << ")\n";
            }
        }
    }
}



void displayPenalties()
{
    cout << "Current Penalties for Violations:\n";
    for (const auto& violation : violations)
    {
        cout << violation.first << ": Count = " << violation.second.first << ", Total Penalty = $" << violation.second.second << "\n";
    }
}



void updateRoadCapacity(vector<vector<Edge>>& adjList, int city1, int city2, int newCapacity)
{
    for (Edge& edge : adjList[city1])
    {
        if (edge.destination == city2)
        {
            edge.capacity = newCapacity;
        }
    }
    for (Edge& edge : adjList[city2])
    {
        if (edge.destination == city1)
        {
            edge.capacity = newCapacity;
        }
    }
}



vector<vector<int>> findAllPaths(const vector<vector<Edge>>& adjList, int start, int end)
{
    vector<bool> visited(adjList.size(), false); // To keep track of visited nodes
    vector<int> path; // To store the current path
    vector<vector<int>> allPaths; // To store all the paths

    // Function to perform DFS and find all paths
    function<void(int)> findAllPathsUtil = [&](int u)
    {
        visited[u] = true; // Mark the current node as visited
        path.push_back(u); // Add the current node to the path

        // If the current node is the destination, add the path to allPaths
        if (u == end)
        {
            allPaths.push_back(path);
        }
        else
        {
            // Recur for all the vertices adjacent to the current vertex
            for (const auto& edge : adjList[u])
            {
                if (!visited[edge.destination])
                {
                    findAllPathsUtil(edge.destination);
                }
            }
        }

        path.pop_back(); // Remove the current node from the path
        visited[u] = false; // Mark the current node as unvisited
    };


    findAllPathsUtil(start); // Start the DFS from the start node
    return allPaths; // Return all the paths found
}



// Function to find the longest path between two cities
vector<int> findLongestPath(const vector<vector<Edge>>& adjList, int start, int end, int& totalDistance)
{
    vector<vector<int>> allPaths = findAllPaths(adjList, start, end);
    vector<int> longestPath;
    totalDistance = 0;

    for (const auto& path : allPaths)
    {
        int distance = 0;
        for (size_t i = 0; i < path.size() - 1; ++i)
        {
            for (const Edge& edge : adjList[path[i]])
            {
                if (edge.destination == path[i + 1])
                {
                    distance += edge.distance;
                    break;
                }
            }
        }
        if (distance > totalDistance)
        {
            totalDistance = distance;
            longestPath = path;
        }
    }

    return longestPath;
}




// Function to display all cities
void displayAllCities(const map<string, int>& cityToIndex)
{
    cout << "List of all cities:\n";
    for (const auto& city : cityToIndex)
    {
        cout << city.first << endl;
    }
}




// Function to display all roads
void displayAllRoads(const vector<vector<Edge>>& adjList, const unordered_map<int, string>& indexToCity)
{
    cout << "List of all roads:\n";
    for (size_t i = 0; i < adjList.size(); ++i)
    {
        for (const Edge& edge : adjList[i])
        {
            if (i < edge.destination) // Avoid printing both directions
            {
                cout << indexToCity.at(i) << " -> " << indexToCity.at(edge.destination)
                     << " (" << edge.distance << " km, " << edge.traffic << "% traffic, " << edge.capacity << " vehicles, " << edge.roadType << ")\n";
            }
        }
    }
}



// Function to calculate the total number of roads
int calculateTotalRoads(const vector<vector<Edge>>& adjList)
{
    int totalRoads = 0;
    for (const auto& edges : adjList)
    {
        totalRoads += edges.size();
    }
    // Since each road is counted twice in an undirected graph
    return totalRoads / 2;
}



int touristconnect()
{
    int numCities = 20;
    map<string, int> cityToIndex = {
        {"delhi", 0}, {"mumbai", 1}, {"kolkata", 2}, {"bangalore", 3}, {"chennai", 4},
        {"hyderabad", 5}, {"ahmedabad", 6}, {"pune", 7}, {"jaipur", 8}, {"lucknow", 9},
        {"surat", 10}, {"kanpur", 11}, {"nagpur", 12}, {"indore", 13}, {"thane", 14},
        {"patna", 15}, {"vadodara", 16}, {"agra", 17}, {"nashik", 18}, {"allahabad", 19}
    };

    unordered_map<int, string> indexToCity = {
        {0, "delhi"}, {1, "mumbai"}, {2, "kolkata"}, {3, "bangalore"}, {4, "chennai"},
        {5, "hyderabad"}, {6, "ahmedabad"}, {7, "pune"}, {8, "jaipur"}, {9, "lucknow"},
        {10, "surat"}, {11, "kanpur"}, {12, "nagpur"}, {13, "indore"}, {14, "thane"},
        {15, "patna"}, {16, "vadodara"}, {17, "agra"}, {18, "nashik"}, {19, "allahabad"}
    };

    vector<vector<Edge>> adjList(numCities);

    vector<int> hospitals;

    vector<vector<string>> hospitalsInCity = {
        {"Hospital AIIMS", "Hospital Fortis"}, {"Hospital Hinduja", "Hospital Lilavati"},
        {"Hospital AMRI", "Hospital CMRI"}, {"Hospital Manipal", "Hospital Sakra"},
        {"Hospital Apollo", "Hospital Fortis"}, {"Hospital Care", "Hospital Continental"},
        {"Hospital Sterling", "Hospital Shalby"}, {"Hospital Ruby Hall", "Hospital Sahyadri"},
        {"Hospital Fortis", "Hospital Narayana"}, {"Hospital RML", "Hospital Medanta"},
        {"Hospital SMIMER", "Hospital New Civil"}, {"Hospital Regency", "Hospital Metro"},
        {"Hospital Orange City", "Hospital Care"}, {"Hospital Bombay", "Hospital Tata"},
        {"Hospital Jupiter", "Hospital Holy Cross"}, {"Hospital Patna", "Hospital Satyam"},
        {"Hospital CIMS", "Hospital Sterling"}, {"Hospital Shreeji", "Hospital Shree"},
        {"Hospital Nashik", "Hospital Indira Gandhi"}, {"Hospital Shree Ganesh", "Hospital Asopa"}
    };




    // Adding the roads between the cities
    addEdge(adjList, cityToIndex["delhi"], cityToIndex["mumbai"], 10, 30, "Highway", 5000);
    addEdge(adjList, cityToIndex["mumbai"], cityToIndex["kolkata"], 20, 40, "Ghat", 2000);
    addEdge(adjList, cityToIndex["kolkata"], cityToIndex["bangalore"], 25, 20, "City Road", 3000);
    addEdge(adjList, cityToIndex["bangalore"], cityToIndex["chennai"], 15, 50, "School Zone", 1500);
    addEdge(adjList, cityToIndex["delhi"], cityToIndex["chennai"], 40, 10, "Highway", 6000);
    // Add more edges for other cities as needed


    // Sample local roads for each city (city index, distance in km)
    vector<pair<int, int>> localRoadsDelhi = {{1, 5}, {2, 10}};
    vector<pair<int, int>> localRoadsMumbai = {{0, 5}, {2, 15}};
    vector<pair<int, int>> localRoadsKolkata = {{1, 5}, {3, 20}};
    vector<pair<int, int>> localRoadsBangalore = {{2, 15}, {4, 10}};
    vector<pair<int, int>> localRoadsChennai = {{3, 10}, {0, 30}};
    vector<pair<int, int>> localRoadsHyderabad = {{0, 20}, {1, 10}};
    vector<pair<int, int>> localRoadsAhmedabad = {{1, 25}, {2, 15}};
    vector<pair<int, int>> localRoadsPune = {{2, 5}, {3, 10}};
    vector<pair<int, int>> localRoadsJaipur = {{3, 20}, {4, 15}};
    vector<pair<int, int>> localRoadsLucknow = {{4, 25}, {5, 30}};
    vector<pair<int, int>> localRoadsSurat = {{5, 10}, {6, 20}};
    vector<pair<int, int>> localRoadsKanpur = {{6, 5}, {7, 15}};
    vector<pair<int, int>> localRoadsNagpur = {{7, 10}, {8, 30}};
    vector<pair<int, int>> localRoadsIndore = {{8, 5}, {9, 20}};
    vector<pair<int, int>> localRoadsThane = {{9, 10}, {10, 15}};
    vector<pair<int, int>> localRoadsPatna = {{10, 20}, {11, 25}};
    vector<pair<int, int>> localRoadsVadodara = {{11, 5}, {12, 10}};
    vector<pair<int, int>> localRoadsAgra = {{12, 15}, {13, 25}};
    vector<pair<int, int>> localRoadsNashik = {{13, 10}, {14, 20}};
    vector<pair<int, int>> localRoadsAllahabad = {{14, 15}, {15, 20}};



    // Add the local roads to each city
    addLocalRoads(adjList, 0, localRoadsDelhi);
    addLocalRoads(adjList, 1, localRoadsMumbai);
    addLocalRoads(adjList, 2, localRoadsKolkata);
    addLocalRoads(adjList, 3, localRoadsBangalore);
    addLocalRoads(adjList, 4, localRoadsChennai);
    addLocalRoads(adjList, 5, localRoadsHyderabad);
    addLocalRoads(adjList, 6, localRoadsAhmedabad);
    addLocalRoads(adjList, 7, localRoadsPune);
    addLocalRoads(adjList, 8, localRoadsJaipur);
    addLocalRoads(adjList, 9, localRoadsLucknow);
    addLocalRoads(adjList, 10, localRoadsSurat);
    addLocalRoads(adjList, 11, localRoadsKanpur);
    addLocalRoads(adjList, 12, localRoadsNagpur);
    addLocalRoads(adjList, 13, localRoadsIndore);
    addLocalRoads(adjList, 14, localRoadsThane);
    addLocalRoads(adjList, 15, localRoadsPatna);
    addLocalRoads(adjList, 16, localRoadsVadodara);
    addLocalRoads(adjList, 17, localRoadsAgra);
    addLocalRoads(adjList, 18, localRoadsNashik);
    addLocalRoads(adjList, 19, localRoadsAllahabad);


    // Additional Graph for primary route, backup route and night-time optimization
    Graph1 g(numCities);


    // Add roads to the Graph g for those functionalities
    g.addRoad(cityToIndex["delhi"], cityToIndex["mumbai"], 10);
    g.addRoad(cityToIndex["mumbai"], cityToIndex["kolkata"], 20);
    g.addRoad(cityToIndex["kolkata"], cityToIndex["bangalore"], 25);
    g.addRoad(cityToIndex["bangalore"], cityToIndex["chennai"], 15);
    g.addRoad(cityToIndex["delhi"], cityToIndex["chennai"], 40);
    // Add more roads as needed



    // Local places for each city
    vector<vector<LocalPlace>> localPlaces = {
        {  // City A (Delhi) local places
            {"India Gate", 3, 20, "City Road"},
            {"Red Fort", 5, 25, "National Highway"},
            {"Lotus Temple", 4, 30, "City Road"},
            {"Qutub Minar", 7, 35, "City Road"},
            {"Connaught Place", 6, 15, "Local Road"},
            {"Akshardham Temple", 5, 20, "City Road"},
            {"Jama Masjid", 3, 10, "Local Road"},
            {"National Museum", 4, 25, "City Road"},
            {"Delhi Zoo", 8, 40, "City Road"},
            {"Lodhi Garden", 2, 10, "City Road"}
        },

        {  // City B (Mumbai) local places
            {"Gateway of India", 3, 20, "City Road"},
            {"Marine Drive", 6, 30, "Local Road"},
            {"Juhu Beach", 5, 25, "City Road"},
            {"Haji Ali Dargah", 7, 35, "City Road"},
            {"Chhatrapati Shivaji Terminus", 8, 40, "City Road"},
            {"Sanjay Gandhi National Park", 10, 50, "Highway"},
            {"Elephanta Caves", 12, 60, "Ferry Route"},
            {"Crawford Market", 4, 15, "Local Road"},
            {"Colaba Causeway", 5, 20, "City Road"},
            {"Worli Sea Link", 7, 30, "City Road"}
        },

        {  // City C (Bangalore) local places
            {"Lalbagh Botanical Garden", 4, 15, "City Road"},
            {"Cubbon Park", 5, 20, "City Road"},
            {"Bangalore Palace", 6, 30, "City Road"},
            {"Bannerghatta National Park", 10, 50, "Highway"},
            {"ISKCON Temple", 7, 35, "City Road"},
            {"Commercial Street", 4, 15, "Local Road"},
            {"MG Road", 3, 10, "City Road"},
            {"Vidhana Soudha", 5, 25, "City Road"},
            {"UB City Mall", 8, 40, "City Road"},
            {"Tipu Sultan's Summer Palace", 6, 25, "City Road"}
        },

        {  // City D (Chennai) local places
            {"Marina Beach", 6, 20, "City Road"},
            {"Kapaleeshwarar Temple", 5, 25, "City Road"},
            {"Fort St. George", 4, 30, "Local Road"},
            {"Vivekananda House", 3, 15, "City Road"},
            {"Guindy National Park", 8, 35, "City Road"},
            {"Elliot's Beach", 7, 30, "City Road"},
            {"Valluvar Kottam", 5, 20, "City Road"},
            {"T Nagar Market", 3, 10, "City Road"},
            {"DakshinaChitra", 9, 45, "City Road"},
            {"Chennai Central Station", 4, 15, "City Road"}
        },

        {  // City E (Kolkata) local places
            {"Victoria Memorial", 5, 20, "City Road"},
            {"Howrah Bridge", 6, 25, "City Road"},
            {"Indian Museum", 4, 15, "City Road"},
            {"Eco Park", 8, 40, "City Road"},
            {"Birla Planetarium", 3, 10, "City Road"},
            {"Dakshineswar Temple", 10, 50, "Highway"},
            {"Science City", 7, 30, "City Road"},
            {"Eden Gardens", 5, 25, "City Road"},
            {"Mother Teresa's House", 3, 10, "Local Road"},
            {"Alipore Zoo", 8, 35, "City Road"}
        }
    };


    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Display graph\n";


        cout << "2. Find shortest path\n";
        cout << "3. Record and manage rule violations\n";


        cout << "4. Handle emergency (find nearest hospital)\n";
        cout << "5. Find road type between two cities\n";
        cout << "6. Check if road is full and find alternative\n";
        cout << "7. Travel within the city\n";
        cout << "8. Travel between two cities\n";
        cout << "9. Find Primary Route\n";
        cout << "10. Find Backup Route\n";
        cout << "11. Night-Time Optimization\n";
        cout << "12. Display traffic information\n";
        cout << "13. Display penalties\n";

        cout << "14. Find all paths between two cities\n";

        cout << "15. Display local places in a city\n";
        cout << "16. Find longest path between two cities\n";
        cout << "17. Display all cities\n";
        cout << "18. Display all roads\n";
        cout << "19. Calculate total number of roads\n";
        cout << "20. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:

                displayGraph(adjList, indexToCity);
                break;



            case 2:
            {
                string startCity, endCity;
                int totalDistance;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;

                startCity = toLower(startCity);
                endCity = toLower(endCity);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<int> path = dijkstra(adjList, cityToIndex[startCity], cityToIndex[endCity], totalDistance);
                if (path.empty())
                {
                    cout << "No path found between the cities.\n";
                }
                else
                {
                    cout << "Shortest path from " << startCity << " to " << endCity << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << "\nTotal distance: " << totalDistance << " km\n";
                }

                break;
            }


            case 3:
            {
                cout << "Violation Types:\n";
                cout << "1. Speeding (Penalty: $100)\n";
                cout << "2. Signal Jumping (Penalty: $200)\n";
                cout << "3. Illegal Parking (Penalty: $50)\n";
                cout << "4. Driving Without License (Penalty: $300)\n";
                cout << "5. Overloading Vehicle (Penalty: $150)\n";
                cout << "Enter violation type (1-5): ";
                int violationType;
                cin >> violationType;

                string violationName;
                switch (violationType)
                {
                    case 1: violationName = "Speeding"; break;
                    case 2: violationName = "Signal Jumping"; break;
                    case 3: violationName = "Illegal Parking"; break;
                    case 4: violationName = "Driving Without License"; break;
                    case 5: violationName = "Overloading Vehicle"; break;
                    default:
                        cout << "Invalid violation type. Please try again.\n";
                        continue;
                }


                // Update violations
                violations[violationName].first += 1;
                violations[violationName].second += penaltyAmount[violationName];

                cout << "Violation recorded: " << violationName
                     << " | Total Penalty: $" << violations[violationName].second << "\n";

                // Ask if user wants to pay the penalty
                cout << "Do you want to pay the penalty now? (y/n): ";
                char payPenalty;
                cin >> payPenalty;

                if (payPenalty == 'y' || payPenalty == 'Y')
                {
                    violations[violationName].second = 0; // Clear penalty
                    cout << "Penalty cleared for " << violationName << ".\n";
                }
                else
                {
                    cout << "Penalty not paid. Additional fines may apply for future violations.\n";
                }

                break;
            }



            case 4:
            {
                string startCity;
                cout << "Enter the city to handle emergency (find nearest hospital): ";
                cin >> startCity;

                startCity = toLower(startCity);

                if (cityToIndex.find(startCity) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                cout << "Nearest hospitals in " << startCity << ": ";
                for (const string& hospital : hospitalsInCity[cityToIndex[startCity]])
                {
                    cout << hospital << " ";
                }
                cout << endl;

                break;
            }

            case 5:
            {
                string city1, city2;
                cout << "Enter two cities to find the road type between them: ";
                cin >> city1 >> city2;
                city1 = toLower(city1);
                city2 = toLower(city2);

                bool found = false;
                for (const Edge& edge : adjList[cityToIndex[city1]])
                {
                    if (edge.destination == cityToIndex[city2])
                    {
                        cout << "Road type between " << city1 << " and " << city2 << ": " << edge.roadType << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "No road found between the cities.\n";
                }

                break;
            }


            case 6:
            {
                string city1, city2;
                cout << "Enter city1 and city2 to check road capacity: ";
                cin >> city1 >> city2;
                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                // Check for the road capacity and provide an alternative if full
                bool found = false;
                for (const Edge& edge : adjList[cityToIndex[city1]])
                {
                    if (edge.destination == cityToIndex[city2])
                    {
                        if (edge.vehiclesOnRoad >= edge.capacity)
                        {
                            cout << "Road is full. Finding alternative roads...\n";
                            // Find alternative roads

                            bool alternativeFound = false;
                            for (const Edge& altEdge : adjList[cityToIndex[city1]])
                            {
                                if (altEdge.capacity > altEdge.vehiclesOnRoad)
                                {
                                    cout << "Alternative road to " << indexToCity[altEdge.destination] << " found with capacity "
                                         << altEdge.capacity << " vehicles\n";
                                    alternativeFound = true;
                                }
                            }
                            if (!alternativeFound)
                            {
                                cout << "No alternatives available.\n";
                            }
                        }
                        else
                        {
                            cout << "Road capacity is available.\n";
                        }
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "No road found between the cities.\n";
                }

                break;
            }


            case 7:
            {
                // Travel within the city
                string city;
                cout << "Enter the city to travel within: ";
                cin >> city;
                city = toLower(city);

                if (cityToIndex.find(city) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                int cityIndex = cityToIndex[city];
                cout << "Traveling within " << city << " is available.\n";
                cout << "Local places in " << city << ":\n";

                for (const LocalPlace& place : localPlaces[cityIndex])
                {
                    cout << "Place: " << place.name
                         << " | Distance: " << place.distance << " km"
                         << " | Traffic: " << place.traffic << "% traffic"
                         << " | Road Type: " << place.roadType << endl;
                }

                break;
            }


            case 8:
            {
                // Travel between two cities
                string city1, city2;
                int totalDistance;
                cout << "Enter start city: ";
                cin >> city1;
                cout << "Enter end city: ";
                cin >> city2;

                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<int> path = dijkstra(adjList, cityToIndex[city1], cityToIndex[city2], totalDistance);

                if (path.empty())
                {
                    cout << "No path found between the cities.\n";
                }
                else
                {
                    cout << "Shortest path from " << city1 << " to " << city2 << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << "\nTotal distance: " << totalDistance << " km\n";
                }

                break;
            }


            case 9:
            {
                // Find Primary Route
                string startCity, endCity;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;

                startCity = toLower(startCity);
                endCity = toLower(endCity);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<int> path = g.dijkstra(cityToIndex[startCity], cityToIndex[endCity]);
                if (path.empty())
                {
                    cout << "No path found between the cities.\n";
                }
                else
                {
                    cout << "Primary route from " << startCity << " to " << endCity << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << endl;
                }
                break;
            }


            case 10:
            {
                // Find Backup Route
                string startCity, endCity, excludeCity1, excludeCity2;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;
                cout << "Enter road to exclude (two cities): ";
                cin >> excludeCity1 >> excludeCity2;

                startCity = toLower(startCity);
                endCity = toLower(endCity);
                excludeCity1 = toLower(excludeCity1);
                excludeCity2 = toLower(excludeCity2);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end() ||
                    cityToIndex.find(excludeCity1) == cityToIndex.end() || cityToIndex.find(excludeCity2) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<int> path = g.backupRoute(cityToIndex[startCity], cityToIndex[endCity], cityToIndex[excludeCity1], cityToIndex[excludeCity2]);
                if (path.empty())
                {
                    cout << "No backup route found between the cities.\n";
                }
                else
                {
                    cout << "Backup route from " << startCity << " to " << endCity << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << endl;
                }
                break;
            }


            case 11:
            {
                // Night-Time Optimization
                int numPoorlyLit;
                cout << "Enter number of poorly lit roads: ";
                cin >> numPoorlyLit;

                vector<pair<int, int>> poorlyLitRoads;
                cout << "Enter the poorly lit roads (two city indices each):" << endl;
                for (int i = 0; i < numPoorlyLit; i++)
                {
                    string uCity, vCity;
                    cin >> uCity >> vCity;

                    uCity = toLower(uCity);
                    vCity = toLower(vCity);

                    if (cityToIndex.find(uCity) == cityToIndex.end() || cityToIndex.find(vCity) == cityToIndex.end())
                    {
                        cout << "Invalid city indices. Please try again.\n";
                        --i;
                        continue;
                    }

                    poorlyLitRoads.push_back({cityToIndex[uCity], cityToIndex[vCity]});
                }


                g.adjustForNightTime(poorlyLitRoads);
                cout << "Night-time optimization completed.\n";

                string startCity, endCity;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;


                startCity = toLower(startCity);
                endCity = toLower(endCity);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }


                vector<int> path = g.dijkstra(cityToIndex[startCity], cityToIndex[endCity]);
                if (path.empty())
                {
                    cout << "No path found between the cities.\n";
                }
                else
                {
                    cout << "Night-time optimized route from " << startCity << " to " << endCity << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << endl;
                }
                break;
            }


            case 12:
            {
                // Display traffic information
                displayTrafficInfo(adjList, indexToCity);
                break;
            }


            case 13:
            {
                // Display penalties
                displayPenalties();
                break;
            }



            case 14:

            {
                // Find all paths between two cities
                string startCity, endCity;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;

                startCity = toLower(startCity);
                endCity = toLower(endCity);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<vector<int>> allPaths = findAllPaths(adjList, cityToIndex[startCity], cityToIndex[endCity]);
                if (allPaths.empty())
                {
                    cout << "No paths found between the cities.\n";
                }
                else
                {
                    cout << "All paths from " << startCity << " to " << endCity << ":\n";
                    for (const auto& path : allPaths)
                    {
                        for (int city : path)
                        {
                            cout << indexToCity[city] << " ";
                        }
                        cout << endl;
                    }
                }
                break;
            }


            case 15:
            {
                // Display local places in a city
                string city;
                cout << "Enter the city: ";
                cin >> city;
                city = toLower(city);

                if (cityToIndex.find(city) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                int cityIndex = cityToIndex[city];
                cout << "Local places in " << city << ":\n";
                for (const LocalPlace& place : localPlaces[cityIndex])
                {
                    cout << "Place: " << place.name
                         << " | Distance: " << place.distance << " km"
                         << " | Traffic: " << place.traffic << "% traffic"
                         << " | Road Type: " << place.roadType << endl;
                }
                break;
            }


            case 16:
            {
                // Find longest path between two cities
                string startCity, endCity;
                int totalDistance;
                cout << "Enter start city: ";
                cin >> startCity;
                cout << "Enter end city: ";
                cin >> endCity;

                startCity = toLower(startCity);
                endCity = toLower(endCity);

                if (cityToIndex.find(startCity) == cityToIndex.end() || cityToIndex.find(endCity) == cityToIndex.end())
                {
                    cout << "Invalid city names. Please try again.\n";
                    break;
                }

                vector<int> path = findLongestPath(adjList, cityToIndex[startCity], cityToIndex[endCity], totalDistance);
                if (path.empty())
                {
                    cout << "No path found between the cities.\n";
                }
                else
                {
                    cout << "Longest path from " << startCity << " to " << endCity << ": ";
                    for (int city : path)
                    {
                        cout << indexToCity[city] << " ";
                    }
                    cout << "\nTotal distance: " << totalDistance << " km\n";
                }
                break;
            }


            case 17:
            {
                // Display all cities
                displayAllCities(cityToIndex);
                break;
            }


            case 18:
            {
                // Display all roads
                displayAllRoads(adjList, indexToCity);
                break;
            }


            case 19:
            {
                // Calculate total number of roads
                int totalRoads = calculateTotalRoads(adjList);
                cout << "Total number of roads: " << totalRoads << endl;
                break;
            }


            case 20:
                return 0;


            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }
    }
    return 0;
}



int adminconnect()
{
    int numCities = 20;

    map<string, int> cityToIndex = {
        {"delhi", 0}, {"mumbai", 1}, {"kolkata", 2}, {"bangalore", 3}, {"chennai", 4},
        {"hyderabad", 5}, {"ahmedabad", 6}, {"pune", 7}, {"jaipur", 8}, {"lucknow", 9},
        {"surat", 10}, {"kanpur", 11}, {"nagpur", 12}, {"indore", 13}, {"thane", 14},
        {"patna", 15}, {"vadodara", 16}, {"agra", 17}, {"nashik", 18}, {"allahabad", 19}
    };

    unordered_map<int, string> indexToCity = {
        {0, "delhi"}, {1, "mumbai"}, {2, "kolkata"}, {3, "bangalore"}, {4, "chennai"},
        {5, "hyderabad"}, {6, "ahmedabad"}, {7, "pune"}, {8, "jaipur"}, {9, "lucknow"},
        {10, "surat"}, {11, "kanpur"}, {12, "nagpur"}, {13, "indore"}, {14, "thane"},
        {15, "patna"}, {16, "vadodara"}, {17, "agra"}, {18, "nashik"}, {19, "allahabad"}
    };

    vector<vector<Edge>> adjList(numCities);


    vector<int> hospitals;

    vector<vector<string>> hospitalsInCity = {
        {"Hospital AIIMS", "Hospital Fortis"}, {"Hospital Hinduja", "Hospital Lilavati"},
        {"Hospital AMRI", "Hospital CMRI"}, {"Hospital Manipal", "Hospital Sakra"},
        {"Hospital Apollo", "Hospital Fortis"}, {"Hospital Care", "Hospital Continental"},
        {"Hospital Sterling", "Hospital Shalby"}, {"Hospital Ruby Hall", "Hospital Sahyadri"},
        {"Hospital Fortis", "Hospital Narayana"}, {"Hospital RML", "Hospital Medanta"},
        {"Hospital SMIMER", "Hospital New Civil"}, {"Hospital Regency", "Hospital Metro"},
        {"Hospital Orange City", "Hospital Care"}, {"Hospital Bombay", "Hospital Tata"},
        {"Hospital Jupiter", "Hospital Holy Cross"}, {"Hospital Patna", "Hospital Satyam"},
        {"Hospital CIMS", "Hospital Sterling"}, {"Hospital Shreeji", "Hospital Shree"},
        {"Hospital Nashik", "Hospital Indira Gandhi"}, {"Hospital Shree Ganesh", "Hospital Asopa"}
    };


    // Adding the roads between the cities
    addEdge(adjList, cityToIndex["delhi"], cityToIndex["mumbai"], 10, 30, "Highway", 5000);
    addEdge(adjList, cityToIndex["mumbai"], cityToIndex["kolkata"], 20, 40, "Ghat", 2000);
    addEdge(adjList, cityToIndex["kolkata"], cityToIndex["bangalore"], 25, 20, "City Road", 3000);
    addEdge(adjList, cityToIndex["bangalore"], cityToIndex["chennai"], 15, 50, "School Zone", 1500);
    addEdge(adjList, cityToIndex["delhi"], cityToIndex["chennai"], 40, 10, "Highway", 6000);
    // Add more edges for other cities as needed


    // Sample local roads for each city (city index, distance in km)
    vector<pair<int, int>> localRoadsDelhi = {{1, 5}, {2, 10}};
    vector<pair<int, int>> localRoadsMumbai = {{0, 5}, {2, 15}};
    vector<pair<int, int>> localRoadsKolkata = {{1, 5}, {3, 20}};
    vector<pair<int, int>> localRoadsBangalore = {{2, 15}, {4, 10}};
    vector<pair<int, int>> localRoadsChennai = {{3, 10}, {0, 30}};
    vector<pair<int, int>> localRoadsHyderabad = {{0, 20}, {1, 10}};
    vector<pair<int, int>> localRoadsAhmedabad = {{1, 25}, {2, 15}};
    vector<pair<int, int>> localRoadsPune = {{2, 5}, {3, 10}};
    vector<pair<int, int>> localRoadsJaipur = {{3, 20}, {4, 15}};
    vector<pair<int, int>> localRoadsLucknow = {{4, 25}, {5, 30}};
    vector<pair<int, int>> localRoadsSurat = {{5, 10}, {6, 20}};
    vector<pair<int, int>> localRoadsKanpur = {{6, 5}, {7, 15}};
    vector<pair<int, int>> localRoadsNagpur = {{7, 10}, {8, 30}};
    vector<pair<int, int>> localRoadsIndore = {{8, 5}, {9, 20}};
    vector<pair<int, int>> localRoadsThane = {{9, 10}, {10, 15}};
    vector<pair<int, int>> localRoadsPatna = {{10, 20}, {11, 25}};
    vector<pair<int, int>> localRoadsVadodara = {{11, 5}, {12, 10}};
    vector<pair<int, int>> localRoadsAgra = {{12, 15}, {13, 25}};
    vector<pair<int, int>> localRoadsNashik = {{13, 10}, {14, 20}};
    vector<pair<int, int>> localRoadsAllahabad = {{14, 15}, {15, 20}};


    // Add the local roads to each city
    addLocalRoads(adjList, 0, localRoadsDelhi);
    addLocalRoads(adjList, 1, localRoadsMumbai);
    addLocalRoads(adjList, 2, localRoadsKolkata);
    addLocalRoads(adjList, 3, localRoadsBangalore);
    addLocalRoads(adjList, 4, localRoadsChennai);
    addLocalRoads(adjList, 5, localRoadsHyderabad);
    addLocalRoads(adjList, 6, localRoadsAhmedabad);
    addLocalRoads(adjList, 7, localRoadsPune);
    addLocalRoads(adjList, 8, localRoadsJaipur);
    addLocalRoads(adjList, 9, localRoadsLucknow);
    addLocalRoads(adjList, 10, localRoadsSurat);
    addLocalRoads(adjList, 11, localRoadsKanpur);
    addLocalRoads(adjList, 12, localRoadsNagpur);
    addLocalRoads(adjList, 13, localRoadsIndore);
    addLocalRoads(adjList, 14, localRoadsThane);
    addLocalRoads(adjList, 15, localRoadsPatna);
    addLocalRoads(adjList, 16, localRoadsVadodara);
    addLocalRoads(adjList, 17, localRoadsAgra);
    addLocalRoads(adjList, 18, localRoadsNashik);
    addLocalRoads(adjList, 19, localRoadsAllahabad);


    // Additional Graph for primary route, backup route and night-time optimization
    Graph1 g(numCities);


    // Add roads to the Graph g for those functionalities
    g.addRoad(cityToIndex["delhi"], cityToIndex["mumbai"], 10);
    g.addRoad(cityToIndex["mumbai"], cityToIndex["kolkata"], 20);
    g.addRoad(cityToIndex["kolkata"], cityToIndex["bangalore"], 25);
    g.addRoad(cityToIndex["bangalore"], cityToIndex["chennai"], 15);
    g.addRoad(cityToIndex["delhi"], cityToIndex["chennai"], 40);
    // Add more roads as needed


    // Local places for each city
    vector<vector<LocalPlace>> localPlaces = {
        {  // City A (Delhi) local places
            {"India Gate", 3, 20, "City Road"},
            {"Red Fort", 5, 25, "National Highway"},
            {"Lotus Temple", 4, 30, "City Road"},
            {"Qutub Minar", 7, 35, "City Road"},
            {"Connaught Place", 6, 15, "Local Road"},
            {"Akshardham Temple", 5, 20, "City Road"},
            {"Jama Masjid", 3, 10, "Local Road"},
            {"National Museum", 4, 25, "City Road"},
            {"Delhi Zoo", 8, 40, "City Road"},
            {"Lodhi Garden", 2, 10, "City Road"}
        },

        {  // City B (Mumbai) local places
            {"Gateway of India", 3, 20, "City Road"},
            {"Marine Drive", 6, 30, "Local Road"},
            {"Juhu Beach", 5, 25, "City Road"},
            {"Haji Ali Dargah", 7, 35, "City Road"},
            {"Chhatrapati Shivaji Terminus", 8, 40, "City Road"},
            {"Sanjay Gandhi National Park", 10, 50, "Highway"},
            {"Elephanta Caves", 12, 60, "Ferry Route"},
            {"Crawford Market", 4, 15, "Local Road"},
            {"Colaba Causeway", 5, 20, "City Road"},
            {"Worli Sea Link", 7, 30, "City Road"}
        },

        {  // City C (Bangalore) local places
            {"Lalbagh Botanical Garden", 4, 15, "City Road"},
            {"Cubbon Park", 5, 20, "City Road"},
            {"Bangalore Palace", 6, 30, "City Road"},
            {"Bannerghatta National Park", 10, 50, "Highway"},
            {"ISKCON Temple", 7, 35, "City Road"},
            {"Commercial Street", 4, 15, "Local Road"},
            {"MG Road", 3, 10, "City Road"},
            {"Vidhana Soudha", 5, 25, "City Road"},
            {"UB City Mall", 8, 40, "City Road"},
            {"Tipu Sultan's Summer Palace", 6, 25, "City Road"}
        },

        {  // City D (Chennai) local places
            {"Marina Beach", 6, 20, "City Road"},
            {"Kapaleeshwarar Temple", 5, 25, "City Road"},
            {"Fort St. George", 4, 30, "Local Road"},
            {"Vivekananda House", 3, 15, "City Road"},
            {"Guindy National Park", 8, 35, "City Road"},
            {"Elliot's Beach", 7, 30, "City Road"},
            {"Valluvar Kottam", 5, 20, "City Road"},
            {"T Nagar Market", 3, 10, "City Road"},
            {"DakshinaChitra", 9, 45, "City Road"},
            {"Chennai Central Station", 4, 15, "City Road"}
        },

        {  // City E (Kolkata) local places
            {"Victoria Memorial", 5, 20, "City Road"},
            {"Howrah Bridge", 6, 25, "City Road"},
            {"Indian Museum", 4, 15, "City Road"},
            {"Eco Park", 8, 40, "City Road"},
            {"Birla Planetarium", 3, 10, "City Road"},
            {"Dakshineswar Temple", 10, 50, "Highway"},
            {"Science City", 7, 30, "City Road"},
            {"Eden Gardens", 5, 25, "City Road"},
            {"Mother Teresa's House", 3, 10, "Local Road"},
            {"Alipore Zoo", 8, 35, "City Road"}
        }
    };


    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Display graph\n";
        cout << "2. Update traffic\n";
        cout << "3. Remove road\n";


        cout << "4. Add new city\n";
        cout << "5. Add new connection\n";


        cout << "6. Display traffic information\n";

        cout << "7. Update road capacity\n";

        cout << "8. Add new local place\n";
        cout << "9. Display local places in a city\n";

        cout << "10. Display all cities\n";
        cout << "11. Display all roads\n";
        cout << "12. Calculate total number of roads\n";
        cout << "13. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:

                displayGraph(adjList, indexToCity);
                break;

            case 2:
            {
                string city1, city2;
                int newTraffic;
                cout << "Enter the two cities to update traffic (city1 city2): ";
                cin >> city1 >> city2;
                cout << "Enter new traffic percentage: ";
                cin >> newTraffic;
                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "One or both cities not found.\n";
                    break;
                }

                updateTraffic(adjList, cityToIndex[city1], cityToIndex[city2], newTraffic);
                cout << "Traffic updated successfully.\n";
                break;
            }

            case 3:
            {
                string city1, city2;
                cout << "Enter the two cities to remove road (city1 city2): ";
                cin >> city1 >> city2;
                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "One or both cities not found.\n";
                    break;
                }

                removeRoad(adjList, cityToIndex[city1], cityToIndex[city2]);
                cout << "Road removed successfully.\n";
                break;
            }




            case 4:
            {
                string newCity;
                cout << "Enter the new city name: ";
                cin >> newCity;
                newCity = toLower(newCity);

                if (cityToIndex.find(newCity) != cityToIndex.end())
                {
                    cout << "City already exists.\n";
                    break;
                }

                int newIndex = cityToIndex.size();
                cityToIndex[newCity] = newIndex;
                indexToCity[newIndex] = newCity;
                adjList.push_back({});

                cout << "City added successfully.\n";
                break;
            }


            case 5:
            {
                string city1, city2, roadType;
                int distance, traffic, capacity;
                cout << "Enter the cities (city1 city2), road type, distance, traffic percentage, and capacity: ";
                cin >> city1 >> city2 >> roadType >> distance >> traffic >> capacity;

                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "Invalid cities. Try again.\n";
                    break;
                }

                addEdge(adjList, cityToIndex[city1], cityToIndex[city2], distance, traffic, roadType, capacity);
                cout << "New connection added.\n";

                break;
            }



            case 6:
            {
                // Display traffic information
                displayTrafficInfo(adjList, indexToCity);
                break;
            }



            case 7:
            {
                // Update road capacity
                string city1, city2;
                int newCapacity;
                cout << "Enter the cities (city1 city2) to update road capacity: ";
                cin >> city1 >> city2;
                cout << "Enter new capacity: ";
                cin >> newCapacity;

                city1 = toLower(city1);
                city2 = toLower(city2);

                if (cityToIndex.find(city1) == cityToIndex.end() || cityToIndex.find(city2) == cityToIndex.end())
                {
                    cout << "Invalid cities. Try again.\n";
                    break;
                }

                updateRoadCapacity(adjList, cityToIndex[city1], cityToIndex[city2], newCapacity);
                cout << "Road capacity updated successfully.\n";
                break;
            }


            case 8:
            {
                // Add new local place
                string city;
                string placeName;
                int distance, traffic;
                string roadType;

                cout << "Enter the city: ";
                cin >> city;
                city = toLower(city);

                if (cityToIndex.find(city) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                cout << "Enter the name of the place: ";
                cin.ignore();
                getline(cin, placeName);
                cout << "Enter the distance (in km) from city center: ";
                cin >> distance;
                cout << "Enter the traffic percentage: ";
                cin >> traffic;
                cout << "Enter the road type: ";
                cin >> roadType;

                LocalPlace newPlace = {placeName, distance, traffic, roadType};
                localPlaces[cityToIndex[city]].push_back(newPlace);

                cout << "New local place added successfully.\n";
                break;
            }


            case 9:
            {
                // Display local places in a city
                string city;
                cout << "Enter the city: ";
                cin >> city;
                city = toLower(city);

                if (cityToIndex.find(city) == cityToIndex.end())
                {
                    cout << "City not found.\n";
                    break;
                }

                int cityIndex = cityToIndex[city];
                cout << "Local places in " << city << ":\n";
                for (const LocalPlace& place : localPlaces[cityIndex])
                {
                    cout << "Place: " << place.name
                         << " | Distance: " << place.distance << " km"
                         << " | Traffic: " << place.traffic << "% traffic"
                         << " | Road Type: " << place.roadType << endl;
                }
                break;
            }



            case 10:
            {
                // Display all cities
                displayAllCities(cityToIndex);
                break;
            }


            case 11:
            {
                // Display all roads
                displayAllRoads(adjList, indexToCity);
                break;
            }


            case 12:
            {
                // Calculate total number of roads
                int totalRoads = calculateTotalRoads(adjList);
                cout << "Total number of roads: " << totalRoads << endl;
                break;
            }


            case 13:
                return 0;


            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }
    }
    return 0;
}




#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


struct Review1
 {
    string username;
    string text;
    float rating;
};


struct Restaurant
{
    string name;
    string cuisineType;
    string mealType;
    string address;
    string contact;
    float rating;
    string description;
    vector<Review1> reviews;
};


struct User1
 {
    string username;
    string password;
    vector<string> recommendationHistory;
    vector<Review1> userReviews;
};


void displayOptions(const vector<string>& options)
{
    for (int i = 0; i < options.size(); ++i)
        {
        cout << i + 1 << ". " << options[i] << endl;
    }
}

void createUser1(vector<User1>& users)
{
    cout << "Enter a username to sign up: ";
    string username;
    cin >> username;

    cout << "Enter a password: ";
    string password;
    cin >> password;

    users.push_back({username, password});
    cout << "User account created successfully! You can now log in.\n";
}


bool authenticateUser1(const vector<User1>& users, string& username)
{
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    string password;
    cin >> password;

    for (const auto& user : users)
        {
        if (user.username == username && user.password == password)
         {
            cout << "Authentication successful! Welcome, " << username << "!\n";
            return true;
        }
    }
    return false;
}


void getRecommendations(const vector<Restaurant>& cityRestaurants, User1& user)
{
    if (cityRestaurants.empty())
        {
        cout << "No recommendations available for this city.\n";
        return;
    }

    for (const auto& restaurant : cityRestaurants)
        {
        cout << "- " << restaurant.name << " | Address: " << restaurant.address << " | Contact: " << restaurant.contact << " | Rating: " << restaurant.rating << endl;
        cout << "  Description: " << restaurant.description << endl;
        user.recommendationHistory.push_back(restaurant.name);
    }
}



void getRecommendationsByMeal(const vector<Restaurant>& cityRestaurants, const string& meal, User1& user)
{
    bool found = false;
    for (const auto& restaurant : cityRestaurants)
        {
        if (restaurant.mealType == meal)
        {
            cout << "- " << restaurant.name << " | Address: " << restaurant.address << " | Contact: " << restaurant.contact << " | Rating: " << restaurant.rating << endl;
            cout << "  Description: " << restaurant.description << endl;
            user.recommendationHistory.push_back(restaurant.name + " for " + meal);
            found = true;
        }
    }

    if (!found)
        {
        cout << "No recommendations available for " << meal << " in this city.\n";
    }
}


void getRecommendationsByCuisine(const vector<Restaurant>& cityRestaurants, const string& meal, const string& cuisine, User1& user)
 {
    bool found = false;
    for (const auto& restaurant : cityRestaurants)
        {
        if (restaurant.mealType == meal && restaurant.cuisineType == cuisine)
         {
            cout << "- " << restaurant.name << " | Address: " << restaurant.address << " | Contact: " << restaurant.contact << " | Rating: " << restaurant.rating << endl;
            cout << "  Description: " << restaurant.description << endl;
            user.recommendationHistory.push_back(restaurant.name + " (" + cuisine + ")");
            found = true;
        }
    }

    if (!found)
        {
        cout << "No recommendations available for " << cuisine << " cuisine and " << meal << " in this city.\n";
    }
}


void leaveFeedback(User1& user)
{
    cout << "Please enter the name of the restaurant you want to review: ";
    string restaurantName;
    cin.ignore();
    getline(cin, restaurantName);

    cout << "Please leave your review (up to 500 characters): ";
    string feedback;
    getline(cin, feedback);

    cout << "Please rate the restaurant (0.0 to 5.0): ";
    float rating;
    cin >> rating;

    Review1 review = {user.username, feedback, rating};
    user.userReviews.push_back(review);

    cout << "Thank you for your feedback!\n";
}


void viewHistory(const User1& user)
{
    cout << "Recommendation history for " << user.username << ":\n";
    if (user.recommendationHistory.empty())
        {
        cout << "No recommendations yet.\n";
    }
    else
        {
        for (const auto& entry : user.recommendationHistory)
         {
            cout << "- " << entry << endl;
        }
    }
}


void viewUserReviews(const User1& user)
{
    cout << "Reviews by " << user.username << ":\n";
    if (user.userReviews.empty())
        {
        cout << "No reviews yet.\n";
    }
    else
        {
        for (const auto& review : user.userReviews)
        {
            cout << "Restaurant: " << review.text << " | Rating: " << review.rating << endl;
        }
    }
}


void searchRestaurantByName(const map<string, vector<Restaurant>>& restaurants)
 {
    cout << "Enter the name of the restaurant to search: ";
    string restaurantName;
    cin.ignore();
    getline(cin, restaurantName);

    for (const auto& cityRestaurants : restaurants)
        {
        for (const auto& restaurant : cityRestaurants.second)
         {
            if (restaurant.name == restaurantName)
             {
                cout << "Restaurant found: " << restaurant.name << " | Address: " << restaurant.address << " | Contact: " << restaurant.contact << " | Rating: " << restaurant.rating << endl;
                cout << "Description: " << restaurant.description << endl;
                return;
            }
        }
    }

    cout << "Restaurant not found.\n";
}



void displayAllRestaurants(const map<string, vector<Restaurant>>& restaurants)
 {
    for (const auto& cityRestaurants : restaurants)
        {
        for (const auto& restaurant : cityRestaurants.second)
        {
            cout << "City: " << cityRestaurants.first << " | Name: " << restaurant.name << " | Address: " << restaurant.address << " | Contact: " << restaurant.contact << " | Rating: " << restaurant.rating << endl;
            cout << "Description: " << restaurant.description << endl;
        }
    }
}


map<string, vector<Restaurant>> initializeRestaurants()
{
    return
    {
        {"Bengaluru", {{"The North Kitchen", "North Indian", "Lunch", "MG Road, Bengaluru", "1234567890", 4.5, "A cozy place offering the best North Indian cuisine."},
                       {"South Spice", "South Indian", "Dinner", "Koramangala, Bengaluru", "0987654321", 4.0, "Delicious South Indian dishes with a modern twist."},
                       {"Pizza Place", "Italian", "Lunch", "Whitefield, Bengaluru", "1122334455", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                       {"Burger Hub", "American", "Dinner", "Indiranagar, Bengaluru", "5678901234", 4.4, "Gourmet burgers with a variety of toppings."},
                       {"Sushi World", "Japanese", "Lunch", "Brigade Road, Bengaluru", "3456789012", 4.6, "Fresh sushi and sashimi with a modern twist."},
                       {"Cafe Delight", "Cafe", "Breakfast", "Residency Road, Bengaluru", "4567890123", 4.2, "A perfect place for a morning coffee and breakfast."},
                       {"Grill House", "American", "Dinner", "Jayanagar, Bengaluru", "5678901234", 4.3, "Delicious grilled dishes with a variety of options."},
                       {"Spice Route", "Indian", "Lunch", "Indiranagar, Bengaluru", "6789012345", 4.5, "A mix of traditional and modern Indian cuisine."},
                       {"Ocean's Catch", "Seafood", "Dinner", "Whitefield, Bengaluru", "7890123456", 4.4, "Fresh seafood with a coastal ambiance."},
                       {"Green Bowl", "Vegan", "Lunch", "MG Road, Bengaluru", "8901234567", 4.3, "Healthy and delicious vegan dishes."}}},
        {"Mumbai", {{"Mumbai Thali", "North Indian", "Dinner", "Andheri, Mumbai", "2233445566", 4.2, "Traditional Mumbai thalis with a variety of flavors."},
                    {"Dosa Delight", "South Indian", "Breakfast", "Juhu, Mumbai", "6677889900", 4.4, "The best place for crispy dosas and other South Indian delicacies."},
                    {"Pasta Paradise", "Italian", "Lunch", "Bandra, Mumbai", "5566778899", 4.3, "Delicious pasta dishes with a variety of sauces."},
                    {"Seafood Shack", "Seafood", "Dinner", "Colaba, Mumbai", "7788990011", 4.5, "Fresh seafood with a beachside ambiance."},
                    {"Spice House", "Indian", "Lunch", "Dadar, Mumbai", "9900112233", 4.4, "A mix of Indian cuisines with a modern twist."},
                    {"Burger Joint", "American", "Lunch", "Bandra, Mumbai", "1122334455", 4.3, "Gourmet burgers with a variety of toppings."},
                    {"Sushi Corner", "Japanese", "Dinner", "Colaba, Mumbai", "2233445566", 4.6, "Fresh sushi and sashimi prepared by expert chefs."},
                    {"Pizza Plaza", "Italian", "Dinner", "Andheri, Mumbai", "3344556677", 4.4, "Authentic Italian pizzas with a variety of toppings."},
                    {"Tandoor Treats", "North Indian", "Lunch", "Juhu, Mumbai", "4455667788", 4.3, "Delicious tandoori dishes with rich flavors."},
                    {"Cafe Bliss", "Cafe", "Breakfast", "Dadar, Mumbai", "5566778899", 4.2, "A cozy cafe for a relaxing breakfast."}}},
        {"Delhi", {{"Royal Tandoor", "North Indian", "Lunch", "Connaught Place, Delhi", "9988776655", 4.6, "Delectable North Indian cuisine with rich flavors."},
                   {"Delhi Biryani", "North Indian", "Dinner", "Karol Bagh, Delhi", "5566778899", 4.3, "Famous for its exquisite biryani and tandoori dishes."},
                   {"Peking Express", "Chinese", "Lunch", "Chandni Chowk, Delhi", "1234567890", 4.4, "Quick and delicious Chinese dishes."},
                   {"Cafe Italiano", "Italian", "Dinner", "Hauz Khas, Delhi", "2345678901", 4.5, "Authentic Italian cuisine in a cozy setting."},
                   {"Rajma Chawal Junction", "North Indian", "Lunch", "Rajouri Garden, Delhi", "3456789012", 4.3, "Comfort food with rich North Indian flavors."},
                   {"Burger Joint", "American", "Lunch", "Saket, Delhi", "4567890123", 4.4, "Juicy burgers with a variety of toppings."},
                   {"Sushi Stop", "Japanese", "Dinner", "Vasant Kunj, Delhi", "5678901234", 4.5, "Fresh sushi and sashimi with a modern twist."},
                   {"Pizza House", "Italian", "Lunch", "Connaught Place, Delhi", "6789012345", 4.3, "Delicious pizzas with fresh ingredients."},
                   {"Spice Garden", "Indian", "Dinner", "Karol Bagh, Delhi", "7890123456", 4.2, "A variety of Indian dishes with rich flavors."},
                   {"Cafe Aroma", "Cafe", "Breakfast", "Hauz Khas, Delhi", "8901234567", 4.3, "A perfect spot for a morning coffee and breakfast."}}},
        {"Chennai", {{"Chettinad Corner", "South Indian", "Lunch", "T. Nagar, Chennai", "4455667788", 4.5, "A popular spot for traditional Chettinad dishes."},
                     {"Idli Express", "South Indian", "Breakfast", "Anna Nagar, Chennai", "3322110099", 4.2, "Perfect for a quick breakfast with fresh idlis and vadas."},
                     {"Pasta Street", "Italian", "Lunch", "Nungambakkam, Chennai", "5566778899", 4.3, "A delightful Italian dining experience."},
                     {"Dragon House", "Chinese", "Dinner", "Velachery, Chennai", "6677889900", 4.4, "Serving authentic Chinese cuisine."},
                     {"Burger Bliss", "American", "Dinner", "Besant Nagar, Chennai", "7788990011", 4.5, "Gourmet burgers with a variety of toppings."},
                     {"Cafe Delight", "Cafe", "Breakfast", "T. Nagar, Chennai", "8899001122", 4.3, "A cozy cafe for a hearty breakfast."},
                     {"Seafood Shack", "Seafood", "Lunch", "Marina Beach, Chennai", "9900112233", 4.4, "Fresh seafood with a coastal vibe."},
                     {"Sushi Spot", "Japanese", "Dinner", "Nungambakkam, Chennai", "1122334455", 4.6, "Delicious sushi and sashimi prepared by expert chefs."},
                     {"Pizza Palace", "Italian", "Lunch", "Anna Nagar, Chennai", "2233445566", 4.3, "Authentic Italian pizzas with a variety of toppings."},
                     {"Spice Route", "Indian", "Dinner", "Velachery, Chennai", "3344556677", 4.2, "A mix of traditional and modern Indian cuisine."}}},
        {"Kolkata", {{"Kolkata Curry", "North Indian", "Dinner", "Salt Lake, Kolkata", "2233445566", 4.4, "Authentic North Indian cuisine with a Bengali twist."},
                     {"Bengal Feast", "North Indian", "Lunch", "Park Street, Kolkata", "7788990011", 4.3, "Known for its sumptuous thalis and Bengali delicacies."},
                     {"Chinese Dragon", "Chinese", "Lunch", "Tangra, Kolkata", "9988776655", 4.2, "Delicious Chinese food in the heart of Kolkata."},
                     {"Pizza Haven", "Italian", "Dinner", "Camac Street, Kolkata", "2233445566", 4.5, "Heavenly pizzas with fresh ingredients."},
                     {"Fish Fry Delight", "Bengali", "Lunch", "Esplanade, Kolkata", "3344556677", 4.3, "Famous for its crispy fish fry and Bengali delicacies."},
                     {"Burger Joint", "American", "Lunch", "Park Street, Kolkata", "4455667788", 4.4, "Juicy burgers with a variety of toppings."},
                     {"Sushi Corner", "Japanese", "Dinner", "Salt Lake, Kolkata", "5566778899", 4.6, "Fresh sushi and sashimi prepared by expert chefs."},
                     {"Pasta House", "Italian", "Lunch", "Camac Street, Kolkata", "6677889900", 4.3, "Delicious pasta dishes with a variety of sauces."},
                     {"Spice Garden", "Indian", "Dinner", "Tangra, Kolkata", "7788990011", 4.2, "A mix of traditional and modern Indian cuisine."},
                     {"Cafe Bliss", "Cafe", "Breakfast", "Park Street, Kolkata", "8899001122", 4.3, "A perfect spot for a morning coffee and breakfast."}}},
        {"Hyderabad", {{"Biryani Palace", "North Indian", "Lunch", "Hitech City, Hyderabad", "9988112233", 4.5, "A palace of flavors serving the best biryani."},
                       {"Andhra Spice", "South Indian", "Dinner", "Banjara Hills, Hyderabad", "1122998899", 4.4, "Famous for its spicy Andhra cuisine."},
                       {"Tandoori Treats", "North Indian", "Lunch", "Jubilee Hills, Hyderabad", "3344556677", 4.6, "Tandoori dishes with a rich taste."},
                       {"Pasta Perfection", "Italian", "Dinner", "Gachibowli, Hyderabad", "2233445566", 4.3, "Perfect pasta dishes with authentic flavors."},
                       {"Seafood Sensation", "Seafood", "Lunch", "Madhapur, Hyderabad", "1122334455", 4.5, "A seafood lover's paradise."},
                       {"Burger Joint", "American", "Lunch", "Hitech City, Hyderabad", "2233445566", 4.3, "Gourmet burgers with a variety of toppings."},
                       {"Sushi Stop", "Japanese", "Dinner", "Banjara Hills, Hyderabad", "3344556677", 4.5, "Fresh sushi and sashimi with a modern twist."},
                       {"Pizza Plaza", "Italian", "Lunch", "Jubilee Hills, Hyderabad", "4455667788", 4.4, "Authentic Italian pizzas with a variety of toppings."},
                       {"Spice Route", "Indian", "Dinner", "Madhapur, Hyderabad", "5566778899", 4.2, "A mix of traditional and modern Indian cuisine."},
                       {"Cafe Delight", "Cafe", "Breakfast", "Gachibowli, Hyderabad", "6677889900", 4.3, "A cozy cafe for a hearty breakfast."}}},
        {"Pune", {{"Pune Tandoor", "North Indian", "Dinner", "Koregaon Park, Pune", "6677889922", 4.3, "A delightful experience of North Indian tandoori dishes."},
                  {"Masala Dosa Hub", "South Indian", "Breakfast", "FC Road, Pune", "2211334455", 4.5, "A hub for the best masala dosas in town."},
                  {"Italiano Bistro", "Italian", "Lunch", "Baner, Pune", "3344556677", 4.2, "Italian delicacies in a cozy setting."},
                  {"Asian Delight", "Chinese", "Dinner", "Viman Nagar, Pune", "4455667788", 4.4, "Delicious Chinese cuisine with a variety of options."},
                  {"Burger Bonanza", "American", "Lunch", "Camp, Pune", "5566778899", 4.3, "Juicy burgers with a variety of toppings."},
                  {"Sushi Corner", "Japanese", "Dinner", "Koregaon Park, Pune", "6677889900", 4.6, "Fresh sushi and sashimi with a modern twist."},
                  {"Pizza Point", "Italian", "Lunch", "Baner, Pune", "7788990011", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                  {"Spice Route", "Indian", "Dinner", "Viman Nagar, Pune", "8899001122", 4.2, "A mix of traditional and modern Indian cuisine."},
                  {"Cafe Delight", "Cafe", "Breakfast", "FC Road, Pune", "9900112233", 4.3, "A cozy cafe for a hearty breakfast."},
                  {"Seafood Shack", "Seafood", "Lunch", "Camp, Pune", "1122334455", 4.4, "Fresh seafood with a coastal vibe."},
                  {"Grill House", "American", "Dinner", "Koregaon Park, Pune", "2233445566", 4.5, "Delicious grilled dishes with a variety of options."}}},
        {"Jaipur", {{"Rajputana Feast", "North Indian", "Lunch", "MI Road, Jaipur", "3344556677", 4.4, "A feast of traditional Rajputana cuisine."},
                    {"Jaipur Dhaba", "North Indian", "Dinner", "Vaishali Nagar, Jaipur", "6677885511", 4.2, "A dhaba-style experience with mouthwatering dishes."},
                    {"Pizza Palace", "Italian", "Lunch", "C-Scheme, Jaipur", "7788990011", 4.3, "Delicious Italian pizzas and pasta."},
                    {"Chinese Express", "Chinese", "Dinner", "Malviya Nagar, Jaipur", "2233445566", 4.4, "Quick and tasty Chinese food."},
                    {"Burger King", "American", "Lunch", "Bapu Nagar, Jaipur", "4455667788", 4.5, "Delicious burgers with a variety of toppings."},
                    {"Sushi Spot", "Japanese", "Dinner", "C-Scheme, Jaipur", "5566778899", 4.6, "Fresh sushi and sashimi with a modern twist."},
                    {"Pasta Point", "Italian", "Lunch", "Vaishali Nagar, Jaipur", "6677889900", 4.3, "Authentic Italian pasta dishes."},
                    {"Spice Route", "Indian", "Dinner", "Malviya Nagar, Jaipur", "7788990011", 4.2, "A mix of traditional and modern Indian cuisine."},
                    {"Cafe Delight", "Cafe", "Breakfast", "MI Road, Jaipur", "8899001122", 4.3, "A cozy cafe for a hearty breakfast."},
                    {"Seafood Shack", "Seafood", "Lunch", "Bapu Nagar, Jaipur", "9900112233", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Ahmedabad", {{"Gujarat Thali", "North Indian", "Dinner", "Navrangpura, Ahmedabad", "4455667788", 4.6, "A thali restaurant serving authentic Gujarati cuisine."},
                       {"Dhokla Delight", "South Indian", "Breakfast", "Maninagar, Ahmedabad", "9988223355", 4.4, "Known for its delicious dhoklas and South Indian snacks."},
                       {"Pasta Paradise", "Italian", "Lunch", "CG Road, Ahmedabad", "2233445566", 4.3, "A paradise for pasta lovers."},
                       {"Sichuan House", "Chinese", "Dinner", "SG Highway, Ahmedabad", "5566778899", 4.2, "Authentic Chinese cuisine with a Sichuan twist."},
                       {"Burger World", "American", "Lunch", "Vastrapur, Ahmedabad", "6677889900", 4.5, "Gourmet burgers with a variety of toppings."},
                       {"Sushi Stop", "Japanese", "Dinner", "Navrangpura, Ahmedabad", "7788990011", 4.6, "Fresh sushi and sashimi with a modern twist."},
                       {"Pizza Point", "Italian", "Lunch", "Maninagar, Ahmedabad", "8899001122", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                       {"Spice Garden", "Indian", "Dinner", "SG Highway, Ahmedabad", "9900112233", 4.2, "A mix of traditional and modern Indian cuisine."},
                       {"Cafe Bliss", "Cafe", "Breakfast", "Vastrapur, Ahmedabad", "1122334455", 4.3, "A perfect spot for a morning coffee and breakfast."},
                       {"Seafood Shack", "Seafood", "Lunch", "Navrangpura, Ahmedabad", "2233445566", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Lucknow", {{"Awadhi Cuisine", "North Indian", "Lunch", "Hazratganj, Lucknow", "5566774433", 4.5, "Delicious Awadhi cuisine with rich flavors."},
                     {"Lucknowi Kebab", "North Indian", "Dinner", "Aminabad, Lucknow", "1122445599", 4.6, "Famous for its kebabs and Mughlai dishes."},
                     {"Pizza Plus", "Italian", "Lunch", "Aliganj, Lucknow", "2233445566", 4.3, "Delicious pizzas with a variety of toppings."},
                     {"Chinese Delight", "Chinese", "Dinner", "Gomti Nagar, Lucknow", "3344556677", 4.2, "Authentic Chinese food with a variety of options."},
                     {"Burger Barn", "American", "Lunch", "Indira Nagar, Lucknow", "4455667788", 4.4, "Juicy burgers with a variety of toppings."},
                     {"Sushi Stop", "Japanese", "Dinner", "Hazratganj, Lucknow", "5566778899", 4.6, "Fresh sushi and sashimi with a modern twist."},
                     {"Pasta House", "Italian", "Lunch", "Aminabad, Lucknow", "6677889900", 4.3, "Authentic Italian pasta dishes."},
                     {"Spice Route", "Indian", "Dinner", "Gomti Nagar, Lucknow", "7788990011", 4.2, "A mix of traditional and modern Indian cuisine."},
                     {"Cafe Bliss", "Cafe", "Breakfast", "Aliganj, Lucknow", "8899001122", 4.3, "A cozy cafe for a hearty breakfast."},
                     {"Seafood Shack", "Seafood", "Lunch", "Indira Nagar, Lucknow", "9900112233", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Coimbatore", {{"Tamil Taste", "South Indian", "Lunch", "RS Puram, Coimbatore", "6677448899", 4.3, "A place for authentic Tamil cuisine."},
                        {"Coconut Grove", "South Indian", "Breakfast", "Gandhipuram, Coimbatore", "3344552288", 4.2, "Great spot for a hearty South Indian breakfast."},
                        {"Pasta Point", "Italian", "Lunch", "Saibaba Colony, Coimbatore", "5566778899", 4.3, "A cozy spot for Italian food lovers."},
                        {"Dragon Diner", "Chinese", "Dinner", "Race Course, Coimbatore", "6677889900", 4.4, "Delicious Chinese food with a variety of options."},
                        {"Burger Boulevard", "American", "Lunch", "Avinashi Road, Coimbatore", "7788990011", 4.5, "Juicy burgers with gourmet toppings."},
                        {"Sushi Corner", "Japanese", "Dinner", "RS Puram, Coimbatore", "8899001122", 4.6, "Fresh sushi and sashimi with a modern twist."},
                        {"Pizza House", "Italian", "Lunch", "Gandhipuram, Coimbatore", "9900112233", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                        {"Spice Route", "Indian", "Dinner", "Saibaba Colony, Coimbatore", "1122334455", 4.2, "A mix of traditional and modern Indian cuisine."},
                        {"Cafe Delight", "Cafe", "Breakfast", "Race Course, Coimbatore", "2233445566", 4.3, "A cozy cafe for a hearty breakfast."},
                        {"Seafood Shack", "Seafood", "Lunch", "Avinashi Road, Coimbatore", "3344556677", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Bhopal", {{"Madhya Platter", "North Indian", "Dinner", "MP Nagar, Bhopal", "7788332211", 4.3, "Serving a variety of North Indian dishes."},
                    {"Bhopal Bite", "North Indian", "Lunch", "Arera Colony, Bhopal", "9988775533", 4.4, "A local favorite for lunch with diverse options."},
                    {"Pasta Passion", "Italian", "Lunch", "New Market, Bhopal", "2233445566", 4.2, "A delight for pasta lovers."},
                    {"Chinese Corner", "Chinese", "Dinner", "Maharana Pratap Nagar, Bhopal", "3344556677", 4.3, "Authentic Chinese cuisine with a variety of options."},
                    {"Burger Haven", "American", "Lunch", "TT Nagar, Bhopal", "4455667788", 4.4, "Delicious burgers with a variety of toppings."},
                    {"Sushi Stop", "Japanese", "Dinner", "MP Nagar, Bhopal", "5566778899", 4.6, "Fresh sushi and sashimi with a modern twist."},
                    {"Pizza Palace", "Italian", "Lunch", "Arera Colony, Bhopal", "6677889900", 4.3, "Authentic Italian pasta dishes."},
                    {"Spice Route", "Indian", "Dinner", "New Market, Bhopal", "7788990011", 4.2, "A mix of traditional and modern Indian cuisine."},
                    {"Cafe Bliss", "Cafe", "Breakfast", "TT Nagar, Bhopal", "8899001122", 4.3, "A cozy cafe for a hearty breakfast."},
                    {"Seafood Shack", "Seafood", "Lunch", "Maharana Pratap Nagar, Bhopal", "9900112233", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Darjeeling", {{"Darjeeling Tea House", "North Indian", "Breakfast", "Mall Road, Darjeeling", "1234567801", 4.5, "A perfect place to enjoy breakfast with tea."},
                        {"Himalayan Flavors", "South Indian", "Dinner", "Chowrasta, Darjeeling", "2345678901", 4.3, "Offering a mix of South Indian and local dishes."},
                        {"Pasta Point", "Italian", "Lunch", "Mall Road, Darjeeling", "3456789012", 4.2, "A cozy spot for Italian cuisine."},
                        {"Chinese Delight", "Chinese", "Lunch", "Gandhi Road, Darjeeling", "4567890123", 4.4, "Delicious Chinese food in a scenic setting."},
                        {"Burger Bliss", "American", "Dinner", "Ladenla Road, Darjeeling", "5678901234", 4.5, "Gourmet burgers with a variety of toppings."},
                        {"Sushi Corner", "Japanese", "Dinner", "Chowrasta, Darjeeling", "6789012345", 4.6, "Fresh sushi and sashimi with a modern twist."},
                        {"Pizza House", "Italian", "Lunch", "Mall Road, Darjeeling", "7890123456", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                        {"Spice Garden", "Indian", "Dinner", "Gandhi Road, Darjeeling", "8901234567", 4.2, "A mix of traditional and modern Indian cuisine."},
                        {"Cafe Bliss", "Cafe", "Breakfast", "Ladenla Road, Darjeeling", "9012345678", 4.3, "A perfect spot for a morning coffee and breakfast."},
                        {"Seafood Shack", "Seafood", "Lunch", "Chowrasta, Darjeeling", "0123456789", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Shimla", {{"Himalayan Dhaba", "North Indian", "Lunch", "Mall Road, Shimla", "3456789012", 4.4, "Authentic North Indian food with a scenic view."},
                    {"Shimla Sweets", "South Indian", "Breakfast", "Lakkar Bazaar, Shimla", "4567890123", 4.2, "Serving a variety of South Indian breakfast options."},
                    {"Pasta Place", "Italian", "Lunch", "Mall Road, Shimla", "5678901234", 4.3, "Delicious Italian food with a great view."},
                    {"China Town", "Chinese", "Dinner", "The Ridge, Shimla", "6789012345", 4.5, "Chinese cuisine with a beautiful setting."},
                    {"Burger Bay", "American", "Lunch", "Lower Bazaar, Shimla", "7890123456", 4.4, "Juicy burgers with a variety of toppings."},
                    {"Sushi Spot", "Japanese", "Dinner", "Mall Road, Shimla", "8901234567", 4.6, "Fresh sushi and sashimi with a modern twist."},
                    {"Pizza Point", "Italian", "Lunch", "The Ridge, Shimla", "9012345678", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                    {"Spice Garden", "Indian", "Dinner", "Lakkar Bazaar, Shimla", "0123456789", 4.2, "A mix of traditional and modern Indian cuisine."},
                    {"Cafe Bliss", "Cafe", "Breakfast", "Lower Bazaar, Shimla", "1234567890", 4.3, "A cozy cafe for a hearty breakfast."},
                    {"Seafood Shack", "Seafood", "Lunch", "Mall Road, Shimla", "2345678901", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Goa", {{"Goan Delights", "North Indian", "Lunch", "Calangute Beach, Goa", "5678901234", 4.5, "Delicious North Indian food by the beach."},
                 {"Beach Bites", "South Indian", "Dinner", "Baga Beach, Goa", "6789012345", 4.4, "A great spot for dinner with South Indian delicacies."},
                 {"Pizza Paradise", "Italian", "Lunch", "Anjuna, Goa", "7890123456", 4.3, "Heavenly pizzas with fresh ingredients."},
                 {"Dragon Inn", "Chinese", "Dinner", "Candolim, Goa", "8901234567", 4.5, "Chinese food with a beachside view."},
                 {"Burger Beach", "American", "Lunch", "Palolem, Goa", "9012345678", 4.4, "Gourmet burgers with a variety of toppings."},
                 {"Sushi Spot", "Japanese", "Dinner", "Calangute Beach, Goa", "0123456789", 4.6, "Fresh sushi and sashimi with a modern twist."},
                 {"Pasta House", "Italian", "Lunch", "Baga Beach, Goa", "1234567890", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                 {"Spice Garden", "Indian", "Dinner", "Anjuna, Goa", "2345678901", 4.2, "A mix of traditional and modern Indian cuisine."},
                 {"Cafe Bliss", "Cafe", "Breakfast", "Candolim, Goa", "3456789012", 4.3, "A perfect spot for a morning coffee and breakfast."},
                 {"Seafood Shack", "Seafood", "Lunch", "Palolem, Goa", "4567890123", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Agra", {{"Taj Mahal Treats", "North Indian", "Lunch", "Tajganj, Agra", "7890123456", 4.6, "Amazing North Indian cuisine with a view of the Taj Mahal."},
                  {"Agra Biryani House", "South Indian", "Dinner", "Sadar Bazaar, Agra", "8901234567", 4.4, "Known for its flavorful biryanis."},
                  {"Pasta Point", "Italian", "Lunch", "Fatehabad Road, Agra", "9012345678", 4.3, "Delicious pasta dishes with a variety of sauces."},
                  {"Chinese Delight", "Chinese", "Dinner", "MG Road, Agra", "0123456789", 4.5, "Authentic Chinese food in the heart of Agra."},
                  {"Burger Bliss", "American", "Lunch", "Civil Lines, Agra", "1234567890", 4.4, "Juicy burgers with a variety of toppings."},
                  {"Sushi Stop", "Japanese", "Dinner", "Tajganj, Agra", "2345678901", 4.6, "Fresh sushi and sashimi with a modern twist."},
                  {"Pizza House", "Italian", "Lunch", "Sadar Bazaar, Agra", "3456789012", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                  {"Spice Garden", "Indian", "Dinner", "Fatehabad Road, Agra", "4567890123", 4.2, "A mix of traditional and modern Indian cuisine."},
                  {"Cafe Bliss", "Cafe", "Breakfast", "Civil Lines, Agra", "5678901234", 4.3, "A cozy cafe for a hearty breakfast."},
                  {"Seafood Shack", "Seafood", "Lunch", "MG Road, Agra", "6789012345", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Varanasi", {{"Ganga Ghat Cafe", "North Indian", "Breakfast", "Assi Ghat, Varanasi", "9012345678", 4.3, "A serene place to enjoy a hearty breakfast by the Ganges."},
                      {"Varanasi Veggies", "South Indian", "Lunch", "Lanka, Varanasi", "0123456789", 4.2, "Popular for its vegetarian South Indian meals."},
                      {"Pasta Palace", "Italian", "Lunch", "Sarnath, Varanasi", "1234567890", 4.4, "A palace for Italian food lovers."},
                      {"China Garden", "Chinese", "Dinner", "Godowlia, Varanasi", "2345678901", 4.5, "Delicious Chinese food with a local twist."},
                      {"Burger Barn", "American", "Lunch", "Ramnagar, Varanasi", "3456789012", 4.3, "Gourmet burgers with a variety of toppings."},
                      {"Sushi Spot", "Japanese", "Dinner", "Assi Ghat, Varanasi", "4567890123", 4.6, "Fresh sushi and sashimi with a modern twist."},
                      {"Pizza House", "Italian", "Lunch", "Lanka, Varanasi", "5678901234", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                      {"Spice Route", "Indian", "Dinner", "Godowlia, Varanasi", "6789012345", 4.2, "A mix of traditional and modern Indian cuisine."},
                      {"Cafe Bliss", "Cafe", "Breakfast", "Sarnath, Varanasi", "7890123456", 4.3, "A perfect spot for a morning coffee and breakfast."},
                      {"Seafood Shack", "Seafood", "Lunch", "Ramnagar, Varanasi", "8901234567", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Udaipur", {{"Lakeview Lounge", "North Indian", "Dinner", "Pichola Lake, Udaipur", "1122334455", 4.7, "Romantic dining experience with North Indian food by the lake."},
                     {"Rajasthani Feast", "South Indian", "Lunch", "City Palace, Udaipur", "2233445566", 4.5, "A royal experience with South Indian cuisine."},
                     {"Pasta Plaza", "Italian", "Lunch", "Fateh Sagar, Udaipur", "3344556677", 4.3, "Delicious Italian food with a beautiful view."},
                     {"China Town", "Chinese", "Dinner", "Surajpole, Udaipur", "4455667788", 4.4, "Authentic Chinese cuisine with a variety of options."},
                     {"Burger Bistro", "American", "Lunch", "Hiran Magri, Udaipur", "5566778899", 4.5, "Juicy burgers with gourmet toppings."},
                     {"Sushi Stop", "Japanese", "Dinner", "Pichola Lake, Udaipur", "6677889900", 4.6, "Fresh sushi and sashimi with a modern twist."},
                     {"Pizza House", "Italian", "Lunch", "City Palace, Udaipur", "7788990011", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                     {"Spice Route", "Indian", "Dinner", "Fateh Sagar, Udaipur", "8899001122", 4.2, "A mix of traditional and modern Indian cuisine."},
                     {"Cafe Bliss", "Cafe", "Breakfast", "Hiran Magri, Udaipur", "9900112233", 4.3, "A cozy cafe for a hearty breakfast."},
                     {"Seafood Shack", "Seafood", "Lunch", "Surajpole, Udaipur", "1122334455", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Amritsar", {{"Golden Temple Tandoor", "North Indian", "Dinner", "Golden Temple, Amritsar", "3344556677", 4.8, "Famous for its tandoori dishes near the Golden Temple."},
                      {"Amritsari Dhaba", "South Indian", "Breakfast", "Hall Bazaar, Amritsar", "4455667788", 4.6, "A bustling place for a hearty South Indian breakfast."},
                      {"Pasta Palace", "Italian", "Lunch", "Ranjit Avenue, Amritsar", "5566778899", 4.3, "A palace for pasta lovers."},
                      {"Chinese Corner", "Chinese", "Dinner", "Lawrence Road, Amritsar", "6677889900", 4.4, "Quick and delicious Chinese food."},
                      {"Burger Bay", "American", "Lunch", "Mall Road, Amritsar", "7788990011", 4.5, "Gourmet burgers with a variety of toppings."},
                      {"Sushi Stop", "Japanese", "Dinner", "Golden Temple, Amritsar", "8899001122", 4.6, "Fresh sushi and sashimi with a modern twist."},
                      {"Pizza House", "Italian", "Lunch", "Hall Bazaar, Amritsar", "9900112233", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                      {"Spice Route", "Indian", "Dinner", "Lawrence Road, Amritsar", "1122334455", 4.2, "A mix of traditional and modern Indian cuisine."},
                      {"Cafe Bliss", "Cafe", "Breakfast", "Mall Road, Amritsar", "2233445566", 4.3, "A perfect spot for a morning coffee and breakfast."},
                      {"Seafood Shack", "Seafood", "Lunch", "Ranjit Avenue, Amritsar", "3344556677", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Mysore", {{"Palace Platter", "North Indian", "Lunch", "Mysore Palace, Mysore", "5566778899", 4.4, "A royal spread of North Indian dishes."},
                    {"Mysore Masala", "South Indian", "Dinner", "Chamundi Hill, Mysore", "6677889900", 4.3, "Offering a variety of South Indian delicacies."},
                    {"Pasta Point", "Italian", "Lunch", "KR Circle, Mysore", "7788990011", 4.3, "A delightful spot for Italian food lovers."},
                    {"China Garden", "Chinese", "Dinner", "Devaraja Mohalla, Mysore", "8899001122", 4.4, "Delicious Chinese cuisine with a variety of options."},
                    {"Burger Bistro", "American", "Lunch", "Vani Vilasa Road, Mysore", "9900112233", 4.5, "Gourmet burgers with a variety of toppings."},
                    {"Sushi Stop", "Japanese", "Dinner", "Mysore Palace, Mysore", "1122334455", 4.6, "Fresh sushi and sashimi with a modern twist."},
                    {"Pizza House", "Italian", "Lunch", "Chamundi Hill, Mysore", "2233445566", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                    {"Spice Route", "Indian", "Dinner", "KR Circle, Mysore", "3344556677", 4.2, "A mix of traditional and modern Indian cuisine."},
                    {"Cafe Bliss", "Cafe", "Breakfast", "Devaraja Mohalla, Mysore", "4455667788", 4.3, "A cozy cafe for a hearty breakfast."},
                    {"Seafood Shack", "Seafood", "Lunch", "Vani Vilasa Road, Mysore", "5566778899", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Thiruvananthapuram", {{"Kerala Kitchen", "North Indian", "Lunch", "MG Road, Thiruvananthapuram", "7788990011", 4.6, "A blend of North Indian and Kerala flavors."},
                                {"Coconut Coast", "South Indian", "Breakfast", "Kovalam Beach, Thiruvananthapuram", "8899001122", 4.5, "A beachside spot for a refreshing South Indian breakfast."},
                                {"Pasta Paradise", "Italian", "Lunch", "Technopark, Thiruvananthapuram", "9900112233", 4.3, "Delicious pasta dishes with a variety of sauces."},
                                {"China Town", "Chinese", "Dinner", "Vazhuthacaud, Thiruvananthapuram", "1122334455", 4.4, "Quick and delicious Chinese food."},
                                {"Burger Bay", "American", "Lunch", "Pattom, Thiruvananthapuram", "2233445566", 4.5, "Gourmet burgers with a variety of toppings."},
                                {"Sushi Spot", "Japanese", "Dinner", "MG Road, Thiruvananthapuram", "3344556677", 4.6, "Fresh sushi and sashimi with a modern twist."},
                                {"Pizza House", "Italian", "Lunch", "Kovalam Beach, Thiruvananthapuram", "4455667788", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                                {"Spice Route", "Indian", "Dinner", "Technopark, Thiruvananthapuram", "5566778899", 4.2, "A mix of traditional and modern Indian cuisine."},
                                {"Cafe Bliss", "Cafe", "Breakfast", "Pattom, Thiruvananthapuram", "6677889900", 4.3, "A perfect spot for a morning coffee and breakfast."},
                                {"Seafood Shack", "Seafood", "Lunch", "Vazhuthacaud, Thiruvananthapuram", "7788990011", 4.4, "Fresh seafood with a coastal vibe."}}},
        {"Vishakhapatnam", {{"Vizag Veggies", "North Indian", "Dinner", "Beach Road, Vishakhapatnam", "9900112233", 4.4, "A popular spot for North Indian vegetarian cuisine."},
                            {"Seaside Spice", "South Indian", "Lunch", "RK Beach, Vishakhapatnam", "1011121314", 4.3, "South Indian food with a view of the sea."},
                            {"Pasta Point", "Italian", "Lunch", "Dwaraka Nagar, Vishakhapatnam", "1122334455", 4.2, "A cozy spot for Italian cuisine."},
                            {"Dragon's Den", "Chinese", "Dinner", "Gajuwaka, Vishakhapatnam", "2233445566", 4.5, "Delicious Chinese food with a local twist."},
                            {"Burger Bliss", "American", "Lunch", "Siripuram, Vishakhapatnam", "3344556677", 4.3, "Juicy burgers with a variety of toppings."},
                            {"Sushi Stop", "Japanese", "Dinner", "Beach Road, Vishakhapatnam", "4455667788", 4.6, "Fresh sushi and sashimi with a modern twist."},
                            {"Pizza House", "Italian", "Lunch", "RK Beach, Vishakhapatnam", "5566778899", 4.3, "Authentic Italian pizzas with fresh ingredients."},
                            {"Spice Route", "Indian", "Dinner", "Gajuwaka, Vishakhapatnam", "6677889900", 4.2, "A mix of traditional and modern Indian cuisine."},
                            {"Cafe Bliss", "Cafe", "Breakfast", "Dwaraka Nagar, Vishakhapatnam", "7788990011", 4.3, "A cozy cafe for a hearty breakfast."},
                            {"Seafood Shack", "Seafood", "Lunch", "Siripuram, Vishakhapatnam", "8899001122", 4.4, "Fresh seafood with a coastal vibe."}}}
    };
}


/*int food()
 {
    vector<string> cities = {"Bengaluru", "Mumbai", "Delhi", "Chennai", "Kolkata", "Hyderabad", "Pune", "Jaipur", "Ahmedabad", "Lucknow", "Coimbatore", "Bhopal",
    "Darjeeling", "Shimla", "Goa", "Agra", "Varanasi", "Udaipur", "Amritsar", "Mysore", "Thiruvananthapuram", "Vishakhapatnam"};
    vector<string> meals = {"Breakfast", "Lunch", "Dinner"};
    vector<string> cuisines = {"North Indian", "South Indian", "Italian", "Chinese", "Mexican"};


    map<string, vector<Restaurant>> restaurants = initializeRestaurants();


    vector<User1> users;


    cout << "Welcome to Food Recommendation!\n";

    // Sign Up Section
    createUser1(users);


    // Log In Section
    string username;
    while (!authenticateUser1(users, username))
        {
        cout << "Authentication failed. Please try again.\n";
    }

    User1* currentUser = nullptr;
    for (auto& user : users)
        {
        if (user.username == username)
         {
            currentUser = &user;
            break;
        }
    }


    cout << "Would you like to see all restaurants in the system? (y/n): ";
    char seeAllRestaurantsChoice;
    cin >> seeAllRestaurantsChoice;
    if (seeAllRestaurantsChoice == 'y' || seeAllRestaurantsChoice == 'Y')
        {
        displayAllRestaurants(restaurants);
    }


    cout << "Please select a city:\n";
    displayOptions(cities);

    int cityChoice;
    cin >> cityChoice;
    string selectedCity = cities[cityChoice - 1];

    cout << "Would you like to see all recommendations from " << selectedCity << "? (y/n): ";
    char seeAllChoice;
    cin >> seeAllChoice;
    if (seeAllChoice == 'y' || seeAllChoice == 'Y')
        {
        if (restaurants.find(selectedCity) != restaurants.end())
         {
            getRecommendations(restaurants[selectedCity], *currentUser);
        }
        else
        {
            cout << "No data available for the city " << selectedCity << "." << endl;
        }
    }


    cout << "Please select a meal type:\n";
    displayOptions(meals);

    int mealChoice;
    cin >> mealChoice;
    string selectedMeal = meals[mealChoice - 1];

    cout << "Restaurants in " << selectedCity << " available for " << selectedMeal << ":\n";
    getRecommendationsByMeal(restaurants[selectedCity], selectedMeal, *currentUser);


    cout << "Please select a cuisine type:\n";
    displayOptions(cuisines);


    int cuisineChoice;
    cin >> cuisineChoice;
    string selectedCuisine = cuisines[cuisineChoice - 1];


    cout << "Restaurants in " << selectedCity << " for " << selectedCuisine << " cuisine and " << selectedMeal << ":\n";
    getRecommendationsByCuisine(restaurants[selectedCity], selectedMeal, selectedCuisine, *currentUser);


    cout << "Would you like to leave feedback? (y/n): ";
    char feedbackChoice;
    cin >> feedbackChoice;
    if (feedbackChoice == 'y' || feedbackChoice == 'Y')
        {
        leaveFeedback(*currentUser);
    }

    cout << "Would you like to view your recommendation history? (y/n): ";
    char historyChoice;
    cin >> historyChoice;
    if (historyChoice == 'y' || historyChoice == 'Y')
        {
        if (currentUser)
        {
            viewHistory(*currentUser);
        }
    }


    cout << "Would you like to view your reviews? (y/n): ";
    char reviewChoice;
    cin >> reviewChoice;
    if (reviewChoice == 'y' || reviewChoice == 'Y')
        {
        if (currentUser)
        {
            viewUserReviews(*currentUser);
        }
    }


    cout << "Would you like to search for a restaurant by name? (y/n): ";
    char searchChoice;
    cin >> searchChoice;
    if (searchChoice == 'y' || searchChoice == 'Y')
        {
        searchRestaurantByName(restaurants);
    }

    cout << "Thank you for using the Food Recommendation Program, " << username << "!\n";

    return 0;

}*/

void mainMenuLoop(vector<User1>& users, map<string, vector<Restaurant>>& restaurants)
{
    User1* currentUser = nullptr;
    string username;

    if (users.empty())
    {
        // Sign Up Section
        createUser1(users);
    }

    // Log In Section
    while (!authenticateUser1(users, username))
    {
        cout << "Authentication failed. Please try again.\n";
    }

    for (auto& user : users)
    {
        if (user.username == username)
        {
            currentUser = &user;
            break;
        }
    }

    while (true)
    {
        cout << "Welcome to Food Recommendation, " << username << "!\n";
        cout << "Would you like to see all restaurants in the system? (y/n): ";
        char seeAllRestaurantsChoice;
        cin >> seeAllRestaurantsChoice;
        if (seeAllRestaurantsChoice == 'y' || seeAllRestaurantsChoice == 'Y')
        {
            displayAllRestaurants(restaurants);
        }

        cout << "Please select a city:\n";
        vector<string> cities = {"Bengaluru", "Mumbai", "Delhi", "Chennai", "Kolkata", "Hyderabad", "Pune", "Jaipur", "Ahmedabad", "Lucknow", "Coimbatore", "Bhopal",
            "Darjeeling", "Shimla", "Goa", "Agra", "Varanasi", "Udaipur", "Amritsar", "Mysore", "Thiruvananthapuram", "Vishakhapatnam"};
        displayOptions(cities);

        int cityChoice;
        cin >> cityChoice;
        if (cityChoice < 1 || cityChoice > cities.size())
        {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }
        string selectedCity = cities[cityChoice - 1];

        cout << "Would you like to see all recommendations from " << selectedCity << "? (y/n): ";
        char seeAllChoice;
        cin >> seeAllChoice;
        if (seeAllChoice == 'y' || seeAllChoice == 'Y')
        {
            if (restaurants.find(selectedCity) != restaurants.end())
            {
                getRecommendations(restaurants[selectedCity], *currentUser);
            }
            else
            {
                cout << "No data available for the city " << selectedCity << "." << endl;
            }
        }

        cout << "Please select a meal type:\n";
        vector<string> meals = {"Breakfast", "Lunch", "Dinner"};
        displayOptions(meals);

        int mealChoice;
        cin >> mealChoice;
        if (mealChoice < 1 || mealChoice > meals.size())
        {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }
        string selectedMeal = meals[mealChoice - 1];

        cout << "Restaurants in " << selectedCity << " available for " << selectedMeal << ":\n";
        getRecommendationsByMeal(restaurants[selectedCity], selectedMeal, *currentUser);

        cout << "Please select a cuisine type:\n";
        vector<string> cuisines = {"North Indian", "South Indian", "Italian", "Chinese", "Mexican"};
        displayOptions(cuisines);

        int cuisineChoice;
        cin >> cuisineChoice;
        if (cuisineChoice < 1 || cuisineChoice > cuisines.size())
        {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }
        string selectedCuisine = cuisines[cuisineChoice - 1];

        cout << "Restaurants in " << selectedCity << " for " << selectedCuisine << " cuisine and " << selectedMeal << ":\n";
        getRecommendationsByCuisine(restaurants[selectedCity], selectedMeal, selectedCuisine, *currentUser);

        cout << "Would you like to leave feedback? (y/n): ";
        char feedbackChoice;
        cin >> feedbackChoice;
        if (feedbackChoice == 'y' || feedbackChoice == 'Y')
        {
            leaveFeedback(*currentUser);
        }

        cout << "Would you like to view your recommendation history? (y/n): ";
        char historyChoice;
        cin >> historyChoice;
        if (historyChoice == 'y' || historyChoice == 'Y')
        {
            viewHistory(*currentUser);
        }

        cout << "Would you like to view your reviews? (y/n): ";
        char reviewChoice;
        cin >> reviewChoice;
        if (reviewChoice == 'y' || reviewChoice == 'Y')
        {
            viewUserReviews(*currentUser);
        }

        cout << "Would you like to search for a restaurant by name? (y/n): ";
        char searchChoice;
        cin >> searchChoice;
        if (searchChoice == 'y' || searchChoice == 'Y')
        {
            searchRestaurantByName(restaurants);
        }

        cout << "Would you like to perform another action? (y/n): ";
        char continueChoice;
        cin >> continueChoice;
        if (continueChoice != 'y' && continueChoice != 'Y')
        {
            break;
        }
    }

    cout << "Thank you for using the Food Recommendation Program, " << username << "!\n";
}

int food()
{
    vector<User1> users;
    map<string, vector<Restaurant>> restaurants = initializeRestaurants();

    mainMenuLoop(users, restaurants);

    return 0;
}
