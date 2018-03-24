/******************************************************

			mqtt based networking

*****************************************************/
#pragma once

namespace Qwerty
{
	//sub listener used by Qwerty::Listener
	class SubActionListener : public virtual mqtt::iaction_listener
	{
	public:
		SubActionListener(const std::string& name) : mName(name) {}

	private:
		std::string mName;

		void on_failure(const mqtt::token& tok) override;

		void on_success(const mqtt::token& tok) override;
	};




	//Listener for message/IO arrival
	//(CallBack functions)
	class Listener : public virtual mqtt::callback,
		public virtual mqtt::iaction_listener
	{
	public:
		Listener();

		bool Init(std::string topicToSubsribe, mqtt::async_client* pCli, mqtt::connect_options* pConnOpts);

	private:

		static const int	cQOS = 1;

		static const int	cRetryAttemptCount = 5;

		mqtt::async_client*	 m_pRefClient;		//reference to MQTT client

		mqtt::connect_options* m_pRefConnectionOpt; //reference to connection options

		SubActionListener	 mSubListener;		// An action listener to display the result of actions.

		int mConnectionRetryCount;		// Counter for the number of connection retries

		std::string mTopicToSubscribe;


		//reconnect if connection is lost
		void mFunction_Reconnect();

		// Re-connection failure
		void on_failure(const mqtt::token& tok) override;

		// Re-connection success
		void on_success(const mqtt::token& tok) override;

		// Callback for when the connection is lost.
		// This will initiate the attempt to manually reconnect.
		void connection_lost(const std::string& cause) override;

		// Callback for when a message arrives.
		void message_arrived(mqtt::const_message_ptr msg) override;

		void delivery_complete(mqtt::delivery_token_ptr token) override;

	};




	//Network Transimission module based on MQTT protocol
	class NetworkModule
	{
	public:

		NetworkModule();

		bool Init(std::string serverAddr, std::string clientId, std::string topic);

		bool Connect();

		void	Send(std::string s);

		bool Disconnect();

	private:

		Listener mMqttListener; //listener to messages/receiver (callback functions)

		mqtt::connect_options* m_pConnectionOpt;// Options to use if we need to reconnect

		mqtt::async_client* m_pClient;

		std::string mServerAddress;

		std::string mClientId;

		std::string mTopic;
	};


}