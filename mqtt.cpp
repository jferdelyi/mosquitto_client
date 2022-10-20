#include "mqtt.h"

#include <csignal>  // raise
#include <iostream> // std::cout etc

mqtt_client::mqtt_client(const char* p_id, const char* p_host, const int p_port, const int p_keep_alive) : mosquittopp(p_id) {
	connect(p_host, p_port, p_keep_alive);
	m_buf_size = 10;
	m_buf = new char[m_buf_size];
}

mqtt_client::~mqtt_client() {
	delete[] m_buf;
}

void mqtt_client::realloc_buffer(const size_t p_payload_size) {
	// Realloc the buffer to payload size + 10
	delete[] m_buf;
	m_buf_size = p_payload_size + 10;
	m_buf = new char[m_buf_size];
}

void mqtt_client::send_message(const std::string& p_topic, const std::string& p_message) /*override*/ {
	// Here we assume that we are using QoS 0 see section Quality of Service of https://mosquitto.org/man/mqtt-7.html
	if (p_message.empty()) {
		// About message ID
		// If not NULL, the function will set this to the message id of this particular message.  
		// This can be then used with the publish callback to determine when the message has been sent.  
		// Note that although the MQTT protocol doesn�t use message ids for messages with QoS=0, libmosquitto assigns them message ids so they can be tracked with this parameter.
		// 
		// About retain message
		// A retained message is a normal MQTT message with the retained flag set to true.
		// The broker stores the last retained messageand the corresponding QoS for that topic.
		// Each client that subscribes to a topic pattern that matches the topic of the retained message receives the retained message immediately after they subscribe.The broker stores only one retained message per topic.
		publish(NULL, p_topic.c_str());
	} else {
		publish(NULL, p_topic.c_str(), static_cast<int>(p_message.length()), &p_message[0]);
	}
}

void mqtt_client::on_connect(int p_rc) /*override*/ {
	std::cout << "[Connect] - reason code " << p_rc << std::endl;
}

void mqtt_client::on_connect_with_flags(int p_rc, int p_flags) /*override*/ {
	std::cout << "[Connect] - reason code " << p_rc << " - flags " << p_flags << std::endl;
}

void mqtt_client::on_disconnect(int p_rc) /*override*/ {
	std::cout << "[Disconnect] - reason code " << p_rc << std::endl;
}

void mqtt_client::on_publish(int p_mid) /*override*/ {
#if VERBOSE 
	std::cout << "[Publish] - message ID " << p_mid << std::endl;
#endif
}

void mqtt_client::on_message(const struct mosquitto_message* p_message) /*override*/ {
	// If the message is not empty
	if (p_message->payloadlen != 0) {
		const size_t l_payload_size = static_cast<size_t>(p_message->payloadlen) + 1;

		// Check if the buffer can contains the payload
		if (l_payload_size > m_buf_size) {
			realloc_buffer(l_payload_size);
		}

		// Clean and copy data to the buffer
		memset(m_buf, 0, l_payload_size * sizeof(char));
		memcpy(m_buf, p_message->payload, l_payload_size * sizeof(char));

		// Print buf (debug)
		const std::string l_string = std::string(m_buf);
		std::cout << "[Message] - " << l_string << std::endl;

		// Check if the message is exit
		if (l_string == "EXIT") {
			raise(SIGINT);
		}
	}

	// If the topic is exit
	if (strcmp(p_message->topic, "EXIT") == 0) {
		raise(SIGINT);
	}
}

void mqtt_client::on_subscribe(int p_mid, int p_qos_count, const int* p_granted_qos) /*override*/ {
	std::cout << "[Subscribe] - message ID " << p_mid  << " - QoS count " << p_qos_count << " - granted QoS " << p_granted_qos << std::endl;
}

void mqtt_client::on_unsubscribe(int p_mid) /*override*/ {
	std::cout << "[Unsubscribe] - message ID " << p_mid  << std::endl;
}

void mqtt_client::on_log(int p_level, const char* p_str) /*override*/ {
#if VERBOSE 
	std::cout << "[Log] - message level " << p_level << " - message " << p_str << std::endl;
#endif
}

void mqtt_client::on_error() /*override*/ {
	std::cout << "[Error]" << std::endl;
}

