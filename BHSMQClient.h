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
		public delegate void MQClientErrorHandler(String^ sId , String^ sType);  //thanat
		
		//public __gc class BHSMQClient : public MessageQueue
		public ref class BHSMQClient : public MessageQueue
		{

			//Constructor
		public:
			BHSMQClient(void) { }

			
			//EventHandler
		public:
			event MQClientErrorHandler^ MQClientError;
			//__event void MQClientError(String* sId, String* sType);
			//Attribute
		public:

			static String^ m_sFilePath = ".\\";
			//static String * m_sFilePath = S".\\";

			//Property
			//public: __property static String * get_FilePath()
		public:  static String^ get_FilePath()
		{
			return m_sFilePath;
		}
		public: static void set_FilePath(String^ sFilePath)
			//public:	__property static void set_FilePath(String * sFilePath)
		{
			m_sFilePath = String::Copy(sFilePath);
		}
				//Implementation


		public:	void SendMQMessage(String^ sMQName, String^ sMsg)
			//public:	void SendMQMessage(String * sMQName, String * sMsg)
		{
			try
			{
				//Write EventLog (TELEGRAM)

				BHSEventLog::WriteEventLogTelegram("< MESSAGE QUEUE : SEND >", sMsg, EEventType::kInformation);
				//	BHSEventLog::WriteEventLogTelegram(S"< MESSAGE QUEUE : SEND >", sMsg, EEventType::kInformation);

				//Write LogFile
				//	BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
				//	, S"< MESSAGE QUEUE : SEND >"
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : SEND >"
					, sMsg);

				//Check telegram length
				if (sMsg->Length >= BHS::Telegram::Header::Size())
				{
					//this->Path = String::Format(S".\\Private$\\{0}", sMQName);
					//System::Messaging::Message	* pNewMsg;
					//pNewMsg = new System::Messaging::Message(sMsg, new ActiveXMessageFormatter());
					//this->Send(pNewMsg);
					this->Path = String::Format(".\\Private$\\{0}", sMQName);
					System::Messaging::Message^ pNewMsg;
					pNewMsg = gcnew System::Messaging::Message(sMsg, gcnew ActiveXMessageFormatter());
					//pNewMsg = new System::Messaging::Message(sMsg, new ActiveXMessageFormatter());
					this->Send(pNewMsg);
				}
				else
				{	//Write EventLog
					//	String * sTmp = String::Format(S"Telegram length error.\nTelegram length must be at least {0} chars but receive {1} char(s)."
					String^ sTmp = String::Format("Telegram length error.\nTelegram length must be at least {0} chars but receive {1} char(s)."
						, BHS::Telegram::Header::Size().ToString()
						, sMsg->Length.ToString());
					//	BHSEventLog::WriteEventLogTelegram(String::Concat(S"< MESSAGE QUEUE : SEND > - TELEGRAM ERROR", "\n\n", sTmp)
					BHSEventLog::WriteEventLogTelegram(String::Concat("< MESSAGE QUEUE : SEND > - TELEGRAM ERROR", "\n\n", sTmp)
						, sMsg
						, EEventType::kError);
					//Write LogFile
					//BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
					//, S"< MESSAGE QUEUE : SEND > - TELEGRAM ERROR"
					//	, String::Format(S"{0} \nTELEGRAM : '{1}'", sTmp, sMsg));
					BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
						, "< MESSAGE QUEUE : SEND > - TELEGRAM ERROR"
						, String::Format("{0} \nTELEGRAM : '{1}'", sTmp, sMsg));
				}
			}
			//catch (MessageQueueException *mqe)
			catch (MessageQueueException ^mqe)
			{	//Write EventLog
				BHSEventLog::WriteEventLog("< MESSAGE QUEUE : SEND > - MESSAGE QUEUE EXCEPTION"
					//BHSEventLog::WriteEventLog(S"< MESSAGE QUEUE : SEND > - MESSAGE QUEUE EXCEPTION"
					, mqe->Message
					, EEventType::kError);

				//Write LogFile
				//BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
				//	, S"< MESSAGE QUEUE : SEND > - MESSAGE QUEUE EXCEPTION"
				//	, mqe->Message);

				//MQClientError(S"0030", ERROR_TYPE);
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : SEND > - MESSAGE QUEUE EXCEPTION"
					, mqe->Message);
  
			//	void ShowMessage(String^ sId, String^ sTypeg)
				//{
					MQClientError("0030", ERROR_TYPE);
			//	}



				//MQClientError("0030", ERROR_TYPE);

			}
			//	catch (Exception *e)
			//	{	//Write EventLog
			//		BHSEventLog::WriteEventLog(S"< MESSAGE QUEUE : SEND > - EXCEPTION"
			//			, e->Message
			//		, EEventType::kError);
			//Write LogFile
			//	BHSTelegramUty::WriteLogFile(m_sFilePath, S"Log"
			//		, S"< MESSAGE QUEUE : SEND > - EXCEPTION"
			//		, e->Message);

			//	MQClientError(S"0031", ERROR_TYPE);
			//	}

			catch (Exception ^e)
			{	//Write EventLog
				BHSEventLog::WriteEventLog("< MESSAGE QUEUE : SEND > - EXCEPTION"
					, e->Message
					, EEventType::kError);
				//Write LogFile
				BHSTelegramUty::WriteLogFile(m_sFilePath, "Log"
					, "< MESSAGE QUEUE : SEND > - EXCEPTION"
					, e->Message);

				MQClientError("0031", ERROR_TYPE);
			}
		}
		};
	}



}