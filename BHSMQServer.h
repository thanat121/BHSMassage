#pragma once
// BHSMessageQueue.h
#pragma once

#include "BHSTelegramStructure.h"

using namespace System;
using namespace System::Messaging;
//.DLL
using namespace BHS::Common::EventLog;
using namespace BHS::Common::TelegramUty;

namespace BHS
{
	namespace MessageQueue
	{
		public delegate void MyRecvMQ(System::String^ sMsg);  //thanat
		public delegate void MyServerError(String^ sId, String^ sType); //thanat 
		
		//public __gc class BHSMQServer : public MessageQueue
		public ref class BHSMQServer : public MessageQueue
		{
			
			//Constructor
		public:
			BHSMQServer(void)
			{
				bHasEventHandler = false;
			}
			//EventHandler
		public:
			event MyRecvMQ^ RecvMQ;
			void add(String^ sMsg)
			{
				RecvMQ(sMsg);
			}
			event MyServerError^ MQServerError;
			void add(String^ sId, String^ sType)
			{
				MQServerError( sId, sType);
			}

			//void MQServerError(String^ sId, String^ sType) {
			//	(sId,sType);
		//	}
		//	__event void RecvMQ(String^ sMsg);
		//	__event void MQServerError(String^ sId, String^ sType);
			
			//Attribute
		public:
			static String^ m_sMQName = "";
			static String^ m_sFilePath = ".\\";
			//	static String * m_sMQName	= S"";
			//	static String * m_sFilePath = S".\\";

		private:
			bool bHasEventHandler;

			//Property
		public: static String^ get_FilePath()
			//public: __property static String * get_FilePath()
		{
			return m_sFilePath;
		}
		public: static void set_FilePath(String^ sFilePath)
			//public:	__property static void set_FilePath(String * sFilePath)
		{
			m_sFilePath = String::Copy(sFilePath);
		}
				//Implementation
		public:	int StartMQ(String^ sMQName)
			//public:	int StartMQ(String * sMQName)
		{


			//	if (sMQName == '\0' || sMQName->Length == 0)
			if (sMQName->CompareTo('\0') || sMQName->Length == 0)
				return -1;

			m_sMQName = String::Copy(sMQName);

			this->Path = String::Format(".\\Private$\\{0}", m_sMQName);
			//this->Path = String::Format(S".\\Private$\\{0}", m_sMQName);
			if (!MessageQueue::Exists(this->Path))
			{
				MessageQueue::Create(this->Path);
			}
			MessageQueue::Purge();

			//	this->Formatter = new ActiveXMessageFormatter();
			this->Formatter = gcnew ActiveXMessageFormatter();
			//Add EventHandler
			if (!bHasEventHandler)
			{
				this->ReceiveCompleted += gcnew ReceiveCompletedEventHandler(this, &BHSMQServer::OnReceiveCompleted);
				bHasEventHandler = true;
			}
			
			try
			{
				this->BeginReceive();
			}
			//catch (MessageQueueException* mqe)
			catch (MessageQueueException^ mqe)
			{	//Write EventLog
				//BHSEventLog::WriteEventLog( S"< FUNCTION BHSMQServer::StartMQ >"
				BHSEventLog::WriteEventLog("< FUNCTION BHSMQServer::StartMQ >"
					, mqe->Message
					, EEventType::kError);
				//Write LogFile
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< FUNCTION BHSMQServer::StartMQ >"
					, mqe->Message);
				
					MQServerError("0020", ERROR_TYPE);
				
				//BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
				//	, S"< FUNCTION BHSMQServer::StartMQ >"
				//	, mqe->Message);

				//MQServerError(S"0020", ERROR_TYPE);
				return -3;
			}

			return 0;
		}

			

