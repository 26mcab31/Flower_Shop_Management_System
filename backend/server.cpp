
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../FlowerLinkedList.h"
#include "../OrderQueue.h"

using namespace std;

FlowerLinkedList flowerList;
OrderQueue orderQueue;

// Read a string or number from a simple JSON object.
string getJsonValue(const string& json, const string& key)
{
    string searchKey = "\"" + key + "\"";
    size_t pos = json.find(searchKey);

    if (pos == string::npos)
        return "";

    pos = json.find(':', pos + searchKey.size());

    if (pos == string::npos)
        return "";

    pos++;

    while (pos < json.size() &&
           isspace(static_cast<unsigned char>(json[pos])))
        pos++;

    if (pos >= json.size())
        return "";

    if (json[pos] == '"')
    {
        pos++;
        string value;

        while (pos < json.size())
        {
            if (json[pos] == '\\' && pos + 1 < json.size())
            {
                char next = json[pos + 1];

                if (next == 'n')
                    value += '\n';
                else if (next == 'r')
                    value += '\r';
                else
                    value += next;

                pos += 2;
            }
            else if (json[pos] == '"')
            {
                break;
            }
            else
            {
                value += json[pos++];
            }
        }

        return value;
    }

    size_t end = pos;

    while (end < json.size() &&
           json[end] != ',' &&
           json[end] != '}' &&
           !isspace(static_cast<unsigned char>(json[end])))
        end++;

    return json.substr(pos, end - pos);
}

string escapeJson(const string& value)
{
    string result;

    for (unsigned char c : value)
    {
        if (c == '"' || c == '\\')
        {
            result += '\\';
            result += static_cast<char>(c);
        }
        else if (c == '\n')
            result += "\\n";
        else if (c == '\r')
            result += "\\r";
        else if (c == '\t')
            result += "\\t";
        else if (c >= 32)
            result += static_cast<char>(c);
    }

    return result;
}

// ---------------- FLOWER FUNCTIONS ----------------

string flowerToJson(const Flower& flower)
{
    ostringstream out;

    out << "{\"flowerId\":" << flower.flowerId
        << ",\"flowerName\":\"" << escapeJson(flower.flowerName)
        << "\",\"flowerType\":\"" << escapeJson(flower.flowerType)
        << "\",\"price\":" << flower.price
        << ",\"quantity\":" << flower.quantity
        << ",\"availability\":"
        << (flower.availability ? "true" : "false")
        << "}";

    return out.str();
}

string getAllFlowersJson()
{
    vector<Flower> flowers = flowerList.getAllFlowers();
    ostringstream out;

    out << "{\"status\":\"success\",\"flowers\":[";

    for (size_t i = 0; i < flowers.size(); i++)
    {
        if (i > 0)
            out << ",";

        out << flowerToJson(flowers[i]);
    }

    out << "]}";
    return out.str();
}

// ---------------- ORDER FUNCTIONS ----------------

string orderToJson(const Order& order)
{
    ostringstream out;

    out << "{\"orderId\":" << order.orderId
        << ",\"customerName\":\"" << escapeJson(order.customerName)
        << "\",\"flowerName\":\"" << escapeJson(order.flowerName)
        << "\",\"quantity\":" << order.quantity
        << "}";

    return out.str();
}

string getAllOrdersJson()
{
    vector<Order> orders = orderQueue.getPendingOrders();
    ostringstream out;

    out << "{\"status\":\"success\",\"orders\":[";

    for (size_t i = 0; i < orders.size(); i++)
    {
        if (i > 0)
            out << ",";

        out << orderToJson(orders[i]);
    }

    out << "]}";
    return out.str();
}

// ---------------- API ROUTING ----------------

