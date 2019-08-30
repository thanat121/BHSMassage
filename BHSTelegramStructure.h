#pragma once
#pragma once

using namespace System;
using namespace System::Text;

/**********************************************************************/
/** HEADER **/
#define FROM_SIZE			8
#define TO_SIZE				8
#define TIMESTAMP_SIZE		14 // YYYYMMDDHHMISS
#define TELEGRAMNO_SIZE		6
#define LENGTH_SIZE			4

/** SUBHEADER **/
#define TELEGRAMID_SIZE		4
#define RESPCODE_SIZE		1
#define RESULTCODE_SIZE		3
#define DETAILCODE_SIZE		4
#define RESERVE_SIZE		28

/** ALARM/EVENT TYPE **/
#define INFO_TYPE		"INFO"
#define WARNING_TYPE	"WARNING"
#define ERROR_TYPE		"ERROR"

/**********************************************************************/
namespace BHS
{
	namespace Telegram
	{

		ref class Header
			//class Header
			//__gc class Header
		{
		public: //Variable
				//Telegram Header


			System::String^ sFrom;
			System::String^ sTo;
			System::String^ sTimestamp;
			System::String^ sTelegramNo;
			System::String^ sLength;
			//System::String* sFrom;
			//System::String* sTo;
			//System::String* sTimestamp;
			//System::String* sTelegramNo;
			//System::String* sLength;

			//Telegram Sub-Header
			System::String^ sTelegramID;
			System::String^ sRespCode;
			System::String^ sResultCode;
			System::String^ sDetailCode;
			System::String^ sReserve;

		public: //Static
			static System::Int32 SizeOfHeaderPart()
			{
				return (FROM_SIZE + TO_SIZE + TIMESTAMP_SIZE + TELEGRAMNO_SIZE + LENGTH_SIZE);
			}
			static System::Int32 SizeOfSubHeaderPart()
			{
				return (TELEGRAMID_SIZE + RESPCODE_SIZE + RESULTCODE_SIZE + DETAILCODE_SIZE + RESERVE_SIZE);
			}
			static System::Int32 Size()
			{
				//Total Telegram Header Size : Header Size + Sub-Header Size
				return	(SizeOfHeaderPart() + SizeOfSubHeaderPart());
				//	return	(BHS::Telegram::Header::SizeOfHeaderPart() + BHS::Telegram::Header::SizeOfSubHeaderPart());			
			}

		public: //Method
			Header()	//Constructor
			{
				//Header
				this->sFrom = "";
				this->sTo = "";
				this->sTimestamp = "";
				this->sTelegramNo = "";
				this->sLength = "";
				//Sub-Header
				this->sTelegramID = "";
				this->sRespCode = "";
				this->sResultCode = "";
				this->sDetailCode = "";
				this->sReserve = "";
			}

			System::Void Parse(String^ msg)
			{
				//Parse each item - index = 0
				this->Parse(msg, 0);
			}
			System::Void Parse(String^ msg, int index)
			{
				//Parse each item
				//index: beginning position of data

				//--1. sFrom 
				this->sFrom = msg->Substring(index, FROM_SIZE);
				index += FROM_SIZE;

				//--2. sTo
				this->sTo = msg->Substring(index, TO_SIZE);
				index += TO_SIZE;

				//--3. sTimestamp
				this->sTimestamp = msg->Substring(index, TIMESTAMP_SIZE);
				index += TIMESTAMP_SIZE;

				//--4. sTelegramNo
				this->sTelegramNo = msg->Substring(index, TELEGRAMNO_SIZE);
				index += TELEGRAMNO_SIZE;

				//--5. sLength
				this->sLength = msg->Substring(index, LENGTH_SIZE);
				index += LENGTH_SIZE;

				//--6. sTelegramID
				this->sTelegramID = msg->Substring(index, TELEGRAMID_SIZE);
				index += TELEGRAMID_SIZE;

				//--7. sRespCode
				this->sRespCode = msg->Substring(index, RESPCODE_SIZE);
				index += RESPCODE_SIZE;

				//--8. sResultCode
				this->sResultCode = msg->Substring(index, RESULTCODE_SIZE);
				index += RESULTCODE_SIZE;

				//--9. sDetailCode
				this->sDetailCode = msg->Substring(index, DETAILCODE_SIZE);
				index += DETAILCODE_SIZE;

				//-10. sReserve
				this->sReserve = msg->Substring(index, RESERVE_SIZE);
				index += RESERVE_SIZE;
			}

			System::Void ParseHeaderPart(String^ msg)
			{
				this->ParseHeaderPart(msg, 0);
			}

			System::Void ParseHeaderPart(String^ msg, int index)
			{
				//Parse each item
				//index: beginning position of data

				//--1. sFrom 
				this->sFrom = msg->Substring(index, FROM_SIZE);
				index += FROM_SIZE;

				//--2. sTo
				this->sTo = msg->Substring(index, TO_SIZE);
				index += TO_SIZE;

				//--3. sTimestamp
				this->sTimestamp = msg->Substring(index, TIMESTAMP_SIZE);
				index += TIMESTAMP_SIZE;

				//--4. sTelegramNo
				this->sTelegramNo = msg->Substring(index, TELEGRAMNO_SIZE);
				index += TELEGRAMNO_SIZE;

				//--5. sLength
				this->sLength = msg->Substring(index, LENGTH_SIZE);
				index += LENGTH_SIZE;
			}

			System::Boolean IsSendTo(String^ destination)
			{
				//if(String::Compare(sTo,0,destination,0,TO_SIZE)==0)
				if (String::Equals(sTo->Trim(), destination->Trim()))
					return true;
				else
					return false;
			}

			System::Boolean CheckTelegramID(String^ id)
			{
				if (String::Equals(sTelegramID->Trim(), id->Trim()))
					return true;
				else
					return false;
			}

			System::Boolean CheckTelegramNo(String^ sRefTelegramNo)
			{
				if (String::Compare(sTelegramNo, 0, sRefTelegramNo, 0, TELEGRAMNO_SIZE) == 0)
					return true;
				else
					return false;
			}

			//System::String* ToString()
			System::String^ ToString(String^)
			{

				//Generate output telegram
				//Use Substring to limit size of each element.
				//StringBuilder* sbd = new StringBuilder();
				StringBuilder^ sbd = gcnew StringBuilder();
				sbd->Append(this->sFrom->PadRight(FROM_SIZE, L' ')->Substring(0, FROM_SIZE));						//--1 sFrom
				sbd->Append(this->sTo->PadRight(TO_SIZE, L' ')->Substring(0, TO_SIZE));							//--2 sTo
				sbd->Append(this->sTimestamp->PadRight(TIMESTAMP_SIZE, L' ')->Substring(0, TIMESTAMP_SIZE));		//--3 sTimestamp
				sbd->Append(this->sTelegramNo->PadRight(TELEGRAMNO_SIZE, L' ')->Substring(0, TELEGRAMNO_SIZE));	//--4 sTelegramNo
				sbd->Append(this->sLength->PadRight(FROM_SIZE, L' ')->Substring(0, LENGTH_SIZE));					//--5 sLength

				sbd->Append(this->sTelegramID->PadRight(TELEGRAMID_SIZE, L' ')->Substring(0, TELEGRAMID_SIZE));	//--6 sTelegramID
				sbd->Append(this->sRespCode->PadRight(RESPCODE_SIZE, L' ')->Substring(0, RESPCODE_SIZE));			//--7 sRespCode
				sbd->Append(this->sResultCode->PadRight(RESULTCODE_SIZE, L' ')->Substring(0, RESULTCODE_SIZE));	//--8 sResultCode
				sbd->Append(this->sDetailCode->PadRight(DETAILCODE_SIZE, L' ')->Substring(0, DETAILCODE_SIZE));	//--9 sDetailCode
				sbd->Append(this->sReserve->PadRight(RESERVE_SIZE, L' ')->Substring(0, RESERVE_SIZE));			//-10 sReserve

				return sbd->ToString();
			}
		};
	}
}

/**********************************************************************/
