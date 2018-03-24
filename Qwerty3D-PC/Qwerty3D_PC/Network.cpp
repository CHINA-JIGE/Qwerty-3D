/******************************************************

				Network transmission module

*****************************************************/

#include "Qwerty3D-PC.h"

//--------------Network Transimission module based on MQTT protocol-------------------

Qwerty::NetworkModule::NetworkModule(std::string serverAddr, std::string clientId, std::string topic):
	mServerAddress(serverAddr),//"tcp://39.108.118.166:1883"
	mClientId(clientId),//"WindyIceXXX"
	mClient(serverAddr,clientId),
	mMqttListener(topic,mClient,mConnectionOpt)
{
}

bool Qwerty::NetworkModule::InitConnection()
{
	mConnectionOpt.set_keep_alive_interval(20);
	mConnectionOpt.set_clean_session(true);
	mClient.set_callback(mMqttListener);

	// Start the connection.
	// When completed, the callback will subscribe to topic.
	try {
		mClient.connect(mConnectionOpt, nullptr, mMqttListener);
	}
	catch (const mqtt::exception&) {
		//unable to connect to MQTT server
		return false;
	}

	return true;
}

bool Qwerty::NetworkModule::Disconnect()
{
	try {
		//Disconnecting
		mClient.disconnect()->wait();
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

Qwerty::Listener::Listener(std::string topicToSubsribe,mqtt::async_client& cli, mqtt::connect_options& connOpts)
	: mConnectionRetryCount(0), 
	mRefClient(cli), 
	mRefConnectionOpt(connOpts), 
	mSubListener("Subscription"),
	mTopicToSubscribe(topicToSubsribe)
{
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
		mRefClient.connect(mRefConnectionOpt, nullptr, *this);
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

	mRefClient.subscribe(mTopicToSubscribe, cQOS, nullptr, mSubListener);
}

void Qwerty::Listener::connection_lost(const std::string & cause)
{
	//if (!cause.empty())std::cout << "\tcause: " << cause << std::endl;

	//try to reconnect
	mConnectionRetryCount = 0;
	mFunction_Reconnect();
}

void Qwerty::Listener::message_arrived(mqtt::const_message_ptr msg)
{
	/*std::cout << "Message arrived" << std::endl;
	std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
	std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;*/
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

void Qwerty::SubActionListener::on_success(const mqtt::token & tok) {
	/*std::cout << mName << " success";
	if (tok.get_message_id() != 0)
	std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
	auto top = tok.get_topics();
	if (top && !top->empty())
	std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
	std::cout << std::endl;*/
}