string processRequest(const string& method,
                      const string& path,
                      const string& body,
                      string& status)
{
    status = "200 OK";

    if (method == "OPTIONS")
        return "";

    if (method == "GET" && path == "/")
    {
        return "{\"status\":\"success\","
               "\"message\":\"Flower Shop Backend Connected!\"}";
    }

    // View all flowers.
    if (method == "GET" && path == "/flowers")
        return getAllFlowersJson();

    // Add a flower.
    if (method == "POST" && path == "/flowers")
    {
        string idText = getJsonValue(body, "flowerId");
        string name = getJsonValue(body, "flowerName");
        string type = getJsonValue(body, "flowerType");
        string priceText = getJsonValue(body, "price");
        string quantityText = getJsonValue(body, "quantity");

        if (idText.empty() || name.empty() || type.empty() ||
            priceText.empty() || quantityText.empty())
        {
            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Please provide all required fields\"}";
        }

        try
        {
            Flower flower;
            flower.flowerId = stoi(idText);
            flower.flowerName = name;
            flower.flowerType = type;
            flower.price = stod(priceText);
            flower.quantity = stoi(quantityText);
            flower.availability = flower.quantity > 0;

            if (flower.flowerId <= 0 || flower.price < 0 ||
                flower.quantity < 0 || flower.flowerName.empty() ||
                flower.flowerType.empty())
                throw 1;

            if (flowerList.addFlowerData(flower))
            {
                return "{\"status\":\"success\","
                       "\"message\":\"Flower added successfully\"}";
            }

            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Invalid data or duplicate flower ID\"}";
        }
        catch (...)
        {
            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Invalid flower data\"}";
        }
    }

    // View, update or delete one flower.
    if (path.rfind("/flowers/", 0) == 0)
    {
        int id;

        try
        {
            id = stoi(path.substr(9));
        }
        catch (...)
        {
            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Invalid flower ID\"}";
        }

        if (method == "GET")
        {
            Flower flower;

            if (flowerList.findFlowerById(id, flower))
            {
                return "{\"status\":\"success\",\"flower\":" +
                       flowerToJson(flower) + "}";
            }

            status = "404 Not Found";
            return "{\"status\":\"error\","
                   "\"message\":\"Flower not found\"}";
        }

        if (method == "PUT")
        {
            string priceText = getJsonValue(body, "price");
            string quantityText = getJsonValue(body, "quantity");

            try
            {
                if (priceText.empty() || quantityText.empty())
                    throw 1;

                double price = stod(priceText);
                int quantity = stoi(quantityText);

                if (price < 0 || quantity < 0)
                    throw 1;

                if (flowerList.updateFlowerData(id, price, quantity))
                {
                    return "{\"status\":\"success\","
                           "\"message\":\"Flower updated successfully\"}";
                }

                status = "404 Not Found";
                return "{\"status\":\"error\","
                       "\"message\":\"Flower not found or invalid data\"}";
            }
            catch (...)
            {
                status = "400 Bad Request";
                return "{\"status\":\"error\","
                       "\"message\":\"Invalid price or quantity\"}";
            }
        }

        if (method == "DELETE")
        {
            if (flowerList.deleteFlowerById(id))
            {
                return "{\"status\":\"success\","
                       "\"message\":\"Flower deleted successfully\"}";
            }

            status = "404 Not Found";
            return "{\"status\":\"error\","
                   "\"message\":\"Flower not found\"}";
        }
    }

    // View all pending orders.
    if (method == "GET" && path == "/orders")
        return getAllOrdersJson();

    // Place a new order.
    if (method == "POST" && path == "/orders")
    {
        string idText = getJsonValue(body, "orderId");
        string customerName = getJsonValue(body, "customerName");
        string flowerName = getJsonValue(body, "flowerName");
        string quantityText = getJsonValue(body, "quantity");

        if (idText.empty() || customerName.empty() ||
            flowerName.empty() || quantityText.empty())
        {
            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Please provide all order fields\"}";
        }

        try
        {
            Order order;
            order.orderId = stoi(idText);
            order.customerName = customerName;
            order.flowerName = flowerName;
            order.quantity = stoi(quantityText);

            if (orderQueue.addOrder(order))
            {
                return "{\"status\":\"success\","
                       "\"message\":\"Order placed successfully\","
                       "\"order\":" + orderToJson(order) + "}";
            }

            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Invalid order or duplicate pending order ID\"}";
        }
        catch (...)
        {
            status = "400 Bad Request";
            return "{\"status\":\"error\","
                   "\"message\":\"Invalid order data\"}";
        }
    }

    // Process the next order in FIFO order.
    if (method == "POST" && path == "/orders/process")
    {
        Order processedOrder;

        if (orderQueue.processNextOrder(processedOrder))
        {
            return "{\"status\":\"success\","
                   "\"message\":\"Order processed successfully\","
                   "\"order\":" + orderToJson(processedOrder) + "}";
        }

        status = "400 Bad Request";
        return "{\"status\":\"error\","
               "\"message\":\"There are no pending orders\"}";
    }

    status = "404 Not Found";
    return "{\"status\":\"error\",\"message\":\"Endpoint not found\"}";
}

