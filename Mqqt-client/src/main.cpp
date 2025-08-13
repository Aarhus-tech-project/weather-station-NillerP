#include <iostream>
#include <string>
#include <mosquitto.h>
#include <csignal>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#define MQTT_ADDRESS "192.168.113.11"
#define MQTT_PORT 1883
#define CLIENTID "Client"
#define TOPIC "vejr/data"
#define QOS 1
const std::string DB_CONN_STR = "host=192.168.113.11 port=5432 dbname=vejrdata user=postgres password=Datait2025!";
volatile bool running = true;

void signal_handler(int)
{
    running = false;
}

void on_message(struct mosquitto *, void *, const struct mosquitto_message *msg)
{
    std::string payload(static_cast<char *>(msg->payload), msg->payloadlen);
    std::cout << "Received on topic " << msg->topic << ": " << payload << std::endl;

    try
    {
        auto j = nlohmann::json::parse(payload);
        std::cout << "Temperatur: " << j["temperatur"] << " °C" << std::endl;
        std::cout << "Luftfugtighed: " << j["luftfugtighed"] << " %" << std::endl;
        std::cout << "Tryk: " << j["tryk"] << " hPa" << std::endl;

        // --- Insert into PostgreSQL ---
        pqxx::connection c(DB_CONN_STR);
        pqxx::work txn(c);
        txn.exec_params(
            "INSERT INTO vejrdata (temperatur, luftfugtighed, tryk) VALUES ($1, $2, $3)",
            j["temperatur"].get<double>(),
            j["luftfugtighed"].get<double>(),
            j["tryk"].get<double>());
        txn.commit();
        std::cout << "Data inserted into database." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "JSON parse error or DB error: " << e.what() << std::endl;
    }
}

int main()
{
    std::signal(SIGINT, signal_handler);

    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(CLIENTID, true, nullptr);

    if (!mosq)
    {
        std::cerr << "Failed to create mosquitto client\n";
        return 1;
    }

    int rc = mosquitto_username_pw_set(mosq, "vejrbruger", "Datait2025!");
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Failed to set username/password: " << mosquitto_strerror(rc) << std::endl;
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    rc = mosquitto_connect(mosq, MQTT_ADDRESS, MQTT_PORT, 60);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Failed to connect: " << mosquitto_strerror(rc) << std::endl;
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }
    std::cout << "Connected to MQTT broker at " << MQTT_ADDRESS << std::endl;

    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_subscribe(mosq, nullptr, TOPIC, QOS);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Failed to subscribe: " << mosquitto_strerror(rc) << std::endl;
    }
    else
    {
        std::cout << "Subscribed to topic: " << TOPIC << std::endl;
    }

    while (running)
    {
        mosquitto_loop(mosq, -1, 1);
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}