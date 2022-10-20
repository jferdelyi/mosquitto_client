#ifndef SIMPLECLIENT_MQTT_H
#define SIMPLECLIENT_MQTT_H

#include <string>
#include <mosquittopp.h>

// @see https://mosquitto.org/api/files/mosquitto-h.html
class mqtt_client : public mosqpp::mosquittopp {
	protected:
		// Buffer (use on_message)
		char* m_buf;
		// Bufer size by default = 10
		size_t m_buf_size;
		
	public:
		/**
		* Constructor
		* @param p_id the client ID
		* @param p_host the hostname or ip address of the broker to connect to
		* @param p_port the network port to connect to. Usually 1883
		* @param p_keep_alive the number of seconds after which the broker should send a PING message to the client if no other messages have been exchanged in that time
		*/
		mqtt_client (const char* p_id, const char* p_host, const int p_port, const int p_keep_alive);
		
		/**
		* Destrucotr
		*/
		~mqtt_client();

		/**
		* Realloc the buffer
		* @param p_payload_size the size of the payload to be stored
		*/
		void realloc_buffer(const size_t p_payload_size);

		/**
		* Wrapper to send a messaeg
		* @param p_topic the topic name
		* @param p_message the message
		*/
		void send_message(const std::string& p_topic, const std::string& p_message = "");

		/**
		* On connect override
		* The values of the reason code are defined by the MQTT protocol version in use:
		*  - For MQTT v5.0, look at section 3.2.2.2 Connect Reason code https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html 
		*  - For MQTT v3.1.1, look at section 3.2.2.3 Connect Return code http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html
		* @parma p_rc the return code of the connection response
		*/
		void on_connect(int p_rc) override;

		/**
		* On connect with flags override
		* The values of the reason code are defined by the MQTT protocol version in use:
		*  - For MQTT v5.0, look at section 3.2.2.2 Connect Reason code https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html 
		*  - For MQTT v3.1.1, look at section 3.2.2.3 Connect Return code http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html
		* @parma p_rc the return code of the connection response
		* @parma p_flags the connect flags (see section Connect Flags https://docs.solace.com/API/MQTT-311-Prtl-Conformance-Spec/MQTT%20Control%20Packets.htm)
		*/
		void on_connect_with_flags(int p_rc, int p_flags) override;
	
		/**
		* On disconect override
		* The values of the reason code are defined by the MQTT protocol version in use:
		*  - For MQTT v5.0, look at section 3.2.2.2 Connect Reason code https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html 
		*  - For MQTT v3.1.1, look at section 3.2.2.3 Connect Return code http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html
		* @parma p_rc the return code of the connection response
		*/
		void on_disconnect(int p_rc) override;

		/**
		* On publish override
		* @parma p_mid the message id of the subscribe message
		*/
		void on_publish(int p_mid) override;

		/**
		* On message override
		* @parma p_message the message received
		*/
		void on_message(const struct mosquitto_message* p_message) override;

		/**
		* On subscribe override
		* @parma p_mid the message id of the subscribe message
		* @parma p_qos_count the number of granted subscriptions (size of granted_qos)
		* @parma p_granted_qos an array of integers indicating the granted QoS for each of the subscriptions
		*/
		void on_subscribe(int p_mid, int p_qos_count, const int* p_granted_qos) override;

		/**
		* On unsubscribe override
		* @parma p_mid the message id of the subscribe message
		*/
		virtual void on_unsubscribe(int p_mid) override;

		/**
		* On unsubscribe override
		* @parma p_level the message level (MOSQ_LOG_INFO, MOSQ_LOG_NOTICE, MOSQ_LOG_WARNING, MOSQ_LOG_ERR, MOSQ_LOG_DEBUG)
		* @parma p_str the log message
		*/
		virtual void on_log(int p_level, const char* p_str) override;

		/**
		* On error override
		*/
		virtual void on_error() override;
};

#endif //SIMPLECLIENT_MQTT_H
