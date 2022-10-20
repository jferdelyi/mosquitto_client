#include <csignal>
#include <string>
#include <vector>

#include "mqtt.h"

static bool s_running = true;
// Uncomment it for verbose log
//#define VERBOSE

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int) {
	s_running = false;
}

int main() {
	signal(SIGINT, signal_callback_handler);

	std::vector<std::string> l_topics = {"PUB0", "PUB1", "PUB2", "PUB3", "PUB4", "PUB5", "PUB6", "PUB7", "PUB8", "PUB9"};
	bool l_subcribed = false;
	int l_rc;

	mosqpp::lib_init();
	mqtt_client *l_iot_client = new mqtt_client("CLIENT", "localhost", 1883, 60);
	while (s_running) {
		l_rc = l_iot_client->loop();
		if (l_rc) {
			l_subcribed = false;
			l_iot_client->reconnect();
		} 
		
		if (!l_subcribed) {
			for (const auto& l_topic : l_topics) {
				// About message ID
				// If not NULL, the function will set this to the message id of this particular message.  
				// This can be then used with the publish callback to determine when the message has been sent.  
				// Note that although the MQTT protocol doesn’t use message ids for messages with QoS=0, libmosquitto assigns them message ids so they can be tracked with this parameter.
				l_iot_client->subscribe(NULL, l_topic.c_str());
				l_iot_client->send_message(l_topic, "LISTENING");
			}
			l_subcribed = true;
		} else {
			for (const auto& l_topic : l_topics) {
				l_iot_client->send_message(l_topic);
				l_iot_client->loop_read();
			}
		}
	}

	delete l_iot_client;
	l_iot_client = nullptr;
	mosqpp::lib_cleanup();

	return EXIT_SUCCESS;
}

