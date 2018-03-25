/******************************************************

				Network transmission module

*****************************************************/

#include "Qwerty3D-PC.h"

//--------------Network Transimission module based on MQTT protocol-------------------

Qwerty::NetworkModule::NetworkModule()
{
}

bool Qwerty::NetworkModule::Init(std::string serverAddr, std::string clientId, std::string topic)
{
	mServerAddress = serverAddr;
	mClientId = clientId;
	mTopic = topic;
	m_pClient = new mqtt::async_client(serverAddr, clientId);
	m_pConnectionOpt = new mqtt::connect_options();

	//pass the reference of mqtt::async_client and mqtt::connectionOpt to init listener
	return mMqttListener.Init(mTopic,m_pClient,m_pConnectionOpt);
}

bool Qwerty::NetworkModule::Connect()
{
	m_pConnectionOpt->set_keep_alive_interval(20);
	m_pConnectionOpt->set_clean_session(true);
	m_pClient->set_callback(mMqttListener);

	// Start the connection.
	// When completed, the callback will subscribe to topic.
	try {
		m_pClient->connect(*m_pConnectionOpt, nullptr, mMqttListener);
	}
	catch (const mqtt::exception&) {
		//unable to connect to MQTT server
		return false;
	}

	return true;
}

void Qwerty::NetworkModule::Send(std::string s)
{
	m_pClient->publish(mTopic,&s.at(0), s.size());
}

bool Qwerty::NetworkModule::Disconnect()
{
	try {
		//Disconnecting
		m_pClient->disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		//failed to disconnect
		exc.what();
		return false;
	}

	return true;
}


//----------------------Listener for messages---------------------
Qwerty::Listener::Listener()
	: mConnectionRetryCount(0),
	m_pRefClient(nullptr),
	m_pRefConnectionOpt(nullptr),
	mTopicToSubscribe(""),
	mSubListener("Subscription")
{
}

bool Qwerty::Listener::Init(std::string topicToSubsribe, mqtt::async_client* pCli, mqtt::connect_options* pConnOpts)
{
	mTopicToSubscribe = topicToSubsribe;
	m_pRefClient = pCli;
	m_pRefConnectionOpt = pConnOpts;

	return true;
}

void Qwerty::Listener::mFunction_Reconnect()
{
	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	std::this_thread::sleep_for(std::chrono::milliseconds(2500));
	try {
		m_pRefClient->connect(*m_pRefConnectionOpt, nullptr, *this);
	}
	catch (const mqtt::exception& exc) {
		//reconnection error
		exc.what();
			return;
	}
}

void Qwerty::Listener::on_failure(const mqtt::token & tok)
{
	if (++mConnectionRetryCount > cRetryAttemptCount)
		exit(1);
	mFunction_Reconnect();
}

void Qwerty::Listener::on_success(const mqtt::token & tok)
{
	/*std::cout << "\nConnection success" << std::endl;
	std::cout << "\nSubscribing to topic '" << mTopic << "'\n"
		<< "\tfor client " << mClientId
		<< " using QoS" << cQOS << "\n"
		<< "\nPress Q<Enter> to quit\n" << std::endl;*/

	//connection success
	m_pRefClient->subscribe(mTopicToSubscribe, cQOS, nullptr, mSubListener);
}

void Qwerty::Listener::connection_lost(const std::string & cause)
{
	//if (!cause.empty())std::cout << "\tcause: " << cause << std::endl;

	//try to reconnect
	mConnectionRetryCount = 0;
	mFunction_Reconnect();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MESSAGE ARRIVAL!!!!!!!!!!!!!!!!!!!!!!
void Qwerty::Listener::message_arrived(mqtt::const_message_ptr msg)
{
	/*std::cout << "Message arrived" << std::endl;
	std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
	std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;*/
	std::string payload = msg->to_string();
	gMainApp.OnMessage(std::move(payload));
}

void Qwerty::Listener::delivery_complete(mqtt::delivery_token_ptr token)
{
}

//----------------------Sub Action Listener---------------------

void Qwerty::SubActionListener::on_failure(const mqtt::token & tok)
{
	/*std::cout << mName << " failure";
	if (tok.get_message_id() != 0)
	std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
	std::cout << std::endl;*/
}

void Qwerty::SubActionListener::on_success(const mqtt::token & tok) 
{
	/*std::cout << mName << " success";
	if (tok.get_message_id() != 0)
	std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
	auto top = tok.get_topics();
	if (top && !top->empty())
	std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
	std::cout << std::endl;*/


}