				//private: void OnReceiveCompleted(Object* source, ReceiveCompletedEventArgs* asyncResult)
		private: void OnReceiveCompleted(Object^ source, ReceiveCompletedEventArgs^ asyncResult)
		{
			//try
			//{
			//System::Messaging::Message * pNewMsg = this->EndReceive(asyncResult->AsyncResult);
			//String * sMsg = dynamic_cast<String*>(pNewMsg->Body);

			//Write EventLog (TELEGRAM)
			//BHSEventLog::WriteEventLogTelegram(S"< MESSAGE QUEUE : RECEIVE >"
			//, sMsg
			//, EEventType::kInformation);
			//Write LogFile
			//BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
			//, S"< MESSAGE QUEUE : RECEIVE >"
			//, sMsg);

			//Check telegram length
			//if (sMsg->Length >= BHS::Telegram::Header::Size())
			//{
			//RecvMQ(sMsg);
			//}
			//else
			//	{	//Write EventLog
			//String * sTmp = String::Format(S"Telegram length error.\nTelegram length must be at least {0} chars but receive {1} char(s)."
			//, BHS::Telegram::Header::Size().ToString()
			//, sMsg->Length.ToString());
			//BHSEventLog::WriteEventLogTelegram(String::Concat(S"< MESSAGE QUEUE : RECEIVE > - TELEGRAM ERROR", "\n\n", sTmp)
			//, sMsg
			//	, EEventType::kError);
			//Write LogFile
			//	BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
			//, S"< MESSAGE QUEUE : RECEIVE > - TELEGRAM ERROR"
			//	, String::Format(S"{0} \nTELEGRAM : '{1}'", sTmp, sMsg));
			//}


			try
			{
				System::Messaging::Message ^ pNewMsg = this->EndReceive(asyncResult->AsyncResult);
				String ^ sMsg = dynamic_cast<String^>(pNewMsg->Body);

				//Write EventLog (TELEGRAM)
				BHSEventLog::WriteEventLogTelegram("< MESSAGE QUEUE : RECEIVE >"
					, sMsg
					, EEventType::kInformation);
				//Write LogFile
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : RECEIVE >"
					, sMsg);

				//Check telegram length
				if (sMsg->Length >= BHS::Telegram::Header::Size())
				{
					RecvMQ(sMsg);
					
				}
				else
				{	//Write EventLog
					String ^ sTmp = String::Format("Telegram length error.\nTelegram length must be at least {0} chars but receive {1} char(s)."
						, BHS::Telegram::Header::Size().ToString()
						, sMsg->Length.ToString());
					BHSEventLog::WriteEventLogTelegram(String::Concat("< MESSAGE QUEUE : RECEIVE > - TELEGRAM ERROR", "\n\n", sTmp)
						, sMsg
						, EEventType::kError);
					//Write LogFile
					BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
						, "< MESSAGE QUEUE : RECEIVE > - TELEGRAM ERROR"
						, String::Format("{0} \nTELEGRAM : '{1}'", sTmp, sMsg));
				}

			}
			catch (MessageQueueException^ mqe)
			{	//Write EventLog
				BHSEventLog::WriteEventLog("< MESSAGE QUEUE : RECEIVE > - MESSAGE QUEUE EXCEPTION"
					, mqe->Message
					, EEventType::kError);
				//Write LogFile
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : RECEIVE > - MESSAGE QUEUE EXCEPTION"
					, mqe->Message);

				 ("0021", ERROR_TYPE);
			}
			catch (Exception ^ e)
			{	//Write EventLog
				BHSEventLog::WriteEventLog("< MESSAGE QUEUE : RECEIVE > - EXCEPTION"
					, e->Message
					, EEventType::kError);
				//Write LogFile
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : RECEIVE > - EXCEPTION"
					, e->Message);

				MQServerError("0022", ERROR_TYPE);


				//	catch (MessageQueueException* mqe)
				//	{	//Write EventLog
				//	BHSEventLog::WriteEventLog(S"< MESSAGE QUEUE : RECEIVE > - MESSAGE QUEUE EXCEPTION"
				//		, mqe->Message
				//		, EEventType::kError);
				//Write LogFile
				//	BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
				//		, S"< MESSAGE QUEUE : RECEIVE > - MESSAGE QUEUE EXCEPTION"
				//		, mqe->Message);

				//	MQServerError(S"0021", ERROR_TYPE);
				//	}
				//	catch (Exception * e)
				//	{	//Write EventLog
				//	BHSEventLog::WriteEventLog(S"< MESSAGE QUEUE : RECEIVE > - EXCEPTION"
				//			, e->Message
				//		, EEventType::kError);
				//Write LogFile
				//	BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
				//		, S"< MESSAGE QUEUE : RECEIVE > - EXCEPTION"
				//		, e->Message);

				//	MQServerError(S"0022", ERROR_TYPE);
			}

			// Restart Asynchronous Receive Operation.
			this->BeginReceive();
		}
		};
	}
}