// ---------------- SERVER STARTUP ----------------

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "Winsock initialization failed." << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (sockaddr*)&serverAddress,
             sizeof(serverAddress)) == SOCKET_ERROR)
    {
        cerr << "Bind failed. Stop the other server using port 8080 first."
             << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cerr << "Listen failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Flower Shop Backend Server" << endl;
    cout << "Listening at http://127.0.0.1:8080" << endl;
    cout << "GET /flowers          - View flowers" << endl;
    cout << "POST /flowers         - Add flower" << endl;
    cout << "GET /flowers/{id}     - Search flower" << endl;
    cout << "PUT /flowers/{id}     - Update flower" << endl;
    cout << "DELETE /flowers/{id}  - Delete flower" << endl;
    cout << "GET /orders           - View pending orders" << endl;
    cout << "POST /orders          - Place order" << endl;
    cout << "POST /orders/process  - Process next order" << endl;

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET)
            continue;

        string request;
        char buffer[4096];

        // Read HTTP headers.
        while (request.find("\r\n\r\n") == string::npos)
        {
            int received = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (received <= 0)
                break;

            request.append(buffer, received);

            if (request.size() > 1024 * 1024)
                break;
        }

        size_t headerEnd = request.find("\r\n\r\n");

        if (headerEnd != string::npos)
        {
            string headers = request.substr(0, headerEnd);
            string body = request.substr(headerEnd + 4);

            size_t firstSpace = headers.find(' ');
            size_t secondSpace = headers.find(' ', firstSpace + 1);

            string method = headers.substr(0, firstSpace);
            string path = headers.substr(
                firstSpace + 1,
                secondSpace - firstSpace - 1
            );

            if (path.find('?') != string::npos)
                path = path.substr(0, path.find('?'));

            // Read any remaining request body.
            size_t contentLength = 0;
            string lowerHeaders = headers;

            for (char& c : lowerHeaders)
                c = static_cast<char>(
                    tolower(static_cast<unsigned char>(c))
                );

            size_t clPos = lowerHeaders.find("content-length:");

            if (clPos != string::npos)
            {
                clPos += 15;

                while (clPos < lowerHeaders.size() &&
                       isspace(static_cast<unsigned char>(lowerHeaders[clPos])))
                    clPos++;

                contentLength = static_cast<size_t>(
                    strtoul(lowerHeaders.c_str() + clPos, nullptr, 10)
                );
            }

            while (body.size() < contentLength)
            {
                int received = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (received <= 0)
                    break;

                body.append(buffer, received);
            }

            string status;
            string responseBody = processRequest(method, path, body, status);

            cout << method << " " << path << " -> " << status << endl;

            string response =
                "HTTP/1.1 " + status + "\r\n"
                "Content-Type: application/json; charset=utf-8\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n"
                "Connection: close\r\n"
                "Content-Length: " + to_string(responseBody.size()) +
                "\r\n\r\n" + responseBody;

            size_t sent = 0;

            while (sent < response.size())
            {
                int result = send(
                    clientSocket,
                    response.c_str() + sent,
                    static_cast<int>(response.size() - sent),
                    0
                );

                if (result <= 0)
                    break;

                sent += result;
            }
        }

        shutdown(clientSocket, SD_SEND);
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
